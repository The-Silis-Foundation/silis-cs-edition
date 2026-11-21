---
name: silis-eda-architect
description: Mission-critical ASIC physical design EDA standards, strict IEEE semiconductor standards, zero-defect data verification, OpenROAD downstream handoff integrity, 100% GUI-SDK architectural parity, strongly object-oriented Python SDK design, physical design engineering rules (DRC/LVS/CTS/STA/PDN/Antenna), pure native Qt6 conventions, and token-lean execution.
version: 5.0.0
---

# Silis ASIC EDA Master Architectural Specification & Tapeout Integrity Protocol

This document defines the production engineering standards, semiconductor design rules, data-integrity contracts, Qt6 software patterns, Python SDK architecture, and execution protocol for developing **Silis**: a first-of-its-kind, real-time ASIC physical design and multi-domain power studio EDA suite.

---

## 1. Tapeout-Critical Design Principle: Downstream Data Integrity

> [!CAUTION]
> **Silis is the Authoritative Physical Design Orchestrator**:
> Downstream engines—including **OpenROAD**, **Yosys**, **KLayout DRC/LVS**, and foundry sign-off decks (Calibre, Pegasus)—execute Silis instructions verbatim.
> A corrupted coordinate, an overlapping macro boundary, an illegal halo, an unaligned power grid, or a truncated UPF isolation net will propagate directly through global placement, CTS, routing, and tapeout.
> A mask defect or silicon fabrication failure at 130nm, 28nm, or FinFET nodes causes direct multi-million-dollar respins, wafer loss, and irreversible project cancellation.
> **Every byte of TCL, DEF, UPF, and SDC emitted by Silis must pass strict mathematical and physical pre-flight verification.**

---

## 2. Deep Physical Design (PD) & EDA Engineering Rules

Silis developers must adhere to real-world silicon tapeout requirements across all physical design phases:

### 2.1 Floorplanning & Die-Core Architecture
- **Core Utilization Constraints**:
  - Target standard-cell core utilization must remain between **40% and 70%** ($\rho \in [0.40, 0.70]$). Above 70%, route congestion spikes exponentially, causing OpenROAD detailed routing DRC explosions.
  - Channel width between adjacent macros:
    $$W_{\text{channel}} \ge 2 \times \text{Halo} + W_{\text{routing\_tracks}}$$
    Minimum channel spacing must accommodate at least 15–20 routing tracks on intermediate metal layers (e.g. `met2`/`met3`).
- **Macro Placement Symmetry & Pin Orientation**:
  - Macros with concentrated pin terminals must face towards the core cavity, never directed outwards against the die perimeter or edge pads.
  - Corner macro placement must preserve dedicated L-shaped power mesh feed channels.

### 2.2 Multi-Voltage Power Intent (IEEE 1801 UPF 3.0)
- **Power Domain Isolation & Level Shifters**:
  - When domain $PD_A$ power gates ($V_{A} \to 0\text{V}$) while $PD_{\text{TOP}}$ remains active ($V_{\text{TOP}} = 1.8\text{V}$), every boundary net must clamp to a known legal state via isolation cells:
    ```tcl
    set_isolation iso_pda -domain PD_A -clamp_value 0 -applies_to outputs
    ```
  - Level shifters must resolve both stepping up ($0.95\text{V} \to 1.8\text{V}$) and stepping down ($1.8\text{V} \to 0.95\text{V}$).
  - Gated power domains must specify standard cell row tapcells with distinct supply/ground rails (`sky130_fd_sc_hd__tapvpwrvgnd_1`).

### 2.3 PDN (Power Distribution Network) & IR Drop Physics
- **Dual-Layer Orthogonal Grid**:
  - Horizontal straps on Metal $M_i$ (e.g. `met5`), vertical trunks on Metal $M_{i-1}$ (e.g. `met4`).
  - Trunk width and pitch must satisfy static electromigration (EM) limits:
    $$J_{\text{avg}} = \frac{I_{\text{rms}}}{W_{\text{strap}} \times T_{\text{metal}}} \le J_{\text{max,foundry}}$$
- **PSM (Power Source Model) & Static IR Drop**:
  - Maximum allowable static IR drop on the core mesh is **$\le 5\%$ of $V_{\text{nominal}}$** (e.g. $\le 90\text{mV}$ on a $1.8\text{V}$ rail, $\le 47.5\text{mV}$ on a $0.95\text{V}$ rail).
  - VDD/VSS supply pad placement around the periphery must balance current distribution to prevent localized starvation hotspots.

### 2.4 Clock Tree Synthesis (CTS) Pre-Conditions
- **Symmetrical Macro Tree Constraints**:
  - Macro register clock pins must have legal routing corridors with zero hard blockage obstructions.
  - Silis clock pin placement must minimize clock skew variance across synchronized memory banks.
- **Clock Tree Buffer Exclusion**:
  - Clock buffers and inverters must never be placed inside macro keepout regions or non-clock voltage island areas.

### 2.5 Detailed Routing & Antenna DRC Prevention
- **Antenna Ratio Invariant**:
  - Metal-to-gate area ratio during layer buildup must not exceed the foundry antenna threshold:
    $$\text{Ratio}_{\text{antenna}} = \frac{\text{Area}(\text{Interconnect Metal})}{\text{Area}(\text{Gate Oxide})} \le R_{\text{max}}$$
  - Silis pad and pin assigners must provide diode insertion sites adjacent to long wire runs to prevent thin oxide breakdown during plasma etching.

---

## 3. GUI-SDK Dual Architecture: Zero Dataflow Deviance

### 3.1 The Parity Law (GUI is a Thin Presentation Shell over SDK)
- **Zero GUI-SDK Deviance**: The GUI and the Python SDK must never implement divergent physical design algorithms, calculations, or file exporters.
- **Single Source of Truth**:
  Every interactive canvas action (moving a macro, auto-aligning pads, synthesizing a power mesh, expanding die bounds, running DRC verification) must invoke the **exact same underlying SDK class method** that a Python script or headless batch worker calls.
- **Architectural Flow**:
  $$\text{User Interaction / Qt Event} \longrightarrow \text{Silis SDK Object Method} \longrightarrow \text{State & Physics Mutation} \longrightarrow \text{Canvas View Update}$$
  $$\text{Python CLI / Automation Script} \longrightarrow \text{Silis SDK Object Method} \longrightarrow \text{State & Physics Mutation} \longrightarrow \text{Disk Emission}$$
  *Rule*: The GUI must never contain standalone physical calculations that do not exist in the SDK.

### 3.2 Strongly Object-Oriented SDK Architecture
All ASIC entities must be represented by strongly typed, encapsulated object models residing in `silis.core.sdk`:

```python
# Canonical Silis SDK Hierarchy
class SilisProject:
    """Root design context, PDK configuration, netlist hash, and technology libraries."""
    def get_floorplan(self) -> "Floorplan": ...
    def get_power_network(self) -> "PowerNetwork": ...
    def get_timing_graph(self) -> "TimingGraph": ...

class Floorplan:
    """Die boundary, core rectangle, manufacturing grid quantization, utilization."""
    def set_die_dimensions(self, width: float, height: float, margins: Margins) -> None: ...
    def get_macros(self) -> List["MacroInstance"]: ...
    def add_macro(self, master: str, inst_name: str, x: float, y: float, orientation: Orientation) -> "MacroInstance": ...
    def place_peripheral_macros(self, strategy: PlacementStrategy) -> PlacementResult: ...
    def align_pads_to_regions(self) -> PadAlignmentResult: ...
    def export_tcl(self, path: str) -> None: ...
    def export_def(self, path: str) -> None: ...

class MacroInstance:
    """Physical macro block, exact bounding box, halo, orientation, and pin terminals."""
    def move_to(self, x: float, y: float) -> None: ...
    def set_orientation(self, orient: Orientation) -> None: ...
    def set_halo(self, left: float, right: float, top: float, bottom: float) -> None: ...
    def overlaps(self, other: "MacroInstance") -> bool: ...

class PowerDomain:
    """IEEE 1801 UPF domain boundary, supply nets, primary voltage, and cell instances."""
    def set_primary_voltage(self, voltage: float) -> None: ...
    def add_element(self, hierarchical_instance: str) -> None: ...
    def get_isolation_rules(self) -> List["IsolationRule"]: ...
    def get_level_shifters(self) -> List["LevelShifterRule"]: ...
```

- **Encapsulation & Validation**:
  SDK object setters (`move_to()`, `set_die_dimensions()`, `set_primary_voltage()`) must execute DRC legalization and value assertions internally. Invalid physical states must raise typed domain exceptions (`DRCViolationError`, `OverlappingHaloError`, `IllegalVoltageError`).
- **Complete Testability**:
  Any bug reproducible in the GUI canvas must be 100% reproducible in a 5-line Python script using SDK objects without importing Qt.

---

## 4. Strict Verification & Data Handling Contracts

Before writing any configuration to disk, rendering to canvas, or generating downstream TCL commands for OpenROAD, the following verification gates must pass:

### 4.1 Geometric Legalization & DRC Pre-Flight Check
1. **Manufacturing Grid Invariance**:
   - Every coordinate $(x, y)$, width $w$, height $h$, and margin must satisfy:
     $$(x \pmod{g}) = 0, \quad (y \pmod{g}) = 0$$
     where $g = 0.005\ \mu\text{m}$ (5 DBU in Sky130).
   - Floating-point representations must be quantized via round-to-nearest integer grid steps:
     ```python
     def legalize_coord(val: float, grid: float = 0.005) -> float:
         return round(round(val / grid) * grid, 3)
     ```
2. **Die and Core Bounding Invariants**:
   - Die coordinates must strictly encompass the core area:
     $$\text{core.left} \ge \text{die.left} + \text{margin}_{\text{left}}$$
     $$\text{core.bottom} \ge \text{die.bottom} + \text{margin}_{\text{bottom}}$$
     $$\text{core.right} \le \text{die.right} - \text{margin}_{\text{right}}$$
     $$\text{core.top} \le \text{die.top} - \text{margin}_{\text{top}}$$
   - Core width and height must evaluate strictly $> 0$. Self-healing logic must trigger if margin inputs exceed die dimensions.
3. **Macro Placement & Halo Non-Overlap Invariants**:
   - **Zero Tolerance for Physical Collisions**:
     For any two distinct macro instances $M_i, M_j$ placed on canvas:
     $$\text{BoundingBox}(M_i + \text{Halo}_i) \cap \text{BoundingBox}(M_j + \text{Halo}_j) = \emptyset$$
   - **Core Containment**:
     Every placed macro must sit entirely inside the core boundary unless explicitly declared as an I/O pad or corner cell.
   - **Site Row Snapping**:
     Macro $y$-origin must align to standard cell row boundaries ($y = k \cdot 2.72\ \mu\text{m}$ for Sky130 `unithd`).
4. **I/O Pin Pitch and Peripheral Enclosure**:
   - Pad pitch must exceed minimum metal spacing and pad width ($w_{\text{pad}} + s_{\text{min}}$).
   - No two pins or pads may occupy the same coordinate or overlapping bounding box along die edges.
   - Outer ring and staggered inner ring pads must preserve bond-wire clearance angles according to packaging specs.

### 4.2 IEEE 1801 (UPF 3.0) & IEEE 1497 (SDC) Standards Verification
1. **Multi-Voltage Power Intent (IEEE 1801)**:
   - **Explicit Voltage Integrity**: Every declared power domain (`PD_TOP`, `PD_COMPUTE`, `PD_LOWPWR`) must have an explicit numeric supply net voltage extracted from UPF (`-voltage <val>`). Monolithic defaults (e.g. assigning 1.8V to all domains) are strictly prohibited.
   - **Isolation Policy Completeness**: Signals transitioning from a switched/gated power domain to an always-on domain must have an IEEE 1801 `set_isolation` strategy with defined isolation cells and enable signals.
   - **Level Shifter Verification**: Any cross-domain net crossing a voltage gradient $|V_1 - V_2| > 0.05\text{V}$ must declare an explicit `set_level_shifter` rule specifying direction (`input`, `output`, or `both`).
   - **Domain Fence & Element Exclusivity**:
     $$\text{Elements}(\text{Domain}_A) \cap \text{Elements}(\text{Domain}_B) = \emptyset$$
     No hierarchical instance may be assigned to multiple power domains simultaneously.
2. **Timing Constraints (IEEE 1497 SDC)**:
   - Clocks declared in SDC (`create_clock`) must match top-level port declarations in the netlist.
   - Generated clocks, input/output delays, and multi-cycle paths must be validated before PnR invocation.

### 4.3 Netlist & Checksum Consistency Contract
- **Netlist Digest Audit**: On session restore, Silis must compute the SHA-256 hash of `netlist/<design>_netlist.v` and verify it against `netlist_checksum` in `.silis_fp_session.json`.
- **Instance Existence Assertion**:
  Before persisting or restoring macro locations, every instance identifier (`inst_name`) must be verified to exist in the current gate-level netlist or synthesized module graph. Stale synthetic instances (e.g. `macro_inst_4`) must be audited, warned, and rejected.

---

## 5. Downstream Script Emission Standards (OpenROAD Execution Guard)

When Silis writes out TCL scripts (`floorplan.tcl`, `domain_areas.tcl`, `pdn.tcl`, `route.tcl`), all commands must be synthesized defensively:

1. **Explicit Parameters Only**:
   - Never omit layer names, widths, offsets, or routing grids.
   - Always export coordinates formatted to three decimal places (`{:.3f}`).
2. **Defensive TCL Assertions**:
   - Wrap critical OpenROAD commands with error checks and status codes.
   - Floorplan initialization must explicitly declare both core and die boundaries:
     ```tcl
     initialize_floorplan -die_area { 0.000 0.000 <die_w> <die_h> } \
                          -core_area { <core_x1> <core_y1> <core_x2> <core_y2> } \
                          -site unithd
     ```
3. **Atomic File Persistence**:
   - All generated session files and TCL scripts must use transactional writing (write to `.tmp` file and perform an atomic `os.replace`). This guarantees zero file truncation or corrupted scripts when processes are interrupted.

---

## 6. UI/UX Architecture: Pure Native Qt6 Engineering

### 6.1 Zero Toy Elements & Zero Emojis
- **Absolute Emoji Ban**: Never use emojis in UI titles, button labels, logs, status bar messages, dialogs, tooltips, or agent chat responses.
- Use precise industry-standard terminology:
  - Prohibited: `Save`, `Warning!`, `Power Domain Ready`
  - Required: `Save Floorplan Session`, `DRC Violation: Overlapping Halo Detected`, `Power Domain Configuration Applied`

### 6.2 Native Qt6 Semantic Architecture
- **No Injected CSS Clutter**: Avoid arbitrary neon hex codes, web-style rounded buttons, or custom cartoonish stylesheets.
- **Stock Qt6 Widgets & Palettes**:
  - Rely on native `QPalette` semantic roles (`QPalette.ColorRole.Window`, `Base`, `Text`, `Highlight`, `PlaceholderText`).
  - Use `QStyle` standard system icons (`self.style().standardIcon(...)`).
  - Ensure theme neutrality across Linux desktop environments (GNOME, KDE Plasma, XFCE).

### 6.3 Engineering Typography & Metric Formats
- Coordinates, DBU units, voltages, and instance counts must use system monospace typography:
  ```python
  font = QFontDatabase.systemFont(QFontDatabase.SystemFont.FixedFont)
  font.setPointSize(9)
  ```
- Always include standard physical units:
  - Pitch & Distance: `µm` (microns) or `DBU` (database units)
  - Voltage: `V` (e.g. `1.800 V`, `0.950 V`)
  - Timing: `ps`, `ns`
  - Area: `µm²`, `mm²`
  - Power: `µW`, `mW`

---

## 8. Memory Layer & Token-Lean Navigation Protocol

### 8.1 Zero-Guessing Memory Architecture
Before executing multi-call probes, consult the persistent pre-indexed memory vault:
- **Architecture & Symbol Maps**:
  - `silis/docs/memory/CODEBASE_KNOWLEDGE.md` (Master API, class hierarchies, and execution graph)
  - `silis/docs/memory/SYMBOL_INDEX.json` (Exact line mappings for 2400+ classes, methods, and Tcl commands)
- **Downstream Engine Diagnostics**:
  - `silis/docs/memory/EDA_DOWNSTREAM_PLAYBOOK.md` (Physical failure modes for OpenROAD, Yosys, KLayout)
  - `silis/docs/memory/eda_error_registry.json` (Error registry queried via `python3 tools/eda_err.py <CODE>`)
- **Strict Exclusion Zone**:
  - Never index or inspect `third-party/`, `dist/`, `build/`, `build_appimage/`, or `.git/`.

### 8.2 Two-Call Verification Rule & Executable Diagnostic Tooling
When inspecting code or debugging downstream execution failures, **NEVER perform raw text greps across massive DEF or codebase files**. Use the deterministic bundled tools in `scripts/`:

1. **Token-Lean DEF Inspector** (`scripts/def_inspector.py`):
   Avoid dumping multi-megabyte DEF files into context.
   ```bash
   # Summary (die, components count, nets, tracks)
   python3 .agents/skills/silis-eda-architect/scripts/def_inspector.py results/temp.def summary

   # Search placed macros or cells
   python3 .agents/skills/silis-eda-architect/scripts/def_inspector.py results/temp.def components -p sram -l 10 --placed

   # Search I/O pins and coordinates
   python3 .agents/skills/silis-eda-architect/scripts/def_inspector.py results/temp.def pins -p clk

   # Inspect a specific net definition without reading 50,000 lines
   python3 .agents/skills/silis-eda-architect/scripts/def_inspector.py results/temp.def net vdd
   ```

2. **AST Index & Query Engine** (`scripts/silis_ast.py`):
   ```bash
   python3 .agents/skills/silis-eda-architect/scripts/silis_ast.py class PowerDomain
   python3 .agents/skills/silis-eda-architect/scripts/silis_ast.py method set_row_site
   python3 .agents/skills/silis-eda-architect/scripts/silis_ast.py signal domainApplied
   ```

3. **Downstream Error Diagnostic CLI** (`scripts/eda_err.py`):
   ```bash
   python3 .agents/skills/silis-eda-architect/scripts/eda_err.py PDN-0240
   ```

4. **Detailed Reference Manuals** (`references/`):
   Consult on-demand without bloating context:
   - [CODEBASE_KNOWLEDGE.md](./references/CODEBASE_KNOWLEDGE.md)
   - [EDA_DOWNSTREAM_PLAYBOOK.md](./references/EDA_DOWNSTREAM_PLAYBOOK.md)
   - [SILIS_PYTHON_SDK_MANUAL.md](./references/SILIS_PYTHON_SDK_MANUAL.md)
   - [OPENROAD_FLOW_REFERENCE.md](./references/OPENROAD_FLOW_REFERENCE.md)


---

## 9. Obsidian Second Brain Protocol for EDA Software Builders

Silis engineers are **EDA software builders and compiler architects**, not manual layout technicians. Obsidian acts as our second brain and active scratchpad, integrating the workflows from [`adriangrantdotorg/Obsidian-Skills`](https://github.com/adriangrantdotorg/Obsidian-Skills).

### 9.1 Developer Scratchpad Functions
- **Compiler IR & AST Schemata**: Abstract syntax trees for Verilog, IEEE 1801 UPF 3.0, and IEEE 1497 SDC mapped to DEF/LEF geometric IRs.
- **Algorithmic Scratchpad**: Deriving coordinate quantization, sweep-line segment collision checkers, and rectilinear polygon decomposition for power domains.
- **Downstream Engine Failure Triaging**: Root-cause analysis for OpenROAD, Yosys, KLayout, and Magic issues (e.g. `PDN-0240`, `PDN-0178`, `DPL-0006`, segfaults in `pdn::PdnGen::importUPF`).
- **GUI-SDK Parity Contracts**: State machine diagrams and signal/slot specs ensuring the Qt6 GUI remains a strict presentation shell over the Python SDK.
- **Multi-PDK Regression Matrices**: Tracking LEF site heights (`unithd` vs `unithddbl`), routing layer rules, and power switch footprints across Sky130, GF180, and modern FinFET nodes.

### 9.2 Vault Organization (`silisfoundation/docs/vault/`)
- `00_Index.md` — Master navigation and architectural contracts
- `01_Agent_Directives_EDA_Parity.md` — Strict GUI-SDK parity and zero-catch laws
- `02_EDA_Error_Playbook.md` — Downstream engine error code triaging
- `03_Multi_Voltage_UPF_Standards.md` — IEEE 1801 UPF 3.0 multi-domain specifications
- `04_Floorplan_Geometry_and_Macro_Placement.md` — Legalization, site snapping, channel formulas
- `05_PDN_Mesh_and_Pad_Trunk_Architecture.md` — Dual-layer orthogonal grids & pad trunks
- `06_Verification_and_Signoff_Checklist.md` — Pre-flight DRC/LVS/CTS gates
- `scratchpad/` — Active algorithmic sketches, layout planning notes, and testbench designs

### 9.3 Obsidian CLI Best Practices (from `adriangrantdotorg/Obsidian-Skills`)
- Target specific vaults explicitly: `obsidian vault="vault" search query="PDN-0240"`
- Append or create notes silently to preserve IDE focus: `obsidian append file="scratchpad/macro_plan" content="..." silent`
- On Linux, use `--no-sandbox` if running in automated headless subshells: `obsidian help --no-sandbox`
- When developing custom plugins or UI overrides, adhere to null-safety on `app.internalPlugins`, use zero-width spaces (`\u200B`) to clear native titles, and read editor coordinates via `CodeMirror 6` document state rather than brittle DOM elements.


