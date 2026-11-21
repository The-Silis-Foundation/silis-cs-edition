---
name: ui-state-verifier
description: Headless Qt/desktop UI test automation, event loop management, modal dialog suppression, widget state machine validation, and visual snapshot auditing.
---

# Native Qt6 UI Automation & Headless Verification

## 1. Headless Environment Invariants
- Whenever running automated tests, CI pipelines, or headless SDK flows:
  ```python
  import os
  os.environ["SILIS_HEADLESS_TEST"] = "1"
  os.environ["QT_QPA_PLATFORM"] = "offscreen"
  ```
- Any modal popup (`QMessageBox.exec()`, `QFileDialog.getOpenFileName()`) will block the process indefinitely if not suppressed.
  - In headless test harness, monkey-patch blocking modal dialogs to return predefined default responses (`QMessageBox.exec = lambda self: QMessageBox.StandardButton.Ok`).

## 2. Event Loop Management & Non-Blocking Execution
- Never use blocking sleep calls (`time.sleep()`) inside the Qt GUI main thread.
- When waiting for asynchronous worker threads or subprocesses (`QProcess`), pump the event loop:
  ```python
  from PyQt6.QtCore import QCoreApplication
  # Process pending events without blocking the GUI thread
  QCoreApplication.processEvents()
  ```
- Use `qtbot.waitSignal()` or non-blocking polling loops with timeout assertions to prevent zombie test runs.

## 3. Pure Native Semantic Conventions
- **Absolute Emoji Ban**: Zero emojis in dialog titles, buttons, status bars, tooltips, or logs.
- Use native `QPalette` semantic color roles (`Window`, `Base`, `Text`, `Highlight`) rather than hardcoded neon hex codes to ensure clean rendering across desktop environments (GNOME, KDE, XFCE).
- Display physical coordinates, voltages, and instance counts using system monospace typography (`QFontDatabase.SystemFont.FixedFont`).
