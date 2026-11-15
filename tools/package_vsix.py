#!/usr/bin/env python3
import os
import zipfile
import shutil
import tempfile

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
EXT_DIR = os.path.join(REPO_ROOT, "extension")
VSIX_PATH = os.path.join(EXT_DIR, "silis-cad-workbench-1.0.0.vsix")

def main():
    print(f"[VSIX Packager] Building {VSIX_PATH}...")
    temp_dir = tempfile.mkdtemp(prefix="silis_vsix_")
    try:
        # 1. Extract manifest files if existing VSIX exists
        if os.path.isfile(VSIX_PATH):
            with zipfile.ZipFile(VSIX_PATH, "r") as z:
                for item in ["[Content_Types].xml", "extension.vsixmanifest"]:
                    if item in z.namelist():
                        z.extract(item, temp_dir)

        # Ensure minimal [Content_Types].xml if missing
        ct_path = os.path.join(temp_dir, "[Content_Types].xml")
        if not os.path.isfile(ct_path):
            with open(ct_path, "w", encoding="utf-8") as f:
                f.write('''<?xml version="1.0" encoding="utf-8"?>
<Types xmlns="http://schemas.openxmlformats.org/package/2006/content-types">
  <Default Extension="xml" ContentType="text/xml"/>
  <Default Extension="vsixmanifest" ContentType="text/xml"/>
  <Default Extension="json" ContentType="application/json"/>
  <Default Extension="js" ContentType="application/javascript"/>
  <Default Extension="html" ContentType="text/html"/>
  <Default Extension="css" ContentType="text/css"/>
  <Default Extension="wasm" ContentType="application/wasm"/>
  <Default Extension="png" ContentType="image/png"/>
  <Default Extension="svg" ContentType="image/svg+xml"/>
</Types>''')

        # Ensure minimal extension.vsixmanifest if missing
        mf_path = os.path.join(temp_dir, "extension.vsixmanifest")
        if not os.path.isfile(mf_path):
            with open(mf_path, "w", encoding="utf-8") as f:
                f.write('''<?xml version="1.0" encoding="utf-8"?>
<PackageManifest Version="2.0.0" xmlns="http://schemas.microsoft.com/developer/vsx-schema/2011">
  <Metadata>
    <Identity Id="silis-cad-workbench" Version="1.0.0" Publisher="the-silis-foundation"/>
    <DisplayName>Silis ASIC CAD Workbench</DisplayName>
    <Description xml:space="preserve">Cloud EDA workbench for GitHub Codespaces & VS Code Web.</Description>
  </Metadata>
  <Installation>
    <InstallationTarget Id="Microsoft.VisualStudio.Code"/>
  </Installation>
  <Assets>
    <Asset Type="Microsoft.VisualStudio.Code.Manifest" Path="extension/package.json" Addressable="true"/>
  </Assets>
</PackageManifest>''')

        # 2. Copy extension files into temp_dir/extension
        stage_ext = os.path.join(temp_dir, "extension")
        os.makedirs(stage_ext, exist_ok=True)
        shutil.copy2(os.path.join(EXT_DIR, "package.json"), os.path.join(stage_ext, "package.json"))
        shutil.copy2(os.path.join(EXT_DIR, "extension.js"), os.path.join(stage_ext, "extension.js"))
        shutil.copytree(os.path.join(EXT_DIR, "web"), os.path.join(stage_ext, "web"))

        # 3. Create zip archive
        new_vsix = VSIX_PATH + ".tmp"
        with zipfile.ZipFile(new_vsix, "w", compression=zipfile.ZIP_DEFLATED) as z:
            for root, dirs, files in os.walk(temp_dir):
                for f in files:
                    full_p = os.path.join(root, f)
                    rel_p = os.path.relpath(full_p, temp_dir)
                    z.write(full_p, rel_p)

        os.replace(new_vsix, VSIX_PATH)
        size_mb = os.path.getsize(VSIX_PATH) / (1024 * 1024)
        print(f"[VSIX Packager] [OK] Successfully packaged {VSIX_PATH} ({size_mb:.2f} MB).")
    finally:
        shutil.rmtree(temp_dir, ignore_errors=True)

if __name__ == "__main__":
    main()
