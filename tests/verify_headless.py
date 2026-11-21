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

// 4. Binary duplex RFB exchange
let receivedMsg = false;
sock.on('message', () => { receivedMsg = true; });
const rfbGreeting = [0x52, 0x46, 0x42, 0x20, 0x30, 0x30, 0x33, 0x2e, 0x30, 0x30, 0x38, 0x0a];
globalThis._msgHandler({ data: { type: 'vnc_data', data: rfbGreeting } });
const decoded = sock.rQshiftStr(12);
if (decoded !== 'RFB 003.008\\n') throw new Error('RFB version decoding failed');
console.log('  [PASS] Received & decoded RFB handshake frame: ' + JSON.stringify(decoded));

// 5. Send RFB response
sock.sQpushString('RFB 003.008\\n');
sock.flush();
const lastMsg = postedMessages[postedMessages.length - 1];
if (lastMsg.type !== 'vnc_data' || lastMsg.data.length !== 12) throw new Error('Binary flush failed');
console.log('  [PASS] Sent binary RFB frame across VS Code IPC pipe (12 bytes)');
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
    print("  [PASS] VSIX archive contains all required portless noVNC and extension files")
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
