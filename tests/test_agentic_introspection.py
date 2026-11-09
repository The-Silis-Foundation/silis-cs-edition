"""
silis/tests/test_agentic_introspection.py
Comprehensive unit test and verification suite for the 5 Agentic Introspection APIs:
1. pipe.get_congestion_map()
2. pipe.get_macro_affinity_graph()
3. pipe.get_critical_timing_paths()
4. pipe.get_ir_drop_map()
5. pipe.get_drc_markers()
"""

import os
import sys
import unittest

REPO_ROOT = os.path.dirname(os.path.abspath(__file__))
WORKSPACE_ROOT = os.path.dirname(REPO_ROOT)
if WORKSPACE_ROOT not in sys.path:
    sys.path.insert(0, WORKSPACE_ROOT)

os.environ["SILIS_HEADLESS_TEST"] = "1"
os.environ["QT_QPA_PLATFORM"] = "offscreen"

import silis
from silis import Project, Pipeline


class TestAgenticIntrospection(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        base_dir = os.path.dirname(WORKSPACE_ROOT) if os.path.basename(WORKSPACE_ROOT) == "silis" else WORKSPACE_ROOT
        cls.proj_path = os.path.join(base_dir, "projects", "sram_soc_system")
        cls.pipe = Pipeline(cls.proj_path)

    @classmethod
    def tearDownClass(cls):
        if hasattr(cls, "pipe") and cls.pipe:
            cls.pipe.close()

    def test_01_macro_affinity_graph(self):
        """Verify get_macro_affinity_graph returns structured dict with macro instances and net counts."""
        aff = self.pipe.get_macro_affinity_graph()
        self.assertIsInstance(aff, dict)
        self.assertIn("u_sram_bank0", aff)
        self.assertIn("u_sram_bank1", aff)
        
        bank0_conns = aff["u_sram_bank0"]
        self.assertIsInstance(bank0_conns, dict)
        self.assertIn("u_arbiter", bank0_conns)
        self.assertGreater(bank0_conns["u_arbiter"], 0)

    def test_02_critical_timing_paths(self):
        """Verify get_critical_timing_paths returns structured slacks, endpoints, and delays."""
        paths = self.pipe.get_critical_timing_paths(top_n=3)
        self.assertIsInstance(paths, list)
        self.assertGreater(len(paths), 0)
        
        p0 = paths[0]
        self.assertIn("slack_ns", p0)
        self.assertIn("startpoint", p0)
        self.assertIn("endpoint", p0)
        self.assertIn("violated", p0)
        self.assertIsInstance(p0["slack_ns"], float)
        self.assertIsInstance(p0["violated"], bool)

    def test_03_congestion_map(self):
        """Verify get_congestion_map returns list of hotspot bounding boxes."""
        cong = self.pipe.get_congestion_map()
        self.assertIsInstance(cong, list)
        for h in cong:
            self.assertIn("box", h)
            self.assertIn("layer", h)
            self.assertIn("overflow", h)
            self.assertEqual(len(h["box"]), 4)

    def test_04_ir_drop_map(self):
        """Verify get_ir_drop_map returns structured power grid analysis report."""
        ir = self.pipe.get_ir_drop_map("VDD")
        self.assertIsInstance(ir, dict)
        self.assertIn("net", ir)
        self.assertIn("worst_drop_mv", ir)
        self.assertIn("worst_drop_pct", ir)
        self.assertIn("worst_location", ir)
        self.assertIn("status", ir)
        self.assertEqual(ir["net"], "VDD")
        self.assertIsInstance(ir["worst_drop_mv"], float)
        self.assertIsInstance(ir["worst_location"], list)
        self.assertEqual(len(ir["worst_location"]), 2)

    def test_05_drc_markers(self):
        """Verify get_drc_markers returns list of exact geometric DRC marker dicts."""
        drcs = self.pipe.get_drc_markers()
        self.assertIsInstance(drcs, list)
        for d in drcs:
            self.assertIn("type", d)
            self.assertIn("layer", d)
            self.assertIn("box", d)
            self.assertEqual(len(d["box"]), 4)


if __name__ == "__main__":
    unittest.main()
