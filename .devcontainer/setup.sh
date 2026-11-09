#!/usr/bin/env bash
set -e

echo "================================================================="
echo "SILIS CS EDITION: SETTING UP CONTAINER DEPENDENCIES"
echo "================================================================="

# 1. Update package lists and install minimal display and X11/VNC server tools
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
    wget \
    zsync

# Fix missing index.html in /usr/share/novnc if necessary
if [ -f "/usr/share/novnc/vnc.html" ] && [ ! -f "/usr/share/novnc/index.html" ]; then
    sudo ln -s /usr/share/novnc/vnc.html /usr/share/novnc/index.html
fi

# 2. Provision Standalone AppImage as the Python & Qt Runtime Container
APPIMAGE_DIR="/opt/silis"
APPIMAGE_BIN="${APPIMAGE_DIR}/Silis-x86_64.AppImage"
sudo mkdir -p "${APPIMAGE_DIR}"

if [ ! -f "${APPIMAGE_BIN}" ]; then
    echo "Downloading standalone Silis AppImage runtime (Python 3.12 + Qt6 + PDK)..."
    sudo wget -q --show-progress \
        "https://github.com/The-Silis-Foundation/silis/releases/latest/download/Silis-x86_64.AppImage" \
        -O "${APPIMAGE_BIN}" || true
    if [ -f "${APPIMAGE_BIN}" ]; then
        sudo chmod +x "${APPIMAGE_BIN}"
        echo "Extracting AppImage into /opt/silis/runtime for instant access..."
        (cd "${APPIMAGE_DIR}" && sudo "${APPIMAGE_BIN}" --appimage-extract && sudo mv squashfs-root runtime) || true
    fi
fi

# 3. Configure Python environment: Link embedded AppImage Python/Qt or bridge system packages
if [ -d "/opt/silis/runtime/opt/python" ]; then
    echo "Configuring /opt/silis embedded Python 3.12 as default..."
    sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/local/bin/python3
    sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/local/bin/python
    sudo ln -sf /opt/silis/runtime/opt/python/bin/pip3 /usr/local/bin/pip3
    sudo ln -sf /opt/silis/runtime/opt/python/bin/pip3 /usr/local/bin/pip
    PYTHON_BIN="/opt/silis/runtime/opt/python/bin/python3"
else
    # Fallback to system apt packages if AppImage download is bypassed
    sudo apt-get install -y --no-install-recommends python3 python3-pyqt6 python3-pyqt6.qsci python3-pyqt6.qtwebengine
    PYTHON_BIN="$(which python3 || echo "/usr/bin/python3")"
    PIP_BIN="$(which pip3 || echo "$PYTHON_BIN -m pip")"
    ACTIVE_SITE="$($PYTHON_BIN -c "import site; print(site.getsitepackages()[0])" 2>/dev/null || echo "")"
    if [ -n "$ACTIVE_SITE" ] && [ -d "/usr/lib/python3/dist-packages" ]; then
        echo "/usr/lib/python3/dist-packages" | sudo tee "$ACTIVE_SITE/system_dist_packages.pth" > /dev/null
    fi
    $PIP_BIN install --no-cache-dir gdstk numpy websockify || true
fi

# 3. Install Silis CAD Workbench VS Code Extension locally
EXT_TARGET_DIR="/home/vscode/.vscode-server/extensions/the-silis-foundation.silis-cad-workbench-1.0.0"
if [ ! -d "/home/vscode/.vscode-server/extensions" ]; then
    mkdir -p "/home/vscode/.vscode-server/extensions"
fi
rm -rf "$EXT_TARGET_DIR"
cp -r "${WORKSPACE_DIR}/extension" "$EXT_TARGET_DIR"
chown -R vscode:vscode "/home/vscode/.vscode-server" 2>/dev/null || true

# 4. Environment configuration & system-wide symlinks
WORKSPACE_DIR="/workspaces/silis-cs-edition"
if [ ! -d "$WORKSPACE_DIR" ]; then
    WORKSPACE_DIR="$(pwd)"
fi

# Ensure /usr/bin/silis symlink is always available in PATH
sudo ln -sf "${WORKSPACE_DIR}/bin/silis" /usr/local/bin/silis
sudo ln -sf "${WORKSPACE_DIR}/bin/silis" /usr/bin/silis

# Ensure python3 and python symlinks are present in both /usr/bin and /usr/local/bin
PYTHON_EXECUTABLE="$(command -v python3 || command -v python || echo "")"
if [ -n "$PYTHON_EXECUTABLE" ]; then
    sudo ln -sf "$PYTHON_EXECUTABLE" /usr/bin/python3
    sudo ln -sf "$PYTHON_EXECUTABLE" /usr/bin/python
    sudo ln -sf "$PYTHON_EXECUTABLE" /usr/local/bin/python3
    sudo ln -sf "$PYTHON_EXECUTABLE" /usr/local/bin/python
fi

{
    echo "export PATH=\"${WORKSPACE_DIR}/bin:${WORKSPACE_DIR}/third-party/bin:\$PATH\""
    echo "export PYTHONPATH=\"${WORKSPACE_DIR}:${WORKSPACE_DIR}/core:${WORKSPACE_DIR}/third-party/siliconcompiler:\$PYTHONPATH\""
    echo 'export LIBGL_ALWAYS_SOFTWARE=1'
    echo 'export QT_QPA_PLATFORM=xcb'
} | sudo tee /etc/profile.d/silis_env.sh > /dev/null

echo "Setup completed successfully."
