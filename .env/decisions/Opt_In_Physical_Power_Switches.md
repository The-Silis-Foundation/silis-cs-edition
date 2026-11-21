# Decision: Opt-In Physical Power Switches

## Context & Historical Hindrance
During pipeline execution of Step 4 (`pipe.pdn()`), power network synthesis repeatedly halted with:
1. `[ERROR PDN-0240] No rows found that match the power cell: POWER_SWITCH`
2. `Signal 11 received in pdn::PdnGen::importUPF` (C++ null-pointer segfault).

### Root Causes
- In SkyWater 130nm, physical power switch cells (`sky130_power_switch`) require double-height standard cell rows (`SITE unithddbl`, $5.44\,\mu\text{m}$). Standard logic rows are `SITE unithd` ($2.72\,\mu\text{m}$).
- OpenROAD's UPF shim unconditionally invoked `map_power_switch`, which crashed when double-height rows were absent.
- Furthermore, invoking `create_power_switch` with dummy or unmapped supply ports triggered an unhandled null pointer dereference inside OpenROAD's C++ `PdnGen.cc`.

## Decision
Silis treats IEEE 1801 UPF power domains as **Architectural Power Domains** by default:
1. `map_power_switch` is disabled unless `SILIS_ENABLE_PHYSICAL_POWER_SWITCH=1` is explicitly opted into by the user.
2. The user can configure power domain site types (`unithd` vs `unithddbl`) and toggle physical power switch insertion via both SDK and GUI.

## Consequences
- **Positive**: Clean, 100% deterministic PDN generation with zero downstream crashes.
- **Tradeoff**: Physical power switch standard cells are omitted unless dedicated double-height row regions are created in the floorplan.

## Related Notes
- [[00_Index]]
- [[power/Architectural_vs_Physical_Switches]]
- [[power/Why_Power_Domains_Exist]]
- [[02_EDA_Error_Playbook]]
