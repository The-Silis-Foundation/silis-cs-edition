---
title: Silis CS Edition & Native Layout Viewers Ground Truth Architecture
tags: [eda, silis, codespaces, fastsiliconpeeker, layer-viewer, openroad, vnc, webviews]
---

# Silis CS Edition & Native Layout Viewers Ground Truth Architecture

> [!IMPORTANT]
> **Ground Truth Law**: Never conflate native C++/Qt6 EDA engines with lightweight VS Code Webview previews.
> Every statement below is backed by verified source lines in the repository.

---

## 1. The Two Authentic Native Qt6 Layout Viewers in `silis/`

The Silis desktop platform contains **two separate, native C++/Qt6 layout viewers**, both orchestrated inside `silis/core/backendflow/flow/backendflow.py`:

```
silis Desktop IDE (backendflow.py)
│
├── [Mode 0: Instance-Wise View] ──► FastSiliconPeeker (silis/core/backendflow/siliconpeeker/peeker.py)
│                                    Backed by: fast_layout_viewer (C++ OpenGL QWidget)
│                                    Purpose: High-FPS Standard Cell Placement, LOD culling,
│                                             Heatmap overlays (IR Drop, Congestion, Power), Ruler
│
└── [Mode 1: Layer-Wise View]    ──► LayerViewerPanel (silis/core/layer_viewer/layer_viewer_panel.py)
                                     Backed by: SilisNativeLayerViewer (C++ Qt6 QWidget) + OpenDB
                                     Purpose: Signoff-grade OpenROAD visual parity, physical metal
                                              shapes (li1, met1–met5), cut vias, PDN rails,
                                              DesignPropertyInspector sidebar
```

### 1.1 FastSiliconPeeker (Native PyQt6 / C++ OpenGL)
- **Source**: `silis/core/backendflow/siliconpeeker/peeker.py` (lines 842–1791)
- **C++ Engine**: `silis/core/backendflow/fast_viewer/FastLayoutViewer.cpp` (exported via PyBind11 as `fast_layout_viewer`)
- **Key Methods**:
  - `load_cells_flat(const float* data, size_t count)`: Ingests 2D cell coordinates directly as a flat C-float buffer.
  - `set_lod(std_cell_threshold, nets_threshold)`: Dynamic Level-of-Detail thresholding.
  - `load_heatmap_density()`, `load_heatmap_congestion()`: Surface alpha-blended physical density maps.
- **Python SDK Invocation**:
  ```python
  import silis
  proj = silis.load_project("projects/demo_soc")
  proj.layout.view(layer_view=False)   # launches FastSiliconPeeker
  # or standalone:
  silis.view_layout("projects/demo_soc/results/temp.def")
  ```

### 1.2 Native Layer Viewer (`LayerViewerPanel` & `SilisNativeLayerViewer`)
- **Source**: `silis/core/layer_viewer/layer_viewer_panel.py` (lines 395–604)
- **C++ Canvas**: `silis/core/layer_viewer/src/SilisNativeLayerViewer.cpp` (lines 20–350)
- **C++ Binding**: PyBind11 module `silis_layer_viewer`
- **Data Pipeline**:
  - Spawns `LayerLoadWorker` (`QThread`), running `openroad -exit -python -c ...` to query OpenDB for exact wire coordinates and via cuts without freezing the GUI event loop.
  - Renders authentic OpenROAD layer colors (VRL Brown palette: Blue `met1`, Red `met2`, Green `met3`, Yellow-Green `met4`, Magenta `met5`).
  - Embeds `DesignPropertyInspector` (`inspector_widget.py`) in a horizontal `QSplitter`.
- **Python SDK Invocation**:
  ```python
  import silis
  proj = silis.load_project("projects/demo_soc")
  proj.layout.view_layers()            # launches LayerViewerPanel
  # or standalone:
  silis.view_layers("projects/demo_soc/results/temp.def")
  ```

---

## 2. What Is Inside `extension/web/` (VS Code Webviews)

The VS Code extension (`silis-cad-workbench`) provides in-browser visualization inside VS Code Webview panels (Chromium iframes). It hosts:

1. **DigitalJS Schematics** (`extension/web/schematic/`):
   - Pure client-side SVG + JointJS + ELK graph layout rendering Yosys JSON netlists.
   - Communicates via internal `postMessage` (`loadSchematic`, `componentSelected`).
2. **Intel ROHD Waveforms** (`extension/web/waveforms/`):
   - Pure client-side Flutter Web + CanvasKit (`canvaskit.wasm`) + Rust Wellen VCD parser (`wellen_bridge_bg.wasm`).
   - Receives VCD text via `safePostMessage` and executes in-browser time-scrubbing.
3. **Floorplan & Power Studio (FPS)** (`extension/web/fps/`):
   - Handwritten HTML5 Canvas 2D multi-voltage power domain (UPF) editor and macro placement tool.
4. **2D DEF Layout Canvas** (`extension/web/peeker/`):
   - A lightweight client-side HTML5 Canvas 2D preview script (`peeker_canvas.js`) parsing DEF bounding boxes and IO pin markers.
   - **NOT** FastSiliconPeeker! It does not use C++, does not run OpenGL, and does not perform LOD standard-cell culling.

---

## 3. The GitHub Codespaces Reality & Browser Privilege Model

### 3.1 Why Webviews CANNOT Host Native Qt6 Widgets
- VS Code Webviews are Chromium sandboxed `<iframe>` elements hosted on isolated webview origins (`*.vscode-cdn.net` or `https://<hash>.webview.<location>.github.dev`).
- A Linux C++/Qt6 window requires an X11 (`XOpenDisplay`) or Wayland server. It cannot be coerced into a DOM element.

### 3.2 The Virtual Display & Streaming Architecture
In headless cloud environments (GitHub Codespaces):
```text
[PyQt6 / C++ Process]
(FastSiliconPeeker / LayerViewerPanel / OpenROAD GUI)
         │
         ▼ renders to
[Xvfb :1] (Virtual Framebuffer, 1920x1080x24)
         │
         ▼ captured by
[x11vnc :1] (RFB Port 5901)
         │
         ▼ proxied by
[websockify] (Port 6081)
         │
         ▼ streamed to
[noVNC / Browser Client]
```

### 3.3 The Browser Privilege Traps
When serving `noVNC` on port 6081 inside Codespaces:

1. **Trap A: Private Port Authentication via Iframe**:
   - Codespaces private ports require authentication through `github.com/login`.
   - When loaded in a VS Code Webview iframe, modern browser **Third-Party Cookie Partitioning (CHIPS)** drops the GitHub authentication cookies.
   - Result: GitHub returns `X-Frame-Options: DENY` or an infinite redirect loop $\to$ **blank iframe**.

2. **Trap B: Public Port Anti-Phishing Interstitial**:
   - If port 6081 is made `public`, GitHub serves an anti-phishing HTML warning page (*"You are about to connect to a port made public by..."*).
   - This warning page intercepts the HTTP upgrade request and breaks the WebSocket (`ws://` / `wss://`) handshake needed by RFB/noVNC $\to$ **WebSocket connection failed**.

3. **The Solution: Dedicated Browser Tab Navigation**:
   - The native Qt6 viewers (`FastSiliconPeeker`, `LayerViewerPanel`, and `openroad -gui`) **must not be embedded in a Webview iframe**.
   - Instead, port 6081 is opened in a **Dedicated Top-Level Browser Tab** (or VS Code Simple Browser / external VNC client). Top-level tabs carry full browser privileges, handle GitHub OAuth cookies natively, and establish raw WebSockets without iframe sandboxing restrictions.

---

## 4. Verification Registry

| Component | Language / Framework | Exact Path | Verification Status |
|---|---|---|---|
| `FastSiliconPeeker` | Python / PyQt6 | `silis/core/backendflow/siliconpeeker/peeker.py:842` | Verified present |
| `FastLayoutViewerCore` | C++17 / OpenGL | `silis/core/backendflow/fast_viewer/FastLayoutViewer.cpp` | Verified compiled (.so active) |
| `LayerViewerPanel` | Python / PyQt6 | `silis/core/layer_viewer/layer_viewer_panel.py:395` | Verified present |
| `SilisNativeLayerViewer` | C++17 / Qt6 | `silis/core/layer_viewer/src/SilisNativeLayerViewer.cpp:20` | Verified compiled (.so active) |
| `OpenDB Extraction` | Python / OpenROAD | `silis/core/layer_viewer/layer_viewer_panel.py:37` (`LayerLoadWorker`) | Verified present |
| `DigitalJS Schematic` | SVG / JointJS / ELK | `extension/web/schematic/` | Verified pure client-side |
| `ROHD Waveforms` | Flutter / WASM | `extension/web/waveforms/` | Verified pure client-side |
| `FPS Studio` | HTML5 Canvas 2D | `extension/web/fps/` | Verified pure client-side |
| `2D Peeker Canvas` | HTML5 Canvas 2D | `extension/web/peeker/` | Verified lightweight JS sketcher |
