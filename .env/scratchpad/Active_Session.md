# Active Session Log: 2026-11-07

## Real-Time Working Memory & Intent

### Critical Pending Architecture Task: Adaptive Coarse-Placement Guided Power Switch & Level Shifter Insertion
1. **Status**: **STILL PENDING IN ACTIVE SESSION** (Identified in `beast_processor_top` triaging, implementation pending flow execution).
2. **The Defect**:
   - In `Pipeline.run_fps()`, the TCL generator was stamping power switch columns starting at the domain's lower-left boundary:
     ```tcl
     set _sw_x [expr {$_rx1 + 15.0 + $_c * $_pitch}]
     ```
   - In `beast_processor_top`, the gates inside `PD_NPU` did not populate the lower-left boundary; during global placement, they naturally clumped on the eastern boundary around $(X_{\text{CoG}}, Y_{\text{CoG}}) \approx (902.34\,\mu\text{m}, 1052.33\,\mu\text{m})$.
   - Because `spec_power_switches(adaptive_placement=True)` had not been linked to run a mock placement and extract $(X_{\text{CoG}}, Y_{\text{CoG}})$, the header switches were placed across empty silicon ($X = 415 \to 535\,\mu\text{m}$), hundreds of microns away from the standard cell logic cluster.
   - This defect leads to massive localized static IR drop on regional `met1` rails and unrouted long power jogs.
   - In current `results/temp.def`, standard logic cells remain unplaced (`- u_cpu/_498_ sky130_fd_sc_hd__nand2_1 ;` without coordinates), preventing logic polygon formation until global placement runs.
3. **Resolution Architecture**:
   - **Step 1: Mock Coarse Placement Pass**: Run `global_placement -density 0.55 -skip_initial_place` via OpenROAD to determine the natural logic centroid $(X_{\text{CoG}}, Y_{\text{CoG}})$.
   - **Step 2: Centered Switch Column Anchoring**: Anchor header switch columns centered around $X_{\text{CoG}}$:
     $$X_{\text{start}} = X_{\text{CoG}} - \frac{(\text{Cols} - 1) \times \text{Pitch}}{2}$$
   - **Step 3: Boundary Level Shifter Snapping**: Anchor level shifters on the domain perimeter directly intersecting crossing logic nets rather than arbitrary perimeter increments.
   - **Step 4**: Regenerate the physical DEF and verify with `def_inspector.py trace-electrons` and `check --verify-all`.

---

### Critical Architectural Initiative: Revitalize & Expand SSAForge (Silis Standard Aliases)
1. **Problem Statement**:
   - `SSAForge` was designed as the central PDK abstraction forge in Silis (`silis/core/pdkmanagers/pdk/manager.py`) to decouple the IDE, SDK, and downstream engines from foundry-specific naming conventions.
   - Over time, parts of the codebase regressed into hardcoding PDK paths (`sky130A` / `sky130B`) or performing uncontrolled glob scans that crash OpenDB with `ODB-0292` (tech layer conflicts).
   - Currently, adding a custom or new PDK family requires manual hardcoded python dictionary additions.
2. **Revitalization & Expansion Architecture**:
   - **Pluggable PDK Family Definition Schema (`pdk_families.json` / Dynamic Registration)**:
     Allow registering arbitrary PDK families (e.g. `asap7`, `ihp_sg13g2`, `tsmcN16`, `custom_fab`) programmatically via SDK (`SSAForge.register_family(...)`) or via user JSON configs.
   - **Required SSA Alias Vector**:
     - Standard sites: `site_single`, `site_double` (`unithd`, `unithddbl`, `GF018hv5v_green_sc9`, etc.)
     - Power/Ground rail primitives: `tap_cell`, `tap_dist`, `power_switch_cell`
     - Timing & Clock: `cts_root`, `cts_leaf`, `clock_buffer_list`
     - Multi-Voltage: `level_shifter_hl`, `level_shifter_lh`, `iso_cell_clamp_0`, `iso_cell_clamp_1`
     - Physical Fill: `fill_cells`, `decap_cells`, `tie_hi`, `tie_lo`
     - Interconnect: `min_routing_layer`, `max_routing_layer`, `rail_layer`, `mesh_layers`
   - **GUI & SDK "Add PDK Family" Wizard**:
     - SDK API: `PDKManager.add_family(name, tlef, lefs, libs, aliases)` with schema validation.
     - Native Qt6 Dialog: Import custom PDK directory, auto-detect Tech LEF and Cell LEFs, extract site names, and test-compile against OpenDB.
   - **Zero Hardcoding Enforcement**:
     All viewers (`SilisNativeLayerViewer`), floorplanners, and placement engines must route through `SSAForge` without exception.

---

### Active Engineering Milestones & Completed Resolutions
- [x] Integrate Obsidian as the authoritative developer Second Brain (`docs/vault/` with `tools/brain` CLI).
- [x] Resolve `PDN-0240` and null-port segfaults in OpenROAD multi-voltage PDN generation.
- [x] Add user-configurable site row height (`unithd` vs `unithddbl`) and physical switch insertion to SDK and GUI.
- [x] Verify GUI table mutations headlessly via `ui-state-verifier` test harness.
- [x] **RESOLVED**: Fix placement blocker: resolve `DPL-0006` site alignment failure on peripheral I/O pads & orphan tapcells.
- [x] **RESOLVED**: Clock Tree Synthesis (`pipe.cts()`) completed and timing legalized.
- [x] **RESOLVED**: Detailed Routing (`pipe.route()`): Resolved `GRT-0116` with `-allow_congestion`, TritonRoute completed with **0 DRC violations**.
- [x] **RESOLVED**: OpenROAD Native Qt6 Layout Viewer, DisplayControls, and Property Inspector (`silis/core/layer_viewer/`).
- [ ] **ACTIVE FOCUS 1**: Link `spec_power_switches(adaptive_placement=True)` with coarse mock placement to center header switch columns over logic centroids and snap level shifters to boundary-crossing nets. (STILL PENDING IN ACTIVE SESSION)
- [ ] **ACTIVE FOCUS 2**: Revitalize and expand `SSAForge`: Implement dynamic "Add PDK Family" system, eradicate hardcoded PDK paths across `LayerViewerPanel` and `Pipeline`, and wire `SSAForge` to full GUI/SDK parity.

---

## Related Notes
- [[00_Index]]
- [[07_Dual_Mode_Viewer_and_Adaptive_Placement_Architecture]]
- [[decisions/Detailed_Placement_Pad_Alignment_Blocker]]
- [[decisions/User_Configurable_Domain_Row_Height]]
- [[roadmap/Beast_Processor_Intent]]
- [[scratchpad/beast_processor_floorplan_plan]]
