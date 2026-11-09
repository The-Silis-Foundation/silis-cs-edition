#!/usr/bin/env bash
set -e

echo "================================================================="
echo "SILIS CS EDITION: DIRECT REPO RUNTIME ACTIVATION"
echo "================================================================="

WORKSPACE_DIR="/workspaces/silis-cs-edition"
if [ ! -d "$WORKSPACE_DIR" ]; then
    WORKSPACE_DIR="$(pwd)"
fi

# Ensure novnc index.html link
if [ -f "/usr/share/novnc/vnc.html" ] && [ ! -f "/usr/share/novnc/index.html" ]; then
    sudo ln -s /usr/share/novnc/vnc.html /usr/share/novnc/index.html
fi

# 1. Unpack AppImage committed directly in repo (dist/silis-python-qt-x86_64.AppImage)
OPT_DIR="/opt/silis/runtime"
REPO_APPIMAGE="${WORKSPACE_DIR}/dist/silis-python-qt-x86_64.AppImage"

# Guard: if the AppImage is a Git LFS pointer, pull the actual binary
if [ -f "$REPO_APPIMAGE" ] && grep -q "git-lfs.github.com" "$REPO_APPIMAGE" 2>/dev/null; then
    echo "LFS pointer detected -- pulling AppImage binary..."
    cd "$WORKSPACE_DIR" && git lfs pull --include="dist/silis-python-qt-x86_64.AppImage"
fi

if [ ! -d "$OPT_DIR" ]; then
    if [ -f "$REPO_APPIMAGE" ]; then
        echo "Extracting runtime from repo binary: $REPO_APPIMAGE..."
        sudo mkdir -p /opt/silis
        chmod +x "$REPO_APPIMAGE"
        (cd /opt/silis && sudo "$REPO_APPIMAGE" --appimage-extract && sudo mv squashfs-root runtime && sudo chmod -R a+rX /opt/silis)
    else
        echo "Error: Repo AppImage $REPO_APPIMAGE not found!"
        exit 1
    fi
fi

# 2. Configure system shared libraries and Python symlinks
if [ -d "/opt/silis/runtime/opt/python" ]; then
    echo "Activating /opt/silis/runtime..."
    sudo chmod -R a+rX /opt/silis
    sudo mkdir -p /etc/ld.so.conf.d
    echo "/opt/silis/runtime/usr/lib/x86_64-linux-gnu" | sudo tee /etc/ld.so.conf.d/silis_runtime.conf > /dev/null
    echo "/opt/silis/runtime/opt/python/lib" | sudo tee -a /etc/ld.so.conf.d/silis_runtime.conf > /dev/null
    sudo ldconfig 2>/dev/null || true

    sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/local/bin/python3
    sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/local/bin/python
    sudo ln -sf /opt/silis/runtime/opt/python/bin/pip3 /usr/local/bin/pip3
    sudo ln -sf /opt/silis/runtime/opt/python/bin/pip3 /usr/local/bin/pip
    sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/bin/python3 2>/dev/null || true
    sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/bin/python 2>/dev/null || true
    python3 -m pip install Pillow --no-warn-script-location 2>/dev/null || true
fi

# 3. Install Silis CAD Workbench VS Code Extension locally via symlink
for EXT_BASE in "/home/vscode/.vscode-server/extensions" "/home/vscode/.vscode-remote/extensions" "${HOME}/.vscode-server/extensions"; do
    if mkdir -p "$EXT_BASE" 2>/dev/null; then
        TARGET="${EXT_BASE}/the-silis-foundation.silis-cad-workbench-1.0.0"
        rm -rf "$TARGET"
        ln -sf "${WORKSPACE_DIR}/extension" "$TARGET" 2>/dev/null || true
    fi
done
chown -R vscode:vscode "/home/vscode/.vscode-server" "/home/vscode/.vscode-remote" 2>/dev/null || true

# 4. CLI symlinks in PATH
sudo ln -sf "${WORKSPACE_DIR}/bin/silis" /usr/local/bin/silis
sudo ln -sf "${WORKSPACE_DIR}/bin/silis" /usr/bin/silis

{
    echo "export PATH=\"/opt/silis/runtime/opt/python/bin:${WORKSPACE_DIR}/bin:${WORKSPACE_DIR}/third-party/bin:\$PATH\""
    echo "export PYTHONPATH=\"/opt/silis/runtime/opt/python/lib/python3.12/site-packages:${WORKSPACE_DIR}:${WORKSPACE_DIR}/core:${WORKSPACE_DIR}/third-party/siliconcompiler:\$PYTHONPATH\""
    echo "export LD_LIBRARY_PATH=\"/opt/silis/runtime/usr/lib/x86_64-linux-gnu:/opt/silis/runtime/opt/python/lib:\$LD_LIBRARY_PATH\""
    echo 'export LIBGL_ALWAYS_SOFTWARE=1'
    echo 'export QT_QPA_PLATFORM=xcb'
} | sudo tee /etc/profile.d/silis_env.sh > /dev/null

echo "Silis CS Edition setup completed directly from repository binary in seconds."
