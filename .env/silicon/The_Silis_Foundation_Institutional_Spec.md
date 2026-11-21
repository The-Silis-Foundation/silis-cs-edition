# The Silis Foundation / Institutional Specification & Roadmap

## 1. Overview & Mandate

The Silis Foundation is an open-source hardware research and physical silicon design organization. Rather than operating as an ad-hoc collection of repositories, the Foundation investigates **deterministic electronic design automation (EDA)**, **custom domain-specific microarchitectures**, and **bare-metal execution runtimes**.

---

## 2. Research Divisions & Official Repositories

### Division Ⅰ: Physical Design Automation & Toolchain Systems
Focuses on synthesis-to-tapeout determinism, geometry visualization, and open EDA orchestration.

* **Silis IDE**
  * **Repository**: [https://github.com/The-Silis-Foundation/silis](https://github.com/The-Silis-Foundation/silis)
  * **Releases**: [GitHub Releases & AppImage](https://github.com/The-Silis-Foundation/silis/releases)
  * **Description**: Modular open-source RTL-to-GDSII ASIC design workbench integrating Yosys, OpenROAD, OpenSTA, and Magic VLSI. Features native C++ DEF parsing engines bound via `pybind11` for sub-millisecond layout decoding. Standalone Linux AppImage binaries available.

---

### Division Ⅱ: Spatial Computing & Microarchitectures
Explores tightly coupled processor coprocessors, execution pipeline extensions, and high-throughput systolic hardware IP.

* **GEMMrv32**
  * **Repository**: [https://github.com/The-Silis-Foundation/GEMMrv32](https://github.com/The-Silis-Foundation/GEMMrv32)
  * **Description**: Spatial 8×8 systolic-array matrix coprocessor integrated into the NEORV32 RISC-V Custom Functions Subsystem (CFS) with local ping-pong data tiling registers.

* **PulseGEMM**
  * **Foundation Fork**: [https://github.com/The-Silis-Foundation/PulseGEMM](https://github.com/The-Silis-Foundation/PulseGEMM)
  * **Upstream / Original**: [https://github.com/JeromeAntonyRobin/PulseGEMM](https://github.com/JeromeAntonyRobin/PulseGEMM)
  * **Description**: 16×16 INT8 systolic compute accelerator validated on Microchip PolarFire SoC featuring 64-bit AXI4-DMA, double-buffered SRAM tiling, and zero-overhead Linux UIO drivers (256 MACs/cycle). Maintained under the Foundation umbrella for silicon acceleration research.

---

### Division Ⅲ: System Runtimes & Verification
Investigates OS-less bare-metal firmware, cycle-accurate coprocessor simulation, and register-level telemetry.

* **GEMMrvStudio**
  * **Repository**: [https://github.com/The-Silis-Foundation/GEMMrvStudio](https://github.com/The-Silis-Foundation/GEMMrvStudio)
  * **Description**: Developer telemetry, simulation profiling, and waveform verification workbench for custom RISC-V coprocessors.

* **DOOM on GEMMrv32**
  * **Repository**: [https://github.com/JeromeAntonyRobin/DOOM-on-GEMMrv32](https://github.com/JeromeAntonyRobin/DOOM-on-GEMMrv32)
  * **Description**: Bare-metal, OS-less port of the classic DOOM engine targeted to custom GEMMrv32 RISC-V architecture, offloading 3D transformation matrices directly to systolic hardware coprocessor PEs.

---

## 3. Engineering Invariants & Principles

1. **Deterministic Execution**: Toolchains and packaging must eliminate unpinned host dependencies, providing reproducible binaries (e.g. self-contained AppImage distributions).
2. **Native Performance**: Layout geometry parsing, clock-tree visualization, and matrix compute leverage modern C++ and hardware functional units rather than interpreted scripting bottlenecks.
3. **Hardware Grounding**: All architectural proposals and accelerator IP are empirically validated against physical FPGA silicon and open-source standard-cell libraries (SkyWater 130nm).

---

## 4. Institutional Action Checklist

- [ ] Update `The-Silis-Foundation/.github/profile/README.md` with the new `The-Silis-Foundation/PulseGEMM` fork repository URL.
- [ ] Update `JeromeAntonyRobin/index.html` schema and cards to reference both the foundation fork and upstream source where appropriate.
- [ ] Maintain academic rigor and institutional framing across all organizational touchpoints.
