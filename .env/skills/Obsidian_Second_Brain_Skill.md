---
name: obsidian-second-brain
description: Autonomous second brain and developer scratchpad management inside Obsidian for EDA software architects. Implements the patterns from adriangrantdotorg/Obsidian-Skills (obsidian-cli, plugin & theme development, vault indexing) combined with deep compiler IR design, algorithm derivations, OpenROAD/Yosys failure triaging, and multi-PDK regression tracking.
---

# Obsidian Second Brain & EDA Software Builder Scratchpad

This skill defines the operational protocol for using **Obsidian** as an autonomous, persistent "Second Brain" and developer scratchpad for **EDA Software Architects and Compiler Engineers** working on the Silis platform. It incorporates the best practices and CLI workflows established by [adriangrantdotorg/Obsidian-Skills](https://github.com/adriangrantdotorg/Obsidian-Skills).

---

## 1. Core Persona & Architecture

> [!IMPORTANT]
> **EDA Software Builder Identity**:
> You are **NOT** a physical design technician manually tweaking layout coordinates in a viewer.
> You are an **EDA Software Architect and Systems Compiler Engineer**.
> You design the intermediate representations (IR), algorithms (placement, CTS, PDN, routing), SDK objects, and automated toolchain drivers that automate physical design at scale.

Obsidian is your **working memory layer**, structured to support:
1. **Compiler IR & AST Schemata**: Abstract syntax trees for Verilog, IEEE 1801 UPF 3.0, and IEEE 1497 SDC mapped to DEF/LEF geometric IRs.
2. **Algorithmic Scratchpad**: Deriving coordinate snapping, sweep-line segment collision checkers, and rectilinear polygon decomposition for power domains.
3. **Downstream Engine Failure Triaging**: Root-cause analysis for OpenROAD, Yosys, KLayout, and Magic issues (e.g. `PDN-0240`, `PDN-0178`, `DPL-0006`, segfaults in `pdn::PdnGen::importUPF`).
4. **GUI $\leftrightarrow$ SDK Parity Contracts**: State machine diagrams and signal/slot specs ensuring the Qt6 GUI remains a strict presentation shell over the Python SDK.
5. **Multi-PDK Regression Matrices**: LEF site heights (`unithd` vs `unithddbl`), routing layer rules, and power switch footprints across Sky130, GF180, and modern FinFET nodes.

---

## 2. Vault Organization & Structure

The canonical Silis Obsidian Vault is located at:
`silisfoundation/docs/vault/`

```text
silisfoundation/docs/vault/
├── .obsidian/                           # Vault settings, workspace, core-plugins
├── 00_Index.md                          # Master navigation and architectural contracts
├── 01_Agent_Directives_EDA_Parity.md    # Strict GUI-SDK parity and zero-catch laws
├── 02_EDA_Error_Playbook.md             # Downstream engine error code triaging
├── 03_Multi_Voltage_UPF_Standards.md    # IEEE 1801 UPF 3.0 multi-domain specifications
├── 04_Floorplan_Geometry_and_Macro_Placement.md # Legalization, site snapping, channel formulas
├── 05_PDN_Mesh_and_Pad_Trunk_Architecture.md    # Dual-layer orthogonal grids & pad trunks
├── 06_Verification_and_Signoff_Checklist.md     # Pre-flight DRC/LVS/CTS gates
├── compiler/                            # IR definitions, AST transformations, Tcl shims
├── algorithms/                          # Placement heuristics, polygon operations, proofs
├── pdks/                                # Sky130A, Sky130B, GF180MCU layer and cell profiles
└── scratchpad/                          # Active engineering notes, logs, and layout drafts
```

---

## 3. Obsidian CLI & Automation Workflows

Incorporating patterns from `adriangrantdotorg/Obsidian-Skills`:

### 3.1 Vault Navigation & Note Operations
When interacting with Obsidian via CLI or headless scripts:
- **Search Vault**:
  ```bash
  obsidian vault="vault" search query="PDN-0240" limit=5
  ```
- **Read & Append to Scratchpad**:
  ```bash
  obsidian read file="02_EDA_Error_Playbook"
  obsidian append file="scratchpad/active_session" content="- [x] Verified zero catch blocks on pdn step"
  ```
- **Create Note with Frontmatter**:
  ```bash
  obsidian create name="scratchpad/macro_topology_beast" content="# 6-SRAM Beast Topology\n..." silent
  ```

### 3.2 Linux Sandbox & Snap Safety Rules
- On Linux desktop distributions (Ubuntu 22.04/24.04), always invoke the CLI with `--no-sandbox` if running in an automated or headless environment:
  ```bash
  obsidian help --no-sandbox
  ```
- If installed via Snap:
  ```bash
  snap run obsidian help
  ```

---

## 4. Plugin & Theme Development Patterns

When extending Obsidian (or developing custom viewers/visualizers inside Obsidian for Silis):
- **Null Safety on Internal APIs**: Never assume `app.internalPlugins` or sub-plugins are fully loaded. Always safely traverse (`app.internalPlugins?.plugins?.workspaces?.instance?.activeWorkspace`).
- **CodeMirror 6 Live Preview**: Never extract links or tokens from DOM `.cm-link` elements directly. Convert pointer coordinates via `cmView.posAtCoords({x, y})` and slice raw Markdown text from `cmView.state.doc`.
- **Zero-Width Space Overrides**: When clearing titles or labels, use `\u200B` rather than pure whitespace to prevent Obsidian from falling back to default text.
- **Fast Build-Reload Loop**:
  ```bash
  npm run build && obsidian plugin:reload id=<plugin-id>
  ```
- **Live Streamed Diagnostics**:
  ```bash
  obsidian dev:errors
  obsidian dev:console level=error
  ```

---

## 5. EDA Software Developer Scratchpad Protocol

Whenever tackling a new EDA feature or downstream failure:
1. **Document the Failure in Obsidian First**:
   Record the exact command line, engine output, error code (e.g. `[ERROR PDN-0240]`), and relevant LEF/DEF excerpts in `docs/vault/02_EDA_Error_Playbook.md`.
2. **Derive the Architectural Fix**:
   Formulate the software fix (e.g. gating physical power switches behind `PowerDomain.enable_physical_power_switches()`, standardizing row height attributes).
3. **Verify the Zero-Catch Law**:
   Ensure no Tcl or Python code uses `catch` blocks or `try...except pass` to hide engine warnings.
4. **Link Related Notes**:
   Use Obsidian wikilinks `[[04_Floorplan_Geometry_and_Macro_Placement]]` to cross-reference rules and algorithms.
