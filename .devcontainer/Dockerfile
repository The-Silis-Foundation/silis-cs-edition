FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

# Lightweight system tools for noVNC, Xvfb, Mesa OpenGL, and Qt6 runtime dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    openssh-server \
    xvfb \
    x11vnc \
    novnc \
    websockify \
    net-tools \
    libgl1-mesa-dri \
    libgl1 \
    libglx-mesa0 \
    libosmesa6-dev \
    libegl1 \
    libegl-mesa0 \
    libxcomposite1 \
    libxdamage1 \
    libxrender1 \
    libxrandr2 \
    libxtst6 \
    libxi6 \
    libxkbcommon-x11-0 \
    libxkbcommon0 \
    libxcb-cursor0 \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-randr0 \
    libxcb-render-util0 \
    libxcb-render0 \
    libxcb-shape0 \
    libxcb-sync1 \
    libxcb-util1 \
    libxcb-xfixes0 \
    libxcb-xinerama0 \
    libxcb-xkb1 \
    libfontconfig1 \
    libnss3 \
    libnspr4 \
    libasound2t64 \
    libtcl8.6 \
    libtk8.6 \
    git-lfs \
    wget \
    zsync \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /var/run/sshd
