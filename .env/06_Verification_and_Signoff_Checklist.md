# Verification & Signoff Checklist

This checklist defines the physical and electrical verification gates required before any design proceeds to tapeout manifest generation.

---

## 1. Physical Verification (DRC & Containment)

| Check | Requirement | Verification Method |
| :--- | :--- | :--- |
| **Manufacturing Grid** | $x \pmod{5\,\text{DBU}} = 0$, $y \pmod{5\,\text{DBU}} = 0$ | Pre-flight AST Quantizer |
| **Macro Collisions** | Zero overlapping bounding boxes including halos | `check_floorplan_collisions(strict=True)` |
| **Voltage Island Containment** | Placed logic must stay strictly inside voltage fences | `check_placement` & OpenDB group audit |
| **Core Enclosure** | $\text{Core} \subset \text{Die}$ with margin $\ge 20\,\mu\text{m}$ | Geometric box containment assertion |
| **Channel Corridors** | $\ge 25.0\,\mu\text{m}$ between adjacent macros | Macro channel distance calculation |

---

## 2. Power & IR Drop Verification

| Check | Requirement | Verification Method |
| :--- | :--- | :--- |
| **Static IR Drop** | $\le 5\%$ of $V_{\text{nominal}}$ ($< 90\,\text{mV}$ on 1.8V rail) | PSM Analysis / OpenROAD PDN report |
| **Followpin Rail Continuity**| Zero disconnected row rails on `met1` | `pdngen` verification log |
| **Pad Trunk Connectivity** | All VDD/VSS pads tied to core or domain rings | OpenDB `dbSWire` net traversal |
| **Isolation Net Clamping** | Switched domain boundary outputs clamped to 0/1 | IEEE 1801 UPF verification deck |

---

## 3. Timing & Clock Tree Synthesis (CTS)

| Check | Requirement | Verification Method |
| :--- | :--- | :--- |
| **Worst Negative Slack (WNS)** | $\ge 0.0\,\text{ps}$ (zero setup violations) | OpenSTA `report_checks -path_delay max` |
| **Total Negative Slack (TNS)** | $0.0\,\text{ps}$ across all clock domains | Signoff STA report |
| **Clock Skew** | $\le 100\,\text{ps}$ across synchronized macro banks | TritonCTS report |
| **Antenna Ratio** | Interconnect metal-to-gate area $\le R_{\text{max}}$ | OpenROAD detailed router antenna check |
