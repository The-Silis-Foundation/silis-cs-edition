#!/usr/bin/env bash
set -e

echo "================================================================="
echo "SILIS CS EDITION: SETTING UP CONTAINER DEPENDENCIES"
echo "================================================================="

# 1. Update package lists and install runtime packages
sudo apt-get update
sudo apt-get install -y --no-install-recommends \
    xvfb \
    x11vnc \
    novnc \
    websockify \
    net-tools \
    python3-pip \
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

# 2. Install Python runtime dependencies
pip3 install --no-cache-dir --break-system-packages \
    gdstk \
    numpy \
    websockify

# 3. Environment configuration
WORKSPACE_DIR="/workspaces/silis-cs-edition"
if [ ! -d "$WORKSPACE_DIR" ]; then
    WORKSPACE_DIR="$(pwd)"
fi

{
    echo "export PATH=\"${WORKSPACE_DIR}/third-party/bin:\$PATH\""
    echo "export PYTHONPATH=\"${WORKSPACE_DIR}:${WORKSPACE_DIR}/core:${WORKSPACE_DIR}/third-party/siliconcompiler:\$PYTHONPATH\""
    echo 'export LIBGL_ALWAYS_SOFTWARE=1'
    echo 'export QT_QPA_PLATFORM=xcb'
} | sudo tee /etc/profile.d/silis_env.sh > /dev/null

echo "Setup completed successfully."
