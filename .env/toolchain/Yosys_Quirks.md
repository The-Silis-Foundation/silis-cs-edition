# Yosys Synthesis Quirks

Yosys is our authoritative logic synthesis compiler.

## Key Quirks & Behaviors

1. **Unintentional Latch Inference**:
   If an `always @(*)` combinational block fails to assign a signal under all conditional branches, Yosys infers an asynchronous transparent latch (`$_DLATCH_P_`). Latches break downstream STA (Static Timing Analysis) and CTS.
   - *Fix*: Always initialize signals at the top of combinational blocks.

2. **Blackbox Macro Handling**:
   SRAM memory blocks and analog cells must be declared with `(* blackbox *)` in Verilog so Yosys does not attempt to synthesize their internal bitcells into logic gates.
   - *Trap*: Ensure the blackbox port widths match the instantiated ports exactly.

3. **Hierarchy Preservation for Power Domains**:
   To bind an RTL module (`u_npu`) to an IEEE 1801 power domain, its hierarchy must be preserved during synthesis. Using aggressive flattened synthesis (`synth -flatten`) destroys domain boundaries.

## Related Notes
- [[00_Index]]
- [[philosophy/Physical_Design_as_Compilation]]
- [[power/Why_Power_Domains_Exist]]
