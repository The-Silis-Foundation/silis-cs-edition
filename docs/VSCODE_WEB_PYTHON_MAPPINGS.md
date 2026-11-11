# Silis CS Edition: VS Code Web UI Button to Python Function Mappings

This document provides a single-source-of-truth reference mapping every interactive button, menu item, and command in VS Code Web to its exact underlying Python function and execution pipeline.

---

## 1. Top Bar & Flow Execution Controls

| UI Control / Button | VS Code Command ID | Underlying Python / Flow Function | Source Implementation | Execution Mode | Emitted Artifacts |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Play (Run Simulation)** | `silis.runSimulation` | `flow.run_sim(project_dir)` | `tools/flow.py:39` | Headless CLI (Verilator Compiler & Testbench Runner) | `waveform.vcd`, `build/structural_synth.json` |
| **Debug (Lint RTL)** | `silis.runLint` | `SilisCadHandler._run_lint(project_dir)` | `core/server/cs_server.py:539` | Headless REST API (`/api/lint`) -> Verilator `--lint-only` | Diagnostics in VS Code Output Channel |
| **Run Synthesis (Yosys)** | `silis.runSynthesis` | `flow.run_synth(project_dir)` | `tools/flow.py:176` | Headless CLI (Yosys `hierarchy -auto-top; prep; write_json; write_verilog`) | `build/structural_synth.json`, `build/synth.v` |
| **Run Tapeout Flow** | `silis.runAll` | `flow.run_tapeout(project_dir)` | `tools/flow.py:206` | Headless CLI (Yosys synth + OpenROAD PnR) | `results/temp.def`, `results/final_routed.def` |

---

## 2. Interactive CAD Viewer Docking

| UI Control / Button | VS Code Command ID | Web / Native Endpoint | Underlying Python / C++ Engine | Description |
| :--- | :--- | :--- | :--- | :--- |
| **Schematic (DigitalJS)** | `silis.openSchematic` | `http://127.0.0.1:8765/schematic/digitaljs.html?project=...` | `cs_server.py:_get_or_generate_netlist` + Yosys Web Bridge | Generates/fetches `structural_synth.json` via Yosys, renders interactive gate-level schematic beside active editor. |
| **Waveforms (ROHD)** | `silis.openWaveforms` | `http://127.0.0.1:8765/waveforms/index.html?project=...` | `cs_server.py:_locate_vcd_path` + ROHD WebAssembly Engine | Locates `waveform.vcd`, loads hardware trace in bottom panel dock with cursor-synchronized cross-probing. |
| **Floorplan Studio (FPS)** | `silis.openFloorplanner` | `http://127.0.0.1:8765/floorplanner/index.html?project=...` | `cs_server.py:/api/floorplan/*` + `core/backendflow/floorplanner/web/index.html` | Interactive HTML5 Canvas floorplan & power grid studio: die/core margins, macro placement, UPF power domains, DRC checks, and OpenROAD TCL export. |
| **2D Layout (Peeker)** | `silis.openLayout` | Display `:1` (noVNC port 6081) / `results/temp.def` | `backendflow.siliconpeeker.peeker.FastSiliconPeeker` | High-performance C++ OpenGL layout viewer (`fast_layout_viewer.so`) with sub-pixel culling and true LEF-derived cell scaling. |
| **OpenROAD Native GUI** | `silis.openOpenRoadGui` | `tools/flow.py --view-openroad --project ...` | `flow.run_openroad_gui` / `silis.core.sdk.viewers.view_openroad_gui` | Dynamically locates project DEF, Sky130 LEF/TLEF, macro LEFs, synthesizes loader TCL script, and launches native `openroad -gui`. |

---

## 3. Project Management & Intake

| UI Control / Button | Action / Handler | Underlying Python Engine | Source Implementation | Description |
| :--- | :--- | :--- | :--- | :--- |
| **+ New Silis Project** | `silis.newProject` | `cs_server.py:/api/projects/create` | `cs_server.py:294` (100% parity with `core/projectwizard/codes.py`) | 5-stage project wizard supporting system-wide file picker for RTL sources, SDC constraints, testbenches, IEEE 1801 UPF power intent, and I/O pin config. |
| **Select Design (Table / Sidebar)** | `silis.openProject` | `cs_server.py:/api/projects` + `extension.js:silis.openProject` | `extension/extension.js:423` | Scans registered projects, switches active context on server, reveals directory, and immediately opens the design's top RTL file in `ViewColumn.One`. |
| **Refresh Dashboard** | `silis.refreshReadme` | `tools/gen_readme.py` | `tools/gen_readme.py` | Updates workspace project metadata, gate count metrics, and CAD engine status badges. |

---

## 4. Cross-Probing Dataflow (Schematic <-> Waveform <-> Editor)

```
[DigitalJS Schematic Webview]
         │
         │ addToWaveform (leafName)
         ▼
[extension.js Host Bridge] ─── postMessage({ addSignal }) ───► [ROHD Waveform Webview]
         │
         │ gotoSource (file, line)
         ▼
[VS Code Text Editor] (opens file at exact line in ViewColumn.One)

[ROHD Waveform Webview]
         │
         │ cursorMoved (timePs, signals)
         ▼
[cs_server.py /api/waveform/values] ── (FastVcdTracker)
         │
         ▼
[DigitalJS Schematic Webview] ── (setValues: updates logic wire probe bubbles)
```
