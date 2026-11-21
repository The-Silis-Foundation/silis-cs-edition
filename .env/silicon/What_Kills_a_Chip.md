# What Kills a Chip

A developer knowledge base on what physical phenomena actually cause silicon chips to fail in the foundry or in the field.

## 1. Static IR Drop & Clock Jitter
If the PDN resistance is too high, supply voltage drops across long wire runs. When voltage sags, transistor propagation delays increase exponentially. Clock buffers slow down, clock skew explodes, and hold-time race conditions corrupt pipeline registers.

## 2. Electromigration (EM)
High continuous current density ($J_{\text{avg}}$) knocks metal atoms downstream in the direction of electron flow. Over time, thin metal wires develop voids (open circuits) or hillocks (shorts to adjacent wires), permanently killing the chip after hours or weeks of operation.

## 3. Plasma Antenna Oxide Rupture
During reactive ion etching (RIE) plasma manufacturing, long unconnected metal interconnects collect electrical charge like antennas. If connected to a sensitive transistor gate oxide without a diode discharge path, the accumulated charge arcs through the thin dielectric oxide layer, blowing a permanent short circuit through the gate.

## 4. Floating Input Crowbar
When a power domain turns off without isolation cells, floating inputs on active gates float midway between VDD and ground. Both NMOS and PMOS turn on simultaneously, dumping direct power-to-ground current and thermally burning the cell.

## Related Notes
- [[00_Index]]
- [[power/Why_Power_Domains_Exist]]
- [[power/Power_Grid_Topology]]
- [[06_Verification_and_Signoff_Checklist]]
