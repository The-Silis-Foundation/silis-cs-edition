# Architectural vs Physical Switches

A critical distinction in multi-voltage physical design: how power gating is realized in layout.

## Architectural Power Gating
- The power domain is defined as a distinct voltage island in UPF 3.0 and DEF `REGIONS`.
- Dedicated power rings and straps (`VDD_NPU`, `VSS`) supply the island.
- Downstream tools (OpenROAD) treat power control as an external supply network.
- **Advantage**: Robust, portable, zero row-height dependency, 100% stable in standard flows.

## Physical Power Gating
- Specialized standard cells (`POWER_SWITCH` or `HEADSWITCH`) are inserted directly into the standard cell rows.
- Daisy-chained control wires sleep and wake up gates progressively to manage inrush current.
- **The Catch**: Physical power switch cells often require double-height standard cell rows (`SITE unithddbl` in Sky130). If placed in standard `unithd` rows, tools crash or throw `PDN-0240`.

## Silis Policy
Default to **Architectural Power Domains** for clean, portable downstream handoff. Gate physical switch cell mapping behind explicit user opt-in when custom double-height row regions are allocated.

## Related Notes
- [[00_Index]]
- [[power/Why_Power_Domains_Exist]]
- [[decisions/Opt_In_Physical_Power_Switches]]
- [[02_EDA_Error_Playbook]]
