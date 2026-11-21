# EDA Error Playbook & Downstream Root Causes

This playbook documents verified failure modes in the OpenROAD and Yosys toolchains, their physical root causes, and exact solutions.

---

## 1. PDN & Power Synthesis Errors

### `[ERROR PDN-0240] No rows found that match the power cell: POWER_SWITCH`
- **Symptom**: `pdngen` fails immediately when inserting power domain grids (`grid_pd_npu`, `grid_pd_crypto`).
- **Physical Root Cause**:
  `create_power_switch` in UPF was physically mapped to the library macro `POWER_SWITCH` via `map_power_switch`. The Sky130 `POWER_SWITCH` macro requires double-height rows (`SITE unithddbl` = $5.44\,\mu\text{m}$). The floorplan was initialized with standard single-height rows (`SITE unithd` = $2.72\,\mu\text{m}$).
- **Solution**:
  - Treat power switches **architecturally** (virtual power net gating) by default. Do not invoke `map_power_switch` unless the user explicitly requested physical switch insertion and created `unithddbl` rows.
  - In `openroad_upf_shim.tcl`, gate `map_power_switch` behind `SILIS_ENABLE_PHYSICAL_POWER_SWITCH`.

---

### `[WARNING PDN-0178]` & `[ERROR PDN-0179] Unable to repair all channels`
- **Symptom**: `pdngen` fails with channel repair errors on `met1` followpin rails near power domain boundaries.
- **Physical Root Cause**:
  When UPF voltage domains end and macro halo blockages begin, standard cell rows are sliced. If sliced row fragments (e.g. 6 dead row fragments) remain in a narrow corridor without adjacent rows, `pdngen` cannot meet minimum enclosure spacing.
- **Solution**:
  Prune orphan dead row fragments in the corridor using OpenDB before running `pdngen`:
  ```tcl
  set _dead_rows [list]
  foreach _r [$_blk getRows] {
      set _y [expr {[lindex [$_r getOrigin] 1] / 1000.0}]
      if {$_y >= 750.700 && $_y <= 766.000} {
          lappend _dead_rows $_r
      }
  }
  foreach _dr $_dead_rows { odb::dbRow_destroy $_dr }
  pdngen
  ```

---

### `[ERROR PDN-0186] Connect between layers met4 and met5 already exists in grid`
- **Symptom**: OpenROAD aborts during grid strap definition.
- **Root Cause**: Calling `add_pdn_connect -grid <grid> -layers "met4 met5"` twice on the same grid (once for the core ring, once for straps).
- **Solution**: Call `add_pdn_connect` once per unique layer pair per grid.

---

## 2. Floorplanning & Region Errors

### `[ERROR PDN-0103] <DOMAIN> region must have a shape`
- **Symptom**: OpenROAD aborts during `set_voltage_domain -region <DOMAIN>`.
- **Root Cause**: `set_domain_area <DOMAIN> -area {x1 y1 x2 y2}` was either omitted or executed *after* `initialize_floorplan`.
- **Solution**: In IEEE 1801 flows, `set_domain_area` MUST be declared **before** `initialize_floorplan` so OpenROAD instantiates the physical voltage island fence.

---

## 3. Placement & Legalization Errors

### `[WARNING DPL-0006] Site aligned check failed` / `[ERROR DPL-0033]`
- **Symptom**: `check_placement` fails after detailed placement.
- **Root Cause**:
  `check_placement` checks every placed instance against standard cell site rows (`unithd`). I/O pads (`sky130_ef_io`, `sky130_fd_io`) are $208\,\mu\text{m}$ deep and sit on the chip periphery outside core `unithd` rows.
- **Solution**:
  Ensure peripheral pads are either excluded from standard core legalization checks or placed on dedicated `IO_SITE` peripheral rows.

---

### `[ERROR GPL-0305] RePlAce divergent step length (NaN)`
- **Symptom**: Global placement crashes with floating-point overflow during Nesterov iterations.
- **Root Cause**:
  Placing hard blockages that overlap 100% of an exclusive FENCE region leaves $0.0\,\mu\text{m}^2$ available free area, resulting in division by zero ($0/0 = \text{NaN}$).
- **Solution**:
  Temporarily destroy hard blockages before global placement, allow the Nesterov solver to place logic guided by fences, and restore blockages as soft blockages before detailed placement and routing.
