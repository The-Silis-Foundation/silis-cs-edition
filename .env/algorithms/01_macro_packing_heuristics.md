# Algorithmic Macro Placement & Corridor Spacing Heuristics

This note derives the mathematical constraints for automatic macro floorplanning, corridor preservation, and pin escape corridors in the Silis engine.

## Related Notes
- [[00_Index]]
- [[04_Floorplan_Geometry_and_Macro_Placement]]
- [[compiler/01_compiler_ir_architecture]]
- [[scratchpad/beast_processor_floorplan_plan]]

---

## 1. Channel Spacing Formulation
For two macros $M_a, M_b$ separated by a routing corridor:
$$W_{\text{channel}} \ge \text{Halo}_a + \text{Halo}_b + W_{\text{routing\_tracks}}$$

Where:
- Standard macro halo: $\text{Halo} = 10.0\,\mu\text{m}$
- Intermediate routing layer track allocation (`met2` pitch $0.46\,\mu\text{m}$, `met3` pitch $0.68\,\mu\text{m}$):
  $$W_{\text{routing\_tracks}} \approx 20 \times 0.68\,\mu\text{m} \approx 13.6\,\mu\text{m} \implies \text{Margin} \approx 15.0\,\mu\text{m}$$
- Total mandatory corridor:
  $$W_{\text{channel}} \ge 10.0 + 10.0 + 15.0 = 35.0\,\mu\text{m}$$

---

## 2. Standard Cell Row Snapping
For SkyWater 130nm `unithd`:
- Step height $H_{\text{row}} = 2.720\,\mu\text{m}$
- For any placed macro $M_i$:
  $$y_{\text{origin}} = \text{round}\left(\frac{y}{2.720}\right) \times 2.720\,\mu\text{m}$$
  $$\text{assert } (y_{\text{origin}} \pmod{2.720}) \equiv 0.000$$

---

## 3. Rectilinear Domain Fence Decomposition
When an IEEE 1801 voltage island encompasses macros and core cells:
$$\text{Area}(\text{Domain}) \ge \sum \text{Area}(M_i) + \frac{\text{Area}(\text{Domain Cells})}{\rho_{\text{target}}}$$
Target utilization $\rho_{\text{target}} \in [0.45, 0.60]$.
