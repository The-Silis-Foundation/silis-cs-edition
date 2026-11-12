#!/usr/bin/env bash
set -e

WORKSPACE_DIR="/workspaces/silis-cs-edition"
[ -d "$WORKSPACE_DIR" ] || WORKSPACE_DIR="$(pwd)"

# 1. Check and init git submodule silis
git -C "$WORKSPACE_DIR" submodule update --init silis 2>/dev/null || true

# 2. Extract AppImage runtime into /opt/silis/runtime if not present
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

# 3. DO NOT overwrite system /usr/bin/python3. Create /usr/local/bin/silis-python3 and export PATH in /etc/profile.d/silis_env.sh
sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/local/bin/silis-python3 2>/dev/null || true
sudo ln -sf "${WORKSPACE_DIR}/assets/bin/silis" /usr/local/bin/silis 2>/dev/null || true

cat << 'EOF' | sudo tee /etc/profile.d/silis_env.sh > /dev/null
export PATH="/opt/silis/runtime/opt/python/bin:/workspaces/silis-cs-edition/assets/bin:$PATH"
export PYTHONPATH="/workspaces/silis-cs-edition:/workspaces/silis-cs-edition/silis:/workspaces/silis-cs-edition/silis/core:$PYTHONPATH"
export LD_LIBRARY_PATH="/opt/silis/runtime/usr/lib/x86_64-linux-gnu:/opt/silis/runtime/opt/python/lib:$LD_LIBRARY_PATH"
export LIBGL_ALWAYS_SOFTWARE=1
export QT_QPA_PLATFORM=xcb
EOF

# 4. Enforce non-root ownership (UID 1000) on home directory. DO NOT manually create or chown ~/.vscode-server as root.
CODESPACE_USER="$(id -un 1000 2>/dev/null || echo codespace)"
CODESPACE_HOME="$(getent passwd "$CODESPACE_USER" 2>/dev/null | cut -d: -f6 || echo "/home/$CODESPACE_USER")"
if [ -d "$CODESPACE_HOME" ]; then
    sudo chown -R 1000:1000 "$CODESPACE_HOME" 2>/dev/null || true
    # Copy extension to non-root user extension directories with 1000:1000 ownership
    for EDIR in "${CODESPACE_HOME}/.vscode-remote/extensions" "${CODESPACE_HOME}/.vscode-server/extensions"; do
        mkdir -p "$EDIR" 2>/dev/null || true
        TARGET="${EDIR}/the-silis-foundation.silis-cad-workbench-1.0.0"
        rm -rf "$TARGET"
        cp -r "${WORKSPACE_DIR}/extension" "$TARGET" 2>/dev/null || true
        chown -R 1000:1000 "$TARGET" 2>/dev/null || true
    done
fi

echo "Silis CS Edition setup completed successfully."
