<div align="center">

<img src="silisfoundationicon.jpeg" height="110" alt="The Silis Foundation" style="border-radius: 20px; box-shadow: 0 8px 30px rgba(0,0,0,0.45); margin-bottom: 14px;" />

# The Silis Foundation
<sub>Open-source silicon design environments & digital hardware research.</sub>

<br>

<p align="center" style="max-width: 720px; font-size: 14.5px; line-height: 1.7; color: #cbd5e1;">
The Silis Foundation is an open-source initiative dedicated to electronic design automation (EDA) infrastructure and digital silicon research. The foundation engineers unified, production-grade ASIC design tooling and verified hardware compute architectures, eliminating legacy toolchain fragmentation and lowering the barrier to physical silicon tapeouts for researchers, developers, and students worldwide.
</p>

</div>

---

### ❖ Engineering Overview

<table>
<tr>
<td width="50%" valign="top">

### ⬡ Silicon Design Environments
*Native, turnkey EDA software stacks from RTL to physical GDSII.*

<br>

#### **[`Silis`](https://github.com/The-Silis-Foundation/silis)** <img src="silisicon.png" height="18" style="vertical-align: sub;" />
> **Full-Stack RTL-to-GDSII ASIC Design IDE**  
> Standalone AppImage bundling the complete open EDA stack (Yosys, OpenROAD, OpenSTA, Magic, SkyWater 130nm). Powered by custom C++ DEF layout parsing engines running at 60 FPS, live schematic virtualization, and interactive timing visualizers—zero host toolchain setup required.
> 
> `x86_64 AppImage` · `C++20 / PyQt6` · `Interactive Cockpit`

<br>

#### **[`Silis CS Edition`](https://github.com/The-Silis-Foundation/silis-cs-edition)**
> **Cloud ASIC Workbench for GitHub Codespaces**  
> The Silis IDE adapted natively as an authentic VS Code extension (`.vsix`) with in-repo pre-compiled ASIC binaries. Features browser-native Webview visualizers with live tri-way cross-probing between RTL source, DigitalJS schematics, and ROHD waveforms.
> 
> `Native VSIX` · `GitHub Codespaces` · `Tri-Way Cross-Probing`

</td>
<td width="50%" valign="top">

### ⬡ Hardware Research & Silicon IP
*Empirically validated compute fabrics and domain-specific microarchitectures.*

<br>

#### **[`PulseGEMM`](https://github.com/The-Silis-Foundation/PulseGEMM)**
> **Universal Systolic Matrix Accelerator IP**  
> High-throughput INT8 systolic tensor compute engine engineered for cross-platform deployment. Adapted by the Foundation from low-power PolarFire architectures to target all major open and proprietary FPGA fabrics with 64-bit AXI4-DMA and Linux UIO drivers.
> 
> `16×16 INT8 Systolic` · `AXI4-DMA` · `Cross-FPGA IP`

<br>

#### **[`GEMMrv32`](https://github.com/The-Silis-Foundation/GEMMrv32)**
> **Spatial RISC-V Systolic Coprocessor**  
> Custom 8×8 systolic matrix multiplier integrated directly into the NEORV32 core pipeline via the Custom Functions Subsystem (CFS). Executes dense matrix compute in processor register space, bypassing traditional software instruction bottlenecks.
> 
> `8×8 PE Grid` · `NEORV32 CFS` · `Hardware/Software Co-Design`

</td>
</tr>
</table>

---

<div align="center">
<sub>Designed & Maintained by <a href="https://jeromeantonyrobin.github.io/JeromeAntonyRobin/"><strong>Jerome Antony Robin</strong></a> · <strong>The Silis Foundation</strong> · Open Silicon for Everyone</sub>
</div>
