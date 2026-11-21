---
title: Strictly Read-Only OpenDB C++ Bridge Architecture
tags: [eda, opendb, odb, c++, pybind11, read-only, architecture]
---

# Strictly Read-Only OpenDB C++ Bridge Architecture (`silis/core/OpenROAD_odb/`)

## 1. Architectural Contract & The "Zero-Mutation" Invariant

Silis interfaces with OpenROAD's database (`odb`) exclusively as a **read-only observer**:
- **Zero In-Memory Mutations**: `silis_odb` will never expose mutating C++ methods (`dbInst::create`, `dbNet::setWire`, `dbBlock::destroy`).
- **Zero Modifications to OpenROAD**: `silis/third-party/OpenROAD` remains **100% untouched upstream code**.
- **Execution Separation**:
  - Interactive visualization, property queries, and DRC pre-checks happen via `silis_odb.so`.
  - Flow steps (placement, CTS, routing) are executed by the unmodified upstream OpenROAD binary via `silis -openroad`.

## 2. Directory Layout

```
silis/
  ├── third-party/OpenROAD/src/odb/       <-- 100% UNTOUCHED UPSTREAM SOURCE
  └── core/OpenROAD_odb/                  <-- STRICTLY READ-ONLY C++ BRIDGE
        ├── CMakeLists.txt                <-- Standalone build targeting OpenDB source
        ├── include/
        │     ├── OdbReadOnlyBridge.h     <-- Const database wrapper & query API
        │     └── SilisLogger.h           <-- Lightweight utl::Logger replacement
        ├── src/
        │     ├── OdbReadOnlyBridge.cpp   <-- Pure read-only C++ implementations
        │     ├── SilisLogger.cpp         <-- Console logging bridge
        │     └── bindings.cpp            <-- Pybind11 read-only exports
        └── build/
              └── silis_odb.so            <-- Standalone Python C-extension
```

## 3. High-Speed Const Query API

```cpp
namespace silis::odb_bridge {

struct InstSummary {
    std::string name;
    std::string master_name;
    int origin_x_dbu;
    int origin_y_dbu;
    int bbox_x1_dbu;
    int bbox_y1_dbu;
    int bbox_x2_dbu;
    int bbox_y2_dbu;
    std::string orient;
    std::string placement_status;
    bool is_macro;
};

class OdbReadOnlyBridge {
public:
    // Read-only ingestion
    bool read_tlef(const std::string& path);
    bool read_lef(const std::string& path);
    bool read_def(const std::string& path);
    void clear();

    // Global properties
    int get_dbu() const;
    void get_die_area(int& x1, int& y1, int& x2, int& y2) const;
    void get_core_area(int& x1, int& y1, int& x2, int& y2) const;

    // Fast zero-copy batch queries
    std::vector<InstSummary> get_all_instances() const;
    std::vector<InstSummary> get_instances_in_box(int x1, int y1, int x2, int y2) const;

    // Direct C++ const pointers for layoutViewer
    const odb::dbDatabase* get_raw_db() const;
    const odb::dbBlock* get_raw_block() const;
};

}
```

## 4. Related Notes
- [[00_Index]]
- [[08_OpenROAD_Native_Layer_Viewer_Plan]]
- [[decisions/Authoritative_SDK_State]]
