# EDA Compiler IR & Frontend AST Contracts

In the Silis EDA architecture, physical design orchestration is treated as a **multi-stage hardware compiler**.

```text
[Verilog RTL]       [IEEE 1801 UPF 3.0]      [IEEE 1497 SDC]
      │                      │                     │
      ▼                      ▼                     ▼
[Yosys Synth Netlist]   [Power Intent AST]    [Timing Graph]
      │                      │                     │
      └──────────────┬───────┴─────────────────────┘
                     ▼
       [Silis Project SDK Data Model]
       ├── Floorplan (Die, Core, Tracks, Halos)
       ├── PowerNetwork (Domains, Nets, Straps)
       └── MacroPlacementEngine (Packing, Corridors)
                     │
         [DEF / LEF Geometric IR]
                     │
                     ▼
   [Downstream Engine Bytecode (OpenROAD Tcl)]
```

## Related Notes
- [[00_Index]]
- [[01_Agent_Directives_EDA_Parity]]
- [[03_Multi_Voltage_UPF_Standards]]
- [[04_Floorplan_Geometry_and_Macro_Placement]]
- [[algorithms/01_macro_packing_heuristics]]
- [[pdks/01_sky130_site_and_cell_profiles]]

## Key Compiler Invariants
1. **Frontend $\to$ Intermediate Representation**:
   - `PowerDomain` in Python SDK maps 1:1 with UPF `create_power_domain`.
   - Domain area boundaries map directly to rectilinear DEF regions (`REGIONS` section in DEF).
2. **Defensive Bytecode Emission**:
   - Zero catch blocks.
   - Coordinates rounded strictly to 3 decimal places (`{:.3f}`).
   - Explicit site declarations (`-site unithd`).
