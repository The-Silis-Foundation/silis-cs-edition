# Decision: Token-Lean EDA Investigation

## Context & Historical Hindrance
During physical design debugging, traditional LLM agent patterns rely on brute-force text `grep` or reading `.def` files directly into context.
- ASIC DEF files for even modest SoCs are 15 MB to 50 MB (tens of thousands of lines).
- Attempting to inspect macro positions or trace power nets via raw file reads flooded the conversation context window with 100,000+ tokens, triggered turn timeouts, and led to context compaction that wiped out active working memory.

## Decision
All Silis engineering investigations must use deterministic, pre-indexed tools:
1. **`def_inspector.py`**: Custom parser that queries components, I/O pins, nets, and physical overlaps in under 50ms without loading whole files into context.
2. **`silis_ast.py`**: AST indexer querying classes, methods, and Qt signals in Python source.
3. **`eda_err.py`**: Pre-indexed dictionary of OpenROAD/Yosys error codes.

## Consequences
- **Positive**: Near-instant answers without token exhaustion.
- **Positive**: Deterministic answers based on geometry parsing rather than fuzzy regex.

## Related Notes
- [[00_Index]]
- [[01_Agent_Directives_EDA_Parity]]
