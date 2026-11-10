#!/usr/bin/env python3
"""
Silis CS Edition -- Dynamic README Project Table & Control Center Generator.
Scans projects/ directory and updates the PROJECTS section in README.md.
Called automatically by postStartCommand on Codespace boot and after project creation.
"""

import os
import re
import sys
import json
import urllib.parse
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
PROJECTS_DIR = REPO_ROOT / "projects"
README_PATH = REPO_ROOT / "README.md"

START_MARKER = "<!-- PROJECTS:START -->"
END_MARKER = "<!-- PROJECTS:END -->"


def discover_projects():
    """Return list of project metadata dicts from projects/*/silis.silisproj."""
    projects = []
    if not PROJECTS_DIR.is_dir():
        return projects

    for proj_dir in sorted(PROJECTS_DIR.iterdir()):
        if not proj_dir.is_dir() or proj_dir.name.startswith("."):
            continue

        silisproj_file = None
        for f in proj_dir.iterdir():
            if f.suffix == ".silisproj":
                silisproj_file = f
                break

        meta = {}
        if silisproj_file and silisproj_file.is_file():
            try:
                with open(silisproj_file, "r", encoding="utf-8") as fh:
                    meta = json.load(fh)
            except Exception as e:
                print(f"[gen_readme] Warning: Could not read {silisproj_file}: {e}", file=sys.stderr)

        # Check for layout and waveform artifacts
        has_def = any(proj_dir.rglob("*.def"))
        has_vcd = any(proj_dir.rglob("*.vcd")) or any(proj_dir.rglob("*.fst"))
        rtl_files = meta.get("rtl_files", [])

        projects.append({
            "name": proj_dir.name,
            "top_module": meta.get("top_module", proj_dir.name),
            "pdk": meta.get("pdk", "sky130"),
            "has_def": has_def,
            "has_vcd": has_vcd,
            "rtl_count": len(rtl_files) if isinstance(rtl_files, list) else 0,
        })

    return projects


def encode_arg(s):
    """Encode argument for VS Code command URI: command:silis.action?quoted_json."""
    return urllib.parse.quote(json.dumps(s))


def build_dashboard_markdown(projects):
    """Generate the Markdown content for the project dashboard."""
    lines = []
    lines.append(START_MARKER)
    lines.append("")
    lines.append("## Workbench Guide & Control Center")
    lines.append("")
    lines.append("> [!IMPORTANT]")
    lines.append("> **How to Navigate Silis in VS Code Web / GitHub Codespaces**:")
    lines.append("> 1. **Activity Bar (Left Sidebar)**: Click the **Silis CAD** icon (circuit-board icon on the leftmost bar) to access the interactive Control Center, create new projects with the authentic 6-stage wizard, and open visual CAD surfaces.")
    lines.append("> 2. **Run Simulation (Play Button)**: Click the **Play button** on the top right editor title bar (or run `Silis: Compile & Run Simulation`) to compile design logic, open the DigitalJS schematic side-by-side on the right (`ViewColumn.Beside`), and load the waveform inspector in the bottom panel dock.")
    lines.append("> 3. **Lint RTL (Debug Button / Ctrl+L)**: Click the **Bug button** on the editor title bar or press `Ctrl+L` to run strict Verilator syntax and design rule checks.")
    lines.append("> 4. **Tri-Way Cross-Probing**:")
    lines.append(">    - **Waveform -> Schematic**: Scrubbing the waveform cursor at timestamp `t` live-evaluates and annotates wire logic levels in the schematic.")
    lines.append(">    - **Schematic -> Waveform**: Right-click any wire/gate and select *Show in waveform* to add and highlight the signal in the bottom waveform dock.")
    lines.append(">    - **Schematic -> RTL Editor**: Right-click any component and select *Go to source* / *Go to definition* to navigate directly to the declaration in the left editor.")
    lines.append("")
    lines.append("### Registered ASIC Projects")
    lines.append("")

    if not projects:
        lines.append("> [!NOTE]")
        lines.append("> No ASIC projects detected in `projects/`. Use the **Silis CAD** sidebar to initialize your first project scaffold.")
        lines.append("")
    else:
        lines.append("| Project | Process (PDK) | Top Module | Layout (DEF) | Waveforms | Source Files |")
        lines.append("| :--- | :--- | :--- | :--- | :--- | :--- |")
        for p in projects:
            name = p["name"]
            pdk = p["pdk"]
            top = p["top_module"]
            layout_status = "Available" if p["has_def"] else "Pending PnR"
            wave_status = "Available" if p["has_vcd"] else "Pending Sim"
            rtl_count = f"{p['rtl_count']} file(s)" if p.get('rtl_count') else "Ready"
            lines.append(f"| **{name}** | `{pdk}` | `{top}` | {layout_status} | {wave_status} | {rtl_count} |")
        lines.append("")

    lines.append("### Key Commands (`Ctrl+Shift+P`)")
    lines.append("")
    lines.append("- `Silis: Open Project` -- Select active project, focus directory in Explorer, and open top RTL on left")
    lines.append("- `Silis: Compile & Run Simulation` -- Compile design, display schematic split beside RTL, focus bottom waveform dock")
    lines.append("- `Silis: Lint RTL (Ctrl+L)` -- Run Verilator strict RTL lint checks")
    lines.append("- `Silis: Open 2D Layout Canvas` -- Open FastSiliconPeeker 2D DEF layout canvas")
    lines.append("- `Silis: Open Floorplan & Power Studio` -- Open multi-domain UPF floorplanning canvas")
    lines.append("- `Silis: Run Synthesis (Yosys)` -- Synthesize active project into gate-level netlist")
    lines.append("- `Silis: Run Tapeout Flow (OpenROAD)` -- Execute floorplan, CTS, placement, and routing")
    lines.append("")
    lines.append(END_MARKER)

    return "\n".join(lines)


def update_readme(projects):
    """Atomically update README.md with the latest projects dashboard."""
    if not README_PATH.is_file():
        print(f"[gen_readme] Error: README.md not found at {README_PATH}", file=sys.stderr)
        return False

    content = README_PATH.read_text(encoding="utf-8")
    dashboard_md = build_dashboard_markdown(projects)

    if START_MARKER in content and END_MARKER in content:
        pattern = re.compile(
            re.escape(START_MARKER) + r".*?" + re.escape(END_MARKER),
            re.DOTALL
        )
        new_content = pattern.sub(dashboard_md, content)
    else:
        # Insert right after the top header banner and first divider
        # Look for first '---'
        first_divider = content.find("\n---\n")
        if first_divider != -1:
            split_pos = first_divider + len("\n---\n")
            new_content = content[:split_pos] + "\n" + dashboard_md + "\n\n---\n" + content[split_pos:]
        else:
            new_content = content.rstrip() + "\n\n" + dashboard_md + "\n"

    tmp_path = README_PATH.with_suffix(".md.tmp")
    tmp_path.write_text(new_content, encoding="utf-8")
    os.replace(tmp_path, README_PATH)
    print(f"[gen_readme] Successfully updated README.md with {len(projects)} project(s).")
    return True


if __name__ == "__main__":
    found_projects = discover_projects()
    update_readme(found_projects)
