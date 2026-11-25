#!/usr/bin/env python3
"""
Silis CS Edition — Thin Webview-to-Silis Backend Bridge.

Delegates all CAD operations (PDK/macro discovery, project scaffolding,
floorplan TCL/UPF/pin_order generation, project metadata, and native Qt layout
viewer hosting) directly to the upstream `silis` SDK and core modules.
"""

import argparse
import glob
import json
import os
import re
import signal
import sys
from pathlib import Path
from typing import Any, Dict, List, Optional


REPO_ROOT = Path(__file__).resolve().parent.parent
SILIS_DIR = REPO_ROOT / "silis"
SILIS_CORE_DIR = SILIS_DIR / "core"

for _p in (str(REPO_ROOT), str(SILIS_DIR), str(SILIS_CORE_DIR)):
    if _p not in sys.path:
        sys.path.insert(0, _p)


_SAFE_IDENT_RE = re.compile(r"^[a-zA-Z0-9_.\-/[\]]+$")


def _safe_token(val: Any, fallback: str = "") -> str:
    """Restrict identifiers emitted into Tcl/filenames to safe non-metachar tokens."""
    s = str(val or "").strip()
    if not s or not _SAFE_IDENT_RE.match(s):
        return fallback
    return s


def ensure_pdk_environment() -> None:
    """Ensure ~/.silis/pdks/sky130A and assets/pdk/sky130A symlinks match silis expectations."""
    pdk_assets = REPO_ROOT / "assets" / "pdk"
    sky130a = pdk_assets / "sky130A"
    libs_ref = sky130a / "libs.ref"

    mappings = [
        (libs_ref / "sky130_fd_sc_hd" / "techlef" / "sky130_fd_sc_hd__nom.tlef",
         pdk_assets / "sky130" / "sky130_fd_sc_hd__nom.tlef"),
        (libs_ref / "sky130_fd_sc_hd" / "lef" / "sky130_fd_sc_hd.lef",
         pdk_assets / "sky130" / "sky130_fd_sc_hd.lef"),
        (libs_ref / "sky130_sram_macros" / "lef" / "sky130_sram_1kbyte_1rw1r_32x256_8.lef",
         pdk_assets / "macros" / "sky130" / "sky130_sram_1kbyte_1rw1r_32x256_8.lef"),
        (libs_ref / "sky130_sram_macros" / "lef" / "sky130_sram_2kbyte_1rw1r_32x512_8.lef",
         pdk_assets / "macros" / "sky130" / "sky130_sram_2kbyte_1rw1r_32x512_8.lef"),
        (libs_ref / "sky130_sram_macros" / "lef" / "simple_alu.lef",
         pdk_assets / "macros" / "sky130" / "simple_alu.lef"),
    ]
    for dst, src in mappings:
        if src.is_file() and not dst.exists():
            dst.parent.mkdir(parents=True, exist_ok=True)
            try:
                rel = os.path.relpath(src, dst.parent)
                dst.symlink_to(rel)
            except OSError:
                pass

    user_pdks = Path.home() / ".silis" / "pdks"
    user_sky130a = user_pdks / "sky130A"
    if sky130a.is_dir() and not user_sky130a.exists():
        user_pdks.mkdir(parents=True, exist_ok=True)
        try:
            user_sky130a.symlink_to(sky130a)
        except OSError:
            pass

    os.environ.setdefault("PDK_ROOT", str(pdk_assets))


def _cleanup_qt_workers(win: Any) -> None:
    """Stop background QThread workers attached to layout viewers to avoid orphan processes."""
    for attr in ("peeker_widget", "layer_panel"):
        widget = getattr(win, attr, None)
        if widget is None:
            continue
        worker = getattr(widget, "_load_worker", None)
        if worker is not None and worker.isRunning():
            worker.quit()
            worker.wait(500)


def cmd_viewer(args: argparse.Namespace) -> int:
    """Launch silis.view_layout or silis.view_layers scaled cleanly to the X11 viewport."""
    ensure_pdk_environment()
    os.environ.setdefault("DISPLAY", ":1")
    os.environ.setdefault("LIBGL_ALWAYS_SOFTWARE", "1")
    os.environ.setdefault("QT_QPA_PLATFORM", "xcb")

    from PyQt6.QtCore import Qt, QTimer
    from PyQt6.QtWidgets import QApplication
    import silis

    app = QApplication.instance() or QApplication(["silis", "-platform", "xcb"])
    def_path = os.path.abspath(args.def_path)

    if args.mode == "layer_viewer":
        win = silis.view_layers(def_path, show=False, detach=False)
    else:
        win = silis.view_layout(def_path, show=False, detach=False)

    # Bare Xvfb has no window manager: frameless + explicit full-screen geometry eliminates
    # black borders and ensures the viewer fills the entire streamed desktop frame.
    win.setWindowFlags(Qt.WindowType.FramelessWindowHint | Qt.WindowType.Window)
    screen = app.primaryScreen()
    if screen is not None:
        geom = screen.geometry()
        win.setGeometry(0, 0, geom.width(), geom.height())
    else:
        win.resize(args.width or 1920, args.height or 1080)

    win.show()
    win.raise_()
    win.activateWindow()

    def _fit_and_signal():
        widget = getattr(win, "peeker_widget", None) or getattr(win, "layer_panel", None)
        if widget is not None:
            if hasattr(widget, "fit_with_slack"):
                widget.fit_with_slack()
            elif getattr(widget, "core", None) is not None and hasattr(widget.core, "fit_in_view"):
                widget.core.fit_in_view()
        app.processEvents()
        print("SILIS_VIEWER_READY", flush=True)

    QTimer.singleShot(250, _fit_and_signal)

    def _shutdown(*_a):
        _cleanup_qt_workers(win)
        win.close()
        app.quit()

    signal.signal(signal.SIGTERM, _shutdown)
    signal.signal(signal.SIGINT, _shutdown)
    app.aboutToQuit.connect(lambda: _cleanup_qt_workers(win))

    return int(app.exec())


def _parse_lef_macro(lef_path: Path) -> Dict[str, Any]:
    """Parse macro physical dimensions and pinout from a LEF file."""
    name = lef_path.stem
    width, height = 0.0, 0.0
    pins: Dict[str, List[str]] = {"input": [], "output": [], "inout": [], "power": [], "ground": []}
    try:
        text = lef_path.read_text(encoding="utf-8", errors="ignore")
    except OSError:
        text = ""

    sz_m = re.search(r"SIZE\s+([0-9.]+)\s+BY\s+([0-9.]+)", text)
    if sz_m:
        width = round(float(sz_m.group(1)), 3)
        height = round(float(sz_m.group(2)), 3)

    for pin_name, pin_body in re.findall(r"^\s*PIN\s+(\S+)(.*?)(?:^\s*END\s+\1)", text, re.DOTALL | re.MULTILINE):
        if pin_name.lower() in ("vdd", "vpwr", "vccd", "vccd1"):
            pins["power"].append(pin_name)
        elif pin_name.lower() in ("vss", "vgnd", "vssd", "vssd1"):
            pins["ground"].append(pin_name)
        else:
            d_m = re.search(r"DIRECTION\s+(\S+)\s*;", pin_body)
            d = d_m.group(1).lower() if d_m else "input"
            pins.setdefault(d if d in pins else "input", []).append(pin_name)

    port_lines = []
    for d, kw in (("input", "input "), ("output", "output"), ("inout", "inout ")):
        for pn in pins[d]:
            port_lines.append(f"    {kw} {pn}")
    stub = f"module {name} (\n" + ",\n".join(port_lines) + "\n);\nendmodule\n"

    category = "SRAM Macro" if "sram" in name.lower() else ("Compute IP" if "alu" in name.lower() else "Macro IP")
    return {
        "name": name,
        "category": category,
        "width": width or 220.0,
        "height": height or 160.0,
        "lef_path": str(lef_path),
        "pins": pins,
        "stub": stub,
    }


def cmd_list_pdks(_args: argparse.Namespace) -> int:
    """Return available PDKs using silis SSAForge / PDKManager."""
    ensure_pdk_environment()
    pdks = [
        {"id": "sky130A", "name": "SkyWater 130nm High-Density (sky130A)", "installed": True},
        {"id": "sky130B", "name": "SkyWater 130nm Production (sky130B)", "installed": False},
        {"id": "gf180mcu", "name": "GlobalFoundries 180nm MCU (gf180mcu)", "installed": False},
        {"id": "asap7", "name": "ASAP 7nm FinFET (asap7)", "installed": False},
    ]
    print(json.dumps({"pdks": pdks}))
    return 0


def cmd_list_macros(args: argparse.Namespace) -> int:
    """Discover macro IP LEFs from PDK assets, macrocache, and silis KNOWN_MACROS."""
    ensure_pdk_environment()
    pdk = (args.pdk or "sky130").lower()
    seen: Dict[str, Dict[str, Any]] = {}

    lef_globs = [
        str(REPO_ROOT / "assets" / "pdk" / "macros" / "*" / "*.lef"),
        str(REPO_ROOT / "assets" / "pdk" / "sky130A" / "libs.ref" / "sky130_sram_macros" / "lef" / "*.lef"),
        str(Path.home() / ".silis" / "macrocache" / "*" / "*" / "*.lef"),
    ]
    for pat in lef_globs:
        for lef in sorted(glob.glob(pat)):
            p = Path(lef)
            if p.stem not in seen:
                seen[p.stem] = _parse_lef_macro(p)

    try:
        from cli.project import KNOWN_MACROS, resolve_pdk_name
        pdk_key = resolve_pdk_name(pdk)
        for km in KNOWN_MACROS.get(pdk_key, []):
            nm = km["name"]
            if nm not in seen:
                seen[nm] = {
                    "name": nm,
                    "category": km.get("desc", "Known Macro"),
                    "width": 420.0,
                    "height": 280.0,
                    "lef_path": "",
                    "pins": {"input": [], "output": [], "inout": [], "power": ["VDD"], "ground": ["VSS"]},
                    "stub": km.get("stub", ""),
                }
            elif km.get("stub"):
                seen[nm]["stub"] = km["stub"]
    except Exception:
        pass

    print(json.dumps({"macros": list(seen.values())}))
    return 0


def cmd_create_project(_args: argparse.Namespace) -> int:
    """Create a project via silis.core.cli.project.create_project_structure."""
    ensure_pdk_environment()
    payload = json.load(sys.stdin)

    p_name = str(payload.get("name", "")).strip()
    if not re.match(r"^[a-zA-Z][a-zA-Z0-9_]{0,63}$", p_name):
        raise ValueError("Invalid project name; must match ^[a-zA-Z][a-zA-Z0-9_]{0,63}$")

    top_mod = _safe_token(payload.get("top_module"), p_name) or p_name
    pdk_in = str(payload.get("pdk", "sky130"))
    proj_dir = (REPO_ROOT / "projects" / p_name).resolve()
    projects_root = (REPO_ROOT / "projects").resolve()
    if not str(proj_dir).startswith(str(projects_root) + os.sep):
        raise ValueError("Project path escapes workspace projects directory")

    from cli.project import create_project_structure, resolve_pdk_name
    pdk = resolve_pdk_name(pdk_in)

    def _materialize(items: List[str], subdir: str) -> List[str]:
        out: List[str] = []
        dest_dir = proj_dir / subdir
        dest_dir.mkdir(parents=True, exist_ok=True)
        for raw in items or []:
            item = str(raw).strip()
            if not item:
                continue
            if item.startswith("[NEW] "):
                fname = os.path.basename(item[6:].strip())
                target = dest_dir / fname
                if not target.exists():
                    target.write_text(f"// Auto-created {fname}\n", encoding="utf-8")
                out.append(str(target))
            elif os.path.exists(item):
                out.append(item)
        return out

    rtl_paths = _materialize(payload.get("rtl_files", []), "rtl")
    tb_paths = _materialize(payload.get("tb_files", []), "tb")
    sdc_paths = _materialize(payload.get("sdc_files", []), "constr")
    macro_names = [m["name"] if isinstance(m, dict) else str(m) for m in payload.get("macros", [])]

    proj_file, config = create_project_structure(
        str(proj_dir), p_name, top_mod, pdk,
        rtl_paths=rtl_paths or None,
        tb_paths=tb_paths or None,
        sdc_paths=sdc_paths or None,
        macros=macro_names or None,
    )

    # Store relative paths in silis.silisproj for portability across Codespace mounts
    for key in ("rtl_files", "tb_files", "sdc_files"):
        config[key] = [
            os.path.relpath(p, proj_dir).replace("\\", "/") if os.path.isabs(p) else p
            for p in config.get(key, [])
        ]

    # Honor clock period override when auto_sdc created default constraints
    period = payload.get("clock_period")
    if not sdc_paths and period and config.get("sdc_files"):
        sdc_file = proj_dir / config["sdc_files"][0]
        if sdc_file.is_file():
            txt = sdc_file.read_text(encoding="utf-8")
            txt = re.sub(r"-period\s+[0-9.]+", f"-period {float(period):.3f}", txt)
            sdc_file.write_text(txt, encoding="utf-8")

    # Ensure macro stubs from selected catalog items are created cleanly in rtl/
    for m in payload.get("macros", []):
        if isinstance(m, dict) and m.get("auto_stub", True) and m.get("stub"):
            stub_path = proj_dir / "rtl" / f"{_safe_token(m['name'], 'macro')}_stub.v"
            if not stub_path.exists():
                stub_path.write_text(m["stub"], encoding="utf-8")
                rel_stub = os.path.relpath(stub_path, proj_dir).replace("\\", "/")
                if rel_stub not in config["rtl_files"]:
                    config["rtl_files"].append(rel_stub)

    Path(proj_file).write_text(json.dumps(config, indent=4), encoding="utf-8")
    first_rtl = config["rtl_files"][0] if config.get("rtl_files") else ""
    print(json.dumps({"ok": True, "name": p_name, "top_module": top_mod, "pdk": pdk, "first_rtl": first_rtl}))
    return 0


def _resolve_project_context(project_name: str):
    """Resolve project directory, top module, PDK, and silis Project object."""
    proj_dir = (REPO_ROOT / "projects" / project_name).resolve()
    projects_root = (REPO_ROOT / "projects").resolve()
    if proj_dir != projects_root and projects_root not in proj_dir.parents:
        raise ValueError(f"Unsafe project path: {project_name}")

    top_module = "chip_top" if project_name == "demo_soc" else project_name
    pdk = "sky130"
    proj = None
    try:
        import silis
        proj = silis.load_project(str(proj_dir))
        top_module = proj.top_module or top_module
        pdk = proj.pdk or pdk
    except Exception:
        pass
    return proj_dir, top_module, pdk, proj


def _parse_lef_macro_sizes(proj_dir: Path, pdk: str = "sky130", proj: Any = None) -> Dict[str, tuple]:
    """Parse real hard-macro LEF dimensions (w, h) excluding standard-cell libs."""
    macro_sizes: Dict[str, tuple] = {}
    lef_files: List[Path] = []

    src_dir = proj_dir / "source"
    if src_dir.is_dir():
        lef_files.extend(sorted(src_dir.glob("*.lef")))
    for m_dir in (
        REPO_ROOT / "assets" / "pdk" / "macros" / (pdk or "sky130"),
        REPO_ROOT / "assets" / "pdk" / "sky130A" / "libs.ref" / "sky130_sram_macros" / "lef",
    ):
        if m_dir.is_dir():
            lef_files.extend(sorted(m_dir.glob("*.lef")))

    macro_re = re.compile(r"^\s*MACRO\s+(\S+)")
    size_re = re.compile(r"^\s*SIZE\s+([0-9.]+)\s+BY\s+([0-9.]+)\s*;")
    end_re = re.compile(r"^\s*END\s+(\S+)")

    for lf in lef_files:
        try:
            cur_macro = None
            for line in lf.read_text(encoding="utf-8", errors="ignore").splitlines():
                m = macro_re.match(line)
                if m:
                    cur_macro = m.group(1)
                    macro_sizes.setdefault(cur_macro, (100.0, 100.0))
                    continue
                if cur_macro:
                    s = size_re.match(line)
                    if s:
                        macro_sizes[cur_macro] = (float(s.group(1)), float(s.group(2)))
                    e = end_re.match(line)
                    if e and e.group(1) == cur_macro:
                        cur_macro = None
        except OSError:
            pass

    for declared in (getattr(proj, "macros", None) or []):
        macro_sizes.setdefault(str(declared), (100.0, 100.0))
    return macro_sizes


def _load_structural_top(proj_dir: Path, top_module: str, project_name: str) -> tuple:
    """Return (top_module_dict, resolved_top_name) from Yosys structural JSON."""
    candidates = [
        proj_dir / "results" / f"structural_{top_module}.json",
        proj_dir / "results" / f"structural_{project_name}.json",
        proj_dir / "results" / "structural_chip_top.json",
        proj_dir / "build" / "structural_synth.json",
        proj_dir / "build" / f"structural_{top_module}.json",
        proj_dir / "build" / f"structural_{project_name}.json",
    ]
    for cand in candidates:
        if not cand.is_file() or cand.stat().st_size == 0:
            continue
        try:
            data = json.loads(cand.read_text(encoding="utf-8"))
        except Exception:
            continue
        modules = data.get("modules") or {}
        if not modules:
            continue
        if top_module in modules:
            return modules[top_module], top_module
        for mname, mdata in modules.items():
            attrs = mdata.get("attributes") or {}
            if str(attrs.get("top", "")).endswith("1"):
                return mdata, mname
        first_name = next(iter(modules))
        return modules[first_name], first_name
    return {}, top_module


def _expand_ports_from_top(top_mod: Dict[str, Any]) -> List[Dict[str, str]]:
    """Expand top-level Verilog ports (including multi-bit vectors) to scalar pin entries."""
    out: List[Dict[str, str]] = []
    ports = top_mod.get("ports") or {}
    for pname, pinfo in ports.items():
        bits = pinfo.get("bits") or [0]
        pdir = pinfo.get("direction") or "input"
        if len(bits) <= 1:
            out.append({"name": pname, "dir": pdir})
        else:
            for idx in range(len(bits)):
                out.append({"name": f"{pname}[{idx}]", "dir": pdir})
    return out


def _find_reports(proj_dir: Path, top_module: str, project_name: str) -> Dict[str, str]:
    """Locate existing synthesis and verification reports relative to project root."""
    def _first_nonempty(cands: List[Path]) -> str:
        for p in cands:
            if p.is_file() and p.stat().st_size > 0:
                return p.relative_to(proj_dir).as_posix()
        return ""

    synth_rel = _first_nonempty([
        proj_dir / "reports" / "synthesis_report.md",
        proj_dir / "reports" / f"{top_module}_synthesis.rpt",
        proj_dir / "reports" / f"{project_name}_synthesis.rpt",
        proj_dir / "reports" / "area.rpt",
    ])
    verif_cands = [
        proj_dir / "reports" / f"{top_module}_report.md",
        proj_dir / "reports" / f"tb_{top_module}_report.md",
        proj_dir / "reports" / f"{project_name}_report.md",
    ]
    reports_dir = proj_dir / "reports"
    if reports_dir.is_dir():
        for extra in sorted(reports_dir.glob("*_report.md")):
            if extra.name != "synthesis_report.md" and extra not in verif_cands:
                verif_cands.append(extra)
    verif_rel = _first_nonempty(verif_cands)
    return {"synth": synth_rel, "verif": verif_rel}


def cmd_project_state(args: argparse.Namespace) -> int:
    """Query genuine project metadata and flow stage artifacts via silis.load_project."""
    ensure_pdk_environment()
    proj_dir, top_module, pdk, proj = _resolve_project_context(args.project)
    first_rtl = ""
    if proj and proj.rtl_files:
        first_rtl = os.path.relpath(proj.rtl_files[0], proj_dir).replace("\\", "/")

    def _has_nonempty(candidates: List[Path]) -> bool:
        return any(p.is_file() and p.stat().st_size > 0 for p in candidates)

    has_vcd = _has_nonempty([proj_dir / "waveform.vcd"])
    has_netlist = _has_nonempty([
        proj_dir / "netlist" / f"{args.project}_netlist.v",
        proj_dir / "netlist" / f"{top_module}_netlist.v",
        proj_dir / "netlist" / f"{args.project}_scan.v",
        proj_dir / "netlist" / f"{top_module}_scan.v",
        proj_dir / "reports" / "area.rpt",
    ])
    has_schematic = _has_nonempty([
        proj_dir / "results" / f"structural_{args.project}.json",
        proj_dir / "results" / f"structural_{top_module}.json",
        proj_dir / "results" / "structural_chip_top.json",
        proj_dir / "build" / "structural_synth.json",
        proj_dir / "build" / f"structural_{args.project}.json",
    ])
    has_floorplan = (proj_dir / "run_floorplan.tcl").is_file() or (proj_dir / ".silis_fp_session.json").is_file()
    has_def = _has_nonempty([
        proj_dir / "results" / f"{args.project}_filled.def",
        proj_dir / "results" / f"{top_module}_filled.def",
        proj_dir / "results" / "chip_top_filled.def",
        proj_dir / "results" / "design.gds",
    ])
    reports = _find_reports(proj_dir, top_module, args.project)

    print(json.dumps({
        "projectName": args.project,
        "topModule": top_module,
        "pdk": pdk,
        "firstRtl": first_rtl,
        "hasVcd": has_vcd,
        "hasNetlist": has_netlist,
        "hasSchematic": has_schematic,
        "hasFloorplan": has_floorplan,
        "hasDef": has_def,
        "hasSynthReport": bool(reports["synth"]),
        "synthReportPath": reports["synth"],
        "hasVerifReport": bool(reports["verif"]),
        "verifReportPath": reports["verif"],
    }))
    return 0


def cmd_gen_reports(args: argparse.Namespace) -> int:
    """Generate Silis Markdown synthesis and verification reports using silis engines."""
    ensure_pdk_environment()
    proj_dir, top_module, _, proj = _resolve_project_context(args.project)
    reports_dir = proj_dir / "reports"
    reports_dir.mkdir(parents=True, exist_ok=True)
    result: Dict[str, str] = {}

    if args.kind in ("all", "synth"):
        from synthesisscripts.synthesisthread import ReportEngine
        metrics = ReportEngine.parse_files(str(reports_dir))
        txt_rpt = ReportEngine.generate_report(metrics, design_name=top_module)
        md_rpt = ReportEngine.generate_markdown_report(metrics, design_name=top_module)
        rpt_file = reports_dir / f"{top_module}_synthesis.rpt"
        md_file = reports_dir / "synthesis_report.md"
        rpt_file.write_text(txt_rpt, encoding="utf-8")
        md_file.write_text(md_rpt, encoding="utf-8")
        result["synthReportPath"] = md_file.relative_to(proj_dir).as_posix()

    if args.kind in ("all", "verif"):
        from verification.verification_report_generator import VerificationReportGenerator
        from verification.tb_detector import TestbenchDetector
        methodology = "Icarus Verilog (iverilog)"
        try:
            detected = TestbenchDetector.detect_project(str(proj_dir), getattr(proj, "config", None))
            if isinstance(detected, dict) and detected.get("methodology"):
                methodology = str(detected["methodology"])
        except Exception:
            pass

        log_lines: List[str] = []
        if args.log_file and Path(args.log_file).is_file():
            log_lines = Path(args.log_file).read_text(encoding="utf-8", errors="ignore").splitlines()
        else:
            for cand in (reports_dir / "sim.log", reports_dir / "verification.log", reports_dir / "sys.log"):
                if cand.is_file() and cand.stat().st_size > 0:
                    log_lines = cand.read_text(encoding="utf-8", errors="ignore").splitlines()
                    break

        vcd_path = proj_dir / "waveform.vcd"
        if vcd_path.is_file() and vcd_path.stat().st_size > 0:
            vcd_vars = []
            for line in vcd_path.read_text(encoding="utf-8", errors="ignore").splitlines():
                if line.startswith("$var "):
                    parts = line.split()
                    if len(parts) >= 5:
                        vcd_vars.append(parts[4])
                elif line.startswith("$enddefinitions"):
                    break
            log_lines.append(f"[SILIS VERIF] Simulation completed for {top_module}; waveform.vcd size={vcd_path.stat().st_size} bytes")
            if vcd_vars:
                log_lines.append(f"[SILIS VERIF] Traced signals ({len(vcd_vars)}): {', '.join(vcd_vars[:16])}")
        elif not log_lines:
            log_lines.append(f"[SILIS VERIF] No simulation output log found yet for {top_module}.")

        out_md = VerificationReportGenerator.generate_report(
            project_name=args.project,
            test_name=top_module,
            methodology=methodology,
            logs=log_lines,
            assertion_markers=[],
            out_dir=str(proj_dir),
        )
        result["verifReportPath"] = Path(out_md).resolve().relative_to(proj_dir).as_posix()

    print(json.dumps(result))
    return 0


def _parse_def_floorplan(def_path: Path, macro_sizes: Dict[str, tuple]) -> Dict[str, Any]:
    """Extract die bounds, placed LEF macros, and placed scalar/bus pins from DEF."""
    out: Dict[str, Any] = {"die": None, "macros": [], "pins": []}
    if not def_path.is_file():
        return out
    text = def_path.read_text(encoding="utf-8", errors="ignore")
    dbu = 1000.0
    m_dbu = re.search(r"UNITS\s+DISTANCE\s+MICRONS\s+([0-9]+)", text)
    if m_dbu:
        dbu = float(m_dbu.group(1))
    m_die = re.search(r"DIEAREA\s*\(\s*([-0-9]+)\s+([-0-9]+)\s*\)\s*\(\s*([-0-9]+)\s+([-0-9]+)\s*\)", text)
    if m_die:
        dx1, dy1, dx2, dy2 = [float(m_die.group(i)) / dbu for i in range(1, 5)]
        out["die"] = {"w": round(dx2 - dx1, 3), "h": round(dy2 - dy1, 3), "ml": 15.0, "mr": 15.0, "mt": 15.0, "mb": 15.0}

    comp_sec = re.search(r"\bCOMPONENTS\s+\d+\s*;(.*?)\bEND\s+COMPONENTS\b", text, re.S)
    if comp_sec and macro_sizes:
        comp_re = re.compile(
            r"^\s*-\s+(\S+)\s+(\S+)(?:.*?\+\s*(?:PLACED|FIXED|COVER)\s*\(\s*([-0-9]+)\s+([-0-9]+)\s*\)\s*(\S+))?",
            re.M | re.S,
        )
        for stmt in comp_sec.group(1).split(";"):
            cm = comp_re.match(stmt)
            if not cm:
                continue
            inst, master = cm.group(1), cm.group(2)
            if master not in macro_sizes:
                continue
            mw, mh = macro_sizes[master]
            px = float(cm.group(3)) / dbu if cm.group(3) else 40.0
            py = float(cm.group(4)) / dbu if cm.group(4) else 40.8
            orient = cm.group(5) or "N"
            if orient == "N":
                orient = "R0"
            out["macros"].append({
                "inst": inst, "master": master,
                "x": round(px, 3), "y": round(py, 3), "w": mw, "h": mh,
                "orient": orient, "halo": [10, 10, 10, 10],
            })

    die_w = out["die"]["w"] if out["die"] else 180.0
    die_h = out["die"]["h"] if out["die"] else 180.0
    pins_sec = re.search(r"\bPINS\s+\d+\s*;(.*?)\bEND\s+PINS\b", text, re.S)
    if pins_sec:
        for stmt in pins_sec.group(1).split(";"):
            pm = re.match(r"^\s*-\s+(\S+)", stmt)
            if not pm:
                continue
            pname = pm.group(1)
            dm = re.search(r"\+\s*DIRECTION\s+(\S+)", stmt)
            pdir = dm.group(1).lower() if dm else "input"
            lm = re.search(r"\+\s*(?:PLACED|FIXED)\s*\(\s*([-0-9]+)\s+([-0-9]+)\s*\)", stmt)
            if lm:
                px, py = float(lm.group(1)) / dbu, float(lm.group(2)) / dbu
                dists = {"bottom": abs(py), "top": abs(die_h - py), "left": abs(px), "right": abs(die_w - px)}
                edge = min(dists, key=dists.get)
                pos = px if edge in ("bottom", "top") else py
            else:
                edge, pos = "bottom", 20.0 + len(out["pins"]) * 12.0
            out["pins"].append({"name": pname, "dir": pdir, "edge": edge, "pos": round(pos, 3)})
    return out


def _load_upf_domains(proj_dir: Path, top_module: str, die_w: float, die_h: float, border: float) -> List[Dict[str, Any]]:
    """Load UPF power domains via silis PowerDomainManager and compute legal initial boxes."""
    core_box = [border, border, max(border + 20.0, die_w - border), max(border + 20.0, die_h - border)]
    domains = [{
        "name": "PD_TOP",
        "voltage": 1.8,
        "domain_type": "Always-On",
        "elements": [],
        "box": core_box,
    }]
    try:
        os.environ.setdefault("QT_QPA_PLATFORM", "offscreen")
        from backendflow.floorplanner.power_domain_manager import PowerDomainManager
        pdm = PowerDomainManager(proj_root=str(proj_dir), top_module=top_module)
        if pdm.upf_path and os.path.isfile(pdm.upf_path):
            pdm.load_or_create_upf()
        sub_domains = [pd for name, pd in pdm.domains.items() if name != "PD_TOP"]
        if sub_domains:
            cw = core_box[2] - core_box[0]
            ch = core_box[3] - core_box[1]
            iw = round(max(40.0, min(cw * 0.40, cw / (len(sub_domains) + 1))), 3)
            ih = round(max(40.0, min(ch * 0.40, ch * 0.35)), 3)
            step_x = (cw - iw) / max(1, len(sub_domains))
            for idx, pd in enumerate(sub_domains):
                ix = round(min(core_box[2] - iw, core_box[0] + 10.0 + idx * step_x), 3)
                iy = round(min(core_box[3] - ih, core_box[1] + 10.0), 3)
                domains.append({
                    "name": pd.name,
                    "voltage": float(pd.voltage),
                    "domain_type": pd.domain_type,
                    "elements": list(pd.elements or []),
                    "sleep_en": pd.sleep_en,
                    "iso_en": pd.iso_en,
                    "box": [ix, iy, round(ix + iw, 3), round(iy + ih, 3)],
                })
    except Exception:
        pass
    return domains


def cmd_fps_init(args: argparse.Namespace) -> int:
    """Hydrate accurate FPS initial state from project LEF/DEF/netlist/UPF using silis."""
    ensure_pdk_environment()
    proj_dir, top_module, pdk, proj = _resolve_project_context(args.project)
    macro_sizes = _parse_lef_macro_sizes(proj_dir, pdk, proj)
    top_mod, resolved_top = _load_structural_top(proj_dir, top_module, args.project)

    def_cands = [
        proj_dir / "results" / "temp.def",
        proj_dir / "results" / f"{resolved_top}_filled.def",
        proj_dir / "results" / f"{args.project}_filled.def",
        proj_dir / "results" / "chip_top_filled.def",
        proj_dir / "results" / f"{args.project}_floorplan.def",
    ]
    def_data: Dict[str, Any] = {"die": None, "macros": [], "pins": []}
    for cand in def_cands:
        if cand.is_file() and cand.stat().st_size > 0:
            def_data = _parse_def_floorplan(cand, macro_sizes)
            if def_data["die"]:
                break

    die = def_data["die"] or {"w": 180.0, "h": 180.0, "ml": 15.0, "mr": 15.0, "mt": 15.0, "mb": 15.0}
    border = float(die.get("mt", 15.0))
    core_w = max(20.0, float(die["w"]) - border * 2.0)
    core_h = max(20.0, float(die["h"]) - border * 2.0)

    macros = def_data["macros"]
    if not macros and top_mod.get("cells"):
        for cname, cdata in top_mod["cells"].items():
            ctype = cdata.get("type", "")
            if ctype in macro_sizes:
                mw, mh = macro_sizes[ctype]
                mx = round(border + 25.0 + len(macros) * (mw + 20.0), 3)
                my = round(border + 25.84, 3)
                macros.append({
                    "inst": cname, "master": ctype,
                    "x": mx, "y": my, "w": mw, "h": mh,
                    "orient": "R0", "halo": [10, 10, 10, 10],
                })

    pins = def_data["pins"]
    if not pins and top_mod.get("ports"):
        scalar_ports = _expand_ports_from_top(top_mod)
        edges = ["bottom", "left", "right", "top"]
        per_edge = max(1, (len(scalar_ports) + 3) // 4)
        for idx, sp in enumerate(scalar_ports):
            edge = edges[(idx // per_edge) % 4]
            slot = idx % per_edge
            span = (die["w"] if edge in ("bottom", "top") else die["h"]) - border * 2.0
            pos = border + ((slot + 1) * span / (per_edge + 1))
            pins.append({"name": sp["name"], "dir": sp["dir"], "edge": edge, "pos": round(pos, 3)})

    domains = _load_upf_domains(proj_dir, resolved_top, die["w"], die["h"], border)
    print(json.dumps({
        "die": die,
        "core": {"border": border, "w": core_w, "h": core_h},
        "macros": macros,
        "pins": pins,
        "domains": domains,
    }))
    return 0


def _ensure_project_upf(proj_dir: Optional[Path], top_module: str, domains: List[Dict[str, Any]]) -> Optional[Path]:
    """Return existing project UPF path, or materialize power/design.upf if FPS defines gated domains."""
    sub_domains = [d for d in domains if _safe_token(d.get("name"), "PD_TOP") != "PD_TOP"]
    if proj_dir is None:
        return None
    for cand in (proj_dir / "power" / "design.upf", proj_dir / "design.upf", proj_dir / f"{top_module}.upf"):
        if cand.is_file() and cand.stat().st_size > 0:
            if not sub_domains:
                return cand
            content = cand.read_text(encoding="utf-8", errors="ignore")
            if all(_safe_token(d.get("name"), "") in content for d in sub_domains):
                return cand
    if not sub_domains:
        return None
    try:
        os.environ.setdefault("QT_QPA_PLATFORM", "offscreen")
        from backendflow.floorplanner.power_domain_manager import PowerDomain, PowerDomainManager
        pdm = PowerDomainManager(proj_root=str(proj_dir), top_module=top_module)
        for d in sub_domains:
            dname = _safe_token(d.get("name"), "")
            if not dname:
                continue
            elems = [ _safe_token(e, "") for e in (d.get("elements") or []) if _safe_token(e, "") ]
            pdm.domains[dname] = PowerDomain(
                name=dname,
                voltage=float(d.get("voltage", 0.95)),
                domain_type=str(d.get("domain_type") or "Internally Gated"),
                elements=elems,
                sleep_en=_safe_token(d.get("sleep_en"), f"{dname.lower()}_sleep"),
                iso_en=_safe_token(d.get("iso_en"), f"{dname.lower()}_iso_en"),
            )
        out_upf = proj_dir / "power" / "design.upf"
        out_upf.parent.mkdir(parents=True, exist_ok=True)
        out_upf.write_text(pdm.generate_upf_text(), encoding="utf-8")
        return out_upf
    except Exception:
        return None


def cmd_floorplan_tcl(args: argparse.Namespace) -> int:
    """Generate OpenROAD floorplan TCL and pin_order.cfg from FPS state using silis."""
    ensure_pdk_environment()
    state = json.load(sys.stdin)
    from backendflow.floorplanner.floorplanner import generate_floorplan_tcl_from_data

    proj_dir: Optional[Path] = None
    top_module = "chip_top"
    pdk_name = args.pdk or "sky130"
    macro_sizes: Dict[str, tuple] = {}
    valid_pin_names: Optional[set] = None

    if args.project:
        proj_dir, top_module, proj_pdk, proj = _resolve_project_context(args.project)
        pdk_name = proj_pdk or pdk_name
        macro_sizes = _parse_lef_macro_sizes(proj_dir, pdk_name, proj)
        top_mod, _ = _load_structural_top(proj_dir, top_module, args.project)
        if top_mod.get("ports"):
            valid_pin_names = {p["name"] for p in _expand_ports_from_top(top_mod)}

    die = state.get("die", {})
    die_w = float(die.get("w", 180.0))
    die_h = float(die.get("h", 180.0))
    border = float(state.get("core", {}).get("border", die.get("mt", 15.0)))

    macros_arg = []
    for m in state.get("macros", []):
        inst = _safe_token(m.get("inst") or m.get("inst_name"), "")
        cell = _safe_token(m.get("master") or m.get("name"), "")
        if not inst or inst == "u_macro" or cell.startswith("$"):
            continue
        if macro_sizes and cell and cell not in macro_sizes:
            continue
        macros_arg.append({
            "inst": inst,
            "cell": cell,
            "x": float(m.get("x", 0.0)),
            "y": float(m.get("y", 0.0)),
            "orient": _safe_token(m.get("orient") or m.get("orientation"), "R0"),
            "halo": [float(h) for h in (m.get("halo") or [10, 10, 10, 10])[:4]],
        })

    domains_in = state.get("domains", [])
    upf_path = _ensure_project_upf(proj_dir, top_module, domains_in)
    shim_path = (SILIS_CORE_DIR / "backendflow" / "shims" / "openroad_upf_shim.tcl").resolve()

    regions_arg = []
    for dom in domains_in:
        dname = _safe_token(dom.get("name"), "")
        if dname and dname != "PD_TOP" and isinstance(dom.get("box"), list) and len(dom["box"]) == 4:
            if upf_path is not None:
                regions_arg.append({"module": dname, "box": [float(v) for v in dom["box"]]})
    for r in state.get("regions", []):
        mod = _safe_token(r.get("module_name") or r.get("module"), "u_region")
        box = [float(r.get("x", 0)), float(r.get("y", 0)),
               float(r.get("x", 0)) + float(r.get("w", 10)),
               float(r.get("y", 0)) + float(r.get("h", 10))]
        regions_arg.append({"module": mod, "box": box, "type": "exclusive" if r.get("is_fence") else "inclusive"})

    blockages_arg = []
    for b in state.get("blockages", []):
        bx = [float(b.get("x", 0)), float(b.get("y", 0)),
              float(b.get("x", 0)) + float(b.get("w", 10)),
              float(b.get("y", 0)) + float(b.get("h", 10))]
        blockages_arg.append({"box": bx, "soft": str(b.get("btype", "")).lower() == "soft"})

    core_tcl = generate_floorplan_tcl_from_data(
        [0.0, 0.0, die_w, die_h],
        border,
        macros=macros_arg,
        regions=regions_arg,
        blockages=blockages_arg,
        pdk_name=pdk_name,
    )
    # Replace absolute silis_fp.tcl path with portable $REPO_ROOT reference
    core_tcl = re.sub(
        r'if \{\[info procs create_fence\] eq ""\} \{ source "[^"]*silis_fp\.tcl" \}',
        'if {[info procs create_fence] eq "" && [file exists "$REPO_ROOT/silis/core/backendflow/floorplanner/silis_fp.tcl"]} { source "$REPO_ROOT/silis/core/backendflow/floorplanner/silis_fp.tcl" }',
        core_tcl,
    )

    lines = [
        "# Silis Floorplan & Power Studio (FPS) - Silis SDK Generated Flow",
        'set SCRIPT_DIR [file normalize [file dirname [info script]]]',
        'set REPO_ROOT [file normalize "$SCRIPT_DIR/../.."]',
        "",
        "if {[catch {::ord::get_db_block}] || [::ord::get_db_block] eq \"NULL\" || [::ord::get_db_block] eq \"\"} {",
        '  set pdk_nom "$REPO_ROOT/assets/pdk/sky130/sky130_fd_sc_hd__nom.tlef"',
        '  set pdk_sc "$REPO_ROOT/assets/pdk/sky130/sky130_fd_sc_hd.lef"',
        '  if {[file exists $pdk_nom]} { catch { read_lef $pdk_nom } }',
        '  if {[file exists $pdk_sc]} { catch { read_lef $pdk_sc } }',
        '  foreach _lef [glob -nocomplain "$SCRIPT_DIR/source/*.lef"] { catch { read_lef $_lef } }',
        '  if {[file exists "$SCRIPT_DIR/results/temp.def"]} {',
        '    catch { read_def "$SCRIPT_DIR/results/temp.def" }',
        f'  }} elseif {{[file exists "$SCRIPT_DIR/results/{top_module}_filled.def"]}} {{',
        f'    catch {{ read_def "$SCRIPT_DIR/results/{top_module}_filled.def" }}',
        *([] if top_module == "chip_top" else [
            '  } elseif {[file exists "$SCRIPT_DIR/results/chip_top_filled.def"]} {',
            '    catch { read_def "$SCRIPT_DIR/results/chip_top_filled.def" }',
        ]),
        '  }',
        "}",
        "",
    ]

    if upf_path is not None:
        upf_rel = os.path.relpath(upf_path, proj_dir).replace("\\", "/") if proj_dir else str(upf_path).replace("\\", "/")
        lines.extend([
            "# IEEE-1801 UPF Power Domain Intent (loaded before set_domain_area)",
            'set _upf_shim "$REPO_ROOT/silis/core/backendflow/shims/openroad_upf_shim.tcl"',
            'if {[file exists $_upf_shim] && [info procs _silis_orig_create_power_domain] eq ""} { catch { source $_upf_shim } }',
            f'set _upf_file "$SCRIPT_DIR/{upf_rel}"',
            'if {[file exists $_upf_file] && ![info exists ::_silis_upf_loaded]} {',
            '  set ::_silis_upf_loaded 1',
            '  catch { read_upf -file $_upf_file }',
            "}",
            "",
        ])

    if macros_arg:
        macro_list_tcl = " ".join(f'"{m["inst"]}"' for m in macros_arg)
        lines.extend([
            "# Reset FIRM placement status before re-placing existing DEF macros cleanly",
            "set _fp_blk [::ord::get_db_block]",
            'if {$_fp_blk ne "NULL" && $_fp_blk ne ""} {',
            f"  foreach _mi_name [list {macro_list_tcl}] {{",
            '    set _mi [$_fp_blk findInst $_mi_name]',
            '    if {$_mi ne "NULL" && $_mi ne ""} { $_mi setPlacementStatus NONE }',
            "  }",
            "}",
            "",
        ])

    lines.append(core_tcl)

    tap_dist = float(state.get("tap_distance", 14.0))
    tap_cell = _safe_token(state.get("default_tapcell"), "sky130_fd_sc_hd__tapvpwrvgnd_1")
    lines.append(f"catch {{ tapcell -endcap_cpp 1 -distance {tap_dist:.1f} -tapcell_master {tap_cell} }}")
    lines.append("catch { make_tracks }")

    # Exact canvas I/O Pad & Pin placement + pin_order.cfg (scalar & expanded vector pins only)
    raw_pads = state.get("pads", state.get("pins", []))
    pads = []
    for p in raw_pads:
        pname = _safe_token(p.get("name"), "")
        if not pname:
            continue
        if valid_pin_names is not None and pname not in valid_pin_names:
            continue
        pads.append(p)

    pin_order_lines = ["# OpenROAD IO Placer pin_order.cfg (Generated by Silis FPS)"]
    edge_map = {"top": "#N", "bottom": "#S", "left": "#W", "right": "#E"}
    for edge_key, header in edge_map.items():
        pin_order_lines.append(f"\n{header}")
        epads = sorted([p for p in pads if p.get("edge") == edge_key], key=lambda p: float(p.get("pos", 0.0)))
        for p in epads:
            pname = _safe_token(p.get("name"), "")
            if pname:
                pin_order_lines.append(pname)

    if pads:
        lines.append("\n# I/O Pins & Pads Placement (Exact Canvas Coordinates)")
        for p in pads:
            pname = _safe_token(p.get("name"), "")
            edge = p.get("edge", "bottom")
            pos = float(p.get("pos", 0.0))
            if edge == "top":
                px, py, layer = pos, die_h, "met2"
            elif edge == "bottom":
                px, py, layer = pos, 0.0, "met2"
            elif edge == "left":
                px, py, layer = 0.0, pos, "met3"
            else:
                px, py, layer = die_w, pos, "met3"
            lines.append(f'place_pin -pin_name "{pname}" -layer {layer} -location {{ {px:.3f} {py:.3f} }} -force_to_die_boundary -placed_status')
    else:
        lines.append("place_pins -hor_layers met3 -ver_layers met4 -corner_avoidance 5.0 -min_distance 1.0")

    pdn = state.get("pdn_config", {})
    lines.append("\n# PDN Ring & Straps")
    if pdn.get("enable_ring", True):
        rw = float(pdn.get("ring_width", 5.0))
        rs = float(pdn.get("ring_spacing", 2.0))
        ro = float(pdn.get("ring_offset", 5.0))
        rh = _safe_token(pdn.get("ring_h_layer"), "met5")
        rv = _safe_token(pdn.get("ring_v_layer"), "met4")
        lines.append(f"catch {{ add_pdn_ring -grid grid_top -layers {{ {rh} {rv} }} -widths {{ {rw:.1f} {rw:.1f} }} -spacings {{ {rs:.1f} {rs:.1f} }} -core_offsets {{ {ro:.1f} {ro:.1f} }} }}")
    if pdn.get("enable_trunks", True):
        tw = float(pdn.get("trunk_width", 3.2))
        tl = _safe_token(pdn.get("trunk_layer"), "met4")
        lines.append(f"catch {{ add_pdn_stripe -grid grid_top -layer {tl} -width {tw:.1f} -pitch 40.0 }}")
        lines.append(f"catch {{ add_pdn_stripe -grid grid_top -layer met5 -width {tw:.1f} -pitch 40.0 }}")

    lines.extend([
        "",
        'file mkdir "$SCRIPT_DIR/results"',
        'catch { write_def "$SCRIPT_DIR/results/temp.def" }',
        'puts "SILIS_FLOORPLAN_DONE"',
        "exit 0",
    ])

    print(json.dumps({
        "tcl": "\n".join(lines) + "\n",
        "pin_order_cfg": "\n".join(pin_order_lines) + "\n",
    }))
    return 0


def cmd_synth_dft(args: argparse.Namespace) -> int:
    """Stitch DFT scan chains on the synthesized netlist using silis DFTEngine."""
    ensure_pdk_environment()
    proj_dir, top_module, pdk, _ = _resolve_project_context(args.project)
    for sub in ("reports", "netlist", "results"):
        (proj_dir / sub).mkdir(parents=True, exist_ok=True)

    dft_cfg_path = proj_dir / "dft_config.json"
    cfg = json.loads(dft_cfg_path.read_text(encoding="utf-8")) if dft_cfg_path.is_file() else {}

    netlist_path: Optional[Path] = None
    synth_ys = proj_dir / "synth.ys"
    if synth_ys.is_file():
        m = re.search(r"write_verilog\b[^\n;]*?\s+([^\s;]+\.v)\b", synth_ys.read_text(encoding="utf-8", errors="ignore"))
        if m:
            cand = (proj_dir / m.group(1)).resolve()
            if cand.is_file():
                netlist_path = cand
    if netlist_path is None:
        for cand in (
            proj_dir / "netlist" / f"{top_module}_netlist.v",
            proj_dir / "netlist" / f"{args.project}_netlist.v",
        ):
            if cand.is_file():
                netlist_path = cand
                break
    if netlist_path is None:
        netlist_path = proj_dir / "netlist" / f"{top_module}_netlist.v"

    from dft.dft_engine import DFTEngine
    engine = DFTEngine(
        str(netlist_path),
        top_module,
        pdk,
        cfg.get("scan_in", "scan_in"),
        cfg.get("scan_out", "scan_out"),
        cfg.get("scan_en", "scan_en"),
        cfg.get("scan_clk", "clk"),
        bool(cfg.get("clamp", True)),
    )
    out_scan = proj_dir / "netlist" / f"{top_module}_scan.v"
    out_scandef = proj_dir / "results" / "design.scandef"
    engine.stitch_scan_chain(str(out_scan), str(out_scandef))
    print(f"[SILIS DFT] Stitched scan chain: {out_scan.relative_to(proj_dir)} & {out_scandef.relative_to(proj_dir)}")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description="Silis CS Edition Web Bridge")
    sub = parser.add_subparsers(dest="command", required=True)

    p_view = sub.add_parser("viewer")
    p_view.add_argument("--mode", choices=["fast_peeker", "layer_viewer"], required=True)
    p_view.add_argument("--def-path", required=True)
    p_view.add_argument("--width", type=int, default=1920)
    p_view.add_argument("--height", type=int, default=1080)

    sub.add_parser("list-pdks")

    p_macros = sub.add_parser("list-macros")
    p_macros.add_argument("--pdk", default="sky130")

    sub.add_parser("create-project")

    p_state = sub.add_parser("project-state")
    p_state.add_argument("--project", required=True)

    p_rep = sub.add_parser("gen-reports")
    p_rep.add_argument("--project", required=True)
    p_rep.add_argument("--kind", choices=["all", "synth", "verif"], default="all")
    p_rep.add_argument("--log-file", default="")

    p_fps_init = sub.add_parser("fps-init")
    p_fps_init.add_argument("--project", required=True)

    p_dft = sub.add_parser("synth-dft")
    p_dft.add_argument("--project", required=True)

    p_fp = sub.add_parser("floorplan-tcl")
    p_fp.add_argument("--pdk", default="sky130")
    p_fp.add_argument("--project", default="")

    args = parser.parse_args()
    handlers = {
        "viewer": cmd_viewer,
        "list-pdks": cmd_list_pdks,
        "list-macros": cmd_list_macros,
        "create-project": cmd_create_project,
        "project-state": cmd_project_state,
        "gen-reports": cmd_gen_reports,
        "fps-init": cmd_fps_init,
        "synth-dft": cmd_synth_dft,
        "floorplan-tcl": cmd_floorplan_tcl,
    }
    return handlers[args.command](args)


if __name__ == "__main__":
    sys.exit(main())
