#!/usr/bin/env python3
"""
Silis CS Edition CLI & Workspace Orchestrator
Handles `silis [project]` to configure VS Code workspace, launch local CAD daemons,
and open the canonical frontend format:
Fileselector | Code Editor | Schematic Web GUI on top,
Tabbed Terminal / ROHD Waveform Viewer on the bottom drawer.
"""

import os
import sys
import json
import shutil
import subprocess
import time
from pathlib import Path

def find_top_rtl_file(project_root: str, project_config: dict = None) -> tuple[str, str]:
    """
    Finds (top_module_name, top_file_path).
    """
    top_name = None
    if project_config:
        top_name = project_config.get("top_module")

    # Search candidates in source/, rtl/, or root
    candidates = []
    for d in ["source", "rtl", "."]:
        dir_path = os.path.join(project_root, d)
        if os.path.isdir(dir_path):
            for fname in os.listdir(dir_path):
                if (fname.endswith(".v") or fname.endswith(".sv") or fname.endswith(".vhd")) and not fname.startswith("tb_"):
                    candidates.append(os.path.join(dir_path, fname))

    if not candidates:
        return (top_name or "top", None)

    # If top_name is known, search for matching filename
    if top_name:
        for c in candidates:
            base = os.path.splitext(os.path.basename(c))[0]
            if base == top_name:
                return (top_name, c)

    # Inspect contents for module declaration
    import re
    for c in candidates:
        try:
            with open(c, "r", encoding="utf-8", errors="ignore") as f:
                content = f.read()
                matches = re.findall(r'\b(?:module|entity)\s+([a-zA-Z_][a-zA-Z0-9_$]*)', content)
                if matches:
                    if top_name and top_name in matches:
                        return (top_name, c)
                    if not top_name:
                        return (matches[0], c)
        except Exception:
            pass

    # Fallback to the first candidate
    first = candidates[0]
    base = os.path.splitext(os.path.basename(first))[0]
    return (top_name or base, first)


def ensure_cs_cad_server():
    """Starts core/server/cs_server.py in background if not already running."""
    import urllib.request
    try:
        urllib.request.urlopen("http://127.0.0.1:8765/", timeout=0.5)
        return True
    except Exception:
        pass

    repo_root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
    server_script = os.path.join(repo_root, "core", "server", "cs_server.py")
    if os.path.isfile(server_script):
        subprocess.Popen([sys.executable, server_script, "8765"], start_new_session=True)
        time.sleep(0.5)
        return True
    return False


def setup_vscode_workspace(project_root: str, top_file: str, top_module: str):
    """
    Configures .vscode/settings.json and .vscode/tasks.json for seamless Codespaces CAD usage.
    """
    vscode_dir = os.path.join(project_root, ".vscode")
    os.makedirs(vscode_dir, exist_ok=True)

    settings_file = os.path.join(vscode_dir, "settings.json")
    settings = {
        "workbench.editor.showTabs": "multiple",
        "workbench.editor.openSideBySideDirection": "right",
        "files.associations": {
            "*.v": "verilog",
            "*.sv": "systemverilog",
            "*.sdc": "tcl",
            "*.upf": "tcl",
            "*.silisproj": "json"
        },
        "simpleBrowser.focusOnOpen": False
    }
    if os.path.isfile(settings_file):
        try:
            with open(settings_file, "r") as f:
                existing = json.load(f)
                existing.update(settings)
                settings = existing
        except Exception:
            pass

    with open(settings_file, "w") as f:
        json.dump(settings, f, indent=2)

    tasks_file = os.path.join(vscode_dir, "tasks.json")
    tasks = {
        "version": "2.0.0",
        "tasks": [
            {
                "label": "Silis: Open Schematic Viewer",
                "type": "shell",
                "command": f"code --command simpleBrowser.show http://localhost:8765/schematic/digitaljs.html?module={top_module}",
                "presentation": {"reveal": "silent"},
                "problemMatcher": []
            },
            {
                "label": "Silis: Open ROHD Waveforms",
                "type": "shell",
                "command": "code --command simpleBrowser.show http://localhost:8765/waveforms/index.html",
                "presentation": {"reveal": "silent"},
                "problemMatcher": []
            },
            {
                "label": "Silis: Run Synthesis (Yosys)",
                "type": "shell",
                "command": "silis --run-synth",
                "group": "build",
                "problemMatcher": []
            },
            {
                "label": "Silis: Run Full Tapeout Flow (OpenROAD)",
                "type": "shell",
                "command": "silis --run-all",
                "group": {"kind": "build", "isDefault": True},
                "problemMatcher": []
            }
        ]
    }
    with open(tasks_file, "w") as f:
        json.dump(tasks, f, indent=2)


def launch_project(project_target: str) -> bool:
    """
    Main entry point for `silis [project]`.
    """
    abs_target = os.path.abspath(project_target)
    if os.path.isfile(abs_target):
        project_root = os.path.dirname(abs_target)
        proj_file = abs_target
    else:
        project_root = abs_target
        proj_file = os.path.join(project_root, "silis.silisproj")

    if not os.path.isdir(project_root):
        print(f"[Silis] Directory not found: {project_root}")
        return False

    project_config = {}
    if os.path.isfile(proj_file):
        try:
            with open(proj_file, "r") as f:
                project_config = json.load(f)
        except Exception as e:
            print(f"[Silis] Warning reading project file: {e}")

    top_module, top_file = find_top_rtl_file(project_root, project_config)
    
    # 1. Start local CAD server for Schematic & Waveforms
    ensure_cs_cad_server()

    # 2. Inject .vscode configuration
    setup_vscode_workspace(project_root, top_file, top_module)

    # 3. Command VS Code
    code_bin = shutil.which("code") or shutil.which("code-insiders")
    print("=================================================================")
    print(f"SILIS CS WORKSPACE: {os.path.basename(project_root)}")
    print(f"  • Top Module: {top_module}")
    if top_file:
        print(f"  • Primary Source: {os.path.relpath(top_file, project_root)}")
    print("  • Schematic Viewer: http://localhost:8765/schematic/digitaljs.html")
    print("  • Waveform Viewer:  http://localhost:8765/waveforms/index.html")
    print("  • Layout Peeker:    http://localhost:6081/vnc_lite.html?autoconnect=true&resize=scale")
    print("  • Floorplan Studio: http://localhost:6084/vnc_lite.html?autoconnect=true&resize=scale")
    print("=================================================================")

    if code_bin:
        # Open project folder and top source file
        cmd = [code_bin, "-r", project_root]
        if top_file and os.path.isfile(top_file):
            cmd.extend(["-g", f"{top_file}:1:1"])
        subprocess.run(cmd)

        # Open Schematic Web GUI side-by-side
        schem_url = f"http://localhost:8765/schematic/digitaljs.html?module={top_module}&project={project_root}"
        workbench_url = f"http://localhost:8765/workbench?module={top_module}&project={project_root}"
        
        # Open simpleBrowser for both side-by-side Schematic and workbench
        try:
            subprocess.run([code_bin, "--command", "simpleBrowser.show", schem_url])
        except Exception:
            pass
        return True
    else:
        print("[Silis] VS Code CLI ('code') not detected in PATH. Ready in headless mode.")
        return True
