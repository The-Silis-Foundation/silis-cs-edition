#!/usr/bin/env python3
"""
Silis CS Edition VNC & Virtual Display Manager
Manages Xvfb (:1), x11vnc (:1 -> 5901), and websockify/noVNC (6081 -> 5901)
for headless cloud ASIC layout inspection via OpenROAD GUI and native EDA binaries.
Hardened with zombie process detection, port collision resolution, container
restart stale lock cleanup, and robust error logging.
"""

import os
import sys
import time
import socket
import signal
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

LOG_DIR = "/tmp"
XVFB_LOG = os.path.join(LOG_DIR, "silis_xvfb.log")
X11VNC_LOG = os.path.join(LOG_DIR, "silis_x11vnc.log")
WEBSOCKIFY_LOG = os.path.join(LOG_DIR, "silis_websockify.log")
PID_FILE = os.path.join(LOG_DIR, ".silis_vnc_pids.json")


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


def get_active_pids(pattern):
    """
    Returns active (non-zombie / non-defunct) PIDs matching pattern.
    Inspects /proc/<pid>/status to filter out dead or zombie processes.
    """
    try:
        res = subprocess.run(["pgrep", "-f", pattern], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        if res.returncode != 0:
            return []
        active = []
        for line in res.stdout.strip().splitlines():
            line = line.strip()
            if not line.isdigit():
                continue
            pid = int(line)
            # Filter our own PID
            if pid == os.getpid():
                continue
            stat_file = f"/proc/{pid}/status"
            if os.path.isfile(stat_file):
                try:
                    with open(stat_file, "r") as f:
                        stat_text = f.read()
                        # State: Z (zombie)
                        if "State:\tZ" in stat_text or "State: Z" in stat_text:
                            continue
                        active.append(pid)
                except Exception:
                    active.append(pid)
            else:
                active.append(pid)
        return active
    except Exception:
        return []


def is_process_running(pattern):
    """Checks if an active, non-zombie process matching pattern is currently running."""
    return len(get_active_pids(pattern)) > 0


def is_port_in_use(port, host="127.0.0.1"):
    """Checks if a TCP port is currently open and bound."""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.settimeout(0.5)
        try:
            s.connect((host, port))
            return True
        except (socket.timeout, ConnectionRefusedError, OSError):
            return False


def cleanup_stale_locks_and_zombies():
    """
    Cleans up stale X11 lock files and UNIX domain sockets left behind by crashed
    X servers or previous container restarts.
    Safely verifies if the PID recorded in the lock file is dead before unlinking.
    """
    disp_num = DISPLAY_NUM.replace(":", "")
    lock_file = f"/tmp/.X{disp_num}-lock"
    sock_file = f"/tmp/.X11-unix/X{disp_num}"

    is_pid_alive = False
    if os.path.isfile(lock_file):
        try:
            with open(lock_file, "r") as f:
                content = f.read().strip()
                if content.isdigit():
                    pid = int(content)
                    if os.path.isdir(f"/proc/{pid}"):
                        stat_file = f"/proc/{pid}/status"
                        if os.path.isfile(stat_file):
                            with open(stat_file, "r") as sf:
                                st = sf.read()
                                if "State:\tZ" not in st and "State: Z" not in st:
                                    is_pid_alive = True
                        else:
                            is_pid_alive = True
        except Exception:
            pass

    active_xvfb = get_active_pids(f"Xvfb.*{DISPLAY_NUM}")
    if not active_xvfb and not is_pid_alive:
        # Reap or kill any defunct/zombie Xvfb
        try:
            subprocess.run(["pkill", "-9", "-f", f"Xvfb.*{DISPLAY_NUM}"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        except Exception:
            pass

        if os.path.isfile(lock_file):
            try:
                os.remove(lock_file)
                print(f"[Silis VNC] Removed stale display lock: {lock_file}")
            except Exception as e:
                print(f"[Silis VNC] [WARN] Could not remove {lock_file}: {e}")

        if os.path.exists(sock_file):
            try:
                os.remove(sock_file)
                print(f"[Silis VNC] Removed stale display UNIX socket: {sock_file}")
            except Exception as e:
                print(f"[Silis VNC] [WARN] Could not remove {sock_file}: {e}")


def resolve_port_collision(port, service_name, pattern):
    """
    Detects if port is in use. If held by a stale service matching pattern, terminates it.
    If held by an external process, prints warning.
    """
    if not is_port_in_use(port):
        return True

    print(f"[Silis VNC] [WARN] Port {port} for {service_name} is already in use. Checking for stale processes...")
    pids = get_active_pids(pattern)
    if pids:
        print(f"[Silis VNC] Terminating stale {service_name} PIDs: {pids}")
        for pid in pids:
            try:
                os.kill(pid, signal.SIGTERM)
            except Exception:
                pass
        time.sleep(0.5)
        if is_port_in_use(port):
            for pid in pids:
                try:
                    os.kill(pid, signal.SIGKILL)
                except Exception:
                    pass
            time.sleep(0.3)

    if is_port_in_use(port):
        print(f"[Silis VNC] [ERR] Port {port} collision: Port is occupied by another process.")
        return False
    return True


def get_vnc_status():
    """Returns detailed status of Xvfb, x11vnc, websockify, and noVNC."""
    xvfb_alive = is_process_running(f"Xvfb.*{DISPLAY_NUM}")
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
        "url_path": "/vnc.html?autoconnect=true&resize=remote&reconnect=true",
    }


def start_vnc():
    """Ensures Xvfb, x11vnc, and websockify are running."""
    # Pre-flight container restart cleanup
    cleanup_stale_locks_and_zombies()

    status = get_vnc_status()
    if status["ready"]:
        print(f"[Silis VNC] All VNC infrastructure already active on {DISPLAY_NUM} (Port {NOVNC_PORT}).")
        return status

    xvfb_bin = find_binary("Xvfb")
    x11vnc_bin = find_binary("x11vnc")
    websockify_bin = find_binary("websockify")
    novnc_dir = find_novnc_web()

    pids_recorded = {}
    if os.path.isfile(PID_FILE):
        try:
            with open(PID_FILE, "r") as f:
                pids_recorded = json.load(f)
        except Exception:
            pids_recorded = {}

    # 1. Start Xvfb if not running
    if not status["xvfb_running"]:
        if not xvfb_bin:
            print("[Silis VNC] [WARN] Xvfb binary not found. Cannot create virtual display.")
        else:
            print(f"[Silis VNC] Launching virtual display {DISPLAY_NUM} (1920x1080x24)...")
            cleanup_stale_locks_and_zombies()
            try:
                xvfb_log_fp = open(XVFB_LOG, "a", encoding="utf-8")
                xvfb_proc = subprocess.Popen(
                    [xvfb_bin, DISPLAY_NUM, "-screen", "0", "1920x1080x24", "-ac", "+extension", "GLX", "+render", "-noreset"],
                    stdout=xvfb_log_fp,
                    stderr=xvfb_log_fp,
                    start_new_session=True,
                )
                pids_recorded["xvfb"] = xvfb_proc.pid
                time.sleep(0.5)
                if xvfb_proc.poll() is not None:
                    print(f"[Silis VNC] [ERR] Xvfb exited unexpectedly with code {xvfb_proc.returncode}. Check {XVFB_LOG}")
            except Exception as e:
                print(f"[Silis VNC] [ERR] Failed to start Xvfb: {e}")

    # 2. Start x11vnc if not running
    if not status["x11vnc_running"]:
        if not x11vnc_bin:
            print("[Silis VNC] [WARN] x11vnc binary not found.")
        else:
            resolve_port_collision(RFB_PORT, "x11vnc", f"x11vnc.*{RFB_PORT}")
            print(f"[Silis VNC] Launching x11vnc on {DISPLAY_NUM} (RFB Port {RFB_PORT})...")
            try:
                x11vnc_log_fp = open(X11VNC_LOG, "a", encoding="utf-8")
                x11vnc_proc = subprocess.Popen(
                    [x11vnc_bin, "-display", DISPLAY_NUM, "-nopw", "-listen", "127.0.0.1", "-xkb", "-ncache", "10", "-ncache_cr", "-forever", "-shared", "-rfbport", str(RFB_PORT)],
                    stdout=x11vnc_log_fp,
                    stderr=x11vnc_log_fp,
                    start_new_session=True,
                )
                pids_recorded["x11vnc"] = x11vnc_proc.pid
                time.sleep(0.5)
                if x11vnc_proc.poll() is not None:
                    print(f"[Silis VNC] [ERR] x11vnc exited unexpectedly with code {x11vnc_proc.returncode}. Check {X11VNC_LOG}")
            except Exception as e:
                print(f"[Silis VNC] [ERR] Failed to start x11vnc: {e}")

    # 3. Start websockify if not running
    if not status["websockify_running"]:
        if not websockify_bin:
            print("[Silis VNC] [WARN] websockify binary not found.")
        else:
            resolve_port_collision(NOVNC_PORT, "websockify", f"websockify.*{NOVNC_PORT}")
            web_arg = ["--web", novnc_dir] if novnc_dir else []
            print(f"[Silis VNC] Launching websockify on port {NOVNC_PORT} -> {RFB_PORT}...")
            cmd = [websockify_bin] + web_arg + [str(NOVNC_PORT), f"127.0.0.1:{RFB_PORT}"]
            try:
                ws_log_fp = open(WEBSOCKIFY_LOG, "a", encoding="utf-8")
                ws_proc = subprocess.Popen(
                    cmd,
                    stdout=ws_log_fp,
                    stderr=ws_log_fp,
                    start_new_session=True,
                )
                pids_recorded["websockify"] = ws_proc.pid
                time.sleep(0.5)
                if ws_proc.poll() is not None:
                    print(f"[Silis VNC] [ERR] websockify exited unexpectedly with code {ws_proc.returncode}. Check {WEBSOCKIFY_LOG}")
            except Exception as e:
                print(f"[Silis VNC] [ERR] Failed to start websockify: {e}")

    try:
        with open(PID_FILE, "w", encoding="utf-8") as f:
            json.dump(pids_recorded, f, indent=2)
    except Exception:
        pass

    # 4. Configure Codespaces port visibility safely (defaults to private)
    cs_name = os.environ.get("CODESPACE_NAME", "")
    target_vis = os.environ.get("CODESPACE_PORT_VISIBILITY", "private")
    if cs_name and shutil.which("gh"):
        try:
            subprocess.run(
                ["gh", "codespace", "ports", "visibility", f"{NOVNC_PORT}:{target_vis}", "-c", cs_name],
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
                timeout=5,
            )
        except Exception:
            pass

    return get_vnc_status()


def stop_vnc():
    """Stops Xvfb, x11vnc, and websockify cleanly with timeout and SIGKILL fallback."""
    print("[Silis VNC] Stopping VNC infrastructure...")
    patterns = [f"websockify.*{NOVNC_PORT}", f"x11vnc.*{DISPLAY_NUM}", f"x11vnc.*{RFB_PORT}", f"Xvfb.*{DISPLAY_NUM}"]
    for pat in patterns:
        pids = get_active_pids(pat)
        for pid in pids:
            try:
                os.kill(pid, signal.SIGTERM)
            except Exception:
                pass
    time.sleep(0.5)
    for pat in patterns:
        pids = get_active_pids(pat)
        for pid in pids:
            try:
                os.kill(pid, signal.SIGKILL)
            except Exception:
                pass

    cleanup_stale_locks_and_zombies()
    if os.path.isfile(PID_FILE):
        try:
            os.remove(PID_FILE)
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
