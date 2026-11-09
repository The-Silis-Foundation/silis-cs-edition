import sys
import os

os.environ["DISPLAY"] = ":1"
os.environ["XAUTHORITY"] = "/run/user/1000/gdm/Xauthority"
if "QT_QPA_PLATFORM" in os.environ:
    del os.environ["QT_QPA_PLATFORM"]

REPO_ROOT = "/home/eatheswar/silisfoundation/silis"
sys.path.insert(0, REPO_ROOT)
import qt_setup
import silis
from PyQt6.QtWidgets import QApplication

app = QApplication(sys.argv)
def_file = "/home/eatheswar/silisfoundation/silis/projects/sram_soc_quad_pure_sdk/results/temp.def"
win = silis.view_layout(def_file, title="SILIS Live Layout Viewer")
win.resize(1100, 750)
win.show()
win.raise_()
win.activateWindow()
print("[GUI] Window shown on screen! Executing event loop...")
sys.exit(app.exec())
