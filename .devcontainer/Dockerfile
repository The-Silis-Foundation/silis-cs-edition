FROM mcr.microsoft.com/devcontainers/base:ubuntu-22.04

# Minimal host utilities for noVNC, Xvfb, and AppImage retrieval.
# All Python 3.12, PyQt6, QtWebEngine, Tcl 8.6, Tk 8.6, Mesa OpenGL,
# and runtime shared libraries are bundled directly in the standalone
# silis-python-qt-x86_64.AppImage unpacked by setup.sh into /opt/silis/runtime.
RUN apt-get update && apt-get install -y --no-install-recommends \
    xvfb \
    x11vnc \
    novnc \
    websockify \
    net-tools \
    wget \
    curl \
    && rm -rf /var/lib/apt/lists/*
