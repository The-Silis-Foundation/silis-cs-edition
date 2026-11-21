# Architecture Decision Record (ADR): Physical Design Cross-Verification & Pad Ring Architecture

**Date:** 2026-11-07  
**Status:** Approved & Implemented  
**Scope:** `silis.core.sdk`, `silis.core.backendflow.floorplanner`, `Pipeline`, `Floorplan`

---

## 1. Context & Motivation

During stress-testing of dense multi-macro SoC designs in Silis, physical defects and flow regressions were observed:
1. **Missing I/O Pad Cells**: Procedural flows executed bare OpenROAD `place_pins`, creating boundary metal `PINS` but omitting physical pad macro cells (`sky130_ef_io`) and corner cells in `COMPONENTS`.
2. **Macro Grid Misalignments & Corridor Deficits**: Macro Y-origins produced by hierarchical placers were non-integer multiples of standard cell row height ($2.72\,\mu\text{m}$ in Sky130 `unithd`), and adjacent macro channels fell below the $35.0\,\mu\text{m}$ clearance required for $2 \times \text{Halo} + W_{\text{routing}}$.
3. **Pipeline Re-Run Collision (`[ERROR ODB-0359]`)**: Re-running `Pipeline.run_fps()` caused OpenROAD to reload `temp.def` where instances were marked `FIRM`, rejecting new coordinates.
4. **Silent Physical Defect Propagation**: Downstream tools proceeded even when upstream steps omitted critical structures like PDN `SPECIALNETS`.

---

## 2. Decision & Architectural Upgrades

### 2.1 Native Peripheral Pad Ring Synthesis (`Pipeline.spec_iopins(mode="padring")`)
- `Pipeline.spec_iopins(mode="padring")` provisions a complete, gapless peripheral pad ring around the die boundary.
- Generates 4 corner pad instances (`sky130_ef_io__corner_pad`) at `(0, 0)`, `(die_w - 200, 0)`, `(die_w - 200, die_h - 204)`, and `(0, die_h - 204)` with respective orientations `R0`, `MY`, `R180`, `MX`.
- Places core power clamp pads (`vccd_lvc_clamped_pad`, `vssd_lvc_clamped_pad`) and I/O supply clamp pads (`vddio_hvc_clamped_pad`, `vssio_hvc_clamped_pad`) across all four die edges.
- Synthesizes signal GPIO pads using `sky130_ef_io__gpiov2_pad_wrapped`, exposing physical `PIN PAD` openings on `met5`.
- Inserts abutting pad filler cells (`sky130_ef_io__com_bus_slice_*`) to form continuous peripheral power and ESD rings.

### 2.2 Mandatory Post-Step Physical Cross-Verification (`Pipeline._cross_verify_step()`)
Every pipeline stage automatically validates the generated DEF database against semiconductor invariants before completing:
- **Base Geometry**: Checks for `DIEAREA`, `COMPONENTS`, and `TRACKS`. Raises `DRCViolationError` if malformed.
- **Power Grid Continuity**: For `fps`, `pdn`, and `floorplan` (with PDN), asserts `SPECIALNETS` exists with `VDD` and `VSS`. Raises `PowerGridSynthesisError` on omission.
- **Placement Legalization**: For `place`, `cts`, and `route`, verifies that no instance remains `UNPLACED`.

### 2.3 Strict Geometric Legalization & Dynamic Macro Snapping
- `MacroInstance.__init__` and `move_to` in `silis/core/sdk/floorplan.py` enforce:
  $$y = \text{round}\left(\text{round}\left(\frac{y}{2.72}\right) \times 2.72,\, 3\right)$$
  $$x = \text{round}\left(\text{round}\left(\frac{x}{0.005}\right) \times 0.005,\, 3\right)$$
- `Floorplan.check_collisions()` and `verify_legal_placement()` verify that channel gaps satisfy:
  $$W_{\text{channel}} \ge 2 \times \text{Halo} + W_{\text{routing\_tracks}} \ge 35.0\,\mu\text{m}$$
- The interactive Qt6 canvas (`MacroItem.itemChange`) mirrors this logic so user moves in the GUI snap to legal coordinates.

### 2.4 Idempotent Pipeline Floorplan Execution
- `Pipeline.run_fps(fresh=True)` clears session backend state and re-links cleanly from the synthesized gate-level netlist, preventing `[ERROR ODB-0359]` collisions on re-run.
- OpenROAD Tcl macro placer resets instance status (`$inst_obj setPlacementStatus NONE`) before invoking `place_inst`.

---

## 3. Consequences & Verification
- 100% parity between Python SDK API and Qt6 interactive canvas.
- No silent failures: any omitted power net, floating pad, or unplaced cell halts the pipeline with an actionable typed exception.
- Verified on multi-macro designs (`sram_soc_system` and `titan_x_stress_soc`) with zero DRC infractions, zero FIRM collisions, and clean pass status across all cross-verification gates.
