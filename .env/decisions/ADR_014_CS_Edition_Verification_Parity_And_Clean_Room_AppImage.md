# ADR 014: CS Edition Verification Parity, Dynamic PDK IP Catalog, and Clean-Room AppImage

## Status
Accepted and Enforced (2026-11-15)

## Context
During user testing of Silis Desktop AppImage and Silis Codespaces Edition (`silis-cs-edition`), two critical classes of defect emerged:
1. **Desktop AppImage Shippability & Clean-Room Leakage**:
   - `core/sdk/pipeline.py` contained a hardcoded host path pointing to `/home/econsystems/.../silis_fp.tcl`. On clean host machines without that path, OpenROAD floorplan execution failed immediately.
   - `assets/ssa/asap7/config.json` and `assets/ssa/gf180mcu/config.json` contained hardcoded developer home paths.
   - `packaging/AppRun` omitted `$APPDIR/third-party/plugins` from `BASE_LD_PATH`, breaking dynamic loading for `ghdl.so` (`libghdl-4_1_0.so` and `libgnat-13.so`).
   - Deliverable AppImage was desynchronized from the latest source tree, retaining the `--disable-gpu-compositing` flag and lacking dual-mode behavioral macro stubs.
2. **Codespaces Edition Functional Parity Omissions**:
   - The project wizard was limited to 5 steps, lacking any testbench (`tb/`) selection step, unlike Desktop Silis which has dedicated testbench methodology configuration.
   - The PDK dropdown and macro picker were hardcoded to a single PDK and failed to dynamically query the server or central macro caches.
   - Placement blockages hardcoded `Hard` and `100%` density regardless of user UI selection, and floorplan TCL emission generated non-existent commands (`create_voltage_fence`, `set_region`).

## Decisions

### 1. Zero Host Path Invariant & Dynamic Resolution
- All runtime TCL sourcing in `Pipeline` must resolve paths dynamically relative to `__file__`:
  ```python
  fp_tcl = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "backendflow", "floorplanner", "silis_fp.tcl")).replace("\\", "/")
  ```
- SSA manifest paths must use standard `~/.silis/pdks/<pdk>` or relative paths within the PDK root.
- `packaging/AppRun` must include all third-party plugin paths in `BASE_LD_PATH`:
  ```bash
  BASE_LD_PATH="$APPDIR/usr/lib:$APPDIR/usr/lib/x86_64-linux-gnu:$APPDIR/opt/python/lib:$QT6_LIB:$WEBENGINE_LIB:$APPDIR/third-party/bin:$APPDIR/third-party/plugins:$APPDIR/third-party/ordlibs:..."
  ```

### 2. 6-Stage Wizard Parity in CS Edition
- `silis-cs-edition` project wizard conforms to the authentic 6-stage physical design flow:
  1. Project Details
  2. RTL Sources
  3. Testbench Sources (`tb/` folder, `.v`/`.sv`/`.vhd` files, top testbench module `#tb-top`)
  4. Constraints (SDC)
  5. Power & IO (UPF / IO)
  6. Silicon Target & Top Module
- Server `/api/projects/create` automatically creates `<project>/tb/` and generates an authentic default verification testbench instantiated against the top module if none is provided.

### 3. Dynamic Multi-PDK & Dual-Mode Simulation Macro Catalog
- CS server exposes `GET /api/pdks` discovering all installed PDKs (`sky130A`, `sky130B`, `gf180mcu`, `asap7`).
- `scan_available_macros(pdk_name)` scans PDK `libs.ref/*/lef/*.lef` and `~/.silis/macrocache/**`, synthesizing dual-mode stubs (`ifndef SYNTHESIS ... `else ... `endif) providing simulation arrays for Verilator/Icarus and blackbox modules for Yosys/OpenROAD.

### 4. Rectilinear Blockages and OpenROAD Command Parity
- Blockage drawing preserves user-selected type (`Hard`, `Soft`, `Partial`) and density percentage.
- TCL emission uses standard OpenROAD commands:
  - Fences: `create_fence -module <name> -box { x1 y1 x2 y2 }`
  - Regions: `create_region -module <name> -box { x1 y1 x2 y2 } -type inclusive`
  - Blockages: `create_placement_blockage -area { x1 y1 x2 y2 } -type hard|soft` or `-type partial -density <pct>`.

## Consequences
- Clean-room portability is 100% verified: `Silis-x86_64.AppImage` runs in isolated `HOME=/tmp/clean_test_home` with zero host path leaks and all dynamic libraries resolved.
- Full parity between Desktop Silis and Codespaces Edition across project creation, testbench configuration, and floorplan script synthesis.
- Hardware-accelerated OpenGL viewports (FastViewer, GDS3D) and GPU compositing operate without diagnostic fallbacks.
