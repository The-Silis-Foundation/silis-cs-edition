FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

# 1. Pre-install openssh-server, xvfb, x11vnc, Python 3, Node.js, iverilog, yosys, and X11/GL libraries for native CAD layout viewers
RUN apt-get update -qq && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y -qq --no-install-recommends \
        openssh-server \
        git-lfs \
        xvfb \
        x11vnc \
        nodejs \
        iverilog \
        vvp \
        verilator \
        yosys \
        python3 \
        python3-pip \
        libx11-xcb1 \
        libxcb-glx0 \
        libxcb-keysyms1 \
        libxcb-image0 \
        libxcb-shm0 \
        libxcb-icccm4 \
        libxcb-sync1 \
        libxcb-xfixes0 \
        libxcb-shape0 \
        libxcb-randr0 \
        libxcb-render-util0 \
        libxkbcommon-x11-0 \
        libgl1-mesa-dri \
        libglib2.0-0t64 \
        libdbus-1-3 && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# 2. Pre-stage Silis Python Qt runtime in /opt/silis/runtime from repo assets via Git LFS (zero network fetch)
COPY assets/silis-python-qt-x86_64.AppImage /tmp/silis-runtime.AppImage
RUN chmod +x /tmp/silis-runtime.AppImage && \
    mkdir -p /opt/silis && \
    cd /opt/silis && \
    /tmp/silis-runtime.AppImage --appimage-extract && \
    mv squashfs-root runtime && \
    chmod -R a+rX /opt/silis && \
    rm -f /tmp/silis-runtime.AppImage && \
    ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/local/bin/silis-python3 && \
    /opt/silis/runtime/opt/python/bin/python3 -m pip install --no-cache-dir Pillow

# 3. Pre-configure system-wide environment
ENV DISPLAY=:1 \
    LIBGL_ALWAYS_SOFTWARE=1 \
    QT_QPA_PLATFORM=xcb \
    LD_LIBRARY_PATH=/opt/silis/runtime/usr/lib/x86_64-linux-gnu:/opt/silis/runtime/opt/python/lib:${LD_LIBRARY_PATH} \
    PATH=/opt/silis/runtime/opt/python/bin:/workspaces/silis-cs-edition/assets/bin:${PATH}


