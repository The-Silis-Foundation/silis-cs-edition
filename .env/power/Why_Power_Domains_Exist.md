# Why Power Domains Exist

Modern ASICs cannot afford to have every transistor running at full voltage simultaneously.

## The Core Concept
Power domains partition a silicon die into distinct electrical islands.
- **Always-On Domain (`PD_TOP`)**: Contains the system controllers, interrupt handlers, and I/O pads.
- **Switched Domains (`PD_NPU`, `PD_CRYPTO`)**: High-performance blocks powered down to 0V when idle to save leakage energy.

## The Physical Consequences
1. **Floating Nets & CMOS Destruction**:
   When a domain powers down ($0\text{V}$), its output wires float. If a floating wire touches an active gate in an always-on domain, both PMOS and NMOS transistors can turn on simultaneously, causing massive crowbar current and oxide destruction.
   - *The Fix*: **Isolation Cells** clamp floating outputs to a known legal binary 0 or 1.
2. **Voltage Gradients**:
   Driving a $0.95\text{V}$ logic signal directly into a $1.8\text{V}$ gate causes slow switching, metastability, and high static leakage.
   - *The Fix*: **Level Shifters** step voltage up or down cleanly.

## Related Notes
- [[00_Index]]
- [[03_Multi_Voltage_UPF_Standards]]
- [[power/Architectural_vs_Physical_Switches]]
- [[power/Power_Grid_Topology]]
