#!/usr/bin/env python3
"""
Silis CS Edition Automated Verification Suite
Architectural Test & Health Gate
Verifies devcontainer configuration, setup scripts, extension manifest,
Web CAD static assets, and live cs_server.py REST endpoints.
"""

import os
import sys
import json
import time
import urllib.request
import urllib.error
import subprocess

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
PASSED = 0
FAILED = 0


def record_result(test_name: str, passed: bool, detail: str = ""):
    global PASSED, FAILED
    if passed:
        PASSED += 1
        print(f"[PASS] {test_name}" + (f": {detail}" if detail else ""))
    else:
        FAILED += 1
        print(f"[FAIL] {test_name}: {detail}")


def test_devcontainer_config():
    path = os.path.join(REPO_ROOT, ".devcontainer", "devcontainer.json")
    if not os.path.isfile(path):
        record_result("devcontainer.json existence", False, "File not found")
        return
    try:
        with open(path, "r", encoding="utf-8") as f:
            cfg = json.load(f)
        img = cfg.get("image", "")
        has_universal = "universal" in img
        record_result("devcontainer base image", has_universal, f"Image: {img}")
        ports = cfg.get("forwardPorts", [])
        record_result("devcontainer forwarded ports", 8765 in ports, f"Ports: {ports}")
    except Exception as e:
        record_result("devcontainer.json valid JSON", False, str(e))


def test_setup_script():
    setup_path = os.path.join(REPO_ROOT, ".devcontainer", "setup.sh")
    if not os.path.isfile(setup_path):
        record_result("setup.sh existence", False, "File not found")
        return
    with open(setup_path, "r", encoding="utf-8") as f:
        content = f.read()

    # Invariant: Must never overwrite system /usr/bin/python3
    dangerous_symlink = "ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/bin/python3" in content
    record_result("setup.sh leaves system python intact", not dangerous_symlink, "Verified safe")

    # Invariant: Must not create root-owned .vscode-server
    root_vscode = "sudo mkdir -p \"/home/vscode/.vscode-server\"" in content or "sudo chown -R \"root:root\"" in content
    record_result("setup.sh prevents root ownership hijack", not root_vscode, "Verified safe")


def test_extension_manifest():
    pkg_path = os.path.join(REPO_ROOT, "extension", "package.json")
    if not os.path.isfile(pkg_path):
        record_result("extension/package.json existence", False, "File not found")
        return
    try:
        with open(pkg_path, "r", encoding="utf-8") as f:
            pkg = json.load(f)
        custom_editors = [e.get("viewType") for e in pkg.get("contributes", {}).get("customEditors", [])]
        has_def = "silis.defViewer" in custom_editors
        has_vcd = "silis.vcdViewer" in custom_editors
        has_schem = "silis.schematicViewer" in custom_editors
        record_result("extension custom editors", has_def and has_vcd and has_schem, f"Found: {custom_editors}")
    except Exception as e:
        record_result("extension/package.json valid JSON", False, str(e))


def test_static_web_assets():
    fps_index = os.path.join(REPO_ROOT, "extension", "web", "fps", "index.html")
    fps_js = os.path.join(REPO_ROOT, "extension", "web", "fps", "fps_canvas.js")
    record_result("Web FPS assets exist", os.path.isfile(fps_index) and os.path.isfile(fps_js))

    peeker_index = os.path.join(REPO_ROOT, "extension", "web", "peeker", "index.html")
    peeker_js = os.path.join(REPO_ROOT, "extension", "web", "peeker", "peeker_canvas.js")
    peeker_css = os.path.join(REPO_ROOT, "extension", "web", "peeker", "peeker.css")
    record_result("Web Peeker assets exist", os.path.isfile(peeker_index) and os.path.isfile(peeker_js) and os.path.isfile(peeker_css))


def test_cs_server_endpoints():
    server_script = os.path.join(REPO_ROOT, "tools", "cs_server.py")
    if not os.path.isfile(server_script):
        record_result("cs_server.py existence", False, "File not found")
        return

    test_port = 18765
    proc = subprocess.Popen(
        [sys.executable, server_script, str(test_port)],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )

    time.sleep(1.5)
    base_url = f"http://127.0.0.1:{test_port}"

    try:
        # 1. /api/status
        req = urllib.request.Request(f"{base_url}/api/status")
        with urllib.request.urlopen(req, timeout=3) as resp:
            data = json.loads(resp.read().decode())
            record_result("HTTP GET /api/status", resp.status == 200 and data.get("status") in ["ready", "running"])

        # 2. /api/projects
        req = urllib.request.Request(f"{base_url}/api/projects")
        with urllib.request.urlopen(req, timeout=3) as resp:
            data = json.loads(resp.read().decode())
            record_result("HTTP GET /api/projects", resp.status == 200 and isinstance(data, list))

        # 3. /floorplanner/index.html
        req = urllib.request.Request(f"{base_url}/floorplanner/index.html")
        with urllib.request.urlopen(req, timeout=3) as resp:
            content = resp.read().decode()
            record_result("HTTP GET /floorplanner/", resp.status == 200 and "<canvas" in content)

        # 4. /peeker/index.html
        req = urllib.request.Request(f"{base_url}/peeker/index.html")
        with urllib.request.urlopen(req, timeout=3) as resp:
            content = resp.read().decode()
            record_result("HTTP GET /peeker/", resp.status == 200 and "<canvas" in content)

        # 5. /api/def/parse
        req = urllib.request.Request(f"{base_url}/api/def/parse")
        with urllib.request.urlopen(req, timeout=3) as resp:
            data = json.loads(resp.read().decode())
            record_result("HTTP GET /api/def/parse", resp.status == 200 and "die" in data)

        # 6. /api/schematic
        req = urllib.request.Request(f"{base_url}/api/schematic?project=projects/demo_soc")
        with urllib.request.urlopen(req, timeout=3) as resp:
            record_result("HTTP GET /api/schematic", resp.status == 200)

        # 7. /api/power/switch_matrix
        req = urllib.request.Request(f"{base_url}/api/power/switch_matrix?width_um=400&height_um=400&peak_current_ma=50")
        with urllib.request.urlopen(req, timeout=3) as resp:
            psw_data = json.loads(resp.read().decode())
            record_result("HTTP GET /api/power/switch_matrix", resp.status == 200 and psw_data.get("status") == "PASS")

        # 8. POST /api/floorplan/quality_report
        post_data = json.dumps({"project": "demo_soc", "session": {"die": {"w": 2000, "h": 2000}, "core": {"border": 20}, "macros": []}}).encode("utf-8")
        req = urllib.request.Request(f"{base_url}/api/floorplan/quality_report", data=post_data, headers={"Content-Type": "application/json"}, method="POST")
        with urllib.request.urlopen(req, timeout=3) as resp:
            rep_res = json.loads(resp.read().decode())
            record_result("HTTP POST /api/floorplan/quality_report", resp.status == 200 and "report" in rep_res)

    except Exception as e:
        record_result("cs_server.py HTTP endpoint tests", False, str(e))
    finally:
        proc.terminate()
        try:
            proc.wait(timeout=2)
        except subprocess.TimeoutExpired:
            proc.kill()


def test_flow_cli():
    """Verify flow.py CLI supports --pnr, --floorplan, and --lint flags without argparse error."""
    flow_script = os.path.join(REPO_ROOT, "tools", "flow.py")
    res = subprocess.run([sys.executable, flow_script, "--help"], capture_output=True, text=True)
    has_flags = ("--pnr" in res.stdout and "--floorplan" in res.stdout and "--lint" in res.stdout)
    record_result("flow.py CLI flags (--pnr, --floorplan, --lint)", res.returncode == 0 and has_flags)


def test_bin_launcher():
    """Verify assets/bin/silis launcher executes successfully."""
    bin_launcher = os.path.join(REPO_ROOT, "assets", "bin", "silis")
    if os.path.isfile(bin_launcher) and os.access(bin_launcher, os.X_OK):
        res = subprocess.run([bin_launcher, "--help"], capture_output=True, text=True)
        record_result("assets/bin/silis CLI launcher", res.returncode == 0 and "Silis Headless Flow CLI" in res.stdout)
    else:
        record_result("assets/bin/silis CLI launcher", False, "Launcher binary missing or non-executable")


def test_def_parser():
    """Verify DEF parser handles multi-point DIEAREA and multi-line PINS."""
    try:
        sys.path.insert(0, os.path.join(REPO_ROOT, "tools"))
        from cs_server import parse_def_file
        dummy_def = os.path.join(REPO_ROOT, "projects", "_test_poly.def")
        with open(dummy_def, "w") as f:
            f.write("""VERSION 5.8 ;
UNITS DISTANCE MICRONS 1000 ;
DIEAREA ( 0 0 ) ( 0 2000000 ) ( 3000000 2000000 ) ( 3000000 0 ) ;
PINS 1 ;
  - clk + NET clk + DIRECTION INPUT
    + PORT
      + FIXED ( 500000 0 ) N ;
END PINS
END DESIGN
""")
        res = parse_def_file(dummy_def)
        if os.path.isfile(dummy_def):
            os.remove(dummy_def)
        die_ok = bool(res and res["die"]["w"] == 3000.0 and res["die"]["h"] == 2000.0)
        pins_ok = bool(res and len(res["pins"]) == 1 and res["pins"][0]["x"] == 500.0)
        record_result("DEF parser multi-point DIEAREA & multi-line PINS", die_ok and pins_ok)
    except Exception as e:
        record_result("DEF parser multi-point DIEAREA & multi-line PINS", False, str(e))


def main():
    print("=================================================================")
    print("SILIS CS EDITION: AUTOMATED ARCHITECTURAL VERIFICATION SUITE")
    print("=================================================================")

    test_devcontainer_config()
    test_setup_script()
    test_extension_manifest()
    test_static_web_assets()
    test_flow_cli()
    test_bin_launcher()
    test_def_parser()
    test_cs_server_endpoints()

    print("=================================================================")
    print(f"RESULTS: {PASSED} passed, {FAILED} failed")
    print("=================================================================")

    if FAILED > 0:
        sys.exit(1)
    sys.exit(0)


if __name__ == "__main__":
    main()
