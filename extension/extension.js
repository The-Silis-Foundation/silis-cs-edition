const vscode = require('vscode');
const path = require('path');
const fs = require('fs');
const net = require('net');
const cp = require('child_process');

function escapeHtml(str) {
    if (!str) return '';
    return String(str)
        .replace(/&/g, '&amp;')
        .replace(/</g, '&lt;')
        .replace(/>/g, '&gt;')
        .replace(/"/g, '&quot;')
        .replace(/'/g, '&#39;');
}

function safePostMessage(webviewOrPanel, msg) {
    try {
        if (!webviewOrPanel) return;
        const webview = webviewOrPanel.webview || webviewOrPanel;
        webview.postMessage(msg);
    } catch (e) {
        // Target webview was disposed before message could be delivered
    }
}

function atomicWriteFileSync(filePath, content) {
    const tmpPath = `${filePath}.tmp.${Date.now()}`;
    fs.writeFileSync(tmpPath, content);
    fs.renameSync(tmpPath, filePath);
}

// -----------------------------------------------------------------------------
// Native In-Memory Web EDA Engines (Zero Ports, Zero Loopback Overhead)
// -----------------------------------------------------------------------------

function getVcdValuesAtTimestamp(vcdText, timePs) {
    if (!vcdText || typeof vcdText !== 'string') return [];
    try {
        const lines = vcdText.split(/\r?\n/);
        let timescaleMult = 1.0;
        const varMap = {};
        const scope = [];
        let i = 0;

        for (; i < lines.length; i++) {
            const line = lines[i].trim();
            if (!line) continue;
            if (line.startsWith('$timescale')) {
                let tsStr = line;
                while (!tsStr.includes('$end') && i + 1 < lines.length) {
                    i++;
                    tsStr += ' ' + lines[i].trim();
                }
                const m = tsStr.match(/(\d+)\s*([a-zA-Z]+)/);
                if (m) {
                    const num = parseInt(m[1], 10);
                    const unit = m[2].toLowerCase();
                    const scaleMap = { 's': 1e12, 'ms': 1e9, 'us': 1e6, 'ns': 1e3, 'ps': 1, 'fs': 0.001 };
                    timescaleMult = num * (scaleMap[unit] || 1);
                }
            } else if (line.startsWith('$scope')) {
                const parts = line.split(/\s+/);
                if (parts.length >= 3) scope.push(parts[2]);
            } else if (line.startsWith('$upscope')) {
                if (scope.length > 0) scope.pop();
            } else if (line.startsWith('$var')) {
                const parts = line.split(/\s+/);
                if (parts.length >= 5) {
                    const sym = parts[3];
                    const name = parts[4];
                    const full = scope.concat(name).join('.');
                    if (!varMap[sym]) varMap[sym] = [];
                    varMap[sym].push({ full, name });
                }
            } else if (line.startsWith('$enddefinitions')) {
                i++;
                break;
            }
        }

        const targetTick = timescaleMult > 0 ? (timePs / timescaleMult) : timePs;
        const currentVals = {};
        let curTick = 0;

        for (; i < lines.length; i++) {
            const line = lines[i].trim();
            if (!line) continue;
            if (line[0] === '#') {
                curTick = parseInt(line.slice(1), 10);
                if (curTick > targetTick) break;
            } else if (line[0] === 'b' || line[0] === 'B' || line[0] === 'r' || line[0] === 'R') {
                const parts = line.split(/\s+/);
                if (parts.length >= 2) {
                    currentVals[parts[1]] = parts[0].slice(1);
                }
            } else if ('01xXzZ'.includes(line[0]) && line.length >= 2) {
                currentVals[line.slice(1)] = line[0];
            }
        }

        const updates = [];
        for (const [sym, val] of Object.entries(currentVals)) {
            const aliases = varMap[sym];
            if (aliases) {
                for (const a of aliases) {
                    updates.push({ name: a.full, values: [val] });
                    updates.push({ name: a.name, values: [val] });
                }
            }
        }
        return updates;
    } catch (e) {
        return [];
    }
}

function parseDefContent(content) {
    if (!content || typeof content !== 'string') return null;
    const lines = content.split(/\r?\n/);
    let units = 1000;
    let die = { w: 1000, h: 1000 };
    let core = { border: 20, w: 960, h: 960 };
    const components = [];
    const pins = [];

    let inComponents = false;
    let inPins = false;

    for (let i = 0; i < lines.length; i++) {
        const line = lines[i].trim();
        if (!line || line.startsWith('#')) continue;

        if (line.startsWith('UNITS DISTANCE MICRONS')) {
            const m = line.match(/UNITS\s+DISTANCE\s+MICRONS\s+(\d+)/);
            if (m) units = parseInt(m[1], 10) || 1000;
        } else if (line.startsWith('DIEAREA')) {
            const coords = [];
            const re = /\(\s*(-?\d+)\s+(-?\d+)\s*\)/g;
            let match;
            while ((match = re.exec(line)) !== null) {
                coords.push([parseInt(match[1], 10), parseInt(match[2], 10)]);
            }
            if (coords.length >= 2) {
                const xs = coords.map(c => c[0]);
                const ys = coords.map(c => c[1]);
                const minX = Math.min(...xs);
                const maxX = Math.max(...xs);
                const minY = Math.min(...ys);
                const maxY = Math.max(...ys);
                die = {
                    x: minX / units,
                    y: minY / units,
                    w: (maxX - minX) / units,
                    h: (maxY - minY) / units
                };
                core = {
                    border: 20,
                    w: Math.max(10, die.w - 40),
                    h: Math.max(10, die.h - 40)
                };
            }
        } else if (line.startsWith('COMPONENTS')) {
            inComponents = true;
        } else if (line.startsWith('END COMPONENTS')) {
            inComponents = false;
        } else if (line.startsWith('PINS')) {
            inPins = true;
        } else if (line.startsWith('END PINS')) {
            inPins = false;
        } else if (inComponents && line.startsWith('-')) {
            const parts = line.split(/\s+/);
            if (parts.length >= 3) {
                const inst = parts[1];
                const master = parts[2];
                let x = 0, y = 0, orient = 'R0';
                const placedIdx = parts.indexOf('PLACED') !== -1 ? parts.indexOf('PLACED') : parts.indexOf('FIXED');
                if (placedIdx !== -1 && placedIdx + 4 < parts.length) {
                    const rx = parseInt(parts[placedIdx + 2], 10);
                    const ry = parseInt(parts[placedIdx + 3], 10);
                    if (!isNaN(rx)) x = rx / units;
                    if (!isNaN(ry)) y = ry / units;
                    orient = parts[placedIdx + 5] || 'R0';
                }
                components.push({ inst, master, x, y, orient, w: 200, h: 150 });
            }
        } else if (inPins && line.startsWith('-')) {
            const parts = line.split(/\s+/);
            const name = parts[1];
            let dir = 'input';
            let layer = 'met3';
            let x = 0, y = 0;
            const dirIdx = parts.indexOf('DIRECTION');
            if (dirIdx !== -1 && dirIdx + 1 < parts.length) dir = parts[dirIdx + 1].toLowerCase();
            const layerIdx = parts.indexOf('LAYER');
            if (layerIdx !== -1 && layerIdx + 1 < parts.length) layer = parts[layerIdx + 1];
            const placedIdx = parts.indexOf('PLACED') !== -1 ? parts.indexOf('PLACED') : parts.indexOf('FIXED');
            if (placedIdx !== -1 && placedIdx + 4 < parts.length) {
                const rx = parseInt(parts[placedIdx + 2], 10);
                const ry = parseInt(parts[placedIdx + 3], 10);
                if (!isNaN(rx)) x = rx / units;
                if (!isNaN(ry)) y = ry / units;
            }
            pins.push({ name, dir, layer, x, y });
        }
    }
    return { die, core, components, macros: components, pins };
}

async function loadNativeProjectNetlist(pName) {
    const wsFolders = vscode.workspace.workspaceFolders;
    if (!wsFolders || wsFolders.length === 0) return null;
    const rootUri = wsFolders[0].uri;
    const cleanName = (pName || '').replace(/^projects\//, '');
    const projUri = vscode.Uri.joinPath(rootUri, 'projects', cleanName);

    const candidates = [
        `results/structural_${cleanName}.json`,
        'results/structural_chip_top.json',
        'build/structural_synth.json',
        `build/structural_${cleanName}.json`,
        'build/structural_FA.json',
        'results/chip_top_sizes.json'
    ];

    for (const rel of candidates) {
        try {
            const candUri = vscode.Uri.joinPath(projUri, rel);
            const bytes = await vscode.workspace.fs.readFile(candUri);
            const parsed = JSON.parse(new TextDecoder().decode(bytes));
            if (parsed && (parsed.modules || Object.keys(parsed).length > 0)) {
                return parsed;
            }
        } catch (e) {}
    }

    try {
        const found = await vscode.workspace.findFiles(`projects/${cleanName}/**/structural_*.json`, null, 1);
        if (found && found.length > 0) {
            const bytes = await vscode.workspace.fs.readFile(found[0]);
            return JSON.parse(new TextDecoder().decode(bytes));
        }
    } catch (e) {}

    // Fallback: On-demand Yosys schematic elaboration if JSON not yet generated
    try {
        const cp = require('child_process');
        const projPath = projUri.fsPath;
        const candidateDirs = ['source', 'rtl'];
        const svFiles = [];
        for (const dir of candidateDirs) {
            const fullDir = path.join(projPath, dir);
            if (fs.existsSync(fullDir)) {
                fs.readdirSync(fullDir).forEach(f => {
                    if ((f.endsWith('.sv') || f.endsWith('.v')) && !f.startsWith('tb_') && !f.startsWith('tb.')) {
                        svFiles.push(path.join(dir, f));
                    }
                });
            }
        }
        if (svFiles.length > 0) {
            const resultsDir = path.join(projPath, 'results');
            if (!fs.existsSync(resultsDir)) fs.mkdirSync(resultsDir, { recursive: true });
            const outJson = path.join(resultsDir, `structural_${cleanName}.json`);
            cp.execSync(`yosys -p "read_verilog -sv ${svFiles.join(' ')}; hierarchy -auto-top; proc; write_json ${outJson}"`, { cwd: projPath, timeout: 10000 });
            if (fs.existsSync(outJson)) {
                return JSON.parse(fs.readFileSync(outJson, 'utf8'));
            }
        }
    } catch (e) {
        console.warn('[Silis] On-demand Yosys schematic generation skipped:', e.message);
    }

    return null;
}

async function loadNativeWaveformText(pName) {
    const wsFolders = vscode.workspace.workspaceFolders;
    if (!wsFolders || wsFolders.length === 0) return '';
    const rootUri = wsFolders[0].uri;
    const cleanName = (pName || '').replace(/^projects\//, '');
    const projUri = vscode.Uri.joinPath(rootUri, 'projects', cleanName);

    try {
        const defaultUri = vscode.Uri.joinPath(projUri, 'waveform.vcd');
        const bytes = await vscode.workspace.fs.readFile(defaultUri);
        return new TextDecoder().decode(bytes);
    } catch (e) {}

    try {
        const found = await vscode.workspace.findFiles(`projects/${cleanName}/**/*.vcd`, null, 1);
        if (found && found.length > 0) {
            const bytes = await vscode.workspace.fs.readFile(found[0]);
            return new TextDecoder().decode(bytes);
        }
    } catch (e) {}

    return '';
}

async function loadNativeDefData(pName) {
    const wsFolders = vscode.workspace.workspaceFolders;
    if (!wsFolders || wsFolders.length === 0) return null;
    const cleanName = (pName || '').replace(/^projects\//, '');
    try {
        const found = await vscode.workspace.findFiles(`projects/${cleanName}/**/*.def`, null, 1);
        if (found && found.length > 0) {
            const bytes = await vscode.workspace.fs.readFile(found[0]);
            return parseDefContent(new TextDecoder().decode(bytes));
        }
    } catch (e) {}
    return null;
}

async function getProjectsList(currentActive) {
    const wsFolders = vscode.workspace.workspaceFolders;
    if (!wsFolders || wsFolders.length === 0) return [];
    const rootUri = wsFolders[0].uri;
    const projectsUri = vscode.Uri.joinPath(rootUri, 'projects');
    const projects = [];

    try {
        const entries = await vscode.workspace.fs.readDirectory(projectsUri);
        for (const [name, type] of entries) {
            if (type === vscode.FileType.Directory && !name.startsWith('.')) {
                const projDir = vscode.Uri.joinPath(projectsUri, name);
                let meta = {};
                try {
                    const metaUri = vscode.Uri.joinPath(projDir, 'silis.silisproj');
                    const bytes = await vscode.workspace.fs.readFile(metaUri);
                    meta = JSON.parse(new TextDecoder().decode(bytes));
                } catch (e) {
                    try {
                        const altUri = vscode.Uri.joinPath(projDir, `${name}.silisproj`);
                        const bytes = await vscode.workspace.fs.readFile(altUri);
                        meta = JSON.parse(new TextDecoder().decode(bytes));
                    } catch (e2) {}
                }

                let hasDef = false;
                let hasVcd = false;
                try {
                    const subEntries = await vscode.workspace.fs.readDirectory(projDir);
                    for (const [subName] of subEntries) {
                        if (subName.endsWith('.def')) hasDef = true;
                        if (subName.endsWith('.vcd') || subName.endsWith('.fst')) hasVcd = true;
                    }
                } catch (e) {}

                projects.push({
                    name: name,
                    top_module: meta.top_module || name,
                    pdk: meta.pdk || 'sky130',
                    has_def: hasDef,
                    has_vcd: hasVcd,
                    rtl_files: meta.rtl_files || [],
                    path: projDir.fsPath,
                    is_active: (name === currentActive)
                });
            }
        }
    } catch (e) {
        console.error('getProjectsList error:', e);
    }
    return projects;
}

async function refreshReadmeNative(currentActive) {
    try {
        const wsFolders = vscode.workspace.workspaceFolders;
        if (!wsFolders || wsFolders.length === 0) return;
        const rootUri = wsFolders[0].uri;
        const readmeUri = vscode.Uri.joinPath(rootUri, 'README.md');
        const projs = await getProjectsList(currentActive);

        let readmeText = '';
        try {
            const bytes = await vscode.workspace.fs.readFile(readmeUri);
            readmeText = new TextDecoder().decode(bytes);
        } catch (e) { return; }

        const startMarker = '<!-- PROJECTS:START -->';
        const endMarker = '<!-- PROJECTS:END -->';
        const startIdx = readmeText.indexOf(startMarker);
        const endIdx = readmeText.indexOf(endMarker);
        if (startIdx === -1 || endIdx === -1) return;

        let tableMd = '\n\n## Workbench Guide & Control Center\n\n' +
            '> [!IMPORTANT]\n' +
            '> **How to Navigate Silis in VS Code Web / GitHub Codespaces**:\n' +
            '> 1. **Activity Bar (Left Sidebar)**: Click the **Silis CAD** icon to access the Control Center.\n' +
            '> 2. **Run Simulation (Play Button)**: Compile design logic, open schematic split beside RTL, load waveform inspector.\n' +
            '> 3. **Lint RTL (Debug Button / Ctrl+L)**: Run Verilator design rule checks.\n' +
            '> 4. **Tri-Way Cross-Probing**: Waveform <-> Schematic <-> RTL Editor.\n\n' +
            '### Registered ASIC Projects\n\n' +
            '| Project | Process (PDK) | Top Module | Layout (DEF) | Waveforms | Source Files |\n' +
            '| :--- | :--- | :--- | :--- | :--- | :--- |\n';

        for (const p of projs) {
            const defStr = p.has_def ? 'Available' : 'Pending PnR';
            const vcdStr = p.has_vcd ? 'Available' : 'Pending Sim';
            const cnt = (p.rtl_files && p.rtl_files.length) || 1;
            tableMd += `| **${p.name}** | \`${p.pdk}\` | \`${p.top_module}\` | ${defStr} | ${vcdStr} | ${cnt} file(s) |\n`;
        }

        tableMd += '\n### Key Commands (`Ctrl+Shift+P`)\n\n' +
            '- `Silis: Open Unified CAD Workbench` -- Open tabbed CAD tools in single Webview\n' +
            '- `Silis: Open Project` -- Select active project, focus directory in Explorer, open top RTL\n' +
            '- `Silis: Compile & Run Simulation` -- Compile design, display schematic split beside RTL, focus bottom waveform dock\n' +
            '- `Silis: Lint RTL (Ctrl+L)` -- Run Verilator strict RTL lint checks\n' +
            '- `Silis: Open 2D Layout Canvas` -- Open FastSiliconPeeker 2D DEF layout canvas\n' +
            '- `Silis: Open Floorplan & Power Studio` -- Open multi-domain UPF floorplanning canvas\n' +
            '- `Silis: Run Synthesis (Yosys)` -- Synthesize active project into gate-level netlist\n' +
            '- `Silis: Run Tapeout Flow (OpenROAD)` -- Execute floorplan, CTS, placement, and routing\n\n';

        const updatedReadme = readmeText.slice(0, startIdx + startMarker.length) + tableMd + readmeText.slice(endIdx);
        await vscode.workspace.fs.writeFile(readmeUri, new TextEncoder().encode(updatedReadme));
    } catch (e) {
        console.error('refreshReadmeNative error:', e);
    }
}

function activate(context) {
    console.log('[Silis CAD Extension] Activating Silis CS EDA Workbench...');

    let activeProject = context.workspaceState.get('silis.activeProject', 'demo_soc');

    function getActiveProjectDir(pName) {
        let name = pName || activeProject || 'demo_soc';
        if (typeof name === 'object' && name.project) {
            name = name.project;
        }
        if (typeof name === 'string' && name.startsWith('projects/')) {
            name = name.replace(/^projects\//, '');
        }
        return `projects/${name}`;
    }

    function getProjectFsPath(pName) {
        let name = pName || activeProject || 'demo_soc';
        if (typeof name === 'object' && name.project) {
            name = name.project;
        }
        if (typeof name === 'string') {
            if (path.isAbsolute(name)) return name;
            name = name.replace(/^projects\//, '');
        }
        const wsFolders = vscode.workspace.workspaceFolders;
        const rootPath = wsFolders && wsFolders.length > 0 ? wsFolders[0].uri.fsPath : process.cwd();
        return path.join(rootPath, 'projects', name);
    }

    let activeSidebarWebview = null;

    // 1. Sidebar Webview Provider (Silis CAD in Activity Bar)
    const sidebarProvider = {
        async resolveWebviewView(webviewView) {
            activeSidebarWebview = webviewView;
            webviewView.webview.options = { enableScripts: true };
            webviewView.webview.html = getSidebarHtml(activeProject);

            webviewView.webview.onDidReceiveMessage(async (message) => {
                switch (message.command) {
                    case 'openDashboard':
                        vscode.commands.executeCommand('silis.openDashboard');
                        break;
                    case 'newProject':
                        vscode.commands.executeCommand('silis.newProject');
                        break;
                    case 'openProject':
                        vscode.commands.executeCommand('silis.openProject', message.project);
                        break;
                    case 'openSchematic':
                        vscode.commands.executeCommand('silis.openSchematic', activeProject);
                        break;
                    case 'openWaveforms':
                        vscode.commands.executeCommand('silis.openWaveforms', activeProject);
                        break;
                    case 'openLayout':
                        vscode.commands.executeCommand('silis.openLayout', activeProject);
                        break;
                    case 'openFloorplanner':
                        vscode.commands.executeCommand('silis.openFloorplanner', activeProject);
                        break;
                    case 'getProjects':
                        try {
                            const projs = await getProjectsList(activeProject);
                            safePostMessage(webviewView, { type: 'projectsLoaded', projects: projs, active: activeProject });
                        } catch(e) {}
                        break;
                    case 'runSynthesis':
                        vscode.commands.executeCommand('silis.runSynthesis');
                        break;
                    case 'runAll':
                        vscode.commands.executeCommand('silis.runAll');
                        break;
                }
            });

            webviewView.onDidDispose(() => {
                if (activeSidebarWebview === webviewView) {
                    activeSidebarWebview = null;
                }
            });
        }
    };
    context.subscriptions.push(
        vscode.window.registerWebviewViewProvider('silis-cad-sidebar', sidebarProvider)
    );

    // 1b. Bottom Panel Dock Provider (Waveforms docked tabbed next to Terminal)
    let bottomDockWebview = null;
    let activeSchematicPanel = null;
    let activeLayoutPanel = null;

    function getSchematicWebviewHtml(webview, extensionUri, pName) {
        const schemDir = vscode.Uri.joinPath(extensionUri, 'web', 'schematic');
        const cssUri = webview.asWebviewUri(vscode.Uri.joinPath(schemDir, 'schematic_webview.css'));
        const yosysBridgeUri = webview.asWebviewUri(vscode.Uri.joinPath(schemDir, 'yosys_bridge.js'));
        const jsUri = webview.asWebviewUri(vscode.Uri.joinPath(schemDir, 'schematic_webview.js'));
        const htmlPath = path.join(extensionUri.fsPath, 'web', 'schematic', 'index.html');
        let html = fs.readFileSync(htmlPath, 'utf8');
        html = html.replace('href="schematic_webview.css"', `href="${cssUri}"`);
        html = html.replace('src="yosys_bridge.js"', `src="${yosysBridgeUri}"`);
        html = html.replace('src="schematic_webview.js"', `src="${jsUri}"`);
        return html;
    }

    function getWaveformsWebviewHtml(webview, extensionUri) {
        const waveDir = vscode.Uri.joinPath(extensionUri, 'web', 'waveforms');
        const baseUri = webview.asWebviewUri(waveDir);
        const htmlPath = path.join(extensionUri.fsPath, 'web', 'waveforms', 'index.html');
        let html = fs.readFileSync(htmlPath, 'utf8');
        html = html.replace('<base href="./">', `<base href="${baseUri}/">`);
        return html;
    }

    function getNativeLayoutWebviewHtml(webview, extensionUri, pName) {
        const novncDir = vscode.Uri.joinPath(extensionUri, 'web', 'novnc');
        const cssUri = webview.asWebviewUri(vscode.Uri.joinPath(novncDir, 'novnc_viewer.css'));
        const jsUri = webview.asWebviewUri(vscode.Uri.joinPath(novncDir, 'novnc_viewer.js'));
        const htmlPath = path.join(extensionUri.fsPath, 'web', 'novnc', 'index.html');
        let html = fs.readFileSync(htmlPath, 'utf8');

        const cspSource = webview.cspSource;
        const csp = `<meta http-equiv="Content-Security-Policy" content="default-src 'none'; img-src ${cspSource} data: blob:; style-src ${cspSource} 'unsafe-inline'; script-src ${cspSource} 'unsafe-inline' 'unsafe-eval'; connect-src ${cspSource} blob: data:;">`;

        html = html.replace('<head>', `<head>\n    ${csp}`);
        html = html.replace('href="novnc_viewer.css"', `href="${cssUri}"`);
        html = html.replace('src="novnc_viewer.js"', `src="${jsUri}"`);
        html = html.replace('<span id="project-tag" class="project-tag">demo_soc</span>', `<span id="project-tag" class="project-tag">${pName || 'demo_soc'}</span>`);
        return html;
    }
    const getPeekerWebviewHtml = getNativeLayoutWebviewHtml;

    function getTrackerWebviewHtml(webview, extensionUri) {
        const trackerDir = vscode.Uri.joinPath(extensionUri, 'web', 'tracker');
        const cssUri = webview.asWebviewUri(vscode.Uri.joinPath(trackerDir, 'tracker.css'));
        const htmlPath = path.join(extensionUri.fsPath, 'web', 'tracker', 'index.html');
        let html = fs.readFileSync(htmlPath, 'utf8');
        html = html.replace('href="tracker.css"', `href="${cssUri}"`);
        return html;
    }

    function getSynthConfirmWebviewHtml(webview, extensionUri) {
        const confirmDir = vscode.Uri.joinPath(extensionUri, 'web', 'synth_confirm');
        const cssUri = webview.asWebviewUri(vscode.Uri.joinPath(confirmDir, 'synth_confirm.css'));
        const htmlPath = path.join(extensionUri.fsPath, 'web', 'synth_confirm', 'index.html');
        let html = fs.readFileSync(htmlPath, 'utf8');
        html = html.replace('href="synth_confirm.css"', `href="${cssUri}"`);
        return html;
    }

    async function checkTcpPort(port, host = '127.0.0.1', timeoutMs = 300) {
        return new Promise((resolve) => {
            const sock = new net.Socket();
            let done = false;
            sock.setTimeout(timeoutMs);
            sock.on('connect', () => {
                if (!done) {
                    done = true;
                    sock.destroy();
                    resolve(true);
                }
            });
            sock.on('timeout', () => {
                if (!done) {
                    done = true;
                    sock.destroy();
                    resolve(false);
                }
            });
            sock.on('error', () => {
                if (!done) {
                    done = true;
                    sock.destroy();
                    resolve(false);
                }
            });
            sock.connect(port, host);
        });
    }

    async function ensureHeadlessDisplayBridge() {
        const isOpen = await checkTcpPort(5901, '127.0.0.1', 250);
        if (isOpen) {
            return true;
        }

        try {
            const pgrepXvfb = cp.spawnSync('pgrep', ['-f', 'Xvfb :1'], { encoding: 'utf8' });
            if (!pgrepXvfb.stdout || pgrepXvfb.stdout.trim() === '') {
                const xvfb = cp.spawn('Xvfb', [':1', '-screen', '0', '1920x1080x24', '-ac', '+extension', 'GLX', '+render', '-noreset'], {
                    detached: true,
                    stdio: 'ignore'
                });
                xvfb.unref();
            }
        } catch (e) {
            console.warn('Could not launch Xvfb directly:', e.message);
        }

        await new Promise(r => setTimeout(r, 200));

        try {
            const pgrepVnc = cp.spawnSync('pgrep', ['-f', 'x11vnc.*5901'], { encoding: 'utf8' });
            if (!pgrepVnc.stdout || pgrepVnc.stdout.trim() === '') {
                const vnc = cp.spawn('x11vnc', [
                    '-display', ':1',
                    '-listen', '127.0.0.1',
                    '-rfbport', '5901',
                    '-rfbportv6', '0',
                    '-no6',
                    '-noipv6',
                    '-nopw',
                    '-shared',
                    '-forever'
                ], {
                    detached: true,
                    stdio: 'ignore'
                });
                vnc.unref();
            }
        } catch (e) {
            console.warn('Could not launch x11vnc directly:', e.message);
        }

        for (let i = 0; i < 15; i++) {
            await new Promise(r => setTimeout(r, 200));
            if (await checkTcpPort(5901, '127.0.0.1', 250)) {
                return true;
            }
        }

        return false;
    }

    function findProjectDefFile(pName) {
        const wsFolders = vscode.workspace.workspaceFolders;
        const rootPath = wsFolders ? wsFolders[0].uri.fsPath : process.cwd();
        const cleanName = (pName || 'demo_soc').replace(/^projects\//, '');
        const projDir = path.join(rootPath, 'projects', cleanName);
        const candidates = [
            path.join(projDir, 'results', 'chip_top_filled.def'),
            path.join(projDir, 'results', 'temp.def'),
            path.join(projDir, 'results', 'routed.def'),
            path.join(projDir, 'results', 'placed.def'),
            path.join(projDir, 'results', 'floorplanned.def'),
        ];
        for (const c of candidates) {
            if (fs.existsSync(c)) return c;
        }
        const resultsDir = path.join(projDir, 'results');
        if (fs.existsSync(resultsDir)) {
            try {
                const files = fs.readdirSync(resultsDir);
                for (const f of files) {
                    if (f.endsWith('.def')) return path.join(resultsDir, f);
                }
            } catch (e) {}
        }
        return path.join(projDir, 'results', 'temp.def');
    }

    function launchNativeViewer(mode, pName, specificDefPath = null) {
        const wsFolders = vscode.workspace.workspaceFolders;
        const rootPath = wsFolders ? wsFolders[0].uri.fsPath : process.cwd();
        const cleanName = (pName || activeProject || 'demo_soc').replace(/^projects\//, '');
        const defPath = specificDefPath || findProjectDefFile(cleanName);

        let pythonBin = 'python3';
        if (fs.existsSync('/opt/silis/runtime/opt/python/bin/python3')) {
            pythonBin = '/opt/silis/runtime/opt/python/bin/python3';
        } else if (fs.existsSync('/usr/local/bin/silis-python3')) {
            pythonBin = '/usr/local/bin/silis-python3';
        }

        const runtimeLd = '/opt/silis/runtime/usr/lib/x86_64-linux-gnu:/opt/silis/runtime/opt/python/lib';
        const currentLd = process.env.LD_LIBRARY_PATH || '';
        const ldLibPath = currentLd ? `${runtimeLd}:${currentLd}` : runtimeLd;
        const env = Object.assign({}, process.env, {
            DISPLAY: ':1',
            LIBGL_ALWAYS_SOFTWARE: '1',
            QT_QPA_PLATFORM: 'xcb',
            LD_LIBRARY_PATH: ldLibPath,
            PYTHONPATH: `${rootPath}:${rootPath}/silis:${rootPath}/silis/core:${process.env.PYTHONPATH || ''}`,
        });

        try {
            if (mode === 'fast_peeker') {
                const pyScript = `import sys, silis; from PyQt6.QtWidgets import QApplication; app = QApplication.instance() or QApplication(sys.argv); win = silis.view_layout(r'${defPath}', show=True, detach=False); win.show(); sys.exit(app.exec())`;
                const child = cp.spawn(pythonBin, ['-c', pyScript], { env, cwd: rootPath, detached: true, stdio: 'ignore' });
                child.unref();
            } else if (mode === 'layer_viewer') {
                const pyScript = `import sys, silis; from PyQt6.QtWidgets import QApplication; app = QApplication.instance() or QApplication(sys.argv); win = silis.view_layers(r'${defPath}', show=True, detach=False); win.show(); sys.exit(app.exec())`;
                const child = cp.spawn(pythonBin, ['-c', pyScript], { env, cwd: rootPath, detached: true, stdio: 'ignore' });
                child.unref();
            }
        } catch (err) {
            console.error('Failed to spawn native layout viewer:', err);
        }
    }

    function setupNativeLayoutWebview(panel, pName, specificDefPath = null) {
        activeLayoutPanel = panel;

        panel.webview.options = {
            enableScripts: true,
            retainContextWhenHidden: true,
            localResourceRoots: [vscode.Uri.joinPath(context.extensionUri, 'web')]
        };

        panel.webview.html = getNativeLayoutWebviewHtml(panel.webview, context.extensionUri, pName);

        let panelVncSocket = null;

        panel.webview.onDidReceiveMessage(async (msg) => {
            if (!msg || typeof msg !== 'object') return;

            switch (msg.type) {
                case 'vnc_connect': {
                    const displayReady = await ensureHeadlessDisplayBridge();
                    if (!displayReady) {
                        safePostMessage(panel, {
                            type: 'vnc_error',
                            error: 'Could not connect to headless display bridge (127.0.0.1:5901). Ensure xvfb and x11vnc are installed.'
                        });
                        return;
                    }

                    if (panelVncSocket) {
                        try { panelVncSocket.destroy(); } catch (e) {}
                        panelVncSocket = null;
                    }

                    const client = net.connect({ port: 5901, host: '127.0.0.1' }, () => {
                        safePostMessage(panel, { type: 'vnc_open' });
                    });

                    client.on('data', (chunk) => {
                        safePostMessage(panel, {
                            type: 'vnc_data',
                            data: chunk.toString('base64'),
                            isBase64: true
                        });
                    });

                    client.on('error', (err) => {
                        safePostMessage(panel, { type: 'vnc_error', error: err.message });
                    });

                    client.on('close', () => {
                        safePostMessage(panel, { type: 'vnc_close' });
                    });

                    panelVncSocket = client;
                    break;
                }

                case 'vnc_data': {
                    if (panelVncSocket && !panelVncSocket.destroyed) {
                        let buf;
                        if (typeof msg.data === 'string') {
                            buf = Buffer.from(msg.data, 'base64');
                        } else if (Buffer.isBuffer(msg.data)) {
                            buf = msg.data;
                        } else if (msg.data instanceof Uint8Array) {
                            buf = Buffer.from(msg.data.buffer, msg.data.byteOffset, msg.data.byteLength);
                        } else if (Array.isArray(msg.data)) {
                            buf = Buffer.from(msg.data);
                        } else if (msg.data && msg.data.data && Array.isArray(msg.data.data)) {
                            buf = Buffer.from(msg.data.data);
                        } else if (typeof msg.data === 'object' && msg.data !== null) {
                            buf = Buffer.from(Object.values(msg.data));
                        } else {
                            buf = Buffer.from(msg.data);
                        }
                        panelVncSocket.write(buf);
                    }
                    break;
                }

                case 'vnc_close': {
                    if (panelVncSocket) {
                        panelVncSocket.end();
                        panelVncSocket = null;
                    }
                    break;
                }

                case 'rfb_connected': {
                    launchNativeViewer('fast_peeker', pName, specificDefPath);
                    break;
                }

                case 'launch_viewer': {
                    launchNativeViewer(msg.mode, pName, specificDefPath);
                    break;
                }
            }
        });

        panel.onDidDispose(() => {
            if (panelVncSocket) {
                try { panelVncSocket.destroy(); } catch (e) {}
                panelVncSocket = null;
            }
            if (activeLayoutPanel === panel) {
                activeLayoutPanel = null;
            }
        });
    }

    async function openSchematicPanel(projectName, viewColumn = vscode.ViewColumn.Beside) {
        let pName = projectName || activeProject || 'demo_soc';
        if (typeof pName === 'object' && pName.project) {
            pName = pName.project;
        }
        if (typeof pName === 'string' && pName.startsWith('projects/')) {
            pName = pName.replace(/^projects\//, '');
        }
        if (activeSchematicPanel) {
            activeSchematicPanel.reveal(viewColumn);
            return activeSchematicPanel;
        }

        const panel = vscode.window.createWebviewPanel(
            'silisSchematic',
            `Schematic: ${pName}`,
            viewColumn,
            {
                enableScripts: true,
                retainContextWhenHidden: true,
                localResourceRoots: [vscode.Uri.joinPath(context.extensionUri, 'web')]
            }
        );
        activeSchematicPanel = panel;

        panel.webview.html = getSchematicWebviewHtml(panel.webview, context.extensionUri, pName);

        // Load synthesized netlist natively from workspace
        const sendNetlistToPanel = async () => {
            try {
                const data = await loadNativeProjectNetlist(pName);
                if (data && (data.modules || Object.keys(data).length > 0)) {
                    safePostMessage(panel, { command: 'loadSchematic', netlist: data });
                }
            } catch (e) {
                console.error('[Silis Native Schem] Failed to fetch schematic data:', e);
            }
        };

        // Send netlist immediately and with staggered delays
        sendNetlistToPanel();
        setTimeout(sendNetlistToPanel, 300);
        setTimeout(sendNetlistToPanel, 800);
        setTimeout(sendNetlistToPanel, 1500);

        panel.webview.onDidReceiveMessage(async (msg) => {
            const cadMsg = (msg && msg.fromSilisSchem && msg.data) ? msg.data : msg;
            if (cadMsg) {
                if (cadMsg.command === 'schematicReady' || cadMsg.type === 'schematicReady' || cadMsg.type === 'ready') {
                    sendNetlistToPanel();
                    return;
                }
                if (cadMsg.type === 'wireSelected' && (cadMsg.net || cadMsg.name)) {
                    const netName = cadMsg.net || cadMsg.name;
                    if (bottomDockWebview) {
                        safePostMessage(bottomDockWebview, { type: 'addSignal', signal: netName, name: netName, signalName: netName });
                    }
                    vscode.commands.executeCommand('silis-bottom-dock.focus');
                } else if (cadMsg.type === 'componentSelected' && cadMsg.name) {
                    if (activeLayoutPanel) {
                        safePostMessage(activeLayoutPanel, { type: 'highlightComponent', name: cadMsg.name });
                    }
                } else if (cadMsg.type === 'contextAction') {
                    if (cadMsg.action === 'addToWaveform' && cadMsg.leafName) {
                        if (bottomDockWebview) {
                            safePostMessage(bottomDockWebview, { type: 'addSignal', signal: cadMsg.leafName, name: cadMsg.leafName, signalName: cadMsg.leafName });
                        }
                        vscode.commands.executeCommand('silis-bottom-dock.focus');
                    } else if ((cadMsg.action === 'gotoSource' || cadMsg.action === 'gotoDefinition') && cadMsg.sourcePositions && cadMsg.sourcePositions.length > 0) {
                        const pos = cadMsg.sourcePositions[0];
                        const wsFolders = vscode.workspace.workspaceFolders;
                        if (wsFolders && wsFolders.length > 0) {
                            let relPath = pos.name;
                            if (!relPath.startsWith('projects/') && !relPath.startsWith('/')) {
                                relPath = `projects/${pName}/${relPath}`;
                            }
                            const fileUri = relPath.startsWith('/')
                                ? vscode.Uri.file(relPath)
                                : vscode.Uri.joinPath(wsFolders[0].uri, relPath);
                            const line = (pos.from && pos.from.line) ? pos.from.line : 1;
                            const col = (pos.from && pos.from.column) ? pos.from.column : 0;
                            try {
                                const doc = await vscode.workspace.openTextDocument(fileUri);
                                await vscode.window.showTextDocument(doc, {
                                    viewColumn: vscode.ViewColumn.One,
                                    selection: new vscode.Range(Math.max(0, line - 1), col, Math.max(0, line - 1), col),
                                    preview: false
                                });
                            } catch (e) {
                                console.error('[Silis Cross-Probe] Failed to open source document:', e);
                            }
                        }
                    }
                }
            }
        });

        panel.onDidDispose(() => {
            if (activeSchematicPanel === panel) {
                activeSchematicPanel = null;
            }
        });

        return panel;
    }

    let _pendingVcdFile = null;

    async function loadWaveformInBottomDock(fileOrProject) {
        let pDir = fileOrProject || getActiveProjectDir();
        let pName = typeof pDir === 'string' ? pDir.replace(/^projects\//, '').replace(/\/waveform\.vcd$/, '') : activeProject;
        _pendingVcdFile = fileOrProject || pDir;

        if (bottomDockWebview) {
            bottomDockWebview.webview.html = getWaveformsWebviewHtml(bottomDockWebview.webview, context.extensionUri);

            let vcdText = '';
            if (typeof fileOrProject === 'string' && fileOrProject.endsWith('.vcd')) {
                try {
                    if (fs.existsSync(fileOrProject)) {
                        vcdText = fs.readFileSync(fileOrProject, 'utf8');
                    }
                } catch (e) {}
            }
            if (!vcdText) {
                vcdText = await loadNativeWaveformText(pName);
            }

            if (typeof vcdText === 'string' && vcdText.length > 0) {
                setTimeout(() => {
                    safePostMessage(bottomDockWebview, {
                        command: 'loadVcd',
                        text: vcdText,
                        fileName: path.basename(typeof fileOrProject === 'string' && fileOrProject.endsWith('.vcd') ? fileOrProject : 'waveform.vcd')
                    });
                }, 500);
            }
        }
    }

    const bottomDockProvider = {
        async resolveWebviewView(webviewView) {
            bottomDockWebview = webviewView;
            webviewView.webview.options = {
                enableScripts: true,
                retainContextWhenHidden: true,
                localResourceRoots: [vscode.Uri.joinPath(context.extensionUri, 'web')]
            };

            // Check if pending VCD or active project already has waveform.vcd; if so, load it immediately
            const pDir = _pendingVcdFile || getActiveProjectDir();
            let hasVcd = false;
            try {
                if (typeof pDir === 'string' && pDir.endsWith('.vcd') && fs.existsSync(pDir)) {
                    hasVcd = true;
                } else {
                    const wsFolders = vscode.workspace.workspaceFolders;
                    if (wsFolders && wsFolders.length > 0) {
                        const vcdUri = vscode.Uri.joinPath(wsFolders[0].uri, pDir, 'waveform.vcd');
                        try {
                            const st = await vscode.workspace.fs.stat(vcdUri);
                            hasVcd = (st.size > 0);
                        } catch (e) {}
                    }
                }
            } catch (e) {}

            if (hasVcd) {
                await loadWaveformInBottomDock(pDir);
            } else {
                webviewView.webview.html = getBottomDockIdleHtml();
            }

            let _scrubTimer = null;
            let _pendingScrubTime = null;

            webviewView.webview.onDidReceiveMessage(async (msg) => {
                if (msg && msg.fromSilisRohd && msg.data) {
                    const rohdMsg = msg.data;
                    if (rohdMsg.type === 'cursorMoved' && activeSchematicPanel) {
                        _pendingScrubTime = rohdMsg.time || 0;
                        if (_scrubTimer) clearTimeout(_scrubTimer);
                        _scrubTimer = setTimeout(async () => {
                            _scrubTimer = null;
                            const timePs = _pendingScrubTime;
                            try {
                                const vcdText = await loadNativeWaveformText(activeProject);
                                const updates = getVcdValuesAtTimestamp(vcdText, timePs);
                                if (Array.isArray(updates) && updates.length > 0 && activeSchematicPanel) {
                                    safePostMessage(activeSchematicPanel, { type: 'setValues', updates: updates });
                                }
                            } catch (e) {
                                console.error('[Silis Cross-Probe] Native VCD value evaluation failed:', e);
                            }
                        }, 25);
                    } else if (rohdMsg.type === 'jumpToAssertion' && rohdMsg.file) {
                        const wsFolders = vscode.workspace.workspaceFolders;
                        if (wsFolders && wsFolders.length > 0) {
                            let relPath = rohdMsg.file;
                            if (!relPath.startsWith('projects/') && !relPath.startsWith('/')) {
                                relPath = `projects/${activeProject}/${relPath}`;
                            }
                            const fileUri = relPath.startsWith('/')
                                ? vscode.Uri.file(relPath)
                                : vscode.Uri.joinPath(wsFolders[0].uri, relPath);
                            const line = rohdMsg.line ? Math.max(0, rohdMsg.line - 1) : 0;
                            try {
                                const doc = await vscode.workspace.openTextDocument(fileUri);
                                await vscode.window.showTextDocument(doc, {
                                    viewColumn: vscode.ViewColumn.One,
                                    selection: new vscode.Range(line, 0, line, 0),
                                    preview: false
                                });
                            } catch (e) {
                                console.error('[Silis Assertion] Failed to open source document:', e);
                            }
                        }
                    } else if (rohdMsg.type === 'openWaveformFile') {
                        vscode.commands.executeCommand('silis.openWaveforms');
                    }
                }
            });

            webviewView.onDidDispose(() => {
                if (_scrubTimer) {
                    clearTimeout(_scrubTimer);
                    _scrubTimer = null;
                }
                bottomDockWebview = null;
            });
        }
    };
    context.subscriptions.push(
        vscode.window.registerWebviewViewProvider('silis-bottom-dock', bottomDockProvider)
    );

    // 1c. Project State Tracker Webview View Provider (Option A: First-Class Tab beside Terminal)
    let stateTrackerWebview = null;
    let _isSimRunning = false;
    let _isSynthRunning = false;

    async function getProjectFlowState(pName) {
        const cleanName = (pName || activeProject || 'demo_soc').replace(/^projects\//, '');
        const projDir = getProjectFsPath(cleanName);

        const vcdPath = path.join(projDir, 'waveform.vcd');
        const hasVcd = fs.existsSync(vcdPath) && fs.statSync(vcdPath).size > 0;

        // Gate-level Netlist & synthesis area reports (distinct from schematic AST)
        const netlistCandidates = [
            path.join(projDir, 'netlist', `${cleanName}_netlist.v`),
            path.join(projDir, 'netlist', 'chip_top_netlist.v'),
            path.join(projDir, 'netlist', `${cleanName}_scan.v`),
            path.join(projDir, 'reports', 'area.rpt')
        ];
        const hasNetlist = netlistCandidates.some(f => fs.existsSync(f) && fs.statSync(f).size > 0);

        // Pre-synthesis structural AST for schematic generator
        const schemCandidates = [
            path.join(projDir, 'results', `structural_${cleanName}.json`),
            path.join(projDir, 'results', 'structural_chip_top.json'),
            path.join(projDir, 'build', 'structural_synth.json'),
            path.join(projDir, 'build', `structural_${cleanName}.json`)
        ];
        const hasSchematic = schemCandidates.some(f => fs.existsSync(f) && fs.statSync(f).size > 0);

        const hasFloorplan = fs.existsSync(path.join(projDir, 'run_floorplan.tcl')) || fs.existsSync(path.join(projDir, '.silis_fp_session.json'));

        const defCandidates = [
            path.join(projDir, 'results', `${cleanName}_filled.def`),
            path.join(projDir, 'results', 'chip_top_filled.def'),
            path.join(projDir, 'results', 'design.gds')
        ];
        const hasDef = defCandidates.some(f => fs.existsSync(f) && fs.statSync(f).size > 0);

        return {
            projectName: cleanName,
            topModule: cleanName === 'demo_soc' ? 'chip_top' : cleanName,
            pdk: 'sky130_fd_sc_hd',
            hasVcd,
            hasNetlist,
            hasSchematic,
            hasFloorplan,
            hasDef,
            isSimRunning: _isSimRunning,
            isSynthRunning: _isSynthRunning
        };
    }

    async function syncStateTracker() {
        if (!stateTrackerWebview) return;
        try {
            const state = await getProjectFlowState(activeProject);
            safePostMessage(stateTrackerWebview, { command: 'updateState', state });
        } catch (e) {}
    }

    const stateTrackerProvider = {
        async resolveWebviewView(webviewView) {
            stateTrackerWebview = webviewView;
            webviewView.webview.options = {
                enableScripts: true,
                retainContextWhenHidden: true,
                localResourceRoots: [vscode.Uri.joinPath(context.extensionUri, 'web')]
            };
            webviewView.webview.html = getTrackerWebviewHtml(webviewView.webview, context.extensionUri);

            setTimeout(syncStateTracker, 200);

            webviewView.onDidChangeVisibility(() => {
                if (webviewView.visible) {
                    syncStateTracker();
                }
            });

            webviewView.webview.onDidReceiveMessage(async (msg) => {
                if (!msg) return;
                if (msg.command === 'trackerReady' || msg.command === 'refreshState') {
                    syncStateTracker();
                    return;
                }
                if (msg.command === 'trackerAction') {
                    const action = msg.action;
                    if (action === 'runLint') {
                        vscode.commands.executeCommand('silis.runLint');
                    } else if (action === 'runSimulation') {
                        vscode.commands.executeCommand('silis.runSimulation');
                    } else if (action === 'openWaveforms') {
                        vscode.commands.executeCommand('silis.openWaveformInBottomDock');
                    } else if (action === 'openSynthConfirm') {
                        vscode.commands.executeCommand('silis.openSynthConfirm');
                    } else if (action === 'openSchematic') {
                        await openSchematicPanel(activeProject, vscode.ViewColumn.Beside);
                    } else if (action === 'openFloorplanner') {
                        vscode.commands.executeCommand('silis.openFloorplanner');
                    } else if (action === 'runPlacement') {
                        const term = getFlowTerminal();
                        term.show();
                        const pDir = getProjectFsPath(activeProject);
                        term.sendText(`cd "${pDir}" && if [ -f run_place.tcl ]; then openroad -no_init -exit run_place.tcl; else echo "Run floorplan commit first."; fi`);
                    } else if (action === 'openLayout') {
                        vscode.commands.executeCommand('silis.openLayout');
                    } else if (action === 'runTapeout') {
                        vscode.commands.executeCommand('silis.runAll');
                    } else if (action === 'openFile' && msg.path) {
                        const wsFolders = vscode.workspace.workspaceFolders;
                        if (wsFolders && wsFolders.length > 0) {
                            let p = msg.path;
                            if (!p.startsWith('projects/') && !p.startsWith('/')) {
                                p = `projects/${activeProject}/${p}`;
                            }
                            const fileUri = p.startsWith('/') ? vscode.Uri.file(p) : vscode.Uri.joinPath(wsFolders[0].uri, p);
                            try {
                                const doc = await vscode.workspace.openTextDocument(fileUri);
                                await vscode.window.showTextDocument(doc, { viewColumn: vscode.ViewColumn.One, preview: false });
                            } catch (e) {}
                        }
                    }
                }
            });

            webviewView.onDidDispose(() => {
                stateTrackerWebview = null;
            });
        }
    };
    context.subscriptions.push(
        vscode.window.registerWebviewViewProvider('silis-project-tracker', stateTrackerProvider)
    );

    // 2. Custom Editor Provider for VCD / Waveforms
    const vcdEditorProvider = {
        async resolveCustomTextEditor(document, webviewPanel) {
            webviewPanel.webview.options = {
                enableScripts: true,
                retainContextWhenHidden: true,
                localResourceRoots: [vscode.Uri.joinPath(context.extensionUri, 'web')]
            };
            webviewPanel.webview.html = getWaveformsWebviewHtml(webviewPanel.webview, context.extensionUri);
            try {
                const text = document.getText();
                setTimeout(() => {
                    safePostMessage(webviewPanel, {
                        command: 'loadVcd',
                        text: text,
                        fileName: path.basename(document.fileName)
                    });
                }, 400);
            } catch (e) {}

            let _docScrubTimer = null;
            webviewPanel.webview.onDidReceiveMessage(async (msg) => {
                if (msg && msg.fromSilisRohd && msg.data) {
                    const rohdMsg = msg.data;
                    if (rohdMsg.type === 'cursorMoved' && activeSchematicPanel) {
                        if (_docScrubTimer) clearTimeout(_docScrubTimer);
                        _docScrubTimer = setTimeout(async () => {
                            _docScrubTimer = null;
                            try {
                                const vcdText = document.getText();
                                const updates = getVcdValuesAtTimestamp(vcdText, rohdMsg.time || 0);
                                if (Array.isArray(updates) && updates.length > 0 && activeSchematicPanel) {
                                    safePostMessage(activeSchematicPanel, { type: 'setValues', updates: updates });
                                }
                            } catch (e) {}
                        }, 25);
                    }
                }
            });
            webviewPanel.onDidDispose(() => {
                if (_docScrubTimer) clearTimeout(_docScrubTimer);
            });
        }
    };
    context.subscriptions.push(
        vscode.window.registerCustomEditorProvider('silis.vcdViewer', vcdEditorProvider)
    );

    // 3. Custom Editor Provider for Structural Netlists (DigitalJS Schematic)
    const schematicEditorProvider = {
        async resolveCustomTextEditor(document, webviewPanel) {
            webviewPanel.webview.options = {
                enableScripts: true,
                retainContextWhenHidden: true,
                localResourceRoots: [vscode.Uri.joinPath(context.extensionUri, 'web')]
            };
            webviewPanel.webview.html = getSchematicWebviewHtml(webviewPanel.webview, context.extensionUri, activeProject);
            activeSchematicPanel = webviewPanel;
            webviewPanel.onDidDispose(() => {
                if (activeSchematicPanel === webviewPanel) activeSchematicPanel = null;
            });
            try {
                const content = document.getText();
                const netlist = JSON.parse(content);
                setTimeout(() => {
                    safePostMessage(webviewPanel, { command: 'loadSchematic', netlist: netlist });
                }, 400);
            } catch (e) {}

            webviewPanel.webview.onDidReceiveMessage(async (msg) => {
                const cadMsg = (msg && msg.fromSilisSchem && msg.data) ? msg.data : msg;
                if (cadMsg) {
                    if (cadMsg.type === 'wireSelected' && (cadMsg.net || cadMsg.name)) {
                        const netName = cadMsg.net || cadMsg.name;
                        if (bottomDockWebview) {
                            safePostMessage(bottomDockWebview, { type: 'addSignal', signal: netName, name: netName, signalName: netName });
                        }
                        vscode.commands.executeCommand('silis-bottom-dock.focus');
                    } else if (cadMsg.type === 'componentSelected' && cadMsg.name) {
                        if (activeLayoutPanel) {
                            safePostMessage(activeLayoutPanel, { type: 'highlightComponent', name: cadMsg.name });
                        }
                    } else if (cadMsg.type === 'contextAction' && cadMsg.action === 'addToWaveform' && cadMsg.leafName) {
                        if (bottomDockWebview) {
                            safePostMessage(bottomDockWebview, { type: 'addSignal', signal: cadMsg.leafName, name: cadMsg.leafName, signalName: cadMsg.leafName });
                        }
                        vscode.commands.executeCommand('silis-bottom-dock.focus');
                    }
                }
            });
        }
    };
    context.subscriptions.push(
        vscode.window.registerCustomEditorProvider('silis.schematicViewer', schematicEditorProvider)
    );

    // 3b. Custom Editor Provider for DEF 2D Silicon Layout (Portless Native Bridge)
    const defEditorProvider = {
        async resolveCustomTextEditor(document, webviewPanel) {
            const defPath = document.fileName;
            setupNativeLayoutWebview(webviewPanel, activeProject, defPath);
        }
    };
    context.subscriptions.push(
        vscode.window.registerCustomEditorProvider('silis.defViewer', defEditorProvider)
    );

    // 3c. Custom Editor Provider for GDSII
    const gdsEditorProvider = {
        async resolveCustomTextEditor(document, webviewPanel) {
            webviewPanel.webview.options = { enableScripts: true };
            const baseName = escapeHtml(require('path').basename(document.fileName));
            webviewPanel.webview.html = `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; background: var(--vscode-editor-background); color: var(--vscode-foreground); display: flex; align-items: center; justify-content: center; height: 100vh; margin: 0; }
        .card { max-width: 520px; text-align: center; padding: 32px; border: 1px solid var(--vscode-widget-border, #333); border-radius: 8px; }
        h2 { font-size: 16px; margin-bottom: 12px; color: var(--vscode-textLink-foreground); }
        p { font-size: 13px; line-height: 1.6; color: var(--vscode-descriptionForeground); }
        code { background: var(--vscode-textCodeBlock-background); padding: 2px 6px; border-radius: 4px; font-family: monospace; }
    </style>
</head>
<body>
    <div class="card">
        <h2>GDSII Binary Layout: ${baseName}</h2>
        <p>In cloud Codespaces, interactive 2D layout inspection is rendered natively for DEF physical designs via <strong>Silis 2D Layout Peeker</strong>.</p>
        <p>To inspect binary GDSII geometry, run KLayout or OpenROAD from the terminal:</p>
        <p><code>klayout ${baseName}</code> or <code>openroad -gui</code></p>
    </div>
</body>
</html>`;
        }
    };
    context.subscriptions.push(
        vscode.window.registerCustomEditorProvider('silis.gdsViewer', gdsEditorProvider)
    );

    // 4. Silis ASIC Studio Dashboard Webview Tab (Primary Welcome & Project Hub)
    let dashboardPanel = null;
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openDashboard', async () => {
            if (dashboardPanel) {
                dashboardPanel.reveal(vscode.ViewColumn.Active);
                return;
            }
            dashboardPanel = vscode.window.createWebviewPanel(
                'silisDashboard',
                'Silis ASIC Studio',
                vscode.ViewColumn.Active,
                { enableScripts: true, retainContextWhenHidden: true }
            );

            dashboardPanel.webview.html = getDashboardHtml(activeProject);

            dashboardPanel.webview.onDidReceiveMessage(async (msg) => {
                switch (msg.command) {
                    case 'newProject':
                        vscode.commands.executeCommand('silis.newProject');
                        break;
                    case 'openProject':
                        vscode.commands.executeCommand('silis.openProject', msg.project);
                        break;
                    case 'openRTL':
                        if (msg.file) {
                            const wsFolders = vscode.workspace.workspaceFolders;
                            if (wsFolders && wsFolders.length > 0) {
                                const fileUri = (typeof msg.file === 'string' && msg.file.startsWith('/'))
                                    ? vscode.Uri.file(msg.file)
                                    : vscode.Uri.joinPath(wsFolders[0].uri, msg.file);
                                vscode.window.showTextDocument(fileUri, { preview: false });
                            }
                        }
                        break;
                    case 'openSchematic':
                        vscode.commands.executeCommand('silis.openSchematic', msg.project || activeProject);
                        break;
                    case 'openWaveforms':
                        vscode.commands.executeCommand('silis.openWaveforms', msg.project || activeProject);
                        break;
                    case 'openLayout':
                        vscode.commands.executeCommand('silis.openLayout', msg.project || activeProject);
                        break;
                    case 'openFloorplanner':
                        vscode.commands.executeCommand('silis.openFloorplanner', msg.project || activeProject);
                        break;
                    case 'getProjects':
                        try {
                            const projs = await getProjectsList(activeProject);
                            safePostMessage(dashboardPanel, { type: 'projectsLoaded', projects: projs, active: activeProject });
                        } catch(e) {}
                        break;
                    case 'runSynthesis':
                        vscode.commands.executeCommand('silis.runSynthesis');
                        break;
                    case 'runAll':
                        vscode.commands.executeCommand('silis.runAll');
                        break;
                    case 'refresh':
                        vscode.commands.executeCommand('silis.refreshReadme');
                        break;
                }
            });

            dashboardPanel.onDidDispose(() => {
                dashboardPanel = null;
            });
        })
    );

    // 5. Authentic 6-Stage Silis Project Creation Wizard Webview
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.newProject', async () => {
            const panel = vscode.window.createWebviewPanel(
                'silisNewProject',
                'Create New Silis Project',
                vscode.ViewColumn.Active,
                { enableScripts: true, retainContextWhenHidden: true }
            );

            // Scan workspace for existing RTL/HDL files that could be imported
            let workspaceFiles = [];
            try {
                const uris = await vscode.workspace.findFiles('**/*.{v,sv,vhd,vhdl}', '**/node_modules/**');
                workspaceFiles = uris.map(u => vscode.workspace.asRelativePath(u));
            } catch (e) {}

            panel.webview.html = getProjectWizardHtml(workspaceFiles);

            panel.webview.onDidReceiveMessage(async (message) => {
                if (message.command === 'fetchPdks') {
                    safePostMessage(panel, { type: 'pdksLoaded', pdks: ['sky130', 'sky130A', 'sky130B', 'gf180mcu', 'asap7', 'freepdk45'] });
                    return;
                }

                if (message.command === 'fetchMacros') {
                    const macros = [
                        { name: 'simple_alu', w: 220, h: 160 },
                        { name: 'sky130_sram_1kbyte_1rw1r_32x256_8', w: 420, h: 280 },
                        { name: 'sky130_sram_2kbyte_1rw1r_32x512_8', w: 420, h: 280 }
                    ];
                    safePostMessage(panel, { type: 'macrosLoaded', macros: macros });
                    return;
                }

                if (message.command === 'browseFiles') {
                    const opts = {
                        canSelectFiles: message.canSelectFiles !== false,
                        canSelectFolders: !!message.canSelectFolders,
                        canSelectMany: !!message.canSelectMany,
                        openLabel: message.title || 'Select',
                        filters: message.filters || undefined
                    };
                    const uris = await vscode.window.showOpenDialog(opts);
                    if (uris && uris.length > 0) {
                        safePostMessage(panel, {
                            type: 'filesSelected',
                            target: message.target,
                            files: uris.map(u => u.fsPath)
                        });
                    }
                    return;
                }

                if (message.command === 'createProject') {
                    const payload = message.data;
                    try {
                        const wsFolders = vscode.workspace.workspaceFolders;
                        if (!wsFolders || wsFolders.length === 0) return;
                        const rootUri = wsFolders[0].uri;
                        const pName = payload.name;
                        const topMod = payload.top_module || pName;
                        const projUri = vscode.Uri.joinPath(rootUri, 'projects', pName);

                        await vscode.workspace.fs.createDirectory(projUri);
                        await vscode.workspace.fs.createDirectory(vscode.Uri.joinPath(projUri, 'rtl'));
                        await vscode.workspace.fs.createDirectory(vscode.Uri.joinPath(projUri, 'tb'));
                        await vscode.workspace.fs.createDirectory(vscode.Uri.joinPath(projUri, 'constr'));

                        const meta = {
                            project_name: pName,
                            top_module: topMod,
                            pdk: payload.pdk || 'sky130',
                            macros: payload.macros || [],
                            auto_sdc: true,
                            rtl_files: [`rtl/${pName}.sv`],
                            sdc_files: ['constr/default.sdc'],
                            tb_files: [`tb/tb_${pName}.v`]
                        };
                        await vscode.workspace.fs.writeFile(
                            vscode.Uri.joinPath(projUri, 'silis.silisproj'),
                            new TextEncoder().encode(JSON.stringify(meta, null, 4))
                        );

                        const rtlTmpl = `// ${pName}.sv - Top Module: ${topMod}\n` +
                            `module ${topMod} (\n` +
                            `    input  logic clk,\n` +
                            `    input  logic rst_n,\n` +
                            `    input  logic [7:0] data_in,\n` +
                            `    output logic [7:0] data_out\n` +
                            `);\n` +
                            `    always_ff @(posedge clk or negedge rst_n) begin\n` +
                            `        if (!rst_n) data_out <= 8'h00;\n` +
                            `        else        data_out <= data_in + 8'h01;\n` +
                            `    end\n` +
                            `endmodule\n`;
                        await vscode.workspace.fs.writeFile(
                            vscode.Uri.joinPath(projUri, 'rtl', `${pName}.sv`),
                            new TextEncoder().encode(rtlTmpl)
                        );

                        const sdcTmpl = `create_clock -name clk -period 10.0 [get_ports clk]\n` +
                            `set_input_delay 2.0 -clock clk [all_inputs]\n` +
                            `set_output_delay 2.0 -clock clk [all_outputs]\n`;
                        await vscode.workspace.fs.writeFile(
                            vscode.Uri.joinPath(projUri, 'constr', 'default.sdc'),
                            new TextEncoder().encode(sdcTmpl)
                        );

                        const tbTmpl = `\`timescale 1ns/1ps\n` +
                            `module tb_${pName};\n` +
                            `    logic clk = 0;\n` +
                            `    logic rst_n = 0;\n` +
                            `    logic [7:0] din = 0;\n` +
                            `    wire [7:0] dout;\n` +
                            `    always #5 clk = ~clk;\n` +
                            `    ${topMod} dut (.clk(clk), .rst_n(rst_n), .data_in(din), .data_out(dout));\n` +
                            `    initial begin\n` +
                            `        $dumpfile("waveform.vcd");\n` +
                            `        $dumpvars(0, tb_${pName});\n` +
                            `        #20 rst_n = 1; din = 8'h10;\n` +
                            `        #40 din = 8'h20;\n` +
                            `        #60 $finish;\n` +
                            `    end\n` +
                            `endmodule\n`;
                        await vscode.workspace.fs.writeFile(
                            vscode.Uri.joinPath(projUri, 'tb', `tb_${pName}.v`),
                            new TextEncoder().encode(tbTmpl)
                        );

                        activeProject = pName;
                        await context.workspaceState.update('silis.activeProject', activeProject);
                        safePostMessage(panel, { type: 'created', name: pName, top_module: topMod });

                        vscode.window.showInformationMessage(`Silis Project "${activeProject}" created successfully.`);

                        const rtlUri = vscode.Uri.joinPath(projUri, 'rtl', `${pName}.sv`);
                        vscode.window.showTextDocument(rtlUri, { preview: false });

                        // Refresh dynamic README, dashboard, and sidebar
                        await refreshReadmeNative(activeProject);
                        const projs = await getProjectsList(activeProject);
                        if (dashboardPanel) {
                            safePostMessage(dashboardPanel, { type: 'projectsLoaded', projects: projs, active: activeProject });
                        }
                        if (activeSidebarWebview) {
                            safePostMessage(activeSidebarWebview, { type: 'projectsLoaded', projects: projs, active: activeProject });
                        }

                        // Auto-close wizard dialog
                        setTimeout(() => {
                            try { panel.dispose(); } catch (e) {}
                        }, 600);
                    } catch (e) {
                        vscode.window.showErrorMessage(`Failed to create project: ${e.message}`);
                    }
                    return;
                }
            });
        })
    );

    // 6. Open / Switch Active Project (Desktop Parity: Open top RTL on left, reveal in Explorer)
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openProject', async (projectName) => {
            if (!projectName) {
                try {
                    const projects = await getProjectsList(activeProject);
                    if (Array.isArray(projects) && projects.length > 0) {
                        const items = projects.map(p => ({
                            label: p.name,
                            description: `${p.pdk} - Top: ${p.top_module}`,
                            detail: p.path
                        }));
                        const chosen = await vscode.window.showQuickPick(items, {
                            placeHolder: 'Select active Silis project'
                        });
                        if (chosen) {
                            projectName = chosen.label;
                        }
                    }
                } catch (e) {}
            }

            if (projectName) {
                activeProject = projectName;
                await context.workspaceState.update('silis.activeProject', activeProject);

                // Fetch registered project metadata natively
                let matchedProject = null;
                try {
                    const projects = await getProjectsList(activeProject);
                    if (Array.isArray(projects)) {
                        matchedProject = projects.find(p => p.name === activeProject || p.path === activeProject);
                    }
                } catch (e) {}

                const wsFolders = vscode.workspace.workspaceFolders;
                const pathMod = require('path');
                const fsMod = require('fs');

                let projFsPath = null;
                if (matchedProject && matchedProject.path && fsMod.existsSync(matchedProject.path)) {
                    projFsPath = matchedProject.path;
                } else if (wsFolders && wsFolders.length > 0) {
                    projFsPath = pathMod.join(wsFolders[0].uri.fsPath, 'projects', activeProject);
                    if (!fsMod.existsSync(projFsPath)) {
                        projFsPath = pathMod.join(wsFolders[0].uri.fsPath, activeProject);
                    }
                }

                if (projFsPath && fsMod.existsSync(projFsPath)) {
                    const projUri = vscode.Uri.file(projFsPath);
                    try {
                        await vscode.commands.executeCommand('revealInExplorer', projUri);
                    } catch (e) {}

                    // Open top RTL file in ViewColumn.One
                    let topFileUri = null;

                    // 1. Check explicit rtl_files from project metadata
                    if (matchedProject && matchedProject.rtl_files && matchedProject.rtl_files.length > 0) {
                        for (const rf of matchedProject.rtl_files) {
                            const candPath = pathMod.isAbsolute(rf) ? rf : pathMod.join(projFsPath, rf);
                            if (fsMod.existsSync(candPath) && !fsMod.statSync(candPath).isDirectory()) {
                                topFileUri = vscode.Uri.file(candPath);
                                break;
                            }
                        }
                    }

                    // 2. Check top_module candidates in standard dirs
                    if (!topFileUri) {
                        const topMod = (matchedProject && matchedProject.top_module) ? matchedProject.top_module : activeProject;
                        for (const sub of ['rtl', 'source', 'src', '.']) {
                            for (const ext of ['.v', '.sv']) {
                                const candPath = pathMod.join(projFsPath, sub, topMod + ext);
                                if (fsMod.existsSync(candPath) && !fsMod.statSync(candPath).isDirectory()) {
                                    topFileUri = vscode.Uri.file(candPath);
                                    break;
                                }
                            }
                            if (topFileUri) break;
                        }
                    }

                    // 3. Scan project directory for any non-testbench HDL file
                    if (!topFileUri) {
                        for (const sub of ['rtl', 'source', 'src', '.']) {
                            const subDir = pathMod.join(projFsPath, sub);
                            if (fsMod.existsSync(subDir) && fsMod.statSync(subDir).isDirectory()) {
                                const files = fsMod.readdirSync(subDir);
                                const hdl = files.find(f => (f.endsWith('.v') || f.endsWith('.sv')) && !f.startsWith('tb_') && !f.includes('_tb'));
                                if (hdl) {
                                    topFileUri = vscode.Uri.file(pathMod.join(subDir, hdl));
                                    break;
                                }
                            }
                        }
                    }

                    if (topFileUri) {
                        try {
                            const doc = await vscode.workspace.openTextDocument(topFileUri);
                            await vscode.window.showTextDocument(doc, { viewColumn: vscode.ViewColumn.One, preview: false });
                        } catch (e) {
                            console.error('[Silis] Failed to open top RTL document:', e);
                        }
                    }
                }

                vscode.window.showInformationMessage(`Active Project: ${activeProject}`);

                if (dashboardPanel) {
                    safePostMessage(dashboardPanel, { type: 'refresh' });
                }
                if (activeSidebarWebview) {
                    safePostMessage(activeSidebarWebview, { type: 'activeProjectChanged', active: activeProject });
                }
            }
        })
    );

    // 7. Waveforms: Dock in Bottom Panel Next to Terminal
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openWaveforms', async (param) => {
            if (param && !param.endsWith('.vcd') && !param.endsWith('.fst') && param !== activeProject) {
                activeProject = param;
                await context.workspaceState.update('silis.activeProject', activeProject);
            }
            await loadWaveformInBottomDock(param || getActiveProjectDir());
        })
    );

    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openWaveformInBottomDock', async (uri) => {
            const filePath = uri ? uri.fsPath : null;
            await loadWaveformInBottomDock(filePath || getActiveProjectDir());
        })
    );

    // 8. Schematic Viewer (World 0: Split beside RTL)
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openSchematic', async (projectName) => {
            await openSchematicPanel(projectName, vscode.ViewColumn.Beside);
        })
    );

    // 9. Native Layout Canvas (Portless Webview IPC Tunnel to FastSiliconPeeker & LayerViewerPanel)
    const openLayoutHandler = async (projectName) => {
        const pName = projectName || activeProject || 'demo_soc';
        if (activeLayoutPanel) {
            activeLayoutPanel.reveal(vscode.ViewColumn.Active);
            return;
        }
        activeLayoutPanel = vscode.window.createWebviewPanel(
            'silisLayout',
            `Native Layout: ${pName}`,
            vscode.ViewColumn.Active,
            {
                enableScripts: true,
                retainContextWhenHidden: true,
                localResourceRoots: [vscode.Uri.joinPath(context.extensionUri, 'web')]
            }
        );
        setupNativeLayoutWebview(activeLayoutPanel, pName);
    };

    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openLayout', openLayoutHandler)
    );
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openNativeLayout', openLayoutHandler)
    );

    // 10. Floorplan & Power Studio (Native Webview CAD Canvas & Physics Engine)
    let activeFloorplanPanel = null;

    function getFpsWebviewHtml(webview, extensionUri, pName) {
        const fpsDir = vscode.Uri.joinPath(extensionUri, 'web', 'fps');
        const cssUri = webview.asWebviewUri(vscode.Uri.joinPath(fpsDir, 'fps.css'));
        const jsUri = webview.asWebviewUri(vscode.Uri.joinPath(fpsDir, 'fps_canvas.js'));
        const htmlPath = path.join(extensionUri.fsPath, 'web', 'fps', 'index.html');
        let html = fs.readFileSync(htmlPath, 'utf8');
        html = html.replace('href="fps.css"', `href="${cssUri}"`);
        html = html.replace('src="fps_canvas.js"', `src="${jsUri}"`);
        return html;
    }

    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openFloorplanner', async (projectName) => {
            const pName = projectName || activeProject || 'demo_soc';

            // Minimize/dispose split schematic panel so FPS has the full main editor workspace
            if (activeSchematicPanel) {
                try {
                    activeSchematicPanel.dispose();
                } catch (e) {}
                activeSchematicPanel = null;
            }

            if (activeFloorplanPanel) {
                activeFloorplanPanel.reveal(vscode.ViewColumn.One);
                return;
            }
            activeFloorplanPanel = vscode.window.createWebviewPanel(
                'silisFPS',
                `Floorplan & Power Studio: ${pName}`,
                vscode.ViewColumn.One,
                { enableScripts: true, retainContextWhenHidden: true }
            );

            activeFloorplanPanel.webview.html = getFpsWebviewHtml(activeFloorplanPanel.webview, context.extensionUri, pName);

            // Hydrate existing floorplan session state from .silis_fp_session.json or netlist defaults
            const wsFolders = vscode.workspace.workspaceFolders;
            const rootPath = wsFolders ? wsFolders[0].uri.fsPath : '';
            const sessionPath = path.join(rootPath, 'projects', pName, '.silis_fp_session.json');
            if (fs.existsSync(sessionPath)) {
                try {
                    const sessionData = JSON.parse(fs.readFileSync(sessionPath, 'utf8'));
                    setTimeout(() => {
                        safePostMessage(activeFloorplanPanel, { command: 'loadSession', session: sessionData });
                    }, 400);
                } catch (e) {
                    console.error('Failed to parse existing .silis_fp_session.json', e);
                }
            } else {
                // Initialize default demo session with macros if structural netlist is available
                setTimeout(async () => {
                    try {
                        const netlist = await loadNativeProjectNetlist(pName);
                        const pins = [];
                        const macros = [];
                        if (netlist && netlist.modules) {
                            for (const [mname, mdata] of Object.entries(netlist.modules)) {
                                if (mdata.ports) {
                                    for (const [pname, pdata] of Object.entries(mdata.ports)) {
                                        pins.push({ name: pname, dir: pdata.direction || 'input', edge: 'bottom', pos: pins.length * 20 + 20 });
                                    }
                                }
                                if (mdata.cells) {
                                    for (const [cname, cdata] of Object.entries(mdata.cells)) {
                                        macros.push({ inst: cname, master: cdata.type, x: 80 + macros.length * 120, y: 80, w: 100, h: 80, orient: 'R0' });
                                    }
                                }
                            }
                        }
                        const initSession = {
                            die: { w: 180, h: 180, ml: 15, mr: 15, mt: 15, mb: 15 },
                            core: { border: 15, w: 150, h: 150 },
                            macros: macros.length > 0 ? macros : [{ inst: 'u_alu_core', master: 'simple_alu', x: 40, y: 40.8, w: 80, h: 60, orient: 'R0' }],
                            pins: pins.length > 0 ? pins.slice(0, 16) : [{ name: 'clk', edge: 'bottom', pos: 30, dir: 'input' }, { name: 'rst_n', edge: 'bottom', pos: 60, dir: 'input' }]
                        };
                        safePostMessage(activeFloorplanPanel, { command: 'loadSession', session: initSession });
                    } catch (e) {}
                }, 400);
            }

            activeFloorplanPanel.webview.onDidReceiveMessage(async (msg) => {
                if (msg && msg.command === 'fpsAction') {
                    const action = msg.action;
                    const projDir = getProjectFsPath(pName);

                    if (action === 'saveSession') {
                        const sessionPath = path.join(projDir, '.silis_fp_session.json');
                        atomicWriteFileSync(sessionPath, JSON.stringify(msg.state || {}, null, 2));
                        vscode.window.showInformationMessage(`Floorplan session saved to ${sessionPath}`);
                    } else if (action === 'exportTcl' || action === 'commitFlow') {
                        const tclPath = path.join(projDir, 'run_floorplan.tcl');
                        if (msg.tcl) {
                            atomicWriteFileSync(tclPath, msg.tcl);
                        }
                        if (action === 'commitFlow') {
                            const term = getFlowTerminal();
                            term.show();
                            term.sendText(`cd "${projDir}" && if [ -f run_floorplan.tcl ]; then openroad -no_init -exit run_floorplan.tcl; fi`);
                        } else {
                            vscode.window.showInformationMessage(`OpenROAD Floorplan TCL exported to ${tclPath}`);
                        }
                    } else if (action === 'autoPlaceMacros') {
                        vscode.window.showInformationMessage('Macro placement calculated. Check FPS canvas or export TCL.');
                    } else if (action === 'runDrcCheck') {
                        const term = getFlowTerminal();
                        term.show();
                        term.sendText(`cd "${projDir}" && verilator --lint-only -sv -Wall -Wno-fatal -Irtl -Isource rtl/*.sv source/*.sv 2>/dev/null || true`);
                    }
                }
            });

            activeFloorplanPanel.onDidDispose(() => {
                activeFloorplanPanel = null;
            });
        })
    );

    // 10c. Open CAD Workbench Dock
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openWorkbench', async () => {
            vscode.commands.executeCommand('silis.openDashboard');
        })
    );

    function getFlowTerminal() {
        let term = vscode.window.terminals.find(t => t.name === 'Silis Flow');
        if (!term) {
            term = vscode.window.createTerminal('Silis Flow');
        }
        return term;
    }

    // 11. Run Simulation (Play Button: Runs compile & sim, opens Schematic on right, loads Waveform dock upon generation)
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.runSimulation', async () => {
            const pName = activeProject || 'demo_soc';
            const projDir = getProjectFsPath(pName);
            const vcdPath = path.join(projDir, 'waveform.vcd');
            const vvpPath = path.join(projDir, 'sim.vvp');

            vscode.window.showInformationMessage(`Running simulation for project "${pName}"...`);

            if (vscode.window.activeTextEditor) {
                await vscode.window.activeTextEditor.document.save();
            }

            // Stale artifact purge: ensure we never read stale waveforms
            try { if (fs.existsSync(vvpPath)) fs.unlinkSync(vvpPath); } catch (e) {}
            try { if (fs.existsSync(vcdPath)) fs.unlinkSync(vcdPath); } catch (e) {}

            _isSimRunning = true;
            syncStateTracker();

            // 1. Show terminal and execute simulation cleanly inside projDir
            const term = getFlowTerminal();
            term.show();
            term.sendText(`cd "${projDir}" && rm -f sim.vvp waveform.vcd && echo "=== [Silis Simulation: ${pName}] ===" && if [ -f tb/tb_${pName}.v ]; then iverilog -g2012 -o sim.vvp rtl/*.sv tb/*.v && vvp sim.vvp; elif [ -f source/tb_chip_top.v ]; then iverilog -g2012 -o sim.vvp source/*.sv source/*.v && vvp sim.vvp; elif [ -f source/tb_${pName}.v ]; then iverilog -g2012 -o sim.vvp source/*.sv source/*.v && vvp sim.vvp; else TB=\$(find . -maxdepth 2 -name "tb_*.v" -o -name "*_tb.v" 2>/dev/null | head -n 1); if [ -n "\$TB" ]; then iverilog -g2012 -o sim.vvp source/*.sv source/*.v rtl/*.sv rtl/*.v "\$TB" && vvp sim.vvp; else echo "[Silis Error] No testbench found in ${projDir}"; fi; fi`);

            // 2. Open schematic beside RTL (Native web render)
            await openSchematicPanel(pName, vscode.ViewColumn.Beside);

            // 3. Poll for sim.vvp and completed waveform.vcd (only when Run is clicked)
            const startTime = Date.now();
            let pollTimer = null;
            pollTimer = setInterval(async () => {
                const elapsed = Date.now() - startTime;
                if (elapsed > 20000) {
                    clearInterval(pollTimer);
                    pollTimer = null;
                    _isSimRunning = false;
                    syncStateTracker();
                    return;
                }

                if (fs.existsSync(vcdPath)) {
                    try {
                        const st = fs.statSync(vcdPath);
                        if (st.size > 32) {
                            const content = fs.readFileSync(vcdPath, 'utf8');
                            if (content.includes('$enddefinitions') || content.includes('$dumpvars') || elapsed > 3500) {
                                clearInterval(pollTimer);
                                pollTimer = null;
                                _isSimRunning = false;
                                syncStateTracker();
                                await vscode.commands.executeCommand('silis-bottom-dock.focus');
                                await loadWaveformInBottomDock(vcdPath);
                            }
                        }
                    } catch (e) {}
                }
            }, 200);
        })
    );

    // Watch for waveform.vcd, netlists, and DEF updates across workspace to sync State Tracker & Waveforms
    try {
        let _vcdDebounce = null;
        const vcdWatcher = vscode.workspace.createFileSystemWatcher('**/waveform.vcd');
        const handleVcdUpdate = (uri) => {
            if (_isSimRunning) return; // Simulation polling owns loading while run is active
            if (_vcdDebounce) clearTimeout(_vcdDebounce);
            _vcdDebounce = setTimeout(async () => {
                try {
                    if (fs.existsSync(uri.fsPath) && fs.statSync(uri.fsPath).size > 32) {
                        await loadWaveformInBottomDock(uri.fsPath);
                        syncStateTracker();
                    }
                } catch (e) {}
            }, 300);
        };
        vcdWatcher.onDidChange(handleVcdUpdate);
        vcdWatcher.onDidCreate(handleVcdUpdate);
        context.subscriptions.push(vcdWatcher);

        const netlistWatcher = vscode.workspace.createFileSystemWatcher('**/{netlist/*_netlist.v,reports/area.rpt}');
        netlistWatcher.onDidChange(() => syncStateTracker());
        netlistWatcher.onDidCreate(() => syncStateTracker());
        context.subscriptions.push(netlistWatcher);

        const schemWatcher = vscode.workspace.createFileSystemWatcher('**/results/structural_*.json');
        schemWatcher.onDidChange(() => syncStateTracker());
        schemWatcher.onDidCreate(() => syncStateTracker());
        context.subscriptions.push(schemWatcher);

        const defWatcher = vscode.workspace.createFileSystemWatcher('**/results/*_filled.def');
        defWatcher.onDidChange(() => syncStateTracker());
        defWatcher.onDidCreate(() => syncStateTracker());
        context.subscriptions.push(defWatcher);
    } catch (e) {}

    // 12. Run Verilator RTL Lint (Debug Button & Ctrl+L)
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.runLint', async () => {
            const pName = activeProject || 'demo_soc';
            const outputChan = vscode.window.createOutputChannel('Silis RTL Lint');
            outputChan.show(true);
            outputChan.appendLine(`[LINT] Running Verilator RTL Lint on project "${pName}" (Ctrl+L)...`);

            const term = getFlowTerminal();
            term.show();
            term.sendText(`verilator --lint-only -sv -Wall -Wno-fatal -Iprojects/${pName}/rtl -Iprojects/${pName}/source projects/${pName}/rtl/*.sv projects/${pName}/rtl/*.v projects/${pName}/source/*.sv projects/${pName}/source/*.v 2>/dev/null || true`);
            outputChan.appendLine(`[LINT] Executed Verilator in Silis Flow terminal for projects/${pName}`);
            setTimeout(syncStateTracker, 1500);
        })
    );

    // 12b. Project State Tracker Focus
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openStateTracker', async () => {
            await vscode.commands.executeCommand('silis-project-tracker.focus');
            syncStateTracker();
        })
    );

    // 12c. Synthesis & Scan Confirmation Tab (100% Silis Parity)
    let synthConfirmPanel = null;

    async function openSynthConfirmPanel(projectName) {
        const pName = (projectName || activeProject || 'demo_soc').replace(/^projects\//, '');
        const wsFolders = vscode.workspace.workspaceFolders;
        const rootPath = wsFolders ? wsFolders[0].uri.fsPath : '';
        const projDir = path.join(rootPath, 'projects', pName);
        const synthYsPath = path.join(projDir, 'synth.ys');

        let scriptText = '';
        if (fs.existsSync(synthYsPath)) {
            try { scriptText = fs.readFileSync(synthYsPath, 'utf8'); } catch (e) {}
        }
        if (!scriptText.trim()) {
            scriptText = `# Silis CS Edition - Project Synthesis Script (${pName})\nread_verilog -sv source/chip_top.sv source/simple_alu.v 2>/dev/null || read_verilog -sv rtl/*.sv 2>/dev/null;\nhierarchy -top ${pName === 'demo_soc' ? 'chip_top' : pName};\nsynth -top ${pName === 'demo_soc' ? 'chip_top' : pName};\ntee -o reports/area.rpt stat;\nwrite_json results/structural_${pName}.json;\nwrite_verilog -noattr netlist/${pName}_netlist.v;\n`;
        }

        let dftConfig = { enabled: false, scan_in: 'scan_in', scan_out: 'scan_out', scan_en: 'scan_en', scan_clk: 'clk', clamp: true };
        const dftCfgPath = path.join(projDir, 'dft_config.json');
        if (fs.existsSync(dftCfgPath)) {
            try { dftConfig = JSON.parse(fs.readFileSync(dftCfgPath, 'utf8')); } catch (e) {}
        }

        if (synthConfirmPanel) {
            synthConfirmPanel.reveal(vscode.ViewColumn.One);
            safePostMessage(synthConfirmPanel, {
                command: 'loadConfig',
                projectName: pName,
                topModule: pName === 'demo_soc' ? 'chip_top' : pName,
                pdk: 'sky130',
                script: scriptText,
                dft: dftConfig
            });
            return synthConfirmPanel;
        }

        synthConfirmPanel = vscode.window.createWebviewPanel(
            'silisSynthConfirm',
            `Confirm Synthesis: ${pName}`,
            vscode.ViewColumn.One,
            { enableScripts: true, retainContextWhenHidden: true }
        );

        synthConfirmPanel.webview.html = getSynthConfirmWebviewHtml(synthConfirmPanel.webview, context.extensionUri);

        const sendConfig = () => {
            safePostMessage(synthConfirmPanel, {
                command: 'loadConfig',
                projectName: pName,
                topModule: pName === 'demo_soc' ? 'chip_top' : pName,
                pdk: 'sky130',
                script: scriptText,
                dft: dftConfig
            });
        };
        setTimeout(sendConfig, 300);

        synthConfirmPanel.webview.onDidReceiveMessage(async (msg) => {
            if (!msg) return;
            if (msg.command === 'synthConfirmReady') {
                sendConfig();
                return;
            }
            if (msg.command === 'cancelSynthesis') {
                if (synthConfirmPanel) {
                    synthConfirmPanel.dispose();
                    synthConfirmPanel = null;
                }
                return;
            }
            if (msg.command === 'confirmSynthesis') {
                try {
                    if (msg.script) {
                        atomicWriteFileSync(synthYsPath, msg.script);
                    }
                    if (msg.dft) {
                        atomicWriteFileSync(dftCfgPath, JSON.stringify(msg.dft, null, 2));
                    }

                    if (synthConfirmPanel) {
                        synthConfirmPanel.dispose();
                        synthConfirmPanel = null;
                    }

                    const term = getFlowTerminal();
                    term.show();
                    term.sendText(`cd "${projDir}" && echo "=== [Silis Synthesis & Scan Flow: ${pName}] ===" && yosys -s synth.ys`);

                    if (msg.dft && msg.dft.enabled) {
                        term.sendText(`cd "${projDir}" && python3 -c "import os, sys; sys.path.insert(0, '.'); sys.path.insert(0, '${rootPath}'); sys.path.insert(0, '${rootPath}/silis'); sys.path.insert(0, '${rootPath}/silis/core'); from dft.dft_engine import DFTEngine; e = DFTEngine('netlist/${pName}_netlist.v', '${pName === 'demo_soc' ? 'chip_top' : pName}', 'sky130', '${msg.dft.scan_in}', '${msg.dft.scan_out}', '${msg.dft.scan_en}', '${msg.dft.scan_clk}', ${msg.dft.clamp ? 'True' : 'False'}); e.stitch_scan_chain('netlist/${pName}_scan.v', 'results/design.scandef')" 2>/dev/null || true`);
                    }

                    _isSynthRunning = true;
                    syncStateTracker();

                    vscode.window.showInformationMessage(`Synthesis flow started for projects/${pName}`);

                    const synthStart = Date.now();
                    const synthPoll = setInterval(() => {
                        const elapsed = Date.now() - synthStart;
                        const netlistV = path.join(projDir, 'netlist', `${pName}_netlist.v`);
                        const areaRpt = path.join(projDir, 'reports', 'area.rpt');
                        const isDone = (fs.existsSync(netlistV) && fs.statSync(netlistV).size > 0) || (fs.existsSync(areaRpt) && fs.statSync(areaRpt).size > 0);
                        if (isDone || elapsed > 30000) {
                            clearInterval(synthPoll);
                            _isSynthRunning = false;
                            syncStateTracker();
                        }
                    }, 500);
                } catch (err) {
                    vscode.window.showErrorMessage(`Failed to initiate synthesis: ${err.message}`);
                }
            }
        });

        synthConfirmPanel.onDidDispose(() => {
            synthConfirmPanel = null;
        });

        return synthConfirmPanel;
    }

    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openSynthConfirm', (projectName) => {
            openSynthConfirmPanel(projectName);
        })
    );

    // 13. Flow Execution (Synthesize directs to Confirm dialog)
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.runSynthesis', (projectName) => {
            openSynthConfirmPanel(projectName);
        })
    );

    context.subscriptions.push(
        vscode.commands.registerCommand('silis.runAll', () => {
            const pName = activeProject || 'demo_soc';
            const projDir = getProjectFsPath(pName);
            const term = getFlowTerminal();
            term.show();
            term.sendText(`cd "${projDir}" && if [ -f run_init.tcl ]; then openroad -no_init -exit run_init.tcl; else echo "No run_init.tcl found in ${projDir}"; fi`);
        })
    );

    context.subscriptions.push(
        vscode.commands.registerCommand('silis.refreshReadme', async () => {
            await refreshReadmeNative(activeProject);
            vscode.window.showInformationMessage('Silis: README.md project table updated successfully.');
        })
    );

    // Auto-open Silis ASIC Studio Dashboard on first boot
    setTimeout(() => {
        try {
            vscode.commands.executeCommand('silis.openDashboard');
        } catch (e) {}
    }, 800);
}

// HTTP API helper using node http/https
function getBottomDockIdleHtml() {
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <style>
        body, html { margin: 0; padding: 0; width: 100%; height: 100%; display: flex; align-items: center; justify-content: center; background: var(--vscode-editor-background); color: var(--vscode-descriptionForeground); font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; font-size: 13px; }
        .box { text-align: center; max-width: 440px; padding: 24px; }
        p { margin: 0 0 8px 0; }
        .hint { font-size: 11px; opacity: 0.8; }
    </style>
</head>
<body>
    <div class="box">
        <p><strong>ROHD Waveform Inspector Idle</strong></p>
        <p class="hint">Click <strong>Run Simulation (Play)</strong> to compile RTL and stream interactive VCD waveforms into this dock.</p>
    </div>
</body>
</html>`;
}

function getDashboardHtml(currentActive) {
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; color: var(--vscode-foreground); background: var(--vscode-editor-background); padding: 28px 36px; max-width: 900px; margin: 0 auto; line-height: 1.5; }
        h1 { font-size: 20px; font-weight: 700; margin: 0 0 6px 0; }
        .sub { font-size: 13px; color: var(--vscode-descriptionForeground); margin-bottom: 20px; }
        .active-info { font-size: 15px; font-weight: 600; }
        .active-pdk { font-size: 12px; color: var(--vscode-textLink-foreground); font-family: monospace; margin-left: 8px; }
        .btn-group { display: flex; gap: 8px; flex-wrap: wrap; }
        button { padding: 6px 14px; background: var(--vscode-button-secondaryBackground, #21262d); color: var(--vscode-button-secondaryForeground, #c9d1d9); border: 1px solid var(--vscode-button-border, #30363d); border-radius: 4px; cursor: pointer; font-size: 12px; font-weight: 500; font-family: inherit; }
        button:hover { background: var(--vscode-button-secondaryHoverBackground, #30363d); }
        button.primary { background: var(--vscode-button-background, #1f6feb); color: var(--vscode-button-foreground, #ffffff); font-weight: 600; border: none; }
        button.primary:hover { background: var(--vscode-button-hoverBackground, #388bfd); opacity: 0.95; }
        table { width: 100%; border-collapse: collapse; font-size: 12px; }
        th { text-align: left; padding: 8px 10px; color: var(--vscode-descriptionForeground); border-bottom: 1px solid var(--vscode-editorWidget-border); font-weight: 600; }
        td { padding: 10px; border-bottom: 1px solid var(--vscode-editorWidget-border); }
        tr:hover td { background: var(--vscode-list-hoverBackground); }
        .tag { font-size: 10px; padding: 2px 6px; border-radius: 3px; font-family: monospace; }
        .tag.ready { background: #23863633; color: #3fb950; }
        .tag.none { background: var(--vscode-editorWidget-border, #30363d); color: var(--vscode-descriptionForeground); }
        .flow-bar { display: flex; gap: 10px; margin-top: 10px; }
    </style>
</head>
<body>
    <h1>SILIS ASIC Studio</h1>
    <div class="sub">Native Physical Design & Multi-Domain Power Studio for VS Code Web</div>

    <div class="card">
        <div class="card-title">Active Silicon Design</div>
        <div class="active-row">
            <div>
                <span class="active-info" id="active-name">${currentActive || 'demo_soc'}</span>
                <span class="active-pdk" id="active-pdk">PDK: sky130</span>
            </div>
            <div class="btn-group">
                <button onclick="msg('openSchematic')">Schematic (DigitalJS)</button>
                <button onclick="msg('openWaveforms')">Waveforms (ROHD)</button>
                <button onclick="msg('openFloorplanner')">Floorplan Studio (FPS)</button>
                <button onclick="msg('openLayout')">2D Layout (Peeker)</button>
            </div>
        </div>
        <div class="flow-bar">
            <button class="primary" onclick="msg('runSynthesis')">Run Synthesis (Yosys)</button>
            <button class="primary" onclick="msg('runAll')">Run Tapeout Flow (OpenROAD)</button>
        </div>
    </div>

    <div class="card">
        <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:12px;">
            <div class="card-title" style="margin-bottom:0;">Workspace Designs</div>
            <button class="primary" onclick="msg('newProject')">+ Create New Silis Project</button>
        </div>
        <table>
            <thead>
                <tr>
                    <th>Project</th>
                    <th>Process (PDK)</th>
                    <th>Top Module</th>
                    <th>Layout (DEF)</th>
                    <th>In-Tab Actions</th>
                </tr>
            </thead>
            <tbody id="proj-tbody">
                <tr><td colspan="5" style="color:var(--vscode-descriptionForeground);">Loading designs...</td></tr>
            </tbody>
        </table>
    </div>

    <script>
        const vscode = acquireVsCodeApi();

        function msg(cmd, data) {
            vscode.postMessage({ command: cmd, ...data });
        }

        function renderDashboardProjects(projs) {
            const tbody = document.getElementById('proj-tbody');
            if (!Array.isArray(projs) || projs.length === 0) {
                tbody.innerHTML = '<tr><td colspan="5" style="color:var(--vscode-descriptionForeground);">No designs found. Create your first project above.</td></tr>';
                return;
            }
            tbody.innerHTML = projs.map(p => {
                const isAct = p.is_active;
                const defTag = p.has_def ? '<span class="tag ready">Available</span>' : '<span class="tag none">Pending PnR</span>';
                return '<tr>' +
                    '<td><strong>' + p.name + '</strong> ' + (isAct ? '<span class="tag ready" style="margin-left:4px;">ACTIVE</span>' : '') + '</td>' +
                    '<td><code>' + p.pdk + '</code></td>' +
                    '<td><code>' + p.top_module + '</code></td>' +
                    '<td>' + defTag + '</td>' +
                    '<td>' +
                        '<button onclick="msg(\\'openProject\\', {project:\\'' + p.name + '\\'})">Select</button> ' +
                        '<button onclick="msg(\\'openSchematic\\', {project:\\'' + p.name + '\\'})">Schem</button> ' +
                        '<button onclick="msg(\\'openWaveforms\\', {project:\\'' + p.name + '\\'})">Waves</button> ' +
                        '<button onclick="msg(\\'openFloorplanner\\', {project:\\'' + p.name + '\\'})">FPS</button> ' +
                        '<button onclick="msg(\\'openLayout\\', {project:\\'' + p.name + '\\'})">Layout</button>' +
                    '</td>' +
                '</tr>';
            }).join('');

            const act = projs.find(p => p.is_active);
            if (act) {
                document.getElementById('active-name').textContent = act.name;
                document.getElementById('active-pdk').textContent = 'PDK: ' + act.pdk;
            }
        }

        function refresh() {
            msg('getProjects');
        }

        window.addEventListener('message', e => {
            if (e.data && (e.data.type === 'refresh' || e.data.type === 'projectsLoaded')) {
                if (e.data.projects) renderDashboardProjects(e.data.projects);
                else refresh();
            }
        });

        refresh();
        setInterval(refresh, 5000);
    </script>
</body>
</html>`;
}

// -----------------------------------------------------------------------------
// Authentic 6-Stage Silis Project Creation Wizard HTML
// -----------------------------------------------------------------------------
function getProjectWizardHtml(workspaceFiles) {
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; color: var(--vscode-foreground); background: var(--vscode-editor-background); padding: 28px; max-width: 680px; margin: 0 auto; line-height: 1.5; }
        .stepper { display: flex; gap: 8px; margin-bottom: 24px; border-bottom: 1px solid var(--vscode-editorWidget-border); padding-bottom: 12px; }
        .step-num { width: 22px; height: 22px; border-radius: 50%; background: var(--vscode-editorWidget-border); color: var(--vscode-descriptionForeground); display: inline-flex; align-items: center; justify-content: center; font-size: 11px; font-weight: bold; margin-right: 6px; }
        .step-item { font-size: 12px; color: var(--vscode-descriptionForeground); display: flex; align-items: center; }
        .step-item.active { color: var(--vscode-textLink-foreground); font-weight: 600; }
        .step-item.active .step-num { background: var(--vscode-textLink-foreground); color: var(--vscode-editor-background); }
        .step-content { display: none; }
        .step-content.active { display: block; }
        h2 { font-size: 16px; margin: 0 0 4px 0; }
        .step-desc { font-size: 12px; color: var(--vscode-descriptionForeground); margin-bottom: 20px; }
        label { display: block; font-size: 12px; font-weight: 600; margin-bottom: 6px; }
        input, select { width: 100%; box-sizing: border-box; padding: 8px 10px; margin-bottom: 16px; background: var(--vscode-input-background); color: var(--vscode-input-foreground); border: 1px solid var(--vscode-input-border, #30363d); border-radius: 4px; font-size: 13px; font-family: inherit; }
        input:focus, select:focus { outline: none; border-color: var(--vscode-focusBorder); }
        .hint { font-size: 11px; color: var(--vscode-descriptionForeground); margin-top: -12px; margin-bottom: 16px; }
        .card-box { background: var(--vscode-editorWidget-background, #161b22); border: 1px solid var(--vscode-editorWidget-border, #30363d); border-radius: 6px; padding: 14px; margin-bottom: 16px; }
        .nav-footer { display: flex; justify-content: space-between; margin-top: 24px; border-top: 1px solid var(--vscode-editorWidget-border); padding-top: 16px; }
        button { padding: 8px 16px; border-radius: 4px; cursor: pointer; font-size: 12px; font-weight: 600; border: 1px solid var(--vscode-editorWidget-border); background: var(--vscode-button-secondaryBackground, #21262d); color: var(--vscode-button-secondaryForeground, #c9d1d9); }
        button:hover { background: var(--vscode-button-secondaryHoverBackground, #30363d); }
        button.primary { background: var(--vscode-button-background, #1f6feb); color: var(--vscode-button-foreground, #ffffff); border: none; }
        button.primary:hover { background: var(--vscode-button-hoverBackground, #388bfd); opacity: 0.95; }
        .btn-row { display: flex; gap: 8px; margin-bottom: 12px; }
        .file-list { background: var(--vscode-input-background); border: 1px solid var(--vscode-editorWidget-border); border-radius: 4px; min-height: 80px; max-height: 150px; overflow-y: auto; padding: 6px; margin-bottom: 12px; font-family: monospace; font-size: 12px; }
        .file-entry { display: flex; justify-content: space-between; align-items: center; padding: 4px 6px; border-bottom: 1px solid var(--vscode-editorWidget-border); }
        .file-entry:last-child { border-bottom: none; }
        .del-btn { color: #f85149; cursor: pointer; font-weight: bold; margin-left: 8px; }
        .browse-row { display: flex; gap: 8px; margin-bottom: 16px; }
        .browse-row input { margin-bottom: 0; }
        .browse-row button { white-space: nowrap; }
        #status-msg { margin-top: 16px; padding: 12px; border-radius: 4px; font-size: 12px; display: none; }
        #status-msg.success { background: #23863633; color: #3fb950; border: 1px solid #238636; }
        #status-msg.error { background: #da363333; color: #f85149; border: 1px solid #f85149; }
        .catalog-container { display: flex; gap: 10px; margin-top: 8px; margin-bottom: 12px; height: 260px; border: 1px solid var(--vscode-editorWidget-border); border-radius: 6px; overflow: hidden; background: var(--vscode-editorWidget-background); }
        .catalog-list { width: 40%; border-right: 1px solid var(--vscode-editorWidget-border); overflow-y: auto; padding: 6px; }
        .catalog-item { padding: 6px 8px; border-radius: 4px; cursor: pointer; font-size: 11px; margin-bottom: 4px; border: 1px solid transparent; }
        .catalog-item:hover { background: var(--vscode-list-hoverBackground); }
        .catalog-item.selected { background: var(--vscode-list-activeSelectionBackground); color: var(--vscode-list-activeSelectionForeground); border-color: var(--vscode-focusBorder); }
        .item-name { font-weight: 600; font-family: monospace; font-size: 11px; word-break: break-all; }
        .item-meta { font-size: 10px; opacity: 0.8; margin-top: 2px; }
        .catalog-preview { width: 60%; display: flex; flex-direction: column; padding: 10px; overflow-y: auto; font-size: 11px; }
        .preview-header { font-weight: 600; font-size: 12px; margin-bottom: 4px; color: var(--vscode-textLink-foreground); }
        .preview-dims { font-size: 10px; opacity: 0.8; margin-bottom: 8px; font-family: monospace; }
        .preview-pins { font-size: 10px; line-height: 1.4; margin-bottom: 8px; background: var(--vscode-input-background); padding: 6px 8px; border-radius: 4px; border: 1px solid var(--vscode-editorWidget-border); max-height: 70px; overflow-y: auto; }
        .stub-box { flex: 1; min-height: 80px; max-height: 110px; font-family: monospace; font-size: 10px; background: var(--vscode-input-background); color: var(--vscode-input-foreground); border: 1px solid var(--vscode-editorWidget-border); border-radius: 4px; padding: 6px; white-space: pre; overflow: auto; user-select: text; }
        .cart-summary { margin-top: 10px; padding: 10px 12px; background: var(--vscode-input-background); border: 1px solid var(--vscode-editorWidget-border); border-radius: 6px; margin-bottom: 12px; }
        .cart-header { font-size: 12px; font-weight: 600; display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px; }
        .badge { background: var(--vscode-badge-background, #21262d); color: var(--vscode-badge-foreground, #c9d1d9); font-size: 10px; padding: 2px 6px; border-radius: 10px; }
        .cart-items { display: flex; flex-wrap: wrap; gap: 6px; }
        .cart-pill { display: inline-flex; align-items: center; gap: 6px; padding: 4px 8px; background: var(--vscode-editorWidget-background); border: 1px solid var(--vscode-editorWidget-border); border-radius: 4px; font-size: 11px; font-family: monospace; }
        .cart-del { cursor: pointer; color: #f85149; font-weight: bold; }
    </style>
</head>
<body>
    <div class="stepper">
        <div class="step-item active" id="st-1"><span class="step-num">1</span>Project</div>
        <div class="step-item" id="st-2"><span class="step-num">2</span>RTL Sources</div>
        <div class="step-item" id="st-3"><span class="step-num">3</span>Testbenches</div>
        <div class="step-item" id="st-4"><span class="step-num">4</span>Constraints</div>
        <div class="step-item" id="st-5"><span class="step-num">5</span>Power &amp; IO</div>
        <div class="step-item" id="st-6"><span class="step-num">6</span>Silicon Target</div>
    </div>

    <!-- Step 1: Project Details (Page 1) -->
    <div class="step-content active" id="sp-1">
        <h2>Project Details</h2>
        <div class="step-desc">Specify the project name and location in the workspace.</div>

        <label for="p-name">Project Name</label>
        <input id="p-name" type="text" placeholder="e.g. riscv_core, full_adder" maxlength="64" autofocus>
        <div class="hint">Alphanumeric characters and underscores only. Must start with a letter.</div>

        <label for="p-loc">Target Directory</label>
        <input id="p-loc" type="text" readonly value="projects/<project_name>">
    </div>

    <!-- Step 2: RTL Sources (Page 2 - 100% Silis Desktop Parity) -->
    <div class="step-content" id="sp-2">
        <h2>Add Sources</h2>
        <div class="step-desc">Select RTL files (.v, .sv, .vhd) from your system to add to the project.</div>

        <div class="btn-row">
            <button onclick="browseFiles('rtl', false)">+ Add RTL Files...</button>
            <button onclick="browseFiles('rtl', true)">+ Add Folder...</button>
            <button onclick="createNewFile('rtl')">+ Create New File...</button>
        </div>

        <label>Selected RTL Sources:</label>
        <div class="file-list" id="rtl-list"><div style="color:var(--vscode-descriptionForeground); padding:8px;">No files selected. Click 'Add RTL Files' to select from your system or 'Create New File'.</div></div>

        <div class="card-box">
            <label style="font-weight:normal; display:flex; align-items:center; gap:8px; margin:0; cursor:pointer;">
                <input type="checkbox" id="copy-sources" checked style="width:auto; margin:0;">
                <span>Copy sources into project directory</span>
            </label>
        </div>
    </div>

    <!-- Step 3: Testbench Sources (Page 3 - 6-Stage Wizard Parity) -->
    <div class="step-content" id="sp-3">
        <h2>Add Testbench Sources</h2>
        <div class="step-desc">Select testbench files (.v, .sv, .vhd, .vhdl) or directory from your system to verify the design.</div>

        <div class="btn-row">
            <button onclick="browseFiles('tb', false)">+ Add TB Files...</button>
            <button onclick="browseFiles('tb', true)">+ Add TB Folder...</button>
            <button onclick="createNewFile('tb')">+ Create New File...</button>
        </div>

        <label>Selected Testbench Sources:</label>
        <div class="file-list" id="tb-list"><div style="color:var(--vscode-descriptionForeground); padding:8px;">No testbench files selected. Default testbench template matching top RTL module will be auto-generated.</div></div>

        <label for="tb-top">Top Testbench Module Name</label>
        <input id="tb-top" type="text" placeholder="e.g. tb or <project_name>_tb">
        <div class="hint">Top-level verification testbench module identifier. Defaults to tb or &lt;project_name&gt;_tb.</div>

        <div class="card-box">
            <label style="font-weight:normal; display:flex; align-items:center; gap:8px; margin:0; cursor:pointer;">
                <input type="checkbox" id="copy-tb-sources" checked style="width:auto; margin:0;">
                <span>Copy testbench sources into project directory (tb/)</span>
            </label>
        </div>
    </div>

    <!-- Step 4: Constraints (Page 4 - 100% Silis Desktop Parity) -->
    <div class="step-content" id="sp-4">
        <h2>Add Constraints</h2>
        <div class="step-desc">Add SDC timing constraints for synthesis and physical design.</div>

        <div class="btn-row">
            <button onclick="browseFiles('sdc', false)">+ Add SDC File...</button>
            <button onclick="createNewFile('sdc')">+ Create New File...</button>
        </div>

        <label>Selected SDC Files:</label>
        <div class="file-list" id="sdc-list"><div style="color:var(--vscode-descriptionForeground); padding:8px;">No SDC files selected. Default 100MHz (10.0ns) clock will be auto-generated.</div></div>

        <label for="c-period">Clock Period (ns) for Auto SDC</label>
        <input id="c-period" type="number" value="10.0" min="0.5" max="1000.0" step="0.5">
    </div>

    <!-- Step 5: Additional Design Intent (Page 5 - 100% Silis Desktop Parity) -->
    <div class="step-content" id="sp-5">
        <h2>Additional Design Intent</h2>
        <div class="step-desc">Configure power gating (UPF/CPF) and physical I/O pin ordering.</div>

        <label>Power Intent (IEEE 1801 UPF / CPF)</label>
        <div class="browse-row">
            <input type="text" id="p-upf" placeholder="Optional: Browse existing .upf or .cpf file...">
            <button onclick="browseSingle('upf')">Browse...</button>
        </div>
        <div class="hint">If omitted, baseline default template (PD_TOP) is generated automatically.</div>

        <label>I/O Pin &amp; Pad Ordering (.cfg / .io)</label>
        <div class="browse-row">
            <input type="text" id="p-io" placeholder="Optional: Browse existing pin_order.cfg or .io file...">
            <button onclick="browseSingle('io')">Browse...</button>
        </div>
        <div class="hint">If omitted, baseline perimeter edge ordering is generated automatically.</div>
    </div>

    <!-- Step 6: Silicon Target & Top Module (Page 6 - Multi-PDK Parity) -->
    <div class="step-content" id="sp-6">
        <h2>Silicon Target &amp; Top Module</h2>
        <div class="step-desc">Select target semiconductor process kit, top module, and add macros from the IP Catalog.</div>

        <label for="p-pdk">Process Design Kit (PDK)</label>
        <select id="p-pdk" onchange="fetchMacrosForPdk()">
            <option value="sky130A">SkyWater 130nm High-Density (sky130A / sky130_fd_sc_hd)</option>
            <option value="sky130B">SkyWater 130nm Production (sky130B)</option>
            <option value="gf180mcu">GlobalFoundries 180nm MCU (gf180mcu / gf180mcuC)</option>
            <option value="asap7">ASAP 7nm FinFET (asap7)</option>
        </select>

        <label for="p-top">Top Module Name</label>
        <input id="p-top" type="text" placeholder="e.g. full_adder">
        <div class="hint">Top-level Verilog module identifier. Defaults to project name.</div>

        <!-- Macro IP Catalog (Shopping Cart) -->
        <label>Macro IP Catalog (Shopping Cart)</label>
        <div class="hint">Select macros (SRAMs, ALUs, Accelerators) to include in your chip floorplan.</div>

        <input type="text" id="macro-search" placeholder="Search macros (e.g. sram, alu, crypto)..." oninput="filterMacroList()">

        <div class="catalog-container">
            <div class="catalog-list" id="macro-list-view">
                <div style="padding:10px; font-size:11px; opacity:0.7;">Loading macro catalog...</div>
            </div>
            <div class="catalog-preview" id="macro-preview-view">
                <div style="padding:20px; text-align:center; opacity:0.7; font-size:12px;">Select a macro from the catalog to preview pinout and black-box stub.</div>
            </div>
        </div>

        <div class="cart-summary">
            <div class="cart-header">
                <span>Shopping Cart Checkout</span>
                <span class="badge" id="cart-badge">0 item(s)</span>
            </div>
            <div class="cart-items" id="cart-items-view">
                <span style="font-size:11px; opacity:0.7;">Cart is empty. Select macros above and click "Add to Shopping Cart" or proceed without macros.</span>
            </div>
        </div>
    </div>

    <div id="status-msg"></div>

    <div class="nav-footer">
        <button id="btn-prev" onclick="prevStep()" style="display:none;">Back</button>
        <div style="flex:1;"></div>
        <button id="btn-next" class="primary" onclick="nextStep()">Next</button>
        <button id="btn-finish" class="primary" onclick="submitProject()" style="display:none;">Initialize Project</button>
    </div>

    <script>
        const vscode = acquireVsCodeApi();
        let currentStep = 1;
        const selectedFiles = { rtl: [], tb: [], sdc: [] };
        let allMacros = [];
        let filteredMacros = [];
        let selectedMacro = null;
        const cartMacros = [];

        document.getElementById('p-name').addEventListener('input', e => {
            const val = e.target.value.trim();
            document.getElementById('p-loc').value = 'projects/' + (val || '<project_name>');
            if (!document.getElementById('p-top').value || document.getElementById('p-top').dataset.touched !== '1') {
                document.getElementById('p-top').value = val;
            }
            if (!document.getElementById('tb-top').value || document.getElementById('tb-top').dataset.touched !== '1') {
                document.getElementById('tb-top').value = val ? (val + '_tb') : '';
            }
        });
        document.getElementById('p-top').addEventListener('input', () => {
            document.getElementById('p-top').dataset.touched = '1';
        });
        document.getElementById('tb-top').addEventListener('input', () => {
            document.getElementById('tb-top').dataset.touched = '1';
        });

        function fetchAvailablePdks() {
            vscode.postMessage({ command: 'fetchPdks' });
        }

        function fetchMacrosForPdk() {
            const pdk = document.getElementById('p-pdk').value;
            vscode.postMessage({ command: 'fetchMacros', pdk: pdk });
        }

        function filterMacroList() {
            const q = (document.getElementById('macro-search').value || '').toLowerCase().trim();
            filteredMacros = allMacros.filter(m => !q || m.name.toLowerCase().includes(q) || (m.category && m.category.toLowerCase().includes(q)));
            renderMacroListView();
        }

        function renderMacroListView() {
            const container = document.getElementById('macro-list-view');
            if (filteredMacros.length === 0) {
                container.innerHTML = '<div style="padding:10px; font-size:11px; opacity:0.7;">No matching macros found.</div>';
                return;
            }
            container.innerHTML = filteredMacros.map((m, idx) => {
                const isSel = selectedMacro && selectedMacro.name === m.name;
                return '<div class="catalog-item' + (isSel ? ' selected' : '') + '" onclick="selectMacroByIndex(' + idx + ')">' +
                    '<div class="item-name">' + m.name + '</div>' +
                    '<div class="item-meta">' + (m.category || 'Macro') + ' | ' + m.width + 'x' + m.height + 'µm</div>' +
                '</div>';
            }).join('');
        }

        function selectMacroByIndex(idx) {
            selectedMacro = filteredMacros[idx];
            renderMacroListView();
            renderMacroPreviewView();
        }

        function renderMacroPreviewView() {
            const pv = document.getElementById('macro-preview-view');
            if (!selectedMacro) {
                pv.innerHTML = '<div style="padding:20px; text-align:center; opacity:0.7; font-size:12px;">Select a macro from the catalog to preview pinout and black-box stub.</div>';
                return;
            }
            const m = selectedMacro;
            const pins = m.pins || {};
            const inCount = (pins.input || []).length;
            const outCount = (pins.output || []).length;
            const pwrCount = (pins.power || []).length + (pins.ground || []).length;

            const inPinsStr = (pins.input || []).slice(0, 8).join(', ') + (inCount > 8 ? ' ... (+' + (inCount - 8) + ' more)' : '');
            const outPinsStr = (pins.output || []).slice(0, 8).join(', ') + (outCount > 8 ? ' ... (+' + (outCount - 8) + ' more)' : '');

            const inCart = cartMacros.some(c => c.name === m.name);

            pv.innerHTML =
                '<div class="preview-header">' + m.name + ' (' + (m.category || 'Macro') + ')</div>' +
                '<div class="preview-dims">Physical Footprint: ' + m.width + 'µm × ' + m.height + 'µm</div>' +
                '<div class="preview-pins">' +
                    '<div><strong>Inputs (' + inCount + '):</strong> ' + (inPinsStr || 'None') + '</div>' +
                    '<div><strong>Outputs (' + outCount + '):</strong> ' + (outPinsStr || 'None') + '</div>' +
                    '<div><strong>Power/Ground:</strong> ' + ((pins.power || []).concat(pins.ground || [])).join(', ') + '</div>' +
                '</div>' +
                '<label style="font-size:10px; margin-bottom:4px;">Verilog Black-Box Stub Preview:</label>' +
                '<div class="stub-box">' + (m.stub || '// No stub available') + '</div>' +
                '<div style="display:flex; justify-content:space-between; align-items:center; margin-top:8px;">' +
                    '<label style="margin:0; font-size:11px; display:inline-flex; align-items:center; gap:6px; cursor:pointer;">' +
                        '<input type="checkbox" id="cb-auto-stub" checked style="width:auto; margin:0;">' +
                        '<span>Auto-Generate Verilog Stub on Checkout</span>' +
                    '</label>' +
                    (inCart
                        ? '<button onclick="removeFromCartByName(\\'' + m.name + '\\')">Remove from Cart</button>'
                        : '<button class="primary" onclick="addSelectedToCart()">+ Add to Shopping Cart</button>') +
                '</div>';
        }

        function addSelectedToCart() {
            if (!selectedMacro) return;
            if (cartMacros.some(c => c.name === selectedMacro.name)) return;
            const autoStub = document.getElementById('cb-auto-stub') ? document.getElementById('cb-auto-stub').checked : true;
            cartMacros.push({
                name: selectedMacro.name,
                category: selectedMacro.category,
                width: selectedMacro.width,
                height: selectedMacro.height,
                lef_path: selectedMacro.lef_path,
                stub: selectedMacro.stub,
                auto_stub: autoStub
            });
            renderCartView();
            renderMacroPreviewView();
        }

        function removeFromCartByName(name) {
            const idx = cartMacros.findIndex(c => c.name === name);
            if (idx !== -1) {
                cartMacros.splice(idx, 1);
                renderCartView();
                renderMacroPreviewView();
            }
        }

        function removeCartItem(idx) {
            cartMacros.splice(idx, 1);
            renderCartView();
            renderMacroPreviewView();
        }

        function renderCartView() {
            const count = cartMacros.length;
            document.getElementById('cart-badge').textContent = count + ' item(s)';
            const container = document.getElementById('cart-items-view');
            if (count === 0) {
                container.innerHTML = '<span style="font-size:11px; opacity:0.7;">Cart is empty. Select macros above and click "Add to Shopping Cart" or proceed without macros.</span>';
                return;
            }
            container.innerHTML = cartMacros.map((m, i) =>
                '<div class="cart-pill">' +
                    '<span>' + m.name + ' (' + m.width + 'x' + m.height + 'µm)</span>' +
                    '<span class="cart-del" onclick="removeCartItem(' + i + ')">&times;</span>' +
                '</div>'
            ).join('');
        }

        function browseFiles(target, isFolder) {
            const isHDL = (target === 'rtl' || target === 'tb');
            vscode.postMessage({
                command: 'browseFiles',
                target: target,
                canSelectFiles: !isFolder,
                canSelectFolders: isFolder,
                canSelectMany: true,
                title: isFolder ? 'Select Directory' : 'Select Source Files',
                filters: isHDL ? { 'HDL Files': ['v', 'sv', 'vhd', 'vhdl'], 'All Files': ['*'] } : { 'SDC Files': ['sdc'], 'All Files': ['*'] }
            });
        }

        function browseSingle(target) {
            vscode.postMessage({
                command: 'browseFiles',
                target: target,
                canSelectFiles: true,
                canSelectFolders: false,
                canSelectMany: false,
                title: 'Select File',
                filters: target === 'upf' ? { 'Power Intent': ['upf', 'cpf'], 'All Files': ['*'] } : { 'Pin Config': ['cfg', 'io', 'tcl'], 'All Files': ['*'] }
            });
        }

        function createNewFile(target) {
            const ext = (target === 'rtl' || target === 'tb') ? '.v' : '.sdc';
            const promptName = target === 'rtl' ? 'module.v' : (target === 'tb' ? 'tb.v' : 'timing.sdc');
            const name = prompt('Enter filename (e.g. ' + promptName + '):');
            if (name && name.trim()) {
                let fname = name.trim();
                if (!fname.endsWith(ext) && !fname.endsWith('.sv') && !fname.endsWith('.vhd') && !fname.endsWith('.vhdl')) fname += ext;
                selectedFiles[target].push('[NEW] ' + fname);
                renderFileList(target);
            }
        }

        function removeFile(target, idx) {
            selectedFiles[target].splice(idx, 1);
            renderFileList(target);
        }

        function renderFileList(target) {
            const container = document.getElementById(target + '-list');
            const list = selectedFiles[target];
            if (list.length === 0) {
                container.innerHTML = '<div style="color:var(--vscode-descriptionForeground); padding:8px;">No files selected.</div>';
                return;
            }
            container.innerHTML = list.map((f, i) =>
                '<div class="file-entry">' +
                    '<span>' + f + '</span>' +
                    '<span class="del-btn" onclick="removeFile(\\'' + target + '\\', ' + i + ')">&times;</span>' +
                '</div>'
            ).join('');
        }

        function showStep(s) {
            currentStep = s;
            for (let i = 1; i <= 6; i++) {
                document.getElementById('sp-' + i).className = 'step-content' + (i === s ? ' active' : '');
                document.getElementById('st-' + i).className = 'step-item' + (i === s ? ' active' : '');
            }
            document.getElementById('btn-prev').style.display = s > 1 ? 'inline-block' : 'none';
            document.getElementById('btn-next').style.display = s < 6 ? 'inline-block' : 'none';
            document.getElementById('btn-finish').style.display = s === 6 ? 'inline-block' : 'none';
            if (s === 6 && allMacros.length === 0) {
                fetchMacrosForPdk();
            }
        }

        function nextStep() {
            if (currentStep === 1) {
                const name = document.getElementById('p-name').value.trim();
                if (!/^[a-zA-Z][a-zA-Z0-9_]{0,63}$/.test(name)) {
                    alert('Project name must begin with a letter and contain only alphanumeric characters and underscores.');
                    return;
                }
            }
            if (currentStep < 6) showStep(currentStep + 1);
        }

        function prevStep() {
            if (currentStep > 1) showStep(currentStep - 1);
        }

        function submitProject() {
            const name = document.getElementById('p-name').value.trim();
            const top = document.getElementById('p-top').value.trim() || name;
            const topTb = document.getElementById('tb-top').value.trim() || (top + '_tb');
            const pdk = document.getElementById('p-pdk').value;
            const period = parseFloat(document.getElementById('c-period').value) || 10.0;
            const upf = document.getElementById('p-upf').value.trim();
            const io = document.getElementById('p-io').value.trim();

            document.getElementById('status-msg').className = 'success';
            document.getElementById('status-msg').style.display = 'block';
            document.getElementById('status-msg').innerHTML = 'Initializing Silis project scaffold...';

            vscode.postMessage({
                command: 'createProject',
                data: {
                    name: name,
                    top_module: top,
                    top_tb: topTb,
                    pdk: pdk,
                    clock_period: period,
                    rtl_files: selectedFiles.rtl,
                    tb_files: selectedFiles.tb,
                    sdc_files: selectedFiles.sdc,
                    upf_file: upf,
                    io_file: io,
                    macros: cartMacros
                }
            });
        }

        window.addEventListener('message', e => {
            const msg = e.data;
            if (msg.type === 'pdksLoaded' && Array.isArray(msg.pdks) && msg.pdks.length > 0) {
                const sel = document.getElementById('p-pdk');
                const curVal = sel.value;
                sel.innerHTML = msg.pdks.map(p => {
                    const isSel = (p.id === curVal) || (curVal === 'sky130_fd_sc_hd' && p.id === 'sky130A');
                    const badge = (p.installed === false) ? ' (Available)' : '';
                    return '<option value="' + p.id + '"' + (isSel ? ' selected' : '') + '>' + (p.display_name || p.name || p.id) + badge + '</option>';
                }).join('');
            } else if (msg.type === 'macrosLoaded') {
                allMacros = msg.macros || [];
                filterMacroList();
                if (allMacros.length > 0 && !selectedMacro) {
                    selectMacroByIndex(0);
                }
            } else if (msg.type === 'filesSelected') {
                if (msg.target === 'upf') {
                    if (msg.files.length > 0) document.getElementById('p-upf').value = msg.files[0];
                } else if (msg.target === 'io') {
                    if (msg.files.length > 0) document.getElementById('p-io').value = msg.files[0];
                } else if (selectedFiles[msg.target]) {
                    msg.files.forEach(f => {
                        if (!selectedFiles[msg.target].includes(f)) selectedFiles[msg.target].push(f);
                    });
                    renderFileList(msg.target);
                }
            } else if (msg.type === 'created') {
                const el = document.getElementById('status-msg');
                el.className = 'success';
                el.style.display = 'block';
                el.innerHTML = '<strong>Project Created Successfully</strong><br>Project <code>' + msg.name + '</code> is now active. RTL source opened in editor.';
            } else if (msg.type === 'error') {
                const el = document.getElementById('status-msg');
                el.className = 'error';
                el.style.display = 'block';
                el.textContent = 'Error: ' + msg.message;
            }
        });
        fetchAvailablePdks();
        fetchMacrosForPdk();
    </script>
</body>
</html>`;
}
function getSidebarHtml(currentActive) {
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, monospace; color: var(--vscode-foreground); padding: 12px; font-size: 13px; line-height: 1.5; }
        h3 { font-size: 11px; text-transform: uppercase; letter-spacing: 1px; color: var(--vscode-sideBarTitle-foreground); margin: 16px 0 8px 0; }
        button { display: block; width: 100%; margin-bottom: 6px; padding: 7px 12px; background: var(--vscode-button-secondaryBackground, #21262d); color: var(--vscode-button-secondaryForeground, #c9d1d9); border: 1px solid var(--vscode-button-border, #30363d); border-radius: 4px; text-align: left; cursor: pointer; font-size: 12px; }
        button:hover { background: var(--vscode-button-secondaryHoverBackground, #30363d); }
        button.primary { background: var(--vscode-button-background, #1f6feb); color: var(--vscode-button-foreground, #ffffff); font-weight: 600; border: none; }
        button.primary:hover { background: var(--vscode-button-hoverBackground, #388bfd); opacity: 0.95; }
        .card { background: var(--vscode-editorWidget-background, #161b22); border: 1px solid var(--vscode-editorWidget-border, #30363d); border-radius: 6px; padding: 10px; margin-bottom: 12px; }
        .meta { font-size: 11px; color: var(--vscode-descriptionForeground); }
        .project-item { padding: 6px 8px; margin-bottom: 4px; border-left: 2px solid var(--vscode-button-border, #30363d); border-radius: 2px; cursor: pointer; background: var(--vscode-editor-background); }
        .project-item:hover { border-left-color: var(--vscode-button-background, #1f6feb); background: var(--vscode-list-hoverBackground); }
        .project-item.active { border-left-color: #3fb950; background: var(--vscode-list-activeSelectionBackground, #21262d); }
        .project-name { font-weight: 600; font-size: 12px; }
        .project-meta { font-size: 10px; color: var(--vscode-descriptionForeground); }
        #active-label { color: #3fb950; font-weight: 600; }
    </style>
</head>
<body>
    <div class="card">
        <strong>Silis CS Edition</strong>
        <div class="meta">Active: <span id="active-label">${currentActive || 'loading...'}</span></div>
    </div>

    <button class="primary" onclick="vscode.postMessage({command:'openDashboard'})">ASIC Studio Dashboard</button>
    <button class="primary" onclick="vscode.postMessage({command:'newProject'})">+ New Silis Project</button>

    <h3>Workspace Designs</h3>
    <div id="project-list"><div class="meta">Loading designs...</div></div>

    <h3>Active Design CAD Viewers</h3>
    <button onclick="vscode.postMessage({command:'openSchematic'})">Schematic (DigitalJS)</button>
    <button onclick="vscode.postMessage({command:'openWaveforms'})">Waveforms (Bottom Panel)</button>
    <button onclick="vscode.postMessage({command:'openFloorplanner'})">Floorplan Studio (FPS)</button>
    <button onclick="vscode.postMessage({command:'openLayout'})">2D Layout (Peeker)</button>

    <h3>Flow Execution</h3>
    <button onclick="vscode.postMessage({command:'runSynthesis'})">Run Synthesis (Yosys)</button>
    <button onclick="vscode.postMessage({command:'runAll'})">Run Tapeout Flow (OpenROAD)</button>

    <script>
        const vscode = acquireVsCodeApi();

        function renderSidebarProjects(projects) {
            const container = document.getElementById('project-list');
            if (!container) return;
            if (!Array.isArray(projects) || projects.length === 0) {
                container.innerHTML = '<div class="meta">No projects found.</div>';
                return;
            }
            container.innerHTML = projects.map(p => {
                const isActive = p.is_active ? 'active' : '';
                return '<div class="project-item ' + isActive + '" onclick="selectProject(\\'' + p.name + '\\')">' +
                    '<div class="project-name">' + p.name + '</div>' +
                    '<div class="project-meta">' + p.pdk + ' &middot; ' + p.top_module + '</div>' +
                '</div>';
            }).join('');

            const activeObj = projects.find(p => p.is_active);
            if (activeObj) {
                document.getElementById('active-label').textContent = activeObj.name;
            }
        }

        window.addEventListener('message', event => {
            const msg = event.data;
            if (msg && msg.type === 'activeProjectChanged' && msg.active) {
                document.getElementById('active-label').textContent = msg.active;
            }
            if (msg && msg.type === 'projectsLoaded' && Array.isArray(msg.projects)) {
                renderSidebarProjects(msg.projects);
            }
        });

        function loadProjects() {
            vscode.postMessage({ command: 'getProjects' });
        }

        function selectProject(name) {
            vscode.postMessage({ command: 'openProject', project: name });
        }

        loadProjects();
        setInterval(loadProjects, 5000);
    </script>
</body>
</html>`;
}

function deactivate() {}

module.exports = {
    activate,
    deactivate
};
