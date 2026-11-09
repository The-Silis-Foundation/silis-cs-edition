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
    lines.append("## ASIC Projects & Control Center")
    lines.append("")
    lines.append("Manage and inspect chip designs directly from this Codespace workspace. Click any action below to open interactive tools inside dedicated VS Code editor tabs.")
    lines.append("")

    if not projects:
        lines.append("> [!NOTE]")
        lines.append("> No ASIC projects detected in `projects/`. Click the button below to initialize your first project scaffold.")
        lines.append("")
    else:
        lines.append("| Project | Process (PDK) | Top Module | Layout (DEF) | Waveforms | Interactive Actions |")
        lines.append("| :--- | :--- | :--- | :--- | :--- | :--- |")
        for p in projects:
            name = p["name"]
            pdk = p["pdk"]
            top = p["top_module"]
            layout_status = "Available" if p["has_def"] else "Pending PnR"
            wave_status = "Available" if p["has_vcd"] else "Pending Sim"

            arg_enc = encode_arg(name)
            open_link = f"[Select](command:silis.openProject?{arg_enc})"
            schem_link = f"[Schematic](command:silis.openSchematic?{arg_enc})"
            wave_link = f"[Waveforms](command:silis.openWaveforms?{arg_enc})"
            layout_link = f"[Layout](command:silis.openLayout?{arg_enc})"
            fps_link = f"[Floorplan](command:silis.openFloorplanner?{arg_enc})"

            actions = f"{open_link} &middot; {schem_link} &middot; {wave_link} &middot; {layout_link} &middot; {fps_link}"
            lines.append(f"| **{name}** | `{pdk}` | `{top}` | {layout_status} | {wave_status} | {actions} |")
        lines.append("")

    lines.append("### Quick Actions")
    lines.append("")
    lines.append("- [**+ Create New ASIC Project**](command:silis.newProject) -- Launch interactive project setup wizard")
    lines.append("- [**Open Unified CAD Dock**](command:silis.openWorkbench) -- View schematic, waveforms, layout, and floorplan in a tabbed panel")
    lines.append("- [**Run RTL Synthesis (Yosys)**](command:silis.runSynthesis) -- Synthesize active project into gate-level netlist")
    lines.append("- [**Run Tapeout Flow (OpenROAD)**](command:silis.runAll) -- Execute floorplan, CTS, placement, and routing")
    lines.append("- [**Refresh Projects List**](command:silis.refreshReadme) -- Re-scan workspace directories and update this view")
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
