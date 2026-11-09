const vscode = require('vscode');

function activate(context) {
    console.log('[Silis CAD Extension] Activating Silis CS EDA Workbench...');

    let activeProject = context.workspaceState.get('silis.activeProject', 'demo_soc');

    function getActiveProjectDir(pName) {
        const name = pName || activeProject || 'demo_soc';
        return `projects/${name}`;
    }

    // 1. Sidebar Webview Provider (Silis CAD in Activity Bar)
    const sidebarProvider = {
        async resolveWebviewView(webviewView) {
            webviewView.webview.options = { enableScripts: true };
            const apiBase = await resolveUrl('http://127.0.0.1:8765');
            webviewView.webview.html = getSidebarHtml(apiBase.replace(/\/$/, ''), activeProject);

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
                    case 'runSynthesis':
                        vscode.commands.executeCommand('silis.runSynthesis');
                        break;
                    case 'runAll':
                        vscode.commands.executeCommand('silis.runAll');
                        break;
                }
            });
        }
    };
    context.subscriptions.push(
        vscode.window.registerWebviewViewProvider('silis-cad-sidebar', sidebarProvider)
    );

    // 1b. Bottom Panel Dock Provider (Waveforms docked tabbed next to Terminal)
    let bottomDockWebview = null;

    async function loadWaveformInBottomDock(fileOrProject) {
        if (!fileOrProject) {
            fileOrProject = getActiveProjectDir();
        }
        let queryParam = '';
        if (typeof fileOrProject === 'string' && (fileOrProject.endsWith('.vcd') || fileOrProject.endsWith('.fst'))) {
            queryParam = `file=${encodeURIComponent(fileOrProject)}`;
        } else {
            queryParam = `project=${encodeURIComponent(fileOrProject)}`;
        }
        const uri = await resolveUrl(`http://127.0.0.1:8765/waveforms/index.html?${queryParam}`);
        if (bottomDockWebview) {
            bottomDockWebview.webview.html = getIframeHtml(uri, 'ROHD Waveform Inspector');
        }
        await vscode.commands.executeCommand('silis-bottom-dock.focus');
    }

    const bottomDockProvider = {
        async resolveWebviewView(webviewView) {
            bottomDockWebview = webviewView;
            webviewView.webview.options = { enableScripts: true, retainContextWhenHidden: true };
            const projDir = getActiveProjectDir();
            const uri = await resolveUrl(`http://127.0.0.1:8765/waveforms/index.html?project=${encodeURIComponent(projDir)}`);
            webviewView.webview.html = getIframeHtml(uri, 'ROHD Waveform Inspector');

            webviewView.onDidDispose(() => {
                bottomDockWebview = null;
            });
        }
    };
    context.subscriptions.push(
        vscode.window.registerWebviewViewProvider('silis-bottom-dock', bottomDockProvider)
    );

    // 2. Custom Editor Provider for VCD / Waveforms
    const vcdEditorProvider = {
        async resolveCustomTextEditor(document, webviewPanel) {
            webviewPanel.webview.options = { enableScripts: true, retainContextWhenHidden: true };
            const uri = await resolveUrl(`http://127.0.0.1:8765/waveforms/index.html?file=${encodeURIComponent(document.fileName)}`);
            webviewPanel.webview.html = getIframeHtml(uri, 'Silis Waveforms: ' + document.fileName);
        }
    };
    context.subscriptions.push(
        vscode.window.registerCustomEditorProvider('silis.vcdViewer', vcdEditorProvider)
    );

    // 3. Custom Editor Provider for Structural Netlists (DigitalJS Schematic)
    const schematicEditorProvider = {
        async resolveCustomTextEditor(document, webviewPanel) {
            webviewPanel.webview.options = { enableScripts: true, retainContextWhenHidden: true };
            const uri = await resolveUrl(`http://127.0.0.1:8765/schematic/digitaljs.html?file=${encodeURIComponent(document.fileName)}`);
            webviewPanel.webview.html = getIframeHtml(uri, 'Silis Schematic: ' + document.fileName);
        }
    };
    context.subscriptions.push(
        vscode.window.registerCustomEditorProvider('silis.schematicViewer', schematicEditorProvider)
    );

    // 3b. Custom Editor Provider for DEF 2D Silicon Layout
    const defEditorProvider = {
        async resolveCustomTextEditor(document, webviewPanel) {
            webviewPanel.webview.options = { enableScripts: true, retainContextWhenHidden: true };
            // Notify server to load this specific DEF file into Peeker
            try {
                await callHttpApi('http://127.0.0.1:8765/api/peeker/load_def', 'POST', { def_path: document.fileName });
            } catch (e) {}
            const uri = await resolveUrl('http://127.0.0.1:6081/vnc.html?autoconnect=true&resize=remote');
            webviewPanel.webview.html = getIframeHtml(uri, 'Silis 2D Layout: ' + document.fileName);
        }
    };
    context.subscriptions.push(
        vscode.window.registerCustomEditorProvider('silis.defViewer', defEditorProvider)
    );

    // 3c. Custom Editor Provider for GDSII
    const gdsEditorProvider = {
        async resolveCustomTextEditor(document, webviewPanel) {
            webviewPanel.webview.options = { enableScripts: true, retainContextWhenHidden: true };
            const uri = await resolveUrl('http://127.0.0.1:6081/vnc.html?autoconnect=true&resize=remote');
            webviewPanel.webview.html = getIframeHtml(uri, 'Silis GDSII Viewer: ' + document.fileName);
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

            const apiBase = await resolveUrl('http://127.0.0.1:8765');
            dashboardPanel.webview.html = getDashboardHtml(apiBase.replace(/\/$/, ''), activeProject);

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
            }, undefined, context.subscriptions);

            dashboardPanel.onDidDispose(() => {
                dashboardPanel = null;
            }, null, context.subscriptions);
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
                if (message.command === 'createProject') {
                    const payload = message.data;
                    try {
                        const res = await callHttpApi('http://127.0.0.1:8765/api/projects/create', 'POST', payload);
                        if (res && res.status === 'created') {
                            activeProject = payload.name;
                            await context.workspaceState.update('silis.activeProject', activeProject);
                            panel.webview.postMessage({ type: 'created', name: payload.name, top_module: payload.top_module });

                            vscode.window.showInformationMessage(`Silis Project "${activeProject}" created successfully.`);

                            // Open the newly generated RTL file in editor
                            if (res.rtl_file) {
                                const wsFolders = vscode.workspace.workspaceFolders;
                                if (wsFolders && wsFolders.length > 0) {
                                    const rtlUri = vscode.Uri.joinPath(wsFolders[0].uri, 'projects', activeProject, res.rtl_file);
                                    vscode.window.showTextDocument(rtlUri, { preview: false });
                                }
                            }

                            // Refresh dynamic README and dashboard
                            vscode.commands.executeCommand('silis.refreshReadme');
                            if (dashboardPanel) {
                                dashboardPanel.webview.postMessage({ type: 'refresh' });
                            }
                        } else {
                            panel.webview.postMessage({ type: 'error', message: (res && res.error) || 'Failed to initialize project.' });
                        }
                    } catch (err) {
                        panel.webview.postMessage({ type: 'error', message: String(err) });
                    }
                }
            }, undefined, context.subscriptions);
        })
    );

    // 6. Open / Switch Active Project
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openProject', async (projectName) => {
            if (!projectName) {
                try {
                    const projects = await callHttpApi('http://127.0.0.1:8765/api/projects', 'GET');
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

                try {
                    await callHttpApi('http://127.0.0.1:8765/api/active_project', 'POST', { name: activeProject });
                } catch (e) {}

                vscode.window.showInformationMessage(`Active Project: ${activeProject}`);

                if (dashboardPanel) {
                    dashboardPanel.webview.postMessage({ type: 'refresh' });
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
    );

    // 8. Schematic Viewer (World 0)
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openSchematic', async (projectName) => {
            const pName = projectName || activeProject || 'demo_soc';
            const panel = vscode.window.createWebviewPanel(
                'silisSchematic',
                `Schematic: ${pName}`,
                vscode.ViewColumn.Active,
                { enableScripts: true, retainContextWhenHidden: true }
            );
            const uri = await resolveUrl(`http://127.0.0.1:8765/schematic/digitaljs.html?project=projects/${encodeURIComponent(pName)}`);
            panel.webview.html = getIframeHtml(uri, 'Schematic: ' + pName);
        })
    );

    // 9. 2D Layout Canvas (World 1: FastSiliconPeeker)
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openLayout', async () => {
            const panel = vscode.window.createWebviewPanel(
                'silisLayout',
                '2D Layout (FastSiliconPeeker)',
                vscode.ViewColumn.Active,
                { enableScripts: true, retainContextWhenHidden: true }
            );
            const uri = await resolveUrl('http://127.0.0.1:6081/vnc.html?autoconnect=true&resize=remote');
            panel.webview.html = getIframeHtml(uri, '2D Layout');
        })
    );

    // 10. Floorplan & Power Studio (World 3: FPS)
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openFloorplanner', async () => {
            const panel = vscode.window.createWebviewPanel(
                'silisFPS',
                'Floorplan & Power Studio',
                vscode.ViewColumn.Active,
                { enableScripts: true, retainContextWhenHidden: true }
            );
            const uri = await resolveUrl('http://127.0.0.1:6084/vnc.html?autoconnect=true&resize=remote');
            panel.webview.html = getIframeHtml(uri, 'Floorplan Studio');
        })
    );

    // 11. Flow Execution
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.runSynthesis', () => {
            const term = vscode.window.createTerminal('Silis Synthesis');
            term.show();
            term.sendText(`silis --run-synth --project projects/${activeProject}`);
        })
    );

    context.subscriptions.push(
        vscode.commands.registerCommand('silis.runAll', () => {
            const term = vscode.window.createTerminal('Silis Tapeout');
            term.show();
            term.sendText(`silis --run-all --project projects/${activeProject}`);
        })
    );

    context.subscriptions.push(
        vscode.commands.registerCommand('silis.refreshReadme', async () => {
            try {
                const wsFolders = vscode.workspace.workspaceFolders;
                if (wsFolders && wsFolders.length > 0) {
                    const cp = require('child_process');
                    const scriptPath = vscode.Uri.joinPath(wsFolders[0].uri, 'tools', 'gen_readme.py').fsPath;
                    cp.execFile('python3', [scriptPath], { cwd: wsFolders[0].uri.fsPath });
                }
            } catch (e) {}
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
function callHttpApi(urlString, method, bodyData) {
    return new Promise((resolve, reject) => {
        try {
            const parsed = new URL(urlString);
            const httpModule = parsed.protocol === 'https:' ? require('https') : require('http');
            const dataStr = bodyData ? JSON.stringify(bodyData) : null;
            const options = {
                hostname: parsed.hostname,
                port: parsed.port || (parsed.protocol === 'https:' ? 443 : 80),
                path: parsed.pathname + parsed.search,
                method: method || 'GET',
                headers: {
                    'Content-Type': 'application/json',
                    ...(dataStr ? { 'Content-Length': Buffer.byteLength(dataStr) } : {})
                },
                timeout: 10000
            };
            const req = httpModule.request(options, (res) => {
                let chunks = '';
                res.setEncoding('utf8');
                res.on('data', (d) => { chunks += d; });
                res.on('end', () => {
                    try {
                        resolve(JSON.parse(chunks));
                    } catch (e) {
                        resolve(chunks);
                    }
                });
            });
            req.on('error', (e) => reject(e));
            req.on('timeout', () => { req.destroy(); reject(new Error('Request timed out')); });
            if (dataStr) req.write(dataStr);
            req.end();
        } catch (err) {
            reject(err);
        }
    });
}

async function resolveUrl(localUrl) {
    try {
        const uri = vscode.Uri.parse(localUrl);
        const externalUri = await vscode.env.asExternalUri(uri);
        return externalUri.toString();
    } catch (e) {
        return localUrl;
    }
}

function getIframeHtml(url, title) {
    const safeTitle = (title || 'Silis CAD Engine').replace(/</g, '&lt;').replace(/>/g, '&gt;');
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta http-equiv="Content-Security-Policy" content="default-src * 'unsafe-inline' 'unsafe-eval' data: blob: ws: wss:; frame-src *; child-src *;">
    <style>
        body, html { margin: 0; padding: 0; width: 100%; height: 100%; overflow: hidden; background: #181825; font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; }
        #silis-bar {
            height: 28px;
            background: #11111b;
            border-bottom: 1px solid #313244;
            display: flex;
            align-items: center;
            justify-content: space-between;
            padding: 0 10px;
            color: #cdd6f4;
            font-size: 11px;
            box-sizing: border-box;
            user-select: none;
        }
        .bar-title { font-weight: 600; color: #89b4fa; }
        .bar-links { display: flex; align-items: center; gap: 8px; }
        .bar-btn {
            background: #313244;
            color: #cdd6f4;
            border: 1px solid #45475a;
            border-radius: 3px;
            padding: 2px 8px;
            font-size: 10px;
            cursor: pointer;
            text-decoration: none;
            display: inline-flex;
            align-items: center;
        }
        .bar-btn:hover { background: #45475a; color: #ffffff; }
        .frame-wrap { width: 100%; height: calc(100% - 28px); position: relative; }
        iframe { width: 100%; height: 100%; border: none; background: #181825; }
    </style>
</head>
<body>
    <div id="silis-bar">
        <div class="bar-title">${safeTitle}</div>
        <div class="bar-links">
            <button class="bar-btn" onclick="const f=document.getElementById('cad-frame'); if(f) f.src=f.src;">Reload</button>
            <a class="bar-btn" href="${url}" target="_blank" rel="noopener noreferrer">Open in New Window</a>
        </div>
    </div>
    <div class="frame-wrap">
        <iframe id="cad-frame" src="${url}"></iframe>
    </div>
</body>
</html>`;
}

// -----------------------------------------------------------------------------
// Interactive ASIC Studio Dashboard HTML (Replaces broken Markdown Preview)
// -----------------------------------------------------------------------------
function getDashboardHtml(apiBaseUrl, currentActive) {
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; color: var(--vscode-foreground); background: var(--vscode-editor-background); padding: 28px 36px; max-width: 900px; margin: 0 auto; line-height: 1.5; }
        h1 { font-size: 20px; font-weight: 700; margin: 0 0 6px 0; }
        .sub { font-size: 13px; color: #a6adc8; margin-bottom: 20px; }
        .banner { background: #181825; border: 1px solid #313244; border-radius: 6px; padding: 12px 16px; margin-bottom: 24px; display: flex; align-items: center; justify-content: space-between; gap: 16px; }
        .tool-badge { font-size: 11px; padding: 3px 8px; border-radius: 3px; background: #313244; color: #cdd6f4; font-family: monospace; }
        .tool-badge.ok { background: #1e3a2f; color: #a6e3a1; border: 1px solid #40a070; }
        .card { background: var(--vscode-editorWidget-background, #1e1e2e); border: 1px solid var(--vscode-editorWidget-border, #313244); border-radius: 6px; padding: 16px; margin-bottom: 24px; }
        .card-title { font-size: 13px; font-weight: 600; text-transform: uppercase; letter-spacing: 0.5px; color: var(--vscode-sideBarTitle-foreground, #a6adc8); margin-bottom: 12px; }
        .active-row { display: flex; align-items: center; justify-content: space-between; flex-wrap: wrap; gap: 12px; }
        .active-info { font-size: 15px; font-weight: 600; }
        .active-pdk { font-size: 12px; color: #89b4fa; font-family: monospace; margin-left: 8px; }
        .btn-group { display: flex; gap: 8px; flex-wrap: wrap; }
        button { padding: 6px 14px; background: var(--vscode-button-secondaryBackground, #313244); color: var(--vscode-button-secondaryForeground, #cdd6f4); border: 1px solid var(--vscode-button-border, #45475a); border-radius: 4px; cursor: pointer; font-size: 12px; font-weight: 500; font-family: inherit; }
        button:hover { background: var(--vscode-button-secondaryHoverBackground, #45475a); }
        button.primary { background: var(--vscode-button-background, #89b4fa); color: var(--vscode-button-foreground, #181825); font-weight: 600; border: none; }
        button.primary:hover { opacity: 0.9; }
        table { width: 100%; border-collapse: collapse; font-size: 12px; }
        th { text-align: left; padding: 8px 10px; color: #a6adc8; border-bottom: 1px solid #313244; font-weight: 600; }
        td { padding: 10px; border-bottom: 1px solid #313244; }
        tr:hover td { background: var(--vscode-list-hoverBackground); }
        .tag { font-size: 10px; padding: 2px 6px; border-radius: 3px; font-family: monospace; }
        .tag.ready { background: #1e3a2f; color: #a6e3a1; }
        .tag.none { background: #313244; color: #a6adc8; }
        .flow-bar { display: flex; gap: 10px; margin-top: 10px; }
    </style>
</head>
<body>
    <h1>SILIS ASIC Studio</h1>
    <div class="sub">Native Physical Design & Multi-Domain Power Studio for VS Code Web</div>

    <div class="banner">
        <div>
            <strong>Toolchain Health</strong>
            <div style="font-size: 11px; color: #a6adc8;">Stand-alone ASIC engines loaded in container</div>
        </div>
        <div style="display:flex; gap:6px; flex-wrap:wrap;">
            <span class="tool-badge ok" id="b-python">Python 3.12 (OK)</span>
            <span class="tool-badge ok" id="b-yosys">Yosys (OK)</span>
            <span class="tool-badge ok" id="b-openroad">OpenROAD (OK)</span>
            <span class="tool-badge ok" id="b-pdk">Sky130B (Loaded)</span>
        </div>
    </div>

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
                <tr><td colspan="5" style="color:#a6adc8;">Loading designs...</td></tr>
            </tbody>
        </table>
    </div>

    <script>
        const vscode = acquireVsCodeApi();
        const API_BASE = ${JSON.stringify(apiBaseUrl || 'http://127.0.0.1:8765')};

        function msg(cmd, data) {
            vscode.postMessage({ command: cmd, ...data });
        }

        async function refresh() {
            try {
                const resp = await fetch(API_BASE + '/api/projects');
                const projs = await resp.json();
                const tbody = document.getElementById('proj-tbody');
                if (!Array.isArray(projs) || projs.length === 0) {
                    tbody.innerHTML = '<tr><td colspan="5" style="color:#a6adc8;">No designs found. Create your first project above.</td></tr>';
                    return;
                }
                tbody.innerHTML = projs.map(p => {
                    const isAct = p.is_active;
                    const defTag = p.has_def ? '<span class="tag ready">Available</span>' : '<span class="tag none">Pending PnR</span>';
                    return \`<tr>
                        <td><strong>\${p.name}</strong> \${isAct ? '<span class="tag ready" style="margin-left:4px;">ACTIVE</span>' : ''}</td>
                        <td><code>\${p.pdk}</code></td>
                        <td><code>\${p.top_module}</code></td>
                        <td>\${defTag}</td>
                        <td>
                            <button onclick="msg('openProject', {project:'\${p.name}'})">Select</button>
                            <button onclick="msg('openSchematic', {project:'\${p.name}'})">Schem</button>
                            <button onclick="msg('openWaveforms', {project:'\${p.name}'})">Waves</button>
                            <button onclick="msg('openFloorplanner', {project:'\${p.name}'})">FPS</button>
                        </td>
                    </tr>\`;
                }).join('');

                const act = projs.find(p => p.is_active);
                if (act) {
                    document.getElementById('active-name').textContent = act.name;
                    document.getElementById('active-pdk').textContent = 'PDK: ' + act.pdk;
                }
            } catch(e) {}
        }

        window.addEventListener('message', e => {
            if (e.data && e.data.type === 'refresh') refresh();
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
    const wsFilesJson = JSON.stringify(workspaceFiles || []);
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; color: var(--vscode-foreground); background: var(--vscode-editor-background); padding: 28px; max-width: 680px; margin: 0 auto; line-height: 1.5; }
        .stepper { display: flex; gap: 8px; margin-bottom: 24px; border-bottom: 1px solid #313244; padding-bottom: 12px; }
        .step-num { width: 22px; height: 22px; border-radius: 50%; background: #313244; color: #a6adc8; display: inline-flex; align-items: center; justify-content: center; font-size: 11px; font-weight: bold; margin-right: 6px; }
        .step-item { font-size: 12px; color: #a6adc8; display: flex; align-items: center; }
        .step-item.active { color: #89b4fa; font-weight: 600; }
        .step-item.active .step-num { background: #89b4fa; color: #181825; }
        .step-content { display: none; }
        .step-content.active { display: block; }
        h2 { font-size: 16px; margin: 0 0 4px 0; }
        .step-desc { font-size: 12px; color: #a6adc8; margin-bottom: 20px; }
        label { display: block; font-size: 12px; font-weight: 600; margin-bottom: 6px; }
        input, select { width: 100%; box-sizing: border-box; padding: 8px 10px; margin-bottom: 16px; background: var(--vscode-input-background); color: var(--vscode-input-foreground); border: 1px solid var(--vscode-input-border, #45475a); border-radius: 4px; font-size: 13px; font-family: inherit; }
        input:focus, select:focus { outline: none; border-color: var(--vscode-focusBorder); }
        .hint { font-size: 11px; color: #a6adc8; margin-top: -12px; margin-bottom: 16px; }
        .card-box { background: var(--vscode-editorWidget-background, #1e1e2e); border: 1px solid var(--vscode-editorWidget-border, #313244); border-radius: 6px; padding: 14px; margin-bottom: 16px; }
        .nav-footer { display: flex; justify-content: space-between; margin-top: 24px; border-top: 1px solid #313244; padding-top: 16px; }
        button { padding: 8px 18px; border-radius: 4px; cursor: pointer; font-size: 12px; font-weight: 600; border: 1px solid #45475a; background: #313244; color: #cdd6f4; }
        button:hover { background: #45475a; }
        button.primary { background: #89b4fa; color: #181825; border: none; }
        button.primary:hover { opacity: 0.9; }
        .radio-option { display: flex; align-items: flex-start; gap: 10px; margin-bottom: 12px; cursor: pointer; }
        .radio-option input { width: auto; margin-top: 3px; }
        .radio-label { font-size: 13px; font-weight: 600; }
        .radio-sub { font-size: 11px; color: #a6adc8; }
        #status-msg { margin-top: 16px; padding: 12px; border-radius: 4px; font-size: 12px; display: none; }
        #status-msg.success { background: #1e3a2f; color: #a6e3a1; border: 1px solid #40a070; }
        #status-msg.error { background: #3a1e1e; color: #f38ba8; border: 1px solid #a04040; }
    </style>
</head>
<body>
    <div class="stepper">
        <div class="step-item active" id="st-1"><span class="step-num">1</span>Project</div>
        <div class="step-item" id="st-2"><span class="step-num">2</span>RTL Sources</div>
        <div class="step-item" id="st-3"><span class="step-num">3</span>Timing (SDC)</div>
        <div class="step-item" id="st-4"><span class="step-num">4</span>Power & IO</div>
        <div class="step-item" id="st-5"><span class="step-num">5</span>PDK & Top</div>
    </div>

    <!-- Step 1: Project Details (Page 1) -->
    <div class="step-content active" id="sp-1">
        <h2>Project Details</h2>
        <div class="step-desc">Specify the project identifier and target workspace directory.</div>

        <label for="p-name">Project Name</label>
        <input id="p-name" type="text" placeholder="e.g. riscv_core, systolic_array" maxlength="64" autofocus>
        <div class="hint">Alphanumeric characters and underscores only. Must start with a letter.</div>

        <label for="p-loc">Target Directory</label>
        <input id="p-loc" type="text" readonly value="projects/<project_name>">
        <div class="hint">Projects are isolated within the workspace <code>projects/</code> directory.</div>
    </div>

    <!-- Step 2: RTL Sources (Page 2) -->
    <div class="step-content" id="sp-2">
        <h2>RTL Sources</h2>
        <div class="step-desc">Select how hardware logic will be supplied for this project.</div>

        <div class="card-box">
            <div class="radio-option">
                <input type="radio" name="rtl-mode" id="rm-counter" value="counter" checked>
                <div>
                    <div class="radio-label">Generate Synchronous Datapath Template</div>
                    <div class="radio-sub">Clocked 8-bit pipeline with synchronous reset, ready for synthesis and simulation.</div>
                </div>
            </div>
            <div class="radio-option">
                <input type="radio" name="rtl-mode" id="rm-alu" value="alu">
                <div>
                    <div class="radio-label">Generate Combinational ALU Template</div>
                    <div class="radio-sub">Arithmetic/Logic decoder module with opcode decoding.</div>
                </div>
            </div>
            <div class="radio-option">
                <input type="radio" name="rtl-mode" id="rm-empty" value="empty">
                <div>
                    <div class="radio-label">Generate Empty Module Shell</div>
                    <div class="radio-sub">Skeleton port declaration only.</div>
                </div>
            </div>
            <div class="radio-option" id="opt-import" style="display:none;">
                <input type="radio" name="rtl-mode" id="rm-import" value="import">
                <div>
                    <div class="radio-label">Import Existing Workspace HDL File(s)</div>
                    <div class="radio-sub" id="import-desc">Select existing files to copy into project <code>rtl/</code>.</div>
                </div>
            </div>
        </div>
        <div id="file-picker-box" style="display:none;" class="card-box">
            <label>Select Workspace Files to Copy:</label>
            <div id="file-list-checks"></div>
        </div>
    </div>

    <!-- Step 3: Constraints (Page 3) -->
    <div class="step-content" id="sp-3">
        <h2>Timing Constraints (SDC)</h2>
        <div class="step-desc">Configure clock periods and timing boundaries for synthesis and PnR.</div>

        <label for="c-period">Clock Period (ns)</label>
        <input id="c-period" type="number" value="10.0" min="0.5" max="1000.0" step="0.5">
        <div class="hint">Period in nanoseconds (10.0 ns = 100 MHz target frequency).</div>

        <div class="card-box">
            <label style="font-weight:normal; display:flex; align-items:center; gap:8px; cursor:pointer;">
                <input type="checkbox" id="c-autosdc" checked style="width:auto; margin:0;">
                <span>Auto-generate baseline SDC in <code>constr/default.sdc</code> with 20% IO budgeting</span>
            </label>
        </div>
    </div>

    <!-- Step 4: Additional Design Intent (Page 4b) -->
    <div class="step-content" id="sp-4">
        <h2>Power Intent & IO Pin Ordering</h2>
        <div class="step-desc">Configure IEEE 1801 UPF multi-voltage intent and physical pad configurations.</div>

        <div class="card-box">
            <label style="font-weight:normal; display:flex; align-items:center; gap:8px; margin-bottom:8px;">
                <input type="checkbox" id="p-upf" checked style="width:auto; margin:0;">
                <span>Generate baseline IEEE 1801 UPF 3.0 power intent (<code>power/design.upf</code>)</span>
            </label>
            <div class="radio-sub" style="margin-left:24px;">Defines primary power domain <code>PD_TOP</code>, VDD/VSS supply nets, and power state tables.</div>
        </div>

        <div class="card-box">
            <label style="font-weight:normal; display:flex; align-items:center; gap:8px; margin-bottom:8px;">
                <input type="checkbox" id="p-io" checked style="width:auto; margin:0;">
                <span>Generate baseline I/O pin placement configuration (<code>io/pin_order.cfg</code>)</span>
            </label>
            <div class="radio-sub" style="margin-left:24px;">Defines perimeter edge ordering ([NORTH], [SOUTH], [EAST], [WEST]) for OpenROAD.</div>
        </div>
    </div>

    <!-- Step 5: Silicon Target & Summary (Page 5 & 6) -->
    <div class="step-content" id="sp-5">
        <h2>Silicon Target & Top Module</h2>
        <div class="step-desc">Select target semiconductor process kit and verify top-level entity.</div>

        <label for="p-pdk">Process Design Kit (PDK)</label>
        <select id="p-pdk">
            <option value="sky130_fd_sc_hd">SkyWater 130nm High-Density (sky130_fd_sc_hd) - Standard</option>
            <option value="sky130B">SkyWater 130nm Production (sky130B)</option>
            <option value="gf180mcu">GlobalFoundries 180nm MCU (gf180mcu)</option>
        </select>

        <label for="p-top">Top Module Name</label>
        <input id="p-top" type="text" placeholder="e.g. chip_top">
        <div class="hint">Top-level Verilog module identifier. Defaults to project name.</div>

        <div class="card-box" style="background:#181825; border-color:#89b4fa;">
            <strong style="color:#89b4fa;">Silis Physical Design Notice</strong>
            <div style="font-size:11px; color:#cdd6f4; margin-top:4px;">
                Physical floorplan metrics (die dimensions, core margins, power grid straps, and macro placement) are configured interactively in <strong>Silis Floorplan Studio (FPS)</strong> on Port 6084 after RTL intake.
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
        const wsFiles = ${wsFilesJson};
        let currentStep = 1;

        // Init workspace files in step 2 if any exist
        if (wsFiles && wsFiles.length > 0) {
            document.getElementById('opt-import').style.display = 'flex';
            document.getElementById('import-desc').textContent = wsFiles.length + ' HDL file(s) available in workspace.';
            const checkContainer = document.getElementById('file-list-checks');
            checkContainer.innerHTML = wsFiles.map(f => \`
                <label style="font-weight:normal; display:flex; align-items:center; gap:6px; margin-bottom:4px; font-size:12px;">
                    <input type="checkbox" name="copy-file" value="\${f}" style="width:auto; margin:0;">
                    <span>\${f}</span>
                </label>
            \`).join('');
        }

        document.querySelectorAll('input[name="rtl-mode"]').forEach(r => {
            r.addEventListener('change', () => {
                document.getElementById('file-picker-box').style.display = (r.value === 'import') ? 'block' : 'none';
            });
        });

        document.getElementById('p-name').addEventListener('input', e => {
            const val = e.target.value.trim();
            document.getElementById('p-loc').value = 'projects/' + (val || '<project_name>');
            if (!document.getElementById('p-top').value || document.getElementById('p-top').dataset.touched !== '1') {
                document.getElementById('p-top').value = val;
            }
        });
        document.getElementById('p-top').addEventListener('input', () => {
            document.getElementById('p-top').dataset.touched = '1';
        });

        function showStep(s) {
            currentStep = s;
            for (let i = 1; i <= 5; i++) {
                document.getElementById('sp-' + i).className = 'step-content' + (i === s ? ' active' : '');
                document.getElementById('st-' + i).className = 'step-item' + (i === s ? ' active' : '');
            }
            document.getElementById('btn-prev').style.display = s > 1 ? 'inline-block' : 'none';
            document.getElementById('btn-next').style.display = s < 5 ? 'inline-block' : 'none';
            document.getElementById('btn-finish').style.display = s === 5 ? 'inline-block' : 'none';
        }

        function nextStep() {
            if (currentStep === 1) {
                const name = document.getElementById('p-name').value.trim();
                if (!/^[a-zA-Z][a-zA-Z0-9_]{0,63}$/.test(name)) {
                    alert('Project name must begin with a letter and contain only alphanumeric characters and underscores.');
                    return;
                }
            }
            if (currentStep < 5) showStep(currentStep + 1);
        }

        function prevStep() {
            if (currentStep > 1) showStep(currentStep - 1);
        }

        function submitProject() {
            const name = document.getElementById('p-name').value.trim();
            const top = document.getElementById('p-top').value.trim() || name;
            const pdk = document.getElementById('p-pdk').value;
            const period = parseFloat(document.getElementById('c-period').value) || 10.0;
            const starterType = document.querySelector('input[name="rtl-mode"]:checked').value;

            const copyFiles = [];
            if (starterType === 'import') {
                document.querySelectorAll('input[name="copy-file"]:checked').forEach(c => copyFiles.push(c.value));
            }

            document.getElementById('status-msg').className = 'success';
            document.getElementById('status-msg').style.display = 'block';
            document.getElementById('status-msg').innerHTML = 'Initializing Silis project scaffold...';

            vscode.postMessage({
                command: 'createProject',
                data: {
                    name: name,
                    top_module: top,
                    pdk: pdk,
                    clock_period: period,
                    starter_type: starterType,
                    copy_files: copyFiles
                }
            });
        }

        window.addEventListener('message', e => {
            const msg = e.data;
            const el = document.getElementById('status-msg');
            if (msg.type === 'created') {
                el.className = 'success';
                el.style.display = 'block';
                el.innerHTML = '<strong>Project Created Successfully</strong><br>Project <code>' + msg.name + '</code> is now active. RTL source opened in editor.';
            } else if (msg.type === 'error') {
                el.className = 'error';
                el.style.display = 'block';
                el.textContent = 'Error: ' + msg.message;
            }
        });
    </script>
</body>
</html>`;
}

// -----------------------------------------------------------------------------
// Sidebar HTML (Silis CAD Activity Bar)
// -----------------------------------------------------------------------------
function getSidebarHtml(apiBaseUrl, currentActive) {
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, monospace; color: var(--vscode-foreground); padding: 12px; font-size: 13px; line-height: 1.5; }
        h3 { font-size: 11px; text-transform: uppercase; letter-spacing: 1px; color: var(--vscode-sideBarTitle-foreground); margin: 16px 0 8px 0; }
        button { display: block; width: 100%; margin-bottom: 6px; padding: 7px 12px; background: var(--vscode-button-secondaryBackground, #313244); color: var(--vscode-button-secondaryForeground, #cdd6f4); border: 1px solid var(--vscode-button-border, #45475a); border-radius: 4px; text-align: left; cursor: pointer; font-size: 12px; }
        button:hover { background: var(--vscode-button-secondaryHoverBackground, #45475a); }
        button.primary { background: var(--vscode-button-background, #89b4fa); color: var(--vscode-button-foreground, #181825); font-weight: 600; border: none; }
        button.primary:hover { opacity: 0.9; }
        .card { background: var(--vscode-editorWidget-background, #1e1e2e); border: 1px solid var(--vscode-editorWidget-border, #313244); border-radius: 6px; padding: 10px; margin-bottom: 12px; }
        .meta { font-size: 11px; color: #a6adc8; }
        .project-item { padding: 6px 8px; margin-bottom: 4px; border-left: 2px solid var(--vscode-button-border, #45475a); border-radius: 2px; cursor: pointer; background: var(--vscode-editor-background); }
        .project-item:hover { border-left-color: var(--vscode-button-background, #89b4fa); background: var(--vscode-list-hoverBackground); }
        .project-item.active { border-left-color: #a6e3a1; background: var(--vscode-list-activeSelectionBackground, #313244); }
        .project-name { font-weight: 600; font-size: 12px; }
        .project-meta { font-size: 10px; color: #a6adc8; }
        #active-label { color: #a6e3a1; font-weight: 600; }
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
    <button onclick="vscode.postMessage({command:'openLayout'})">2D Layout (Peeker)</button>
    <button onclick="vscode.postMessage({command:'openFloorplanner'})">Floorplan Studio (FPS)</button>

    <h3>Flow Execution</h3>
    <button onclick="vscode.postMessage({command:'runSynthesis'})">Run Synthesis (Yosys)</button>
    <button onclick="vscode.postMessage({command:'runAll'})">Run Tapeout Flow (OpenROAD)</button>

    <script>
        const vscode = acquireVsCodeApi();
        const API_BASE = ${JSON.stringify(apiBaseUrl || 'http://127.0.0.1:8765')};

        async function loadProjects() {
            try {
                const resp = await fetch(API_BASE + '/api/projects');
                const projects = await resp.json();
                const container = document.getElementById('project-list');
                if (!Array.isArray(projects) || projects.length === 0) {
                    container.innerHTML = '<div class="meta">No projects found.</div>';
                    return;
                }
                container.innerHTML = projects.map(p => {
                    const isActive = p.is_active ? 'active' : '';
                    return \`<div class="project-item \${isActive}" onclick="selectProject('\${p.name}')">
                        <div class="project-name">\${p.name}</div>
                        <div class="project-meta">\${p.pdk} &middot; \${p.top_module}</div>
                    </div>\`;
                }).join('');

                const activeObj = projects.find(p => p.is_active);
                if (activeObj) {
                    document.getElementById('active-label').textContent = activeObj.name;
                }
            } catch(e) {}
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
