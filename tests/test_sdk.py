#!/usr/bin/env python3
"""
test_sdk.py
Comprehensive verification test suite for the Silis Python SDK layer.
Tests Project, Pipeline, SynthesisReport, SignoffReport, DRCReport,
and standalone Viewers in headless mode.
"""

import os
import sys

# Ensure offscreen Qt platform for headless test execution
os.environ["QT_QPA_PLATFORM"] = "offscreen"

# Add repo workspace to sys.path
TESTS_DIR = os.path.dirname(os.path.abspath(__file__))
SILIS_REPO = os.path.dirname(TESTS_DIR)
WORKSPACE_ROOT = os.path.dirname(SILIS_REPO)

if SILIS_REPO not in sys.path:
    sys.path.insert(0, SILIS_REPO)
if os.path.join(SILIS_REPO, "core") not in sys.path:
    sys.path.insert(0, os.path.join(SILIS_REPO, "core"))


print("=" * 70)
print("  🚀 SILIS PYTHON SDK COMPREHENSIVE TEST SUITE")
print("=" * 70)

# ------------------------------------------------------------------------------
# Test 1: Package Import & Re-export Verification
# ------------------------------------------------------------------------------
print("\n[TEST 1] Testing package imports and top-level re-exports...")
import silis
from silis import (
    Project,
    Pipeline,
    SignoffReport,
    SynthesisReport,
    DRCReport,
    view_layout,
    view_gds3d,
    view_waveforms,
    view_schematic,
    open_gui,
)

required_symbols = [
    "Project",
    "Pipeline",
    "SignoffReport",
    "SynthesisReport",
    "DRCReport",
    "view_layout",
    "view_gds3d",
    "view_waveforms",
    "view_schematic",
    "open_gui",
]

for sym in required_symbols:
    assert hasattr(silis, sym), f"Symbol '{sym}' missing from top-level 'silis' module!"
    val = getattr(silis, sym)
    assert val is not None, f"Symbol '{sym}' is None in 'silis'!"

print(f"  ✔ All {len(required_symbols)} SDK symbols properly re-exported in 'silis' namespace.")


# ------------------------------------------------------------------------------
# Test 2: Project Loading & File Discovery
# ------------------------------------------------------------------------------
print("\n[TEST 2] Testing Silis Project loader & asset discovery...")
proj_candidates = [
    os.path.join(WORKSPACE_ROOT, "projects", "sram_soc_system"),
    os.path.join(WORKSPACE_ROOT, "projects", "sram_soc_system", "silis.silisproj"),
    os.path.join(SILIS_REPO, "projects", "sram_soc_system"),
    os.path.join(SILIS_REPO, "projects", "sram_soc_system", "silis.silisproj"),
    "sram_soc_system",
]

project = None
for cand in proj_candidates:
    try:
        project = Project.load(cand)
        if project:
            print(f"  ✔ Successfully loaded Project from: '{cand}'")
            break
    except Exception as e:
        continue

assert project is not None, "Failed to load Project 'sram_soc_system'!"
assert project.name == "sram_soc_system", f"Expected 'sram_soc_system', got '{project.name}'"
assert project.top_module == "sram_soc_top", f"Expected 'sram_soc_top', got '{project.top_module}'"
assert project.pdk == "sky130", f"Expected 'sky130', got '{project.pdk}'"
assert len(project.macros) > 0, "Expected at least 1 macro configured"

print(f"  • Project Name: {project.name}")
print(f"  • Top Module:   {project.top_module}")
print(f"  • PDK Target:   {project.pdk}")
print(f"  • Macros:       {project.macros}")

# Test asset discovery
rtls = project.get_rtl_files()
netlists = project.get_netlists()
primary_netlist = project.get_netlist()
defs = project.get_defs()
primary_def = project.get_def()
gds_files = project.get_gds_files()
primary_gds = project.get_gds()
odbs = project.get_odbs()
macro_lefs, macro_libs = project.get_macro_lefs_and_libs()

print(f"  • Discovered RTL sources:  {len(rtls)} files")
print(f"  • Discovered Netlists:     {len(netlists)} files (Primary: {os.path.basename(primary_netlist or '')})")
print(f"  • Discovered DEF files:    {len(defs)} files (Primary: {os.path.basename(primary_def or '')})")
print(f"  • Discovered GDS files:    {len(gds_files)} files (Primary: {os.path.basename(primary_gds or '')})")
print(f"  • Discovered ODBs:         {len(odbs)} files")
print(f"  • Macro LEFs/LIBs found:   {len(macro_lefs)} LEF, {len(macro_libs)} LIB")

assert len(rtls) > 0, "No RTL source files found!"
assert primary_def is not None, "No primary DEF file found in results!"
assert primary_gds is not None, "No primary GDS file found in results!"

# Test summary
proj_summary = project.summary()
assert "SILIS PROJECT" in proj_summary, "Invalid project summary format"
print("  ✔ Project discovery and summary generation passed.")


# ------------------------------------------------------------------------------
# Test 3: Report Engines (SynthesisReport, SignoffReport, DRCReport)
# ------------------------------------------------------------------------------
print("\n[TEST 3] Testing Report parsers and extraction engines...")

# 3a. Synthesis Report
synth_report = project.get_synthesis_report()
assert isinstance(synth_report, SynthesisReport), "Invalid SynthesisReport type"
print(f"  • Synthesis Report -> Cells: {synth_report.cells}, Area: {synth_report.area:.2f} um^2, WNS: {synth_report.wns} ns, Status: {synth_report.status}")
assert synth_report.cells in (953, 1792), f"Expected 953 or 1792 cells, got {synth_report.cells}"
assert synth_report.area > 5000.0, f"Expected area > 5000, got {synth_report.area}"
assert synth_report.wns < 0.0, "Expected negative slack for sram_soc_system synthesis"
synth_summary = synth_report.summary()
assert "POST SYNTHESIS REPORT" in synth_summary
print("  ✔ SynthesisReport parsing and summary generation verified.")

# 3b. Signoff Report (PAT)
signoff_report = project.get_signoff_report()
assert isinstance(signoff_report, SignoffReport), "Invalid SignoffReport type"
print(f"  • Signoff Report   -> Cells: {signoff_report.cells}, Area: {signoff_report.core_area:.2f} um^2, WNS: {signoff_report.wns} ns, Timing: {signoff_report.timing_status}")
assert signoff_report.cells in (5439, 9320, 9323), f"Expected valid cell count, got {signoff_report.cells}"
assert signoff_report.timing_status == "MET", f"Expected 'MET', got '{signoff_report.timing_status}'"
assert signoff_report.is_clean is True, "Expected clean signoff report (MET)"
signoff_summary = signoff_report.summary()
assert "POST ROUTE PAT REPORT" in signoff_summary
print("  ✔ SignoffReport (PAT) parsing and timing extraction verified.")

# 3c. DRC Report
drc_report = project.get_drc_report()
assert isinstance(drc_report, DRCReport), "Invalid DRCReport type"
print(f"  • DRC Report       -> Violations: {drc_report.violations_count}, Clean: {drc_report.is_clean}, Tool: {drc_report.tool}")
assert drc_report.violations_count == 0, f"Expected 0 violations, got {drc_report.violations_count}"
assert drc_report.is_clean is True, "Expected clean DRC report"
drc_summary = drc_report.summary()
assert "CLEAN" in drc_summary
print("  ✔ DRCReport parsing and violation checking verified.")

# 3d. Project get_reports() combined dictionary
reports_dict = project.get_reports()
assert "synthesis" in reports_dict and "signoff" in reports_dict and "drc" in reports_dict
print("  ✔ Combined project.get_reports() dictionary verified.")


# ------------------------------------------------------------------------------
# Test 4: Pipeline Programmatic Interface & Sequencing
# ------------------------------------------------------------------------------
print("\n[TEST 4] Testing Pipeline programmatic controls & sequencing...")
with Pipeline(project, headless=True) as pipe:
    assert pipe.project == project, "Pipeline did not bind correct project"
    assert hasattr(pipe, "runner"), "Pipeline missing underlying HeadlessASICRunner"

    # Verify all expected method signatures exist
    flow_methods = [
        "synthesis", "init", "floorplan", "tapcells", "pdn",
        "iopins", "place", "cts", "route", "gds", "signoff",
        "run_all", "run_until", "run_from", "run_step",
    ]
    for m in flow_methods:
        assert hasattr(pipe, m) and callable(getattr(pipe, m)), f"Method '{m}' missing from Pipeline!"

    # Test step normalization and aliases
    assert pipe._normalize_step("synth") == "synthesis"
    assert pipe._normalize_step("fp") == "floorplan"
    assert pipe._normalize_step("io_pins") == "iopins"
    assert pipe._normalize_step("pat") == "signoff"

    # Test report retrieval via pipeline
    p_synth = pipe.get_synthesis_report()
    p_signoff = pipe.get_signoff_report()
    p_drc = pipe.get_drc_report()
    assert p_synth.cells in (953, 1792)
    assert p_signoff.cells in (5439, 9320, 9323)
    assert p_drc.is_clean is True

    # Test Qt event processing pump
    pipe.process_events(iterations=5, delay_ms=1)

print("  ✔ Pipeline method structure, report hooks, and context manager verified.")


# ------------------------------------------------------------------------------
# Test 5: Standalone Window Viewers
# ------------------------------------------------------------------------------
print("\n[TEST 5] Testing Standalone Window Viewers in offscreen mode...")

# 5a. Layout Viewer (DEF)
def_file = project.get_def()
assert def_file and os.path.exists(def_file)
win_layout = view_layout(def_file, show=False)
assert win_layout is not None
assert hasattr(win_layout, "peeker_widget")
print(f"  ✔ view_layout() created QMainWindow: '{win_layout.windowTitle()}'")

# 5b. 3D GDS Viewer
gds_file = project.get_gds()
assert gds_file and os.path.exists(gds_file)
win_gds = view_gds3d(gds_file, show=False)
assert win_gds is not None
assert hasattr(win_gds, "gds_port")
print(f"  ✔ view_gds3d() created QMainWindow: '{win_gds.windowTitle()}'")

# 5c. Waveform Viewer (VCD)
vcd_candidates = project.get_vcd_files()
if vcd_candidates:
    vcd_file = vcd_candidates[0]
else:
    vcd_file = os.path.join(project.root_dir, "waveform.vcd")

assert os.path.exists(vcd_file), f"VCD file not found at {vcd_file}"
win_wave = view_waveforms(vcd_file, show=False)
assert win_wave is not None
assert hasattr(win_wave, "signal_peeker")
print(f"  ✔ view_waveforms() created QMainWindow: '{win_wave.windowTitle()}'")

# 5d. Schematic Viewer
sch_file = os.path.join(WORKSPACE_ROOT, "silis", "schematicviewer", "digitaljs.html")
win_schem = view_schematic(sch_file, show=False)
assert win_schem is not None
assert hasattr(win_schem, "schem_widget")
print(f"  ✔ view_schematic() created QMainWindow: '{win_schem.windowTitle()}'")

# Close test viewer windows cleanly
for w in [win_layout, win_gds, win_wave, win_schem]:
    w.close()

print("\n" + "=" * 70)
print("  🎉 ALL SILIS PYTHON SDK TESTS PASSED SUCCESSFULLY!")
print("=" * 70 + "\n")
