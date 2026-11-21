# Physical Design as Compilation

Physical design is not manual drafting. It is **hardware compilation**.

Just as an optimizing compiler takes high-level C/Rust, parses it into an AST, lowers it to LLVM IR, and compiles it to machine code:
- **Frontend Source**: Verilog RTL, IEEE 1801 UPF 3.0 (power intent), and IEEE 1497 SDC (timing constraints).
- **Compiler AST**: Synthesized gate netlist + power state tables + clock graphs.
- **Intermediate Representation (IR)**: DEF and LEF physical layout structures.
- **Backend Machine Code**: OpenROAD and downstream engine execution scripts.

## The Mental Model
When something fails downstream, we do not patch the output layout by hand. We debug the **compiler lowering passes** inside the Silis SDK.

## Related Notes
- [[00_Index]]
- [[philosophy/Zero_Catch_Law]]
- [[philosophy/GUI_as_a_Projection]]
- [[decisions/Authoritative_SDK_State]]
