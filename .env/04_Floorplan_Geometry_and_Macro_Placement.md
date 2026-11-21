# Floorplan Geometry, Manufacturing Grid & Macro Placement

This document details the geometric invariants, manufacturing grid legalization, macro keepout halos, and routing channel rules for Silis ASIC floorplanning.

---

## 1. Manufacturing Grid Quantization (Sky130 Invariant)

In the SkyWater 130nm process, the physical database unit is:
$$1\,\mu\text{m} = 1000\,\text{DBU}, \quad g = 0.005\,\mu\text{m}\ (5\,\text{DBU})$$

Every physical entity (die bounds, core rectangle, macro origin, halo width, region box) must satisfy:
$$(x \pmod{g}) = 0, \quad (y \pmod{g}) = 0$$

### Quantization Function:
```python
def legalize_coord(val: float, grid: float = 0.005) -> float:
    return round(round(val / grid) * grid, 3)
```

---

## 2. Standard Cell Row Snapping

Standard cell rows in Sky130 `unithd` have fixed dimensions:
- **Site Height**: $2.720\,\mu\text{m}$
- **Site Width**: $0.460\,\mu\text{m}$

Every macro $Y$-origin and core lower-left corner MUST snap to a legal row origin:
$$y_{\text{origin}} = y_{\text{core\_bottom}} + k \times 2.720\,\mu\text{m}, \quad k \in \mathbb{Z}$$

---

## 3. Macro Halo & Keepout Spacing

Every placed macro (e.g. SRAM memory) must have an exclusion halo to prevent detailed router DRC explosions:
$$\text{Halo}_{\text{min}} = 10.0\,\mu\text{m}$$

```
+-------------------------------------------------------+
|                   KEEP-OUT HALO (10 um)               |
|   +-----------------------------------------------+   |
|   |                                               |   |
|   |               MACRO INSTANCE                  |   |
|   |         (e.g. sky130_sram_1kbyte)             |   |
|   |                                               |   |
|   +-----------------------------------------------+   |
|                                                       |
+-------------------------------------------------------+
```

### Channel Width Invariant:
Between any two adjacent macros $M_1$ and $M_2$:
$$W_{\text{channel}} \ge \text{Halo}_1 + \text{Halo}_2 + W_{\text{routing\_tracks}}$$
Minimum spacing between adjacent macro boundaries must exceed **$25.0\,\mu\text{m}$** to accommodate at least 20 vertical routing tracks on intermediate metal layers (`met2`, `met3`).

---

## 4. Bounding Box & Containment Verification

Before disk emission, Silis executes `check_floorplan_collisions(strict=True)`:
1. **Out of Bounds Check**: Verifies all macros and regions sit entirely within die area.
2. **Macro vs Macro Collision**: Verifies $\text{BBox}(M_i + \text{Halo}_i) \cap \text{BBox}(M_j + \text{Halo}_j) = \emptyset$.
3. **Fence vs Fence Collision**: Verifies exclusive logic islands do not overlap.
4. **Macro vs Fence Containment**: Verifies that macros belonging to a power domain do not penetrate or straddle the domain fence boundary.
