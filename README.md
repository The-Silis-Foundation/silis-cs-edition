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
  <a href="#workbench-canvases">Visual Canvases</a>
  ·
  <a href="#architecture">Architecture</a>
  ·
  <a href="#quick-start">Quick Start</a>
  ·
  <a href="#port-allocation">Display Matrix</a>
  ·
  <a href="#attributions">Attributions</a>
</h4>

<p>
  <img alt="Platform" src="https://img.shields.io/badge/Platform-GitHub%20Codespaces-blue.svg?style=for-the-badge&logo=github&logoColor=white">
  <img alt="VS Code Web" src="https://img.shields.io/badge/Integration-VS%20Code%20Webviews-007ACC.svg?style=for-the-badge&logo=visualstudiocode&logoColor=white">
  <img alt="Architecture" src="https://img.shields.io/badge/Architecture-Isolated%20Framebuffers-green.svg?style=for-the-badge">
</p>

</div>

---

## Workbench Canvases (Dock as VS Code Editor Tabs)

In GitHub Codespaces, visualizer surfaces render to dedicated virtual framebuffers and dock directly inside VS Code Web editor tabs as split-pane webviews using the built-in Simple Browser:

| Subsystem | Port | One-Click Dock Link | Description |
| :--- | :--- | :--- | :--- |
| **Silis Floorplan Studio** | `6084` | [Open Floorplan Studio](http://localhost:6084/vnc.html?autoconnect=true&resize=remote) | Always-open interactive floorplanner, halo clearance inspector, and UPF power domain studio. |
| **FastSiliconPeeker (2D Layout)** | `6081` | [Open 2D Layout Canvas](http://localhost:6081/vnc.html?autoconnect=true&resize=remote) | Auto-docks as soon as `results/temp.def` is synthesized. Sub-pixel hardware-accelerated DEF placement. |
| **Frontend Studio & Waveforms** | `6082` | [Open Waveform Viewer](http://localhost:6082/vnc.html?autoconnect=true&resize=remote) | File explorer, Monaco RTL editor with live hover cards, C++ schematic viewer, and Intel ROHD waveform drawer. |

---

## Architectural Principles

### 1. Division of Labor
- **VS Code Web**: Serves as the primary code editor (Verilog, SystemVerilog, VHDL, C++ HLS), integrated terminal, git workspace, and Python SDK script runner.
- **Dedicated Framebuffer Isolation**: Never run overlapping windows in an uncontrolled desktop manager. Every graphical surface runs on its own isolated `Xvfb` virtual display server paired with an independent `x11vnc` daemon and `websockify` WebSocket bridge.

### 2. Auto-Docking on Layout Synthesis
When a physical design pipeline generates or updates `results/temp.def`, a background watcher automatically loads the new placement into `FastSiliconPeeker` on Display `:1` and invokes VS Code Web's Simple Browser to present the 2D layout side-by-side with your code.

### 3. Native Pre-Compiled Binary Runtime
Silis CS Edition includes pre-compiled, stripped production EDA binaries (`openroad`, `yosys`, `yosys-abc`, `magic`, `sv2v`, `verilator`) and native C++ shared library modules (`silis_layer_viewer.so`, `schematic_engine.so`, `fast_layout_viewer.so`) directly in the workspace filesystem.

---

## Display & Port Allocation Matrix

| Subsystem | Python SDK Trigger | Display | Internal VNC | noVNC Web | Auto-Dock Trigger |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **FastSiliconPeeker** | `silis.view_layout(def_path)` | `:1` | `5901` | `6081` | Automatic upon `temp.def` write |
| **Frontend Studio** | `silis.view_waveforms(vcd_path)` | `:2` | `5902` | `6082` | On-demand or task trigger |
| **Silis FPS** | `silis.open_floorplanner(proj)` | `:4` | `5904` | `6084` | Always running |

---

## Quick Start (Codespaces Terminal)

Launch a complete end-to-end ASIC flow directly from the integrated VS Code terminal:

```bash
# 1. Run headless Python SDK pipeline:
python3 -c "
import silis
pipe = silis.Pipeline('demo_soc', pdk='sky130')
pipe.floorplan(die_area=[0, 0, 1200, 1200], core_margin=20.0)
pipe.place()
"

# FastSiliconPeeker will automatically dock the resulting layout on Port 6081!
```

---

## Third-Party Attributions

Silis orchestrates open-source EDA engines including OpenROAD, Yosys, SiliconCompiler, Magic, Netgen, Verilator, sv2v, GHDL, and Intel ROHD Wave Viewer. Full licensing, source repository links, and developer attributions are documented in [ATTRIBUTION.md](ATTRIBUTION.md).
