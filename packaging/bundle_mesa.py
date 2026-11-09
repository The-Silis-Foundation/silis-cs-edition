import os, shutil, glob

def bundle_mesa(appdir):
    lib_dir = os.path.join(appdir, "usr", "lib", "x86_64-linux-gnu")
    dri_dir = os.path.join(lib_dir, "dri")
    os.makedirs(dri_dir, exist_ok=True)
    
    # 1. Copy libEGL, libGL, libGLX, libGLdispatch, libOpenGL, libOSMesa
    patterns = [
        "/usr/lib/x86_64-linux-gnu/libGL*.so*",
        "/usr/lib/x86_64-linux-gnu/libEGL*.so*",
        "/usr/lib/x86_64-linux-gnu/libOpenGL*.so*",
        "/usr/lib/x86_64-linux-gnu/libOSMesa*.so*",
        "/usr/lib/x86_64-linux-gnu/libgbm*.so*",
        "/usr/lib/x86_64-linux-gnu/libdrm*.so*",
    ]
    for p in patterns:
        for f in glob.glob(p):
            base = os.path.basename(f)
            dest = os.path.join(lib_dir, base)
            if not os.path.exists(dest):
                try:
                    if os.path.islink(f):
                        target = os.readlink(f)
                        os.symlink(target, dest)
                    else:
                        shutil.copy2(f, dest)
                except Exception as e:
                    pass

    # 2. Copy software rasterizer DRI driver (swrast_dri.so, kms_swrast_dri.so)
    for dri in glob.glob("/usr/lib/x86_64-linux-gnu/dri/*swrast*.so*") + glob.glob("/usr/lib/x86_64-linux-gnu/dri/libdril_dri.so"):
        base = os.path.basename(dri)
        dest = os.path.join(dri_dir, base)
        if not os.path.exists(dest):
            try:
                if os.path.islink(dri):
                    os.symlink(os.readlink(dri), dest)
                else:
                    shutil.copy2(dri, dest)
            except Exception:
                pass
    print("Mesa and DRI drivers bundled successfully.")

if __name__ == "__main__":
    import sys
    bundle_mesa(sys.argv[1])
