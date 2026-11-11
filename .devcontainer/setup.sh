#!/usr/bin/env bash
set -e

echo "================================================================="
echo "SILIS CS EDITION: CONTAINER PROVISIONING & SETUP"
echo "================================================================="

WORKSPACE_DIR="/workspaces/silis-cs-edition"
if [ ! -d "$WORKSPACE_DIR" ]; then
    WORKSPACE_DIR="$(pwd)"
fi

# 1. Initialize Git Submodules (Silis core)
echo "Initializing Silis submodules..."
cd "$WORKSPACE_DIR"
git submodule update --init silis 2>/dev/null || true

# 2. Ensure novnc index.html link
if [ -f "/usr/share/novnc/vnc.html" ] && [ ! -f "/usr/share/novnc/index.html" ]; then
    sudo ln -sf /usr/share/novnc/vnc.html /usr/share/novnc/index.html
fi

# 3. Unpack Python + Qt6 Standalone Runtime
OPT_DIR="/opt/silis/runtime"
RUNTIME_APPIMAGE="/tmp/silis-python-qt-x86_64.AppImage"

if [ ! -d "$OPT_DIR" ]; then
    echo "Downloading 270MB Standalone Python+Qt AppImage from release..."
    sudo mkdir -p /opt/silis
    if [ -f "${WORKSPACE_DIR}/dist/silis-python-qt-x86_64.AppImage" ] && [ -s "${WORKSPACE_DIR}/dist/silis-python-qt-x86_64.AppImage" ]; then
        sudo cp "${WORKSPACE_DIR}/dist/silis-python-qt-x86_64.AppImage" "$RUNTIME_APPIMAGE"
    else
        sudo wget -q \
            "https://github.com/The-Silis-Foundation/silis-cs-edition/releases/download/v1.0.0-runtime/silis-python-qt-x86_64.AppImage" \
            -O "$RUNTIME_APPIMAGE"
    fi
    sudo chmod +x "$RUNTIME_APPIMAGE"
    echo "Extracting runtime into $OPT_DIR..."
    (cd /opt/silis && sudo "$RUNTIME_APPIMAGE" --appimage-extract && sudo mv squashfs-root runtime && sudo chmod -R a+rX /opt/silis)
    sudo rm -f "$RUNTIME_APPIMAGE"
fi

# 4. Configure system shared libraries and Python symlinks
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

# 5. Install Silis CAD Workbench VS Code Extension locally
for EXT_BASE in "/home/vscode/.vscode-server/extensions" "/home/vscode/.vscode-remote/extensions" "${HOME}/.vscode-server/extensions" "${HOME}/.vscode-remote/extensions"; do
    if sudo mkdir -p "$EXT_BASE" 2>/dev/null; then
        TARGET="${EXT_BASE}/the-silis-foundation.silis-cad-workbench-1.0.0"
        sudo rm -rf "$TARGET"
        sudo cp -r "${WORKSPACE_DIR}/extension" "$TARGET" 2>/dev/null || true
        sudo chown -R vscode:vscode "$EXT_BASE" 2>/dev/null || true
    fi
done

# 6. CLI symlinks in PATH
sudo ln -sf "${WORKSPACE_DIR}/assets/bin/silis" /usr/local/bin/silis 2>/dev/null || true
sudo ln -sf "${WORKSPACE_DIR}/assets/bin/silis" /usr/bin/silis 2>/dev/null || true

{
    echo "export PATH=\"/opt/silis/runtime/opt/python/bin:${WORKSPACE_DIR}/assets/bin:${WORKSPACE_DIR}/silis/third-party/bin:\$PATH\""
    echo "export PYTHONPATH=\"/opt/silis/runtime/opt/python/lib/python3.12/site-packages:${WORKSPACE_DIR}:${WORKSPACE_DIR}/silis:${WORKSPACE_DIR}/silis/core:${WORKSPACE_DIR}/silis/third-party/siliconcompiler:\$PYTHONPATH\""
    echo "export LD_LIBRARY_PATH=\"/opt/silis/runtime/usr/lib/x86_64-linux-gnu:/opt/silis/runtime/opt/python/lib:\$LD_LIBRARY_PATH\""
    echo 'export LIBGL_ALWAYS_SOFTWARE=1'
    echo 'export QT_QPA_PLATFORM=xcb'
} | sudo tee /etc/profile.d/silis_env.sh > /dev/null

echo "Silis CS Edition setup completed successfully."
