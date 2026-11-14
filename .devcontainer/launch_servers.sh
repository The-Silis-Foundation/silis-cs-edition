#!/usr/bin/env bash

WORKSPACE_DIR="/workspaces/silis-cs-edition"
if [ ! -d "$WORKSPACE_DIR" ]; then
    WORKSPACE_DIR="$(pwd)"
fi

export PATH="${WORKSPACE_DIR}/assets/bin:${WORKSPACE_DIR}/silis/third-party/bin:$PATH"
export PYTHONPATH="${WORKSPACE_DIR}/silis:${WORKSPACE_DIR}/silis/core:${WORKSPACE_DIR}/silis/third-party/siliconcompiler:$PYTHONPATH"
export LIBGL_ALWAYS_SOFTWARE=1
export QT_QPA_PLATFORM=xcb

# Ensure extension is installed in active VS Code server session
VSIX_PATH="${WORKSPACE_DIR}/extension/silis-cad-workbench-1.0.0.vsix"
if command -v code >/dev/null 2>&1 && [ -f "$VSIX_PATH" ]; then
    code --install-extension "$VSIX_PATH" --force 2>/dev/null || true
fi

# Streamline server launch to start python3 tools/cs_server.py 8765 in background with logging to /tmp/cs_cad_server.log
pkill -f "cs_server.py" 2>/dev/null || true
nohup python3 "${WORKSPACE_DIR}/tools/cs_server.py" 8765 > /tmp/cs_cad_server.log 2>&1 &
disown

echo "Silis CS server started on port 8765. Logs at /tmp/cs_cad_server.log."
