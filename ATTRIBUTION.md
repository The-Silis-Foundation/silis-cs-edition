# Open Source Third-Party Attributions & Software Licenses

The **Silis Codespaces Edition** integrates and orchestrates open-source EDA engines, compilers, libraries, and user interface modules. We gratefully acknowledge the authors, maintainers, and developer communities behind the following open-source projects:

---

## 1. Core Physical Design & Synthesis Engines

### OpenROAD
- **Project**: The OpenROAD Project (Foundations of Open Autonomous Design)
- **Repository**: [https://github.com/The-OpenROAD-Project/OpenROAD](https://github.com/The-OpenROAD-Project/OpenROAD)
- **License**: BSD 3-Clause License
- **Role in Silis**: Floorplanning, TritonMacroPlace, global placement (RePlAce), detailed placement (OpenDP), clock tree synthesis (TritonCTS), global routing (FastRoute), detailed routing (TritonRoute), antenna checking, and OpenDB geometric layout database.

### Yosys Open SYnthesis Suite
- **Project**: YosysHQ / Claire Wolf
- **Repository**: [https://github.com/YosysHQ/yosys](https://github.com/YosysHQ/yosys)
- **License**: ISC License
- **Role in Silis**: RTL elaboration, behavioral optimization, arithmetic lowering, ABC standard cell technology mapping, and structural gate-level netlist generation.

### Magic VLSI & Netgen
- **Project**: R. Timothy Edwards & Open Circuit Design
- **Repositories**:
  - Magic: [https://github.com/RTimothyEdwards/magic](https://github.com/RTimothyEdwards/magic) (Open Source BSD-style)
  - Netgen: [https://github.com/RTimothyEdwards/netgen](https://github.com/RTimothyEdwards/netgen) (GNU GPLv1+)
- **Role in Silis**: Physical DRC verification, extraction, and Layout Versus Schematic (LVS) comparison.

---

## 2. SiliconCompiler Framework

### SiliconCompiler
- **Project**: SiliconCompiler Authors & Contributors
- **Repository**: [https://github.com/siliconcompiler/siliconcompiler](https://github.com/siliconcompiler/siliconcompiler)
- **License**: Apache License 2.0
- **Role in Silis**: Vendored co-processor framework, bidirectional schema bridge (`silis.from_sc` / `silis.to_sc`), manifest fileset harvesting, and target PDK abstraction.

---

## 3. Polyglot Frontends, Simulation & Verification

### Verilator
- **Project**: Wilson Snyder, Duane McCracken, and Paul Wasson
- **Repository**: [https://github.com/verilator/verilator](https://github.com/verilator/verilator)
- **License**: GNU LGPL v3.0 / Artistic License 2.0
- **Role in Silis**: High-performance cycle-accurate C++ simulation, multithreaded testbench execution, and coverage analysis.

### sv2v (SystemVerilog to Verilog)
- **Project**: Zachary Snow
- **Repository**: [https://github.com/zachjs/sv2v](https://github.com/zachjs/sv2v)
- **License**: BSD 3-Clause License
- **Role in Silis**: Native translation of synthesizable SystemVerilog constructs into structural Verilog-2005.

### GHDL
- **Project**: Tristan Gingold & GHDL Contributors
- **Repository**: [https://github.com/ghdl/ghdl](https://github.com/ghdl/ghdl)
- **License**: GNU GPL v2.0+
- **Role in Silis**: VHDL-1993/2008 frontend lowering and synthesis integration.

### SymbiYosys (SBY)
- **Project**: YosysHQ
- **Repository**: [https://github.com/YosysHQ/sby](https://github.com/YosysHQ/sby)
- **License**: ISC License
- **Role in Silis**: Bounded model checking (BMC) and formal property verification backend.

---

## 4. Frontend & User Interface Components

### Intel ROHD Wave Viewer
- **Project**: Intel Corporation
- **Repository**: [https://github.com/intel/rohd-wave-viewer](https://github.com/intel/rohd-wave-viewer)
- **License**: BSD 3-Clause License
- **Role in Silis**: Hardware-accelerated interactive digital waveform viewer (`SignalPeeker`).

### Monaco Editor
- **Project**: Microsoft Corporation
- **Repository**: [https://github.com/microsoft/monaco-editor](https://github.com/microsoft/monaco-editor)
- **License**: MIT License
- **Role in Silis**: In-browser code editing, syntax highlighting, and live simulation hover cards.

### DigitalJS
- **Project**: Marek Materzok & Tilburg University
- **Repository**: [https://github.com/tilk/digitaljs](https://github.com/tilk/digitaljs)
- **License**: MIT License
- **Role in Silis**: Interactive schematic layout and logic gate graph rendering.

### noVNC & websockify
- **Projects**:
  - noVNC: [https://github.com/novnc/noVNC](https://github.com/novnc/noVNC) (MPL 2.0)
  - websockify: [https://github.com/novnc/websockify](https://github.com/novnc/websockify) (LGPL 3.0)
- **Role in Silis**: In-browser HTML5 VNC client and WebSocket translation proxy for isolated virtual framebuffers.

---

## 5. Technology Libraries & PDKs

### SkyWater 130nm PDK
- **Collaboration**: Google & SkyWater Technology Foundry
- **Repository**: [https://github.com/google/skywater-pdk](https://github.com/google/skywater-pdk)
- **License**: Apache License 2.0
- **Role in Silis**: Reference open-source 130nm high-density standard cell library (`sky130_fd_sc_hd`).
