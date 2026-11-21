# SkyWater 130nm Quirks

SkyWater 130nm (`sky130A` / `sky130B`) is an open-source 130nm bulk CMOS process.

## Essential Process Quirks

1. **Local Interconnect Layer (`li1`)**:
   Unlike standard foundry stacks, Sky130 features a thin titanium-nitride local interconnect layer (`li1`) directly above diffusion/polysilicon. Standard cells use `li1` for internal routing and pin access. Standard cell rows must have `li1` routing tracks enabled.

2. **The Double-Height Site Trap**:
   Standard logic gates use `SITE unithd` ($2.72\,\mu\text{m}$ high).
   Power switches and specialized cells use `SITE unithddbl` ($5.44\,\mu\text{m}$ high). Mixing them into standard rows causes placement legalization failure.

3. **Routing Metal Stack**:
   - `met1` (Horizontal, pitch $0.34\,\mu\text{m}$)
   - `met2` (Vertical, pitch $0.46\,\mu\text{m}$)
   - `met3` (Horizontal, pitch $0.68\,\mu\text{m}$)
   - `met4` (Vertical, pitch $0.92\,\mu\text{m}$)
   - `met5` (Horizontal, pitch $3.40\,\mu\text{m}$ - thick top power metal)

## Related Notes
- [[00_Index]]
- [[pdks/01_sky130_site_and_cell_profiles]]
- [[power/Architectural_vs_Physical_Switches]]
- [[toolchain/OpenROAD_Personality]]
