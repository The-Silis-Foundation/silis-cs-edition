# Beast Processor Floorplan Architectural Plan (6x 1KB SRAM)

Design specification for the dense, multi-domain "processor beast" testbench.

## Related Notes
- [[00_Index]]
- [[03_Multi_Voltage_UPF_Standards]]
- [[04_Floorplan_Geometry_and_Macro_Placement]]
- [[05_PDN_Mesh_and_Pad_Trunk_Architecture]]
- [[compiler/01_compiler_ir_architecture]]
- [[algorithms/01_macro_packing_heuristics]]
- [[pdks/01_sky130_site_and_cell_profiles]]

---

## 1. Top-Level Entity Architecture
- **Technology Node**: SkyWater 130nm (`sky130B`)
- **Die Dimensions**: $2400\,\mu\text{m} \times 2400\,\mu\text{m}$ ($5.76\,\text{mm}^2$)
- **Core Area**: $(240.0, 240.0) \to (2160.0, 2160.0)\,\mu\text{m}$ (Margin: $240\,\mu\text{m}$ for peripheral pads)

### Memory Configuration: 6x 1KB SRAM Macros
Master: `sky130_sram_1kbyte_1rw1r_32x256_8` ($479.78\,\mu\text{m} \times 397.50\,\mu\text{m}$)
1. `u_icache_sram0` (North-West, Top Domain)
2. `u_icache_sram1` (North-East, Top Domain)
3. `u_dcache_sram0` (Center-North, Top Domain)
4. `u_npu_weight_sram0` (Inside `PD_NPU` Island @ 0.95V)
5. `u_npu_weight_sram1` (Inside `PD_NPU` Island @ 0.95V)
6. `u_crypto_key_sram0` (Inside `PD_CRYPTO` Island @ 1.20V)

---

## 2. Power Domains (IEEE 1801 UPF 3.0)
- **`PD_TOP`**: $1.80\,\text{V}$ (Always-on host core, caches, I/O pad ring)
- **`PD_NPU`**: $0.95\,\text{V}$ Switched domain, low-power neural matrix multiplier
- **`PD_CRYPTO`**: $1.20\,\text{V}$ Switched domain, AES-256 accelerator

---

## 3. Floorplan Coordinates & Legal Site Row Snapping
- Row height: $2.720\,\mu\text{m}$
- Snapped $y$-origins:
  - Row 1 (South Islands): $y = 120 \times 2.72 = 326.400\,\mu\text{m}$
  - Row 2 (Mid-Level): $y = 350 \times 2.72 = 952.000\,\mu\text{m}$
  - Row 3 (North Cache Bank): $y = 580 \times 2.72 = 1577.600\,\mu\text{m}$
- Channel Clearance: Minimum $45\,\mu\text{m}$ corridor between all adjacent RAMs.
