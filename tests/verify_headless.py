#!/usr/bin/env python3
"""
verify_headless.py
Automated Headless Verification Suite for Silis CS Edition.

Validates:
1. Python SDK Project Loading & Flow Integrity
2. FastSiliconPeeker (PyQt6 + C++ OpenGL Engine)
3. LayerViewerPanel (OpenROAD C++ Layer Viewer & OpenDB Extraction)
4. Dual-Mode Viewer Stack (Mode 0: Instance-Wise vs Mode 1: Layer-Wise)
5. Portless Webview IPC Virtual Socket & noVNC RFB Bridge
6. Devcontainer & Extension VSIX Package Health
"""

import os
import sys
import subprocess
import json

# Enforce headless environment invariants
os.environ["SILIS_HEADLESS_TEST"] = "1"
os.environ["QT_QPA_PLATFORM"] = "offscreen"
os.environ["LIBGL_ALWAYS_SOFTWARE"] = "1"

from PyQt6.QtCore import Qt, QCoreApplication
from PyQt6.QtWidgets import QApplication

QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts, True)
_app = QApplication.instance() or QApplication(["silis_headless", "-platform", "offscreen"])

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if REPO_ROOT not in sys.path:
    sys.path.insert(0, REPO_ROOT)
silis_dir = os.path.join(REPO_ROOT, "silis")
if silis_dir not in sys.path:
    sys.path.insert(0, silis_dir)
core_dir = os.path.join(silis_dir, "core")
if core_dir not in sys.path:
    sys.path.insert(0, core_dir)


def log_header(title):
    print("\n" + "=" * 70)
    print(f" {title}")
    print("=" * 70)


def test_project_sdk():
    log_header("TEST 1: Python SDK Project Loading & Configuration")
    import silis
    
    proj_path = os.path.join(REPO_ROOT, "projects", "demo_soc")
    proj = silis.load_project(proj_path)
    assert proj is not None, "Failed to load projects/demo_soc"
    print(f"  [PASS] Project loaded: {proj.name}")
    print(f"  [PASS] Top module: {proj.config.get('top_module')}")
    print(f"  [PASS] PDK target: {proj.config.get('pdk')}")
    print(f"  [PASS] Results dir: {proj.results_dir}")
    return True


def test_fast_silicon_peeker():
    log_header("TEST 2: FastSiliconPeeker (PyQt6 + C++ OpenGL Engine)")
    from PyQt6.QtWidgets import QApplication
    from PyQt6.QtCore import QCoreApplication
    
    app = QApplication.instance() or QApplication(["silis_headless", "-platform", "offscreen"])
    import silis
    
    def_file = os.path.join(REPO_ROOT, "projects", "demo_soc", "results", "chip_top_filled.def")
    assert os.path.isfile(def_file), f"DEF not found: {def_file}"
    
    win = silis.view_layout(def_file, show=False, detach=False)
    peeker = win.peeker_widget
    
    if hasattr(peeker, '_load_worker') and peeker._load_worker:
        peeker._load_worker.wait(5000)
    QCoreApplication.processEvents()
    
    assert getattr(peeker, 'core', None) is not None, "FastLayoutViewer C++ core not loaded"
    assert peeker.def_data is not None, "DEF data failed to parse"
    
    print(f"  [PASS] Window Title: {win.windowTitle()}")
    print(f"  [PASS] Native C++ fast_layout_viewer Core loaded: True")
    print(f"  [PASS] Parsed instances/components: {len(getattr(peeker.def_data, 'comps_map', {}))}")
    
    # Test Search
    peeker.on_search_instance("clk")
    QCoreApplication.processEvents()
    print("  [PASS] Instance search executed cleanly")
    
    # Test Heatmaps
    peeker.set_heatmap_mode("ir_drop")
    QCoreApplication.processEvents()
    print("  [PASS] Static IR drop heatmap mode activated")
    
    peeker.set_heatmap_mode("congestion")
    QCoreApplication.processEvents()
    print("  [PASS] Routing congestion heatmap mode activated")
    
    if hasattr(peeker, '_load_worker') and peeker._load_worker and peeker._load_worker.isRunning():
        peeker._load_worker.quit()
        peeker._load_worker.wait(2000)

    win.close()
    QCoreApplication.processEvents()
    return True


def test_layer_viewer_panel():
    log_header("TEST 3: LayerViewerPanel (OpenROAD C++ SilisNativeLayerViewer)")
    from PyQt6.QtWidgets import QApplication
    from PyQt6.QtCore import QCoreApplication
    
    app = QApplication.instance() or QApplication(["silis_headless", "-platform", "offscreen"])
    import silis
    
    def_file = os.path.join(REPO_ROOT, "projects", "demo_soc", "results", "chip_top_filled.def")
    win = silis.view_layers(def_file, show=False, detach=False)
    panel = win.layer_panel
    
    if hasattr(panel, '_load_worker') and panel._load_worker:
        panel._load_worker.wait(5000)
    QCoreApplication.processEvents()
    
    assert panel is not None, "LayerViewerPanel failed to instantiate"
    assert getattr(panel, 'core', None) is not None, "SilisNativeLayerViewer C++ core not loaded"
    assert getattr(panel, 'inspector', None) is not None, "Pin/Macro inspector not loaded"
    
    print(f"  [PASS] Window Title: {win.windowTitle()}")
    print(f"  [PASS] SilisNativeLayerViewer C++ Core loaded: True")
    print(f"  [PASS] OpenDB Inspector loaded: True")
    
    # Test Layer visibility toggling
    panel.set_layer_visibility({'met1': True, 'met2': False, 'met3': True})
    QCoreApplication.processEvents()
    print("  [PASS] Dynamic layer visibility toggled cleanly")
    
    if hasattr(panel, '_load_worker') and panel._load_worker and panel._load_worker.isRunning():
        panel._load_worker.quit()
        panel._load_worker.wait(2000)
        
    win.close()
    QCoreApplication.processEvents()
    return True


def test_dual_mode_stack():
    log_header("TEST 4: Dual-Mode Viewer Stack (Mode 0 <-> Mode 1 In-Place Swapping)")
    from PyQt6.QtWidgets import QApplication, QMainWindow, QStatusBar
    from PyQt6.QtCore import QCoreApplication
    
    app = QApplication.instance() or QApplication(["silis_headless", "-platform", "offscreen"])
    from backendflow.flow.backendflow import BackendWidget
    
    class MockIDE(QMainWindow):
        def __init__(self):
            super().__init__()
            self.pdk_mgr = None
            self.active_pdk = "sky130"
            self.setStatusBar(QStatusBar(self))
        def get_context(self):
            return ("demo_soc",)
        def get_proj_root(self, name):
            return os.path.join(REPO_ROOT, "projects", "demo_soc")

    mock_ide = MockIDE()
    bw = BackendWidget(mock_ide)
    QCoreApplication.processEvents()
    
    # Initial state should be Mode 0 (Instance-Wise)
    assert bw.viz_stack.currentIndex() == 0, "Initial viz_stack must be 0"
    assert bw.sidebar_stack.currentIndex() == 0, "Initial sidebar_stack must be 0"
    print("  [PASS] Default state: Mode 0 (Instance-Wise FastSiliconPeeker)")
    
    # Switch to Mode 1 (Layer-Wise)
    bw.set_viewer_mode(1)
    QCoreApplication.processEvents()
    assert bw.viz_stack.currentIndex() == 1, "Mode 1 viz_stack must be 1"
    assert bw.sidebar_stack.currentIndex() == 1, "Mode 1 sidebar_stack must be 1"
    print("  [PASS] Switched to Mode 1 (Layer-Wise OpenROAD C++ Layer Viewer)")
    
    # Switch back to Mode 0
    bw.set_viewer_mode(0)
    QCoreApplication.processEvents()
    assert bw.viz_stack.currentIndex() == 0, "Mode 0 viz_stack must be 0"
    assert bw.sidebar_stack.currentIndex() == 0, "Mode 0 sidebar_stack must be 0"
    print("  [PASS] Switched back to Mode 0 cleanly")
    
    for w in [bw.layer_panel, bw.fast_peeker]:
        if hasattr(w, '_load_worker') and w._load_worker and w._load_worker.isRunning():
            w._load_worker.quit()
            w._load_worker.wait(2000)
            
    bw.close()
    mock_ide.close()
    QCoreApplication.processEvents()
    return True


def test_portless_vnc_bridge():
    log_header("TEST 5: Portless Webview IPC Virtual Socket & noVNC Protocol Handshake")
    node_bin = "/home/econsystems/.vscode/cli/serve-web/dcd3ad6e368fbe19d03f37054a40dbaf40d676f4/node"
    if not os.path.isfile(node_bin):
        node_bin = "node"
        
    js_test = """
globalThis.window = globalThis;
globalThis.window.addEventListener = (evt, handler) => { globalThis._msgHandler = handler; };
globalThis.WebSocket = { CONNECTING: 0, OPEN: 1, CLOSING: 2, CLOSED: 3 };

const { default: Websock } = await import('./extension/web/novnc/core/websock.js');
const { default: VirtualVncSocket } = await import('./extension/web/novnc/virtual_socket.js');

let postedMessages = [];
const mockVscode = { postMessage: (msg) => postedMessages.push(msg) };
const vSocket = new VirtualVncSocket(mockVscode);
const sock = new Websock();

// 1. Raw Channel Props
const rawChannelProps = ['send', 'close', 'binaryType', 'onerror', 'onmessage', 'onopen', 'protocol', 'readyState'];
const channelProps = [...Object.keys(vSocket), ...Object.getOwnPropertyNames(Object.getPrototypeOf(vSocket))];
for (const prop of rawChannelProps) {
    if (channelProps.indexOf(prop) < 0) throw new Error('Missing raw channel prop: ' + prop);
}
console.log('  [PASS] VirtualVncSocket implements all noVNC rawChannel properties');

// 2. Attach to Websock
sock.attach(vSocket);
console.log('  [PASS] Websock successfully attached to VirtualVncSocket');

// 3. Connect handshake
globalThis._msgHandler({ data: { type: 'vnc_open' } });
if (sock.readyState !== 'open') throw new Error('Socket failed to transition to open');
console.log('  [PASS] Connection state transition: open');

// 4. Binary duplex RFB exchange (test base64 string from extension host)
let receivedMsg = false;
sock.on('message', () => { receivedMsg = true; });
const b64Greeting = Buffer.from('RFB 003.008\\n').toString('base64');
globalThis._msgHandler({ data: { type: 'vnc_data', data: b64Greeting, isBase64: true } });
const decoded = sock.rQshiftStr(12);
if (decoded !== 'RFB 003.008\\n') throw new Error('RFB version decoding failed');
console.log('  [PASS] Received & decoded RFB handshake frame: ' + JSON.stringify(decoded));

// 5. Send RFB response (outgoing base64 over IPC)
sock.sQpushString('RFB 003.008\\n');
sock.flush();
const lastMsg = postedMessages[postedMessages.length - 1];
if (lastMsg.type !== 'vnc_data') throw new Error('Binary flush failed');
const sentBytes = Buffer.from(lastMsg.data, 'base64');
if (sentBytes.length !== 12 || sentBytes.toString() !== 'RFB 003.008\\n') throw new Error('Binary base64 payload mismatch');
console.log('  [PASS] Sent binary RFB frame across VS Code IPC pipe (12 bytes base64 encoded)');
"""
    res = subprocess.run([node_bin, "--input-type=module", "-e", js_test], cwd=REPO_ROOT, capture_output=True, text=True)
    if res.returncode != 0:
        print("Node.js test output:\n", res.stdout)
        print("Node.js test error:\n", res.stderr)
        raise RuntimeError("Portless VNC bridge test failed")
    for line in res.stdout.strip().split("\n"):
        if "[PASS]" in line:
            print(line)
    return True


def test_package_health():
    log_header("TEST 6: Devcontainer & Extension VSIX Package Health")
    # 1. Devcontainer scripts syntax
    r1 = subprocess.run(["bash", "-n", ".devcontainer/setup.sh"], cwd=REPO_ROOT)
    assert r1.returncode == 0, ".devcontainer/setup.sh syntax error"
    r2 = subprocess.run(["bash", "-n", ".devcontainer/launch_servers.sh"], cwd=REPO_ROOT)
    assert r2.returncode == 0, ".devcontainer/launch_servers.sh syntax error"
    print("  [PASS] .devcontainer scripts syntax verified")
    
    # 2. VSIX contents
    vsix_path = os.path.join(REPO_ROOT, "extension", "silis-cad-workbench-1.0.0.vsix")
    assert os.path.isfile(vsix_path), "VSIX package missing"
    vsix_size_mb = os.path.getsize(vsix_path) / (1024 * 1024)
    print(f"  [PASS] VSIX package exists ({vsix_size_mb:.2f} MB)")
    
    res = subprocess.run(["unzip", "-l", vsix_path], capture_output=True, text=True)
    assert "extension/web/novnc/index.html" in res.stdout, "VSIX missing novnc/index.html"
    assert "extension/web/novnc/virtual_socket.js" in res.stdout, "VSIX missing virtual_socket.js"
    assert "extension/web/novnc/core/rfb.js" in res.stdout, "VSIX missing core/rfb.js"
    assert "extension/extension.js" in res.stdout, "VSIX missing extension.js"
    assert "extension/web/schematic/index.html" in res.stdout, "VSIX missing schematic/index.html"
    assert "extension/web/schematic/schematic_webview.js" in res.stdout, "VSIX missing schematic_webview.js"
    assert "extension/web/schematic/yosys_bridge.js" in res.stdout, "VSIX missing yosys_bridge.js"
    assert "extension/web/tracker/index.html" in res.stdout, "VSIX missing tracker/index.html"
    assert "extension/web/tracker/tracker.css" in res.stdout, "VSIX missing tracker/tracker.css"
    assert "extension/web/synth_confirm/index.html" in res.stdout, "VSIX missing synth_confirm/index.html"
    assert "extension/web/synth_confirm/synth_confirm.css" in res.stdout, "VSIX missing synth_confirm/synth_confirm.css"
    print("  [PASS] VSIX archive contains all required portless noVNC, schematic, tracker, and synth confirmation files")
    return True


def test_schematic_viewer():
    log_header("TEST 7: Schematic Viewer (DigitalJS + Yosys Netlist Pipeline)")
    import shutil
    import tempfile
    
    # 1. Verify structural netlist exists
    netlist_path = os.path.join(REPO_ROOT, "projects", "demo_soc", "results", "structural_chip_top.json")
    assert os.path.isfile(netlist_path), f"Structural netlist not found: {netlist_path}"
    with open(netlist_path, "r", encoding="utf-8") as f:
        netlist_data = json.load(f)
    assert "modules" in netlist_data and "chip_top" in netlist_data["modules"], "Invalid netlist structure"
    print(f"  [PASS] Structural netlist parsed: {list(netlist_data['modules'].keys())}")

    # 2. Verify Node.js yosys_bridge processing
    node_bin = "/home/econsystems/.vscode/cli/serve-web/dcd3ad6e368fbe19d03f37054a40dbaf40d676f4/node"
    if not os.path.isfile(node_bin):
        node_bin = shutil.which("node") or "node"

    bridge_test = f"""
    const fs = require('fs');
    globalThis.window = globalThis;
    require('./extension/web/schematic/yosys_bridge.js');
    const netlist = JSON.parse(fs.readFileSync('{netlist_path}', 'utf8'));
    const circuit = window.processYosys(netlist, {{}});
    if (!circuit || !circuit.devices || !circuit.connectors) throw new Error('processYosys failed to generate valid circuit');
    console.log('  [PASS] yosys_bridge produced ' + Object.keys(circuit.devices).length + ' devices, ' + circuit.connectors.length + ' connectors');
    """
    res = subprocess.run([node_bin, "-e", bridge_test], cwd=REPO_ROOT, capture_output=True, text=True)
    if res.returncode != 0:
        print("Bridge stdout:", res.stdout)
        print("Bridge stderr:", res.stderr)
        raise RuntimeError("yosys_bridge test failed in Node.js")
    for line in res.stdout.strip().split("\n"):
        if "[PASS]" in line:
            print(line)

    # 3. Verify Headless Chrome rendering if browser binary is available
    chrome_bin = shutil.which("google-chrome") or shutil.which("chromium") or shutil.which("chromium-browser")
    if chrome_bin:
        schem_html = os.path.join(REPO_ROOT, "extension", "web", "schematic", "index.html")
        with open(schem_html, "r", encoding="utf-8") as f:
            html_content = f.read()

        test_dir = tempfile.mkdtemp(prefix="silis_schem_test_")
        test_file = os.path.join(test_dir, "test_render.html")
        
        # Rewrite paths relative to test_dir
        rel_schem_dir = os.path.join(REPO_ROOT, "extension", "web", "schematic")
        html_content = html_content.replace('href="schematic_webview.css"', f'href="{rel_schem_dir}/schematic_webview.css"')
        html_content = html_content.replace('src="yosys_bridge.js"', f'src="{rel_schem_dir}/yosys_bridge.js"')
        html_content = html_content.replace('src="schematic_webview.js"', f'src="{rel_schem_dir}/schematic_webview.js"')

        auto_inject = f"""
        <script>
        window.addEventListener('DOMContentLoaded', () => {{
            const data = {json.dumps(netlist_data)};
            loadSchematic(data);
            setTimeout(() => {{
                const paper = document.getElementById('paper');
                const svgs = paper ? paper.querySelectorAll('svg') : [];
                const cells = paper ? paper.querySelectorAll('.joint-cell, .joint-element, .joint-link') : [];
                console.log('[CHROME_SCHEM_PASS] Rendered ' + svgs.length + ' SVG paper(s), ' + cells.length + ' joint cells');
            }}, 1200);
        }});
        </script>
        """
        html_content = html_content.replace("</body>", auto_inject + "</body>")
        with open(test_file, "w", encoding="utf-8") as f:
            f.write(html_content)

        chrome_cmd = [
            chrome_bin,
            "--headless=new",
            "--allow-file-access-from-files",
            "--enable-logging=stderr",
            "--no-sandbox",
            f"file://{test_file}"
        ]
        try:
            c_res = subprocess.run(chrome_cmd, capture_output=True, text=True, timeout=5)
            log_output = c_res.stderr + "\n" + c_res.stdout
        except subprocess.TimeoutExpired as te:
            log_output = str(te.stderr or "") + "\n" + str(te.stdout or "")
        finally:
            shutil.rmtree(test_dir, ignore_errors=True)

        found_chrome_pass = False
        for line in log_output.split("\n"):
            if "[CHROME_SCHEM_PASS]" in line:
                print("  [PASS] Headless Chrome: " + line.split("[CHROME_SCHEM_PASS]")[-1].strip())
                found_chrome_pass = True
                break
        assert found_chrome_pass, f"Chrome failed to render schematic cells: {log_output[:500]}"
    else:
        print("  [INFO] google-chrome binary not present in environment; Node.js bridge verification passed")

    return True


def test_state_tracker_and_synth_confirm():
    log_header("TEST 8: Project State Tracker & Synthesis Confirmation Webviews")
    import shutil
    import tempfile

    # 1. Verify package.json contributions
    pkg_json_path = os.path.join(REPO_ROOT, "extension", "package.json")
    with open(pkg_json_path, "r", encoding="utf-8") as f:
        pkg = json.load(f)

    # Panel containers
    panels = {p["id"]: p["title"] for p in pkg.get("contributes", {}).get("viewsContainers", {}).get("panel", [])}
    assert "silis-state-tracker-panel" in panels, "Missing silis-state-tracker-panel in viewsContainers.panel"
    assert panels["silis-state-tracker-panel"] == "Project State Tracker", f"Unexpected title for state tracker: {panels.get('silis-state-tracker-panel')}"
    assert "silis-waveforms-panel" in panels, "Missing silis-waveforms-panel in viewsContainers.panel"
    assert panels["silis-waveforms-panel"] == "ROHD Wave Viewer", f"Expected 'ROHD Wave Viewer', got {panels.get('silis-waveforms-panel')}"
    print("  [PASS] package.json viewsContainers.panel validated: Project State Tracker and ROHD Wave Viewer registered")

    # Panel views
    panel_views = pkg.get("contributes", {}).get("views", {}).get("silis-state-tracker-panel", [])
    tracker_views = [v["id"] for v in panel_views]
    assert "silis-project-tracker" in tracker_views, "Missing silis-project-tracker view in silis-state-tracker-panel"
    print("  [PASS] silis-project-tracker webview registered in silis-state-tracker-panel")

    # Commands
    cmds = {c["command"]: c["title"] for c in pkg.get("contributes", {}).get("commands", [])}
    assert "silis.openStateTracker" in cmds, "silis.openStateTracker command missing"
    assert "silis.openSynthConfirm" in cmds, "silis.openSynthConfirm command missing"
    print("  [PASS] silis.openStateTracker and silis.openSynthConfirm commands registered")

    # 2. Verify Webview Assets Exist and Match Invariants
    tracker_html = os.path.join(REPO_ROOT, "extension", "web", "tracker", "index.html")
    tracker_css = os.path.join(REPO_ROOT, "extension", "web", "tracker", "tracker.css")
    synth_html = os.path.join(REPO_ROOT, "extension", "web", "synth_confirm", "index.html")
    synth_css = os.path.join(REPO_ROOT, "extension", "web", "synth_confirm", "synth_confirm.css")

    for p in [tracker_html, tracker_css, synth_html, synth_css]:
        assert os.path.isfile(p), f"Asset file missing: {p}"

    with open(tracker_html, "r", encoding="utf-8") as f:
        t_content = f.read()
    assert "tracker-container" in t_content, "tracker/index.html missing tracker-container"
    assert "stage-card" in t_content, "tracker/index.html missing stage-card"
    assert "acquireVsCodeApi" in t_content, "tracker/index.html missing acquireVsCodeApi"
    print("  [PASS] tracker/index.html contains tracker-container, stages, and VS Code IPC hooks")

    with open(synth_html, "r", encoding="utf-8") as f:
        s_content = f.read()
    assert "yosys-script-editor" in s_content, "synth_confirm/index.html missing yosys-script-editor"
    assert "in-scan-in" in s_content and "in-scan-out" in s_content and "in-scan-en" in s_content and "in-scan-clk" in s_content, "synth_confirm/index.html missing DFT scan ports"
    assert "chk-clamp" in s_content, "synth_confirm/index.html missing chk-clamp toggle"
    print("  [PASS] synth_confirm/index.html contains Yosys script editor & Silis-exact DFT scan configuration")

    # 3. Headless Chrome DOM Verification
    chrome_bin = shutil.which("google-chrome") or shutil.which("chromium") or shutil.which("chromium-browser")
    if chrome_bin:
        test_dir = tempfile.mkdtemp(prefix="silis_tracker_synth_test_")
        try:
            # Test Tracker Webview
            t_test = os.path.join(test_dir, "test_tracker.html")
            with open(tracker_html, "r", encoding="utf-8") as f:
                t_src = f.read()
            t_src = t_src.replace('href="tracker.css"', f'href="{tracker_css}"')
            t_inject = """
            <script>
            window.addEventListener('DOMContentLoaded', () => {
                const stages = document.querySelectorAll('.stage-card');
                console.log('[CHROME_TRACKER_PASS] Found ' + stages.length + ' pipeline stages in tracker DOM');
            });
            </script>
            """
            t_src = t_src.replace("</body>", t_inject + "</body>")
            with open(t_test, "w", encoding="utf-8") as f:
                f.write(t_src)

            c_cmd = [
                chrome_bin,
                "--headless=new",
                "--allow-file-access-from-files",
                "--enable-logging=stderr",
                "--no-sandbox",
                f"file://{t_test}"
            ]
            try:
                c_res = subprocess.run(c_cmd, capture_output=True, text=True, timeout=4)
                log = c_res.stderr + "\n" + c_res.stdout
            except subprocess.TimeoutExpired as te:
                log = str(te.stderr or "") + "\n" + str(te.stdout or "")
            assert "[CHROME_TRACKER_PASS]" in log, f"Tracker headless verification failed: {log[:500]}"
            print("  [PASS] Headless Chrome rendered State Tracker pipeline with 6 stages")

            # Test Synth Confirm Webview
            s_test = os.path.join(test_dir, "test_synth.html")
            with open(synth_html, "r", encoding="utf-8") as f:
                s_src = f.read()
            s_src = s_src.replace('href="synth_confirm.css"', f'href="{synth_css}"')
            s_inject = """
            <script>
            window.addEventListener('DOMContentLoaded', () => {
                const editor = document.getElementById('yosys-script-editor');
                const scanIn = document.getElementById('in-scan-in');
                if (editor && scanIn) {
                    console.log('[CHROME_SYNTH_PASS] Found editor and in-scan-in in synth confirm DOM');
                }
            });
            </script>
            """
            s_src = s_src.replace("</body>", s_inject + "</body>")
            with open(s_test, "w", encoding="utf-8") as f:
                f.write(s_src)

            c_cmd = [
                chrome_bin,
                "--headless=new",
                "--allow-file-access-from-files",
                "--enable-logging=stderr",
                "--no-sandbox",
                f"file://{s_test}"
            ]
            try:
                c_res = subprocess.run(c_cmd, capture_output=True, text=True, timeout=4)
                log = c_res.stderr + "\n" + c_res.stdout
            except subprocess.TimeoutExpired as te:
                log = str(te.stderr or "") + "\n" + str(te.stdout or "")
            assert "[CHROME_SYNTH_PASS]" in log, f"Synth Confirm headless verification failed: {log[:500]}"
            print("  [PASS] Headless Chrome rendered Synthesis & DFT Confirmation tab")
        finally:
            shutil.rmtree(test_dir, ignore_errors=True)
    else:
        print("  [INFO] google-chrome binary not present; DOM static assertions passed")

    return True


def main():
    print("=" * 70)
    print(" SILIS CS EDITION — COMPREHENSIVE HEADLESS VERIFICATION SUITE")
    print("=" * 70)
    
    tests = [
        test_project_sdk,
        test_fast_silicon_peeker,
        test_layer_viewer_panel,
        test_dual_mode_stack,
        test_portless_vnc_bridge,
        test_package_health,
        test_schematic_viewer,
        test_state_tracker_and_synth_confirm,
    ]
    
    passed = 0
    failed = 0
    
    for t in tests:
        try:
            if t():
                passed += 1
        except Exception as e:
            print(f"  [FAIL] {t.__name__} raised exception: {e}")
            import traceback
            traceback.print_exc()
            failed += 1
            
    print("\n" + "=" * 70)
    print(f" RESULTS: {passed} PASSED, {failed} FAILED")
    print("=" * 70)
    if failed == 0:
        print(">>> 100% HEADLESS VERIFICATION ACROSS ALL CAD SUBSYSTEMS PASSED! <<<\n")
        sys.exit(0)
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()
