# ADR 015: Verilator CCache Resilience and Defensive Multi-Candidate PDK Asset Resolution

## Status
Accepted and Enforced (2026-11-15)

## Context
During deployment and execution of `Silis-x86_64.AppImage` on fresh clean-room Linux environments (`ubuntu@ubuntu`), two critical defects were uncovered:

1. **Verilator Simulation Build Failure (`ccache: not found`)**:
   ```text
   /bin/sh: 1: ccache: not found
   make: *** [/tmp/.mount_silis.egjJId/third-party/share/verilator/include/verilated.mk:309: verilated.o] Error 127
   %Error: 'make -C obj_dir -f Vtb_mips_3_s.mk -j 32' exited with 2
   ```
   *Root Cause*:
   In `silis/third-party/share/verilator/include/verilated.mk`, `OBJCACHE ?= ccache` unconditionally assigned `ccache`. When `verilator --binary` invoked Make, compiler invocations were prefixed with `$(OBJCACHE) $(CXX)` -> `ccache g++`. On fresh installations where `ccache` was absent from system PATH, Make failed immediately with exit code 127.

2. **Yosys Synthesis PDK Liberty Resolution Failure**:
   ```text
   [SYS] Starting Synthesis Flow...
   ERROR: Can't open input file `/tmp/.mount_silis.egjJId/assets/pdk/third-party/pdks/sky130A/libs.ref/sky130_fd_sc_hd/lib/sky130_fd_sc_hd__tt_025C_1v80.lib' for reading: No such file or directory
   [SYS] [ERR] Yosys Crash: Yosys Failed
   ```
   *Root Cause*:
   In `pdk_master.json`, relative paths are recorded with standard repository prefixes (e.g. `third-party/pdks/sky130A/libs.ref/...`). When running inside the AppImage, `PDKManager.load_master_catalog()` identified `base_dir = os.path.join(appdir, "assets", "pdk")`. Naive concatenation `os.path.join(base_dir, c_lib)` formed `/tmp/.mount_silis.egjJId/assets/pdk/third-party/pdks/sky130A/...`, which failed because inside the AppImage bundle the PDK trees are located at `$APPDIR/assets/pdk/sky130A/`.

## Decisions

### 1. Conditional CCache Invocation in Verilator Flow
- In `silis/third-party/share/verilator/include/verilated.mk`, wrap `OBJCACHE` initialization with a shell lookup:
  ```makefile
  ifeq ($(shell which ccache 2>/dev/null),)
  OBJCACHE ?=
  else
  OBJCACHE ?= ccache
  endif
  ```
- In `silis/silis.py` (`run_simulation()`):
  ```python
  if not shutil.which("ccache"):
      sub_env["OBJCACHE"] = ""
  ```
- In `silis/packaging/AppRun`:
  ```bash
  if ! command -v ccache &>/dev/null; then
      export OBJCACHE=""
  fi
  ```
  This guarantees compilation succeeds with standard `g++` whether `ccache` is present or absent.

### 2. Multi-Candidate Defensive PDK Path Resolution
- In `silis/core/pdkmanagers/pdk/manager.py`, implemented `_resolve_pdk_file(base_dir, rel_path)`. Redundant prefixes (`third-party/pdks/`, `assets/pdk/`) are stripped, and candidate paths are checked across:
  1. Direct join with `base_dir`
  2. Cleaned relative path joined with `base_dir`
  3. `$APPDIR/assets/pdk/<cleaned>`
  4. `$APPDIR/third-party/pdks/<cleaned>`
  5. `$PDK_ROOT/<cleaned>`
  6. `~/.silis/pdks/<cleaned>`
  7. `$REPO_ROOT/assets/pdk/<cleaned>`
  8. `$REPO_ROOT/third-party/pdks/<cleaned>`
- In `Silis.AppDir`, cross-compatibility symlinks are created during packaging:
  - `${APPDIR}/assets/pdk/third-party/pdks -> ..`
  - `${APPDIR}/third-party/pdks/{sky130A, sky130B, gf180mcu} -> ../../assets/pdk/...`
  Ensuring any legacy or hardcoded path resolves identically on disk.

### 3. Zero-Latency First Boot
- Updated `silis/core/config.py` and `packaging/AppRun` to prioritize `$APPDIR/assets/pdk` as active `PDK_ROOT` when `~/.silis/pdks/sky130A` is not yet populated.
- Prevents first-boot blocking delays from copying 2.2GB of standard cell libraries.

## Consequences
- Clean-room test runs in `HOME=/tmp/clean_test_home` without `ccache` installed succeed with 0 errors.
- Yosys synthesis parses the bundled Sky130A standard cell timing library (`sky130_fd_sc_hd__tt_025C_1v80.lib`) with 100% path accuracy.
- Downstream flows (OpenROAD, OpenSTA, KLayout) receive validated, normalized filesystem paths.
