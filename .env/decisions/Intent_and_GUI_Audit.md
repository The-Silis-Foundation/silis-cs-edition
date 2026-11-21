# Intent & Semantic Architecture Integrity Audit

This note documents the conceptual and intent-level verification of the Silis EDA knowledge vault, verifying that all models accurately reflect physical silicon realities rather than superficial markdown links.

## 1. Intent Verification Matrix

### 1.1 The Hardware Compilation Model
- **Core Intent**: Treat physical design not as manual polygon placement, but as an optimizing multi-stage compiler pipeline.
- **Verification**: The frontend AST (`silis.core.sdk.PowerDomain`, `MacroInstance`) directly lowers into DEF `REGIONS` and OpenROAD Tcl without divergence or bypass scripts. Confirmed in [[philosophy/Physical_Design_as_Compilation]].

### 1.2 Zero-Catch Law & Downstream Fail-Fast
- **Core Intent**: Prevent silent physical defects (open vias, floating crowbar nets, shorts) from escaping to mask generation.
- **Verification**: Removed `catch { pdngen }` and unmasked underlying row height collisions (`PDN-0240`). Confirmed in [[philosophy/Zero_Catch_Law]].

### 1.3 Authoritative SDK & Presentation Shell Parity
- **Core Intent**: The GUI must be a pure presentation lens over the Python SDK. No business logic in Qt widgets.
- **Verification**: Interactive mutations in the GUI table (`Row Site / Height` dropdown $\to$ `unithddbl`) immediately mutate the underlying `PowerDomain.site_type` and `row_height` SDK attributes. Confirmed via headless GUI test harness.

---

## 2. Headless GUI Verification Log (`ui-state-verifier`)

- **Target Component**: `InteractiveFloorplannerWidget` -> `table_domain_supplies`
- **Verification Standard**: Headless Qt6 event loop (`QT_QPA_PLATFORM=offscreen`, `SILIS_HEADLESS_TEST=1`) with dialog suppression.
- **Test Results**:
  1. **Table Structure**: Expanded from 4 to 6 columns:
     `['Power Domain', 'Voltage', 'Power Supply Type', 'Supply Net / Source', 'Row Site / Height', 'Physical Switch']`
  2. **Default State**:
     - `PD_TOP`: `unithd (2.72 µm)`, Switch Disabled (Always-on core).
     - `PD_NPU`: `unithd (2.72 µm)`, Switch Disabled (Architectural mesh by default).
     - `PD_CRYPTO`: `unithd (2.72 µm)`, Switch Disabled.
  3. **Interactive Mutation Test**:
     Changing Row 1 from `unithd (2.72 µm)` to `unithddbl (5.44 µm)` and checking `Insert Switch`:
     - `domain.site_type` mutated to `"unithddbl"`
     - `domain.row_height` mutated to `5.44`
     - `domain.enable_physical_switch` mutated to `True`
  - **Verdict**: 100% GUI-to-SDK state mutation parity verified.

## Related Notes
- [[00_Index]]
- [[decisions/User_Configurable_Domain_Row_Height]]
- [[philosophy/GUI_as_a_Projection]]
- [[decisions/Authoritative_SDK_State]]
