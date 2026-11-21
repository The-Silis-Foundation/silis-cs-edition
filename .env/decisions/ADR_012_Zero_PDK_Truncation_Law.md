---
title: "ADR-012: Zero PDK Truncation Law & SSA Configuration Completeness"
date: "2026-11-15"
status: "ACCEPTED_AND_ENFORCED"
tags:
  - pdk
  - ssa
  - packaging
  - appimage
  - tapeout
  - architecture
  - semiconductor
---

# ADR-012: Zero PDK Truncation Law & SSA Configuration Completeness

## 1. Context & Incident Classification
**Classification**: CRITICAL SEMICONDUCTOR INTEGRITY BREACH & PACKAGING ARBITRATION FAILURE  
**Timestamp**: 2026-11-15T06:20:12+05:30  
**Affected Scope**: Silis Release Binary (`dist/Silis-x86_64.AppImage`), First-Boot Provisioning (`core/setup_wizard.py`), and PDK Catalog Registry (`core/pdkmanagers/pdk/manager.py`)

### Problem Statement
In previous iterations, the packaging pipeline was modified to stage an arbitrary "lean subset" of the SkyWater 130nm PDK (single TT corner, merged standard cell LEF, four macro LEFs) to optimize packaging size.
This defect caused catastrophic failures in the platform's core identity:
1. **Multi-Corner Signoff Disabled**: All slow (`ss_*`) and fast (`ff_*`) PVT corners, along with temperature sweep libraries (-40°C to 100°C), were pruned. STA setup and hold timing signoff across PVT corners became impossible.
2. **Multi-Voltage UPF Level Shifting Broken**: The high-voltage standard cell library (`sky130_fd_sc_hvl`) containing level shifters and isolation clamp cells for 3.3V/5.0V was excluded, violating IEEE 1801 multi-voltage physical domain requirements.
3. **Missing Primitives & DRC/LVS Models**: Primitive devices (`sky130_fd_pr`), full OpenRAM SRAM memory macros across all sizes, and IO cell variants were truncated.
4. **SSA Configuration Warning**: First-boot provisioning failed to stage `~/.silis/ssa/` and `pdk_aliases.json`, triggering `[SSA] No external JSON found. Using built-in defaults.`
5. **Unsuppressed Chromium GBM Warning**: Missing Chromium flag suppression caused `GBM is not supported with the current configuration. Fallback to Vulkan rendering in Chromium.` on host systems running standard Linux desktop configurations without direct GBM buffers.

---

## 2. Invariant Laws Established

### Invariant 1: The Zero PDK Truncation Law
> [!CAUTION]
> **Silis is a Complete, Authoritative ASIC Design EDA Studio**:
> The packaging pipeline and first-boot setup orchestrator must **NEVER** prune, filter, or ship "parts" of a PDK.
> Every supported PDK family must ship with 100% of its silicon assets:
> - All standard cell variants (`hd`, `hvl`, `hs`, `ms`, `ls`, `lp`, `hdll`)
> - All PVT corners (SS, FF, TT across all temperature and voltage combinations)
> - All primitive device models (`sky130_fd_pr` spice, mag, cdl)
> - All OpenRAM SRAM memory hard macros (`lef`, `lib`, `verilog`, `gds`, `spice`)
> - All IO pad ring cells (`sky130_fd_io` lef, lib, gds)
> - All foundry physical design decks (`openlane`, `klayout`, `magic`, `netgen`, `ngspice`, `qflow`, `xcircuit`, `xschem`)
> - Full multi-technology PDKs (`sky130A`, `sky130B`, `gf180mcu`)

### Invariant 2: Complete SSA Drop-In Provisioning
The packaging pipeline must bundle all SSA family configuration drop-in files (`sky130a/config.json`, `gf180mcu/config.json`, `asap7/config.json`, and `pdk_aliases.json`) in `${APPDIR}/assets/ssa/`.
The first-boot setup wizard must provision them automatically to `~/.silis/ssa/`, and `SSAForge` must resolve aliases deterministically across all environment paths.

### Invariant 3: Clean Headless & Desktop Display Fallback
1. `packaging/AppRun` must set `QTWEBENGINE_CHROMIUM_FLAGS="--disable-logging --log-level=3 --disable-gpu-compositing"` when running on desktop configurations without direct GBM hardware buffers.
2. `core/qt_setup.py` must only bind active, reachable X11 display sockets and prevent fake display binding during CLI flow execution.

---

## 3. Remediation Verification Matrix
- `Silis.AppDir/assets/pdk/sky130A`: Full 1.1 GB uncompressed PDK.
- `Silis.AppDir/assets/pdk/sky130B`: Full 1.1 GB uncompressed PDK.
- `Silis.AppDir/assets/pdk/gf180mcu`: Full 21 MB uncompressed PDK.
- `Silis.AppDir/assets/ssa`: Full drop-in configurations for `asap7`, `gf180mcu`, and `sky130a`.
- Clean room execution: AppImage executes with zero warnings on clean host systems.
