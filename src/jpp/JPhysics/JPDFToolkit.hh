#ifndef __JPHYSICS__JPDFTOOLKIT__
#define __JPHYSICS__JPDFTOOLKIT__

#include <vector>
#include <cmath>

#include "JLang/JCC.hh"
#include "JPhysics/JConstants.hh"
#include "JTools/JFunction1D_t.hh"
#include "JIO/JSerialisable.hh"

/**
 * \file
 * Auxiliary methods for PDF calculations.
 * \author mdejong
 */

namespace JPHYSICS {}
namespace JPP { using namespace JPHYSICS; }

namespace JPHYSICS {

  using JIO::JReader;
  using JIO::JWriter;
  using JTOOLS::JGridPolint0Function1D_t;
  using JTOOLS::JGridSplineFunction1D_t;


  /**
   * Get minimal wavelength for PDF evaluations.
   *
   * \return            wavelength of light [nm]
   */
  inline double getMinimalWavelength()
  {
    return 300.0;
  }


  /**
   * Get maximal wavelength for PDF evaluations.
   *
   * \return            wavelength of light [nm]
   */
  inline double getMaximalWavelength()
  {
    return 700.0;
  }
  

  /**
   * Number of Cherenkov photons per unit track length and per unit wavelength.
   *
   * \param  lambda     wavelength of light [nm]
   * \param  n          index of refraction
   * \return            number of photons per unit track length and per unit wavelength [m^-1 nm^-1]
   */
  inline double cherenkov(const double lambda,
			  const double n)
  {
    const double x = n*lambda;
    
    return 1.0e9 * 2 * PI * ALPHA_ELECTRO_MAGNETIC * (n*n - 1.0) / (x*x);
  }

  
  /**
   * Equivalent EM-shower energy due to delta-rays per unit muon track length.
   *
   * Internal parameters are obtained with application [script] JDeltaRays[.sh].
   * 
   * \param  E      muon energy [GeV]
   * \return        equivalent energy loss [GeV/m]
   */  
  inline double getDeltaRaysFromMuon(const double E)
  {
    static const double a =  3.186e-01;
    static const double b =  3.384e-01;
    static const double c = -2.759e-02;
    static const double d =  1.630e-03;
    static const double Emin = 0.13078; // [GeV]

    if (E > Emin) {

      const double x = log10(E);                      //
      const double y = a + x*(b + x*(c + x*(d)));     // [MeV g^-1 cm^2]

      return y * DENSITY_SEA_WATER * 1.0e-1;          // [GeV/m]
    }

    return 0.0;
  }

  
  /**
   * Equivalent EM-shower energy due to delta-rays per unit tau track length.
   *
   * Internal parameters are obtained with application [script] JDeltaRays[.sh].
   * 
   * \param  E      tau energy [GeV]
   * \return        equivalent energy loss [GeV/m]
   */  
  inline double getDeltaRaysFromTau(const double E)
  {
    static const double a = -2.374e-01;
    static const double b =  5.143e-01;
    static const double c = -4.213e-02;
    static const double d =  1.804e-03;
    static const double Emin = 2.19500; // [GeV]
    
    if (E > Emin) {

      const double x = log10(E);                      //
      const double y = a + x*(b + x*(c + x*(d)));     // [MeV g^-1 cm^2]

      return y * DENSITY_SEA_WATER * 1.0e-1;          // [GeV/m]
    }

    return 0.0;
  }


  /**
   * Rayleigh cross section.
   *
   * \param  n          index of refraction
   * \param  lambda     wavelength of light [nm]
   * \return            cross section       [cm^2]
   */
  inline const double getRayleighCrossSection(const double n,
					      const double lambda)
  {
    static const double d = 0.36;                    // size of H2O molecule [nm]
    static const double U = PI*PI*PI*PI*PI*2.0/3.0;
    static const double V = d*d*d*d*d*d;

    const double W     = (n*n - 1.0) / (n*n + 2.0);
    const double sigma = 1.0e-14 * U*V*W*W / (lambda*lambda*lambda*lambda);   // [cm^2]

    return sigma;
  }


  /**
   * Rayleigh scattering length.
   *
   * \param  n          index of refraction
   * \param  lambda     wavelength of light [nm]
   * \return            scattering length   [m]
   */
  inline const double getRayleighScatteringLength(const double n,
						  const double lambda)
  {
    static const double amu = 18.01528; // H20 mass in Atomic mass units

    const double sigma = getRayleighCrossSection(n, lambda);
    const double ls    = 1.0e-2 / (DENSITY_SEA_WATER * AVOGADRO * sigma / amu);     // [m]

    return ls;
  }


  /**
   * Absorption length of pure water.
   *
   * CITATION:
   * Jonasz M. 2007. Absorption coefficient of water: Data sources (www.tpdsci.com/Tpc/AbsCfOfWaterDat.php).
   * In: Top. Part. Disp. Sci. (www.tpdsci.com). 			
   *
   * DATA FROM:
   * Wozniak B., Wozniak S. B., Tyszka K., Ostrowska M., Majchrowski R., Ficek D., Dera J. 2005.
   * Modelling the light absorption properties of particulate matter forming organic particles suspended in seawater. Part 2.
   * Modelling results. Oceanologia 47, 621-662.
   * see also
   * Wozniak B., Dera J. 2007.
   * Light absorption in sea water. Springer, Berlin, 456 pp. (see p. 62)
   *
   * NOTES:
   * As stated by the data authors, the data are based on measurement results obtained by various authors 
   * (interpolated by a linear approximation where applicable):
   * Wavelength	Reference
   * - 200-335 nm   Smith R.C., Baker K.S. 1981. Optical properties of the clearest natural waters (200-800 nm). Appl. Opt. 20, 177-184.
   * - 340-370 nm   Sogandares F.M., Fry, E.S. 1997. Absorption spectrum (340 -640 nm) of pure water. I. Photothermal measurements Appl. Opt. 36, 8699-8709.
   * - 380-700 nm   Pope R.M., Fry E.S. 1997. Absorption spectrum (380 -700 nm) of pure water. II. Integrating cavity measurements. Appl. Opt. 36, 8710-8723
   */
  class JAbsorptionLengthOfPureWater :
    public JGridSplineFunction1D_t
  {
  public:
    JAbsorptionLengthOfPureWater() 
    {
      //    wave-	absorption
      //    length	coefficient
      //    [um]	[1/m]
      (*this)[0.200e3]  =  3.07;
      (*this)[0.205e3]  =  2.53;
      (*this)[0.210e3]  =  1.99;
      (*this)[0.215e3]  =  1.65;
      (*this)[0.220e3]  =  1.31;
      (*this)[0.225e3]  =  1.1185;
      (*this)[0.230e3]  =  0.927;
      (*this)[0.235e3]  =  0.8235;
      (*this)[0.240e3]  =  0.72;
      (*this)[0.245e3]  =  0.6395;
      (*this)[0.250e3]  =  0.559;
      (*this)[0.255e3]  =  0.508;
      (*this)[0.260e3]  =  0.457;
      (*this)[0.265e3]  =  0.415;
      (*this)[0.270e3]  =  0.373;
      (*this)[0.275e3]  =  0.3305;
      (*this)[0.280e3]  =  0.288;
      (*this)[0.285e3]  =  0.2515;
      (*this)[0.290e3]  =  0.215;
      (*this)[0.295e3]  =  0.178;
      (*this)[0.300e3]  =  0.141;
      (*this)[0.305e3]  =  0.123;
      (*this)[0.310e3]  =  0.105;
      (*this)[0.315e3]  =  0.0947;
      (*this)[0.320e3]  =  0.0844;
      (*this)[0.325e3]  =  0.0761;
      (*this)[0.330e3]  =  0.0678;
      (*this)[0.335e3]  =  0.06195;
      (*this)[0.340e3]  =  0.0325;
      (*this)[0.345e3]  =  0.02645;
      (*this)[0.350e3]  =  0.0204;
      (*this)[0.355e3]  =  0.018;
      (*this)[0.360e3]  =  0.0156;
      (*this)[0.365e3]  =  0.0135;
      (*this)[0.370e3]  =  0.0114;
      (*this)[0.375e3]  =  0.011385;
      (*this)[0.380e3]  =  0.01137;
      (*this)[0.385e3]  =  0.00941;
      (*this)[0.390e3]  =  0.00851;
      (*this)[0.395e3]  =  0.00813;
      (*this)[0.400e3]  =  0.00663;
      (*this)[0.405e3]  =  0.0053;
      (*this)[0.410e3]  =  0.00473;
      (*this)[0.415e3]  =  0.00444;
      (*this)[0.420e3]  =  0.00454;
      (*this)[0.425e3]  =  0.00478;
      (*this)[0.430e3]  =  0.00495;
      (*this)[0.435e3]  =  0.0053;
      (*this)[0.440e3]  =  0.00635;
      (*this)[0.445e3]  =  0.00751;
      (*this)[0.450e3]  =  0.00922;
      (*this)[0.455e3]  =  0.00962;
      (*this)[0.460e3]  =  0.00979;
      (*this)[0.465e3]  =  0.01011;
      (*this)[0.470e3]  =  0.0106;
      (*this)[0.475e3]  =  0.0114;
      (*this)[0.480e3]  =  0.0127;
      (*this)[0.485e3]  =  0.0136;
      (*this)[0.490e3]  =  0.015;
      (*this)[0.495e3]  =  0.0173;
      (*this)[0.500e3]  =  0.0204;
      (*this)[0.505e3]  =  0.0256;
      (*this)[0.510e3]  =  0.0325;
      (*this)[0.515e3]  =  0.0396;
      (*this)[0.520e3]  =  0.0409;
      (*this)[0.525e3]  =  0.0417;
      (*this)[0.530e3]  =  0.0434;
      (*this)[0.535e3]  =  0.0452;
      (*this)[0.540e3]  =  0.0474;
      (*this)[0.545e3]  =  0.0511;
      (*this)[0.550e3]  =  0.0565;
      (*this)[0.555e3]  =  0.0596;
      (*this)[0.560e3]  =  0.0619;
      (*this)[0.565e3]  =  0.0642;
      (*this)[0.570e3]  =  0.0695;
      (*this)[0.575e3]  =  0.0772;
      (*this)[0.580e3]  =  0.0896;
      (*this)[0.585e3]  =  0.11;
      (*this)[0.590e3]  =  0.1351;
      (*this)[0.595e3]  =  0.1672;
      (*this)[0.600e3]  =  0.2224;
      (*this)[0.605e3]  =  0.2577;
      (*this)[0.610e3]  =  0.2644;
      (*this)[0.615e3]  =  0.2678;
      (*this)[0.620e3]  =  0.2755;
      (*this)[0.625e3]  =  0.2834;
      (*this)[0.630e3]  =  0.2916;
      (*this)[0.635e3]  =  0.3012;
      (*this)[0.640e3]  =  0.3108;
      (*this)[0.645e3]  =  0.325;
      (*this)[0.650e3]  =  0.34;
      (*this)[0.655e3]  =  0.371;
      (*this)[0.660e3]  =  0.41;
      (*this)[0.665e3]  =  0.429;
      (*this)[0.670e3]  =  0.439;
      (*this)[0.675e3]  =  0.448;
      (*this)[0.680e3]  =  0.465;
      (*this)[0.685e3]  =  0.486;
      (*this)[0.690e3]  =  0.516;
      (*this)[0.695e3]  =  0.559;
      (*this)[0.700e3]  =  0.624;

      compile();
    }


    /**
     * Absorption length of pure water.
     *
     * \param  lambda     wavelength of light [nm]
     * \return            absorption length   [m]
     */
    double operator()(const double lambda) const
    {
      const double y = JGridSplineFunction1D_t::operator()(lambda);
      
      return 1.0 / y;
    }
  };
  

  /**
   * Function object for absorption length of pure water.
   */
  static const JAbsorptionLengthOfPureWater getAbsorptionLengthOfPureWater;
}

#endif
