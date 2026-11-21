---
title: OpenROAD Native Qt6 Layout Viewer & Inspector Refactoring Plan
tags: [eda, openroad, layout-viewer, inspector, display-controls, qt6, c++]
---

# OpenROAD Native Qt6 Layout Viewer & Inspector Refactoring Plan

## 1. Architectural Contract & Invariants
- **Zero Window Embedding**: No `createWindowContainer` hacks, no X11 socket juggling, no detached slave processes. The viewer is a pure, in-process Qt6 `QWidget` running on the primary Silis UI event loop.
- **Pure OpenROAD UI Experience**: Retain the exact keyboard shortcuts, context menus, right-click net flightline highlighting, and the deep descriptor Inspector tree (`inspector.cpp`, `displayControls.cpp`, `layoutViewer.cpp`, `dbDescriptors.cpp`).
- **Timing Engine Decoupling**: Stub out `sta::dbSta` timing dependencies via a lightweight `stub_sta.h` header so the GUI compiles against OpenDB (`odb`) without needing the full OpenSTA engine.

---

## 2. Source Files to Refactor (`silis/core/layer_viewer/`)

The viewer lives in `silis/core/layer_viewer/`, pulling source directly from `silis/third-party/OpenROAD/src/gui/`:

```
silis/core/layer_viewer/
├── CMakeLists.txt              # Standalone Qt6 CMake build
├── include/
│   ├── stub_sta.h             # 30-line clean stub for sta::dbNetwork / sta::dbSta
│   └── SilisViewerBridge.h    # Qt6 signal/slot connector to Silis IDE
├── src/
│   ├── layoutViewer.cpp       # Canvas viewport, pan/zoom, paintEvent
│   ├── renderThread.cpp       # Background painter thread
│   ├── painter.cpp            # Cosmetic pens & metal layer renderers
│   ├── search.cpp             # Spatial search index
│   ├── ruler.cpp              # Interactive measurement calipers
│   ├── displayControls.cpp    # Physical layer checkbox tree
│   ├── inspector.cpp          # Deep object & connectivity property tree
│   ├── dbDescriptors.cpp      # OpenDB object property descriptors
│   ├── descriptor_registry.cpp# Descriptor factory
│   └── bindings.cpp           # Pybind11 module exporting LayoutViewer, DisplayControls, Inspector
└── build/
    └── openroad_viewer_qt6.so # Single importable Python C-extension
```

---

## 3. Qt5 to Qt6 Migration Steps
1. **Event Scoping**:
   - `QWheelEvent::pos()` $\to$ `event->position().toPoint()`
   - `QMouseEvent::pos()` $\to$ `event->position().toPoint()`
2. **Enum Namespacing**:
   - `Qt::LeftButton` $\to$ `Qt::MouseButton::LeftButton`
   - `Qt::ClosedHandCursor` $\to$ `Qt::CursorShape::ClosedHandCursor`
   - `QHeaderView::Stretch` $\to$ `QHeaderView::ResizeMode::Stretch`
3. **OpenSTA Decoupling (`stub_sta.h`)**:
   - Provide minimal definitions for `sta::dbSta`, `sta::dbNetwork`, and `sta::LibertyCell` returning empty timing collections so `displayControls.cpp` and `inspector.cpp` compile with zero errors.

---

## 4. Related Notes
- [[00_Index]]
- [[compiler/02_odb_readonly_bridge]]
- [[scratchpad/Active_Session]]
- [[decisions/Authoritative_SDK_State]]
