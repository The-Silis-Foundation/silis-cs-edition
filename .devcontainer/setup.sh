#!/usr/bin/env bash
set -e

WORKSPACE_DIR="/workspaces/silis-cs-edition"
[ -d "$WORKSPACE_DIR" ] || WORKSPACE_DIR="$(pwd)"

# 1. Submodules
git -C "$WORKSPACE_DIR" submodule update --init silis 2>/dev/null || true

# 2. Extract AppImage runtime into /opt/silis/runtime
OPT_DIR="/opt/silis/runtime"
if [ ! -d "$OPT_DIR" ]; then
    sudo mkdir -p /opt/silis "${WORKSPACE_DIR}/assets"
    if [ ! -s "${WORKSPACE_DIR}/assets/silis-python-qt-x86_64.AppImage" ]; then
        curl -sL "https://github.com/The-Silis-Foundation/silis-cs-edition/releases/download/v1.0.0-runtime/silis-python-qt-x86_64.AppImage" \
            -o "${WORKSPACE_DIR}/assets/silis-python-qt-x86_64.AppImage"
    fi
    chmod +x "${WORKSPACE_DIR}/assets/silis-python-qt-x86_64.AppImage"
    (cd /opt/silis && sudo "${WORKSPACE_DIR}/assets/silis-python-qt-x86_64.AppImage" --appimage-extract && sudo mv squashfs-root runtime && sudo chmod -R a+rX /opt/silis)
fi

# 3. Environment & Symlinks (leave system /usr/bin/python3 untouched to preserve VS Code Server)
sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/local/bin/silis-python3 2>/dev/null || true
sudo ln -sf "${WORKSPACE_DIR}/assets/bin/silis" /usr/local/bin/silis 2>/dev/null || true

cat << 'EOF' | sudo tee /etc/profile.d/silis_env.sh > /dev/null
export PATH="/opt/silis/runtime/opt/python/bin:/workspaces/silis-cs-edition/assets/bin:$PATH"
export PYTHONPATH="/workspaces/silis-cs-edition:/workspaces/silis-cs-edition/silis:/workspaces/silis-cs-edition/silis/core:$PYTHONPATH"
export LD_LIBRARY_PATH="/opt/silis/runtime/usr/lib/x86_64-linux-gnu:/opt/silis/runtime/opt/python/lib:$LD_LIBRARY_PATH"
export LIBGL_ALWAYS_SOFTWARE=1
export QT_QPA_PLATFORM=xcb
EOF

# 4. Install local extension with correct non-root permissions
CODESPACE_USER="$(id -un 1000 2>/dev/null || echo codespace)"
CODESPACE_HOME="$(getent passwd "$CODESPACE_USER" 2>/dev/null | cut -d: -f6 || echo "/home/$CODESPACE_USER")"
if [ -d "$CODESPACE_HOME" ]; then
    EXT_DIR="${CODESPACE_HOME}/.vscode-remote/extensions/the-silis-foundation.silis-cad-workbench-1.0.0"
    mkdir -p "$(dirname "$EXT_DIR")"
    rm -rf "$EXT_DIR"
    cp -r "${WORKSPACE_DIR}/extension" "$EXT_DIR" 2>/dev/null || true
    sudo chown -R 1000:1000 "$CODESPACE_HOME" 2>/dev/null || true
fi

# 5. Start Web CAD Server on Port 8765
if [ -f "${WORKSPACE_DIR}/tools/cs_server.py" ]; then
    pkill -f "cs_server.py" 2>/dev/null || true
    nohup python3 "${WORKSPACE_DIR}/tools/cs_server.py" 8765 > /tmp/cs_cad_server.log 2>&1 &
fi

echo "Silis CS Edition setup completed successfully."
