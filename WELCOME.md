# Silis CS Edition — Cloud ASIC Physical Design Studio

Welcome to **Silis CS Edition**, the zero-install cloud EDA workbench for GitHub Codespaces and VS Code Web.  
Developed by **Jerome Antony Robin**. This repository follows a **BDFL Governance model**.  
Co-developed by **Eatheshwar**, **Venkat**, and **Adhil**.  
*Silis took 11 months from POC to pre-release.*

---

## Interactive CAD Canvases

Launch any visualizer directly inside your editor layout using VS Code Web command links:

| Visualizer | Engine | Capabilities | One-Click Launch |
| :--- | :--- | :--- | :--- |
| **Unified CAD Workbench** | HTML5 / Webview | Central cockpit embedding all 4 CAD tools in tabbed panels | [Launch CAD Dock](command:silis.openWorkbench) |
| **DigitalJS Schematic** | DigitalJS + ELK | Structural netlist explorer with automated orthogonal gate routing | [Open Schematic](command:silis.openSchematic) |
| **ROHD Waveforms** | Intel ROHD + WASM | Hardware-accelerated interactive VCD/FST digital waveform inspector | [Open Waveforms](command:silis.openWaveforms) |
| **2D Layout Peeker** | FastSiliconPeeker | Sub-pixel 2D DEF placement, macro halo, and routing visualizer | [Open 2D Layout](command:silis.openLayout) |
| **Floorplan & Power Studio** | Web FPS | Multi-domain IEEE 1801 (UPF 3.0) power fences, straps & macro snapping | [Open Web FPS](command:silis.openFloorplanner) |

*Tip*: You can also access the standalone CAD Web Hub at [http://localhost:8765/workbench](http://localhost:8765/workbench).

---

## Tri-Way Cross-Probing

Silis CS Edition features synchronized bidirectional navigation across design representations:
1. **Waveform -> Schematic**: Scrubbing the waveform cursor at timestamp `t` live-evaluates logic levels and annotates net values across schematic gates.
2. **Schematic -> Waveform**: Right-click any gate or wire and select **Show in waveform** to highlight and focus the signal in the waveform viewer.
3. **Schematic -> RTL Source**: Right-click any component and select **Go to source** to jump directly to its declaration in the left editor panel.

---

## One-Click EDA Execution

- **Compile & Run Simulation**: [Compile RTL & Run Simulation](command:silis.compileAndSim)
- **Lint Active RTL (Verilator)**: [Execute RTL Lint (Ctrl+L)](command:silis.lintRtl)
- **Gate-Level Synthesis (Yosys)**: [Execute Yosys Synthesis](command:silis.runSynthesis)
- **Tapeout PnR Flow (OpenROAD)**: [Execute Full Physical Design](command:silis.runAll)

---

## Bundled Reference Projects

### 1. `projects/demo_soc` (SkyWater 130nm Demonstration SoC)
- **Top Module**: `source/chip_top.sv`
- **Timing Constraints**: `source/chip_top.sdc`, `sta.tcl`
- **Floorplan & PnR Scripts**: `run_floorplan.tcl`, `run_pdn.tcl`, `run_place.tcl`, `run_route.tcl`
- **Layout DEF**: `results/temp.def`
- **Waveform**: `waveform.vcd`

### 2. `projects/FA` (Full Adder Standard Cell)
- **Top Module**: `rtl/full_adder.v`
- **Testbench**: `tb/tb_full_adder.v`
- **Waveform**: `waveform.vcd`

---

## Essential Keyboard Shortcuts

- `Ctrl + Shift + P`: Open Command Palette (type `Silis` for all commands)
- `Ctrl + L`: Run Verilator strict RTL linting on active editor file
- Top-right Editor Bar **Play button**: Compile and launch side-by-side simulation
- Top-right Editor Bar **Bug button**: Execute RTL static analysis
