# SILIS Multi-Language HDL/HLS Source Intake & Lowering System
## Comprehensive Low-Level Implementation Plan

> This document specifies the full design, data structures, algorithms, GUI integration plan, and implementation tasks for extending SILIS with a unified **Smart HDL/HLS Source Intake & Transpilation Manager** supporting VHDL-2008, SystemVerilog, and C/C++ HLS inputs.

---

## 1. Overview & Goals

### 1.1 Problem Statement
SILIS's current GUI exclusively accepts flat synthesizable Verilog RTL. Projects written in VHDL-2008, SystemVerilog (with packages, interfaces, and structs), or C/C++ (high-level synthesis) cannot be imported, structured, or compiled to gate-level netlists through the GUI. The CLI's `pipe.convert()` method implements the lowering engines but has no GUI-accessible entry point.

### 1.2 Deliverable
A first-class GUI dialog called the **Smart Intake & Lowering Wizard** that:
1. Ingests a folder or set of files (VHDL, SV, C/C++).
2. Automatically identifies languages, parses dependencies, and produces a topologically sorted compilation order.
3. Detects the top-level module/entity and verification testbenches automatically.
4. Extracts port interfaces for downstream SDC/IO pin auto-configuration.
5. Runs an optional functional simulation pre-check.
6. Invokes `pipe.convert()` to lower the source to synthesizable Verilog and registers it into the project.

---

## 2. Source Files to Create or Modify

| File | Action | Purpose |
|---|---|---|
| `silis/intake/` | **CREATE DIR** | New package housing all intake logic |
| `silis/intake/__init__.py` | CREATE | Package init |
| `silis/intake/source_scanner.py` | CREATE | File crawler, language tagger, dependency extractor |
| `silis/intake/dependency_graph.py` | CREATE | DAG construction & topological sort (Kahn's algorithm) |
| `silis/intake/top_finder.py` | CREATE | In-degree analysis for top module & testbench classifier |
| `silis/intake/port_extractor.py` | CREATE | Unified port schema extractor per language |
| `silis/intake/intake_dialog.py` | CREATE | Full GUI wizard dialog (PyQt6) |
| `silis/intake/conversion_runner.py` | CREATE | Wrapper that calls `pipe.convert()` from GUI thread |
| `silis/intake/tb_runner.py` | CREATE | Testbench pre-check runner (GHDL sim / iverilog / vvp) |
| `silis/silis.py` | MODIFY | Add "Import HDL/HLS..." toolbar button + keyboard shortcut |
| `silis/sdk/pipeline.py` | MODIFY | Expose `convert()` to accept pre-sorted file list |

---

## 3. Data Structures

### 3.1 `SourceFile` (internal per-file record)
```python
@dataclass
class SourceFile:
    path: str                    # Absolute path to file
    lang: str                    # 'vhdl' | 'sv' | 'verilog' | 'c' | 'cpp'
    std: str                     # e.g. '08', '2012', 'c++17'
    declared_units: list[str]    # Module/entity/package names declared in this file
    required_units: list[str]    # Names this file depends on (use/import/include)
    is_testbench: bool           # True if classified as a simulation-only file
    tier: int                    # Assigned topological tier (0 = bottom/no-deps)
```

### 3.2 `PortSpec` (unified port representation)
```python
@dataclass
class PortSpec:
    name: str                    # Port name as written in source
    direction: str               # 'input' | 'output' | 'inout'
    width: int                   # Bit width (1 for scalar)
    port_type: str               # 'clock' | 'reset' | 'data' | 'uart' | 'spi' | 'gpio' | 'bus'
    raw_type: str                # Original type string e.g. 'std_ulogic_vector(31 downto 0)'
```

### 3.3 `IntakeResult` (output from wizard)
```python
@dataclass
class IntakeResult:
    project_name: str
    language: str                # Primary detected language
    work_library: str            # VHDL work lib name or '' for SV/C
    std: str                     # Language standard
    sorted_rtl_files: list[str]  # Topologically ordered synthesizable files only
    testbench_files: list[str]   # Separated TB files
    top_module: str              # Detected or user-confirmed top name
    ports: list[PortSpec]        # Extracted port interface
    clocks: list[str]            # Names of clock ports
    resets: list[str]            # Names of reset ports
    generics: dict               # VHDL generics or Verilog parameters
    converted_verilog_path: str  # Path to output of pipe.convert()
```

---

## 4. Module-Level Specifications

### 4.1 `source_scanner.py`

#### Purpose
Walk an input directory or file list, detect file languages, and extract declared/required unit names.

#### `scan_sources(paths: list[str]) -> list[SourceFile]`
1. Recursively collect files with extensions: `.vhd`, `.vhdl`, `.sv`, `.svh`, `.v`, `.c`, `.cpp`, `.h`.
2. For each file, call `detect_language(path)` → language tag.
3. Call `extract_units(path, lang)` → `(declared_units, required_units)`.
4. Call `classify_testbench(path, lang, declared_units)` → `bool`.
5. Return list of `SourceFile`.

#### `detect_language(path) -> str`
| Extension | Language |
|---|---|
| `.vhd`, `.vhdl` | `vhdl` |
| `.sv`, `.svh` | `sv` |
| `.v` | `verilog` |
| `.c`, `.cpp`, `.cc` | `c` |
| `.h` | `c` (header, not compiled directly) |

#### `extract_units(path, lang) -> tuple[list[str], list[str]]`
- **VHDL**:
  - **Declares**: Regex for `entity\s+(\w+)\s+is`, `package\s+(\w+)\s+is`, `package\s+body\s+(\w+)\s+is`.
  - **Requires**: Regex for `use\s+(\w+)\.(\w+)\.all`, `use\s+(\w+)\.(\w+)`, component instantiation `:\s*entity\s+\w+\.(\w+)`, or component name from `component\s+(\w+)` declarations.
- **SystemVerilog / Verilog**:
  - **Declares**: Regex for `^\s*module\s+(\w+)`, `^\s*package\s+(\w+)`, `^\s*interface\s+(\w+)`.
  - **Requires**: Regex for module instantiations `^\s*(\w+)\s+(?:#\s*\(.*?\)\s+)?(\w+)\s*\(` where group 1 is a known declared module name.
- **C / C++**:
  - **Declares**: Top-level non-static function definitions matching `^\w[\w\s\*]+\s+(\w+)\s*\(`.
  - **Requires**: `#include\s+"([^"]+)"`, `#include\s+<([^>]+)>`.

---

### 4.2 `dependency_graph.py`

#### Purpose
Build a Directed Acyclic Graph of unit dependencies and produce a topologically sorted compilation order.

#### `build_graph(sources: list[SourceFile]) -> dict[str, list[str]]`
- Key: unit name (declared by a file).
- Value: list of units this file requires.
- Cross-reference: map declared_unit → SourceFile for lookup.

#### `topological_sort(sources: list[SourceFile]) -> list[SourceFile]`
Implements **Kahn's Algorithm**:
1. Compute in-degree of each `SourceFile` node (number of other files that must compile before it).
2. Initialize a queue with all files having `in_degree == 0`.
3. Iteratively pop from queue, add to `sorted_list`, decrement in-degrees of dependents, re-enqueue any that reach 0.
4. If `len(sorted_list) < len(sources)`: **Cycle Detected** → raise `CyclicDependencyError` listing the problematic files.

#### Tier Assignment
After sort, assign `tier` numbers:
- **Tier 0**: Packages and constants (no depends).
- **Tier 1**: Package bodies and shared primitives.
- **Tier 2**: Leaf entities (no sub-entity instantiations).
- **Tier 3+**: Hierarchical sub-systems.
- **Tier N**: Top-level entity (maximum depth).

---

### 4.3 `top_finder.py`

#### Purpose
Identify which file/module is the synthesis Top Module vs. which are testbenches.

#### `find_top_candidates(sources: list[SourceFile]) -> list[str]`
1. Build a set `all_declared = { unit for f in sources for unit in f.declared_units }`.
2. Build a set `all_required = { unit for f in sources for unit in f.required_units }`.
3. **Candidates = `all_declared - all_required`**: Units declared but never instantiated by anything else (in-degree = 0).
4. Filter out testbenches.
5. Rank remaining candidates by name heuristics:
   - **+3 priority**: Name contains `top`, `soc`, `core`, `chip`, `system`.
   - **+2 priority**: Name contains project folder basename.
   - **+1 priority**: Declared in the last tier of the topological sort.
   - **-5 penalty**: Has zero declared ports (likely abstract/empty top).

#### `classify_testbench(path, lang, declared_units) -> bool`
Returns `True` if any of:
1. **Filename**: Matches `*_tb.*`, `*testbench.*`, `*_sim.*`, `tb_*.*`, `*_top_tb.*`.
2. **VHDL**: Entity has empty port list: `entity\s+\w+\s+is\s*(?:generic\s*\([^)]*\)\s*;)?\s*end`.
3. **VHDL**: File contains `wait for \d+ ns` or `wait until rising_edge`.
4. **Verilog/SV**: Module has empty port list: `module\s+\w+\s*;` or `module\s+\w+\s*(\s*)`.
5. **Verilog/SV**: Contains `$dumpfile`, `$dumpvars`, `$finish`, `$display` or time-delay `#\d+`.
6. **C/C++**: Contains `main(`, `sc_main(`, or `assert(`.

---

### 4.4 `port_extractor.py`

#### Purpose
Parse the top-level entity/module declaration and produce a unified `list[PortSpec]`.

#### `extract_ports(top_file: SourceFile, top_name: str) -> list[PortSpec]`

##### VHDL Port Extraction
Full VHDL entity port regex:
```python
# Matches: port_name : [in|out|inout] type_declaration
PORT_RE = re.compile(
    r'(\w+)\s*:\s*(in|out|inout)\s+(.+?)(?:;|$)',
    re.IGNORECASE | re.MULTILINE
)
# Width extraction for std_ulogic_vector / std_logic_vector / unsigned / signed:
WIDTH_RE = re.compile(r'\((\d+)\s+downto\s+(\d+)\)')
```
- Extracts port name, direction, raw VHDL type.
- Computes width from `(N downto 0)` → `N + 1`.
- Scalar `std_ulogic` / `std_logic` / `bit` → `width = 1`.
- Strips `buffer` direction → normalizes to `output`.

##### VHDL Generics Extraction
```python
GENERIC_RE = re.compile(
    r'(\w+)\s*:\s*(\w+)\s*:=\s*([^;]+)', re.IGNORECASE
)
```
Returns `dict[name → default_value]`.

##### SystemVerilog / Verilog Port Extraction
Handles both ANSI-style and non-ANSI declarations:
```python
# ANSI: module top (input wire [31:0] a, output reg [7:0] b);
ANSI_PORT_RE = re.compile(
    r'(?:^|[;,(\s])\s*(input|output|inout)\s+'
    r'(?:wire\s+|reg\s+|logic\s+)?(?:signed\s+)?'
    r'(?:\[(\d+):(\d+)\]\s+)?(\w+)',
    re.MULTILINE
)
# Non-ANSI: input [31:0] a; in module body
```
Expands buses: `[31:0] data` → 32 individual 1-bit ports OR single 32-bit bus entry (controlled by user preference in wizard).

##### C/C++ HLS Interface Extraction
Scan for Bambu/VivadoHLS pragmas:
```c
#pragma HLS INTERFACE axis port=data_in  // AXI-Stream
#pragma HLS INTERFACE s_axilite port=ctrl // AXI-Lite control
#pragma HLS top
```
Read function signature of the HLS top function → map function parameters to I/O ports.

##### Port Type Classification (All Languages)
After name and direction are known, classify `port_type`:
```python
CLOCK_NAMES  = {'clk', 'clock', 'aclk', 'clk_i', 'sys_clk', 'clk_p', 'pclk', 'hclk', 'mclk'}
RESET_NAMES  = {'rst', 'reset', 'rst_n', 'rstn_i', 'rstn', 'aresetn', 'n_rst', 'sys_rst'}
UART_NAMES   = {'uart_rxd', 'uart_txd', 'rxd', 'txd', 'rx', 'tx', 'uart_rx', 'uart_tx'}
SPI_NAMES    = {'spi_sclk', 'spi_mosi', 'spi_miso', 'spi_csn', 'sclk', 'mosi', 'miso', 'csn'}
GPIO_NAMES   = {'gpio', 'gpio_in', 'gpio_out'}
```

---

### 4.5 `intake_dialog.py` — GUI Wizard

#### Class: `IntakeDialog(QDialog)`
Full-width dialog, non-modal, with 3 collapsible sections:

**Section A: Source Ingestion**
- `QLabel("Drop folder or select files:")`
- `DropZoneWidget` — subclass of `QFrame`, accepts `dragEnterEvent` + `dropEvent` for directories/files.
- `QPushButton("Browse...")` → `QFileDialog.getExistingDirectory`.
- `QLCDNumber`-style **file counter badge**: `"28 files detected"`.
- `QComboBox("Language Override:")` — `[ Auto-Detect ▼ | VHDL | SystemVerilog | Verilog | C/C++ ]`.
- `QComboBox("VHDL Standard:")` — `[ 08 ▼ | 93 | 02 | 19 ]` (visible only if VHDL detected).
- `QLineEdit("Work Library:")` — default `neorv32` if NEORV32 files detected else `work`.

**Section B: Dependency & Structure View**
- `QTreeWidget` with 3 columns: **File**, **Tier**, **Role** (`[PKG]`, `[RTL]`, `[TOP]`, `[TB]`).
- Rows color-coded:
  - Tier 0 packages: Dark teal.
  - RTL entities: Default.
  - Auto-detected Top: Bold green with `★` prefix.
  - Testbenches: Italic grey, strikethrough, `[EXCLUDED]` tag.
- `QComboBox("Top Entity:")` — pre-filled with ranked candidates. User can override.
- `QComboBox("Clock Port:")` — pre-filled from port classifier.
- `QComboBox("Reset Port:")` — pre-filled from port classifier.

**Section C: Actions**
- `QPushButton("▶ Run Functional Pre-Check")` → calls `tb_runner.py`.
  - Shows live stdout in embedded `QPlainTextEdit`.
  - On pass: badge turns green `✔ Simulation Passed`.
  - On fail: badge turns red `✘ Simulation Failed` with error line highlighted.
- `QPushButton("🔄 Lower & Convert to Verilog")` → calls `conversion_runner.py`.
  - Disabled until source scan complete.
  - Shows progress bar + live GHDL/sv2v/Bambu log.
  - On success: opens converted `.v` in the Code Editor tab automatically.
- `QPushButton("✔ Add to Project & Close")` → commits `IntakeResult` to project config.

---

### 4.6 `conversion_runner.py`

#### Class: `ConversionWorker(QThread)`
Runs `pipe.convert()` in a background thread so the GUI does not freeze.

```python
class ConversionWorker(QThread):
    progress = pyqtSignal(str)   # line of stdout
    finished = pyqtSignal(str)   # path to output .v
    error    = pyqtSignal(str)   # error message

    def __init__(self, intake_result: IntakeResult):
        ...

    def run(self):
        from sdk.pipeline import SilisPipeline
        # Construct a minimal SilisPipeline from intake result
        pipe = SilisPipeline(project=...)
        out_path = pipe.convert(
            src=intake_result.sorted_rtl_files,
            src_lang=intake_result.language,
            std=intake_result.std,
            hls_top=intake_result.top_module,
            out=os.path.join(project_root, 'netlist', f'{intake_result.top_module}_converted.v')
        )
        self.finished.emit(out_path)
```

#### What Happens After Conversion
1. `converted_verilog_path` is added to `project_config["rtl_files"]`.
2. `project_config["top_module"]` is updated.
3. `silis.silisproj` is written to disk atomically.
4. The GUI file tree refreshes.
5. The converted `.v` file opens in the Code Editor tab automatically.
6. The Synthesis tab is enabled.

---

### 4.7 `tb_runner.py`

#### `class TBRunner`

##### `run_vhdl(sorted_files, tb_entity, std='08', vcd_out='sim/waves.vcd') -> (bool, str)`
```python
# Step 1: Analyse
ghdl('-a', f'--std={std}', '--workdir=sim/', *sorted_files)
# Step 2: Elaborate
ghdl('-e', f'--std={std}', '--workdir=sim/', tb_entity)
# Step 3: Run
ghdl('-r', f'--std={std}', '--workdir=sim/', tb_entity,
     f'--vcd={vcd_out}', '--assert-level=error',
     f'--stop-time=10us')
```
Returns `(passed: bool, log: str)`.

##### `run_verilog(sorted_files, tb_top, vcd_out='sim/waves.vcd') -> (bool, str)`
```python
iverilog('-g2012', '-o', 'sim/tb.vvp', *sorted_files)
vvp('sim/tb.vvp')
```

##### `auto_load_waveform(ide, vcd_path)`
After a successful TB run, the runner calls:
```python
ide.frontend_tabs.setCurrentIndex(1)  # Switch to Waveform tab
ide.tab_waves.load_vcd(vcd_path)
```

---

## 5. Integration into `silis.py`

### 5.1 Toolbar Button
In `setup_toolbar()`:
```python
act_intake = QAction("📥 Import HDL/HLS...", self)
act_intake.setShortcut("Ctrl+Shift+I")
act_intake.triggered.connect(self.open_intake_wizard)
tb.addAction(act_intake)
```

### 5.2 Right-Click Context Menu in File Tree
When right-clicking any `.vhd`, `.sv`, or `.cpp` file in the left sidebar:
```
📥 Import & Structure Project from This File
🔄 Convert to Synthesizable Verilog
```

### 5.3 `open_intake_wizard(self)` Handler
```python
def open_intake_wizard(self):
    from intake.intake_dialog import IntakeDialog
    dlg = IntakeDialog(self, self.project_config, self.pdk_mgr)
    if dlg.exec():
        result = dlg.get_result()
        self._apply_intake_result(result)

def _apply_intake_result(self, result: IntakeResult):
    self.project_config["top_module"] = result.top_module
    self.project_config["rtl_files"] = [result.converted_verilog_path]
    self.project_config["vhdl_work"] = result.work_library
    self.project_config["vhdl_std"] = result.std
    self._save_project()
    self._reload_project_tree()
    self.tab_compile.editor.open_file(result.converted_verilog_path)
    self.frontend_tabs.setCurrentIndex(3)  # Synthesis tab
```

---

## 6. Per-Language Conversion Pipeline Details

### 6.1 VHDL → Verilog (GHDL)

**Tool**: `ghdl --synth --std=08 --out=verilog -e <top> [sorted_files...]`

**Full Command Construction**:
```python
cmd = [
    ghdl_bin,
    '--synth',
    f'--std={std}',
    '--out=verilog',
    '--no-formal',
    '-fsynopsys',         # Enables Synopsys package (std_logic_arith etc.) if needed
    '--workdir=netlist/ghdl_work/',
    '-e', top_entity
] + sorted_synthesizable_files
```

**Potential Failure Modes & Handling**:
| Error | Cause | Fix |
|---|---|---|
| `neorv32_package.vhd:1: no such file` | Sort order wrong | Re-run topological sort, show problematic edge in UI |
| `entity neorv32_cpu not found` | Wrong `--work` library | Prompt user to confirm work library name |
| `wait statement` | Testbench passed to synth | Re-check TB classifier, allow user to manually exclude files |
| `integer range constraint` | Unsupported GHDL synth feature | Warn user, add `--ieee=synopsys` flag automatically |

### 6.2 SystemVerilog → Verilog (sv2v)

**Tool**: `sv2v [packages...] [interfaces...] [sources...] --write=converted.v`

**Sort for sv2v**:
1. `*.pkg.sv` and `*_pkg.svh` files first (package definitions).
2. `*.if.sv` and `*_if.sv` files second (interface definitions).
3. All regular `.sv` module files last.

**Flags**:
```python
cmd = [sv2v_bin] + pkg_files + interface_files + module_files + [f'--write={out}']
# Optional: --define=SYNTHESIS to suppress sim-only blocks
```

### 6.3 C/C++ → Verilog (Bambu HLS)

**Tool**: `bambu --top-fname=<fn> [--clock-period=<ns>] [--memory-allocation-policy=<policy>] -o <out.v> [sources...]`

**Clock Period Selection**: GUI exposes a `QDoubleSpinBox("Target Clock Period (ns):")` defaulting to `10.0 ns` (100 MHz).

**Memory Policy Options in GUI**:
- `NO_BRAM` — Maps arrays to registers.
- `ALL_BRAM` — Infers BRAM macros for all arrays.
- `AUTO` — Bambu heuristic.

**Interface Synthesis**:
```c
// Recognized AXI pragmas that GUI should extract into port schema:
#pragma HLS INTERFACE axis port=in_stream  
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
```

---

## 7. Auto-Generated SDC After Conversion

After port extraction, automatically write a starter SDC file:
```tcl
# Auto-generated by SILIS Intake Wizard
# Project: {project_name}  Top: {top_module}

# Clock Definition (detected from port '{clock_port}')
create_clock -name clk -period {clock_period_ns} [get_ports {clock_port}]
set_clock_uncertainty 0.1 [get_clocks clk]

# Input/Output Delays
set_input_delay  -clock clk 1.0 [all_inputs]
set_output_delay -clock clk 1.0 [all_outputs]

# False Paths for Reset
set_false_path -from [get_ports {reset_port}]
```
Written to `source/{top_module}.sdc` and registered in `project_config["sdc_files"]`.

---

## 8. Project Config Registration Format

After successful intake + conversion, `silis.silisproj` is updated:
```json
{
  "project_name": "GEMMrv32_neorv32_cpu",
  "top_module": "neorv32_cpu",
  "pdk": "sky130",
  "vhdl_std": "08",
  "vhdl_work": "neorv32",
  "rtl_files": [
    "netlist/neorv32_cpu_converted.v"
  ],
  "original_sources": {
    "language": "vhdl",
    "files": [
      "GEMMrv32/rtl/core/neorv32_package.vhd",
      "..."
    ],
    "sorted_order": true
  },
  "sdc_files": [
    "source/neorv32_cpu.sdc"
  ],
  "testbench_files": [
    "GEMMrv32/sim/neorv32_tb.vhd"
  ],
  "port_schema": {
    "clocks": ["clk_i"],
    "resets": ["rstn_i"],
    "ports": [...]
  },
  "macros": []
}
```

---

## 9. Implementation Task Checklist

### Phase 1: Core Parsing Engine
- [ ] `source_scanner.py` — File crawler + language tagger.
- [ ] `source_scanner.py` — VHDL `extract_units()` (package, entity, use-clause parser).
- [ ] `source_scanner.py` — SV `extract_units()` (module, package, interface parser).
- [ ] `source_scanner.py` — C/C++ `extract_units()` (function + include parser).
- [ ] `source_scanner.py` — `classify_testbench()` heuristic classifier.
- [ ] `dependency_graph.py` — DAG builder + Kahn topological sort.
- [ ] `dependency_graph.py` — Cycle detection + error reporting.
- [ ] `dependency_graph.py` — Tier assignment.
- [ ] `top_finder.py` — In-degree analysis for top candidate detection.
- [ ] `top_finder.py` — Priority ranking heuristics.
- [ ] `port_extractor.py` — VHDL entity port + generic parser.
- [ ] `port_extractor.py` — Verilog/SV ANSI + non-ANSI port parser.
- [ ] `port_extractor.py` — C/C++ HLS pragma + function signature parser.
- [ ] `port_extractor.py` — Port type classifier (clock/reset/data/uart/spi).

### Phase 2: Conversion Engine Wrappers
- [ ] `conversion_runner.py` — `ConversionWorker` QThread.
- [ ] `conversion_runner.py` — GHDL invocation with sorted file list.
- [ ] `conversion_runner.py` — sv2v invocation with package-first sort.
- [ ] `conversion_runner.py` — Bambu invocation with clock period + policy.
- [ ] `conversion_runner.py` — `pipeline.py` integration (`pipe.convert()`).
- [ ] `conversion_runner.py` — Post-conversion project registration.
- [ ] Auto-SDC generator.

### Phase 3: Testbench Runner
- [ ] `tb_runner.py` — GHDL 3-step (analyse/elaborate/run).
- [ ] `tb_runner.py` — Icarus Verilog (iverilog/vvp).
- [ ] `tb_runner.py` — VCD auto-load into waveform tab.
- [ ] `tb_runner.py` — Assert failure detection + line highlighting.

### Phase 4: GUI Dialog
- [ ] `intake_dialog.py` — `DropZoneWidget` with drag-and-drop.
- [ ] `intake_dialog.py` — Language + standard selectors.
- [ ] `intake_dialog.py` — `QTreeWidget` with tier/role color-coded view.
- [ ] `intake_dialog.py` — Top module dropdown with ranked candidates.
- [ ] `intake_dialog.py` — Clock / Reset port selectors.
- [ ] `intake_dialog.py` — Pre-Check button + live log panel.
- [ ] `intake_dialog.py` — Convert button + progress bar + live log.
- [ ] `intake_dialog.py` — "Add to Project" commit action.

### Phase 5: silis.py Integration
- [ ] Add `📥 Import HDL/HLS...` toolbar button (`Ctrl+Shift+I`).
- [ ] Add right-click context menu items on `.vhd`, `.sv`, `.cpp` files in file tree.
- [ ] `open_intake_wizard()` handler.
- [ ] `_apply_intake_result()` method.

---

## 10. Decisions & Resolutions

### 10.1 Bus Port Display
**Resolution**: Irrelevant to this implementation scope. Port Schema stores the raw bus as a single entry (`[31:0]`); the IO Placer handles per-bit expansion independently.

---

### 10.2 Waveform Auto-Load Policy
**Resolution: Ask User.**

After `tb_runner.py` completes a successful simulation and a `.vcd` file is detected:
- Show a non-blocking `QMessageBox.question()`:
  ```
  "Simulation passed. A waveform file was generated.
   Open in Waveform Viewer now?"
   [ Open Waveform ] [ Stay Here ]
  ```
- If the user clicks **Open Waveform**: `ide.frontend_tabs.setCurrentIndex(1)` + `ide.tab_waves.load_vcd(vcd_path)`.
- If **Stay Here**: Do nothing, but show a clickable toast notification at the bottom bar:
  `"Waves ready → Click to view"` that triggers the same action on click.

---

### 10.3 Bambu HLS Availability
**Resolution: Bundle in `silis/third-party/bambu/`.**

Bambu HLS is not commonly pre-installed. It will be bundled as a pre-compiled binary under:
```
silis/third-party/bambu/
    bin/
        bambu          ← Linux x86_64 pre-compiled binary
    README.md
    LICENSE
```

**Detection Logic in `conversion_runner.py`**:
```python
BAMBU_SEARCH_PATHS = [
    shutil.which("bambu"),
    os.path.join(REPO_ROOT, "third-party", "bambu", "bin", "bambu"),
]
bambu_bin = next((p for p in BAMBU_SEARCH_PATHS if p and os.path.isfile(p)), None)
if not bambu_bin:
    raise FileNotFoundError(
        "Bambu HLS not found. Expected at silis/third-party/bambu/bin/bambu "
        "or in system PATH."
    )
```

**GUI Handling**:
- If Bambu not found: The **"Lower & Convert to Verilog"** button for C/C++ mode shows a warning icon and tooltip:
  `"Bambu HLS binary not found. See silis/third-party/bambu/README.md for installation."`
- The button remains clickable but immediately shows a `QMessageBox.critical()` with the install path before aborting.

---

### 10.4 Mixed-Language Projects (VHDL + Verilog in Same Project)
**Resolution: Unified Yosys+GHDL Plugin Pass (Primary) with Sequential Lowering Fallback.**

#### Background: How Industry Tools Handle This
The professional open-source standard (OpenLane 2, SiliconCompiler, Coriolis2) is a **unified Yosys synthesis session** using the GHDL-Yosys plugin:
- VHDL is parsed directly inside Yosys by the GHDL backend (no intermediate `.v` file written).
- Verilog files are loaded in the same session with `read_verilog`.
- Both languages co-exist in Yosys's internal **RTLIL** (RTL Intermediate Language) representation.
- A single `synth -top <top>` pass produces the unified gate-level netlist.
- SILIS already uses this approach in `silis/silis.py:870-897` for VHDL-only flows.

#### Strategy for Mixed-Language Intake
**Mode A — Unified Yosys+GHDL Plugin** *(primary, preferred)*:
1. Intake wizard detects mixed-language sources.
2. Runs dependency sort separately per language group.
3. During synthesis (not at convert-time), generates a combined Yosys synthesis script:
   ```tcl
   plugin -i ghdl;
   ghdl --std=08 --work=neorv32 neorv32_package.vhd ... -e neorv32_cpu;
   read_verilog wrapper.v custom_mac.v;
   synth_sky130 -top neorv32_cpu;
   ```
4. No intermediate `.v` files are produced. The converted Verilog output is the **final gate-level netlist** from Yosys.
5. **Testbench simulation** in this mode uses GHDL's co-simulation support: the VHDL testbench can instantiate the Verilog DUT via GHDL's `--vpi` or `ghdl --synth` elaboration.

**Mode B — Sequential Lowering then Merge** *(fallback, explicit user option)*:
1. Lower VHDL → `<top>_vhdl.v` via `ghdl --synth --out=verilog`.
2. Lower SV → `<top>_sv.v` via `sv2v`.
3. Merge all `.v` files into a single flat `<project>_merged.v` via Yosys `read_verilog` + `write_verilog`.
4. Run testbench simulation (Icarus Verilog / VCS) on the flat merged Verilog.
5. Present the flat Verilog in the Code Editor for inspection before synthesis.

#### GUI Exposure
In the Intake Dialog, when mixed-language sources are detected:
```
┌─────────────────────────────────────────────────────────────────────┐
│ ⚠  Mixed-Language Sources Detected (VHDL + Verilog)                 │
│                                                                     │
│ Synthesis Strategy:                                                 │
│  ● Unified Yosys+GHDL Plugin (recommended, no intermediate file)    │
│  ○ Sequential Lowering → Flat Verilog Merge (portable, debuggable)  │
└─────────────────────────────────────────────────────────────────────┘
```
Mode A is selected by default. The user can switch to Mode B if they want a portable flat Verilog output or are debugging cross-language instantiation issues.

#### Testbench in Mixed Mode
- **Mode A**: GHDL runs the VHDL testbench with the Verilog DUT loaded as a foreign module (GHDL VPI/VHPI co-simulation). The waveform dump and assertion results are unified.
- **Mode B**: After merging to flat Verilog, use Icarus Verilog (`iverilog -g2012`) with the flat Verilog plus a Verilog testbench wrapper.
- If the original testbench is in VHDL and Mode B is selected: SILIS auto-generates a skeleton Verilog testbench stub and warns:
  `"Your testbench is VHDL. In Sequential Lowering mode, a Verilog testbench template was generated at sim/<top>_tb.v. Please populate stimulus manually or switch to Unified mode."`
