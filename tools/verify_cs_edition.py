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

        # 9. GET /api/timing/constraints (Iteration 86)
        req = urllib.request.Request(f"{base_url}/api/timing/constraints?project=projects/demo_soc")
        with urllib.request.urlopen(req, timeout=3) as resp:
            t_data = json.loads(resp.read().decode())
            record_result("HTTP GET /api/timing/constraints", resp.status == 200 and "clocks" in t_data)

        # 10. GET /api/power/level_shifter_check (Iteration 87)
        req = urllib.request.Request(f"{base_url}/api/power/level_shifter_check?project=projects/demo_soc")
        with urllib.request.urlopen(req, timeout=3) as resp:
            upf_data = json.loads(resp.read().decode())
            record_result("HTTP GET /api/power/level_shifter_check", resp.status == 200 and "status" in upf_data)

        # 11. GET /api/layout/thumbnail (Iteration 88)
        req = urllib.request.Request(f"{base_url}/api/layout/thumbnail?project=projects/demo_soc")
        with urllib.request.urlopen(req, timeout=3) as resp:
            svg_data = resp.read().decode()
            record_result("HTTP GET /api/layout/thumbnail", resp.status == 200 and "<svg" in svg_data)

        # 12. POST /api/floorplan/export_def (Iteration 89)
        def_post = json.dumps({"project": "demo_soc", "session": {"die": {"w": 2500, "h": 2500}, "core": {"border": 20}, "macros": [{"inst": "u_ram", "master": "sram", "x": 100, "y": 100, "w": 300, "h": 200, "status": "FIXED"}], "pads": [{"name": "clk", "edge": "bottom", "pos": 500, "dir": "input"}]}}).encode("utf-8")
        req = urllib.request.Request(f"{base_url}/api/floorplan/export_def", data=def_post, headers={"Content-Type": "application/json"}, method="POST")
        with urllib.request.urlopen(req, timeout=3) as resp:
            def_res = json.loads(resp.read().decode())
            record_result("HTTP POST /api/floorplan/export_def", resp.status == 200 and def_res.get("status") == "ok")

        # 13. GET /api/pdks (Multi-PDK Catalog)
        req = urllib.request.Request(f"{base_url}/api/pdks")
        with urllib.request.urlopen(req, timeout=3) as resp:
            pdk_data = json.loads(resp.read().decode())
            pdk_ids = [p.get("id") for p in pdk_data] if isinstance(pdk_data, list) else []
            has_core_pdks = all(k in pdk_ids for k in ["sky130A", "sky130B", "gf180mcu", "asap7"])
            record_result("HTTP GET /api/pdks (sky130A, sky130B, gf180mcu, asap7)", resp.status == 200 and has_core_pdks, f"PDKs: {pdk_ids}")

        # 14. GET /api/macros (Dynamic Macro IP Catalog & Dual-Mode Stubs)
        req = urllib.request.Request(f"{base_url}/api/macros?pdk=sky130A")
        with urllib.request.urlopen(req, timeout=5) as resp:
            macro_data = json.loads(resp.read().decode())
            has_macros = isinstance(macro_data, list) and len(macro_data) > 0
            stub_ok = False
            if has_macros:
                sample_stub = macro_data[0].get("stub", "")
                stub_ok = ("`ifndef SYNTHESIS" in sample_stub and "`else" in sample_stub and "`endif" in sample_stub)
            record_result("HTTP GET /api/macros dual-mode stubs", resp.status == 200 and has_macros and stub_ok, f"Count: {len(macro_data) if has_macros else 0}")

        # 15. POST /api/projects/create (Testbench & Metadata Contract)
        test_proj_name = "tb_verify_test_proj"
        test_proj_dir = os.path.join(REPO_ROOT, "projects", test_proj_name)
        if os.path.exists(test_proj_dir):
            import shutil
            shutil.rmtree(test_proj_dir, ignore_errors=True)

        create_payload = json.dumps({
            "name": test_proj_name,
            "top_module": "dut_top",
            "top_tb": "tb_dut_top",
            "pdk": "sky130A"
        }).encode("utf-8")
        req = urllib.request.Request(f"{base_url}/api/projects/create", data=create_payload, headers={"Content-Type": "application/json"}, method="POST")
        with urllib.request.urlopen(req, timeout=5) as resp:
            c_res = json.loads(resp.read().decode())
            tb_folder = os.path.join(test_proj_dir, "tb")
            tb_v = os.path.join(tb_folder, "tb.v")
            proj_json = os.path.join(test_proj_dir, "project.json")
            silis_json = os.path.join(test_proj_dir, ".silis_proj.json")
            silisproj = os.path.join(test_proj_dir, "silis.silisproj")

            tb_created = (
                resp.status == 201 and
                os.path.isdir(tb_folder) and
                os.path.isfile(tb_v) and
                os.path.isfile(proj_json) and
                os.path.isfile(silis_json) and
                os.path.isfile(silisproj)
            )
            # Verify contents of metadata files
            meta_ok = False
            if tb_created:
                with open(proj_json, "r") as pf:
                    pj_data = json.load(pf)
                with open(silis_json, "r") as sf:
                    sj_data = json.load(sf)
                meta_ok = (pj_data.get("top_tb") == "tb_dut_top" and "tb/tb.v" in pj_data.get("tb_files", []) and
                           sj_data.get("top_tb") == "tb_dut_top" and "tb/tb.v" in sj_data.get("tb_files", []))
            record_result("HTTP POST /api/projects/create tb/ and metadata contracts", tb_created and meta_ok)

        # Clean up test project
        if os.path.exists(test_proj_dir):
            import shutil
            shutil.rmtree(test_proj_dir, ignore_errors=True)
            gen_readme_script = os.path.join(REPO_ROOT, "tools", "gen_readme.py")
            if os.path.isfile(gen_readme_script):
                subprocess.run([sys.executable, gen_readme_script], cwd=REPO_ROOT, timeout=10)

        # 16. POST /api/floorplan/export_tcl (OpenROAD Fences, Regions, and Blockages)
        fp_payload = json.dumps({
            "project": "demo_soc",
            "session": {
                "die": {"w": 3000, "h": 3000},
                "core": {"border": 30},
                "macros": [],
                "regions": [
                    {"module_name": "u_dsp", "x": 100, "y": 100, "w": 400, "h": 400, "is_fence": True},
                    {"module_name": "u_alu", "x": 600, "y": 100, "w": 400, "h": 400, "is_fence": False}
                ],
                "blockages": [
                    {"x": 1200, "y": 100, "w": 200, "h": 200, "btype": "Hard"},
                    {"x": 1500, "y": 100, "w": 200, "h": 200, "btype": "Soft"},
                    {"x": 1800, "y": 100, "w": 200, "h": 200, "btype": "Partial", "pct": 45}
                ]
            }
        }).encode("utf-8")
        req = urllib.request.Request(f"{base_url}/api/floorplan/export_tcl", data=fp_payload, headers={"Content-Type": "application/json"}, method="POST")
        with urllib.request.urlopen(req, timeout=3) as resp:
            tcl_res = json.loads(resp.read().decode())
            tcl_text = tcl_res.get("tcl", "")
            has_fence = "create_fence -module u_dsp -box { 100.000 100.000 500.000 500.000 }" in tcl_text
            has_region = "create_region -module u_alu -box { 600.000 100.000 1000.000 500.000 } -type inclusive" in tcl_text
            has_hard = "create_placement_blockage -area { 1200.000 100.000 1400.000 300.000 } -type hard" in tcl_text
            has_soft = "create_placement_blockage -area { 1500.000 100.000 1700.000 300.000 } -type soft" in tcl_text
            has_part = "create_placement_blockage -area { 1800.000 100.000 2000.000 300.000 } -type partial -density 45" in tcl_text
            no_invalid = ("create_voltage_fence" not in tcl_text and "set_region" not in tcl_text)
            tcl_ok = has_fence and has_region and has_hard and has_soft and has_part and no_invalid
            record_result("HTTP POST /api/floorplan/export_tcl OpenROAD compliance", tcl_ok, "Fences, regions, and blockages verified")

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
    """Verify DEF parser handles multi-point DIEAREA, components status, and specialnets."""
    try:
        sys.path.insert(0, os.path.join(REPO_ROOT, "tools"))
        from cs_server import parse_def_file
        dummy_def = os.path.join(REPO_ROOT, "projects", "_test_poly.def")
        with open(dummy_def, "w") as f:
            f.write("""VERSION 5.8 ;
UNITS DISTANCE MICRONS 1000 ;
DIEAREA ( 0 0 ) ( 0 2000000 ) ( 3000000 2000000 ) ( 3000000 0 ) ;
COMPONENTS 1 ;
  - u_mem sram + FIXED ( 100000 200000 ) N ;
END COMPONENTS
SPECIALNETS 1 ;
  - VDD ( PIN VDD ) + ROUTED met4 1600 ( 100000 200000 ) ( 100000 800000 ) ;
END SPECIALNETS
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
        comp_ok = bool(res and len(res["components"]) == 1 and res["components"][0]["status"] == "FIXED")
        snet_ok = bool(res and len(res.get("specialnets", [])) == 1 and res["specialnets"][0]["name"] == "VDD")
        pins_ok = bool(res and len(res["pins"]) == 1 and res["pins"][0]["x"] == 500.0)
        record_result("DEF parser multi-point DIEAREA & status & specialnets", die_ok and comp_ok and snet_ok and pins_ok)
    except Exception as e:
        record_result("DEF parser multi-point DIEAREA & status & specialnets", False, str(e))


def test_fps_parity_features():
    """Verify Web FPS canvas implements all desktop FPA algorithms and features."""
    fps_js = os.path.join(REPO_ROOT, "extension", "web", "fps", "fps_canvas.js")
    if not os.path.isfile(fps_js):
        record_result("Web FPS parity algorithms", False, "fps_canvas.js missing")
        return
    with open(fps_js, "r", encoding="utf-8") as f:
        src = f.read()

    required_symbols = [
        "computePeripheralMacroPlacement",
        "applyAutoplaceMethod",
        "computeMinimumDieForPads",
        "expandDieForPadRing",
        "orderPreservingSpaceEdge",
        "syncUpfPowerPads",
        "routePadTrunkBridges",
        "configureDualFollowpins",
        "triggerAutosave",
        "revertToSavedSession",
        "exportSvg",
        "detectCollisions"
    ]
    missing = [sym for sym in required_symbols if sym not in src]
    record_result("Web FPS parity algorithms (autoplace, pad ring, autosave, svg)", len(missing) == 0, f"Missing: {missing}" if missing else "")


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
    test_fps_parity_features()
    test_cs_server_endpoints()

    print("=================================================================")
    print(f"RESULTS: {PASSED} passed, {FAILED} failed")
    print("=================================================================")

    if FAILED > 0:
        sys.exit(1)
    sys.exit(0)

    if FAILED > 0:
        sys.exit(1)
    sys.exit(0)


if __name__ == "__main__":
    main()
