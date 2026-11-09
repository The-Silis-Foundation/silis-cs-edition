#!/usr/bin/env bash
set -e

echo "================================================================="
echo "SILIS CS EDITION: SETTING UP CONTAINER DEPENDENCIES"
echo "================================================================="

# 1. Update package lists and install runtime packages
sudo apt-get update
sudo apt-get install -y --no-install-recommends \
    python3 \
    python3-minimal \
    python3-pip \
    python3-venv \
    xvfb \
    x11vnc \
    novnc \
    websockify \
    net-tools \
    python3-pyqt6 \
    python3-pyqt6.qsci \
    python3-pyqt6.qtwebengine \
    libgl1-mesa-dri \
    libgl1-mesa-glx \
    libosmesa6-dev \
    libtcl8.6 \
    libtk8.6

# Fix missing index.html in /usr/share/novnc if necessary
if [ -f "/usr/share/novnc/vnc.html" ] && [ ! -f "/usr/share/novnc/index.html" ]; then
    sudo ln -s /usr/share/novnc/vnc.html /usr/share/novnc/index.html
fi

# 2. Link system PyQt6 and libraries directly to active Python environment
PYTHON_BIN="$(which python3 || which python || echo "/usr/bin/python3")"
PIP_BIN="$(which pip3 || which pip || echo "$PYTHON_BIN -m pip")"

# Bridge system dist-packages (PyQt6, QtWebEngine) into the active python site-packages
ACTIVE_SITE="$($PYTHON_BIN -c "import site; print(site.getsitepackages()[0])" 2>/dev/null || echo "")"
if [ -n "$ACTIVE_SITE" ] && [ -d "/usr/lib/python3/dist-packages" ]; then
    echo "Bridging system PyQt6 packages to active Python site-packages ($ACTIVE_SITE)..."
    echo "/usr/lib/python3/dist-packages" | sudo tee "$ACTIVE_SITE/system_dist_packages.pth" > /dev/null
fi

# Verify PyQt6 import
if ! $PYTHON_BIN -c "import PyQt6" 2>/dev/null; then
    echo "Installing PyQt6 directly via pip into active python..."
    $PIP_BIN install --no-cache-dir PyQt6 PyQt6-QScintilla gdstk numpy websockify
else
    echo "PyQt6 successfully detected in active Python environment."
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
