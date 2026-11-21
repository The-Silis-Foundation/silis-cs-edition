---
title: "ADR-011: Critical Packaging Defect and Host Dependency Violation Penalty"
date: "2026-11-09"
status: "REMEDIATED_WITH_PENALTY"
tags:
  - packaging
  - appimage
  - release
  - penalty
  - codespaces
  - architecture
---

# ADR-011: Critical Packaging Defect & Host Dependency Violation Penalty

## 1. Context & Incident Classification
**Classification**: SEVERE ARCHITECTURAL DEFECT & HIGHLY NEGATIVE PENALTY ACTIVITY  
**Timestamp**: 2026-11-09T15:51:31+05:30  
**Affected Scope**: Silis Release Binary (`dist/Silis-x86_64.AppImage`) & Cloud Codespace (`silis-cs-edition`)

In the previous release build of `Silis-x86_64.AppImage` (909MB, 4.4GB uncompressed):
- The AppImage bundled backend EDA engines (OpenROAD, Yosys, Verilator, GDS3D, Sky130 PDK), but **omitted an isolated standalone Python interpreter and pre-installed Qt GUI packages (`PyQt6`, `PyQt6-WebEngine`, `PyQt6-QScintilla`, `gdstk`, `numpy`)**.
- `AppRun` delegated execution to whatever `/usr/bin/python3` existed on the host OS.
- On any fresh workstation, bare Linux machine, or clean Codespace, running the AppImage threw:
  ```
  ModuleNotFoundError: No module named 'PyQt6'
  ```
- This violated the core semiconductor EDA contract of **zero-dependency, single-file turnkey binary portability**.

---

## 2. Invariant Laws Established

### Invariant 1: Mandatory Self-Contained Standalone CPython
The AppImage **must never** execute host `/usr/bin/python3`.
The packaging pipeline (`packaging/build_appimage.sh`) must embed an isolated CPython 3.12 standalone distribution into `${APPDIR}/opt/python/` with its own `bin/python3`, `lib/libpython3.12.so`, and `lib/python3.12/site-packages/`.

### Invariant 2: Complete Pre-Bundled GUI & Compute Stack
The bundled Python environment must embed all required wheels prior to AppImage compilation:
1. `PyQt6` & `PyQt6-Qt6` (complete Qt6 widgets, GUI, core)
2. `PyQt6-WebEngine` & `PyQt6-WebEngine-Qt6` (headless/canvas Chromium WebEngine for DigitalJS & ROHD)
3. `PyQt6-QScintilla` (source code editor syntax highlighting)
4. `gdstk` (GDSII / OASIS layout geometry engine)
5. `numpy` (numeric placement and floorplanning arrays)
6. `websockify` (noVNC canvas bridging)

### Invariant 3: Automated First-Boot Provisioning & SDK Injection
1. **Host Python SDK Injection**: `core/sdk_installer.py` automatically injects `silis.pth` and links `silis` into `~/.local/lib/python3.*/site-packages/` and symlinks `~/.local/bin/silis`.
2. **PDK Auto-Unpack**: `core/setup_wizard.py` checks `~/.silis/.initialized` and unpacks bundled Sky130 standard cell LEF/techlef/lib assets from `assets/pdk/sky130B` into `~/.silis/pdks/sky130B`.

---

## 3. Negative Penalty Assessment
- Any future pull request, script, or commit that removes embedded Python runtimes or reintroduces host Python package dependencies to the standalone AppImage is classified as a **HIGH NEGATIVE PENALTY REGRESSION**.
- Zero-tolerance policy: All release candidates must pass execution in a clean-room container with zero Python packages installed.
