import unittest
import jppy

PDFS = "pdfs/J%p.dat"

class TestMuonPDFEvaluator(unittest.TestCase):
    def test_pdf_evaluator(self):
        E, t0, t_obs, D, cd, theta, phi = [1e3, 56, 292, 50, 0.7, 1.57, 3.14]
        muon_pdf = jppy.pdf_evaluator.MuonPDF(PDFS, E, t0)
        result = muon_pdf.evaluate(D, cd, theta, phi, t_obs)
        self.assertAlmostEqual( 0.003644475, result.f)
        self.assertAlmostEqual(-0.000715558, result.fp)
        self.assertAlmostEqual( 0.033748905, result.v)
        self.assertAlmostEqual( 0.097171157, result.V)

class TestShowerPDFEvaluator(unittest.TestCase):
    def test_pdf_evaluator(self):
        E, t0, t_obs, D, cd, theta, phi = [50, 198, 226, 5, 0.6, 0.5, 0.4]
        shower_pdf = jppy.pdf_evaluator.ShowerPDF(PDFS, E, t0)
        result = shower_pdf.evaluate(D, cd, theta, phi, t_obs)
        self.assertAlmostEqual(0.006204617, result.f)
        self.assertAlmostEqual(0.000641669, result.fp)
        self.assertAlmostEqual(0.013960066, result.v)
        self.assertAlmostEqual(0.296589983, result.V)


    