#!/usr/bin/env python3
"""
Silis CS Edition Headless Physical Design & Flow Orchestrator
Executes Simulation, Synthesis, and OpenROAD PnR flows directly from CLI/terminal
without launching PyQt6 desktop GUI windows.
"""

import os
import sys
import glob
import re
import json
import shutil
import subprocess
import argparse

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
BIN_DIR = os.path.join(REPO_ROOT, "third-party", "bin")
VERILATOR_BIN = os.path.join(BIN_DIR, "verilator")
VERILATOR_ROOT = os.path.join(REPO_ROOT, "third-party", "share", "verilator")
YOSYS_BIN = os.path.join(BIN_DIR, "yosys")
if not os.path.isfile(YOSYS_BIN):
    YOSYS_BIN = shutil.which("yosys") or "yosys"
OPENROAD_BIN = os.path.join(BIN_DIR, "openroad")
if not os.path.isfile(OPENROAD_BIN):
    OPENROAD_BIN = shutil.which("openroad") or "openroad"


def get_project_dir(proj_arg):
    if not proj_arg:
        return os.path.join(REPO_ROOT, "projects", "demo_soc")
    if os.path.isabs(proj_arg) and os.path.isdir(proj_arg):
        return proj_arg

    clean = proj_arg.replace("projects/", "").strip("/")
    cands = [
        os.path.join(REPO_ROOT, "projects", clean),
        os.path.join(REPO_ROOT, proj_arg),
        os.path.join(REPO_ROOT, clean),
    ]
    for c in cands:
        if os.path.isdir(c):
            return c

    # Fuzzy search projects/ by project_name or top_module in .silisproj
    proj_dir = os.path.join(REPO_ROOT, "projects")
    if os.path.isdir(proj_dir):
        for item in os.listdir(proj_dir):
            p = os.path.join(proj_dir, item)
            if not os.path.isdir(p):
                continue
            for sp_name in ["silis.silisproj", f"{item}.silisproj"]:
                sp = os.path.join(p, sp_name)
                if os.path.isfile(sp):
                    try:
                        with open(sp) as f:
                            meta = json.load(f)
                            if meta.get("project_name") == clean or meta.get("top_module") == clean:
                                return p
                    except Exception:
                        pass

    return os.path.join(REPO_ROOT, "projects", clean)


def run_sim(project_dir):
    print(f"[Silis Flow] === Starting Simulation Flow for {os.path.basename(project_dir)} ===")
    
    # 1. Discover all HDL sources and testbenches
    sources = []
    tb_files = []
    for d in ["rtl", "source", "tb", "."]:
        p = os.path.join(project_dir, d)
        if os.path.isdir(p):
            for f in sorted(os.listdir(p)):
                if f.endswith(".v") or f.endswith(".sv"):
                    full_p = os.path.join(p, f)
                    if f.startswith("tb_") or "_tb" in f:
                        tb_files.append(full_p)
                    else:
                        sources.append(full_p)
    
    if not sources and not tb_files:
        print("[Silis Flow] [ERR] No Verilog/SystemVerilog sources found in project.")
        return 1

    # Find top module
    top_module = None
    cfg_file = os.path.join(project_dir, "silis.silisproj")
    if os.path.isfile(cfg_file):
        try:
            with open(cfg_file) as f:
                top_module = json.load(f).get("top_module")
        except Exception:
            pass

    # Discover module names declared across files
    declared_modules = {}
    for f in sources + tb_files:
        try:
            with open(f, "r", encoding="utf-8", errors="ignore") as fp:
                for m in re.finditer(r'\bmodule\s+([a-zA-Z0-9_]+)', fp.read()):
                    declared_modules[m.group(1)] = f
        except Exception:
            pass

    # Find simulation testbench top
    sim_top = None
    for m in declared_modules:
        if m.startswith("tb_") or m.endswith("_tb") or m == "tb":
            sim_top = m
            break
    if not sim_top and tb_files:
        sim_top = list(declared_modules.keys())[-1]
    if not sim_top:
        sim_top = top_module or (list(declared_modules.keys())[0] if declared_modules else "top")

    print(f"[Silis Flow] Detected Simulation Top: {sim_top}")
    print(f"[Silis Flow] Compiling {len(sources)} RTL sources + {len(tb_files)} testbench files...")

    # Inject VCD waveform dumping if not present
    vcd_out = os.path.join(project_dir, "waveform.vcd")
    has_dump = False
    for f in tb_files:
        try:
            with open(f, "r", encoding="utf-8", errors="ignore") as fp:
                if "$dumpfile" in fp.read():
                    has_dump = True
                    break
        except Exception:
            pass

    auto_dump_file = None
    if not has_dump and sim_top:
        # Create lightweight VCD dump companion
        os.makedirs(os.path.join(project_dir, "build"), exist_ok=True)
        auto_dump_file = os.path.join(project_dir, "build", "silis_vcd_dump.sv")
        with open(auto_dump_file, "w", encoding="utf-8") as f:
            f.write(f"""// Auto-injected by Silis CS Edition for instant waveform generation
module _silis_vcd_tracer;
    initial begin
        $dumpfile("{vcd_out}");
        $dumpvars(0, {sim_top});
    end
endmodule
""")

    compile_files = list(sources) + list(tb_files)
    if auto_dump_file:
        compile_files.append(auto_dump_file)

    # 2. Build and run with Verilator
    env = os.environ.copy()
    if os.path.isdir(VERILATOR_ROOT):
        env["VERILATOR_ROOT"] = VERILATOR_ROOT
    env["OBJCACHE"] = ""

    obj_dir = os.path.join(project_dir, "build", "obj_dir")
    os.makedirs(obj_dir, exist_ok=True)
    out_bin = os.path.join(obj_dir, f"V{sim_top}")

    inc_args = [f"-I{project_dir}/rtl", f"-I{project_dir}/tb", f"-I{project_dir}/source", f"-I{project_dir}"]
    v_cmd = [
        VERILATOR_BIN, "--binary", "--timing", "-sv",
        "--timescale", "1ns/1ps",
        "--trace", "-Wno-fatal",
        "-Wno-TIMESCALEMOD", "-Wno-MODDUP",
        "--Mdir", obj_dir,
        "-o", out_bin,
        f"--top-module", sim_top
    ] + inc_args + compile_files

    print(f"[Silis Flow] Executing Verilator native compilation...")
    res = subprocess.run(v_cmd, cwd=project_dir, env=env)
    if res.returncode != 0:
        print(f"[Silis Flow] [ERR] Verilator compilation failed with exit code {res.returncode}")
        return res.returncode

    # Run binary
    print(f"[Silis Flow] Running simulation binary: {out_bin}...")
    sim_proc = subprocess.run([out_bin], cwd=project_dir, env=env)
    if sim_proc.returncode != 0:
        print(f"[Silis Flow] [WARN] Simulation exited with code {sim_proc.returncode}")

    if os.path.isfile(vcd_out) and os.path.getsize(vcd_out) > 0:
        print(f"[Silis Flow] [OK] Waveform generated successfully: {vcd_out} ({os.path.getsize(vcd_out)} bytes)")
    else:
        # Check if vcd was dumped elsewhere in project
        cand = glob.glob(os.path.join(project_dir, "**", "*.vcd"), recursive=True)
        if cand:
            shutil.copy2(cand[0], vcd_out)
            print(f"[Silis Flow] [OK] Waveform captured: {vcd_out}")

    # Generate schematic netlist json automatically as part of simulation (matching Silis Desktop parity)
    run_synth(project_dir, json_only=True)
    print(f"[Silis Flow] === Simulation Flow Complete ===")
    return 0


def run_synth(project_dir, json_only=False):
    print(f"[Silis Flow] === Running Synthesis for {os.path.basename(project_dir)} ===")
    os.makedirs(os.path.join(project_dir, "build"), exist_ok=True)
    out_json = os.path.join(project_dir, "build", "structural_synth.json")
    out_v = os.path.join(project_dir, "build", "synth.v")

    sources = []
    for d in ["rtl", "source", "."]:
        p = os.path.join(project_dir, d)
        if os.path.isdir(p):
            for f in sorted(os.listdir(p)):
                if (f.endswith(".v") or f.endswith(".sv")) and not f.startswith("tb_"):
                    sources.append(os.path.join(p, f))

    if not sources:
        print("[Silis Flow] [ERR] No synthesizable RTL files found.")
        return 1

    read_cmds = [f"read_verilog -sv {s}" if s.endswith(".sv") else f"read_verilog {s}" for s in sources]
    script = "; ".join(read_cmds) + f"; hierarchy -auto-top; prep; write_json {out_json}"
    if not json_only:
        script += f"; synth -top full_adder; write_verilog {out_v}"

    cmd = [YOSYS_BIN, "-q", "-p", script]
    res = subprocess.run(cmd, cwd=project_dir)
    if res.returncode == 0:
        print(f"[Silis Flow] [OK] Schematic Netlist generated: {out_json}")
        if not json_only:
            print(f"[Silis Flow] [OK] Gate-level netlist: {out_v}")
    else:
        print(f"[Silis Flow] [ERR] Yosys synthesis failed.")
    return res.returncode


def run_tapeout(project_dir):
    print(f"[Silis Flow] === Running Full Tapeout Flow (OpenROAD) ===")
    run_synth(project_dir, json_only=False)
    # Check if OpenROAD script exists
    fp_tcl = os.path.join(REPO_ROOT, "core", "backendflow", "floorplanner", "silis_fp.tcl")
    if os.path.isfile(fp_tcl):
        print(f"[Silis Flow] Synthesizing floorplan via OpenROAD...")
        subprocess.run([OPENROAD_BIN, fp_tcl], cwd=project_dir)
    print(f"[Silis Flow] Tapeout flow pass complete.")
    return 0


def run_openroad_gui(project_dir):
    print(f"[Silis Flow] === Launching OpenROAD Native GUI for {os.path.basename(project_dir)} ===")
    
    # 1. Locate DEF or ODB file
    def_candidates = [
        os.path.join(project_dir, "results", "temp.def"),
        os.path.join(project_dir, "results", "final_routed.def"),
        os.path.join(project_dir, "results", "routed.def"),
    ]
    def_candidates.extend(glob.glob(os.path.join(project_dir, "results", "*.def")))
    def_candidates.extend(glob.glob(os.path.join(project_dir, "*.def")))
    def_candidates.extend(glob.glob(os.path.join(project_dir, "**", "*.def"), recursive=True))

    def_file = None
    for cand in def_candidates:
        if os.path.isfile(cand) and os.path.getsize(cand) > 0:
            def_file = os.path.abspath(cand)
            break

    if not def_file:
        results_temp = os.path.join(REPO_ROOT, "results", "temp.def")
        if os.path.isfile(results_temp):
            def_file = results_temp
        else:
            print(f"[Silis Flow] [ERR] No placed/routed DEF file found in {project_dir}")
            return 1

    print(f"[Silis Flow] Loading physical layout: {def_file}")

    # 2. Locate LEF files
    lef_dirs = [
        os.path.expanduser("~/.silis/pdks/sky130"),
        os.path.expanduser("~/.silis/pdks"),
        os.path.join(REPO_ROOT, "third-party", "pdks"),
        os.path.join(REPO_ROOT, "..", "silis", "third-party", "pdks"),
        os.path.join(project_dir, "source"),
        os.path.join(project_dir, "rtl"),
        os.path.join(project_dir, "const"),
        project_dir,
    ]

    tlef_file = None
    sc_lef_file = None
    macro_lefs = []

    for d in lef_dirs:
        if not os.path.isdir(d):
            continue
        for f in os.listdir(d):
            if f.endswith(".tlef") and not tlef_file:
                tlef_file = os.path.join(d, f)
            elif f.endswith(".lef"):
                full_path = os.path.join(d, f)
                if ("sc_hd" in f or "std_cell" in f) and not sc_lef_file:
                    sc_lef_file = full_path
                else:
                    if full_path not in macro_lefs:
                        macro_lefs.append(full_path)

    # 3. Generate OpenROAD loader script
    loader_script = os.path.join(os.path.dirname(def_file), "_openroad_gui_loader.tcl")
    lines = []
    if tlef_file:
        lines.append(f'read_lef "{tlef_file}"')
    if sc_lef_file:
        lines.append(f'read_lef "{sc_lef_file}"')
    for m in macro_lefs:
        lines.append(f'read_lef "{m}"')
    lines.append(f'read_def "{def_file}"')
    lines.append('gui::show')

    with open(loader_script, "w") as fp:
        fp.write("\n".join(lines) + "\n")

    print(f"[Silis Flow] Generated OpenROAD loader: {loader_script}")
    env = os.environ.copy()
    if "DISPLAY" not in env:
        env["DISPLAY"] = ":1"

    cmd = [OPENROAD_BIN, "-gui", loader_script]
    print(f"[Silis Flow] Executing: {' '.join(cmd)}")
    proc = subprocess.Popen(cmd, cwd=os.path.dirname(def_file), env=env)
    print(f"[Silis Flow] [OK] OpenROAD GUI launched (PID: {proc.pid})")
    return 0


def main():
    parser = argparse.ArgumentParser(description="Silis Headless Flow CLI")
    parser.add_argument("--sim", "--run-sim", dest="sim", action="store_true", help="Run RTL compile and simulation")
    parser.add_argument("--synth", "--run-synth", dest="synth", action="store_true", help="Run Yosys synthesis")
    parser.add_argument("--tapeout", "--run-all", dest="tapeout", action="store_true", help="Run full tapeout PnR")
    parser.add_argument("--view-openroad", "--openroad-gui", dest="view_openroad", action="store_true", help="Launch native OpenROAD GUI with project DEF/LEF")
    parser.add_argument("--project", type=str, default="", help="Project path or name")

    args = parser.parse_args()
    proj_dir = get_project_dir(args.project)

    if args.sim:
        sys.exit(run_sim(proj_dir))
    elif args.synth:
        sys.exit(run_synth(proj_dir))
    elif args.tapeout:
        sys.exit(run_tapeout(proj_dir))
    elif args.view_openroad:
        sys.exit(run_openroad_gui(proj_dir))
    else:
        # Default to sim
        sys.exit(run_sim(proj_dir))


if __name__ == "__main__":
    main()
