FROM openroad/flow-ubuntu22.04-builder:latest

# Prevent interactive prompts
ENV DEBIAN_FRONTEND=noninteractive
ENV DISPLAY=:99
ENV QT_QPA_PLATFORM=xcb

# Install system dependencies for GUI and noVNC
RUN apt-get update && apt-get install -y \
    xvfb \
    x11vnc \
    novnc \
    websockify \
    openbox \
    xterm \
    dbus-x11 \
    python3-pip \
    libxcb-xinerama0 \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-randr0 \
    libxcb-render-util0 \
    libxcb-shape0 \
    libxcb-xkb1 \
    libxkbcommon-x11-0 \
    libdbus-1-3 \
    libgl1 \
    libglib2.0-0 \
    libfontconfig1 \
    libfreetype6 \
    libxcb-cursor0 \
    && rm -rf /var/lib/apt/lists/*

# Install Python dependencies for Silis
RUN pip3 install --no-cache-dir \
    PyQt6 \
    PyQt6-Qt6 \
    PyQt6-sip \
    gdstk \
    numpy

# Set up noVNC symlink (fix for missing index.html)
RUN ln -s /usr/share/novnc/vnc.html /usr/share/novnc/index.html

WORKDIR /workspaces/silis

# Ensure start.sh is executable when it's added
COPY start.sh /usr/local/bin/start-silis.sh
RUN chmod +x /usr/local/bin/start-silis.sh
