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

echo "✅ Silis EDA is running!"
echo "✅ Click the PORTS tab → open port 6080"

# Launch Silis app
# Defaulting to eatheswar/pocpnrv37.py as it seems to be the current one.
# Can be updated to your preference.
python3 /workspaces/silis/dev_eatheswar/pocpnrv37.py
