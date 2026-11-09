FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

# Pre-install OpenSSH Server for direct gh cs ssh inspection
RUN apt-get update && apt-get install -y --no-install-recommends \
    openssh-server \
    xvfb \
    x11vnc \
    novnc \
    websockify \
    net-tools \
    libgl1-mesa-dri \
    libgl1-mesa-glx \
    libosmesa6-dev \
    libtcl8.6 \
    libtk8.6 \
    python3 \
    python3-minimal \
    python3-pip \
    python3-pyqt6 \
    python3-pyqt6.qsci \
    python3-pyqt6.qtwebengine \
    wget \
    zsync \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /var/run/sshd
