# Multi-Voltage Power Intent (IEEE 1801 UPF 3.0)

This document specifies the multi-voltage design standards for Silis ASIC flows, detailing power domain declarations, level shifters, isolation clamps, and power switch architectures.

---

## 1. Domain Declaration & Hierarchy

In IEEE 1801 UPF, every power island must be explicitly declared with its constituent hierarchical elements:

```tcl
# Always-On Top Domain
create_power_domain PD_TOP -include_scope

# Switched Domain 1: NPU Engine (0.95V)
create_power_domain PD_NPU -elements {u_npu}

# Switched Domain 2: Crypto Engine (1.20V)
create_power_domain PD_CRYPTO -elements {u_crypto}
```

### Invariants:
1. **Element Exclusivity**:
   $$\text{Elements}(\text{Domain}_A) \cap \text{Elements}(\text{Domain}_B) = \emptyset$$
   No gate-level instance may belong to multiple power domains simultaneously.
2. **Timing of Physical Area**:
   In OpenROAD, `set_domain_area` MUST be executed **before** `initialize_floorplan`:
   ```tcl
   set_domain_area PD_NPU -area {259.900 250.240 989.920 750.720}
   initialize_floorplan -die_area "0 0 2000 2360" -core_area "250 250 1750 2110" -site unithd
   ```

---

## 2. Power Switches: Architectural vs Physical

Silis supports two distinct power-switch implementation models:

### Model A: Architectural / Virtual Power Mesh (Default)
- **Use Case**: Headless PnR, standard ASIC prototyping, early physical exploration.
- **Mechanism**:
  - The UPF declares a switchable supply net (`VDD_PD_NPU`).
  - Dedicated PDN rings, `met1` followpin rails, and `met4` straps are synthesized for `VDD_PD_NPU`.
  - Power gating control is bridged via external I/O pads or PMU power rails.
  - **No physical cell mapping is invoked**: OpenROAD handles the island as a separate voltage domain without searching for switch-site rows.

### Model B: Physical Power Switch Cell Insertion
- **Use Case**: Advanced silicon tapeout with on-chip distributed sleep header networks.
- **Mechanism**:
  - Requires standard-cell rows dedicated to power switches (`SITE unithddbl` = $5.44\,\mu\text{m}$).
  - Invokes `define_power_switch_cell` and `map_power_switch`.
  - OpenROAD places `POWER_SWITCH` macro cells along checkerboard columns or peripheral rings inside the voltage island.

---

## 3. Boundary Net Isolation & Clamping

When domain $PD_{\text{GATED}}$ turns off ($V \to 0\text{V}$) while $PD_{\text{TOP}}$ remains active ($1.8\text{V}$), every boundary output signal will float, causing short-circuit crowbar currents in the receiving domain.

### Isolation Rules:
```tcl
set_isolation iso_npu_out \
    -domain PD_NPU \
    -isolation_power_net VDD \
    -isolation_ground_net VSS \
    -clamp_value 0 \
    -applies_to outputs

set_isolation_control iso_npu_out \
    -domain PD_NPU \
    -isolation_signal u_pmu/npu_iso_en \
    -isolation_sense high
```
- **Clamp Value 0**: Used for active-high signals and general buses.
- **Clamp Value 1**: Used for active-low control signals (e.g. `rst_n`, `chip_select_n`).
