# Contributing to Silis — Codespaces Edition

Thank you for your interest in contributing to **Silis CS Edition**.

Silis is developed and maintained by **The Silis Foundation**.  
Before contributing to the Codespaces edition, please read this document carefully.

---

## 1. License & Contributor Agreement

### License
Silis CS Edition is licensed under the **GNU Affero General Public License v3.0 (AGPL-3.0)**.  
All contributions to this repository are licensed under AGPL-3.0.

### Contributor Assignment & Rights
By submitting a pull request, issue, patch, or any contribution to this repository, you agree to the following:
1. You certify that you have the legal right to submit the contribution.
2. You grant The Silis Foundation a perpetual, worldwide, non-exclusive, irrevocable license to use, modify, distribute, and sublicense your contribution.
3. You agree that The Silis Foundation may:
   - Commercially license Silis separately.
   - Dual-license Silis under alternative commercial or open-source terms.
   - Use your contribution in commercial and academic agreements.

This allows the Foundation to protect the project's sustainability while preserving copyleft guarantees for the public community edition.

---

## 2. Engineering Architecture & The Parity Law

### Zero GUI-SDK Deviance
The Silis Codespaces Edition (Webviews, HTML5 canvases, and REST APIs) must never implement divergent physical design mathematics or file export logic from the core Silis Python SDK (`silis.core.sdk`).
- **Single Source of Truth**: Macro legalization, halo clearance calculations, site-row quantization, and IEEE 1801 UPF syntax generation must match the desktop engine down to the nanometer.
- **Pre-Flight Invariants**: Any coordinate emitted by the Codespaces edition must satisfy manufacturing grid quantization ($x \pmod{0.005} = 0, y \pmod{0.005} = 0$).

### Code Conventions & UI Standards
- **Absolute Emoji Ban**: Do not use emojis in UI titles, button labels, logs, status bar messages, dialogs, tooltips, or commit messages.
- **Physical Units**: Always specify engineering units explicitly (`µm`, `DBU`, `V`, `ps`, `ns`, `µm²`).
- **Typography**: Numerical coordinates, voltages, and memory counts must use monospace formatting.

---

## 3. Local Development & Verification Workflow

### Dev Container Environment
The recommended development environment is GitHub Codespaces or a local VS Code Dev Container using the configuration in `.devcontainer/devcontainer.json`.

```bash
# Verify backend web hub and server endpoints:
python3 tools/verify_cs_edition.py

# Test headless physical design flow:
python3 tools/flow.py

# Update dynamic project registry and README dashboard:
python3 tools/gen_readme.py
```

### VS Code Extension Development
The webview extension source resides in `extension/`. To verify and compile changes:
```bash
cd extension
# Verify extension package integrity
node -e "require('./extension.js'); console.log('Extension syntax valid');"
```

---

## 4. Governance Model

Developed by **Jerome Antony Robin**, this repository follows a **BDFL Governance model**.  
Co-developed by **Eatheshwar**, **Venkat**, and **Adhil**.

Project direction, architectural decisions, and public representations are stewarded by the Founder and BDFL, **Jerome Antony Robin**. *Silis took 11 months from POC to pre-release.*

Contributing code does **not** grant governance rights, project ownership, or trademark rights over the names "Silis", "Silis CS Edition", "Silicon Scaffold", or "The Silis Foundation".

---

## 5. Trademarks & Identity

The names **Silis**, **Silis CS Edition**, **Silicon Scaffold**, and **The Silis Foundation** represent the identity of this project.  
Forks and derived works must comply with [TRADEMARK.md](TRADEMARK.md) and must not imply official affiliation unless explicitly authorized.

---

## 6. Code of Conduct

- Be professional.
- Be technical.
- Deliver evidence-backed code and test verification.
- Zero drama, zero ownership disputes.

---

By submitting a pull request or issue, you acknowledge that you have read and agreed to this document.
