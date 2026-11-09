"""
silis/tests/test_spatial_engine.py
Unit test and verification suite for the Silis Spatial Reasoning & Packing Engine:
1. pipe.get_empty_cavities()
2. pipe.get_macro_pin_terminals()
3. pipe.get_channel_metrics()
4. pipe.simulate_floorplan_patch()
"""

import os
import sys
import unittest

REPO_ROOT = os.path.dirname(os.path.abspath(__file__))
SILIS_DIR = os.path.dirname(REPO_ROOT)
WORKSPACE_ROOT = os.path.dirname(SILIS_DIR)
if WORKSPACE_ROOT not in sys.path:
    sys.path.insert(0, WORKSPACE_ROOT)

os.environ["SILIS_HEADLESS_TEST"] = "1"
os.environ["QT_QPA_PLATFORM"] = "offscreen"
os.environ["QTWEBENGINE_DISABLE_SANDBOX"] = "1"

import silis
from silis import Pipeline
from silis.core.sdk.spatial import SpatialReasoningEngine


class TestSpatialEngine(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.proj_path = os.path.join(WORKSPACE_ROOT, "projects", "sram_soc_system")
        cls.pipe = Pipeline(cls.proj_path)

    @classmethod
    def tearDownClass(cls):
        if hasattr(cls, "pipe") and cls.pipe:
            cls.pipe.close()

    def test_01_empty_cavities_sram_soc(self):
        """Verify maximal rectangular empty cavities computed on sram_soc_system."""
        cavities = self.pipe.get_empty_cavities(min_w=40.0, min_h=40.0)
        self.assertIsInstance(cavities, list)
        self.assertGreater(len(cavities), 0)

        for cav in cavities:
            self.assertIn("box", cav)
            self.assertIn("width", cav)
            self.assertIn("height", cav)
            self.assertIn("area", cav)
            self.assertEqual(len(cav["box"]), 4)
            self.assertGreaterEqual(cav["width"], 40.0)
            self.assertGreaterEqual(cav["height"], 40.0)
            self.assertAlmostEqual(cav["width"] * cav["height"], cav["area"], places=2)

    def test_02_empty_cavities_synthetic_subtraction(self):
        """Verify interval subtraction on explicit synthetic core and macro layout."""
        # Core: [0, 0, 1000, 1000], placed macro at [200, 200, 400, 400]
        cavities = self.pipe.get_empty_cavities(
            min_w=50.0,
            min_h=50.0,
            core_box=(0.0, 0.0, 1000.0, 1000.0),
            placed_macros=[{"box": [200.0, 200.0, 400.0, 400.0]}],
        )
        self.assertEqual(len(cavities), 4)
        boxes = [c["box"] for c in cavities]
        # Should contain [0, 0, 200, 1000], [400, 0, 1000, 1000], [0, 0, 1000, 200], [0, 400, 1000, 1000]
        self.assertIn([0.0, 0.0, 200.0, 1000.0], boxes)
        self.assertIn([400.0, 0.0, 1000.0, 1000.0], boxes)
        self.assertIn([0.0, 0.0, 1000.0, 200.0], boxes)
        self.assertIn([0.0, 400.0, 1000.0, 1000.0], boxes)

    def test_03_macro_pin_terminals_sram(self):
        """Verify macro pin terminals, COG, and dominant face inspection from LEF."""
        pins_data = self.pipe.get_macro_pin_terminals("u_sram_bank0")
        self.assertIsInstance(pins_data, dict)
        self.assertEqual(pins_data["master"], "sky130_sram_1kbyte_1rw1r_32x256_8")
        self.assertAlmostEqual(pins_data["width"], 479.78, places=2)
        self.assertAlmostEqual(pins_data["height"], 397.5, places=2)
        self.assertEqual(pins_data["dominant_face"], "SOUTH")
        self.assertEqual(pins_data["pin_count"], 123)
        self.assertIn("met3", pins_data["metal_layers"])
        self.assertIn("met4", pins_data["metal_layers"])
        
        # Check normalized COG in [0..1]
        norm_x, norm_y = pins_data["pin_cog_normalized"]
        self.assertTrue(0.0 <= norm_x <= 1.0)
        self.assertTrue(0.0 <= norm_y <= 1.0)

    def test_04_channel_metrics_sram_soc(self):
        """Verify routing corridor gap, tracks, and alignment between bank0 and bank1."""
        metrics = self.pipe.get_channel_metrics("u_sram_bank0", "u_sram_bank1")
        self.assertIsInstance(metrics, dict)
        self.assertEqual(metrics["inst_a"], "u_sram_bank0")
        self.assertEqual(metrics["inst_b"], "u_sram_bank1")
        # In sram_soc_system: bank0 ends at ~579.78, bank1 starts at 800.0 -> gap ~ 220.22 um
        self.assertAlmostEqual(metrics["gap"], 220.22, places=1)
        self.assertGreater(metrics["tracks_h"], 300)
        self.assertGreater(metrics["tracks_v"], 300)
        self.assertTrue(metrics["row_aligned"])
        self.assertTrue(metrics["legal"])
        self.assertEqual(metrics["deficit"], 0.0)

    def test_05_channel_metrics_corner_deficit(self):
        """Verify channel metrics detects corridor width deficit."""
        inst_a = {"name": "macro_a", "x": 100.0, "y": 100.64, "width": 100.0, "height": 100.0}
        inst_b = {"name": "macro_b", "x": 210.0, "y": 100.64, "width": 100.0, "height": 100.0}
        # gap = 210 - 200 = 10.0 um. min_channel_gap = 35.0 um -> deficit = 25.0 um
        metrics = self.pipe.get_channel_metrics(inst_a, inst_b, min_channel_gap=35.0)
        self.assertAlmostEqual(metrics["gap"], 10.0, places=1)
        self.assertFalse(metrics["legal"])
        self.assertAlmostEqual(metrics["deficit"], 25.0, places=1)
        self.assertTrue(metrics["row_aligned"])

    def test_06_simulate_floorplan_patch_legal_performance(self):
        """Verify in-memory patch simulation snaps to grid and executes in < 5ms."""
        mutations = [
            {"inst": "u_sram_bank0", "loc": [100.1234, 100.5]},  # needs snapping
            {"inst": "u_sram_bank1", "loc": [800.0, 100.64]},
        ]
        res = self.pipe.simulate_floorplan_patch(mutations)
        self.assertIsInstance(res, dict)
        self.assertTrue(res["legal"])
        self.assertLess(res["elapsed_ms"], 5.0)  # strict latency requirement < 5ms
        self.assertEqual(len(res["snapped_mutations"]), 2)
        
        # Verify grid snapping on bank0
        bank0_snapped = res["snapped_mutations"][0]
        # x snapped to 0.005 um: 100.125
        self.assertEqual(bank0_snapped["loc"][0], 100.125)
        # y snapped to 2.72 um: round(100.5 / 2.72) * 2.72 = 37 * 2.72 = 100.64
        self.assertEqual(bank0_snapped["loc"][1], 100.64)

    def test_07_simulate_floorplan_patch_collision_and_deficit(self):
        """Verify collision and channel deficit detection in floorplan patch simulation."""
        # Collision test: overlapping coordinates
        mutations_overlap = [
            {"inst": "u_sram_bank0", "loc": [100.0, 100.64]},
            {"inst": "u_sram_bank1", "loc": [120.0, 100.64]},
        ]
        res_overlap = self.pipe.simulate_floorplan_patch(mutations_overlap)
        self.assertFalse(res_overlap["legal"])
        self.assertGreater(len(res_overlap["overlaps"]), 0)
        self.assertGreater(len(res_overlap["violations"]), 0)

        # Deficit test: spacing < 35.0 um
        # bank0 ends at 100 + 479.78 = 579.78. bank1 placed at 600.0 -> gap = 20.22 um
        mutations_deficit = [
            {"inst": "u_sram_bank0", "loc": [100.0, 100.64]},
            {"inst": "u_sram_bank1", "loc": [600.0, 100.64]},
        ]
        res_deficit = self.pipe.simulate_floorplan_patch(mutations_deficit, min_channel_gap=35.0)
        self.assertFalse(res_deficit["legal"])
        self.assertGreater(len(res_deficit["channel_deficits"]), 0)
        self.assertAlmostEqual(res_deficit["channel_deficits"][0]["gap"], 20.22, places=1)


if __name__ == "__main__":
    unittest.main()
