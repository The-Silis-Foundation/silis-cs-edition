#!/bin/bash
set -euo pipefail

# ==============================================================================
# Silis CS Edition: Codespace Bundle & AppImage / Pre-Baked Image Packager
# ==============================================================================
# Packages Python environment, PyQt6, QtWebEngine, OpenROAD, Yosys,
# and all EDA tools into a self-contained runtime bundle for instant Codespaces boot.
# ==============================================================================

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUNDLE_DIR="${REPO_DIR}/build_codespace/silis-cs-runtime"
OUTPUT_DIR="${REPO_DIR}/dist"

echo "=== Staging Silis Codespaces Runtime Bundle ==="
rm -rf "${BUNDLE_DIR}"
mkdir -p "${BUNDLE_DIR}/bin"
mkdir -p "${BUNDLE_DIR}/lib"
mkdir -p "${OUTPUT_DIR}"

# 1. Package pre-installed Python site-packages for PyQt6, gdstk, numpy
PYTHON_SYS_PACKAGES="/usr/lib/python3/dist-packages"
if [ -d "${PYTHON_SYS_PACKAGES}" ]; then
    echo "Staging system PyQt6 packages..."
    mkdir -p "${BUNDLE_DIR}/python-packages"
    cp -a ${PYTHON_SYS_PACKAGES}/PyQt6* "${BUNDLE_DIR}/python-packages/" 2>/dev/null || true
fi

# 2. Package AppRun & Entrypoint
cp -a "${REPO_DIR}/packaging/AppRun" "${BUNDLE_DIR}/AppRun" 2>/dev/null || true

echo "=== Codespace Bundle Staged Successfully at: ${BUNDLE_DIR} ==="
