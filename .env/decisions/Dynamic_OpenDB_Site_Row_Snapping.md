# Architecture Decision Record: Dynamic OpenROAD/OpenDB Grid and Site Snapping

## Context
Standard-cell placement and macro placement require adherence to two physical quantization invariants:
1. **Manufacturing Grid**: $(x \pmod g) = 0$ where $g = \text{dbTech::getManufacturingGrid()} / \text{dbu}$.
2. **Site Row Alignment**: $(y \pmod{h_{\text{site}}}) = 0$ where $h_{\text{site}} = \text{dbSite::getHeight()} / \text{dbu}$.

Hardcoding values like `2.72` or `0.005` breaks multi-PDK support (e.g. GF180MCU with $5.04\,\mu\text{m}$ rows).

## Architectural Solution
1. **In-Engine Tcl Snapping (`silis_fp.tcl`)**:
   - Added `_silis_get_mfg_grid` querying `[::ord::get_db_tech] getManufacturingGrid`.
   - Added `_silis_get_site_row_height` querying `[lindex [[::ord::get_db_block] getRows] 0] getSite getHeight`.
   - Updated `place_macro` to use `_silis_snap_row_y` for the $y$-coordinate:
     ```tcl
     set ox [_silis_snap [lindex $origin 0]]
     set oy [_silis_snap_row_y [lindex $origin 1]]
     ```
2. **Python SDK OpenDB Inspector (`opendb_bridge.py`)**:
   - Added `pipe.db.site_height`, `pipe.db.site_width`, and `pipe.db.mfg_grid` dynamically queried from OpenDB.
   - Tested and verified against active design:
     - `site_height`: `2.72 µm`
     - `site_width`: `0.46 µm`
     - `mfg_grid`: `0.005 µm`

## Related Notes
- [[00_Index]]
- [[silicon/SkyWater130_Quirks]]
- [[decisions/Detailed_Placement_Pad_Alignment_Blocker]]
- [[toolchain/OpenROAD_Personality]]
