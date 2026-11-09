import sys
import os

os.environ["DISPLAY"] = ":1"
os.environ["XAUTHORITY"] = "/run/user/1000/gdm/Xauthority"
os.environ["QT_QPA_PLATFORM"] = "xcb"

REPO_ROOT = "/home/eatheswar/silisfoundation/silis"
sys.path.insert(0, REPO_ROOT)
import qt_setup
import silis
from PyQt6.QtWidgets import QApplication

app = QApplication(sys.argv)
def_file = "/home/eatheswar/silisfoundation/silis/projects/sram_soc_quad_pure_sdk/results/temp.def"
win = silis.view_layout(def_file, title="SILIS Live Layout Viewer — SRAM Quad Core")
win.resize(1150, 800)
win.show()
win.raise_()
win.activateWindow()
print("[SUCCESS] Visible Layout Window open on screen!")
sys.exit(app.exec())
