# Beast Processor Intent

## Design Vision
The "Processor Beast" testbench is designed as a rigorous stress-test for the Silis physical design engine.

## Architectural Intent
1. **Memory Bank Density**:
   6x 1KB SRAM macros (`sky130_sram_1kbyte_1rw1r_32x256_8`):
   - 2 Instruction Cache macros (North-West)
   - 1 Data Cache macro (Center-North)
   - 2 Neural Matrix RAMs inside `PD_NPU` (South-West)
   - 1 Cryptographic Key RAM inside `PD_CRYPTO` (South-East)

2. **Multi-Voltage Verification**:
   - `PD_TOP` @ $1.80\,\text{V}$ (Always-on host controller and caches)
   - `PD_NPU` @ $0.95\,\text{V}$ (Switched low-power domain)
   - `PD_CRYPTO` @ $1.20\,\text{V}$ (Switched security domain)

3. **Physical Design Stress Goals**:
   - Verify that OpenROAD PDN generates distinct voltage rings and core meshes for all 3 domains cleanly without row height collisions.
   - Test channel clearance and routing corridor escape between closely packed memory blocks.
   - Achieve 100% legalization and routing without manual coordinate tweaking.

## Related Notes
- [[00_Index]]
- [[power/Why_Power_Domains_Exist]]
- [[power/Power_Grid_Topology]]
- [[scratchpad/beast_processor_floorplan_plan]]
