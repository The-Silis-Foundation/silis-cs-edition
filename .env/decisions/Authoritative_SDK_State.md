# Decision: Authoritative SDK State

## Context
In many legacy EDA tools, the GUI acts as an independent application that directly modifies visual primitives and writes files, leading to divergent logic between interactive sessions and batch scripting pipelines.

## Decision
The Python SDK (`silis.core.sdk`) is the **single source of truth** for all physical design algorithms, DRC legalization, geometry calculations, and file emissions:
1. The GUI is strictly an observer/controller that issues calls to the SDK.
2. The GUI contains zero standalone physical design calculations or file writers.
3. Every GUI canvas interaction can be run headlessly in pure Python scripts.

## Consequences
- **Positive**: 100% bug reproducibility between GUI and headless regression tests.
- **Positive**: Clean separation of presentation and physical design mathematics.

## Related Notes
- [[00_Index]]
- [[philosophy/GUI_as_a_Projection]]
- [[01_Agent_Directives_EDA_Parity]]
