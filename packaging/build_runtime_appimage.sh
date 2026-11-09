#!/bin/bash
set -euo pipefail

# ==============================================================================
# Silis CS Edition: Standalone Python & Qt AppImage Builder
# ==============================================================================
# Packages ONLY the self-contained CPython 3.12 runtime and pre-installed
# PyQt6, PyQt6-WebEngine, PyQt6-QScintilla, gdstk, numpy, and websockify
# into a lightweight, standalone AppImage ("silis-python-qt-x86_64.AppImage").
# ==============================================================================

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${REPO_DIR}/build_runtime"
APPDIR="${BUILD_DIR}/SilisRuntime.AppDir"
OUTPUT_DIR="${REPO_DIR}/dist"
CACHE_DIR="/home/econsystems/silisfoundation/silis/build_appimage/cache"
APPIMAGETOOL="/home/econsystems/silisfoundation/silis/build_appimage/appimagetool-x86_64.AppImage"

echo "=== Staging Silis Python + Qt AppDir at: ${APPDIR} ==="
rm -rf "${APPDIR}"
mkdir -p "${APPDIR}/opt"
mkdir -p "${APPDIR}/usr/bin"
mkdir -p "${OUTPUT_DIR}"

# 1. Extract standalone CPython 3.12
PYTHON_TAR="${CACHE_DIR}/python-standalone.tar.gz"
if [ ! -f "${PYTHON_TAR}" ]; then
    echo "=== Fetching python-standalone archive ==="
    mkdir -p "${CACHE_DIR}"
    wget -c "https://github.com/astral-sh/python-build-standalone/releases/download/20240415/cpython-3.12.3+20240415-x86_64-unknown-linux-gnu-install_only.tar.gz" \
         -O "${PYTHON_TAR}"
fi

echo "=== Extracting Python 3.12 into AppDir/opt/python ==="
tar -xzf "${PYTHON_TAR}" -C "${APPDIR}/opt"

# 2. Install isolated Python wheels directly into AppDir
echo "=== Installing PyQt6, WebEngine, QScintilla, gdstk, numpy, websockify ==="
PYTHONNOUSERSITE=1 "${APPDIR}/opt/python/bin/pip3" install --no-cache-dir \
    PyQt6 \
    PyQt6-WebEngine \
    PyQt6-QScintilla \
    gdstk \
    numpy \
    websockify

# 3. Create AppRun script for the Python/Qt runtime container
cat << 'APPRUN_EOF' > "${APPDIR}/AppRun"
#!/bin/bash
set -e

if [ -z "$APPDIR" ]; then
    APPDIR="$(cd "$(dirname "$0")" && pwd)"
fi

PY_HOME="$APPDIR/opt/python"
PY_SITE="$PY_HOME/lib/python3.12/site-packages"
QT6_LIB="$PY_SITE/PyQt6/Qt6/lib"
WEBENGINE_LIB="$PY_SITE/PyQt6_WebEngine/Qt6/lib"

export PYTHONHOME="$PY_HOME"
export PATH="$PY_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$PY_HOME/lib:$QT6_LIB:$WEBENGINE_LIB:$LD_LIBRARY_PATH"
export PYTHONPATH="$PY_SITE:$PYTHONPATH"

# Qt WebEngine discovery
QTWEBENGINE_EXE="$PY_SITE/PyQt6_WebEngine/Qt6/libexec/QtWebEngineProcess"
if [ -f "$QTWEBENGINE_EXE" ]; then
    export QTWEBENGINEPROCESS_PATH="$QTWEBENGINE_EXE"
    export QTWEBENGINE_RESOURCES_PATH="$PY_SITE/PyQt6_WebEngine/Qt6/resources"
    export QTWEBENGINE_LOCALES_PATH="$PY_SITE/PyQt6_WebEngine/Qt6/translations/qtwebengine_locales"
fi

if [ "$#" -eq 0 ]; then
    exec "$PY_HOME/bin/python3"
else
    exec "$PY_HOME/bin/python3" "$@"
fi
APPRUN_EOF
chmod +x "${APPDIR}/AppRun"

# 4. Desktop file and icon metadata
cat << 'DESKTOP_EOF' > "${APPDIR}/silis-runtime.desktop"
[Desktop Entry]
Type=Application
Name=Silis Python Qt Runtime
Exec=AppRun
Icon=python
Categories=Development;
DESKTOP_EOF

cp "${REPO_DIR}/core/assets/silisicon.png" "${APPDIR}/python.png" 2>/dev/null || \
touch "${APPDIR}/python.png"

# 5. Build the Standalone Runtime AppImage
echo "=== Packaging silis-python-qt-x86_64.AppImage ==="
export ARCH=x86_64
"${APPIMAGETOOL}" "${APPDIR}" "${OUTPUT_DIR}/silis-python-qt-x86_64.AppImage"

echo "=== Runtime AppImage Build Complete: ${OUTPUT_DIR}/silis-python-qt-x86_64.AppImage ==="
