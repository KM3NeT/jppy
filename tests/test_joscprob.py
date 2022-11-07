import unittest
import jppy

class TestOscParameters(unittest.TestCase):
    def test_oscparameters(self):
        parameters1 = jppy.oscprob.JOscParameters("sinsqTh12", 0.304,
                                                  "deltaCP",   1.544,
                                                  "dM21sq",    7.42e-5)

        print("parameters1:\n", parameters1)

        assert(parameters1.is_valid() and parameters1.size() == 3)

        parameters1.set("sinsqTh23", -0.5)

        print("parameters1.sinsqTh23 = -0.5 -> parameters1:\n", parameters1)

        assert(not parameters1.is_valid() and parameters1.size() == 4)

        parameters2 = jppy.oscprob.JOscParameters("dM31sq",   -2.49e-3,
                                                  "sinsqTh13", 0.02241,
                                                  "sinsqTh23", 0.570)

        print("parameters2:\n", parameters2)

        assert(parameters2.is_valid() and parameters2.size() == 3)
        assert(parameters1 != parameters2)
        assert(not parameters1.contains(parameters2))

        print("Joining parameters2 with parameters1")

        parameters1.join(parameters2)

        assert(parameters1.is_valid() and parameters1.size() == 6)
        assert(parameters1.contains(parameters2))

class TestOscChannel(unittest.TestCase):
    def test_oscchannel(self):
        # Test comparisons
        channel1 = jppy.oscprob.JOscChannel(14,16,1)

        print("channel1:\n", channel1)

        channel2 = jppy.oscprob.JOscChannel(jppy.oscprob.JFlavour_t.ELECTRON,
                                            jppy.oscprob.JFlavour_t.MUON,
                                            jppy.oscprob.JChargeParity_t.ANTIPARTICLE)

        print("channel2:\n", channel2)

        channel3 = jppy.oscprob.JOscChannel(jppy.oscprob.JFlavour_t.MUON,
                                            jppy.oscprob.JFlavour_t.TAU,
                                            jppy.oscprob.JChargeParity_t.ANTIPARTICLE)

        print("channel3:\n", channel3)

        assert(channel1 > channel2)
        assert(channel3 > channel2)

        # Test auxiliary functions
        assert(jppy.oscprob.get_flavour(-12) == jppy.oscprob.JFlavour_t.ELECTRON)
        assert(jppy.oscprob.get_flavour(-14) == jppy.oscprob.JFlavour_t.MUON)
        assert(jppy.oscprob.get_flavour(-16) == jppy.oscprob.JFlavour_t.TAU)
        assert(jppy.oscprob.get_flavour(+12) == jppy.oscprob.JFlavour_t.ELECTRON)
        assert(jppy.oscprob.get_flavour(+14) == jppy.oscprob.JFlavour_t.MUON)
        assert(jppy.oscprob.get_flavour(+16) == jppy.oscprob.JFlavour_t.TAU)        

        assert(jppy.oscprob.get_oscprob_flavour(jppy.oscprob.JFlavour_t.ELECTRON) == jppy.oscprob.OscProbFlavour_t.ELECTRON)
        assert(jppy.oscprob.get_oscprob_flavour(jppy.oscprob.JFlavour_t.MUON) == jppy.oscprob.OscProbFlavour_t.MUON)
        assert(jppy.oscprob.get_oscprob_flavour(jppy.oscprob.JFlavour_t.TAU) == jppy.oscprob.OscProbFlavour_t.TAU)

        assert(jppy.oscprob.get_charge_parity(-12) == jppy.oscprob.JChargeParity_t.ANTIPARTICLE)
        assert(jppy.oscprob.get_charge_parity(-14) == jppy.oscprob.JChargeParity_t.ANTIPARTICLE)
        assert(jppy.oscprob.get_charge_parity(-16) == jppy.oscprob.JChargeParity_t.ANTIPARTICLE)
        assert(jppy.oscprob.get_charge_parity(+12) == jppy.oscprob.JChargeParity_t.PARTICLE)
        assert(jppy.oscprob.get_charge_parity(+14) == jppy.oscprob.JChargeParity_t.PARTICLE)
        assert(jppy.oscprob.get_charge_parity(+16) == jppy.oscprob.JChargeParity_t.PARTICLE)        
        
                                            
