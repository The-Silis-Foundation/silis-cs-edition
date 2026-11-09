const vscode = require('vscode');

function activate(context) {
    console.log('[Silis CAD Extension] Activating Silis CS EDA Workbench...');

    // 1. Sidebar Webview Provider
    const sidebarProvider = {
        resolveWebviewView(webviewView) {
            webviewView.webview.options = { enableScripts: true };
            webviewView.webview.html = getSidebarHtml();
            webviewView.webview.onDidReceiveMessage(message => {
                switch (message.command) {
                    case 'openWorkbench':
                        vscode.commands.executeCommand('silis.openWorkbench');
                        break;
                    case 'openSchematic':
                        vscode.commands.executeCommand('silis.openSchematic');
                        break;
                    case 'openWaveforms':
                        vscode.commands.executeCommand('silis.openWaveforms');
                        break;
                    case 'openLayout':
                        vscode.commands.executeCommand('silis.openLayout');
                        break;
                    case 'openFloorplanner':
                        vscode.commands.executeCommand('silis.openFloorplanner');
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

    // 2. Custom Editor Provider for VCD / Waveforms
    const vcdEditorProvider = {
        async resolveCustomTextEditor(document, webviewPanel) {
            webviewPanel.webview.options = { enableScripts: true };
            const vcdText = document.getText();
            webviewPanel.webview.html = getWaveformEditorHtml(document.fileName, vcdText);
        }
    };
    context.subscriptions.push(
        vscode.window.registerCustomEditorProvider('silis.vcdViewer', vcdEditorProvider)
    );

    // 3. Custom Editor Provider for Structural Netlists (DigitalJS Schematic)
    const schematicEditorProvider = {
        async resolveCustomTextEditor(document, webviewPanel) {
            webviewPanel.webview.options = { enableScripts: true };
            const netlistJson = document.getText();
            webviewPanel.webview.html = getSchematicEditorHtml(netlistJson);
        }
    };
    context.subscriptions.push(
        vscode.window.registerCustomEditorProvider('silis.schematicViewer', schematicEditorProvider)
    );

    // 4. Commands
    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openWorkbench', () => {
            const panel = vscode.window.createWebviewPanel(
                'silisWorkbench',
                'Silis CAD Workbench',
                vscode.ViewColumn.Beside,
                { enableScripts: true, retainContextWhenHidden: true }
            );
            panel.webview.html = getIframeHtml('http://localhost:8765/workbench?project=projects/demo_soc', 'Silis CAD Workbench');
        })
    );

    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openSchematic', () => {
            const panel = vscode.window.createWebviewPanel(
                'silisSchematic',
                'Schematic Viewer (DigitalJS)',
                vscode.ViewColumn.Beside,
                { enableScripts: true, retainContextWhenHidden: true }
            );
            panel.webview.html = getIframeHtml('http://localhost:8765/schematic/digitaljs.html?module=chip_top&project=projects/demo_soc', 'Schematic Viewer');
        })
    );

    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openWaveforms', () => {
            const panel = vscode.window.createWebviewPanel(
                'silisWaveforms',
                'Waveforms (ROHD)',
                vscode.ViewColumn.Beside,
                { enableScripts: true, retainContextWhenHidden: true }
            );
            panel.webview.html = getIframeHtml('http://localhost:8765/waveforms/index.html?project=projects/demo_soc', 'Waveforms');
        })
    );

    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openLayout', () => {
            const panel = vscode.window.createWebviewPanel(
                'silisLayout',
                '2D Layout (FastSiliconPeeker)',
                vscode.ViewColumn.Beside,
                { enableScripts: true, retainContextWhenHidden: true }
            );
            panel.webview.html = getIframeHtml('http://localhost:6081/vnc.html?autoconnect=true&resize=remote', '2D Layout');
        })
    );

    context.subscriptions.push(
        vscode.commands.registerCommand('silis.openFloorplanner', () => {
            const panel = vscode.window.createWebviewPanel(
                'silisFPS',
                'Floorplan & Power Studio',
                vscode.ViewColumn.Beside,
                { enableScripts: true, retainContextWhenHidden: true }
            );
            panel.webview.html = getIframeHtml('http://localhost:6084/vnc.html?autoconnect=true&resize=remote', 'Floorplan Studio');
        })
    );

    context.subscriptions.push(
        vscode.commands.registerCommand('silis.runSynthesis', () => {
            const term = vscode.window.createTerminal('Silis Synthesis');
            term.show();
            term.sendText('silis --run-synth');
        })
    );

    context.subscriptions.push(
        vscode.commands.registerCommand('silis.runAll', () => {
            const term = vscode.window.createTerminal('Silis Tapeout');
            term.show();
            term.sendText('silis --run-all');
        })
    );

    // Automatically open WELCOME.md preview in side column on first boot
    setTimeout(() => {
        try {
            const wsFolders = vscode.workspace.workspaceFolders;
            if (wsFolders && wsFolders.length > 0) {
                const welcomeUri = vscode.Uri.joinPath(wsFolders[0].uri, 'WELCOME.md');
                vscode.commands.executeCommand('markdown.showPreview', welcomeUri);
            }
        } catch (e) {
            console.log('[Silis Extension] Could not auto-open welcome markdown:', e);
        }
    }, 1000);
}

function getIframeHtml(url, title) {
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <style>
        body, html { margin: 0; padding: 0; width: 100%; height: 100%; overflow: hidden; background: #181825; }
        iframe { width: 100%; height: 100%; border: none; }
    </style>
</head>
<body>
    <iframe src="${url}"></iframe>
</body>
</html>`;
}

function getSidebarHtml() {
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, monospace; color: var(--vscode-foreground); padding: 12px; font-size: 13px; line-height: 1.5; }
        h3 { font-size: 11px; text-transform: uppercase; letter-spacing: 1px; color: var(--vscode-sideBarTitle-foreground); margin: 16px 0 8px 0; }
        button { display: block; width: 100%; margin-bottom: 8px; padding: 7px 12px; background: var(--vscode-button-secondaryBackground, #313244); color: var(--vscode-button-secondaryForeground, #cdd6f4); border: 1px solid var(--vscode-button-border, #45475a); border-radius: 4px; text-align: left; cursor: pointer; font-size: 12px; }
        button:hover { background: var(--vscode-button-secondaryHoverBackground, #45475a); color: #fff; }
        button.primary { background: var(--vscode-button-background, #89b4fa); color: var(--vscode-button-foreground, #181825); font-weight: 600; border: none; }
        button.primary:hover { opacity: 0.9; }
        .card { background: var(--vscode-editorWidget-background, #1e1e2e); border: 1px solid var(--vscode-editorWidget-border, #313244); border-radius: 6px; padding: 10px; margin-bottom: 12px; }
        .meta { font-size: 11px; color: #a6adc8; }
    </style>
</head>
<body>
    <div class="card">
        <strong>Silis CS Edition</strong>
        <div class="meta">Native Cloud ASIC Workbench</div>
    </div>

    <h3>Interactive CAD Viewers</h3>
    <button class="primary" onclick="vscode.postMessage({command:'openWorkbench'})">Unified CAD Dock</button>
    <button onclick="vscode.postMessage({command:'openSchematic'})">Schematic (DigitalJS)</button>
    <button onclick="vscode.postMessage({command:'openWaveforms'})">Waveforms (ROHD)</button>
    <button onclick="vscode.postMessage({command:'openLayout'})">2D Layout (Peeker)</button>
    <button onclick="vscode.postMessage({command:'openFloorplanner'})">Floorplan Studio (FPS)</button>

    <h3>ASIC Synthesis & Flow</h3>
    <button onclick="vscode.postMessage({command:'runSynthesis'})">Run Synthesis (Yosys)</button>
    <button onclick="vscode.postMessage({command:'runAll'})">Run Tapeout Flow (OpenROAD)</button>

    <script>
        const vscode = acquireVsCodeApi();
    </script>
</body>
</html>`;
}

function getWaveformEditorHtml(fileName, vcdContent) {
    const escaped = JSON.stringify(vcdContent);
    const baseName = JSON.stringify(fileName.split('/').pop());
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <style>body, html { margin:0; padding:0; width:100%; height:100%; overflow:hidden; }</style>
</head>
<body>
    <iframe id="rohd-frame" src="http://localhost:8765/waveforms/index.html" style="width:100%; height:100%; border:none;"></iframe>
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

function getSchematicEditorHtml(netlistJson) {
    const escaped = JSON.stringify(netlistJson);
    return `<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <style>body, html { margin:0; padding:0; width:100%; height:100%; overflow:hidden; }</style>
</head>
<body>
    <iframe id="schem-frame" src="http://localhost:8765/schematic/digitaljs.html" style="width:100%; height:100%; border:none;"></iframe>
    <script>
        const frame = document.getElementById('schem-frame');
        frame.onload = function() {
            setTimeout(() => {
                const b64 = window.btoa(unescape(encodeURIComponent(${escaped})));
                frame.contentWindow.loadSchematic(b64);
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
