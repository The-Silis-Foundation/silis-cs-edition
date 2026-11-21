# SkyWater 130nm & Multi-PDK Technology Profiles

Hardware cell profiles and physical dimensions across supported PDK libraries.

## Related Notes
- [[00_Index]]
- [[02_EDA_Error_Playbook]]
- [[03_Multi_Voltage_UPF_Standards]]
- [[compiler/01_compiler_ir_architecture]]

---

## 1. SRAM Macro Footprints (Sky130B)
Library: `/home/econsystems/.silis/pdks/sky130B/libs.ref/sky130_sram_macros/lef/`

| Cell Master | Width ($\mu\text{m}$) | Height ($\mu\text{m}$) | Aspect Ratio | Pins / Ports |
| :--- | :--- | :--- | :--- | :--- |
| `sky130_sram_1kbyte_1rw1r_32x256_8` | **479.780** | **397.500** | 1.207 | 1RW 1R, 32-bit |
| `sky130_sram_1kbyte_1rw1r_8x1024_8` | **455.300** | **446.460** | 1.020 | 1RW 1R, 8-bit |
| `sky130_sram_2kbyte_1rw1r_32x512_8` | **683.100** | **416.540** | 1.640 | 1RW 1R, 32-bit |

---

## 2. Power Switch & Site Conventions
- Standard Core Row Site: `SITE unithd` (Height: $2.72\,\mu\text{m}$, Width: $0.46\,\mu\text{m}$)
- Physical Power Switch Cell: `sky130_power_switch` requires `SITE unithddbl` (Height: $5.44\,\mu\text{m}$)
- **Downstream Trap**: Mapping `POWER_SWITCH` into a floorplan without double-height rows throws `[ERROR PDN-0240]`. Power switches must remain architectural unless `SILIS_ENABLE_PHYSICAL_POWER_SWITCH=1` and double-height rows are explicitly allocated.
