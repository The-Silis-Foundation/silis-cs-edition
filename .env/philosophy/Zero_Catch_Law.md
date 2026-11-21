# The Zero-Catch Law

Never wrap downstream engine commands in `catch { ... }` or swallow exceptions in Python with `try...except: pass`.

## Why This Rule Exists
In standard web or app software, catching an error keeps the UI alive.
In semiconductor design, catching an error allows a broken circuit, an unrouted power rail, or an overlapping macro boundary to silently proceed down the pipeline.

## The Silicon Reality
A silent error that bypasses DRC or placement checks reaches physical tapeout. Once the quartz mask is etched and silicon is baked at the foundry, a single disconnected via or shorted rail results in a dead chip and millions of dollars in lost wafer runs.

## The Rule
Fail loudly, immediately, and with precise diagnostic context.
If OpenROAD or Yosys throws an error, Silis must fix the root structural flaw in the design intent, never hide the error.

## Related Notes
- [[00_Index]]
- [[philosophy/Physical_Design_as_Compilation]]
- [[02_EDA_Error_Playbook]]
- [[toolchain/OpenROAD_Personality]]
