#!/usr/bin/env bash
set -e

echo "================================================================="
echo "SILIS CS EDITION: CONTAINER SETUP"
echo "================================================================="

WORKSPACE_DIR="/workspaces/silis-cs-edition"
if [ ! -d "$WORKSPACE_DIR" ]; then
    WORKSPACE_DIR="$(pwd)"
fi

# 1. Install lightweight display and network servers
sudo apt-get update
sudo apt-get install -y --no-install-recommends \
    xvfb \
    x11vnc \
    novnc \
    websockify \
    net-tools \
    libgl1-mesa-dri \
    libgl1-mesa-glx \
    libosmesa6-dev \
    libtcl8.6 \
    libtk8.6 \
    wget

# Ensure novnc index.html link
if [ -f "/usr/share/novnc/vnc.html" ] && [ ! -f "/usr/share/novnc/index.html" ]; then
    sudo ln -s /usr/share/novnc/vnc.html /usr/share/novnc/index.html
fi

# 2. Extract or Link Dedicated Python + Qt Runtime AppImage
RUNTIME_APPIMAGE="${WORKSPACE_DIR}/dist/silis-python-qt-x86_64.AppImage"
OPT_DIR="/opt/silis/runtime"

if [ -f "$RUNTIME_APPIMAGE" ]; then
    echo "Found repository Python+Qt AppImage: $RUNTIME_APPIMAGE"
    if [ ! -d "$OPT_DIR" ]; then
        echo "Extracting Python+Qt AppImage into $OPT_DIR..."
        sudo mkdir -p /opt/silis
        cd /opt/silis
        sudo "$RUNTIME_APPIMAGE" --appimage-extract
        sudo mv squashfs-root runtime
    fi
elif [ -d "/opt/silis/runtime/opt/python" ]; then
    echo "Using existing /opt/silis/runtime"
else
    # Fallback to system PyQt6 packages
    sudo apt-get install -y --no-install-recommends python3 python3-pip python3-pyqt6 python3-pyqt6.qsci python3-pyqt6.qtwebengine
fi

# 3. Configure system-wide python symlinks to use the runtime container
if [ -d "/opt/silis/runtime/opt/python" ]; then
    sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/local/bin/python3
    sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/local/bin/python
    sudo ln -sf /opt/silis/runtime/opt/python/bin/pip3 /usr/local/bin/pip3
    sudo ln -sf /opt/silis/runtime/opt/python/bin/pip3 /usr/local/bin/pip
    sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/bin/python3 2>/dev/null || true
    sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/bin/python 2>/dev/null || true
fi

# 4. Install Silis CAD Workbench VS Code Extension locally
EXT_TARGET_DIR="/home/vscode/.vscode-server/extensions/the-silis-foundation.silis-cad-workbench-1.0.0"
if [ ! -d "/home/vscode/.vscode-server/extensions" ]; then
    mkdir -p "/home/vscode/.vscode-server/extensions"
fi
rm -rf "$EXT_TARGET_DIR"
cp -r "${WORKSPACE_DIR}/extension" "$EXT_TARGET_DIR"
chown -R vscode:vscode "/home/vscode/.vscode-server" 2>/dev/null || true

# 5. CLI symlinks in PATH
sudo ln -sf "${WORKSPACE_DIR}/bin/silis" /usr/local/bin/silis
sudo ln -sf "${WORKSPACE_DIR}/bin/silis" /usr/bin/silis

{
    echo "export PATH=\"${WORKSPACE_DIR}/bin:${WORKSPACE_DIR}/third-party/bin:\$PATH\""
    echo "export PYTHONPATH=\"${WORKSPACE_DIR}:${WORKSPACE_DIR}/core:${WORKSPACE_DIR}/third-party/siliconcompiler:\$PYTHONPATH\""
    echo 'export LIBGL_ALWAYS_SOFTWARE=1'
    echo 'export QT_QPA_PLATFORM=xcb'
} | sudo tee /etc/profile.d/silis_env.sh > /dev/null

echo "Setup completed successfully."
