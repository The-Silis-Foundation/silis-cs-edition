import os
import sys
import time

# Ensure Silis package is on path
REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "silis"))
sys.path.insert(0, REPO_ROOT)
import qt_setup
import silis
from PyQt6.QtWidgets import QApplication

# 1. Initialize Application
app = QApplication.instance() or QApplication(['silis', '-platform', 'offscreen'])

print("==================================================================")
print("  🚀 SRAM SOC QUAD CORE: END-TO-END ASIC FLOW VIA PYTHON SDK")
print("==================================================================")

proj_path = os.path.join(REPO_ROOT, "projects", "sram_soc_quad_core")
proj = silis.Project(proj_path)
print(f"[SDK] Loaded Project: {proj.name} ({proj.top_module})")
print(f"[SDK] PDK: {proj.pdk}")
print(f"[SDK] Macros: {proj.macros}")

# 2. Pipeline Execution
with silis.Pipeline(proj) as pipe:
    # A. Backend Design Init
    print("\n--- Step 1: Initializing Backend Design ---")
    pipe.init()

    # B. Floorplanning with 4x R90 SRAMs, 1x R0 2KB SRAM, Halos, Blockages, Regions
    print("\n--- Step 2: Executing Floorplan & Macro Placement ---")
    fp_script = os.path.join(proj.root_dir, "floorplan_clean.tcl")
    with open(fp_script, "r") as f:
        fp_tcl = f.read()
    pipe.runner.execute_tcl(fp_tcl, "floorplan")

    # C. Open Standalone Layout Window for Live Inspection
    print("\n--- Live Inspection: Launching Standalone SiliconPeeker Layout Window ---")
    def_path = os.path.join(proj.results_dir, "temp.def")
    layout_win = silis.view_layout(def_path)
    print(f"[SDK] Layout Window created: '{layout_win.windowTitle()}' (Standalone QMainWindow)")

    # D. Tapcell Insertion (Distinct distance flag)
    print("\n--- Step 3: Well-Tap & Endcap Insertion ---")
    pipe.tapcells()

    # E. Power Distribution Network Synthesis
    print("\n--- Step 4: PDN Grid Synthesis ---")
    pipe.pdn()

    # F. IO Pin Placement (Distinct layer assignment)
    print("\n--- Step 5: Peripheral Pin Assignment ---")
    pipe.iopins()

    # G. Standard Cell Placement (Distinct density & padding flags)
    print("\n--- Step 6: Global & Detailed Placement ---")
    pipe.place()

    # H. Clock Tree Synthesis (Distinct root buffer and clustering)
    print("\n--- Step 7: Clock Tree Synthesis (TritonCTS) ---")
    pipe.cts()

    # I. Signal Routing (TritonRoute)
    print("\n--- Step 8: TritonRoute Detailed Routing ---")
    pipe.route()

    # J. GDS Stream-out
    print("\n--- Step 9: GDS Layout Generation ---")
    pipe.gds()

    # K. Post-Layout Power, Area, Timing (PAT) Signoff
    print("\n--- Step 10: Multi-Corner PAT Signoff ---")
    pat_report = pipe.signoff("PAT")

    print("\n==================================================================")
    print("  🏆 POST-LAYOUT PAT SIGNOFF REPORT SUMMARY")
    print("==================================================================")
    if pat_report:
        print(f"  • Timing Closure Status: {'MET' if pat_report.is_clean else 'VIOLATED'}")
        print(f"  • Worst Negative Slack : {pat_report.wns} ns")
        print(f"  • Total Standard Cells : {pat_report.cells}")
        print(f"  • Total Die Area       : {pat_report.area} um^2")
        print(f"  • Total Chip Power     : {pat_report.total_power_mw} mW")
        print(f"  • Critical Endpoint    : {pat_report.critical_endpoint}")
    else:
        print("  • PAT Signoff Report could not be extracted.")
    print("==================================================================\n")

print("All Python SDK flow steps and signoff completed successfully!")
