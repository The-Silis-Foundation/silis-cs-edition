set SCRIPT_DIR [file normalize [file dirname [info script]]]
set REPO_ROOT [file normalize "$SCRIPT_DIR/../.."]

set lib_cands [list \
    "$REPO_ROOT/silis/third-party/OpenROAD/test/sky130hd/sky130_fd_sc_hd__tt_025C_1v80.lib" \
    "$REPO_ROOT/reference_flows/OpenROAD-flow-scripts/flow/platforms/sky130hd/lib/sky130_fd_sc_hd__tt_025C_1v80.lib" \
]
foreach lib $lib_cands {
    if {[file exists $lib]} {
        read_liberty $lib
        break
    }
}

if {[file exists "$SCRIPT_DIR/netlist/chip_top_netlist.v"]} {
    read_verilog "$SCRIPT_DIR/netlist/chip_top_netlist.v"
    link_design chip_top
}
if {[file exists "$SCRIPT_DIR/source/chip_top.sdc"]} {
    read_sdc "$SCRIPT_DIR/source/chip_top.sdc"
}

file mkdir "$SCRIPT_DIR/reports"
catch { report_checks -path_delay max -format full_clock_expanded > "$SCRIPT_DIR/reports/timing.rpt" }
catch { report_power > "$SCRIPT_DIR/reports/power.rpt" }
exit 0
        