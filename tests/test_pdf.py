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
