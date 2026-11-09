#!/usr/bin/env python3
"""
Silis CS Edition Local CAD Web Server
Serves DigitalJS Schematic Viewer and ROHD Waveform Viewer with zero X11/VNC overhead.
Provides REST endpoints for live Verilog netlist synthesis and cross-probing to VS Code.
"""

import os
import sys
import json
import shutil
import subprocess
import http.server
import socketserver
import urllib.parse
from pathlib import Path

# Paths
REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
SCHEMATIC_DIR = os.path.join(REPO_ROOT, "core", "schematicviewer")
ROHD_DIR = os.path.join(REPO_ROOT, "core", "signalpeeker", "web", "rohd")

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

    def do_GET(self):
        parsed = urllib.parse.urlparse(self.path)
        path = parsed.path
        query = urllib.parse.parse_qs(parsed.query)

        # 1. API: Get Netlist JSON for Schematic
        if path == "/api/netlist":
            module_name = query.get("module", ["chip_top"])[0]
            project_dir = query.get("project", [SilisCadHandler.active_project_dir])[0]
            data = self._get_or_generate_netlist(project_dir, module_name)
            if data:
                self.send_response(200)
                self.send_header("Content-Type", "application/json")
                self.end_headers()
                self.wfile.write(data.encode("utf-8") if isinstance(data, str) else data)
            else:
                self.send_response(500)
                self.send_header("Content-Type", "application/json")
                self.end_headers()
                self.wfile.write(json.dumps({"error": "Failed to synthesize netlist"}).encode("utf-8"))
            return

        # 2. API: Get VCD Waveform text
        if path == "/api/waveform":
            project_dir = query.get("project", [SilisCadHandler.active_project_dir])[0]
            vcd_content = self._find_waveform(project_dir)
            if vcd_content:
                self.send_response(200)
                self.send_header("Content-Type", "text/plain; charset=utf-8")
                self.end_headers()
                self.wfile.write(vcd_content.encode("utf-8"))
            else:
                self.send_response(404)
                self.send_header("Content-Type", "application/json")
                self.end_headers()
                self.wfile.write(json.dumps({"error": "No VCD waveform found"}).encode("utf-8"))
            return

        # 3. Route static files:
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
body { font-family: system-ui, sans-serif; background: #181825; color: #cdd6f4; padding: 2rem; }
a { color: #89b4fa; text-decoration: none; font-size: 1.1rem; }
a:hover { text-decoration: underline; }
ul { line-height: 2; }
</style>
</head>
<body>
<h2>Silis CS Edition - Native CAD Web Engines</h2>
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

        if path == "/api/cross_probe":
            length = int(self.headers.get("Content-Length", 0))
            body = self.rfile.read(length).decode("utf-8") if length > 0 else "{}"
            try:
                msg = json.loads(body)
                file_path = msg.get("file")
                line = msg.get("line", 1)
                if file_path:
                    code_bin = shutil.which("code") or shutil.which("code-insiders")
                    if code_bin:
                        subprocess.Popen([code_bin, "--goto", f"{file_path}:{line}:1"])
                self.send_response(200)
                self.send_header("Content-Type", "application/json")
                self.end_headers()
                self.wfile.write(b'{"status":"ok"}')
            except Exception as e:
                self.send_response(500)
                self.end_headers()
                self.wfile.write(json.dumps({"error": str(e)}).encode("utf-8"))
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
            if ext == ".wasm":
                self.send_header("Cross-Origin-Opener-Policy", "same-origin")
                self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
            self.end_headers()
            self.wfile.write(content)
        except Exception as e:
            self.send_response(500)
            self.end_headers()

    def _find_waveform(self, project_dir):
        candidates = [
            os.path.join(project_dir, "waveform.vcd"),
            os.path.join(project_dir, "results", "waveform.vcd"),
            os.path.join(project_dir, "tb", "waveform.vcd"),
        ]
        import glob
        candidates.extend(glob.glob(os.path.join(project_dir, "*.vcd")))
        candidates.extend(glob.glob(os.path.join(project_dir, "**", "*.vcd"), recursive=True))

        for cand in candidates:
            if os.path.isfile(cand) and os.path.getsize(cand) > 0:
                with open(cand, "r", encoding="utf-8", errors="ignore") as f:
                    return f.read()
        return None

    def _get_or_generate_netlist(self, project_dir, module_name):
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
        for d in ["source", "rtl", "."]:
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
    print(f"  • Schematic Viewer: http://localhost:{port}/schematic/digitaljs.html")
    print(f"  • Waveform Viewer: http://localhost:{port}/waveforms/index.html")
    httpd.serve_forever()

if __name__ == "__main__":
    p = 8765
    if len(sys.argv) > 1 and sys.argv[1].isdigit():
        p = int(sys.argv[1])
    run_server(p)
