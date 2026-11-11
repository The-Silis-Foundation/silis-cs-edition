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
FLOORPLANNER_WEB_DIR = os.path.join(REPO_ROOT, "core", "backendflow", "floorplanner", "web")

# Add core to sys.path so we can import project_registry
CORE_DIR = os.path.join(REPO_ROOT, "core")
if CORE_DIR not in sys.path:
    sys.path.insert(0, CORE_DIR)


def resolve_project_dir(proj_arg):
    """Fuzzy-resolves any project identifier or path into an absolute project directory."""
    if not proj_arg:
        return SilisCadHandler.active_project_dir
    if os.path.isabs(proj_arg) and os.path.isdir(proj_arg):
        return proj_arg

    clean = proj_arg.replace("projects/", "").strip("/")
    cands = [
        os.path.join(REPO_ROOT, "projects", clean),
        os.path.join(REPO_ROOT, proj_arg),
        os.path.join(REPO_ROOT, clean),
    ]
    for c in cands:
        if os.path.isdir(c):
            return c

    # Fuzzy search projects/ by project_name or top_module in .silisproj
    proj_dir = os.path.join(REPO_ROOT, "projects")
    if os.path.isdir(proj_dir):
        for item in os.listdir(proj_dir):
            p = os.path.join(proj_dir, item)
            if not os.path.isdir(p):
                continue
            for sp_name in ["silis.silisproj", f"{item}.silisproj"]:
                sp = os.path.join(p, sp_name)
                if os.path.isfile(sp):
                    try:
                        with open(sp, "r", encoding="utf-8") as f:
                            meta = json.load(f)
                            if meta.get("project_name") == clean or meta.get("top_module") == clean or item == clean:
                                return p
                    except Exception:
                        pass

    if os.path.isdir(SilisCadHandler.active_project_dir):
        return SilisCadHandler.active_project_dir
    demo_dir = os.path.join(REPO_ROOT, "projects", "demo_soc")
    if os.path.isdir(demo_dir):
        return demo_dir
    return os.path.join(REPO_ROOT, proj_arg)


def parse_lef_file(lef_path):
    """Parses a LEF file to extract macro pinout, dimensions, and generate black-box Verilog stub."""
    if not os.path.isfile(lef_path):
        return None
    try:
        with open(lef_path, "r", encoding="utf-8", errors="ignore") as f:
            content = f.read()
    except Exception:
        return None

    name = os.path.splitext(os.path.basename(lef_path))[0]
    m_match = re.search(r"^\s*MACRO\s+(\S+)", content, re.MULTILINE)
    if m_match:
        name = m_match.group(1)

    sz_match = re.search(r"^\s*SIZE\s+([\d\.]+)\s+BY\s+([\d\.]+)\s*;", content, re.MULTILINE)
    w = float(sz_match.group(1)) if sz_match else 0.0
    h = float(sz_match.group(2)) if sz_match else 0.0

    pin_blocks = re.findall(r"^\s*PIN\s+(\S+)(.*?)(?:^\s*END\s+\1)", content, re.DOTALL | re.MULTILINE)
    pins = {"input": [], "output": [], "inout": [], "power": [], "ground": []}

    for pname, pbody in pin_blocks:
        dmatch = re.search(r"DIRECTION\s+(\S+)\s*;", pbody)
        umatch = re.search(r"USE\s+(\S+)\s*;", pbody)
        d = dmatch.group(1).lower() if dmatch else "input"
        u = umatch.group(1).lower() if umatch else "signal"
        if u == "power":
            pins["power"].append(pname)
        elif u == "ground":
            pins["ground"].append(pname)
        elif d == "output":
            pins["output"].append(pname)
        elif d == "inout":
            pins["inout"].append(pname)
        else:
            pins["input"].append(pname)

    def group_buses(raw_pins):
        scalars = []
        buses = {}
        for p in raw_pins:
            m = re.match(r"^(.+)\[(\d+)\]$", p)
            if m:
                bn, idx = m.group(1), int(m.group(2))
                if bn not in buses:
                    buses[bn] = []
                buses[bn].append(idx)
            else:
                scalars.append(p)
        res = []
        for bn, idxs in buses.items():
            res.append(f"[{max(idxs)}:{min(idxs)}] {bn}")
        return scalars + res

    stub = f"module {name} (\n"
    ports = []
    pwr = group_buses(pins["power"] + pins["ground"])
    if pwr:
        stub += "`ifdef USE_POWER_PINS\n"
        for p in pwr:
            stub += f"    inout {p},\n"
        stub += "`endif\n"

    for p in group_buses(pins["input"]):
        ports.append(f"    input  {p}")
    for p in group_buses(pins["output"]):
        ports.append(f"    output {p}")
    for p in group_buses(pins["inout"]):
        ports.append(f"    inout  {p}")

    stub += ",\n".join(ports) + "\n);\n"
    stub += "    // Black-box stub auto-generated by Silis IP Catalog Shopping Cart\n"
    stub += "endmodule\n"

    category = "SRAM / Memory" if ("sram" in name.lower() or "mem" in name.lower()) else "Arithmetic / Core"

    return {
        "name": name,
        "category": category,
        "lef_path": lef_path,
        "width": w,
        "height": h,
        "pins": {
            "input": group_buses(pins["input"]),
            "output": group_buses(pins["output"]),
            "inout": group_buses(pins["inout"]),
            "power": group_buses(pins["power"]),
            "ground": group_buses(pins["ground"]),
        },
        "stub": stub,
    }


def scan_available_macros(pdk_name=None):
    """Scans PDK and project directories for macro LEFs, returning parsed metadata and stubs."""
    macros = []
    seen = set()

    search_dirs = [
        os.path.join(REPO_ROOT, "pdk", "macros"),
        os.path.join(REPO_ROOT, "projects"),
        os.path.expanduser("~/.silis/macrocache"),
    ]
    if pdk_name:
        search_dirs.insert(0, os.path.join(REPO_ROOT, "pdk", "macros", pdk_name))

    for s_dir in search_dirs:
        if not os.path.isdir(s_dir):
            continue
        for lef in glob.glob(os.path.join(s_dir, "**", "*.lef"), recursive=True):
            bname = os.path.splitext(os.path.basename(lef))[0]
            if bname in seen:
                continue
            if "fd_sc" in bname or "ef_sc" in bname:
                continue
            meta = parse_lef_file(lef)
            if meta:
                seen.add(meta["name"])
                macros.append(meta)

    return macros


class SilisCadHandler(http.server.SimpleHTTPRequestHandler):
    active_project_dir = os.path.join(REPO_ROOT, "projects", "demo_soc")
    _vcd_cache = {}

    def end_headers(self):
        # Allow cross-origin and iframe embedding in VS Code Simple Browser & webviews
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "*")
        self.send_header("Cache-Control", "no-cache, no-store, must-revalidate")
        super().end_headers()

    def do_OPTIONS(self):
        self.send_response(200)
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS, HEAD")
        self.send_header("Access-Control-Allow-Headers", "*")
        self.end_headers()

    def do_HEAD(self):
        self.do_GET()

    def _send_json(self, status_code, data):
        self.send_response(status_code)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.end_headers()
        self.wfile.write(json.dumps(data, indent=2).encode("utf-8"))

    def _send_html(self, status_code, html_content):
        self.send_response(status_code)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("X-Frame-Options", "ALLOWALL")
        self.send_header("Content-Security-Policy", "frame-ancestors *")
        self.end_headers()
        self.wfile.write(html_content.encode("utf-8"))

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

        # 1b. API: List available macros for IP Catalog Shopping Cart
        if path == "/api/macros":
            pdk_name = query.get("pdk", ["sky130"])[0]
            macro_list = scan_available_macros(pdk_name)
            self._send_json(200, macro_list)
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
            project_dir = resolve_project_dir(query.get("project", [""])[0])
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
            project_dir = resolve_project_dir(query.get("project", [""])[0])
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

        # 5b. API: Query wire values at timestamp for cross-probing (selective signal evaluation)
        if path == "/api/waveform/values":
            target_file = query.get("file", [None])[0]
            project_dir = resolve_project_dir(query.get("project", [""])[0])
            try:
                time_ps = int(query.get("time", [0])[0])
            except ValueError:
                time_ps = 0
            signals_str = query.get("signals", [None])[0]
            selected_signals = set(s.strip() for s in signals_str.split(",") if s.strip()) if signals_str else None

            vcd_path = self._locate_vcd_path(project_dir, target_file)
            if not vcd_path:
                self._send_json(404, {"error": "No VCD found for values"})
                return

            try:
                mtime = os.path.getmtime(vcd_path)
                cached = SilisCadHandler._vcd_cache.get(vcd_path)
                if not cached or cached["mtime"] != mtime:
                    from signalpeeker.vcd_tracker import FastVcdTracker
                    tracker = FastVcdTracker(vcd_path)
                    SilisCadHandler._vcd_cache[vcd_path] = {"tracker": tracker, "mtime": mtime}
                else:
                    tracker = cached["tracker"]

                updates_dict = tracker.get_all_values_at_ps(time_ps)
                if selected_signals:
                    filtered = {}
                    for k, v in updates_dict.items():
                        leaf = k.split(".")[-1]
                        if k in selected_signals or leaf in selected_signals:
                            filtered[k] = v
                            filtered[leaf] = v
                    updates_dict = filtered

                updates_list = [{"name": k, "values": [v]} for k, v in updates_dict.items()]
                self._send_json(200, updates_list)
            except Exception as e:
                self._send_json(500, {"error": str(e)})
            return

        # 5c. API: Run Verilator RTL Lint (Ctrl+L)
        if path == "/api/lint":
            project_dir = resolve_project_dir(query.get("project", [""])[0])
            lint_res = self._run_lint(project_dir)
            self._send_json(200, lint_res)
            return

        # 5d. API: Get Floorplan session
        if path == "/api/floorplan/session":
            proj_dir = resolve_project_dir(query.get("project", [""])[0])
            proj_name = os.path.basename(proj_dir)
            sess_file = os.path.join(proj_dir, ".silis_fp_session.json")
            if os.path.isfile(sess_file):
                try:
                    with open(sess_file, "r", encoding="utf-8") as f:
                        data = json.load(f)
                    self._send_json(200, data)
                    return
                except Exception as e:
                    self._send_json(500, {"error": str(e)})
                    return
            self._send_json(200, {"status": "none", "project": proj_name})
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

        if path.startswith("/floorplanner/") or path == "/floorplanner" or path == "/floorplanner.html":
            rel_path = path.replace("/floorplanner/", "").replace("/floorplanner.html", "").replace("/floorplanner", "").lstrip("/")
            local_file = os.path.join(FLOORPLANNER_WEB_DIR, rel_path if rel_path else "index.html")
            return self._serve_file(local_file)

        # Root endpoint: if requested by browser/iframe (text/html), serve session restorer hub
        if path == "/" or path == "":
            accept_header = self.headers.get("Accept", "")
            if "text/html" in accept_header:
                recovery_html = """<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Silis CAD Engine Hub</title>
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; background: #1e1e2e; color: #cdd6f4; margin: 0; padding: 40px; }
        .card { max-width: 620px; margin: 0 auto; background: #181825; border: 1px solid #313244; border-radius: 8px; padding: 28px; text-align: center; }
        h1 { font-size: 20px; color: #89b4fa; margin: 0 0 8px 0; }
        p { font-size: 13px; color: #a6adc8; line-height: 1.6; margin: 0 0 20px 0; }
        .spinner { border: 3px solid #313244; border-top: 3px solid #89b4fa; border-radius: 50%; width: 28px; height: 28px; animation: spin 1s linear infinite; margin: 0 auto 20px auto; }
        @keyframes spin { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } }
        .btn-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 12px; margin-top: 20px; }
        a.btn { display: block; padding: 12px 16px; background: #313244; color: #cdd6f4; text-decoration: none; border-radius: 6px; font-size: 13px; font-weight: 600; text-align: center; border: 1px solid #45475a; }
        a.btn:hover { background: #45475a; color: #89b4fa; border-color: #89b4fa; }
    </style>
    <script>
        (function() {
            try {
                if (window.parent && window.parent !== window) {
                    window.parent.postMessage({ type: 'silisHubReady' }, '*');
                }
            } catch(e) {}

            window.addEventListener('message', function(e) {
                if (e.data && e.data.type === 'silisRedirect' && e.data.target) {
                    window.location.replace(e.data.target);
                }
            });

            try {
                if (window.name && window.name.indexOf('silis-target:') === 0) {
                    var target = decodeURIComponent(window.name.substring('silis-target:'.length));
                    if (target && target !== '/' && target !== window.location.href) {
                        window.location.replace(target);
                        return;
                    }
                }
            } catch(e) {}
            try {
                var params = new URLSearchParams(window.location.search);
                var redir = params.get('redirect');
                if (redir) {
                    window.location.replace(redir);
                    return;
                }
            } catch(e) {}
            try {
                if (document.referrer && document.referrer.indexOf('-8765.app.github.dev') !== -1) {
                    var refUrl = new URL(document.referrer);
                    if (refUrl.pathname && refUrl.pathname !== '/' && refUrl.pathname !== '') {
                        window.location.replace(document.referrer);
                        return;
                    }
                }
            } catch(e) {}
        })();
    </script>
</head>
<body>
    <div class="card">
        <h1>Silis CAD Web Engine Hub</h1>
        <div class="spinner"></div>
        <p>Restoring CAD workspace session...</p>
        <div class="btn-grid">
            <a class="btn" href="/schematic/digitaljs.html?project=projects/demo_soc">Schematic (DigitalJS)</a>
            <a class="btn" href="/rohd/index.html?project=projects/demo_soc">Waveforms (ROHD)</a>
            <a class="btn" href="/peeker/index.html?project=projects/demo_soc">2D Layout (FastSiliconPeeker)</a>
            <a class="btn" href="/floorplanner/index.html?project=projects/demo_soc">Floorplan &amp; Power Studio (FPS)</a>
        </div>
    </div>
</body>
</html>"""
                self._send_html(200, recovery_html)
                return

            self._send_json(200, {"status": "ok", "service": "silis-cs-server", "version": "1.0.0"})
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

                # 1. Route and copy files (100% parity with codes.py:route_files)
                rtl_files = []
                raw_rtl = payload.get("rtl_files", payload.get("copy_files", []))
                for item in raw_rtl:
                    if str(item).startswith("[NEW] "):
                        fname = str(item)[6:].strip()
                        fpath = os.path.join(proj_dir, "rtl", fname)
                        mod_name = os.path.splitext(fname)[0]
                        with open(fpath, "w", encoding="utf-8") as nf:
                            nf.write("// Module: " + mod_name + "\nmodule " + mod_name + " (\n    input logic clk,\n    input logic rst_n\n);\n\nendmodule\n")
                        rtl_files.append(f"rtl/{fname}")
                    elif os.path.isfile(item):
                        dst = os.path.join(proj_dir, "rtl", os.path.basename(item))
                        shutil.copy2(item, dst)
                        rtl_files.append(f"rtl/{os.path.basename(item)}")
                    elif os.path.isdir(item):
                        for root, _, fs in os.walk(item):
                            for f in fs:
                                if f.endswith((".v", ".sv", ".vhd", ".vhdl")):
                                    src_f = os.path.join(root, f)
                                    dst = os.path.join(proj_dir, "rtl", f)
                                    shutil.copy2(src_f, dst)
                                    rtl_files.append(f"rtl/{f}")
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

                # 2. Handle SDC Constraints (matching codes.py:549-559)
                sdc_files = []
                for sdc_item in payload.get("sdc_files", []):
                    if os.path.isfile(sdc_item):
                        dst = os.path.join(proj_dir, "constr", os.path.basename(sdc_item))
                        shutil.copy2(sdc_item, dst)
                        sdc_files.append(f"constr/{os.path.basename(sdc_item)}")
                    elif str(sdc_item).startswith("[NEW] "):
                        sfname = str(sdc_item)[6:].strip()
                        sfpath = os.path.join(proj_dir, "constr", sfname)
                        with open(sfpath, "w", encoding="utf-8") as sff:
                            sff.write("create_clock -name clk -period 10.0 [get_ports clk]\n")
                        sdc_files.append(f"constr/{sfname}")

                if not sdc_files:
                    sdc_rel_path = "constr/default.sdc"
                    sdc_abs_path = os.path.join(proj_dir, "constr", "default.sdc")
                    clock_period = float(payload.get("clock_period", 10.0))
                    sdc_content = f"""# Silis Default Timing Constraints for {top_module} ({pdk})
create_clock -name clk -period {clock_period:.1f} [get_ports clk]
set_input_delay  {clock_period * 0.2:.1f} -clock clk [all_inputs]
set_output_delay {clock_period * 0.2:.1f} -clock clk [all_outputs]
"""
                    with open(sdc_abs_path, "w", encoding="utf-8") as f:
                        f.write(sdc_content)
                    sdc_files.append(sdc_rel_path)

                # 3. Handle Testbench (matching codes.py:550)
                tb_files = []
                for tb_item in payload.get("tb_files", []):
                    if os.path.isfile(tb_item):
                        dst = os.path.join(proj_dir, "tb", os.path.basename(tb_item))
                        shutil.copy2(tb_item, dst)
                        tb_files.append(f"tb/{os.path.basename(tb_item)}")
                    elif str(tb_item).startswith("[NEW] "):
                        tfname = str(tb_item)[6:].strip()
                        tfpath = os.path.join(proj_dir, "tb", tfname)
                        with open(tfpath, "w", encoding="utf-8") as tff:
                            tff.write(f"`timescale 1ns/1ps\nmodule tb_{top_module};\n    initial begin\n        $dumpfile(\"waveform.vcd\");\n        $dumpvars(0, tb_{top_module});\n        #100 $finish;\n    end\nendmodule\n")
                        tb_files.append(f"tb/{tfname}")

                if not tb_files:
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

                # 4. Handle Power Intent UPF (matching codes.py:567-571)
                upf_path = os.path.join(proj_dir, "power", "design.upf")
                upf_user = payload.get("upf_file", "").strip()
                if upf_user and os.path.isfile(upf_user):
                    shutil.copy2(upf_user, upf_path)
                else:
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

                # 5. Handle I/O Pin Configuration (matching codes.py:575-580)
                io_path = os.path.join(proj_dir, "io", "pin_order.cfg")
                io_user = payload.get("io_file", "").strip()
                if io_user and os.path.isfile(io_user):
                    shutil.copy2(io_user, io_path)
                else:
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

                # 6. Process Checked-Out Macros from IP Catalog Shopping Cart
                checked_macros = []
                for m_item in payload.get("macros", []):
                    m_name = m_item.get("name")
                    if not m_name:
                        continue
                    m_stub = m_item.get("stub", "")
                    auto_stub = m_item.get("auto_stub", True)
                    if auto_stub and m_stub:
                        stub_path = os.path.join(proj_dir, "rtl", f"{m_name}_stub.v")
                        with open(stub_path, "w", encoding="utf-8") as sf:
                            sf.write(m_stub)
                        rtl_files.append(f"rtl/{m_name}_stub.v")
                    lef_path = m_item.get("lef_path")
                    if lef_path and os.path.isfile(lef_path):
                        dst_lef = os.path.join(proj_dir, "rtl", os.path.basename(lef_path))
                        try:
                            shutil.copy2(lef_path, dst_lef)
                        except Exception:
                            pass
                    checked_macros.append({
                        "name": m_name,
                        "category": m_item.get("category", "Macro"),
                        "width": m_item.get("width", 0.0),
                        "height": m_item.get("height", 0.0),
                        "lef": f"rtl/{os.path.basename(lef_path)}" if (lef_path and os.path.isfile(lef_path)) else "",
                    })

                # 7. Exact silis.silisproj contract (matching codes.py:596)
                proj_config = {
                    "project_name": name,
                    "top_module": top_module,
                    "pdk": pdk,
                    "macros": checked_macros,
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

        # 5. API: Floorplan session save / load
        if path == "/api/floorplan/session":
            proj_arg = payload.get("project") or ""
            proj_dir = resolve_project_dir(proj_arg)
            sess_file = os.path.join(proj_dir, ".silis_fp_session.json")
            try:
                sess_data = payload.get("session", payload)
                tmp_file = sess_file + ".tmp"
                with open(tmp_file, "w", encoding="utf-8") as f:
                    json.dump(sess_data, f, indent=2)
                os.replace(tmp_file, sess_file)
                self._send_json(200, {"status": "saved", "file": sess_file})
            except Exception as e:
                self._send_json(500, {"error": str(e)})
            return

        # 6. API: Export OpenROAD TCL
        if path == "/api/floorplan/export_tcl":
            sess_data = payload.get("session", payload)
            try:
                tcl_code = _generate_floorplan_tcl(sess_data)
                self._send_json(200, {"status": "ok", "tcl": tcl_code})
            except Exception as e:
                self._send_json(500, {"error": str(e)})
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

    def _locate_vcd_path(self, project_dir, target_file=None):
        if target_file and os.path.isfile(target_file) and os.path.getsize(target_file) > 0:
            return target_file

        candidates = [
            os.path.join(project_dir, "waveform.vcd"),
            os.path.join(project_dir, "results", "waveform.vcd"),
            os.path.join(project_dir, "tb", "waveform.vcd"),
        ]
        candidates.extend(glob.glob(os.path.join(project_dir, "*.vcd")))
        candidates.extend(glob.glob(os.path.join(project_dir, "**", "*.vcd"), recursive=True))

        for cand in candidates:
            if os.path.isfile(cand) and os.path.getsize(cand) > 0:
                return cand
        return None

    def _find_waveform(self, project_dir, target_file=None):
        cand = self._locate_vcd_path(project_dir, target_file)
        if cand:
            with open(cand, "r", encoding="utf-8", errors="ignore") as f:
                return f.read()
        return None

    def _run_lint(self, project_dir):
        verilator_bin = os.path.join(REPO_ROOT, "third-party", "bin", "verilator")
        if not (os.path.isfile(verilator_bin) and os.access(verilator_bin, os.X_OK)):
            verilator_bin = shutil.which("verilator") or "verilator"

        v_files = []
        for d in ["rtl", "source", "."]:
            p = os.path.join(project_dir, d)
            if os.path.isdir(p):
                for f in os.listdir(p):
                    if f.endswith((".v", ".sv")) and not f.startswith("tb_"):
                        v_files.append(os.path.join(p, f))

        if not v_files:
            return {"exit_code": 1, "stdout": "", "stderr": "No RTL source files found to lint.", "success": False}

        cmd = [verilator_bin, "--lint-only", "-sv", "-Wall", "-Wno-fatal"]
        for inc in [os.path.join(project_dir, "rtl"), os.path.join(project_dir, "source"), project_dir]:
            if os.path.isdir(inc):
                cmd.append(f"-I{inc}")
        cmd.extend(v_files)

        try:
            res = subprocess.run(cmd, cwd=project_dir, capture_output=True, text=True, timeout=30)
            output = res.stderr or res.stdout or ""
            return {
                "exit_code": res.returncode,
                "stdout": res.stdout,
                "stderr": res.stderr,
                "output": output,
                "success": res.returncode == 0
            }
        except Exception as e:
            return {"exit_code": -1, "stdout": "", "stderr": str(e), "output": str(e), "success": False}

    def _get_or_generate_netlist(self, project_dir, module_name, target_file=None):
        if target_file and os.path.isfile(target_file) and os.path.getsize(target_file) > 0:
            with open(target_file, "r", encoding="utf-8") as f:
                return f.read()

        for cand in [
            os.path.join(project_dir, "build", "structural_synth.json"),
            os.path.join(project_dir, "build", f"structural_{module_name}.json"),
            os.path.join(project_dir, "results", f"structural_{module_name}.json"),
        ] + glob.glob(os.path.join(project_dir, "build", "structural_*.json")) + glob.glob(os.path.join(project_dir, "results", "structural_*.json")):
            if os.path.isfile(cand) and os.path.getsize(cand) > 0:
                with open(cand, "r", encoding="utf-8") as f:
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

        ys_script = "; ".join(read_cmds) + f"; hierarchy -auto-top; prep; write_json {out_json}"
        try:
            subprocess.run([yosys_bin, "-q", "-p", ys_script], cwd=project_dir, timeout=30, check=True)
            if os.path.isfile(out_json):
                with open(out_json, "r", encoding="utf-8") as f:
                    return f.read()
        except Exception as e:
            print(f"[Silis CS Server] Netlist generation error: {e}")

        return None


def _generate_floorplan_tcl(session: dict) -> str:
    """Synthesizes an OpenROAD-compliant floorplan script from session JSON."""
    lines = ["# Silis CS Edition - Synthesized OpenROAD Floorplan", ""]
    die_w = float(session.get("die_w", 500.0))
    die_h = float(session.get("die_h", 500.0))
    ml = float(session.get("marg_l", 15.0))
    mb = float(session.get("marg_b", 15.0))
    mr = float(session.get("marg_r", 15.0))
    mt = float(session.get("marg_t", 15.0))
    cx1 = round(ml, 3)
    cy1 = round(mb, 3)
    cx2 = round(die_w - mr, 3)
    cy2 = round(die_h - mt, 3)

    lines.append(f"initialize_floorplan -die_area {{ 0.000 0.000 {die_w:.3f} {die_h:.3f} }} " + chr(92))
    lines.append(f"                     -core_area {{ {cx1:.3f} {cy1:.3f} {cx2:.3f} {cy2:.3f} }} " + chr(92))
    lines.append("                     -site unithd")
    lines.append("")

    for m in session.get("macros", []):
        iname = m.get("inst_name", m.get("name", "macro"))
        master = m.get("master", iname)
        x = float(m.get("x", 0.0))
        y = float(m.get("y", 0.0))
        orient = m.get("orient", "R0")
        lines.append(f"place_cell -inst_name {iname} -master {master} -origin {{ {x:.3f} {y:.3f} }} -orient {orient} -status FIRM")

    if session.get("macros"):
        lines.append("")

    for dom in session.get("domains", []):
        dname = dom.get("name", "PD_TOP")
        lines.append(f"create_power_domain {dname}")
        area = dom.get("area")
        if area and len(area) == 4:
            lines.append(f"set_domain_area {dname} -area {{ {float(area[0]):.3f} {float(area[1]):.3f} {float(area[2]):.3f} {float(area[3]):.3f} }}")

    lines.append("")
    return chr(10).join(lines)


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
