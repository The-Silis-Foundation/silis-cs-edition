#!/usr/bin/env python3
"""
Silis CS Edition Local CAD Web Server
Serves DigitalJS Schematic Viewer and ROHD Waveform Viewer with zero X11/VNC overhead.
Provides REST endpoints for live Verilog netlist synthesis, project management, and cross-probing.
Matches Silis desktop project structure and contracts with 100% fidelity.
"""

import os
import re
import sys
import glob
import json
import shutil
import platform
import subprocess
import http.server
import socketserver
import urllib.parse
from pathlib import Path

# Paths
REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
PROJECTS_DIR = os.path.join(REPO_ROOT, "projects")
SCHEMATIC_DIR = os.path.join(REPO_ROOT, "core", "schematicviewer")
ROHD_DIR = os.path.join(REPO_ROOT, "core", "signalpeeker", "web", "rohd")

# Add core to sys.path so we can import project_registry
CORE_DIR = os.path.join(REPO_ROOT, "core")
if CORE_DIR not in sys.path:
    sys.path.insert(0, CORE_DIR)


class SilisCadHandler(http.server.SimpleHTTPRequestHandler):
    active_project_dir = os.path.join(REPO_ROOT, "projects", "demo_soc")

    def end_headers(self):
        # Allow cross-origin and iframe embedding in VS Code Simple Browser & webviews
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "*")
        self.send_header("Cache-Control", "no-cache, no-store, must-revalidate")
        super().end_headers()

    def do_OPTIONS(self):
        self.send_response(200)
        self.end_headers()

    def _send_json(self, status_code, data):
        self.send_response(status_code)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.end_headers()
        self.wfile.write(json.dumps(data, indent=2).encode("utf-8"))

    def do_GET(self):
        parsed = urllib.parse.urlparse(self.path)
        path = parsed.path
        query = urllib.parse.parse_qs(parsed.query)

        # 1. API: List all discovered projects
        if path == "/api/projects":
            projects = []
            registered_paths = set()
            try:
                import project_registry
                for p in project_registry.get_registered_projects():
                    p_path = p.get("path")
                    if p_path and os.path.isdir(p_path):
                        registered_paths.add(os.path.abspath(p_path))
                        projects.append({
                            "name": p.get("name", os.path.basename(p_path)),
                            "top_module": p.get("top", "top"),
                            "pdk": p.get("pdk", "sky130"),
                            "has_def": p.get("has_def", False),
                            "has_vcd": len(glob.glob(os.path.join(p_path, "**", "*.vcd"), recursive=True)) > 0,
                            "rtl_files": [],
                            "path": p_path,
                            "is_active": (os.path.abspath(p_path) == os.path.abspath(SilisCadHandler.active_project_dir)),
                        })
            except Exception:
                pass

            # Scan workspace projects directory
            if os.path.isdir(PROJECTS_DIR):
                for item in sorted(os.listdir(PROJECTS_DIR)):
                    p_dir = os.path.join(PROJECTS_DIR, item)
                    if not os.path.isdir(p_dir) or item.startswith(".") or os.path.abspath(p_dir) in registered_paths:
                        continue
                    meta = {}
                    silisproj_candidates = [
                        os.path.join(p_dir, "silis.silisproj"),
                        os.path.join(p_dir, f"{item}.silisproj"),
                    ]
                    for sp in silisproj_candidates:
                        if os.path.isfile(sp):
                            try:
                                with open(sp, "r", encoding="utf-8") as f:
                                    meta = json.load(f)
                                break
                            except Exception:
                                pass

                    has_def = len(glob.glob(os.path.join(p_dir, "**", "*.def"), recursive=True)) > 0
                    has_vcd = len(glob.glob(os.path.join(p_dir, "**", "*.vcd"), recursive=True)) > 0 or \
                              len(glob.glob(os.path.join(p_dir, "**", "*.fst"), recursive=True)) > 0

                    projects.append({
                        "name": item,
                        "top_module": meta.get("top_module", item),
                        "pdk": meta.get("pdk", "sky130"),
                        "has_def": has_def,
                        "has_vcd": has_vcd,
                        "rtl_files": meta.get("rtl_files", []),
                        "path": p_dir,
                        "is_active": (os.path.abspath(p_dir) == os.path.abspath(SilisCadHandler.active_project_dir)),
                    })

            self._send_json(200, projects)
            return

        # 2. API: System / Runtime Status
        if path == "/api/status":
            openroad_path = shutil.which("openroad") or os.path.join(REPO_ROOT, "third-party", "bin", "openroad")
            yosys_path = shutil.which("yosys") or os.path.join(REPO_ROOT, "third-party", "bin", "yosys")

            status = {
                "status": "ready",
                "python": sys.version.split()[0],
                "platform": platform.platform(),
                "openroad": bool(os.path.isfile(openroad_path) and os.access(openroad_path, os.X_OK)) or bool(shutil.which("openroad")),
                "yosys": bool(os.path.isfile(yosys_path) and os.access(yosys_path, os.X_OK)) or bool(shutil.which("yosys")),
                "active_project": os.path.basename(SilisCadHandler.active_project_dir),
                "active_project_dir": SilisCadHandler.active_project_dir,
                "server_root": REPO_ROOT,
            }
            self._send_json(200, status)
            return

        # 3. API: Get active project
        if path == "/api/active_project":
            self._send_json(200, {
                "active_project": os.path.basename(SilisCadHandler.active_project_dir),
                "active_project_dir": SilisCadHandler.active_project_dir,
            })
            return

        # 4. API: Get Netlist JSON for Schematic
        if path == "/api/netlist":
            target_file = query.get("file", [None])[0]
            module_name = query.get("module", ["chip_top"])[0]
            project_dir = query.get("project", [SilisCadHandler.active_project_dir])[0]
            if not os.path.isabs(project_dir):
                project_dir = os.path.join(REPO_ROOT, project_dir)
            data = self._get_or_generate_netlist(project_dir, module_name, target_file)
            if data:
                self.send_response(200)
                self.send_header("Content-Type", "application/json; charset=utf-8")
                self.send_header("Access-Control-Allow-Origin", "*")
                self.send_header("X-Frame-Options", "ALLOWALL")
                self.send_header("Content-Security-Policy", "frame-ancestors *")
                self.end_headers()
                self.wfile.write(data.encode("utf-8") if isinstance(data, str) else data)
            else:
                self._send_json(500, {"error": "Failed to synthesize netlist"})
            return

        # 5. API: Get VCD Waveform text (supports ?file= and ?project=)
        if path == "/api/waveform":
            target_file = query.get("file", [None])[0]
            project_dir = query.get("project", [SilisCadHandler.active_project_dir])[0]
            if not os.path.isabs(project_dir):
                project_dir = os.path.join(REPO_ROOT, project_dir)
            vcd_content = self._find_waveform(project_dir, target_file)
            if vcd_content:
                self.send_response(200)
                self.send_header("Content-Type", "text/plain; charset=utf-8")
                self.send_header("Access-Control-Allow-Origin", "*")
                self.send_header("X-Frame-Options", "ALLOWALL")
                self.send_header("Content-Security-Policy", "frame-ancestors *")
                self.end_headers()
                self.wfile.write(vcd_content.encode("utf-8"))
            else:
                self._send_json(404, {"error": "No VCD waveform found"})
            return

        # 6. Route static files:
        if path == "/workbench" or path == "/workbench.html":
            return self._serve_file(os.path.join(os.path.dirname(__file__), "workbench.html"))

        if path.startswith("/schematic/") or path == "/schematic.html" or path == "/digitaljs.html":
            rel_path = path.replace("/schematic/", "").replace("/schematic.html", "digitaljs.html").replace("/digitaljs.html", "digitaljs.html").lstrip("/")
            local_file = os.path.join(SCHEMATIC_DIR, rel_path if rel_path else "digitaljs.html")
            return self._serve_file(local_file)

        if path.startswith("/waveforms/") or path.startswith("/rohd/") or path == "/waveforms.html":
            rel_path = path.replace("/waveforms/", "").replace("/rohd/", "").replace("/waveforms.html", "index.html").lstrip("/")
            local_file = os.path.join(ROHD_DIR, rel_path if rel_path else "index.html")
            return self._serve_file(local_file)

        # Default root
        if path == "/" or path == "":
            self.send_response(200)
            self.send_header("Content-Type", "text/html; charset=utf-8")
            self.end_headers()
            self.wfile.write(b"""<!DOCTYPE html>
<html>
<head><title>Silis CS Edition Web Hub</title>
<style>
body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, monospace; background: #181825; color: #cdd6f4; padding: 2rem; line-height: 1.6; }
a { color: #89b4fa; text-decoration: none; font-size: 1rem; }
a:hover { text-decoration: underline; }
ul { line-height: 2; }
.card { background: #1e1e2e; border: 1px solid #313244; border-radius: 8px; padding: 16px; margin-bottom: 20px; max-width: 600px; }
</style>
</head>
<body>
<div class="card">
  <h2>Silis CS Edition - Native CAD Web Engines</h2>
  <p>Dedicated virtual display daemons and browser webview interfaces.</p>
</div>
<ul>
  <li><a href="/schematic/digitaljs.html">Schematic Viewer (DigitalJS & ELK Engine)</a></li>
  <li><a href="/waveforms/index.html">Waveform Viewer (ROHD Web & WASM)</a></li>
  <li><a href="http://localhost:6081/vnc.html?autoconnect=true&resize=remote">2D Layout Viewer (FastSiliconPeeker on :1)</a></li>
  <li><a href="http://localhost:6084/vnc.html?autoconnect=true&resize=remote">Floorplan & Power Studio (Silis FPS on :4)</a></li>
</ul>
</body>
</html>""")
            return

        # Fallback to serving from SCHEMATIC_DIR or ROHD_DIR
        if os.path.exists(os.path.join(SCHEMATIC_DIR, path.lstrip("/"))):
            return self._serve_file(os.path.join(SCHEMATIC_DIR, path.lstrip("/")))
        if os.path.exists(os.path.join(ROHD_DIR, path.lstrip("/"))):
            return self._serve_file(os.path.join(ROHD_DIR, path.lstrip("/")))

        self.send_response(404)
        self.end_headers()

    def do_POST(self):
        parsed = urllib.parse.urlparse(self.path)
        path = parsed.path

        # Read JSON body safely
        content_length = int(self.headers.get("Content-Length", 0))
        body_str = self.rfile.read(content_length).decode("utf-8") if content_length > 0 else "{}"
        try:
            payload = json.loads(body_str) if body_str else {}
        except Exception as e:
            self._send_json(400, {"error": f"Invalid JSON payload: {e}"})
            return

        # 1. API: Create New Silis Project (100% parity with SilisProjectWizard in codes.py)
        if path == "/api/projects/create":
            name = payload.get("name", "").strip()
            top_module = payload.get("top_module", "").strip() or name
            pdk = payload.get("pdk", "sky130_fd_sc_hd")

            # Validation
            if not re.match(r"^[a-zA-Z][a-zA-Z0-9_]{0,63}$", name):
                self._send_json(400, {"error": "Project name must begin with a letter and contain only alphanumeric characters and underscores (max 64 chars)."})
                return

            proj_dir = os.path.join(PROJECTS_DIR, name)
            if os.path.exists(proj_dir):
                self._send_json(409, {"error": f"Project directory '{name}' already exists."})
                return

            try:
                # Authentic Silis directory layout (matching codes.py:accept() lines 481-488)
                os.makedirs(os.path.join(proj_dir, "rtl"), exist_ok=True)
                os.makedirs(os.path.join(proj_dir, "constr"), exist_ok=True)
                os.makedirs(os.path.join(proj_dir, "tb"), exist_ok=True)
                os.makedirs(os.path.join(proj_dir, "power"), exist_ok=True)
                os.makedirs(os.path.join(proj_dir, "io"), exist_ok=True)
                os.makedirs(os.path.join(proj_dir, "netlist"), exist_ok=True)
                os.makedirs(os.path.join(proj_dir, "reports"), exist_ok=True)
                os.makedirs(os.path.join(proj_dir, "results"), exist_ok=True)

                # 1. Handle RTL sources
                rtl_files = []
                starter_type = payload.get("starter_type", "counter")

                # If existing files to copy were provided
                copy_files = payload.get("copy_files", [])
                for cf in copy_files:
                    if os.path.isfile(cf):
                        dst = os.path.join(proj_dir, "rtl", os.path.basename(cf))
                        shutil.copy2(cf, dst)
                        rtl_files.append(f"rtl/{os.path.basename(cf)}")

                # If no RTL files provided, generate starter based on selected type
                if not rtl_files:
                    rtl_filename = f"{top_module}.sv"
                    rtl_rel_path = f"rtl/{rtl_filename}"
                    rtl_abs_path = os.path.join(proj_dir, "rtl", rtl_filename)

                    if starter_type == "empty":
                        rtl_content = f"""// Silis ASIC Project: {name}
// Top Module: {top_module}
// Process Design Kit: {pdk}

module {top_module} (
    input  logic clk,
    input  logic rst_n
);

    // Design logic here

endmodule
"""
                    elif starter_type == "alu":
                        rtl_content = f"""// Silis ASIC Project: {name}
// Top Module: {top_module} (Combinational ALU)
// Process Design Kit: {pdk}

module {top_module} (
    input  logic [3:0]  opcode,
    input  logic [7:0]  op_a,
    input  logic [7:0]  op_b,
    output logic [7:0]  result,
    output logic        zero
);

    always_comb begin
        case (opcode)
            4'h0: result = op_a + op_b;
            4'h1: result = op_a - op_b;
            4'h2: result = op_a & op_b;
            4'h3: result = op_a | op_b;
            4'h4: result = op_a ^ op_b;
            default: result = 8'h00;
        endcase
        zero = (result == 8'h00);
    end

endmodule
"""
                    else:  # counter / datapath
                        rtl_content = f"""// Silis ASIC Project: {name}
// Top Module: {top_module}
// Process Design Kit: {pdk}

module {top_module} (
    input  logic        clk,
    input  logic        rst_n,
    input  logic [7:0]  data_in,
    output logic [7:0]  data_out,
    output logic        valid
);

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            data_out <= 8'h00;
            valid    <= 1'b0;
        end else begin
            data_out <= data_in + 8'h01;
            valid    <= 1'b1;
        end
    end

endmodule
"""
                    tmp_rtl = rtl_abs_path + ".tmp"
                    with open(tmp_rtl, "w", encoding="utf-8") as f:
                        f.write(rtl_content)
                    os.replace(tmp_rtl, rtl_abs_path)
                    rtl_files.append(rtl_rel_path)

                # 2. Handle SDC Constraints (matching codes.py:557)
                sdc_files = []
                sdc_rel_path = "constr/default.sdc"
                sdc_abs_path = os.path.join(proj_dir, "constr", "default.sdc")
                clock_period = float(payload.get("clock_period", 10.0))
                sdc_content = f"""# Silis Default Timing Constraints for {top_module} ({pdk})
create_clock -name clk -period {clock_period:.1f} [get_ports clk]
set_input_delay  {clock_period * 0.2:.1f} -clock clk [all_inputs]
set_output_delay {clock_period * 0.2:.1f} -clock clk [all_outputs]
"""
                tmp_sdc = sdc_abs_path + ".tmp"
                with open(tmp_sdc, "w", encoding="utf-8") as f:
                    f.write(sdc_content)
                os.replace(tmp_sdc, sdc_abs_path)
                sdc_files.append(sdc_rel_path)

                # 3. Handle Testbench (matching codes.py:550)
                tb_files = []
                tb_rel_path = f"tb/tb_{top_module}.v"
                tb_abs_path = os.path.join(proj_dir, "tb", f"tb_{top_module}.v")
                tb_content = f"""`timescale 1ns/1ps
module tb_{top_module};
    reg clk;
    reg rst_n;
    reg [7:0] data_in;
    wire [7:0] data_out;
    wire valid;

    {top_module} dut (
        .clk(clk),
        .rst_n(rst_n),
        .data_in(data_in),
        .data_out(data_out),
        .valid(valid)
    );

    initial begin
        $dumpfile("waveform.vcd");
        $dumpvars(0, tb_{top_module});
        clk = 0;
        rst_n = 0;
        data_in = 8'h00;
        #20 rst_n = 1;
        #100 $finish;
    end

    always #5 clk = ~clk;
endmodule
"""
                with open(tb_abs_path, "w", encoding="utf-8") as f:
                    f.write(tb_content)
                tb_files.append(tb_rel_path)

                # 4. Handle Power Intent UPF (matching codes.py:570)
                upf_path = os.path.join(proj_dir, "power", "design.upf")
                upf_content = f"""# IEEE 1801 UPF 3.0 Power Intent
# Project: {name} (Top: {top_module})

create_power_domain PD_TOP -include_scope
create_supply_port VDD -direction in
create_supply_port VSS -direction in
create_supply_net  VDD -domain PD_TOP
create_supply_net  VSS -domain PD_TOP
set_domain_supply_net PD_TOP -primary_power_net VDD -primary_ground_net VSS
"""
                with open(upf_path, "w", encoding="utf-8") as f:
                    f.write(upf_content)

                # 5. Handle I/O Pin Configuration (matching codes.py:580)
                io_path = os.path.join(proj_dir, "io", "pin_order.cfg")
                io_content = f"""# Silis Pin Placement Configuration
# Project: {name}

[NORTH]
clk
rst_n

[SOUTH]

[EAST]
valid

[WEST]
"""
                with open(io_path, "w", encoding="utf-8") as f:
                    f.write(io_content)

                # 6. Exact silis.silisproj contract (matching codes.py:596)
                proj_config = {
                    "project_name": name,
                    "top_module": top_module,
                    "pdk": pdk,
                    "macros": [],
                    "auto_sdc": True,
                    "rtl_files": rtl_files,
                    "sdc_files": sdc_files,
                    "tb_files": tb_files
                }
                config_file = os.path.join(proj_dir, "silis.silisproj")
                tmp_cfg = config_file + ".tmp"
                with open(tmp_cfg, "w", encoding="utf-8") as f:
                    json.dump(proj_config, f, indent=4)
                os.replace(tmp_cfg, config_file)

                # Set active project
                SilisCadHandler.active_project_dir = proj_dir

                # Register in global project_registry
                try:
                    import project_registry
                    project_registry.register_project(proj_dir)
                except Exception:
                    pass

                # Update README.md dynamically
                gen_readme_script = os.path.join(REPO_ROOT, "tools", "gen_readme.py")
                if os.path.isfile(gen_readme_script):
                    subprocess.run([sys.executable, gen_readme_script], cwd=REPO_ROOT, timeout=10)

                self._send_json(201, {
                    "status": "created",
                    "name": name,
                    "top_module": top_module,
                    "pdk": pdk,
                    "path": f"projects/{name}",
                    "rtl_file": rtl_files[0] if rtl_files else "",
                })
            except Exception as e:
                self._send_json(500, {"error": f"Failed to initialize project: {e}"})
            return

        # 2. API: Switch active project
        if path == "/api/active_project":
            target_name = payload.get("name") or payload.get("project")
            if not target_name:
                self._send_json(400, {"error": "Missing 'name' or 'project' parameter."})
                return
            target_dir = os.path.join(PROJECTS_DIR, target_name)
            if not os.path.isdir(target_dir):
                self._send_json(404, {"error": f"Project '{target_name}' not found."})
                return
            SilisCadHandler.active_project_dir = target_dir
            try:
                import project_registry
                project_registry.register_project(target_dir)
            except Exception:
                pass
            self._send_json(200, {
                "status": "ok",
                "active_project": target_name,
                "active_project_dir": target_dir,
            })
            return

        # 3. API: Load DEF file for FastSiliconPeeker 2D Layout
        if path == "/api/peeker/load_def":
            def_path = payload.get("def_path")
            if def_path and os.path.isfile(def_path):
                results_temp = os.path.join(REPO_ROOT, "results", "temp.def")
                os.makedirs(os.path.dirname(results_temp), exist_ok=True)
                shutil.copy2(def_path, results_temp)
                self._send_json(200, {"status": "ok", "loaded": def_path})
                return
            self._send_json(400, {"error": "Invalid def_path"})
            return

        # 4. API: Cross-probe to VS Code editor
        if path == "/api/cross_probe":
            try:
                file_path = payload.get("file")
                line = payload.get("line", 1)
                if file_path:
                    code_bin = shutil.which("code") or shutil.which("code-insiders")
                    if code_bin:
                        subprocess.Popen([code_bin, "--goto", f"{file_path}:{line}:1"])
                self._send_json(200, {"status": "ok"})
            except Exception as e:
                self._send_json(500, {"error": str(e)})
            return

        self.send_response(404)
        self.end_headers()

    def _serve_file(self, full_path):
        if not os.path.isfile(full_path):
            self.send_response(404)
            self.end_headers()
            return

        ext = os.path.splitext(full_path)[1].lower()
        mime_types = {
            ".html": "text/html; charset=utf-8",
            ".js": "application/javascript",
            ".mjs": "application/javascript",
            ".wasm": "application/wasm",
            ".css": "text/css",
            ".json": "application/json",
            ".svg": "image/svg+xml",
            ".png": "image/png",
            ".vcd": "text/plain",
        }
        mime = mime_types.get(ext, "application/octet-stream")

        try:
            with open(full_path, "rb") as f:
                content = f.read()
            self.send_response(200)
            self.send_header("Content-Type", mime)
            self.send_header("Content-Length", str(len(content)))
            self.send_header("Access-Control-Allow-Origin", "*")
            self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
            self.send_header("Access-Control-Allow-Headers", "*")
            self.send_header("X-Frame-Options", "ALLOWALL")
            self.send_header("Content-Security-Policy", "frame-ancestors *")
            if ext == ".wasm":
                self.send_header("Cross-Origin-Opener-Policy", "same-origin")
                self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
            self.end_headers()
            self.wfile.write(content)
        except Exception as e:
            self.send_response(500)
            self.end_headers()

    def _find_waveform(self, project_dir, target_file=None):
        if target_file and os.path.isfile(target_file) and os.path.getsize(target_file) > 0:
            with open(target_file, "r", encoding="utf-8", errors="ignore") as f:
                return f.read()

        candidates = [
            os.path.join(project_dir, "waveform.vcd"),
            os.path.join(project_dir, "results", "waveform.vcd"),
            os.path.join(project_dir, "tb", "waveform.vcd"),
        ]
        candidates.extend(glob.glob(os.path.join(project_dir, "*.vcd")))
        candidates.extend(glob.glob(os.path.join(project_dir, "**", "*.vcd"), recursive=True))

        for cand in candidates:
            if os.path.isfile(cand) and os.path.getsize(cand) > 0:
                with open(cand, "r", encoding="utf-8", errors="ignore") as f:
                    return f.read()
        return None

    def _get_or_generate_netlist(self, project_dir, module_name, target_file=None):
        if target_file and os.path.isfile(target_file) and os.path.getsize(target_file) > 0:
            with open(target_file, "r", encoding="utf-8") as f:
                return f.read()

        res_json = os.path.join(project_dir, "results", f"structural_{module_name}.json")
        if os.path.isfile(res_json) and os.path.getsize(res_json) > 0:
            with open(res_json, "r", encoding="utf-8") as f:
                return f.read()

        build_json = os.path.join(project_dir, "build", f"structural_{module_name}.json")
        if os.path.isfile(build_json) and os.path.getsize(build_json) > 0:
            with open(build_json, "r", encoding="utf-8") as f:
                return f.read()

        yosys_bin = os.path.join(REPO_ROOT, "third-party", "bin", "yosys")
        if not (os.path.isfile(yosys_bin) and os.access(yosys_bin, os.X_OK)):
            yosys_bin = shutil.which("yosys") or "yosys"

        os.makedirs(os.path.join(project_dir, "build"), exist_ok=True)
        out_json = os.path.join(project_dir, "build", f"structural_{module_name}.json")

        v_files = []
        for d in ["rtl", "source", "."]:
            p = os.path.join(project_dir, d)
            if os.path.isdir(p):
                for f in os.listdir(p):
                    if f.endswith(".v") or f.endswith(".sv"):
                        if not f.startswith("tb_"):
                            v_files.append(os.path.join(p, f))

        if not v_files:
            return None

        read_cmds = []
        for vf in v_files:
            if vf.endswith(".sv"):
                read_cmds.append(f"read_verilog -sv {vf}")
            else:
                read_cmds.append(f"read_verilog {vf}")

        ys_script = "; ".join(read_cmds) + f"; hierarchy -top {module_name}; prep -top {module_name}; write_json {out_json}"
        try:
            subprocess.run([yosys_bin, "-q", "-p", ys_script], cwd=project_dir, timeout=30, check=True)
            if os.path.isfile(out_json):
                with open(out_json, "r", encoding="utf-8") as f:
                    return f.read()
        except Exception as e:
            print(f"[Silis CS Server] Netlist generation error: {e}")

        return None


def run_server(port=8765):
    class ThreadingHTTPServer(socketserver.ThreadingMixIn, http.server.HTTPServer):
        daemon_threads = True

    server_address = ("0.0.0.0", port)
    httpd = ThreadingHTTPServer(server_address, SilisCadHandler)
    print(f"[Silis CS] CAD Web Server running at http://0.0.0.0:{port}/")
    print(f"  * Schematic Viewer: http://localhost:{port}/schematic/digitaljs.html")
    print(f"  * Waveform Viewer: http://localhost:{port}/waveforms/index.html")
    httpd.serve_forever()


if __name__ == "__main__":
    p = 8765
    if len(sys.argv) > 1 and sys.argv[1].isdigit():
        p = int(sys.argv[1])
    run_server(p)
