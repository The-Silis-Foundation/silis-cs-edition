#!/usr/bin/env bash

echo "================================================================="
echo "SILIS CS EDITION: LAUNCHING MULTI-FRAMEBUFFER CAD DAEMONS"
echo "================================================================="

WORKSPACE_DIR="/workspaces/silis-cs-edition"
if [ ! -d "$WORKSPACE_DIR" ]; then
    WORKSPACE_DIR="$(pwd)"
fi

export PATH="${WORKSPACE_DIR}/third-party/bin:$PATH"
export PYTHONPATH="${WORKSPACE_DIR}:${WORKSPACE_DIR}/core:${WORKSPACE_DIR}/third-party/siliconcompiler:$PYTHONPATH"
export LIBGL_ALWAYS_SOFTWARE=1
export QT_QPA_PLATFORM=xcb

# Locate novnc web directory
NOVNC_DIR="/usr/share/novnc"
if [ ! -d "$NOVNC_DIR" ]; then
    NOVNC_DIR="/usr/share/novnc-core"
fi

# Function to launch Xvfb, x11vnc, and websockify for a specific display
start_display_pipeline() {
    local disp=$1
    local vnc_port=$2
    local web_port=$3
    local title=$4

    echo "[Silis CS] Starting Display :${disp} (VNC:${vnc_port} -> Web:${web_port}) - ${title}"
    
    # Kill any stale Xvfb on this display (target only the specific display to avoid racing other pipelines)
    pkill -f "Xvfb :${disp}" 2>/dev/null || true
    sudo pkill -f "Xvfb :${disp}" 2>/dev/null || true
    rm -f /tmp/.X${disp}-lock /tmp/.X11-unix/X${disp} 2>/dev/null || true

    Xvfb :${disp} -screen 0 1600x1000x24 -ac +extension GLX +render -noreset &
    sleep 1

    x11vnc -display :${disp} -forever -nopw -shared -rfbport ${vnc_port} -quiet &
    sleep 1

    # Run websockify bound to 0.0.0.0
    websockify --web="${NOVNC_DIR}" 0.0.0.0:${web_port} localhost:${vnc_port} &
}

# 0. Start Native Web CAD Server (Schematic & Waveforms) on Port 8765
python3 "${WORKSPACE_DIR}/core/server/cs_server.py" 8765 > /tmp/cs_cad_server.log 2>&1 &

# 1. Initialize Framebuffers
start_display_pipeline 1 5901 6081 "FastSiliconPeeker 2D Layout"
start_display_pipeline 2 5902 6082 "Frontend Studio & SignalPeeker Waveforms"
start_display_pipeline 4 5904 6084 "Silis Floorplan & Power Studio (FPS)"

sleep 2

# 2. Launch Silis Floorplan & Power Studio (FPS) on Display :4
(
    export DISPLAY=:4
    export PYTHONPATH="${WORKSPACE_DIR}:${WORKSPACE_DIR}/core:${WORKSPACE_DIR}/third-party/siliconcompiler:$PYTHONPATH"
    cd "${WORKSPACE_DIR}"
    python3 -c "
import sys, os
sys.path.insert(0, os.path.join('${WORKSPACE_DIR}', 'core'))
sys.path.insert(0, '${WORKSPACE_DIR}')
from PyQt6.QtWidgets import QApplication, QWidget
from backendflow.flow.backendflow import BackendWidget

app = QApplication(sys.argv)
class MockIDE(QWidget):
    def __init__(self):
        super().__init__()
        self.headless = False
        self.cwd = os.path.join('${WORKSPACE_DIR}', 'projects', 'demo_soc')
        self.project_config = {'pdk': 'sky130'}
        self.pdk_mgr = None
        self.active_pdk = 'sky130'
        self.terminal = None
        self.proj_mgr = None
    def get_proj_root(self, ctx):
        return os.path.join('${WORKSPACE_DIR}', 'projects', 'demo_soc')
    def get_context(self):
        return ('demo_soc', None)

mock = MockIDE()
widget = BackendWidget(mock)
widget.resize(1600, 1000)
widget.showMaximized()
sys.exit(app.exec())
" &
) > /tmp/silis_fps.log 2>&1 &

# 3. Launch FastSiliconPeeker with initial layout on Display :1
if [ -f "${WORKSPACE_DIR}/results/temp.def" ]; then
    (
        export DISPLAY=:1
        export PYTHONPATH="${WORKSPACE_DIR}:${WORKSPACE_DIR}/core:${WORKSPACE_DIR}/third-party/siliconcompiler:$PYTHONPATH"
        python3 -c "
import sys, os
from PyQt6.QtWidgets import QApplication
from backendflow.siliconpeeker.peeker import FastSiliconPeeker

app = QApplication(sys.argv)
peeker = FastSiliconPeeker()
peeker.load_def_file('${WORKSPACE_DIR}/results/temp.def')
peeker.resize(1600, 1000)
peeker.showMaximized()
sys.exit(app.exec())
" &
    ) > /tmp/silis_peeker.log 2>&1 &
fi

# 4. Background DEF File Watcher
(
    while true; do
        if [ -f "${WORKSPACE_DIR}/results/temp.def" ] && [ -s "${WORKSPACE_DIR}/results/temp.def" ]; then
            DEF_FILE="${WORKSPACE_DIR}/results/temp.def"
            if [ ! -f "/tmp/.def_opened" ] || [ "$DEF_FILE" -nt "/tmp/.def_opened" ]; then
                touch "/tmp/.def_opened"
                echo "[Silis CS Watcher] Updated layout detected: ${DEF_FILE}"
                (
                    export DISPLAY=:1
                    export PYTHONPATH="${WORKSPACE_DIR}:${WORKSPACE_DIR}/core:${WORKSPACE_DIR}/third-party/siliconcompiler:$PYTHONPATH"
                    python3 -c "
import sys, os
from PyQt6.QtWidgets import QApplication
from backendflow.siliconpeeker.peeker import FastSiliconPeeker

app = QApplication(sys.argv)
peeker = FastSiliconPeeker()
peeker.load_def_file('${DEF_FILE}')
peeker.resize(1600, 1000)
peeker.showMaximized()
sys.exit(app.exec())
" &
                ) > /tmp/silis_peeker.log 2>&1 &
            fi
        fi
        sleep 2
    done
) &

# 5. Generate and refresh workspace README dashboard
if [ -f "${WORKSPACE_DIR}/tools/gen_readme.py" ]; then
    python3 "${WORKSPACE_DIR}/tools/gen_readme.py" 2>/dev/null || true
fi

echo "================================================================="
echo "SILIS CS EDITION DAEMONS RUNNING"
echo "  * Port 8765: http://localhost:8765/ (Native Schematic & ROHD Web)"
echo "  * Port 6081: http://localhost:6081/vnc.html?autoconnect=true&resize=remote (FastSiliconPeeker Layout)"
echo "  * Port 6084: http://localhost:6084/vnc.html?autoconnect=true&resize=remote (Floorplan & Power Studio)"
echo "================================================================="
