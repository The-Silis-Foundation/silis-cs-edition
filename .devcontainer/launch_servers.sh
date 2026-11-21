#!/usr/bin/env bash

WORKSPACE_DIR="/workspaces/silis-cs-edition"
if [ ! -d "$WORKSPACE_DIR" ]; then
    WORKSPACE_DIR="$(pwd)"
fi

export PATH="${WORKSPACE_DIR}/assets/bin:${WORKSPACE_DIR}/silis/third-party/bin:$PATH"
export PYTHONPATH="${WORKSPACE_DIR}/silis:${WORKSPACE_DIR}/silis/core:${WORKSPACE_DIR}/silis/third-party/siliconcompiler:$PYTHONPATH"
export LIBGL_ALWAYS_SOFTWARE=1
export QT_QPA_PLATFORM=xcb
export DISPLAY=:1

# Ensure headless X11 display and VNC bridge are running for native CAD viewers (zero daemon ports)
if command -v Xvfb >/dev/null 2>&1 && ! pgrep -f "Xvfb :1" >/dev/null 2>&1; then
    Xvfb :1 -screen 0 1920x1080x24 -ac +extension GLX +render -noreset >/dev/null 2>&1 &
fi
if command -v x11vnc >/dev/null 2>&1 && ! pgrep -f "x11vnc.*5901" >/dev/null 2>&1; then
    x11vnc -display :1 -listen 127.0.0.1 -rfbport 5901 -nopw -shared -forever -bg >/dev/null 2>&1 || true
fi

# Ensure extension is installed in active VS Code server session
VSIX_PATH="${WORKSPACE_DIR}/extension/silis-cad-workbench-1.0.0.vsix"
if command -v code >/dev/null 2>&1 && [ -f "$VSIX_PATH" ]; then
    code --install-extension "$VSIX_PATH" --force 2>/dev/null || true
fi

echo "Silis CS Edition runtime environment initialized. CAD viewers run client-side with zero daemon ports."
