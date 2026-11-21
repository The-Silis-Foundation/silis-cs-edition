---
title: "Silis App Entrypoint & CLI Subsystem"
tags:
  - codebase
  - entrypoint
  - cli
  - gui
---

# Silis App Entrypoint & CLI Subsystem

File: [`silis.py`](file:///home/econsystems/silisfoundation/silis/silis.py)

The top-level application root for Silis orchestrates both headless CLI invocations and the full Qt6 IDE desktop application.

```mermaid
graph TD
    CLI["CLI Invocation (sys.argv)"]
    Parser["argparse Parser (silis.py:2840)"]
    ModeCheck{"Execution Mode?"}
    HeadlessFlow["Headless Flow Runner (Pipeline.run_step / run_all)"]
    TUI["Interactive TUI REPL (core/cli/tui_repl.py)"]
    GUI["QApplication & MainWindow (silis.py:200-800)"]

    CLI --> Parser
    Parser --> ModeCheck
    ModeCheck -->|--run-* / --step| HeadlessFlow
    ModeCheck -->|-i / --shell| TUI
    ModeCheck -->|No batch args| GUI

    HeadlessFlow --> SDK["[[codebase/02_Core_SDK_Engine]]"]
    GUI --> Backend["[[codebase/03_Backend_Physical_Design_Flow]]"]
    GUI --> Editor["[[codebase/08_Editor_and_Integrated_Terminal]]"]
    GUI --> PDK["[[codebase/07_PDK_and_Technology_Management]]"]
```

## Key Classes & Entrypoints
- `class MainWindow(QMainWindow)`: Primary application window hosting tabs for Editor, Synthesis, Floorplanner, SiliconPeeker, Backend PnR, Terminal, and Volare.
- `class SilisLauncher(QDialog)`: Interactive startup wizard and recent project selector.
- `def main()`: CLI parser evaluating `--init`, `--rtl`, `--tb`, `--sdc`, `--pdk`, `--top`, `--run-all`, `--run-floorplan`, `--run-pdn`, `--run-route`, and `--bend` (standalone backend mode).

## Connected Graph Nodes
- Upstream: [[codebase/00_Codebase_Graph]]
- Runtime Bundle: [[codebase/09_Packaging_and_Standalone_Runtime]]
- Engine Driver: [[codebase/02_Core_SDK_Engine]]
- Flow Controller: [[codebase/03_Backend_Physical_Design_Flow]]
