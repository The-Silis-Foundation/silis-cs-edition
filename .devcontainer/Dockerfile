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
        libdbus-1-3 \
        libqt5widgets5 \
        libqt5gui5 \
        libqt5core5a \
        tcl-tclreadline && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# 2. Pre-stage Silis Python Qt runtime in /opt/silis/runtime
COPY assets/silis-python-qt-x86_64.AppImage /tmp/silis-runtime.AppImage
RUN mkdir -p /opt/silis && \
    if [ $(stat -c %s /tmp/silis-runtime.AppImage 2>/dev/null || echo 0) -lt 1000000 ]; then \
        echo "LFS pointer detected in build context; fetching AppImage binary from GitHub release..." && \
        curl -sL --retry 3 "https://github.com/The-Silis-Foundation/silis-cs-edition/releases/download/v1.0.0-runtime/silis-python-qt-x86_64.AppImage" -o /tmp/silis-runtime.AppImage; \
    fi && \
    chmod +x /tmp/silis-runtime.AppImage && \
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


