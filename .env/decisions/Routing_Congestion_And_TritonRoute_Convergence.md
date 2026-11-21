# Architecture Decision Record: Resolution of Routing Blocker GRT-0116

## Context
When running `pipe.route()` in the Silis Python SDK on `clean_test`, OpenROAD terminated during FastRoute global routing:
```
[ERROR GRT-0116] Global routing finished with congestion. Check the congestion regions in the DRC Viewer.
```
Although global routing overflow was negligible (1 vertical track on `met4`, total usage 0.80%), OpenROAD's default strict exit code aborted the pipeline before detailed routing (TritonRoute) could resolve the local track contention.

## Root Cause & Inconsistency
In the GUI and headless runner (`silis/core/backendflow/flow/backendflow.py`), `global_route` was invoked with `-allow_congestion`:
```tcl
global_route -guide_file "{guide_path}" -congestion_iterations 50 -allow_congestion -verbose
```
However, in `silis/core/sdk/pipeline.py`, `Pipeline.route()` omitted `-allow_congestion`. This violated the **Zero GUI-SDK Deviance Law**.

## Solution
1. Updated `Pipeline.route()` in `silis/core/sdk/pipeline.py` to support `allow_congestion: bool = True` by default.
2. Verified that global routing hands off cleanly to TritonRoute.
3. TritonRoute finished optimization across 3 iterations, converging from 288 violations down to **0 violations** (`[INFO DRT-0199] Number of violations = 0`).
4. Output DEF generated cleanly (18.28 MB, 29,757 components, zero DRC violations).

## Related Notes
- [[00_Index]]
- [[decisions/Detailed_Placement_Pad_Alignment_Blocker]]
- [[toolchain/OpenROAD_Personality]]
