#!/usr/bin/env python3
"""
Silis CS Edition VNC & Virtual Display Manager
Manages Xvfb (:1), x11vnc (:1 -> 5901), and websockify/noVNC (6081 -> 5901)
for headless cloud ASIC layout inspection via OpenROAD GUI and native EDA binaries.
"""

import os
import sys
import time
import shutil
import subprocess
import json

DISPLAY_NUM = ":1"
RFB_PORT = 5901
NOVNC_PORT = 6081

NOVNC_WEB_DIRS = [
    "/usr/share/novnc",
    "/usr/lib/novnc",
    os.path.expanduser("~/.novnc"),
]


def find_binary(name):
    """Finds an executable binary in PATH or common Linux system locations."""
    cands = [
        shutil.which(name) or "",
        f"/usr/bin/{name}",
        f"/usr/local/bin/{name}",
        f"/opt/silis/runtime/bin/{name}",
    ]
    for c in cands:
        if c and os.path.isfile(c) and os.access(c, os.X_OK):
            return c
    return ""


def find_novnc_web():
    """Finds the noVNC web directory containing vnc.html."""
    for d in NOVNC_WEB_DIRS:
        if os.path.isdir(d):
            # Ensure index.html exists
            vnc_html = os.path.join(d, "vnc.html")
            idx_html = os.path.join(d, "index.html")
            if os.path.isfile(vnc_html) and not os.path.isfile(idx_html):
                try:
                    os.symlink(vnc_html, idx_html)
                except Exception:
                    pass
            return d
    return ""


def is_process_running(pattern):
    """Checks if a process matching pattern is currently running."""
    try:
        res = subprocess.run(["pgrep", "-f", pattern], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        return res.returncode == 0
    except Exception:
        return False


def get_vnc_status():
    """Returns detailed status of Xvfb, x11vnc, websockify, and noVNC."""
    xvfb_alive = is_process_running(f"Xvfb {DISPLAY_NUM}")
    x11vnc_alive = is_process_running(f"x11vnc.*{DISPLAY_NUM}") or is_process_running(f"x11vnc.*{RFB_PORT}")
    websockify_alive = is_process_running(f"websockify.*{NOVNC_PORT}")
    novnc_dir = find_novnc_web()

    all_ready = xvfb_alive and x11vnc_alive and websockify_alive

    return {
        "ready": all_ready,
        "display": DISPLAY_NUM,
        "rfb_port": RFB_PORT,
        "novnc_port": NOVNC_PORT,
        "xvfb_running": xvfb_alive,
        "x11vnc_running": x11vnc_alive,
        "websockify_running": websockify_alive,
        "novnc_web_dir": novnc_dir,
        "url_path": f"/vnc.html?autoconnect=true&resize=remote&reconnect=true",
    }


def start_vnc():
    """Ensures Xvfb, x11vnc, and websockify are running."""
    status = get_vnc_status()
    if status["ready"]:
        print(f"[Silis VNC] All VNC infrastructure already active on {DISPLAY_NUM} (Port {NOVNC_PORT}).")
        return status

    xvfb_bin = find_binary("Xvfb")
    x11vnc_bin = find_binary("x11vnc")
    websockify_bin = find_binary("websockify")
    novnc_dir = find_novnc_web()

    # 1. Start Xvfb if not running
    if not status["xvfb_running"]:
        if not xvfb_bin:
            print("[Silis VNC] [WARN] Xvfb binary not found. Cannot create virtual display.")
        else:
            print(f"[Silis VNC] Launching virtual display {DISPLAY_NUM} (1920x1080x24)...")
            # Remove stale lock if needed
            lock_file = f"/tmp/.X{DISPLAY_NUM.replace(':', '')}-lock"
            if os.path.isfile(lock_file):
                try:
                    os.remove(lock_file)
                except Exception:
                    pass
            subprocess.Popen(
                [xvfb_bin, DISPLAY_NUM, "-screen", "0", "1920x1080x24", "-ac", "+extension", "GLX", "+render", "-noreset"],
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
            )
            time.sleep(0.5)

    # 2. Start x11vnc if not running
    if not status["x11vnc_running"]:
        if not x11vnc_bin:
            print("[Silis VNC] [WARN] x11vnc binary not found.")
        else:
            print(f"[Silis VNC] Launching x11vnc on {DISPLAY_NUM} (RFB Port {RFB_PORT})...")
            subprocess.Popen(
                [x11vnc_bin, "-display", DISPLAY_NUM, "-nopw", "-listen", "127.0.0.1", "-xkb", "-ncache", "10", "-ncache_cr", "-forever", "-shared", "-rfbport", str(RFB_PORT)],
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
            )
            time.sleep(0.5)

    # 3. Start websockify if not running
    if not status["websockify_running"]:
        if not websockify_bin:
            print("[Silis VNC] [WARN] websockify binary not found.")
        else:
            web_arg = ["--web", novnc_dir] if novnc_dir else []
            print(f"[Silis VNC] Launching websockify on port {NOVNC_PORT} -> {RFB_PORT}...")
            cmd = [websockify_bin] + web_arg + [str(NOVNC_PORT), f"localhost:{RFB_PORT}"]
            subprocess.Popen(
                cmd,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
            )
            time.sleep(0.5)

    # 4. Auto-set Codespaces port visibility to public if gh CLI is available
    cs_name = os.environ.get("CODESPACE_NAME", "")
    if cs_name and shutil.which("gh"):
        try:
            subprocess.run(
                ["gh", "codespace", "ports", "visibility", f"{NOVNC_PORT}:public", "-c", cs_name],
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
                timeout=5,
            )
        except Exception:
            pass

    return get_vnc_status()


def stop_vnc():
    """Stops Xvfb, x11vnc, and websockify."""
    print("[Silis VNC] Stopping VNC infrastructure...")
    for pat in [f"websockify.*{NOVNC_PORT}", f"x11vnc.*{DISPLAY_NUM}", f"Xvfb {DISPLAY_NUM}"]:
        try:
            subprocess.run(["pkill", "-f", pat], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        except Exception:
            pass
    print("[Silis VNC] Stopped.")


def main():
    if len(sys.argv) > 1 and sys.argv[1] == "status":
        print(json.dumps(get_vnc_status(), indent=2))
    elif len(sys.argv) > 1 and sys.argv[1] == "stop":
        stop_vnc()
    else:
        st = start_vnc()
        print(f"[Silis VNC] Status: {json.dumps(st, indent=2)}")


if __name__ == "__main__":
    main()
