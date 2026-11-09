#!/usr/bin/env bash
set -e

echo "================================================================="
echo "SILIS CS EDITION: RAPID CONTAINER PROVISIONING"
echo "================================================================="

WORKSPACE_DIR="/workspaces/silis-cs-edition"
if [ ! -d "$WORKSPACE_DIR" ]; then
    WORKSPACE_DIR="$(pwd)"
fi

# Ensure novnc index.html link
if [ -f "/usr/share/novnc/vnc.html" ] && [ ! -f "/usr/share/novnc/index.html" ]; then
    sudo ln -s /usr/share/novnc/vnc.html /usr/share/novnc/index.html
fi

# 1. Fetch and unpack Dedicated Python + Qt Runtime AppImage into /opt/silis/runtime
OPT_DIR="/opt/silis/runtime"
RUNTIME_APPIMAGE="/tmp/silis-python-qt-x86_64.AppImage"

if [ ! -d "$OPT_DIR" ]; then
    echo "Downloading 270MB Standalone Python+Qt AppImage from release..."
    sudo mkdir -p /opt/silis
    sudo wget -q --show-progress \
        "https://github.com/The-Silis-Foundation/silis-cs-edition/releases/download/v1.0.0-runtime/silis-python-qt-x86_64.AppImage" \
        -O "$RUNTIME_APPIMAGE"
    sudo chmod +x "$RUNTIME_APPIMAGE"
    echo "Extracting AppImage into $OPT_DIR..."
    (cd /opt/silis && sudo "$RUNTIME_APPIMAGE" --appimage-extract && sudo mv squashfs-root runtime)
    sudo rm -f "$RUNTIME_APPIMAGE"
fi

# 2. Configure system-wide python symlinks to use the pre-baked AppImage runtime
if [ -d "/opt/silis/runtime/opt/python" ]; then
    echo "Configuring /opt/silis/runtime/opt/python as active system Python 3.12..."
    sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/local/bin/python3
    sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/local/bin/python
    sudo ln -sf /opt/silis/runtime/opt/python/bin/pip3 /usr/local/bin/pip3
    sudo ln -sf /opt/silis/runtime/opt/python/bin/pip3 /usr/local/bin/pip
    sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/bin/python3 2>/dev/null || true
    sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/bin/python 2>/dev/null || true
fi

# 3. Install Silis CAD Workbench VS Code Extension locally
EXT_TARGET_DIR="/home/vscode/.vscode-server/extensions/the-silis-foundation.silis-cad-workbench-1.0.0"
if [ ! -d "/home/vscode/.vscode-server/extensions" ]; then
    mkdir -p "/home/vscode/.vscode-server/extensions"
fi
rm -rf "$EXT_TARGET_DIR"
cp -r "${WORKSPACE_DIR}/extension" "$EXT_TARGET_DIR"
chown -R vscode:vscode "/home/vscode/.vscode-server" 2>/dev/null || true

# 4. CLI symlinks in PATH
sudo ln -sf "${WORKSPACE_DIR}/bin/silis" /usr/local/bin/silis
sudo ln -sf "${WORKSPACE_DIR}/bin/silis" /usr/bin/silis

{
    echo "export PATH=\"/opt/silis/runtime/opt/python/bin:${WORKSPACE_DIR}/bin:${WORKSPACE_DIR}/third-party/bin:\$PATH\""
    echo "export PYTHONPATH=\"/opt/silis/runtime/opt/python/lib/python3.12/site-packages:${WORKSPACE_DIR}:${WORKSPACE_DIR}/core:${WORKSPACE_DIR}/third-party/siliconcompiler:\$PYTHONPATH\""
    echo 'export LIBGL_ALWAYS_SOFTWARE=1'
    echo 'export QT_QPA_PLATFORM=xcb'
} | sudo tee /etc/profile.d/silis_env.sh > /dev/null

echo "Silis CS Edition setup completed in seconds."
