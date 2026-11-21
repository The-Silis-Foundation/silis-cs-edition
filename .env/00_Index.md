# Silis EDA Architecture & Master Knowledge Index

Welcome to the Silis EDA Software Architecture Vault and Second Brain.

## Releases & Deliverables
- [[releases/Release_Notes_v0.1.1-pre]] — Silis v0.1.1-pre Official Release Notes, Download Matrix, First-Boot Guide, and Verification Checksums

## Interactive Codebase Knowledge Graph
- [[codebase/00_Codebase_Graph]] — Comprehensive interactive architecture graph of entrypoints, SDK, backend flow, floorplanning, viewers, and packaging

## Core Philosophy & Mental Models
- [[philosophy/Physical_Design_as_Compilation]] — Hardware Compilation mental model (RTL/UPF $\to$ AST $\to$ DEF/LEF IR $\to$ Bytecode)
- [[philosophy/Zero_Catch_Law]] — Fail loudly, zero catch blocks, preventing silent silicon defects
- [[philosophy/GUI_as_a_Projection]] — GUI as a pure, stateless visual lens over an authoritative Python SDK

## Multi-Voltage & Power Intent Knowledge
- [[power/Why_Power_Domains_Exist]] — Transistor physics, floating net crowbar hazards, isolation, and level shifters
- [[power/Architectural_vs_Physical_Switches]] — Architectural domains vs physical power switch standard cell rows
- [[power/Power_Grid_Topology]] — Dual-layer orthogonal meshes, peripheral pad trunks, and static IR drop limits

## Toolchain Personality & Engine Quirks
- [[toolchain/OpenROAD_Personality]] — Internal C++ quirks, LEF ordering, `importUPF` parser, and detailed placement
- [[toolchain/Yosys_Quirks]] — Latch inference hazards, blackbox macro ports, and hierarchy preservation for UPF

## Silicon Realities & Semiconductor Physics
- [[silicon/SkyWater130_Quirks]] — Local interconnect (`li1`), standard cell site conventions, and routing layers
- [[silicon/What_Kills_a_Chip]] — Real-world failure modes: IR drop, electromigration, plasma antenna oxide rupture

## Architecture Decision Records (ADRs) & Engineering Lessons
- [[decisions/ADR_015_Verilator_CCache_Resilience_And_Defensive_PDK_Resolution]] — **CLEAN-ROOM RESILIENCE**: Verilator ccache graceful fallback and defensive multi-candidate PDK asset resolution
- [[decisions/ADR_014_CS_Edition_Verification_Parity_And_Clean_Room_AppImage]] — **FULL PARITY & CLEAN-ROOM**: 6-stage wizard parity with testbenches, dynamic multi-PDK IP catalog, rectilinear blockages, and zero-host-leak AppImage
- [[decisions/ADR_013_Dual_Mode_Macro_Stubs_And_Editor_Engine_Resilience]] — **ARCHITECTURAL RESILIENCE**: Dual-mode behavioral macro stubs and zero-failure editor engine parity
- [[decisions/ADR_012_Zero_PDK_Truncation_Law]] — **TAPE-OUT CRITICAL**: Zero PDK Truncation Law, SSA Drop-In Completeness, and Clean Runtime Fallback
- [[decisions/AppImage_Host_Dependency_Defect_Penalty]] — **HIGH PENALTY**: Root cause, remediation, and zero-host-dependency invariant for standalone AppImage
- [[decisions/Intent_and_GUI_Audit]] — Intent-level architectural verification and headless Qt6 GUI test results
- [[decisions/Detailed_Placement_Pad_Alignment_Blocker]] — Root cause and resolution strategy for `DPL-0006` site alignment failure during placement
- [[decisions/Dynamic_OpenDB_Site_Row_Snapping]] — Dynamic OpenROAD OpenDB grid and site row snapping eliminating hardcoded coordinates
- [[decisions/Routing_Congestion_And_TritonRoute_Convergence]] — Resolving `GRT-0116` global routing exit code via `-allow_congestion` SDK-GUI parity
- [[decisions/User_Configurable_Domain_Row_Height]] — User control for site row heights and physical switch insertion across SDK & GUI

- [[decisions/Opt_In_Physical_Power_Switches]] — Opt-in physical switch policy preventing `PDN-0240` & segfaults

- [[decisions/Authoritative_SDK_State]] — SDK as single source of truth for 100% bug reproducibility
- [[decisions/Token_Lean_EDA_Investigation]] — Deterministic tools (`def_inspector.py`, `silis_ast.py`) resolving context exhaustion
- [[decisions/Obsidian_Snap_Sandboxing]] — Resolving Snap filesystem isolation and vault discovery



## Active Roadmaps & Plans
- [[roadmap/Upcoming_Features]] — Commercial-grade feature roadmap (Differentiable SGD Placer, PrimeTime Flylines, Voltus IR Heatmap, ICC2 DRC Radar)
- [[08_OpenROAD_Native_Layer_Viewer_Plan]] — In-place left sidebar swapping & OpenROAD native layer viewer architecture
- [[09_Codespaces_and_Native_Layout_Viewers_Architecture]] — Codespaces CS Edition vs Native Qt6 Viewers, Webview IPC, and VNC privilege model
- [[10_Portless_VNC_IPC_Tunnel_Option_B]] — Portless VNC Webview IPC Tunnel (Option B) architecture, zero daemon ports, zero cookie drop
- [[decisions/ADR_016_Portless_VNC_Webview_IPC_Tunnel]] — **ZERO PRIVILEGE DEFECT**: Portless RFB Webview IPC Tunnel bypassing Codespaces browser permissions
- [[scratchpad/Active_Session]] — Live scratchpad, real-time working hypotheses, and active milestone tracking
- [[roadmap/Beast_Processor_Intent]] — Architecture intent for the dense 6-RAM, dual-domain processor beast
- [[scratchpad/beast_processor_floorplan_plan]] — Coordinates, memory bank allocations, and site snapping


## Agent Skills & Automation Tooling
- [[skills/Silis_EDA_Architect_Skill]] — ASIC physical design EDA standards, PDK rules, and toolchain integrity
- [[skills/UI_State_Verifier_Skill]] — Headless Qt6 UI test automation, dialog suppression, and widget state validation
- [[skills/Obsidian_Second_Brain_Skill]] — Autonomous second brain management, CLI workflows, and scratchpad protocols

## Silis Platform & Ecosystem Documentation
- [[docs/SILIS_PYTHON_SDK_MANUAL]] — Complete programmatic ASIC design & floorplanning SDK manual
- [[docs/OPENROAD_FLOW_REFERENCE]] — Deep reference for OpenROAD physical design flow stages
- [[docs/Capability]] — Multi-voltage, macro placement, and DRC signoff capability matrix
- [[docs/book/01_architecture_overview]] — Platform architecture triad: GUI, CLI, and Python SDK
- [[docs/book/02_python_sdk_reference]] — Python SDK class, pipeline method, and SDCBuilder reference
- [[docs/book/03_cli_reference]] — Command-line interface manual (`silis-flow` / `silis-project`)
- [[docs/book/04_siliconcompiler_compatibility]] — SiliconCompiler interoperability & schema translation
- [[docs/book/05_advanced_floorplanning]] — Macro placers, auto-shaper, and Power & PDN GUI/SDK controls
- [[docs/book/06_polyglot_and_formal]] — Polyglot RTL (VHDL, SystemVerilog, Bambu HLS) and SymbiYosys
- [[docs/book/07_reference_examples]] — Runnable quad-core SRAM SoC and GEMM accelerator scripts
- [[docs/memory/CODEBASE_KNOWLEDGE]] — Deep codebase method and symbol execution map
- [[docs/memory/EDA_DOWNSTREAM_PLAYBOOK]] — OpenROAD, Yosys, and Magic failure diagnostics
- [[docs/memory/architecture_map]] — High-level layout viewer and Qt6 UI architecture map

