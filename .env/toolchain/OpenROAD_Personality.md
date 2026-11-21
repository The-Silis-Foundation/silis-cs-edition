# OpenROAD Internal Personality

OpenROAD is an open-source, high-speed C++ physical design engine. To work with it reliably, you must understand its internal quirks and behaviors.

## Key Quirks & Behaviors

1. **LEF File Read Ordering**:
   OpenROAD requires technology LEFs (`*.tlef`) to be loaded before macro cell LEFs. If a macro LEF references a layer before the tech LEF defines it, OpenROAD crashes or drops layer properties silently.

2. **The `pdn::PdnGen` Parser**:
   When UPF power intent is loaded, `importUPF` expects complete, legal supply net definitions. If a power switch is declared without mapped physical supply ports or in an incompatible row, it triggers null-port segfaults (`Signal 11`) inside `PdnGen.cc`.

3. **Detailed Placement (`DPL`) & Pad Alignment Blockers**:
   OpenROAD detailed placement checks macros and standard cells against standard cell sites.
   - *Encountered Hindrance*: `check_placement` flagged 159 site alignment failures (`DPL-0006`).
   - *Root Cause*: OpenROAD evaluated peripheral I/O pads (`sky130_ef_io`, `sky130_fd_io`) against standard core `unithd` rows. Because pads sit along the die boundary outside core cell rows, they triggered false alignment failures.
   - *Resolution*: Pad instances must be placed as `+ FIXED` and isolated from core cell placement validation.

4. **DEF Region Strictness**:
   Non-rectangular voltage domain regions must be decomposed into rectilinear rectangles (`REGIONS`). OpenROAD will reject self-intersecting or overlapping region boxes.

## Related Notes
- [[00_Index]]
- [[02_EDA_Error_Playbook]]
- [[philosophy/Zero_Catch_Law]]
- [[silicon/SkyWater130_Quirks]]
