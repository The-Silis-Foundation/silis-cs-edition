#!/usr/bin/env python3
"""
run_gui_flow.py
Executable CLI runner for Autonomous Agents and Subagents.
Drives Silis ASIC Flow using the headless GUI driver (SilisGUIDriver).

Arguments:
  --project <name_or_path>
  --run all | synth | init | floorplan | place | cts | route | gds
  --step <single_step>
  --stop-at <step>
  --signoff PAT | STA | DRC
  --json-out <path>

Exit codes:
  0: Success (all requested steps / signoffs passed)
  1: Tool error detected (OpenROAD, Yosys, or Magic crash/error)
  2: Timing violation detected (STA / PAT slack violated)
"""

import os
import sys
import argparse

ENV_DIR = os.path.dirname(os.path.abspath(__file__))
SILIS_ROOT = os.path.dirname(ENV_DIR)

if SILIS_ROOT not in sys.path:
    sys.path.insert(0, SILIS_ROOT)
if ENV_DIR not in sys.path:
    sys.path.insert(0, ENV_DIR)

from gui_driver import SilisGUIDriver


def main():
    parser = argparse.ArgumentParser(
        description="Silis Autonomous GUI Flow Runner for Agents & CLI",
        formatter_class=argparse.RawTextHelpFormatter
    )
    parser.add_argument(
        "--project", "-p",
        default=None,
        help="Project name or path to silis.silisproj directory"
    )
    parser.add_argument(
        "--run", "-r",
        choices=["all", "synth", "init", "floorplan", "place", "cts", "route", "gds"],
        help="Run predefined flow pipeline up to target"
    )
    parser.add_argument(
        "--step", "-s",
        help="Execute a single step (e.g. Init, Floorplan, Tapcells, Place, CTS, Route, GDS)"
    )
    parser.add_argument(
        "--stop-at",
        help="Stop sequence execution after reaching specified step"
    )
    parser.add_argument(
        "--signoff",
        choices=["PAT", "STA", "DRC", "DFT"],
        help="Run post-layout signoff timing, power, DRC, or DFT analysis"
    )
    parser.add_argument(
        "--json-out",
        help="Path for saving structured JSON debug report"
    )
    parser.add_argument(
        "--live",
        dest="headless",
        action="store_false",
        help="Run in live visible desktop mode (default)"
    )
    parser.add_argument(
        "--headless",
        dest="headless",
        action="store_true",
        help="Run in headless offscreen mode (for automated/CI testing)"
    )
    parser.add_argument(
        "--delay",
        type=float,
        default=0.35,
        help="Visual action pacing delay in seconds for live mode (default: 0.35s)"
    )
    parser.set_defaults(headless=False)
    parser.add_argument(
        "--quiet", "-q",
        action="store_true",
        help="Suppress streaming tool stdout"
    )

    args = parser.parse_args()

    # If no action is specified, print help and exit
    if not args.run and not args.step and not args.signoff:
        parser.print_help()
        sys.exit(0)

    try:
        driver = SilisGUIDriver(
            args.project,
            headless=args.headless,
            verbose=not args.quiet,
            action_delay=args.delay
        )
    except Exception as e:
        print(f"[ERR] Failed to initialize SilisGUIDriver: {e}", file=sys.stderr)
        sys.exit(1)

    exit_code = 0
    try:
        if args.signoff:
            exit_code = driver.run_signoff(args.signoff)
        elif args.step:
            ok = driver.run_step(args.step)
            if not ok or driver._current_step_errors:
                exit_code = 1
            elif driver._had_timing_violation:
                exit_code = 2
            else:
                exit_code = 0
        elif args.run:
            exit_code = driver.run_flow(target=args.run, stop_at=args.stop_at)

        # Save debug report
        report_path = driver.save_debug_report(args.json_out)
        print(f"\n[REPORT] Debug report generated: {report_path}")

    except Exception as e:
        print(f"[ERR] Execution error: {e}", file=sys.stderr)
        driver.report_data["status"] = "CRASH"
        driver.report_data["tool_errors"].append({
            "step": getattr(driver, "_active_step_name", "Unknown"),
            "code": "CRASH",
            "message": str(e)
        })
        driver.save_debug_report(args.json_out)
        exit_code = 1
    finally:
        driver.close()

    sys.exit(exit_code)


if __name__ == "__main__":
    main()
