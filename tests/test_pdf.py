import unittest
import jppy

PDFS = "pdfs/J%p.dat"

class TestMuonPDF(unittest.TestCase):
    def test_pdf(self):
        muon_pdf = jppy.pdf.JMuonPDF(PDFS, 0)
        result = muon_pdf.calculate(10, 5, 0, 0, 23)
        self.assertAlmostEqual(0.001535784, result.f)
        self.assertAlmostEqual(-2.22809691e-05, result.fp)
        self.assertAlmostEqual(0.024902763, result.v)
        self.assertAlmostEqual(0.116492968, result.V)

class TestShowerPDF(unittest.TestCase):
    def test_pdf(self):
        shower_pdf = jppy.pdf.JShowerPDF(PDFS, 0)
        result = shower_pdf.calculate(100, 10, 0.1, 0.2, 0.3, 6)
        self.assertAlmostEqual(0.001612553295068934, result.f)
        self.assertAlmostEqual(4.000285659029551e-05, result.fp)
        self.assertAlmostEqual(0.010999553987301543, result.v)
        self.assertAlmostEqual(0.1527856994106781, result.V)
