const vscode = require('vscode');

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
                    case 'openOpenRoadGui':
                        vscode.commands.executeCommand('silis.openOpenRoadGui', activeProject);
                        break;
                    case 'getProjects':
                        try {
                            const projs = await callHttpApi('http://127.0.0.1:8765/api/projects', 'GET');
                            webviewView.webview.postMessage({ type: 'projectsLoaded', projects: projs });
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
        }
    };
    context.subscriptions.push(
        vscode.window.registerWebviewViewProvider('silis-cad-sidebar', sidebarProvider)
    );

    // 1b. Bottom Panel Dock Provider (Waveforms docked tabbed next to Terminal)
    let bottomDockWebview = null;
    let activeSchematicPanel = null;
    let activeLayoutPanel = null;

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
            { enableScripts: true, retainContextWhenHidden: true }
        );
        activeSchematicPanel = panel;

        const projDir = getActiveProjectDir(pName);
        const uri = await resolveUrl(`http://127.0.0.1:8765/schematic/digitaljs.html?project=${encodeURIComponent(projDir)}`);
        panel.webview.html = getIframeHtml(uri, 'Schematic: ' + pName);

        panel.webview.onDidReceiveMessage(async (msg) => {
            if (msg && msg.fromSilisSchem && msg.data) {
                const cadMsg = msg.data;
                if (cadMsg.type === 'contextAction') {
                    if (cadMsg.action === 'addToWaveform' && cadMsg.leafName) {
                        if (bottomDockWebview) {
                            bottomDockWebview.webview.postMessage({ type: 'addSignal', signalName: cadMsg.leafName });
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
        }, undefined, context.subscriptions);

        panel.onDidDispose(() => {
            activeSchematicPanel = null;
        }, null, context.subscriptions);

        return panel;
    }

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
        // D5: Do NOT auto-focus the dock here — focus is only triggered by explicit user action.
    }

    const bottomDockProvider = {
        async resolveWebviewView(webviewView) {
            bottomDockWebview = webviewView;
            webviewView.webview.options = { enableScripts: true, retainContextWhenHidden: true };

            // Check if active project already has waveform.vcd; if so, load it immediately
            const pDir = getActiveProjectDir();
            let hasVcd = false;
            try {
                const wsFolders = vscode.workspace.workspaceFolders;
                if (wsFolders && wsFolders.length > 0) {
                    const vcdUri = vscode.Uri.joinPath(wsFolders[0].uri, pDir, 'waveform.vcd');
                    try {
                        const st = await vscode.workspace.fs.stat(vcdUri);
                        hasVcd = (st.size > 0);
                    } catch (e) {}
                }
            } catch (e) {}

            if (hasVcd) {
                await loadWaveformInBottomDock(pDir);
            } else {
                webviewView.webview.html = getBottomDockIdleHtml();
            }

            let _scrubTimer = null;
            let _pendingScrubTime = null;
            let _pendingScrubSigs = null;

            webviewView.webview.onDidReceiveMessage(async (msg) => {
                if (msg && msg.fromSilisRohd && msg.data) {
                    const rohdMsg = msg.data;
                    if (rohdMsg.type === 'cursorMoved' && activeSchematicPanel) {
                        _pendingScrubTime = rohdMsg.time || 0;
                        _pendingScrubSigs = Array.isArray(rohdMsg.signals) ? rohdMsg.signals.join(',') : '';
                        if (_scrubTimer) clearTimeout(_scrubTimer);
                        _scrubTimer = setTimeout(async () => {
                            _scrubTimer = null;
                            const timePs = _pendingScrubTime;
                            const sigs = _pendingScrubSigs;
                            try {
                                const query = `project=${encodeURIComponent(getActiveProjectDir())}&time=${timePs}${sigs ? '&signals=' + encodeURIComponent(sigs) : ''}`;
                                const res = await callHttpApi(`http://127.0.0.1:8765/api/waveform/values?${query}`, 'GET');
                                if (Array.isArray(res) && activeSchematicPanel) {
                                    activeSchematicPanel.webview.postMessage({ type: 'setValues', updates: res });
                                }
                            } catch (e) {
                                console.error('[Silis Cross-Probe] Failed to query waveform values:', e);
                            }
                        }, 25);
                    }
                }
            });

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
            const defPath = document.fileName;
            const uri = await resolveUrl(`http://127.0.0.1:8765/peeker/index.html?def=${encodeURIComponent(defPath)}`);
            webviewPanel.webview.html = getIframeHtml(uri, 'Silis 2D Layout: ' + require('path').basename(defPath));
        }
    };
    context.subscriptions.push(
        vscode.window.registerCustomEditorProvider('silis.defViewer', defEditorProvider)
    );

    // 3c. Custom Editor Provider for GDSII
    const gdsEditorProvider = {
        async resolveCustomTextEditor(document, webviewPanel) {
            webviewPanel.webview.options = { enableScripts: true, retainContextWhenHidden: true };
            const uri = await resolveUrl('http://127.0.0.1:6081/vnc_lite.html?autoconnect=true&resize=scale');
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
                    case 'openOpenRoadGui':
                        vscode.commands.executeCommand('silis.openOpenRoadGui', msg.project || activeProject);
                        break;
                    case 'getProjects':
                        try {
                            const projs = await callHttpApi('http://127.0.0.1:8765/api/projects', 'GET');
                            dashboardPanel.webview.postMessage({ type: 'projectsLoaded', projects: projs });
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
                if (message.command === 'fetchMacros') {
                    try {
                        const pdk = message.pdk || 'sky130';
                        const macros = await callHttpApi(`http://127.0.0.1:8765/api/macros?pdk=${encodeURIComponent(pdk)}`, 'GET');
                        panel.webview.postMessage({ type: 'macrosLoaded', macros: macros || [] });
                    } catch (e) {
                        panel.webview.postMessage({ type: 'macrosLoaded', macros: [] });
                    }
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
                        panel.webview.postMessage({
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

                            // Auto-close wizard dialog (Desktop parity: QDialog.accept closes wizard on completion)
                            setTimeout(() => {
                                try { panel.dispose(); } catch (e) {}
                            }, 600);
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

    // 6. Open / Switch Active Project (Desktop Parity: Open top RTL on left, reveal in Explorer)
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

                // Fetch registered project metadata from server for accurate path & top RTL
                let matchedProject = null;
                try {
                    const projects = await callHttpApi('http://127.0.0.1:8765/api/projects', 'GET');
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

    // 8. Schematic Viewer (World 0: Split beside RTL)
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openSchematic', async (projectName) => {
            await openSchematicPanel(projectName, vscode.ViewColumn.Beside);
        })
    );

    // 9. 2D Layout Canvas (World 1: FastSiliconPeeker with auto-connect)
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openLayout', async (projectName) => {
            const pName = projectName || activeProject || 'demo_soc';
            if (activeLayoutPanel) {
                activeLayoutPanel.reveal(vscode.ViewColumn.Active);
                return;
            }
            activeLayoutPanel = vscode.window.createWebviewPanel(
                'silisLayout',
                '2D Layout (FastSiliconPeeker)',
                vscode.ViewColumn.Active,
                { enableScripts: true, retainContextWhenHidden: true }
            );
            const uri = await resolveUrl(`http://127.0.0.1:8765/peeker/index.html?project=${encodeURIComponent(pName)}`);
            activeLayoutPanel.webview.html = getIframeHtml(uri, '2D Layout: FastSiliconPeeker');
            activeLayoutPanel.onDidDispose(() => { activeLayoutPanel = null; }, null, context.subscriptions);
        })
    );

    // 10. Floorplan & Power Studio (World 3: FPS with auto-connect)
    let activeFloorplanPanel = null;
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openFloorplanner', async (projectName) => {
            const pName = projectName || activeProject || 'demo_soc';
            if (activeFloorplanPanel) {
                activeFloorplanPanel.reveal(vscode.ViewColumn.Active);
                return;
            }
            activeFloorplanPanel = vscode.window.createWebviewPanel(
                'silisFPS',
                'Floorplan & Power Studio',
                vscode.ViewColumn.Active,
                { enableScripts: true, retainContextWhenHidden: true }
            );
            const uri = await resolveUrl(`http://127.0.0.1:8765/floorplanner/index.html?project=${encodeURIComponent(pName)}`);
            activeFloorplanPanel.webview.html = getIframeHtml(uri, 'Floorplan & Power Studio');
            activeFloorplanPanel.onDidDispose(() => { activeFloorplanPanel = null; }, null, context.subscriptions);
        })
    );

    // 10b. OpenROAD Native GUI (Physical Layout Inspection)
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openOpenRoadGui', async (projectName) => {
            const pName = projectName || activeProject || 'demo_soc';
            const term = getFlowTerminal();
            term.show();
            term.sendText(`python3 tools/flow.py --view-openroad --project projects/${pName}`);
        })
    );

    function getFlowTerminal() {
        let term = vscode.window.terminals.find(t => t.name === 'Silis Flow');
        if (!term) {
            term = vscode.window.createTerminal('Silis Flow');
        }
        return term;
    }

    // 11. Run Simulation (Play Button: Runs compile & sim, opens Schematic on right, loads Waveform dock)
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.runSimulation', async () => {
            const pName = activeProject || 'demo_soc';
            vscode.window.showInformationMessage(`Running simulation for project "${pName}"...`);

            if (vscode.window.activeTextEditor) {
                await vscode.window.activeTextEditor.document.save();
            }

            // 1. Show terminal and execute headless simulation
            const term = getFlowTerminal();
            term.show();
            term.sendText(`python3 tools/flow.py --sim --project projects/${pName}`);

            // 2. Open schematic beside RTL
            await openSchematicPanel(pName, vscode.ViewColumn.Beside);

            // 3. Load waveform and focus waveviewer now that Run has been clicked
            await loadWaveformInBottomDock(getActiveProjectDir(pName));
            vscode.commands.executeCommand('silis-bottom-dock.focus');
        })
    );

    // 12. Run Verilator RTL Lint (Debug Button & Ctrl+L)
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.runLint', async () => {
            const pName = activeProject || 'demo_soc';
            const outputChan = vscode.window.createOutputChannel('Silis RTL Lint');
            outputChan.show(true);
            outputChan.appendLine(`[LINT] Running Verilator RTL Lint on project "${pName}" (Ctrl+L)...`);

            try {
                const res = await callHttpApi(`http://127.0.0.1:8765/api/lint?project=projects/${encodeURIComponent(pName)}`, 'GET');
                if (res) {
                    const output = (res.output || res.stderr || res.stdout || '').trim();
                    if (!output && res.success) {
                        outputChan.appendLine('[LINT] [OK] RTL Lint Clean: No warnings or errors found.');
                        vscode.window.showInformationMessage(`Silis Lint: ${pName} is clean.`);
                    } else {
                        outputChan.appendLine(output);
                        outputChan.appendLine(`[LINT] Lint completed with exit code ${res.exit_code}.`);
                        if (!res.success) {
                            vscode.window.showWarningMessage(`Silis Lint: Diagnostics reported for ${pName}. Check Silis RTL Lint output channel.`);
                        }
                    }
                }
            } catch (err) {
                outputChan.appendLine(`[LINT] API request error: ${err.message}`);
                const term = getFlowTerminal();
                term.show();
                term.sendText(`verilator --lint-only -sv -Wall -Wno-fatal -Iprojects/${pName}/rtl projects/${pName}/rtl/*.sv projects/${pName}/rtl/*.v`);
            }
        })
    );

    // 13. Flow Execution
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.runSynthesis', () => {
            const term = getFlowTerminal();
            term.show();
            term.sendText(`python3 tools/flow.py --synth --project projects/${activeProject}`);
        })
    );

    context.subscriptions.push(
        vscode.commands.registerCommand('silis.runAll', () => {
            const term = getFlowTerminal();
            term.show();
            term.sendText(`python3 tools/flow.py --tapeout --project projects/${activeProject}`);
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

function getIframeHtml(url, title) {
    const safeTitle = (title || 'Silis CAD Engine').replace(/</g, '&lt;').replace(/>/g, '&gt;');
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta http-equiv="Content-Security-Policy" content="default-src * 'unsafe-inline' 'unsafe-eval' data: blob: ws: wss:; frame-src *; child-src *;">
    <style>
        body, html { margin: 0; padding: 0; width: 100%; height: 100%; overflow: hidden; background: var(--vscode-editor-background); font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; }
        #silis-bar {
            height: 28px;
            background: var(--vscode-editor-background);
            border-bottom: 1px solid var(--vscode-editorWidget-border);
            display: flex;
            align-items: center;
            justify-content: space-between;
            padding: 0 10px;
            color: var(--vscode-foreground);
            font-size: 11px;
            box-sizing: border-box;
            user-select: none;
        }
        .bar-title { font-weight: 600; color: var(--vscode-textLink-foreground); }
        .bar-links { display: flex; align-items: center; gap: 8px; }
        .bar-btn {
            background: var(--vscode-editorWidget-border);
            color: var(--vscode-foreground);
            border: 1px solid var(--vscode-editorWidget-border);
            border-radius: 3px;
            padding: 2px 8px;
            font-size: 10px;
            cursor: pointer;
            text-decoration: none;
            display: inline-flex;
            align-items: center;
        }
        .bar-btn:hover { background: var(--vscode-list-hoverBackground); color: var(--vscode-foreground); }
        .frame-wrap { width: 100%; height: calc(100% - 28px); position: relative; }
        iframe { width: 100%; height: 100%; border: none; background: var(--vscode-editor-background); }
    </style>
</head>
<body>
    <div id="silis-bar">
        <div class="bar-title">${safeTitle}</div>
        <div class="bar-links">
            <button class="bar-btn" onclick="const f=document.getElementById('cad-frame'); if(f) f.src='${url}';">Reload</button>
            <a class="bar-btn" href="${url}" target="_blank" rel="noopener noreferrer">Open in New Window</a>
        </div>
    </div>
    <div class="frame-wrap">
        <iframe id="cad-frame" name="silis-target:${encodeURIComponent(url)}" src="${url}"></iframe>
    </div>
    <script>
        const vscode = acquireVsCodeApi();
        const targetUrl = '${url}';
        let redirected = false;

        window.addEventListener('message', function(event) {
            // Instant redirect when iframe lands on / from codespace port warning
            if (event.data && event.data.type === 'silisHubReady') {
                const f = document.getElementById('cad-frame');
                if (f && !redirected) {
                    redirected = true;
                    f.src = targetUrl;
                }
            }
            // Forward messages from inner iframe out to VS Code extension host
            if (event.data && (event.data.fromSilisSchem || event.data.fromSilisRohd || event.data.fromSilis)) {
                vscode.postMessage(event.data);
            }
            // Forward messages from VS Code extension host into inner iframe
            if (event.data && (event.data.type === 'setValues' || event.data.type === 'clearValues' || event.data.type === 'addSignal')) {
                const f = document.getElementById('cad-frame');
                if (f && f.contentWindow) {
                    f.contentWindow.postMessage(event.data, '*');
                }
            }
        });

        const frame = document.getElementById('cad-frame');
        if (frame) {
            frame.addEventListener('load', function() {
                try {
                    if (frame.contentWindow) {
                        frame.contentWindow.postMessage({ type: 'silisRedirect', target: targetUrl }, '*');
                    }
                } catch(e) {}
            });
        }
    </script>
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
        .sub { font-size: 13px; color: var(--vscode-descriptionForeground); margin-bottom: 20px; }
        .active-info { font-size: 15px; font-weight: 600; }
        .active-pdk { font-size: 12px; color: var(--vscode-textLink-foreground); font-family: monospace; margin-left: 8px; }
        .btn-group { display: flex; gap: 8px; flex-wrap: wrap; }
        button { padding: 6px 14px; background: var(--vscode-button-secondaryBackground, #313244); color: var(--vscode-button-secondaryForeground, #cdd6f4); border: 1px solid var(--vscode-button-border, #45475a); border-radius: 4px; cursor: pointer; font-size: 12px; font-weight: 500; font-family: inherit; }
        button:hover { background: var(--vscode-button-secondaryHoverBackground, #45475a); }
        button.primary { background: var(--vscode-button-background, #89b4fa); color: var(--vscode-button-foreground, #181825); font-weight: 600; border: none; }
        button.primary:hover { opacity: 0.9; }
        table { width: 100%; border-collapse: collapse; font-size: 12px; }
        th { text-align: left; padding: 8px 10px; color: var(--vscode-descriptionForeground); border-bottom: 1px solid var(--vscode-editorWidget-border); font-weight: 600; }
        td { padding: 10px; border-bottom: 1px solid var(--vscode-editorWidget-border); }
        tr:hover td { background: var(--vscode-list-hoverBackground); }
        .tag { font-size: 10px; padding: 2px 6px; border-radius: 3px; font-family: monospace; }
        .tag.ready { background: var(--vscode-charts-green, #1e3a2f); color: var(--vscode-charts-green, #57a564); }
        .tag.none { background: var(--vscode-editorWidget-border); color: var(--vscode-descriptionForeground); }
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
                <button onclick="msg('openOpenRoadGui')">OpenROAD GUI</button>
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
        const API_BASE = ${JSON.stringify(apiBaseUrl || 'http://127.0.0.1:8765')};

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
                        '<button onclick="msg(\\'openOpenRoadGui\\', {project:\\'' + p.name + '\\'})">OpenROAD</button>' +
                    '</td>' +
                '</tr>';
            }).join('');

            const act = projs.find(p => p.is_active);
            if (act) {
                document.getElementById('active-name').textContent = act.name;
                document.getElementById('active-pdk').textContent = 'PDK: ' + act.pdk;
            }
        }

        async function refresh() {
            msg('getProjects');
            try {
                const resp = await fetch(API_BASE + '/api/projects');
                const projs = await resp.json();
                renderDashboardProjects(projs);
            } catch(e) {}
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
        input, select { width: 100%; box-sizing: border-box; padding: 8px 10px; margin-bottom: 16px; background: var(--vscode-input-background); color: var(--vscode-input-foreground); border: 1px solid var(--vscode-input-border, #45475a); border-radius: 4px; font-size: 13px; font-family: inherit; }
        input:focus, select:focus { outline: none; border-color: var(--vscode-focusBorder); }
        .hint { font-size: 11px; color: var(--vscode-descriptionForeground); margin-top: -12px; margin-bottom: 16px; }
        .card-box { background: var(--vscode-editorWidget-background, #1e1e2e); border: 1px solid var(--vscode-editorWidget-border, #313244); border-radius: 6px; padding: 14px; margin-bottom: 16px; }
        .nav-footer { display: flex; justify-content: space-between; margin-top: 24px; border-top: 1px solid var(--vscode-editorWidget-border); padding-top: 16px; }
        button { padding: 8px 16px; border-radius: 4px; cursor: pointer; font-size: 12px; font-weight: 600; border: 1px solid var(--vscode-editorWidget-border); background: var(--vscode-button-secondaryBackground, var(--vscode-editorWidget-border)); color: var(--vscode-button-secondaryForeground, var(--vscode-foreground)); }
        button:hover { background: var(--vscode-button-secondaryHoverBackground, var(--vscode-list-hoverBackground)); }
        button.primary { background: var(--vscode-button-background); color: var(--vscode-button-foreground); border: none; }
        button.primary:hover { opacity: 0.9; }
        .btn-row { display: flex; gap: 8px; margin-bottom: 12px; }
        .file-list { background: var(--vscode-input-background); border: 1px solid var(--vscode-editorWidget-border); border-radius: 4px; min-height: 80px; max-height: 150px; overflow-y: auto; padding: 6px; margin-bottom: 12px; font-family: monospace; font-size: 12px; }
        .file-entry { display: flex; justify-content: space-between; align-items: center; padding: 4px 6px; border-bottom: 1px solid var(--vscode-editorWidget-border); }
        .file-entry:last-child { border-bottom: none; }
        .del-btn { color: #f38ba8; cursor: pointer; font-weight: bold; margin-left: 8px; }
        .browse-row { display: flex; gap: 8px; margin-bottom: 16px; }
        .browse-row input { margin-bottom: 0; }
        .browse-row button { white-space: nowrap; }
        #status-msg { margin-top: 16px; padding: 12px; border-radius: 4px; font-size: 12px; display: none; }
        #status-msg.success { background: var(--vscode-charts-green, #1e3a2f); color: var(--vscode-charts-green, #57a564); border: 1px solid var(--vscode-charts-green, #40a070); }
        #status-msg.error { background: #3a1e1e; color: #f38ba8; border: 1px solid #a04040; }
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
        .badge { background: var(--vscode-badge-background, #313244); color: var(--vscode-badge-foreground, #cdd6f4); font-size: 10px; padding: 2px 6px; border-radius: 10px; }
        .cart-items { display: flex; flex-wrap: wrap; gap: 6px; }
        .cart-pill { display: inline-flex; align-items: center; gap: 6px; padding: 4px 8px; background: var(--vscode-editorWidget-background); border: 1px solid var(--vscode-editorWidget-border); border-radius: 4px; font-size: 11px; font-family: monospace; }
        .cart-del { cursor: pointer; color: #f38ba8; font-weight: bold; }
    </style>
</head>
<body>
    <div class="stepper">
        <div class="step-item active" id="st-1"><span class="step-num">1</span>Project</div>
        <div class="step-item" id="st-2"><span class="step-num">2</span>RTL Sources</div>
        <div class="step-item" id="st-3"><span class="step-num">3</span>Constraints</div>
        <div class="step-item" id="st-4"><span class="step-num">4</span>Power &amp; IO</div>
        <div class="step-item" id="st-5"><span class="step-num">5</span>Silicon Target</div>
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

    <!-- Step 3: Constraints (Page 3 - 100% Silis Desktop Parity) -->
    <div class="step-content" id="sp-3">
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

    <!-- Step 4: Additional Design Intent (Page 4b - 100% Silis Desktop Parity) -->
    <div class="step-content" id="sp-4">
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

    <!-- Step 5: Silicon Target (Page 5 - 100% Silis Desktop Parity) -->
    <div class="step-content" id="sp-5">
        <h2>Silicon Target &amp; Top Module</h2>
        <div class="step-desc">Select target semiconductor process kit, top module, and add macros from the IP Catalog.</div>

        <label for="p-pdk">Process Design Kit (PDK)</label>
        <select id="p-pdk" onchange="fetchMacrosForPdk()">
            <option value="sky130_fd_sc_hd">SkyWater 130nm High-Density (sky130_fd_sc_hd) - Standard</option>
            <option value="sky130B">SkyWater 130nm Production (sky130B)</option>
            <option value="gf180mcu">GlobalFoundries 180nm MCU (gf180mcu)</option>
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
        const selectedFiles = { rtl: [], sdc: [] };
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
        });
        document.getElementById('p-top').addEventListener('input', () => {
            document.getElementById('p-top').dataset.touched = '1';
        });

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
            vscode.postMessage({
                command: 'browseFiles',
                target: target,
                canSelectFiles: !isFolder,
                canSelectFolders: isFolder,
                canSelectMany: true,
                title: isFolder ? 'Select Directory' : 'Select Source Files',
                filters: target === 'rtl' ? { 'HDL Files': ['v', 'sv', 'vhd', 'vhdl'], 'All Files': ['*'] } : { 'SDC Files': ['sdc'], 'All Files': ['*'] }
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
            const ext = target === 'rtl' ? '.v' : '.sdc';
            const name = prompt('Enter filename (e.g. ' + (target === 'rtl' ? 'module.v' : 'timing.sdc') + '):');
            if (name && name.trim()) {
                let fname = name.trim();
                if (!fname.endsWith(ext) && !fname.endsWith('.sv')) fname += ext;
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
            for (let i = 1; i <= 5; i++) {
                document.getElementById('sp-' + i).className = 'step-content' + (i === s ? ' active' : '');
                document.getElementById('st-' + i).className = 'step-item' + (i === s ? ' active' : '');
            }
            document.getElementById('btn-prev').style.display = s > 1 ? 'inline-block' : 'none';
            document.getElementById('btn-next').style.display = s < 5 ? 'inline-block' : 'none';
            document.getElementById('btn-finish').style.display = s === 5 ? 'inline-block' : 'none';
            if (s === 5 && allMacros.length === 0) {
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
                    pdk: pdk,
                    clock_period: period,
                    rtl_files: selectedFiles.rtl,
                    sdc_files: selectedFiles.sdc,
                    upf_file: upf,
                    io_file: io,
                    macros: cartMacros
                }
            });
        }

        window.addEventListener('message', e => {
            const msg = e.data;
            if (msg.type === 'macrosLoaded') {
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
        fetchMacrosForPdk();
    </script>
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
        .meta { font-size: 11px; color: var(--vscode-descriptionForeground); }
        .project-item { padding: 6px 8px; margin-bottom: 4px; border-left: 2px solid var(--vscode-button-border, #45475a); border-radius: 2px; cursor: pointer; background: var(--vscode-editor-background); }
        .project-item:hover { border-left-color: var(--vscode-button-background, #89b4fa); background: var(--vscode-list-hoverBackground); }
        .project-item.active { border-left-color: var(--vscode-charts-green, #57a564); background: var(--vscode-list-activeSelectionBackground, #313244); }
        .project-name { font-weight: 600; font-size: 12px; }
        .project-meta { font-size: 10px; color: var(--vscode-descriptionForeground); }
        #active-label { color: var(--vscode-charts-green, #57a564); font-weight: 600; }
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
    <button onclick="vscode.postMessage({command:'openOpenRoadGui'})">OpenROAD Native GUI</button>

    <h3>Flow Execution</h3>
    <button onclick="vscode.postMessage({command:'runSynthesis'})">Run Synthesis (Yosys)</button>
    <button onclick="vscode.postMessage({command:'runAll'})">Run Tapeout Flow (OpenROAD)</button>

    <script>
        const vscode = acquireVsCodeApi();
        const API_BASE = ${JSON.stringify(apiBaseUrl || 'http://127.0.0.1:8765')};

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
            if (msg && msg.type === 'projectsLoaded' && Array.isArray(msg.projects)) {
                renderSidebarProjects(msg.projects);
            }
        });

        async function loadProjects() {
            vscode.postMessage({ command: 'getProjects' });
            try {
                const resp = await fetch(API_BASE + '/api/projects');
                const projects = await resp.json();
                renderSidebarProjects(projects);
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
