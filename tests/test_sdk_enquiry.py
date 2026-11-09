"""
silis/test_sdk_enquiry.py
Comprehensive unit test suite for the Silis Python SDK Floorplanning Enquiry Engine.
Tests get_utilization (Styles A & B), get_required_area, suggest_box (Style C),
and error handling on both Project and Pipeline.
"""

import os
import sys
import unittest

# Ensure silis package is on path
REPO_ROOT = os.path.dirname(os.path.abspath(__file__))
WORKSPACE_ROOT = os.path.dirname(REPO_ROOT)
if REPO_ROOT in sys.path:
    sys.path.remove(REPO_ROOT)
if WORKSPACE_ROOT not in sys.path:
    sys.path.insert(0, WORKSPACE_ROOT)

import silis
from silis import Project, Pipeline, get_utilization, get_required_area, suggest_box

class TestSDKEnquiryEngine(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        # Target sram_soc_quad_core in repository projects/
        base_dir = os.path.dirname(WORKSPACE_ROOT) if os.path.basename(WORKSPACE_ROOT) == "silis" else WORKSPACE_ROOT
        cls.proj_path = os.path.join(base_dir, "projects", "sram_soc_quad_core")
        cls.project = Project.load(cls.proj_path)

    def test_01_sizes_and_modules_discovery(self):
        """Verify sizes report discovery and module listing."""
        sizes = self.project.get_sizes()
        self.assertIn("modules", sizes)
        self.assertIn("total_std_cell_area", sizes)
        self.assertGreater(sizes["total_std_cell_area"], 0.0)

        modules = self.project.get_modules()
        self.assertIsInstance(modules, list)
        self.assertIn("u_execution_core", modules)
        self.assertIn("u_crypto_accel", modules)
        self.assertIn("u_dma_ctrl", modules)

    def test_02_module_area_queries(self):
        """Verify standard cell area queries for modules and full core."""
        exec_area = self.project.get_module_area("u_execution_core")
        self.assertAlmostEqual(exec_area, 3637.2384, delta=0.01)

        # Core / top area
        total_area = self.project.get_module_area("core")
        self.assertAlmostEqual(total_area, 15509.8752, delta=0.01)

        top_area = self.project.get_module_area("top")
        self.assertEqual(total_area, top_area)

    def test_03_style_a_direct_area_utilization(self):
        """Verify Style A: direct numeric area, (w, h), and [x1, y1, x2, y2] bounding box."""
        # 1. Direct float area (10,000 um^2)
        util_float = self.project.get_utilization("u_execution_core", area=10000.0)
        self.assertAlmostEqual(util_float, 3637.2384 / 10000.0, places=4)
        self.assertTrue(0.0 <= util_float <= 1.0)

        # 2. Dimensions tuple (100.0 x 100.0 = 10,000 um^2)
        util_dims = self.project.get_utilization("u_execution_core", area=(100.0, 100.0))
        self.assertAlmostEqual(util_dims, util_float, places=5)

        # 3. Bounding box sequence [600, 1300, 1000, 1700] (400 x 400 = 160,000 um^2)
        box = [600.0, 1300.0, 1000.0, 1700.0]
        util_box = self.project.get_utilization("u_execution_core", area=box)
        expected = 3637.2384 / 160000.0
        self.assertAlmostEqual(util_box, expected, places=5)

    def test_04_style_b_region_dict_utilization(self):
        """Verify Style B: passing region dict directly extracts module and box."""
        region = {
            "module": "u_execution_core",
            "box": [600.0, 1300.0, 1000.0, 1700.0],
            "is_fence": True,
            "density": 0.30
        }
        util = self.project.get_utilization(region)
        expected = 3637.2384 / 160000.0
        self.assertAlmostEqual(util, expected, places=5)

        # Test with rect key and w, h keys
        region_rect = {"module": "u_crypto_accel", "rect": [1400.0, 1300.0, 1800.0, 1700.0]}
        util_crypto = self.project.get_utilization(region_rect)
        crypto_area = self.project.get_module_area("u_crypto_accel")
        self.assertAlmostEqual(util_crypto, crypto_area / 160000.0, places=5)

    def test_05_style_c_required_area_and_suggest_box(self):
        """Verify Style C: required physical area calculation and suggested bounding box."""
        # Required area for 40% density
        req_area = self.project.get_required_area("u_execution_core", target_density=0.40)
        expected_area = 3637.2384 / 0.40
        self.assertAlmostEqual(req_area, expected_area, delta=0.01)

        # Suggest box at origin (600, 1300) with aspect ratio 1.0
        box = self.project.suggest_box(
            "u_execution_core",
            origin=(600.0, 1300.0),
            target_density=0.40,
            aspect_ratio=1.0
        )
        self.assertEqual(len(box), 4)
        self.assertEqual(box[0], 600.0)
        self.assertEqual(box[1], 1300.0)

        # Confirm the suggested box gives back ~40% utilization!
        util = self.project.get_utilization("u_execution_core", area=box)
        self.assertAlmostEqual(util, 0.40, delta=0.005)

    def test_06_pipeline_forwarding_methods(self):
        """Verify Pipeline object exposes get_utilization, get_required_area, suggest_box."""
        with Pipeline(self.project) as pipe:
            self.assertEqual(pipe.get_modules(), self.project.get_modules())
            
            u_pipe = pipe.get_utilization("u_execution_core", [600, 1300, 1000, 1700])
            u_proj = self.project.get_utilization("u_execution_core", [600, 1300, 1000, 1700])
            self.assertEqual(u_pipe, u_proj)

            req = pipe.get_required_area("u_crypto_accel", target_density=0.50)
            self.assertAlmostEqual(req, self.project.get_module_area("u_crypto_accel") / 0.50, delta=0.01)

    def test_07_top_level_sdk_exports(self):
        """Verify top-level silis.sdk function exports."""
        u = get_utilization("u_execution_core", 10000.0, project=self.project)
        self.assertAlmostEqual(u, 3637.2384 / 10000.0, places=4)

        needed = get_required_area("u_dma_ctrl", target_density=0.30, project=self.project)
        self.assertAlmostEqual(needed, self.project.get_module_area("u_dma_ctrl") / 0.30, delta=0.01)

        box = suggest_box("u_dma_ctrl", origin=(0, 0), target_density=0.30, project=self.project)
        self.assertEqual(len(box), 4)

    def test_08_error_handling_and_validation(self):
        """Verify robust error messages for invalid modules, non-positive areas, etc."""
        # Non-existent module
        with self.assertRaises(KeyError) as ctx:
            self.project.get_utilization("non_existent_module_xyz", 10000.0)
        self.assertIn("Available modules", str(ctx.exception))

        # Negative area
        with self.assertRaises(ValueError):
            self.project.get_utilization("u_execution_core", area=-500.0)

        # Zero area box
        with self.assertRaises(ValueError):
            self.project.get_utilization("u_execution_core", area=[100, 100, 100, 100])

        # Missing area in dict
        with self.assertRaises(ValueError):
            self.project.get_utilization({"module": "u_execution_core"})

        # Invalid target density
        with self.assertRaises(ValueError):
            self.project.get_required_area("u_execution_core", target_density=0.0)

        with self.assertRaises(ValueError):
            self.project.get_required_area("u_execution_core", target_density=1.5)

if __name__ == "__main__":
    unittest.main()
