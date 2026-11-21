# Power Grid Topology

The power distribution network (PDN) is the circulatory system of the ASIC.

## The Strategy
1. **Peripheral Supply Pads**:
   VDD and VSS pads surround the die perimeter to deliver high current uniformly from bond wires into the chip.
2. **Dedicated Pad Trunks**:
   Wide metal trunks connect peripheral pads directly to core power rings, preventing localized starvation at the die edges.
3. **Dual-Layer Orthogonal Mesh**:
   - High-level horizontal straps on top thick metals (e.g., `met5`).
   - High-level vertical straps on orthogonal layer (e.g., `met4`).
   - Vias at every intersection create a dense, low-impedance mesh.
4. **Standard Cell Rails**:
   Lowest metal layer (`met1`) connects to individual transistor standard cell tapcells, fed vertically by vias dropping down from the mesh.

## The Physics Goal
Keep static and dynamic IR drop under **5% of nominal voltage** across every micron of the die.

## Related Notes
- [[00_Index]]
- [[05_PDN_Mesh_and_Pad_Trunk_Architecture]]
- [[power/Why_Power_Domains_Exist]]
- [[silicon/What_Kills_a_Chip]]
