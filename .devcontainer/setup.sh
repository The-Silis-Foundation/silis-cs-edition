#!/usr/bin/env bash
set -e

echo "================================================================="
echo "SILIS CS EDITION: PROVISIONING MULTI-FRAMEBUFFER CAD PIPELINES"
echo "================================================================="

# 1. Update package lists and install runtime packages
sudo apt-get update
sudo apt-get install -y --no-install-recommends \
    xvfb \
    x11vnc \
    novnc \
    websockify \
    python3-pip \
    python3-pyqt6 \
    python3-pyqt6.qsci \
    python3-pyqt6.qtwebengine \
    libgl1-mesa-dri \
    libgl1-mesa-glx \
    libosmesa6-dev \
    libtcl8.6 \
    libtk8.6 \
    inotify-tools

# 2. Install Python runtime dependencies
pip3 install --no-cache-dir --break-system-packages \
    gdstk \
    numpy

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

export PATH="${WORKSPACE_DIR}/third-party/bin:$PATH"
export PYTHONPATH="${WORKSPACE_DIR}:${WORKSPACE_DIR}/core:${WORKSPACE_DIR}/third-party/siliconcompiler:$PYTHONPATH"
export LIBGL_ALWAYS_SOFTWARE=1
export QT_QPA_PLATFORM=xcb

# 4. Multi-Display Daemon Launcher Function
start_display_pipeline() {
    local disp=$1
    local vnc_port=$2
    local web_port=$3
    local title=$4

    echo "[Silis CS] Initializing Display :${disp} on VNC:${vnc_port} -> Web:${web_port} (${title})"
    Xvfb :${disp} -screen 0 1600x1000x24 -ac +extension GLX +render -noreset &
    sleep 1
    x11vnc -display :${disp} -forever -nopw -shared -rfbport ${vnc_port} -quiet &
    websockify --web=/usr/share/novnc/ ${web_port} localhost:${vnc_port} &
}

# 5. Initialize the three dedicated framebuffers
# Display :1 -> Port 6081: FastSiliconPeeker (2D Layout)
start_display_pipeline 1 5901 6081 "FastSiliconPeeker 2D Layout"

# Display :2 -> Port 6082: Frontend Studio & SignalPeeker Waveform
start_display_pipeline 2 5902 6082 "Frontend Studio & SignalPeeker Waveforms"

# Display :4 -> Port 6084: Silis Floorplan & Power Studio (FPS)
start_display_pipeline 4 5904 6084 "Silis Floorplan & Power Studio"

# 6. Auto-start persistent Floorplan Studio on Display :4
(
    export DISPLAY=:4
    cd "${WORKSPACE_DIR}"
    python3 -c "
import sys, os
from PyQt6.QtWidgets import QApplication
from silis.core.backendflow.flow.backendflow import BackendWidget

app = QApplication(sys.argv)
class MockIDE:
    def __init__(self):
        self.headless = False
        self.cwd = os.getcwd()
        self.project_config = {}
        self.pdk_mgr = None
        self.active_pdk = 'sky130'
        self.terminal = None
        self.proj_mgr = None
    def get_proj_root(self, ctx):
        return os.getcwd()
    def get_context(self):
        return ('cs_design', None)

mock = MockIDE()
widget = BackendWidget(mock)
widget.resize(1600, 1000)
widget.showMaximized()
sys.exit(app.exec())
" &
) > /tmp/silis_fps.log 2>&1 &

# 7. Background Watcher: Auto-open 2D Layout on Port 6081 when temp.def is created
(
    while true; do
        if [ -f "${WORKSPACE_DIR}/results/temp.def" ] && [ -s "${WORKSPACE_DIR}/results/temp.def" ]; then
            DEF_FILE="${WORKSPACE_DIR}/results/temp.def"
            if [ ! -f "/tmp/.def_opened" ] || [ "$DEF_FILE" -nt "/tmp/.def_opened" ]; then
                touch "/tmp/.def_opened"
                echo "[Silis CS Watcher] New layout detected at ${DEF_FILE}, launching FastSiliconPeeker on Display :1..."
                (
                    export DISPLAY=:1
                    python3 -c "
import sys, os
from PyQt6.QtWidgets import QApplication
from silis.core.backendflow.siliconpeeker.peeker import FastSiliconPeeker

app = QApplication.instance() or QApplication(sys.argv)
peeker = FastSiliconPeeker()
peeker.load_def_file('${DEF_FILE}')
peeker.resize(1600, 1000)
peeker.showMaximized()
app.exec()
" &
                ) > /tmp/silis_peeker.log 2>&1 &
                
                # Signal VS Code Web to open Port 6081 in side tab if CLI available
                if command -v code &> /dev/null; then
                    code --command "simpleBrowser.show" "http://localhost:6081/vnc.html?autoconnect=true&resize=remote" || true
                fi
            fi
        fi
        sleep 2
    done
) &

echo "================================================================="
echo "SILIS CS EDITION READY: ALL DEDICATED FRAMEBUFFERS ONLINE"
echo "  • Port 6081: FastSiliconPeeker 2D Layout"
echo "  • Port 6082: Frontend Studio & Waveform Viewer"
echo "  • Port 6084: Floorplan & Power Studio (FPS)"
echo "================================================================="
