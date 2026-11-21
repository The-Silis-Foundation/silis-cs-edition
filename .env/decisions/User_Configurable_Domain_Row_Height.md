# User Control: Configurable Row Site & Power Switch Insertion

## Context
Standard ASIC power gating creates tension between two physical flows:
- **Architectural Flow (Default)**: Switched multi-voltage meshes without cell insertion (passes PDN, placement, and CTS cleanly).
- **Physical Flow**: Inserting physical power switch cells (`POWER_SWITCH` / `sky130_power_switch`), which require double-height standard cell rows (`SITE unithddbl`, $5.44\,\mu\text{m}$) rather than standard single-height rows (`SITE unithd`, $2.72\,\mu\text{m}$).

Attempting to enforce physical cell mapping automatically resulted in `[ERROR PDN-0240]` and null-port crashes in downstream OpenROAD.

## Architectural Decision & Parity Implementation
Rather than hardcoding either assumption inside Silis, full control is exposed to the user across both the SDK and the GUI.

### 1. Python SDK Implementation (`silis.core.sdk.PowerDomain`)
Methods added to `PowerDomain`:
```python
pd_npu = PowerDomain("PD_NPU", primary_voltage=0.95, elements=["u_npu"])

# 1. Configurable Row Site / Height
pd_npu.set_row_site("unithd")      # Single-height 2.72 um (default)
pd_npu.set_row_site("unithddbl")   # Double-height 5.44 um for physical power switch rows

# 2. Explicit Physical Switch Insertion Opt-In
pd_npu.enable_physical_power_switches(True)  # Instructs OpenROAD to map POWER_SWITCH cells
```

### 2. Qt6 GUI Implementation (`PowerDomainManager` & `floorplanner.py`)
In the **Power & PDN** inspector tab:
- **`table_domain_supplies`** expanded with two interactive controls for each declared domain:
  - **Row Site / Height Dropdown**: Toggle between `unithd (2.72 µm)` and `unithddbl (5.44 µm)`.
  - **Physical Switch Checkbox**: `Insert Switch` checkbox enabling or disabling physical cell mapping for that specific voltage island.

## Related Notes
- [[00_Index]]
- [[power/Architectural_vs_Physical_Switches]]
- [[decisions/Opt_In_Physical_Power_Switches]]
- [[decisions/Authoritative_SDK_State]]
- [[toolchain/OpenROAD_Personality]]
