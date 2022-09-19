import unittest
import jppy

PDFS = "pdfs/J%p.dat"

class TestMuonPDF(unittest.TestCase):
    def test_pdf(self):
        muon_pdf = jppy.pdf.JMuonPDF(PDFS, 0)
        result = muon_pdf.calculate(10, 5, 0, 0, 23)
        self.assertAlmostEqual(0.00067762789, result.f)
        self.assertAlmostEqual(-1.220889709e-05, result.fp)
        self.assertAlmostEqual(0.022764524, result.v)
        self.assertAlmostEqual(0.115814468, result.V)

class TestShowerPDF(unittest.TestCase):
    def test_pdf(self):
        shower_pdf = jppy.pdf.JShowerPDF(PDFS, 0)
        result = shower_pdf.calculate(100, 10, 0.1, 0.2, 0.3, 6)
        self.assertAlmostEqual(0.0013937540, result.f)
        self.assertAlmostEqual(5.091930537e-05, result.fp)
        self.assertAlmostEqual(0.010475308, result.v)
        self.assertAlmostEqual(0.149635554, result.V)
