#!/usr/bin/env python3
"""
gui_driver.py
Autonomous GUI Debugger & Test Harness Driver for Silis.

Supports both:
- Live Visible Desktop Driving (--live, default):
  Configures X11 DISPLAY and XAUTHORITY, sets QT_QPA_PLATFORM='xcb',
  raises SilisIDE window, and visually simulates robotic user clicks
  with customizable button highlights, pulses, and pacing.
- Headless Driving (--headless):
  Runs offscreen (QT_QPA_PLATFORM='offscreen') with zero latency for
  automated CI testing and regression suites.

Features:
- Visual action simulation (visual_click, pulsing button borders/backgrounds)
- Paced smooth Qt event pumping (app.processEvents())
- Tab navigation, backend ribbon step button execution, world switching
- Report viewing (SignoffReportViewer, Synthesis dashboard, area/sizes)
- Synchronization with FlowWorker threads, OpenROAD QProcess, and GUI step markers
- Complete Floorplan step automation with peripheral macro placement
- Tool error detection ([ERROR, DPL-0033, ORD-, fatal)
- Structured JSON debug report generation (.env/reports/<project>_debug_report.json)
"""

import os
import sys
import re
import json
import time
import glob
from datetime import datetime

ENV_DIR = os.path.dirname(os.path.abspath(__file__))
SILIS_ROOT = os.path.dirname(ENV_DIR)

if SILIS_ROOT not in sys.path:
    sys.path.insert(0, SILIS_ROOT)
if ENV_DIR not in sys.path:
    sys.path.insert(0, ENV_DIR)

import qt_setup
from PyQt6.QtWidgets import QApplication, QPushButton, QTabBar
from PyQt6.QtCore import QProcess

import silis
from silis import SilisIDE
from backendflow.flow.backendflow import BackendWidget, FlowWorker
from backendflow.floorplanner.floorplanner import (
    InteractiveFloorplannerWidget,
    compute_peripheral_macro_placement,
    generate_floorplan_tcl_from_data
)
from synthesisscripts.synthesisthread import ReportEngine
import project_registry


# Standard step alias mapping
STEP_NAME_MAP = {
    "synth": "Synth",
    "synthesis": "Synth",
    "init": "Init",
    "floorplan": "Floorplan",
    "fp": "Floorplan",
    "tapcells": "Tapcells",
    "tapcell": "Tapcells",
    "tap": "Tapcells",
    "pdn": "PDN",
    "iopins": "IO Pins",
    "io_pins": "IO Pins",
    "io-pins": "IO Pins",
    "io pins": "IO Pins",
    "place": "Place",
    "placement": "Place",
    "cts": "CTS",
    "route": "Route",
    "routing": "Route",
    "antenna": "Antenna",
    "sta": "STA",
    "pat": "PAT",
    "drc": "DRC",
    "gds": "GDS",
    "dft": "DFT",
    "scan": "DFT",
    "atpg": "DFT",
    "fill": "Fill",
    "density": "Fill",
    "decap": "Fill",
    "lvs": "LVS",
    "gls": "GLS",
    "sdf": "GLS",
    "tapeout": "Tapeout",
    "manifest": "Tapeout",
}

# Standard sequence of full flow
FLOW_SEQUENCE = [
    "synth",
    "init",
    "floorplan",
    "tapcells",
    "pdn",
    "iopins",
    "place",
    "cts",
    "route",
    "fill",
    "gds",
    "antenna",
    "sta",
    "drc",
    "pat",
    "dft",
    "lvs",
    "gls",
    "tapeout"
]


def configure_display_and_auth():
    """
    Auto-detects and configures DISPLAY and XAUTHORITY for live visible desktop execution.
    """
    if not os.environ.get("DISPLAY"):
        sockets = glob.glob("/tmp/.X11-unix/X*")
        nums = []
        for s in sockets:
            m = re.search(r'X(\d+)', os.path.basename(s))
            if m:
                nums.append(int(m.group(1)))
        if nums:
            disp_num = 1 if 1 in nums else sorted(nums)[0]
            os.environ["DISPLAY"] = f":{disp_num}"
        else:
            os.environ["DISPLAY"] = ":1"

    if not os.environ.get("XAUTHORITY"):
        uid = os.getuid()
        possible_auths = [
            f"/run/user/{uid}/gdm/Xauthority",
            f"/run/user/{uid}/Xauthority",
            os.path.expanduser("~/.Xauthority")
        ]
        for p in possible_auths:
            if os.path.isfile(p):
                os.environ["XAUTHORITY"] = p
                break


def resolve_project_file(project_arg=None):
    """
    Resolves project argument (name, directory, or .silisproj) to an absolute path.
    """
    if not project_arg:
        # Check current directory
        cwd = os.getcwd()
        cand = os.path.join(cwd, "silis.silisproj")
        if os.path.isfile(cand):
            return os.path.abspath(cand)
        # Check most recent in registry
        recent = project_registry.get_most_recent_project()
        if recent and recent.get("proj_file") and os.path.isfile(recent["proj_file"]):
            return recent["proj_file"]
        return None

    # Exact file match
    if os.path.isfile(project_arg):
        return os.path.abspath(project_arg)

    # Directory match
    if os.path.isdir(project_arg):
        cand = os.path.join(project_arg, "silis.silisproj")
        if os.path.isfile(cand):
            return os.path.abspath(cand)

    # Search in SILIS_ROOT/projects/<name>
    cand_proj = os.path.join(SILIS_ROOT, "projects", project_arg, "silis.silisproj")
    if os.path.isfile(cand_proj):
        return os.path.abspath(cand_proj)

    cand_proj2 = os.path.join(SILIS_ROOT, "projects", project_arg)
    if os.path.isdir(cand_proj2):
        c = os.path.join(cand_proj2, "silis.silisproj")
        if os.path.isfile(c):
            return os.path.abspath(c)

    # Search project registry
    reg_match = project_registry.find_registered_project(project_arg)
    if reg_match:
        if os.path.isfile(reg_match):
            return reg_match
        c = os.path.join(reg_match, "silis.silisproj")
        if os.path.isfile(c):
            return os.path.abspath(c)

    return None


class SilisGUIDriver:
    """
    Autonomous GUI Flow Driver & Debugger for Silis.
    Drives SilisIDE and BackendWidget with 100% parity with GUI button clicks,
    supporting both Live Visible Desktop and Headless Offscreen modes.
    """

    def __init__(self, project_file_or_dir=None, headless=False, verbose=True, action_delay=0.35):
        self.headless = headless
        self.verbose = verbose
        self.action_delay = 0.0 if headless else max(0.05, action_delay)

        # Configure display environment
        if self.headless:
            os.environ["QT_QPA_PLATFORM"] = "offscreen"
        else:
            configure_display_and_auth()
            if os.environ.get("QT_QPA_PLATFORM") == "offscreen":
                del os.environ["QT_QPA_PLATFORM"]
            os.environ["QT_QPA_PLATFORM"] = "xcb"

        self.proj_file = resolve_project_file(project_file_or_dir)
        if not self.proj_file or not os.path.isfile(self.proj_file):
            raise FileNotFoundError(f"Could not locate silis.silisproj for '{project_file_or_dir}'")

        self.proj_dir = os.path.dirname(self.proj_file)
        with open(self.proj_file, "r") as f:
            self.config = json.load(f)

        project_registry.register_project(self.proj_file)

        # Ensure Qt Application
        self.app = QApplication.instance()
        if not self.app:
            if self.headless:
                self.app = QApplication(["silis", "-platform", "offscreen"])
            else:
                self.app = QApplication(["silis", "-platform", "xcb"])

        # Instantiate SilisIDE
        self.ide = SilisIDE(project_file=self.proj_file, headless=self.headless)
        self.ide.project_path = self.proj_dir
        self.ide.cwd = self.proj_dir
        self.backend = self.ide.backend_widget
        self.backend_widget = self.backend

        # Wrap backend run_flow_step and ide run_synthesis_flow to default bypass_prompt=True
        orig_run_step = self.backend.run_flow_step
        def _auto_run_step(step_name, bypass_prompt=True):
            return orig_run_step(step_name, bypass_prompt=bypass_prompt)
        self.backend.run_flow_step = _auto_run_step

        orig_run_synth = self.ide.run_synthesis_flow
        def _auto_run_synth(bypass_prompt=True):
            return orig_run_synth(bypass_prompt=bypass_prompt)
        self.ide.run_synthesis_flow = _auto_run_synth

        # Ensure PDK is active
        pdk_name = self.config.get("pdk", "sky130")
        self.ide.resolve_and_set_pdk(pdk_name)
        if not self.backend.active_pdk and self.ide.active_pdk:
            self.backend.active_pdk = self.ide.active_pdk

        # In live mode: resize, show, raise, and activate the SilisIDE window
        if not self.headless:
            self.ide.resize(1500, 950)
            self.ide.show()
            self.ide.raise_()
            self.ide.activateWindow()
            self.app.processEvents()

        # Hook backend OpenROAD process stdout
        self._current_step_marker = None
        self._marker_seen = False
        self._active_step_name = "Init"
        self._current_step_errors = []
        self._current_step_warnings = []
        self._current_step_logs = []
        self._had_timing_violation = False
        self._setup_stdout_interceptor()

        # Reports directory
        self.reports_dir = os.path.join(ENV_DIR, "reports")
        os.makedirs(self.reports_dir, exist_ok=True)

        proj_name = self.config.get("project_name", self.config.get("top_module", "project"))
        self.default_report_path = os.path.join(self.reports_dir, f"{proj_name}_debug_report.json")

        self.report_data = {
            "project": proj_name,
            "top_module": self.config.get("top_module", "top"),
            "pdk": self.config.get("pdk", "sky130"),
            "mode": "HEADLESS" if self.headless else "LIVE_VISIBLE",
            "timestamp": datetime.now().isoformat(),
            "status": "INITIALIZED",
            "steps": [],
            "tool_errors": [],
            "timing": {
                "status": "UNKNOWN",
                "wns": None,
                "violations": []
            },
            "drc": {
                "violations": None
            },
            "artifacts": {}
        }

    # --------------------------------------------------------------------------
    # Visual Action Simulation & Pacing (Live Robotic Driving)
    # --------------------------------------------------------------------------
    def smooth_sleep(self, seconds: float):
        """Pumps Qt event loop smoothly while sleeping to keep GUI completely responsive."""
        if seconds <= 0:
            self.app.processEvents()
            return
        t0 = time.time()
        while time.time() - t0 < seconds:
            self.app.processEvents()
            time.sleep(0.02)

    def visual_click(self, widget, label=None, delay=None, trigger=True) -> bool:
        """
        Simulates human/robotic click with visual pacing and button border/background pulsing.
        """
        if not widget:
            return False

        dur = delay if delay is not None else self.action_delay
        lbl = label or (widget.text() if hasattr(widget, 'text') else str(widget))

        if self.verbose:
            print(f"[AUTONOMOUS LIVE] 🖱️ Target Click: '{lbl}' (pacing: {dur:.2f}s)")

        if not self.headless and dur > 0:
            orig_style = widget.styleSheet()
            # Highlight border and background with high-contrast accent glow
            pulse_style = orig_style + "; border: 2px solid #58a6ff !important; background-color: #1f6feb !important; color: #ffffff !important; font-weight: bold;"
            widget.setStyleSheet(pulse_style)
            self.smooth_sleep(dur * 0.45)
            widget.setStyleSheet(orig_style)
            self.app.processEvents()
            if trigger:
                if hasattr(widget, 'animateClick'):
                    widget.animateClick()
                elif hasattr(widget, 'click'):
                    widget.click()
            self.smooth_sleep(dur * 0.55)
        else:
            if trigger and hasattr(widget, 'click'):
                widget.click()
            self.app.processEvents()

        return True

    def get_step_button(self, step_name: str):
        """Locates the QPushButton corresponding to a step on the backend ribbon."""
        raw_key = step_name.strip().lower()
        gui_step = STEP_NAME_MAP.get(raw_key, step_name.strip())
        if hasattr(self.backend, 'ribbon') and self.backend.ribbon:
            for btn in self.backend.ribbon.findChildren(QPushButton):
                txt = btn.text().strip()
                if txt.lower() == gui_step.lower() or txt.lower() == raw_key:
                    return btn
        return None

    def switch_world(self, world, delay=None):
        """
        Switches between Frontend (0) and Backend (1) worlds with visual button click.
        """
        idx = 1 if (isinstance(world, str) and "back" in world.lower()) or world == 1 else 0
        world_name = "Backend" if idx == 1 else "Frontend"
        dur = delay if delay is not None else self.action_delay

        if self.verbose:
            print(f"[AUTONOMOUS LIVE] 🌐 World Navigation -> {world_name}")

        target_btn = self.ide.btn_back if idx == 1 else self.ide.btn_front
        if not self.headless and dur > 0:
            self.visual_click(target_btn, label=f"Switch World: {world_name}", delay=dur, trigger=True)
        else:
            self.ide.switch_world(idx)
            self.app.processEvents()

    def select_frontend_tab(self, tab, delay=None):
        """
        Navigates between frontend tabs (0: Compile, 1: Waveform, 2: Schematic, 3: Synthesis).
        """
        dur = delay if delay is not None else self.action_delay
        tab_names = ["compile", "waveform", "schematic", "synthesis"]
        if isinstance(tab, str):
            t_low = tab.strip().lower()
            idx = 0
            for i, name in enumerate(tab_names):
                if t_low in name or name in t_low:
                    idx = i
                    break
        else:
            idx = int(tab)

        tab_label = self.ide.frontend_tabs.tabText(idx)
        if self.verbose:
            print(f"[AUTONOMOUS LIVE] 📑 Frontend Tab Selection -> {tab_label}")

        self.ide.frontend_tabs.setCurrentIndex(idx)
        if not self.headless and dur > 0:
            self.smooth_sleep(dur)
        else:
            self.app.processEvents()

    def select_backend_viz_tab(self, tab, delay=None):
        """
        Navigates between backend visualization tabs (0: Layout, 1: 3D, 2: Clock Tree).
        """
        dur = delay if delay is not None else self.action_delay
        tab_names = ["fast layout viewer", "gds view (3d)", "clock tree"]
        if isinstance(tab, str):
            t_low = tab.strip().lower()
            idx = 0
            for i, name in enumerate(tab_names):
                if t_low in name or name in t_low:
                    idx = i
                    break
        else:
            idx = int(tab)

        tab_label = self.backend.viz_tabs.tabText(idx)
        if self.verbose:
            print(f"[AUTONOMOUS LIVE] 📑 Backend Viz Tab Selection -> {tab_label}")

        self.backend.viz_tabs.setCurrentIndex(idx)
        if not self.headless and dur > 0:
            self.smooth_sleep(dur)
        else:
            self.app.processEvents()

    def click_step_button(self, step_name: str, wait=True) -> bool:
        """
        Visually targets and clicks a flow step button by name.
        """
        raw_key = step_name.strip().lower()
        gui_step = STEP_NAME_MAP.get(raw_key, step_name.strip())

        if gui_step == "Synth":
            self.switch_world(0)
            self.select_frontend_tab(3)
            if hasattr(self.ide, 'tab_synth') and hasattr(self.ide.tab_synth, 'btn_run'):
                self.visual_click(self.ide.tab_synth.btn_run, label="Synthesis: Run Flow")
            return self.run_synthesis() if wait else True
        else:
            self.switch_world(1)
            btn = self.get_step_button(gui_step)
            if btn:
                self.visual_click(btn, label=f"Step: {gui_step}", trigger=False)
            return self.run_step(gui_step) if wait else True

    def view_report(self, report_type="pat", duration=2.0) -> bool:
        """
        Navigates to or displays inspection reports (PAT, Synthesis Dashboard, Area, Sizes).
        """
        dur = max(0.5, duration)
        rpt_low = report_type.strip().lower()
        if self.verbose:
            print(f"[AUTONOMOUS LIVE] 📊 Inspecting Report: '{report_type}' ({dur}s)")

        reports_dir = os.path.join(self.proj_dir, "reports")
        top_name = self.config.get("top_module", "top")

        if rpt_low in ["pat", "signoff"]:
            rpt_file = os.path.join(reports_dir, f"{top_name}_pat_report.rpt")
            if not os.path.isfile(rpt_file):
                cands = glob.glob(os.path.join(reports_dir, "*pat*.rpt"))
                if cands:
                    rpt_file = cands[0]
            if os.path.isfile(rpt_file):
                from backendflow.flow.backendflow import SignoffReportViewer
                viewer = SignoffReportViewer(rpt_file, timing_failed=self._had_timing_violation, parent=self.ide)
                viewer.show()
                viewer.raise_()
                viewer.activateWindow()
                self.smooth_sleep(dur)
                viewer.close()
                return True
            else:
                if self.verbose:
                    print(f"[AUTONOMOUS LIVE] [WARN] PAT report file not found at {rpt_file}")
                return False

        elif rpt_low in ["dft", "atpg", "scan"]:
            dft_json = os.path.join(reports_dir, "dft_summary.json")
            dft_rpt = os.path.join(reports_dir, "dft_summary.rpt")
            if os.path.isfile(dft_json):
                try:
                    from silis.dft.dft_viewer import DFTSignoffViewer
                except (ImportError, ModuleNotFoundError):
                    from dft.dft_viewer import DFTSignoffViewer
                with open(dft_json, "r") as f:
                    summary = json.load(f)
                viewer = DFTSignoffViewer(summary, parent=self.ide, project_dir=self.proj_dir)
                viewer.show()
                viewer.raise_()
                viewer.activateWindow()
                self.smooth_sleep(dur)
                viewer.close()
                return True
            elif os.path.isfile(dft_rpt):
                self.switch_world(0)
                self.select_frontend_tab(0)
                self.ide.open_file_in_editor(dft_rpt)
                self.smooth_sleep(dur)
                return True
            return False

        elif rpt_low in ["lvs"]:
            lvs_json = os.path.join(reports_dir, "lvs_summary.json")
            lvs_rpt = os.path.join(reports_dir, "lvs.rpt")
            if os.path.isfile(lvs_json):
                try:
                    from silis.lvs.lvs_viewer import LVSSignoffViewer
                except (ImportError, ModuleNotFoundError):
                    from lvs.lvs_viewer import LVSSignoffViewer
                with open(lvs_json, "r") as f:
                    summary = json.load(f)
                viewer = LVSSignoffViewer(summary, parent=self.ide, project_dir=self.proj_dir)
                viewer.show()
                viewer.raise_()
                viewer.activateWindow()
                self.smooth_sleep(dur)
                viewer.close()
                return True
            elif os.path.isfile(lvs_rpt):
                self.switch_world(0)
                self.select_frontend_tab(0)
                self.ide.open_file_in_editor(lvs_rpt)
                self.smooth_sleep(dur)
                return True
            return False

        elif rpt_low in ["gls", "sdf"]:
            gls_json = os.path.join(reports_dir, "gls_summary.json")
            gls_rpt = os.path.join(reports_dir, "gls.rpt")
            if os.path.isfile(gls_json):
                try:
                    from silis.simulation.gls_viewer import GLSSignoffViewer
                except (ImportError, ModuleNotFoundError):
                    from simulation.gls_viewer import GLSSignoffViewer
                with open(gls_json, "r") as f:
                    summary = json.load(f)
                viewer = GLSSignoffViewer(summary, parent=self.ide, project_dir=self.proj_dir)
                viewer.show()
                viewer.raise_()
                viewer.activateWindow()
                self.smooth_sleep(dur)
                viewer.close()
                return True
            elif os.path.isfile(gls_rpt):
                self.switch_world(0)
                self.select_frontend_tab(0)
                self.ide.open_file_in_editor(gls_rpt)
                self.smooth_sleep(dur)
                return True
            return False

        elif rpt_low in ["tapeout", "manifest"]:
            tapeout_json = os.path.join(reports_dir, "tapeout_manifest.json")
            tapeout_rpt = os.path.join(reports_dir, "tapeout.rpt")
            if os.path.isfile(tapeout_json):
                try:
                    from silis.tapeout.manifest_viewer import TapeoutManifestViewer
                except (ImportError, ModuleNotFoundError):
                    from tapeout.manifest_viewer import TapeoutManifestViewer
                with open(tapeout_json, "r") as f:
                    manifest = json.load(f)
                viewer = TapeoutManifestViewer(manifest, parent=self.ide, project_dir=self.proj_dir)
                viewer.show()
                viewer.raise_()
                viewer.activateWindow()
                self.smooth_sleep(dur)
                viewer.close()
                return True
            elif os.path.isfile(tapeout_rpt):
                self.switch_world(0)
                self.select_frontend_tab(0)
                self.ide.open_file_in_editor(tapeout_rpt)
                self.smooth_sleep(dur)
                return True
            return False

        elif rpt_low in ["synth", "synthesis", "dashboard"]:
            self.switch_world(0)
            self.select_frontend_tab(3)
            if hasattr(self.ide, 'tab_synth') and self.ide.tab_synth:
                self.ide.tab_synth.update_dashboard()
            self.smooth_sleep(dur)
            return True

        elif rpt_low in ["area", "sizes"]:
            cand = os.path.join(reports_dir, "area.rpt") if rpt_low == "area" else os.path.join(reports_dir, f"{top_name}_sizes.json")
            if os.path.isfile(cand):
                self.switch_world(0)
                self.select_frontend_tab(0)
                self.ide.open_file_in_editor(cand)
                self.smooth_sleep(dur)
                return True
            return False

        elif os.path.isfile(report_type):
            self.switch_world(0)
            self.select_frontend_tab(0)
            self.ide.open_file_in_editor(report_type)
            self.smooth_sleep(dur)
            return True

        return False

    # --------------------------------------------------------------------------
    # Stdout Interception & Error Detection
    # --------------------------------------------------------------------------
    def _setup_stdout_interceptor(self):
        self.backend.read_stdout = self._intercepted_read_stdout
        if self.backend.proc:
            try:
                self.backend.proc.readyReadStandardOutput.disconnect()
            except Exception:
                pass
            self.backend.proc.readyReadStandardOutput.connect(self._intercepted_read_stdout)

    def _intercepted_read_stdout(self):
        if not self.backend.proc:
            return
        try:
            raw_bytes = self.backend.proc.readAllStandardOutput().data()
            text = raw_bytes.decode(errors="replace")
        except Exception:
            return

        if not text:
            return

        if self.verbose:
            print(text, end="", flush=True)

        self.backend.term_log.append(text.strip())

        # Check pending_init
        if self.backend.pending_init and ("OpenROAD" in text or "openroad>" in text):
            cmd = self.backend.pending_init
            self.backend.pending_init = None
            self.backend.send_command_internal(cmd)

        # Detect tool errors in this chunk
        self._scan_for_errors(text)

        # Record into current step logs
        self._current_step_logs.append(text)

        # Check step marker
        marker = self._current_step_marker
        if marker and marker in text:
            self._marker_seen = True
            self.backend.cmd_active = False
            self.backend.stop_spinner()
        elif "openroad>" in text:
            if self.backend.cmd_active:
                self.backend.cmd_active = False
                self.backend.force_refresh_view()
                self.backend.stop_spinner()

    def _scan_for_errors(self, text):
        for line in text.splitlines():
            line_str = line.strip()
            if not line_str:
                continue

            # Skip informational and warning logs
            if line_str.startswith("[INFO") or line_str.startswith("[WARNING"):
                continue

            is_err = False
            err_code = "TOOL_ERROR"

            # 1. Matches [ERROR CODE] e.g. [ERROR DPL-0033] or [ERROR ORD-0020]
            m1 = re.search(r'\[ERROR\s+([A-Za-z0-9_-]+)\]\s*(.*)', line_str)
            if m1:
                is_err = True
                err_code = m1.group(1)
            elif re.search(r'^\[ERROR\]\s*(.*)', line_str) or re.search(r'^ERROR:\s*(.*)', line_str) or re.search(r'^\[ERR\]\s*(.*)', line_str):
                is_err = True
                err_code = "ERROR"
            elif re.search(r'\bERROR\b', line_str, re.IGNORECASE) and any(tag in line_str for tag in ["ORD-", "DPL-", "GPL-", "DRT-", "CTS-", "ANT-", "GRT-", "PDN-", "STA-", "ME-"]):
                m2 = re.search(r'\b((?:ORD|DPL|GPL|DRT|CTS|ANT|GRT|PDN|STA|ME)-\d+)\b', line_str)
                is_err = True
                err_code = m2.group(1) if m2 else "ERROR"
            elif re.search(r'\b(segmentation fault|sigsegv|fatal error|core dumped)\b', line_str, re.IGNORECASE):
                is_err = True
                err_code = "FATAL"

            if is_err:
                err_entry = {
                    "step": getattr(self, "_active_step_name", "Unknown"),
                    "code": err_code,
                    "message": line_str,
                    "timestamp": datetime.now().isoformat()
                }
                self._current_step_errors.append(err_entry)
                self.report_data["tool_errors"].append(err_entry)

            # Check timing violations
            if "slack (VIOLATED)" in line_str:
                self._had_timing_violation = True
                self.report_data["timing"]["status"] = "VIOLATED"
                if line_str not in self.report_data["timing"]["violations"]:
                    self.report_data["timing"]["violations"].append(line_str)

    # --------------------------------------------------------------------------
    # Event Pumping & Worker Polling
    # --------------------------------------------------------------------------
    def wait_for_openroad(self, marker=None, timeout_sec=1200):
        t0 = time.time()
        last_out = time.time()
        time.sleep(0.05)
        self.app.processEvents()

        while self.backend.cmd_active or (marker and not self._marker_seen):
            self.app.processEvents()
            if self.backend.proc and self.backend.proc.state() == QProcess.ProcessState.Running:
                if self.backend.proc.waitForReadyRead(50):
                    last_out = time.time()
            else:
                break

            if marker and self._marker_seen:
                break

            if not self.backend.cmd_active and self._current_step_errors:
                break

            if time.time() - last_out > timeout_sec:
                raise TimeoutError(f"OpenROAD step timed out after {timeout_sec}s of inactivity")
            time.sleep(0.01)

        self.app.processEvents()

    def wait_for_worker(self, worker, timeout_sec=1200):
        t0 = time.time()
        while worker and worker.isRunning():
            self.app.processEvents()
            while not self.ide.queue.empty():
                src, msg = self.ide.queue.get()
                if msg:
                    if self.verbose:
                        print(f"{src} {msg}")
                    self._scan_for_errors(f"{src} {msg}")
            time.sleep(0.05)
            if time.time() - t0 > timeout_sec:
                worker.terminate()
                raise TimeoutError("Worker thread timed out")
        self.app.processEvents()

    def wait_for_synthesis(self, timeout_sec=1200):
        t0 = time.time()
        time.sleep(0.1)
        while getattr(self.ide, "synth_running", False):
            self.app.processEvents()
            while not self.ide.queue.empty():
                src, msg = self.ide.queue.get()
                if msg:
                    if self.verbose:
                        print(f"{src} {msg}")
                    self._scan_for_errors(f"{src} {msg}")
            time.sleep(0.05)
            if time.time() - t0 > timeout_sec:
                raise TimeoutError("Synthesis flow timed out")

        while not self.ide.queue.empty():
            src, msg = self.ide.queue.get()
            if msg:
                if self.verbose:
                    print(f"{src} {msg}")
                self._scan_for_errors(f"{src} {msg}")
        self.app.processEvents()

    # --------------------------------------------------------------------------
    # Flow Step Execution
    # --------------------------------------------------------------------------
    def ensure_design_loaded(self):
        """Ensures design is initialized in OpenROAD before physical steps."""
        if getattr(self, "_design_loaded", False) and self.backend.proc and self.backend.proc.state() == QProcess.ProcessState.Running:
            return True

        def_path = os.path.join(self.proj_dir, "results", "temp.def")
        if os.path.isfile(def_path):
            self.backend.resume_def = def_path
            self.run_init()
            self._design_loaded = True
            return True
        else:
            if self.run_init():
                if self.run_floorplan():
                    self._design_loaded = True
                    return True
        return False

    def run_synthesis(self) -> bool:
        """Executes synthesis flow via SilisIDE.run_synthesis_flow(bypass_prompt=True)."""
        self._active_step_name = "Synth"
        self._current_step_errors = []
        self._current_step_logs = []
        t0 = time.time()

        if self.verbose:
            print("\n" + "=" * 65)
            print(f" [AUTONOMOUS GUI] Running Synthesis: {self.config.get('top_module')}")
            print("=" * 65)

        self.ide.run_synthesis_flow(bypass_prompt=True)
        self.wait_for_synthesis()

        duration = round(time.time() - t0, 2)
        base = self.config.get("top_module", "top")
        v_net = os.path.join(self.proj_dir, "netlist", f"{base}_netlist.v")
        area_rpt = os.path.join(self.proj_dir, "reports", "area.rpt")

        has_err = bool(self.ide.synth_error) or bool(self._current_step_errors)
        success = (not has_err) and os.path.isfile(v_net)

        if os.path.isfile(v_net):
            self.report_data["artifacts"]["netlist"] = v_net
        if os.path.isfile(area_rpt):
            self.report_data["artifacts"]["area_report"] = area_rpt

        if not self.headless and hasattr(self.ide, 'tab_synth') and self.ide.tab_synth:
            self.ide.tab_synth.update_dashboard()
            self.smooth_sleep(self.action_delay)

        step_record = {
            "step": "Synth",
            "status": "SUCCESS" if success else "FAILED",
            "duration_sec": duration,
            "errors": list(self._current_step_errors),
            "artifacts": {
                "netlist": v_net if os.path.isfile(v_net) else None
            }
        }
        self.report_data["steps"].append(step_record)
        self.save_debug_report()

        if self.verbose:
            print(f"[STATUS] Synthesis {'PASSED' if success else 'FAILED'} ({duration}s)")
        return success

    def run_init(self) -> bool:
        """Executes Init step via BackendWidget.run_flow_step('Init', bypass_prompt=True)."""
        self._active_step_name = "Init"
        self._current_step_errors = []
        self._current_step_logs = []
        self._current_step_marker = "SILIS_INIT_DONE"
        self._marker_seen = False
        t0 = time.time()

        if self.verbose:
            print("\n" + "=" * 65)
            print(" [AUTONOMOUS GUI] Running Backend Init & PDK Linking")
            print("=" * 65)

        self.backend.run_flow_step("Init", bypass_prompt=True)
        self.wait_for_openroad(marker="SILIS_INIT_DONE")

        duration = round(time.time() - t0, 2)
        base = self.config.get("top_module", "top")
        sizes_json = os.path.join(self.proj_dir, "reports", f"{base}_sizes.json")

        success = (not self._current_step_errors) and os.path.isfile(sizes_json)
        if os.path.isfile(sizes_json):
            self.report_data["artifacts"]["sizes_json"] = sizes_json

        self._design_loaded = success

        if not self.headless:
            self.backend.force_refresh_view()
            self.smooth_sleep(self.action_delay)

        step_record = {
            "step": "Init",
            "status": "SUCCESS" if success else "FAILED",
            "duration_sec": duration,
            "errors": list(self._current_step_errors),
            "artifacts": {
                "sizes_json": sizes_json if os.path.isfile(sizes_json) else None
            }
        }
        self.report_data["steps"].append(step_record)
        self.save_debug_report()

        if self.verbose:
            print(f"[STATUS] Init {'PASSED' if success else 'FAILED'} ({duration}s)")
        return success

    def run_floorplan(self) -> bool:
        """Executes Floorplan step via BackendWidget with InteractiveFloorplannerWidget logic."""
        self._active_step_name = "Floorplan"
        self._current_step_errors = []
        self._current_step_logs = []
        self._current_step_marker = "SILIS_FLOORPLAN_DONE"
        self._marker_seen = False
        t0 = time.time()

        if self.verbose:
            print("\n" + "=" * 65)
            print(" [AUTONOMOUS GUI] Running Floorplan & Macro Placement")
            print("=" * 65)

        # Ensure OpenROAD is running and design linked
        if not self.backend.proc or self.backend.proc.state() != QProcess.ProcessState.Running:
            if not self.run_init():
                return False

        # Run Floorplan via BackendWidget (which triggers InteractiveFloorplannerWidget headless logic)
        self.backend.run_flow_step("Floorplan", bypass_prompt=True)
        self.wait_for_openroad(marker="SILIS_FLOORPLAN_DONE")

        duration = round(time.time() - t0, 2)
        def_path = os.path.join(self.proj_dir, "results", "temp.def")
        success = (not self._current_step_errors) and os.path.isfile(def_path)

        if os.path.isfile(def_path):
            self.report_data["artifacts"]["def"] = def_path

        if not self.headless:
            self.backend.force_refresh_view()
            self.smooth_sleep(self.action_delay)

        step_record = {
            "step": "Floorplan",
            "status": "SUCCESS" if success else "FAILED",
            "duration_sec": duration,
            "errors": list(self._current_step_errors),
            "artifacts": {
                "def": def_path if os.path.isfile(def_path) else None
            }
        }
        self.report_data["steps"].append(step_record)
        self.save_debug_report()

        if self.verbose:
            print(f"[STATUS] Floorplan {'PASSED' if success else 'FAILED'} ({duration}s)")
        return success

    def run_step(self, step_name: str) -> bool:
        """
        Executes a single flow step with 100% GUI parity and live visual interaction.
        """
        raw_key = step_name.strip().lower()
        gui_step = STEP_NAME_MAP.get(raw_key, step_name.strip())

        if gui_step == "Synth":
            if not self.headless:
                self.switch_world(0)
                self.select_frontend_tab(3)
                if hasattr(self.ide, 'tab_synth') and hasattr(self.ide.tab_synth, 'btn_run'):
                    self.visual_click(self.ide.tab_synth.btn_run, label="Synthesis: Run Flow")
            return self.run_synthesis()
        elif gui_step == "Init":
            if not self.headless:
                self.switch_world(1)
                btn = self.get_step_button("Init")
                if btn:
                    self.visual_click(btn, label="Step: Init", trigger=False)
            return self.run_init()
        elif gui_step == "Floorplan":
            if not self.headless:
                self.switch_world(1)
                btn = self.get_step_button("Floorplan")
                if btn:
                    self.visual_click(btn, label="Step: Floorplan", trigger=False)
            return self.run_floorplan()

        self._active_step_name = gui_step
        self._current_step_errors = []
        self._current_step_logs = []
        t0 = time.time()

        if self.verbose:
            print("\n" + "=" * 65)
            print(f" [AUTONOMOUS GUI] Running Flow Step: {gui_step}")
            print("=" * 65)

        if not self.headless:
            self.switch_world(1)
            btn = self.get_step_button(gui_step)
            if btn:
                self.visual_click(btn, label=f"Step: {gui_step}", trigger=False)

        if gui_step not in ["DRC", "DFT", "LVS", "GLS", "Fill", "Tapeout"]:
            self.ensure_design_loaded()

        results_dir = os.path.join(self.proj_dir, "results")
        reports_dir = os.path.join(self.proj_dir, "reports")
        os.makedirs(results_dir, exist_ok=True)
        os.makedirs(reports_dir, exist_ok=True)

        if gui_step == "PAT":
            self.backend.run_flow_step("PAT", bypass_prompt=True)
            if hasattr(self.backend, 'pat_worker') and self.backend.pat_worker:
                self.wait_for_worker(self.backend.pat_worker)

            base = self.config.get("top_module", "top")
            rpt_file = os.path.join(reports_dir, f"{base}_pat_report.rpt")
            if os.path.isfile(rpt_file):
                self.report_data["artifacts"]["pat_report"] = rpt_file
                try:
                    with open(rpt_file, "r") as rf:
                        content = rf.read()
                    if "slack (VIOLATED)" in content:
                        self._had_timing_violation = True
                        self.report_data["timing"]["status"] = "VIOLATED"
                    elif "slack (MET)" in content:
                        if self.report_data["timing"]["status"] != "VIOLATED":
                            self.report_data["timing"]["status"] = "MET"
                except Exception:
                    pass

        elif gui_step == "STA":
            self.backend.run_flow_step("STA", bypass_prompt=True)
            self.app.processEvents()

        elif gui_step == "DRC":
            gds_file = os.path.join(results_dir, "design.gds")
            if not os.path.isfile(gds_file):
                base = self.config.get("top_module", "top")
                gds_cand = os.path.join(results_dir, f"{base}.gds")
                if os.path.isfile(gds_cand):
                    gds_file = gds_cand

            self.backend.trigger_magic_drc(self.proj_dir, gds_file)
            if hasattr(self.backend, 'drc_worker') and self.backend.drc_worker:
                self.wait_for_worker(self.backend.drc_worker)

        elif gui_step == "GDS":
            self.backend.run_flow_step("GDS", bypass_prompt=True)
            self.wait_for_openroad(timeout_sec=60)
            if hasattr(self.backend, 'magic_worker') and self.backend.magic_worker:
                self.wait_for_worker(self.backend.magic_worker)
            gds_out = os.path.join(results_dir, "design.gds")
            if os.path.isfile(gds_out):
                self.report_data["artifacts"]["gds"] = gds_out

        elif gui_step == "DFT":
            self.backend.run_flow_step("DFT", bypass_prompt=True)
            self.app.processEvents()
            base = self.config.get("top_module", "top")
            scandef = os.path.join(results_dir, "design.scandef")
            stil_path = os.path.join(results_dir, f"{base}_atpg.stil")
            tb_path = os.path.join(results_dir, f"{base}_atpg_tb.v")
            dft_json = os.path.join(reports_dir, "dft_summary.json")

            if os.path.isfile(scandef):
                self.report_data["artifacts"]["scandef"] = scandef
            if os.path.isfile(stil_path):
                self.report_data["artifacts"]["stil"] = stil_path
            if os.path.isfile(tb_path):
                self.report_data["artifacts"]["tb"] = tb_path
            if os.path.isfile(dft_json):
                self.report_data["artifacts"]["dft_summary"] = dft_json
                try:
                    with open(dft_json, "r") as f:
                        self.report_data["dft"] = json.load(f)
                except Exception:
                    pass

        elif gui_step == "Fill":
            self.backend.run_flow_step("Fill", bypass_prompt=True)
            self.app.processEvents()
            fill_rpt = os.path.join(reports_dir, "fill.rpt")
            if os.path.isfile(fill_rpt):
                self.report_data["artifacts"]["fill_report"] = fill_rpt

        elif gui_step == "LVS":
            self.backend.run_flow_step("LVS", bypass_prompt=True)
            self.app.processEvents()
            lvs_json = os.path.join(reports_dir, "lvs_summary.json")
            lvs_rpt = os.path.join(reports_dir, "lvs.rpt")
            if os.path.isfile(lvs_json):
                self.report_data["artifacts"]["lvs_summary"] = lvs_json
                try:
                    with open(lvs_json, "r") as f:
                        self.report_data["lvs"] = json.load(f)
                except Exception:
                    pass
            if os.path.isfile(lvs_rpt):
                self.report_data["artifacts"]["lvs_report"] = lvs_rpt

        elif gui_step == "GLS":
            self.backend.run_flow_step("GLS", bypass_prompt=True)
            self.app.processEvents()
            gls_json = os.path.join(reports_dir, "gls_summary.json")
            gls_rpt = os.path.join(reports_dir, "gls.rpt")
            if os.path.isfile(gls_json):
                self.report_data["artifacts"]["gls_summary"] = gls_json
                try:
                    with open(gls_json, "r") as f:
                        self.report_data["gls"] = json.load(f)
                except Exception:
                    pass
            if os.path.isfile(gls_rpt):
                self.report_data["artifacts"]["gls_report"] = gls_rpt

        elif gui_step == "Tapeout":
            self.backend.run_flow_step("Tapeout", bypass_prompt=True)
            self.app.processEvents()
            manifest_json = os.path.join(reports_dir, "tapeout_manifest.json")
            tapeout_rpt = os.path.join(reports_dir, "tapeout.rpt")
            if os.path.isfile(manifest_json):
                self.report_data["artifacts"]["tapeout_manifest"] = manifest_json
                try:
                    with open(manifest_json, "r") as f:
                        self.report_data["tapeout"] = json.load(f)
                except Exception:
                    pass
            if os.path.isfile(tapeout_rpt):
                self.report_data["artifacts"]["tapeout_report"] = tapeout_rpt

        else:
            # Standard PnR steps: Tapcells, PDN, IO Pins, Place, CTS, Route, Antenna
            step_tag = gui_step.lower().replace(" ", "_")
            marker = f"SILIS_{step_tag.upper()}_DONE"
            self._current_step_marker = marker
            self._marker_seen = False

            self.backend.run_flow_step(gui_step, bypass_prompt=True)
            self.wait_for_openroad(marker=marker)

        duration = round(time.time() - t0, 2)
        success = not bool(self._current_step_errors)
        if gui_step == "DFT":
            scandef = os.path.join(results_dir, "design.scandef")
            success = success and os.path.isfile(scandef)

        if not self.headless:
            self.backend.force_refresh_view()
            self.smooth_sleep(self.action_delay)

        step_record = {
            "step": gui_step,
            "status": "SUCCESS" if success else "FAILED",
            "duration_sec": duration,
            "errors": list(self._current_step_errors)
        }
        self.report_data["steps"].append(step_record)
        self.save_debug_report()

        if self.verbose:
            print(f"[STATUS] Step '{gui_step}' {'PASSED' if success else 'FAILED'} ({duration}s)")
        return success

    def run_flow(self, target="all", stop_at=None) -> int:
        """
        Executes flow up to target or stop_at.
        Returns:
          0: Success
          1: Tool error detected
          2: Timing violation detected
        """
        target_norm = target.strip().lower()
        stop_at_norm = stop_at.strip().lower() if stop_at else None

        steps_to_run = []
        if target_norm == "all":
            steps_to_run = list(FLOW_SEQUENCE)
        elif target_norm == "synth":
            steps_to_run = ["synth"]
        elif target_norm == "init":
            steps_to_run = ["init"]
        elif target_norm == "floorplan":
            steps_to_run = ["init", "floorplan"]
        elif target_norm == "place":
            steps_to_run = ["init", "floorplan", "tapcells", "pdn", "iopins", "place"]
        elif target_norm == "cts":
            steps_to_run = ["cts"]
        elif target_norm == "route":
            steps_to_run = ["route"]
        elif target_norm == "gds":
            steps_to_run = ["gds"]
        else:
            steps_to_run = [target_norm]

        if stop_at_norm:
            filtered = []
            for s in steps_to_run:
                filtered.append(s)
                if s == stop_at_norm:
                    break
            steps_to_run = filtered

        for step in steps_to_run:
            ok = self.run_step(step)
            if not ok or self._current_step_errors:
                self.report_data["status"] = "FAILED"
                self.save_debug_report()
                return 1

        if self._had_timing_violation:
            self.report_data["status"] = "TIMING_VIOLATION"
            self.save_debug_report()
            return 2

        self.report_data["status"] = "SUCCESS"
        self.save_debug_report()
        return 0

    def run_signoff(self, signoff_type: str) -> int:
        """
        Executes Signoff (PAT, STA, or DRC).
        Returns:
          0: Success
          1: Tool error
          2: Timing violation
        """
        st = signoff_type.strip().upper()
        if st in ["PAT", "STA"]:
            ok = self.run_step("PAT")
            if not ok or self._current_step_errors:
                return 1
            if self._had_timing_violation:
                return 2
            return 0
        elif st == "DRC":
            ok = self.run_step("DRC")
            return 0 if ok and not self._current_step_errors else 1
        else:
            ok = self.run_step(st)
            return 0 if ok and not self._current_step_errors else 1

    def execute_tcl(self, cmd: str, step_tag="custom") -> bool:
        """Executes arbitrary TCL in OpenROAD backend process."""
        self._active_step_name = f"TCL_{step_tag}"
        self._current_step_errors = []
        marker = f"SILIS_{step_tag.upper()}_DONE"
        self._current_step_marker = marker
        self._marker_seen = False

        full_cmd = cmd.strip() + f'\nputs "{marker}"\n'
        self.backend.send_command_internal(full_cmd)
        self.wait_for_openroad(marker=marker)
        return not bool(self._current_step_errors)

    def save_debug_report(self, out_path=None) -> str:
        """Saves structured debug report JSON."""
        target_path = out_path if out_path else self.default_report_path
        os.makedirs(os.path.dirname(os.path.abspath(target_path)), exist_ok=True)
        self.report_data["timestamp"] = datetime.now().isoformat()
        with open(target_path, "w") as f:
            json.dump(self.report_data, f, indent=2)
        return target_path

    def close(self):
        """Clean termination of OpenROAD, SilisIDE, and worker threads."""
        if hasattr(self, 'backend') and self.backend and self.backend.proc:
            proc = self.backend.proc
            self.backend.proc = None
            try:
                proc.readyReadStandardOutput.disconnect()
            except Exception:
                pass
            if proc.state() == QProcess.ProcessState.Running:
                try:
                    proc.write(b"exit\n")
                    if not proc.waitForFinished(500):
                        proc.kill()
                        proc.waitForFinished(500)
                except Exception:
                    pass
            proc.deleteLater()
        if hasattr(self, 'ide') and self.ide:
            try:
                self.ide.closeEvent = lambda e: e.accept()
                self.ide.close()
            except Exception:
                pass

    def __del__(self):
        try:
            self.close()
        except Exception:
            pass


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="Silis Autonomous GUI Driver (Live & Headless)")
    parser.add_argument("--project", default="sram_soc_system", help="Project name or path")
    parser.add_argument("--step", default="Init", help="Single step to execute")
    parser.add_argument("--live", dest="headless", action="store_false", help="Live visible desktop mode (default)")
    parser.add_argument("--headless", dest="headless", action="store_true", help="Headless offscreen mode")
    parser.add_argument("--delay", type=float, default=0.35, help="Pacing delay in seconds for live mode")
    parser.set_defaults(headless=False)
    args = parser.parse_args()

    driver = SilisGUIDriver(args.project, headless=args.headless, action_delay=args.delay)
    mode_str = "Headless" if driver.headless else "Live Visible Desktop"
    print(f"Initialized SilisGUIDriver [{mode_str}] for {driver.config.get('top_module')}")
    success = driver.run_step(args.step)
    print(f"Step '{args.step}' execution result: {success}")
    driver.close()
    sys.exit(0 if success else 1)
