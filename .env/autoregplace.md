# Silis Hierarchical Auto Region Placer & Shaper Specification

## 1. Overview & Objective

In modern digital ASIC physical design, floorplanning standard cell modules into distinct physical constraints (`create_region` / `create_fence`) is critical for timing closure, clock-tree balancing, and routing congestion control. However, manual floorplanning suffers from two major bottlenecks:

1. **Arbitrary Geometry & Density Mismatch (The Shaping Problem)**:
   - When designers drag arbitrary bounding boxes, they inadvertently cause over-dense regions (>85% density, causing routing DRCs and placement congestion) or under-dense regions (<20% density, wasting die area and stretching net lengths).
2. **Suboptimal Placement & Overlap (The Placement Problem)**:
   - Placing standard cell regions without accounting for connected macros or existing blockages leads to illegal overlaps, channel blockage, or severe timing penalties across macro-to-logic boundaries.

The **Auto Region Placer & Shaper** provides an automated, hierarchy-aware geometric engine accessible via both the **SilisIDE Interactive Floorplanner GUI** and the **Silis Python SDK/CLI**.

---

## 2. User Experience (GUI Workflow)

The GUI preserves designer autonomy while providing instant mathematical optimization and non-destructive rollbacks.

### 2.1 Workflow Steps
1. **Interactive Definition**:
   - The user selects a module in the hierarchy dropdown (`cmb_region_module`), picks a target density (default `0.50` to `0.60`), and creates a region or fence.
2. **Buffer Memory Snapshot**:
   - Whenever regions exist on the canvas, clicking **"Auto Place Placed Regions"** immediately captures a snapshot of all region positions and dimensions into `self._saved_region_snapshot`.
3. **Automated Hierarchical Execution**:
   - The engine automatically calculates the exact area required, shapes optimal aspect ratios snapped to PDK standard cell site rows, maps module hierarchy to neighboring macros, and packs the regions into valid, non-overlapping core cavities.
   - The visual canvas animates and updates all `RegionItem` objects to their new legalized coordinates.
   - `live_tcl_edit` generates the corresponding OpenROAD `create_region` or `create_fence` commands.
4. **Non-Destructive Restoration**:
   - A **"Restore Regions"** button becomes active. If the designer prefers their manual arrangement or wants to compare layouts, clicking "Restore Regions" immediately rolls the canvas back to the snapshot.

---

## 3. Python SDK & CLI Representation

The Python SDK exposes identical declarative functionality. Users can supply explicit bounding boxes, or declare `is_autoplace: True`:

```python
import silis

pipe = silis.Pipeline("sram_soc_system")

pipe.floorplan(
    die_area=[0, 0, 2000, 2000],
    core_margin=20.0,
    macros=[
        pipe.place_macro("u_sram_0", at=(50, 50), orient="MY", halo=10.0),
        pipe.place_macro("u_sram_1", at=(1500, 50), orient="R0", halo=10.0),
    ],
    regions=[
        {
            "module": "u_code_cache",
            "is_autoplace": True,    # Engine will auto-shape and auto-place
            "is_fence": False,
            "density": 0.55,
            "aspect_ratio": 1.0      # Optional: default is 1.0 (square)
        },
        {
            "module": "u_alu",
            "is_autoplace": True,
            "is_fence": True,
            "density": 0.60,
        },
        {
            # Explicit manual region supported concurrently:
            "module": "u_uart",
            "box": [100.0, 100.0, 300.0, 300.0],
            "is_fence": True,
            "density": 0.50
        }
    ]
)
```

In the pipeline runner, any region with `is_autoplace: True` (or where `box` is omitted) invokes the backend solver before generating OpenROAD TCL.

---

## 4. Backend Computational Algorithms

The backend engine executes a 4-phase optimization pipeline:

```
+-------------------------------------------------------------------+
|               Phase 1: Analytical Module Sizing                   |
|   A_cell = get_module_area(M) -> A_req = A_cell / density         |
|   W = sqrt(A_req * AR), H = A_req / W -> Snap to Site Rows        |
+-------------------------------------------------------------------+
                                  |
                                  v
+-------------------------------------------------------------------+
|            Phase 2: Hierarchical & Macro Affinity                 |
|   Correlate module to driving macros & sibling modules            |
|   Compute target gravitational anchor (x*, y*)                    |
+-------------------------------------------------------------------+
                                  |
                                  v
+-------------------------------------------------------------------+
|               Phase 3: Core Cavity Field Construction             |
|   Cavity = Core - (Macros + Halos + Corridors + Blockages)        |
|   Decompose remaining free space into candidate 2D rectangles    |
+-------------------------------------------------------------------+
                                  |
                                  v
+-------------------------------------------------------------------+
|        Phase 4: Best-Fit Decreasing Packing & Legalization        |
|   Sort regions by area descending -> Allocate to closest cavity   |
|   Verify strictly non-overlapping & clamp inside core boundaries |
+-------------------------------------------------------------------+
```

### 4.1 Phase 1: Analytical Module Sizing & Shaper
For each region $i$ with module $M_i$ and target density $D_i \in (0.0, 1.0]$:
1. Query module standard cell area:
   $$A_{\text{cell}} = \text{get\_module\_area}(M_i)$$
   If the design sizes report (`*_sizes.json`) is not yet generated, fall back to a safe default ($100.0\ \mu\text{m}^2$).
2. Calculate required physical bounding box area:
   $$A_{\text{req}} = \frac{A_{\text{cell}}}{D_i}$$
3. Given aspect ratio $AR = \frac{W}{H}$ (default $1.0$, square):
   $$W_{\text{ideal}} = \sqrt{A_{\text{req}} \times AR}, \quad H_{\text{ideal}} = \frac{A_{\text{req}}}{W_{\text{ideal}}}$$
4. **Site Grid Snapping**:
   - Standard cell height: snapped to integer multiples of PDK row height ($H_{\text{row}} = 2.72\ \mu\text{m}$ on Sky130):
     $$H_{\text{snapped}} = \max\left(H_{\text{row}}, \text{round}\left(\frac{H_{\text{ideal}}}{H_{\text{row}}}\right) \times H_{\text{row}}\right)$$
   - Width: recomputed to maintain target area and snapped to manufacturing grid ($0.005\ \mu\text{m}$):
     $$W_{\text{snapped}} = \text{round}\left(\frac{A_{\text{req}}}{H_{\text{snapped}} \times 0.005}\right) \times 0.005$$

### 4.2 Phase 2: Hierarchical & Macro Affinity Mapping
To minimize routing congestion and wirelength, standard cell regions should be placed close to the hardware blocks they interface with:
1. **Direct Macro Affinity**:
   - If module $M_i$ instantiates or directly routes signals to macro $K_j$ (e.g. `u_code_cache` interfacing with `u_sram_0`), the ideal anchor point $(x^*, y^*)$ is set along the macro's pin channel facing the core interior.
2. **Sibling Module Affinity**:
   - If modules $M_a$ and $M_b$ share a parent hierarchy (e.g. `u_core0/u_alu` and `u_core0/u_regfile`), their gravitational anchors are grouped into the same core quadrant.
3. **Unbound Logic**:
   - Modules with no strong macro affinity (e.g. top-level interconnect, crossbar arbiters) gravitate toward the core center.

### 4.3 Phase 3: Core Cavity Field Construction
1. Start with the full available core area:
   $$\mathcal{C} = [x_{\text{core1}}, y_{\text{core1}}, x_{\text{core2}}, y_{\text{core2}}]$$
2. Subtract all obstacles:
   - Placed macros expanded by their halos plus a $10.0\ \mu\text{m}$ power/routing corridor.
   - Hard and soft placement blockages.
   - Fixed, manually placed regions.
3. Decompose the remaining free space into a set of maximal disjoint rectangles (cavities) $\{\mathcal{R}_1, \mathcal{R}_2, \dots, \mathcal{R}_k\}$.

### 4.4 Phase 4: Best-Fit Decreasing (BFD) Allocation & Legalization
1. Sort regions in descending order of required area $A_{\text{req}}$ (largest modules placed first).
2. For each region:
   - Evaluate candidate cavities $\mathcal{R}_k$ that can enclose dimensions $(W_{\text{snapped}}, H_{\text{snapped}})$.
   - Select the cavity that minimizes the Euclidean distance from the cavity center to the module's gravitational anchor $(x^*, y^*)$.
   - Place the region at $(x_1, y_1)$ inside the cavity, aligned with row tracks.
   - Update the cavity list by guillotine-splitting the remaining free space.
3. Verify that all fence coordinates are strictly disjoint and fully enclosed within `core_rect`.

---

## 5. Summary of Architecture Deliverables

| Subsystem | Component / Method | Description |
| :--- | :--- | :--- |
| **Backend Engine** | `solve_hierarchical_region_placement(...)` | Computes shaped bounding boxes and cavity allocations. |
| **GUI Controls** | `btn_autoplace_regions` | One-click auto place and auto shape for placed regions. |
| **GUI Safety** | `btn_restore_regions` | Reverts regions to the snapshot in `_saved_region_snapshot`. |
| **Python SDK** | `is_autoplace: True` | Declarative region auto-placement parameter in `pipe.floorplan()`. |
| **TCL Emission** | `generate_floorplan_tcl(...)` | Outputs legalized OpenROAD `create_region` and `create_fence` syntax. |
