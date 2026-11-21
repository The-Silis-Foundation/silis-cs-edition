# PDN Mesh & Pad Trunk Architecture

This document defines the Power Distribution Network (PDN) architecture, metal layer distribution, multi-voltage grid integration, and dedicated periphery pad trunk routing.

---

## 1. Dual-Layer Orthogonal Grid Architecture

Silis implements the SiliconCompiler/OpenROAD-proven dual-layer orthogonal power mesh:

| Layer | Orientation | Width | Pitch | Function |
| :--- | :--- | :--- | :--- | :--- |
| `met1` | Horizontal | $0.48\,\mu\text{m}$ | Row height ($2.72\,\mu\text{m}$) | Followpin standard-cell power/ground rails |
| `met4` | Vertical | $1.60\,\mu\text{m}$ | $27.20\,\mu\text{m}$ | Intermediate vertical distribution trunks |
| `met5` | Horizontal | $1.60\,\mu\text{m}$ | $27.20\,\mu\text{m}$ | Top horizontal distribution straps & core ring |

---

## 2. Core Peripheral Power Ring

The chip core perimeter is wrapped with low-impedance power rings:
```tcl
add_pdn_ring -grid {grid_top} \
    -layers "met4 met5" \
    -widths {5.0 5.0} \
    -spacings {2.0 2.0} \
    -core_offsets {5.0 5.0}
```
- **Connection**: `add_pdn_connect -grid {grid_top} -layers "met4 met5"` creates the required via matrix (`M4M5_PR`) at all orthogonal ring intersections.

---

## 3. Dedicated Periphery Pad Trunk Bridges

Pad power/ground pins sit on the chip periphery, separated from the core ring by a $200\,\mu\text{m}$ pad-ring margin. Silis automatically synthesizes dedicated low-resistance metal trunks to bridge pads directly to the ring:

```
[ PAD: pad_vdd_core_89 ] 
           |
           |  (met4 vertical trunk, width = 3.2 um)
           v
  ===================== (met5 Core Power Ring Leg)
```

### Invariants:
1. **Direct Abutment**: Trunks connect pad pins to the nearest core ring leg (South, North, East, West).
2. **Via Stacking**: Connections drop from `met4` to pad pins on `met3` via `M3M4_PR` via arrays.
3. **Domain Net Awareness**: Dedicated domain pads (e.g. `pad_vdd_npu`) bridge directly to their corresponding domain rings (`grid_pd_npu`) rather than the core `VDD` ring.
