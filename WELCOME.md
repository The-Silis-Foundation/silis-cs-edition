# Silis CS Edition — Cloud ASIC Physical Design Studio

Welcome to **Silis CS Edition**, the zero-install cloud EDA workbench for GitHub Codespaces and VS Code Web.

---

## Interactive EDA Viewers

| Visualizer | Engine | Description | Launch |
| :--- | :--- | :--- | :--- |
| **Unified CAD Dock** | Webview | All 4 CAD tools tabbed side-by-side in VS Code | [Launch CAD Dock](command:silis.openWorkbench) |
| **Schematic Viewer** | DigitalJS + ELK | Gate & RTL structural netlist schematic explorer | [Open Schematic](command:silis.openSchematic) |
| **Waveform Viewer** | Intel ROHD + WASM | Fast interactive VCD/FST waveform viewer | [Open Waveforms](command:silis.openWaveforms) |
| **2D Layout Canvas** | SiliconPeeker | Physical DEF macro placement and routing inspector | [Open 2D Layout](command:silis.openLayout) |
| **Floorplan Studio** | Silis FPS | Multi-voltage fences, power straps & halo editor | [Open FPS](command:silis.openFloorplanner) |

---

## One-Click ASIC Flow

- **Run RTL Synthesis**: [Execute Yosys Synthesis](command:silis.runSynthesis)
- **Run Full PnR & Tapeout**: [Execute OpenROAD Flow](command:silis.runAll)

---

## Included Demo Project (`projects/demo_soc`)
- **Top Entity**: `source/chip_top.sv`
- **Floorplan & Timing**: `source/chip_top.sdc`, `sta.tcl`
- **Simulation Waveform**: `waveform.vcd` (click to view directly in editor)
