FROM mcr.microsoft.com/devcontainers/base:ubuntu-22.04

# Lightweight system tools for noVNC, Xvfb, Mesa OpenGL, and EDA dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    xvfb \
    x11vnc \
    novnc \
    websockify \
    net-tools \
    libgl1-mesa-dri \
    libgl1-mesa-glx \
    libosmesa6-dev \
    tcl8.6 \
    tk8.6 \
    wget \
    zsync \
    curl \
    && rm -rf /var/lib/apt/lists/*
