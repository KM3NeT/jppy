import unittest
import jppy

class TestGeaneWater(unittest.TestCase):
    def test_geane(self):
        gwater = jppy.geane.JGeaneWater()
        density_sea_water = 1.038
        assert(gwater.get_a() == 2.30e-1 * density_sea_water)
        assert(gwater.get_b() == 3.40e-4 * density_sea_water)
        self.assertAlmostEqual(gwater.get_E(4e4, 100), 3.857507637293732e+04)
        self.assertAlmostEqual(gwater.get_X(4e4, 4e3), 6.069985857980293e+03)
