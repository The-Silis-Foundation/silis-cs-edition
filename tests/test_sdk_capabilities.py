import os
import sys
import unittest

sys.path.append("/home/econsystems/silisfoundation/silis")
from core.sdk import Project, Pipeline, view_layout

PROJ_PATH = "/home/econsystems/silisfoundation/projects/sram_soc_system"

class TestSDKCapabilities(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.proj = Project.load(PROJ_PATH)
        cls.pipe = Pipeline(cls.proj, headless=True)

    @classmethod
    def tearDownClass(cls):
        cls.pipe.close()

    def test_01_padring_die_sizing(self):
        res = self.pipe.compute_minimum_die_for_pads(num_pads=80, core_w=1200.0, core_h=1000.0)
        self.assertIn("die_w", res)
        self.assertIn("die_h", res)
        self.assertGreaterEqual(res["die_w"], 1200.0 + 2 * res["pad_margin"])
        self.assertGreaterEqual(res["die_h"], 1000.0 + 2 * res["pad_margin"])
        self.assertEqual(sum(res["pads_per_edge"].values()), 80)

    def test_02_optimize_die(self):
        opt = self.pipe.optimize_die(target_density=0.45, padring=True)
        self.assertGreater(opt["die_w"], 0.0)
        self.assertGreater(opt["die_h"], 0.0)

    def test_03_heatmap_generation(self):
        hm = self.pipe.get_heatmap("ir_drop")
        self.assertEqual(hm["type"], "ir_drop")
        self.assertEqual(hm["grid_size"], (160, 160))
        self.assertEqual(hm["grid"].shape, (160, 160))
        self.assertGreater(hm["raw_max"], 0.0)
        self.assertIsInstance(hm["peak_coord_um"], tuple)

        # Test cell density heatmap
        hm_density = self.pipe.get_heatmap("density")
        self.assertEqual(hm_density["type"], "density")
        self.assertEqual(hm_density["grid"].shape, (160, 160))

    def test_04_eco_and_routing_controls(self):
        self.assertTrue(self.pipe.apply_clock_ndr("2w_2s", "met3", "met5"))
        self.assertTrue(self.pipe.repair_clock_buffers())
        self.assertTrue(self.pipe.repair_antennas(iterations=1))

    def test_05_checkpoint_save_and_load(self):
        ck_path = self.pipe.save_checkpoint("test_sdk_snap")
        self.assertTrue(os.path.isfile(ck_path))
        self.assertTrue(self.pipe.load_checkpoint("test_sdk_snap"))

if __name__ == "__main__":
    unittest.main()
