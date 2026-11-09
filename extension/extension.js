const vscode = require('vscode');

function activate(context) {
    console.log('[Silis CAD Extension] Activating Silis CS EDA Workbench...');

    let activeProject = context.workspaceState.get('silis.activeProject', 'demo_soc');

    // Helper to get active project directory path
    function getActiveProjectDir(pName) {
        const name = pName || activeProject || 'demo_soc';
        return `projects/${name}`;
    }

    // 1. Sidebar Webview Provider
    const sidebarProvider = {
        async resolveWebviewView(webviewView) {
            webviewView.webview.options = { enableScripts: true };
            const apiBase = await resolveUrl('http://127.0.0.1:8765');
            webviewView.webview.html = getSidebarHtml(apiBase.replace(/\/$/, ''), activeProject);

            // Listen for messages from sidebar
            webviewView.webview.onDidReceiveMessage(async (message) => {
                switch (message.command) {
                    case 'newProject':
                        vscode.commands.executeCommand('silis.newProject');
                        break;
                    case 'openProject':
                        vscode.commands.executeCommand('silis.openProject', message.project);
                        break;
                    case 'openWorkbench':
                        vscode.commands.executeCommand('silis.openWorkbench', activeProject);
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
                    case 'refresh':
                        vscode.commands.executeCommand('silis.refreshReadme');
                        break;
                }
            });
        }
    };
    context.subscriptions.push(
        vscode.window.registerWebviewViewProvider('silis-cad-sidebar', sidebarProvider)
    );

    // 1b. Bottom Panel Dock Provider (Waveforms tabbed next to Terminal)
    const bottomDockProvider = {
        async resolveWebviewView(webviewView) {
            webviewView.webview.options = { enableScripts: true };
            const projDir = getActiveProjectDir();
            const uri = await resolveUrl(`http://127.0.0.1:8765/waveforms/index.html?project=${encodeURIComponent(projDir)}`);
            webviewView.webview.html = getIframeHtml(uri, 'ROHD Waveforms');
        }
    };
    context.subscriptions.push(
        vscode.window.registerWebviewViewProvider('silis-bottom-dock', bottomDockProvider)
    );

    // 2. Custom Editor Provider for VCD / Waveforms
    const vcdEditorProvider = {
        async resolveCustomTextEditor(document, webviewPanel) {
            webviewPanel.webview.options = { enableScripts: true, retainContextWhenHidden: true };
            const vcdText = document.getText();
            const iframeSrc = await resolveUrl('http://127.0.0.1:8765/waveforms/index.html');
            webviewPanel.webview.html = getWaveformEditorHtml(document.fileName, vcdText, iframeSrc);
        }
    };
    context.subscriptions.push(
        vscode.window.registerCustomEditorProvider('silis.vcdViewer', vcdEditorProvider)
    );

    // 3. Custom Editor Provider for Structural Netlists (DigitalJS Schematic)
    const schematicEditorProvider = {
        async resolveCustomTextEditor(document, webviewPanel) {
            webviewPanel.webview.options = { enableScripts: true, retainContextWhenHidden: true };
            const netlistJson = document.getText();
            const iframeSrc = await resolveUrl('http://127.0.0.1:8765/schematic/digitaljs.html');
            webviewPanel.webview.html = getSchematicEditorHtml(netlistJson, iframeSrc);
        }
    };
    context.subscriptions.push(
        vscode.window.registerCustomEditorProvider('silis.schematicViewer', schematicEditorProvider)
    );

    // 3b. Custom Editor Provider for DEF 2D Silicon Layout
    const defEditorProvider = {
        async resolveCustomTextEditor(document, webviewPanel) {
            webviewPanel.webview.options = { enableScripts: true, retainContextWhenHidden: true };
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

    // 4. Project Management Commands

    // 4a. New ASIC Project Wizard
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.newProject', async () => {
            const panel = vscode.window.createWebviewPanel(
                'silisNewProject',
                'New ASIC Project',
                vscode.ViewColumn.Active,
                { enableScripts: true, retainContextWhenHidden: true }
            );

            panel.webview.html = getProjectWizardHtml();

            panel.webview.onDidReceiveMessage(async (message) => {
                if (message.command === 'createProject') {
                    const payload = message.data;
                    try {
                        const createUrl = await resolveUrl('http://127.0.0.1:8765/api/projects/create');
                        // Use native node fetch or fallback
                        const res = await callHttpApi(createUrl, 'POST', payload);
                        if (res && res.status === 'created') {
                            panel.webview.postMessage({ type: 'created', name: payload.name, top_module: payload.top_module });
                            activeProject = payload.name;
                            await context.workspaceState.update('silis.activeProject', activeProject);
                            vscode.window.showInformationMessage(`ASIC Project "${activeProject}" created successfully.`);

                            // Refresh dynamic README
                            vscode.commands.executeCommand('silis.refreshReadme');

                            // Open the newly generated RTL file in editor
                            if (res.rtl_file) {
                                const wsFolders = vscode.workspace.workspaceFolders;
                                if (wsFolders && wsFolders.length > 0) {
                                    const rtlUri = vscode.Uri.joinPath(wsFolders[0].uri, 'projects', activeProject, res.rtl_file);
                                    vscode.window.showTextDocument(rtlUri, { preview: false });
                                }
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

    // 4b. Open / Switch Active Project
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openProject', async (projectName) => {
            if (!projectName) {
                try {
                    const listUrl = await resolveUrl('http://127.0.0.1:8765/api/projects');
                    const projects = await callHttpApi(listUrl, 'GET');
                    if (Array.isArray(projects) && projects.length > 0) {
                        const items = projects.map(p => ({
                            label: p.name,
                            description: `${p.pdk} - Top: ${p.top_module}`,
                            detail: p.path
                        }));
                        const chosen = await vscode.window.showQuickPick(items, {
                            placeHolder: 'Select active ASIC project'
                        });
                        if (chosen) {
                            projectName = chosen.label;
                        }
                    }
                } catch (e) {
                    console.log('[Silis Extension] Error querying projects:', e);
                }
            }

            if (projectName) {
                activeProject = projectName;
                await context.workspaceState.update('silis.activeProject', activeProject);

                // Notify server of active project change
                try {
                    const switchUrl = await resolveUrl('http://127.0.0.1:8765/api/active_project');
                    await callHttpApi(switchUrl, 'POST', { name: activeProject });
                } catch (e) {
                    // Non-fatal
                }

                vscode.window.showInformationMessage(`Active Project switched to: ${activeProject}`);
            }
        })
    );

    // 4c. Refresh README Dashboard
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.refreshReadme', async () => {
            try {
                const wsFolders = vscode.workspace.workspaceFolders;
                if (wsFolders && wsFolders.length > 0) {
                    const cp = require('child_process');
                    const scriptPath = vscode.Uri.joinPath(wsFolders[0].uri, 'tools', 'gen_readme.py').fsPath;
                    cp.execFile('python3', [scriptPath], { cwd: wsFolders[0].uri.fsPath }, (err, stdout) => {
                        if (err) {
                            console.error('[Silis Extension] Failed to update README:', err);
                        } else {
                            console.log('[Silis Extension] README updated:', stdout);
                        }
                    });
                }
            } catch (e) {
                console.error('[Silis Extension] Refresh README error:', e);
            }
        })
    );

    // 5. Interactive CAD Viewers

    // 5a. Unified Workbench Dock
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openWorkbench', async (projectName) => {
            const pName = projectName || activeProject || 'demo_soc';
            const panel = vscode.window.createWebviewPanel(
                'silisWorkbench',
                `Silis Workbench [${pName}]`,
                vscode.ViewColumn.Active,
                { enableScripts: true, retainContextWhenHidden: true }
            );
            const uri = await resolveUrl(`http://127.0.0.1:8765/workbench?project=projects/${encodeURIComponent(pName)}`);
            panel.webview.html = getIframeHtml(uri, 'Silis CAD Workbench');
        })
    );

    // 5b. Schematic Viewer (DigitalJS)
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
            panel.webview.html = getIframeHtml(uri, 'Schematic Viewer');
        })
    );

    // 5c. Waveform Viewer (ROHD)
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openWaveforms', async (projectName) => {
            const pName = projectName || activeProject || 'demo_soc';
            const panel = vscode.window.createWebviewPanel(
                'silisWaveforms',
                `Waveforms: ${pName}`,
                vscode.ViewColumn.Active,
                { enableScripts: true, retainContextWhenHidden: true }
            );
            const uri = await resolveUrl(`http://127.0.0.1:8765/waveforms/index.html?project=projects/${encodeURIComponent(pName)}`);
            panel.webview.html = getIframeHtml(uri, 'Waveforms');
        })
    );

    // 5d. 2D Layout Canvas (FastSiliconPeeker)
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

    // 5e. Floorplan Studio (FPS)
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

    // 5f. Synthesis & PnR execution commands
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

    // Auto-preview README on startup
    setTimeout(() => {
        try {
            const wsFolders = vscode.workspace.workspaceFolders;
            if (wsFolders && wsFolders.length > 0) {
                const readmeUri = vscode.Uri.joinPath(wsFolders[0].uri, 'README.md');
                vscode.commands.executeCommand('markdown.showPreview', readmeUri);
            }
        } catch (e) {
            console.log('[Silis Extension] Auto-open README preview:', e);
        }
    }, 1200);
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
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta http-equiv="Content-Security-Policy" content="default-src * 'unsafe-inline' 'unsafe-eval' data: blob: ws: wss:; frame-src *; child-src *;">
    <style>
        body, html { margin: 0; padding: 0; width: 100%; height: 100%; overflow: hidden; background: #181825; }
        iframe { width: 100%; height: 100%; border: none; }
    </style>
</head>
<body>
    <iframe src="${url}" sandbox="allow-scripts allow-same-origin allow-forms allow-pointer-lock allow-popups"></iframe>
</body>
</html>`;
}

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
        <div class="meta">Active Design: <span id="active-label">${currentActive || 'loading...'}</span></div>
    </div>

    <h3>Workspace Projects</h3>
    <div id="project-list"><div class="meta">Loading projects...</div></div>
    <button class="primary" onclick="vscode.postMessage({command:'newProject'})">+ New ASIC Project</button>

    <h3>Active Project Viewers</h3>
    <button onclick="vscode.postMessage({command:'openWorkbench'})">Unified CAD Dock</button>
    <button onclick="vscode.postMessage({command:'openSchematic'})">Schematic (DigitalJS)</button>
    <button onclick="vscode.postMessage({command:'openWaveforms'})">Waveforms (ROHD)</button>
    <button onclick="vscode.postMessage({command:'openLayout'})">2D Layout (Peeker)</button>
    <button onclick="vscode.postMessage({command:'openFloorplanner'})">Floorplan Studio (FPS)</button>

    <h3>ASIC Synthesis & Flow</h3>
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
                    container.innerHTML = '<div class="meta">No projects yet. Create one below.</div>';
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
            } catch(e) {
                document.getElementById('project-list').innerHTML = '<div class="meta">Server connecting...</div>';
            }
        }

        function selectProject(name) {
            vscode.postMessage({ command: 'openProject', project: name });
            document.getElementById('active-label').textContent = name;
        }

        loadProjects();
        setInterval(loadProjects, 5000);
    </script>
</body>
</html>`;
}

function getProjectWizardHtml() {
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; color: var(--vscode-foreground); background: var(--vscode-editor-background); padding: 32px; max-width: 580px; margin: 0 auto; line-height: 1.5; }
        h2 { font-size: 16px; margin-bottom: 20px; border-bottom: 1px solid var(--vscode-editorWidget-border, #313244); padding-bottom: 10px; }
        label { display: block; font-size: 12px; font-weight: 600; margin-bottom: 4px; color: var(--vscode-foreground); }
        input, select { width: 100%; box-sizing: border-box; padding: 8px 10px; margin-bottom: 16px; background: var(--vscode-input-background); color: var(--vscode-input-foreground); border: 1px solid var(--vscode-input-border, #45475a); border-radius: 4px; font-size: 13px; font-family: inherit; }
        input:focus, select:focus { outline: none; border-color: var(--vscode-focusBorder); }
        .row { display: flex; gap: 12px; }
        .row > div { flex: 1; }
        button { padding: 9px 20px; border: none; border-radius: 4px; cursor: pointer; font-size: 13px; font-weight: 600; }
        button.primary { background: var(--vscode-button-background, #89b4fa); color: var(--vscode-button-foreground, #181825); }
        button.primary:hover { opacity: 0.9; }
        .hint { font-size: 11px; color: #a6adc8; margin-top: -12px; margin-bottom: 14px; }
        #status-msg { margin-top: 16px; padding: 12px; border-radius: 4px; font-size: 12px; display: none; line-height: 1.4; }
        #status-msg.success { background: #1e3a2f; color: #a6e3a1; border: 1px solid #40a070; }
        #status-msg.error { background: #3a1e1e; color: #f38ba8; border: 1px solid #a04040; }
        .card-tip { background: var(--vscode-editorWidget-background, #1e1e2e); border: 1px solid var(--vscode-editorWidget-border, #313244); border-radius: 6px; padding: 12px; margin-bottom: 20px; font-size: 12px; color: #cdd6f4; }
    </style>
</head>
<body>
    <h2>Create New ASIC Design Project</h2>

    <div class="card-tip">
        Configures an isolated physical design project scaffold with starter SystemVerilog RTL, timing constraints, and an automated OpenROAD floorplan recipe.
    </div>

    <label for="proj-name">Project Identifier (Directory Name)</label>
    <input id="proj-name" type="text" placeholder="e.g. riscv_core, systolic_array" maxlength="64" autofocus>
    <div class="hint">Alphanumeric characters and underscores only. Must start with a letter.</div>

    <label for="top-module">Top Module Name</label>
    <input id="top-module" type="text" placeholder="e.g. chip_top" maxlength="128">
    <div class="hint">Top-level Verilog/SystemVerilog module name. Defaults to project name.</div>

    <label for="pdk">Process Design Kit (PDK)</label>
    <select id="pdk">
        <option value="sky130B">SkyWater 130nm (sky130B) - Production Signoff</option>
        <option value="sky130A">SkyWater 130nm (sky130A) - Standard</option>
        <option value="gf180mcu">GlobalFoundries 180nm (gf180mcu) - 3.3V/5V Analog/MCU</option>
    </select>

    <div class="row">
        <div>
            <label for="die-w">Die Width (um)</label>
            <input id="die-w" type="number" value="600" min="50" max="10000" step="0.005">
        </div>
        <div>
            <label for="die-h">Die Height (um)</label>
            <input id="die-h" type="number" value="600" min="50" max="10000" step="0.005">
        </div>
    </div>

    <label for="util">Core Target Utilization (%)</label>
    <input id="util" type="number" value="50" min="40" max="70" step="1">
    <div class="hint">Valid tapeout range: 40% - 70%. Higher utilization induces detailed routing congestion.</div>

    <button class="primary" onclick="submitProject()">Initialize ASIC Project</button>

    <div id="status-msg"></div>

    <script>
        const vscode = acquireVsCodeApi();

        function showMessage(text, isError) {
            const el = document.getElementById('status-msg');
            el.innerHTML = text;
            el.className = isError ? 'error' : 'success';
            el.style.display = 'block';
        }

        function submitProject() {
            const name = document.getElementById('proj-name').value.trim();
            const topModule = document.getElementById('top-module').value.trim() || name;
            const pdk = document.getElementById('pdk').value;
            const dieW = parseFloat(document.getElementById('die-w').value);
            const dieH = parseFloat(document.getElementById('die-h').value);
            const util = parseInt(document.getElementById('util').value, 10);

            if (!/^[a-zA-Z][a-zA-Z0-9_]{0,63}$/.test(name)) {
                showMessage('Project name must begin with a letter and contain only alphanumeric characters and underscores.', true);
                return;
            }
            if (isNaN(dieW) || isNaN(dieH) || dieW < 50 || dieH < 50) {
                showMessage('Die width and height must be numeric values of at least 50.0 um.', true);
                return;
            }
            if (isNaN(util) || util < 40 || util > 70) {
                showMessage('Target core utilization must be between 40% and 70%.', true);
                return;
            }

            showMessage('Initializing project scaffold and generating physical files...', false);

            vscode.postMessage({
                command: 'createProject',
                data: {
                    name: name,
                    top_module: topModule,
                    pdk: pdk,
                    die_w: dieW,
                    die_h: dieH,
                    utilization: util / 100.0
                }
            });
        }

        window.addEventListener('message', (event) => {
            const msg = event.data;
            if (msg.type === 'created') {
                showMessage(
                    '<strong>Project Created Successfully</strong><br>' +
                    'Project <code>' + msg.name + '</code> is now active. RTL starter template opened in editor.',
                    false
                );
            } else if (msg.type === 'error') {
                showMessage('Error creating project: ' + msg.message, true);
            }
        });
    </script>
</body>
</html>`;
}

function getWaveformEditorHtml(fileName, vcdContent, iframeSrc) {
    const escaped = JSON.stringify(vcdContent);
    const baseName = JSON.stringify(fileName.split('/').pop());
    const srcUrl = iframeSrc || 'http://localhost:8765/waveforms/index.html';
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta http-equiv="Content-Security-Policy" content="default-src * 'unsafe-inline' 'unsafe-eval' data: blob: ws: wss:; frame-src *; child-src *;">
    <style>body, html { margin:0; padding:0; width:100%; height:100%; overflow:hidden; }</style>
</head>
<body>
    <iframe id="rohd-frame" src="${srcUrl}" style="width:100%; height:100%; border:none;"></iframe>
    <script>
        const frame = document.getElementById('rohd-frame');
        frame.onload = function() {
            setTimeout(() => {
                frame.contentWindow.postMessage({
                    type: 'vcdContents',
                    text: ${escaped},
                    fileName: ${baseName}
                }, '*');
            }, 500);
        };
    </script>
</body>
</html>`;
}

function getSchematicEditorHtml(netlistJson, iframeSrc) {
    const escaped = JSON.stringify(netlistJson);
    const srcUrl = iframeSrc || 'http://localhost:8765/schematic/digitaljs.html';
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta http-equiv="Content-Security-Policy" content="default-src * 'unsafe-inline' 'unsafe-eval' data: blob: ws: wss:; frame-src *; child-src *;">
    <style>body, html { margin:0; padding:0; width:100%; height:100%; overflow:hidden; }</style>
</head>
<body>
    <iframe id="schem-frame" src="${srcUrl}" style="width:100%; height:100%; border:none;"></iframe>
    <script>
        const frame = document.getElementById('schem-frame');
        frame.onload = function() {
            setTimeout(() => {
                const b64 = window.btoa(unescape(encodeURIComponent(${escaped})));
                if (frame.contentWindow && frame.contentWindow.loadSchematic) {
                    frame.contentWindow.loadSchematic(b64);
                }
            }, 500);
        };
    </script>
</body>
</html>`;
}

function deactivate() {}

module.exports = {
    activate,
    deactivate
};
