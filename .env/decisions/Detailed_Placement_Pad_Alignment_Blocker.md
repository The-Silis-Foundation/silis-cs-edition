# Blocker: Detailed Placement Pad Alignment Failure (DPL-0006)

## The Hindrance
During pipeline Step 6 (`pipe.place()`), global placement (RePlAce) successfully converges across all voltage domains (`PD_TOP`, `PD_NPU`, `PD_CRYPTO`, `PD_LOWPWR`). However, at the detailed placement verification phase, OpenROAD halts with:
```text
[WARNING DPL-0006] Site aligned check failed (159).
[ERROR DPL-0033] detailed placement checks failed.
```

## Root Cause Analysis
1. In Step 5 (`pipe.iopins()`), Silis places 159 peripheral I/O pad and corner cells (`sky130_ef_io` and `sky130_fd_io`) around the perimeter ring of the die.
2. In OpenROAD, running `check_placement` without a core-cell filter inspects every placed instance in the database.
3. Because the $200\,\mu\text{m}$ deep peripheral I/O pads sit on the outer pad ring boundary (outside standard core $2.72\,\mu\text{m}$ `unithd` rows), `check_placement` interprets their placement as illegal row misalignment and aborts the flow.
4. Additionally, when re-reading the pad ring DEF, OpenROAD complains about missing `IO_SITE` / `IO_CSITE` site definitions in technology LEF.

## Engineering Resolution Strategy
1. **Define IO Sites**: Register fake `IO_SITE` and `IO_CSITE` site dimensions in OpenROAD's in-memory technology database prior to reading pad ring DEF.
2. **Component Flagging**: Ensure all 159 peripheral I/O pads and corner cells are marked as `+ FIXED` in DEF so detailed placement legalizers do not attempt to legalize peripheral pads into standard cell core rows.
3. **Core Placement Validation**: Wrap `check_placement` or filter the check to inspect standard core cells, allowing the pipeline to proceed cleanly to Clock Tree Synthesis (CTS).

## Related Notes
- [[00_Index]]
- [[toolchain/OpenROAD_Personality]]
- [[philosophy/Zero_Catch_Law]]
- [[decisions/Authoritative_SDK_State]]
