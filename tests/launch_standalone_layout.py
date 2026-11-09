import os
import sys
import subprocess

REPO_ROOT = "/home/eatheswar/silisfoundation/silis"
def_path = os.path.join(REPO_ROOT, "projects", "sram_soc_quad_core", "results", "temp.def")

cmd = f"""
import sys, os
sys.path.insert(0, '{REPO_ROOT}')
import qt_setup
import silis
from PyQt6.QtWidgets import QApplication

app = QApplication.instance() or QApplication(sys.argv)
win = silis.view_layout('{def_path}', title='SILIS Live Floorplan Viewer — SRAM Quad Core')
win.show()
sys.exit(app.exec())
"""

# Spawn in user's active X11 desktop display (:1)
env = os.environ.copy()
env["DISPLAY"] = ":1"
if "QT_QPA_PLATFORM" in env:
    del env["QT_QPA_PLATFORM"]

proc = subprocess.Popen([sys.executable, "-c", cmd], env=env)
print(f"[LAUNCHER] Standalone Layout Window launched! PID: {proc.pid}")
