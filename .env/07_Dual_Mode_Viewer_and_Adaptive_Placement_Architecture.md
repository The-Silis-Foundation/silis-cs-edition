---
title: Dual-Mode Layout Viewer and Adaptive Coarse-Placement Guided Insertion Architecture
tags: [eda, qt6, layout-viewer, coarse-placement, power-switches, heatmaps]
---

# Dual-Mode Layout Viewer & Adaptive Coarse-Placement Guided Architecture

## 1. Coarse-Placement Guided Cell Synthesis
1. **Mock Coarse Placement**: Run fast initial global placement to determine standard cell cluster density.
2. **Centroid Extraction**: Calculate standard cell center-of-gravity (CoG) per power domain:
   $$X_{\text{CoG}} = \frac{1}{N} \sum X_i, \quad Y_{\text{CoG}} = \frac{1}{N} \sum Y_i$$
3. **Centroid-Anchored Insertion**: Anchor the power switch header matrix centered over $(X_{\text{CoG}}, Y_{\text{CoG}})$ to minimize static/dynamic IR-drop to active logic.
4. **Boundary Level Shifter Insertion**: Place level shifters on the specific perimeter facing boundary-crossing nets.

## 2. Dual Viewer Modes (Instance-Wise vs Layer-Wise)
- **Top Menu Navigation**: `View -> Viewer Mode -> [Instance-Wise View | Layer-Wise View]`
- **Instance-Wise View**:
  - Semantic cell categorization:
    - Macros: Deep Navy `#1A237E`
    - Power Switches: Vivid Gold `#FFB300`
    - Level Shifters: Cyan `#00E5FF`
    - Clock Cells: Magenta `#D500F9`
    - I/O Pads: Slate Cyan `#006064`
- **Layer-Wise View**:
  - Renders physical metal stack geometry (`li1`, `met1`..`met5`, vias).
  - Dockable layer visibility panel with toggle checkboxes.
- **Unified Heatmap Engine**:
  - Normalized density, congestion, and IR-drop heatmaps projected seamlessly onto both canvas modes with adjustable alpha blending.
