#!/bin/bash
echo "Starting Silis EDA Codespaces Edition..."

# Start virtual display
Xvfb :99 -screen 0 1920x1080x24 &
export DISPLAY=:99
sleep 2

# Start VNC on 5900 (standard for noVNC websockify)
x11vnc -display :99 -nopw -forever -quiet -rfbport 5900 &
sleep 1

# Start noVNC on port 6080
websockify --web /usr/share/novnc 6080 localhost:5900 &
sleep 1

echo "✅ Silis EDA Display Server is running!"
echo "✅ Click the PORTS tab → open port 6080"

# Start window manager (so you can move windows)
openbox-session &
sleep 1

# Open a terminal for the user (in case things go wrong)
xterm -geometry 100x30+10+10 -T "Silis EDA Terminal" -e /bin/bash &
sleep 1

# Launch Silis app in the background
# Defaulting to eatheswar/pocpnrv37.py as it seems to be the current one.
python3 /workspaces/silis/dev_eatheswar/pocpnrv37.py &

# Keep script alive
wait
