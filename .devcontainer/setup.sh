#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WORKSPACE_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

# 1. Check and init git submodule silis
git -C "$WORKSPACE_DIR" submodule update --init silis 2>/dev/null || true

# 1b. Stage precompiled native C++ viewer libraries into submodule
if [ -d "${WORKSPACE_DIR}/assets/lib" ]; then
    mkdir -p "${WORKSPACE_DIR}/silis/core/layer_viewer" \
             "${WORKSPACE_DIR}/silis/core/backendflow/siliconpeeker" \
             "${WORKSPACE_DIR}/silis/core/backendflow/fast_viewer/build"
    cp -f "${WORKSPACE_DIR}/assets/lib/silis_layer_viewer"*.so "${WORKSPACE_DIR}/silis/core/layer_viewer/" 2>/dev/null || true
    cp -f "${WORKSPACE_DIR}/assets/lib/fast_layout_viewer"*.so "${WORKSPACE_DIR}/silis/core/backendflow/siliconpeeker/" 2>/dev/null || true
    cp -f "${WORKSPACE_DIR}/assets/lib/fast_layout_viewer"*.so "${WORKSPACE_DIR}/silis/core/backendflow/fast_viewer/build/" 2>/dev/null || true
    cp -f "${WORKSPACE_DIR}/assets/lib/fast_layout_viewer"*.so "${WORKSPACE_DIR}/silis/core/backendflow/fast_viewer/" 2>/dev/null || true
fi

# 2. Extract AppImage runtime into /opt/silis/runtime if not present
OPT_DIR="/opt/silis/runtime"
APPIMAGE="${WORKSPACE_DIR}/assets/silis-python-qt-x86_64.AppImage"
EXPECTED_SIZE=305945080
if [ ! -d "$OPT_DIR" ]; then
    sudo mkdir -p /opt/silis "${WORKSPACE_DIR}/assets"
    CURRENT_SIZE="$(stat -c %s "$APPIMAGE" 2>/dev/null || echo 0)"
    if [ "$CURRENT_SIZE" -ne "$EXPECTED_SIZE" ]; then
        echo "Fetching silis runtime AppImage (expected size: $EXPECTED_SIZE bytes)..."
        rm -f "$APPIMAGE"
        curl -sL --retry 3 "https://github.com/The-Silis-Foundation/silis-cs-edition/releases/download/v1.0.0-runtime/silis-python-qt-x86_64.AppImage" \
            -o "$APPIMAGE"
    fi
    chmod +x "$APPIMAGE"
    (cd /opt/silis && sudo "$APPIMAGE" --appimage-extract && sudo mv squashfs-root runtime && sudo chmod -R a+rX /opt/silis)
fi

# 3. Install xvfb and x11vnc for headless CAD display bridge (zero daemon ports)
if command -v apt-get >/dev/null 2>&1; then
    echo "Installing xvfb and x11vnc for native CAD layout display bridge..."
    sudo apt-get update -qq && sudo apt-get install -y -qq xvfb x11vnc 2>/dev/null || true
fi

# 4. DO NOT overwrite system /usr/bin/python3. Create /usr/local/bin/silis-python3 and export PATH in /etc/profile.d/silis_env.sh
sudo ln -sf /opt/silis/runtime/opt/python/bin/python3 /usr/local/bin/silis-python3 2>/dev/null || true
sudo ln -sf "${WORKSPACE_DIR}/assets/bin/silis" /usr/local/bin/silis 2>/dev/null || true

cat << EOF | sudo tee /etc/profile.d/silis_env.sh > /dev/null
export PATH="/opt/silis/runtime/opt/python/bin:${WORKSPACE_DIR}/assets/bin:\$PATH"
export PYTHONPATH="${WORKSPACE_DIR}:${WORKSPACE_DIR}/silis:${WORKSPACE_DIR}/silis/core:\$PYTHONPATH"
export LD_LIBRARY_PATH="/opt/silis/runtime/usr/lib/x86_64-linux-gnu:/opt/silis/runtime/opt/python/lib:\$LD_LIBRARY_PATH"
export LIBGL_ALWAYS_SOFTWARE=1
export QT_QPA_PLATFORM=xcb
export DISPLAY=:1
EOF

# Ensure interactive terminals source environment
if [ -f /etc/bash.bashrc ] && ! grep -q "silis_env.sh" /etc/bash.bashrc 2>/dev/null; then
    echo "[ -f /etc/profile.d/silis_env.sh ] && . /etc/profile.d/silis_env.sh" | sudo tee -a /etc/bash.bashrc >/dev/null || true
fi

# 5. Install and register the Silis CAD Workbench extension
VSIX_PATH="${WORKSPACE_DIR}/extension/silis-cad-workbench-1.0.0.vsix"
if command -v code >/dev/null 2>&1 && [ -f "$VSIX_PATH" ]; then
    code --install-extension "$VSIX_PATH" --force 2>/dev/null || true
fi

# 5. Enforce non-root ownership on home directory and register extension in extensions.json
CODESPACE_USER="$(id -un 1000 2>/dev/null || echo codespace)"
CODESPACE_HOME="$(getent passwd "$CODESPACE_USER" 2>/dev/null | cut -d: -f6 || echo "/home/$CODESPACE_USER")"
if [ -d "$CODESPACE_HOME" ]; then
    sudo chown -R 1000:1000 "$CODESPACE_HOME" 2>/dev/null || true
    for EDIR in "${CODESPACE_HOME}/.vscode-remote/extensions" "${CODESPACE_HOME}/.vscode-server/extensions"; do
        mkdir -p "$EDIR" 2>/dev/null || true
        TARGET="${EDIR}/the-silis-foundation.silis-cad-workbench-1.0.0"
        rm -rf "$TARGET"
        cp -r "${WORKSPACE_DIR}/extension" "$TARGET" 2>/dev/null || true
        # Register in extensions.json database
        python3 -c "
import json, os
from pathlib import Path
ext_id = 'the-silis-foundation.silis-cad-workbench'
target = Path('$TARGET')
edir = target.parent
json_file = edir / 'extensions.json'
items = []
if json_file.is_file():
    try:
        items = json.loads(json_file.read_text(encoding='utf-8'))
    except Exception:
        items = []
if not any(x.get('identifier', {}).get('id') == ext_id for x in items):
    items.append({
        'identifier': {'id': ext_id},
        'version': '1.0.0',
        'location': {'\$mid': 1, 'path': str(target), 'scheme': 'file'},
        'relativeLocation': target.name,
        'metadata': {'installedTimestamp': 1762832000000}
    })
    json_file.write_text(json.dumps(items), encoding='utf-8')
" 2>/dev/null || true
        chown -R 1000:1000 "$TARGET" 2>/dev/null || true
        chown 1000:1000 "$EDIR/extensions.json" 2>/dev/null || true
    done
fi

echo "Silis CS Edition setup completed successfully."
