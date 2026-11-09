FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

# Lightweight system tools for noVNC, Xvfb, and Mesa OpenGL
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
    wget \
    zsync \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /var/run/sshd
