# Agent Directives: EDA & Tool Pipeline Debugging

This document defines core behavioral constraints for AI coding agents developing on the **Silis EDA** suite to prevent misalignment, looping, and tool-trap debugging.

---

## 1. Prioritize Parity Over Optimization
- **Reference Project Authority**: If a reference project (e.g. `test-FPS-SoC`) passes end-to-end PnR cleanly, duplicate its tool flags, shim files, and environment constraints exactly.
- **Do Not Attempt Autonomous "Fixes"**: Do not try to optimize or "fix" tool warnings if the reference project deliberately bypasses or ignores them.
- **Zero Deviation**: Flow scripts in `silis/core/backendflow/flow/*.tcl` and `silis/core/sdk/pipeline.py` must stay in 100% synchrony with proven physical flows.

---

## 2. Fail Fast on Hardware Geometry
- **Check Shims and Libraries First**: When encountering OpenROAD physical design errors (`PDN-0240`, row mismatches, site errors, macro overlaps), look at:
  1. The UPF shims (`openroad_upf_shim.tcl`)
  2. The cell mapping directives
  3. The PDK library LEFs loaded during initialization
- **Banned Behaviors**:
  - Never recursively grep `LEF` or `DEF` files to redesign layout rows or site geometries unless explicitly requested.
  - Never guess coordinates or mask structural tool mismatch with trial-and-error coordinate shifting.

---

## 3. Strict No-Catch Block Invariant
- **Banned Construct**: `catch { pdngen }`, `catch { global_placement }`, or wrapping core PnR steps in Tcl `catch` blocks.
- **Physical Integrity**: An error emitted by OpenROAD represents a real physical defect on silicon (e.g., disconnected power net, DRC violation, or overlapping cell). Masking the error with `catch` allows corrupted DEF files to propagate downstream to mask fabrication, causing multi-million dollar tapeout failures.

---

## 4. Headless Execution Protocol
- Always run automated verifications with `QT_QPA_PLATFORM=offscreen` and `SILIS_HEADLESS_TEST=1`.
- Suppress or mock all modal dialogs (`QMessageBox.exec`, `QFileDialog`) during batch headless SDK runs.
- Validate layout outputs directly with the SDK (`silis.view_layout` / `silis.Layout`).

---

## 5. Zero PDK Truncation Law
- Never prune, filter, or ship lean subsets of standard cells, PVT corners, macro LEFs, or primitive devices.
- Bundled packages and releases must include 100% of all technology assets (Sky130A, Sky130B, GF180MCU) to guarantee multi-corner STA signoff and IEEE 1801 multi-voltage level shifting integrity.
- See [[decisions/ADR_012_Zero_PDK_Truncation_Law]].
