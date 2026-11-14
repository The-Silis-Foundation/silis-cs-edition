# Governance Charter — Silis Codespaces Edition

Developed by **Jerome Antony Robin**, this repository follows a **BDFL Governance model**.  
Co-developed by **Eatheshwar**, **Venkat**, and **Adhil**.

*Silis took 11 months from POC to pre-release.*

---

## 1. Benevolent Dictator for Life (BDFL)

**Jerome Antony Robin** is the Lead and BDFL of Silis and the Silis Codespaces Edition.

As BDFL, Jerome Antony Robin holds final decision-making authority over:
- Core physical design algorithms and downstream EDA tool integration (RTL to GDSII flow)
- GUI-SDK Dual Architecture and strict enforcement of the Parity Law across cloud webviews
- Codespaces architecture, devcontainer environments, and Webview extensions
- User experience standards, design tokens, typography, and zero-emoji compliance
- Release lifecycle, version tagging, licensing, and trademark policy

---

## 2. Decision Model

While discussion, feature suggestions, and community pull requests are welcome, Silis does not govern by committee:
- **Technical Proposals**: Proposals and pull requests are evaluated strictly on physical design validity, performance, and code hygiene.
- **Final Determination**: In any matter of design disagreement, roadmap priority, or architectural trade-off, the BDFL holds the definitive and final decision.
- **The Parity Law**: Cloud Webviews, REST APIs, and headless scripts must never deviate from the canonical desktop Silis SDK engine (`silis.core.sdk`).

---

## 3. Co-Developers & Maintainers

Silis is co-developed with **Eatheshwar**, **Venkat**, and **Adhil**. See [MAINTAINERS.md](MAINTAINERS.md) for details.

---

## 4. Scope of Repositories

The Silis ecosystem comprises two official repositories under the stewardship of the BDFL:
1. **[The-Silis-Foundation/silis](https://github.com/The-Silis-Foundation/silis)**: Desktop EDA core engine, native Qt6 application, Python SDK, and standalone AppImage packaging.
2. **[The-Silis-Foundation/silis-cs-edition](https://github.com/The-Silis-Foundation/silis-cs-edition)**: Cloud-native VS Code Web extension, devcontainer runtime, and multi-framebuffer / HTML5 web CAD hub.

---

## 5. Licensing & Commercial Rights

Silis CS Edition is licensed under the **GNU Affero General Public License v3.0 (AGPL-3.0)**.  
The Silis Foundation retains full rights for commercial dual-licensing, distribution agreements, and trademark protection as defined in [CONTRIBUTING.md](CONTRIBUTING.md).
