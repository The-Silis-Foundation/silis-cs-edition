<div align="center">

<pre>
███████╗ ██╗ ██╗      ██╗ ███████╗
██╔════╝ ██║ ██║      ██║ ██╔════╝
███████╗ ██║ ██║      ██║ ███████╗
╚════██║ ██║ ██║      ██║ ╚════██║
███████║ ██║ ███████╗ ██║ ███████║
╚══════╝ ╚═╝ ╚══════╝ ╚═╝ ╚══════╝
</pre>

# SILIS — Codespaces Edition (CS)
### Multi-Framebuffer Cloud CAD Workbench for GitHub Codespaces & VS Code Web

<h4>
  <a href="#workbench-guide--control-center">Control Center</a>
  ·
  <a href="#visual-cad-surfaces">Visual Surfaces</a>
  ·
  <a href="#architectural-principles">Architecture</a>
  ·
  <a href="#port-and-service-matrix">Port Matrix</a>
  ·
  <a href="#quick-start">Quick Start</a>
  ·
  <a href="#third-party-attributions">Attributions</a>
</h4>

<p>
  <img alt="Platform" src="https://img.shields.io/badge/Platform-GitHub%20Codespaces-blue.svg?style=for-the-badge&logo=github&logoColor=white">
  <img alt="VS Code Web" src="https://img.shields.io/badge/Integration-VS%20Code%20Webviews-007ACC.svg?style=for-the-badge&logo=visualstudiocode&logoColor=white">
  <img alt="License" src="https://img.shields.io/badge/License-AGPL%203.0-green.svg?style=for-the-badge">
  <a href="https://github.com/The-Silis-Foundation/silis/releases/tag/v0.1.1-pre"><img alt="Desktop Release" src="https://img.shields.io/badge/Desktop%20Core-v0.1.1--pre-orange.svg?style=for-the-badge"></a>
</p>

<p align="center">
  Developed by <strong>Jerome Antony Robin</strong>. This repository follows a <strong>BDFL Governance model</strong>.<br>
  Co-developed by <strong>Eatheshwar</strong>, <strong>Venkat</strong>, and <strong>Adhil</strong>.<br>
  <em>Silis took 11 months from POC to pre-release.</em>
</p>

</div>

---

<!-- PROJECTS:START -->

## Workbench Guide & Control Center

> [!IMPORTANT]
> **How to Navigate Silis in VS Code Web / GitHub Codespaces**:
> 1. **Activity Bar (Left Sidebar)**: Click the **Silis CAD** icon (circuit-board icon on the leftmost bar) to access the interactive Control Center, create new projects with the authentic 6-stage wizard, and open visual CAD surfaces.
> 2. **Run Simulation (Play Button)**: Click the **Play button** on the top right editor title bar (or run `Silis: Compile & Run Simulation`) to compile design logic, open the DigitalJS schematic side-by-side on the right (`ViewColumn.Beside`), and load the waveform inspector in the bottom panel dock.
> 3. **Lint RTL (Debug Button / Ctrl+L)**: Click the **Bug button** on the editor title bar or press `Ctrl+L` to run strict Verilator syntax and design rule checks.
> 4. **Tri-Way Cross-Probing**:
>    - **Waveform -> Schematic**: Scrubbing the waveform cursor at timestamp `t` live-evaluates and annotates wire logic levels in the schematic.
>    - **Schematic -> Waveform**: Right-click any wire/gate and select *Show in waveform* to add and highlight the signal in the bottom waveform dock.
>    - **Schematic -> RTL Editor**: Right-click any component and select *Go to source* / *Go to definition* to navigate directly to the declaration in the left editor.

### Registered ASIC Projects

| Project | Process (PDK) | Top Module | Layout (DEF) | Waveforms | Source Files |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **FA** | `sky130_fd_sc_hd` | `full_adder` | Pending PnR | Available | 1 file(s) |
| **demo_soc** | `sky130` | `chip_top` | Available | Available | 2 file(s) |

### Key Commands (`Ctrl+Shift+P`)

- `Silis: Open Unified CAD Workbench` -- Open all 4 CAD tools tabbed in a single Webview
- `Silis: Open Project` -- Select active project, focus directory in Explorer, and open top RTL on left
- `Silis: Compile & Run Simulation` -- Compile design, display schematic split beside RTL, focus bottom waveform dock
- `Silis: Lint RTL (Ctrl+L)` -- Run Verilator strict RTL lint checks
- `Silis: Open 2D Layout Canvas` -- Open FastSiliconPeeker 2D DEF layout canvas
- `Silis: Open Floorplan & Power Studio` -- Open multi-domain UPF floorplanning canvas
- `Silis: Run Synthesis (Yosys)` -- Synthesize active project into gate-level netlist
- `Silis: Run Tapeout Flow (OpenROAD)` -- Execute floorplan, CTS, placement, and routing

<!-- PROJECTS:END -->

---

## Visual CAD Surfaces

Silis CS Edition delivers four interactive engineering visualizers directly inside VS Code Web editor tabs and via the standalone CAD Web Hub on Port `8765`:

| Subsystem | Primary Access (VS Code Web) | Standalone Browser Link | Technical Description |
| :--- | :--- | :--- | :--- |
| **Unified CAD Dock** | `Silis: Open Unified CAD Workbench` | [Open CAD Workbench](http://localhost:8765/workbench) | Central engineering cockpit embedding Schematics, Waveforms, 2D Layout, and Floorplanner in synchronized tabbed panels. |
| **Schematic Explorer** | Click `structural_*.json` or `Silis: Open DigitalJS Schematic` | [Open Schematics](http://localhost:8765/schematic) | Interactive DigitalJS netlist visualizer with ELK automatic orthogonal layout and live gate-level logic evaluation. |
| **Waveform Inspector** | Click `*.vcd` / `*.fst` or `Silis: Open ROHD Waveforms` | [Open Waveforms](http://localhost:8765/waveforms) | Hardware-accelerated Intel ROHD digital waveform viewer with signal transition search and synchronized marker cursors. |
| **2D Layout Peeker** | Click `*.def` or `Silis: Open 2D Layout Canvas` | [Open 2D Layout](http://localhost:8765/layout) | Sub-pixel hardware-accelerated FastSiliconPeeker canvas rendering macro placements, halos, pins, and metal routing layers. |
| **Floorplan & Power Studio** | `Silis: Open Floorplan & Power Studio` | [Open Web FPS](http://localhost:8765/floorplan) | Interactive IEEE 1801 (UPF 3.0) multi-voltage power domain editor, voltage fence synthesizers, and macro halo clearance verifier. |

---

## Architectural Principles

### 1. Zero-Install Cloud Workbench
- **VS Code Web Integration**: All CAD tools run natively in your browser using Webview panels, HTML5 Canvas, and WebAssembly. No desktop X11 forwarding or heavy VNC bandwidth is required for everyday RTL synthesis, simulation, and floorplanning.
- **Port 8765 Native Web Hub**: A lightweight Python ASGI/HTTP server (`tools/cs_server.py`) provides fast bidirectional REST endpoints, layout parsing, and cross-probing event routing.

### 2. Full Parity with Desktop Silis Core
- **Identical Physical Invariants**: All geometric quantization (0.005 µm manufacturing grid), site-row snapping, halo clearance rules, and UPF power intent emission strictly mirror the desktop Qt6 engine (`silis.core.sdk`).
- **Synchronized Dataflow**: Floorplan state files (`.silis_fp_session.json`), TCL synthesis scripts, and DEF layouts are 100% interchangeable between desktop AppImage installations and cloud Codespaces.

### 3. Embedded Pre-Compiled EDA Toolchain
Silis CS Edition bundles stripped, production-grade Linux x86-64 EDA binaries and libraries in the workspace:
- **Synthesis & Elaboration**: Yosys, Yosys-ABC, sv2v, GHDL
- **Physical Design & PnR**: OpenROAD (TritonMacroPlace, RePlAce, OpenDP, TritonCTS, FastRoute, TritonRoute)
- **Simulation & Verification**: Verilator, SymbiYosys (SBY), Magic VLSI, Netgen LVS
- **Process Design Kit**: SkyWater 130nm (`sky130_fd_sc_hd` standard cell library, tech LEF, power switch cells)

---

## Port and Service Matrix

| Port | Service | Protocol | Visibility | Purpose |
| :--- | :--- | :--- | :--- | :--- |
| `8765` | **Silis CAD Web Hub** | HTTP / WebSocket | Forwarded / Public | Serves Workbench, DigitalJS schematics, ROHD waveforms, FastSiliconPeeker layout, and REST APIs. |
| `6081` | *FastSiliconPeeker (X11 Fallback)* | HTTP (noVNC) | Local / Optional | Virtual framebuffer `:1` for standalone headless X11 layout viewer binaries. |
| `6082` | *Frontend Studio (X11 Fallback)* | HTTP (noVNC) | Local / Optional | Virtual framebuffer `:2` for standalone headless X11 waveform viewer binaries. |
| `6084` | *Silis FPS (X11 Fallback)* | HTTP (noVNC) | Local / Optional | Virtual framebuffer `:4` for standalone headless X11 Qt6 floorplanner instances. |

---

## Quick Start

### 1. Interactive Web Workflow
1. Open any Verilog/SystemVerilog file in `projects/demo_soc/source/` (e.g. `chip_top.sv`).
2. Click the **Play button** on the top right editor bar to compile and simulate.
3. Observe the structural schematic render side-by-side and waveforms dock in the lower panel.
4. Open the command palette (`Ctrl+Shift+P`) and run `Silis: Open Unified CAD Workbench` to inspect the full design cockpit.

### 2. Headless Python SDK Flow
Execute a complete physical design pass from the integrated terminal:

```bash
python3 -c "
import silis
pipe = silis.Pipeline('demo_soc', pdk='sky130')
pipe.floorplan(die_area=[0, 0, 1200, 1200], core_margin=20.0)
pipe.place()
"
```

The resulting layout will be written to `projects/demo_soc/results/temp.def` and can be immediately viewed by clicking the file in the Explorer.

---

## Third-Party Attributions

Silis orchestrates open-source EDA engines including OpenROAD, Yosys, SiliconCompiler, Magic, Netgen, Verilator, sv2v, GHDL, Intel ROHD Wave Viewer, DigitalJS, and ELKjs. Full licensing, source repository links, and developer attributions are documented in [ATTRIBUTION.md](ATTRIBUTION.md).
