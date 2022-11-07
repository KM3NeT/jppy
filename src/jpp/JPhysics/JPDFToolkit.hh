#ifndef __JPHYSICS__JPDFTOOLKIT__
#define __JPHYSICS__JPDFTOOLKIT__

#include <cmath>

#include "JPhysics/JConstants.hh"


/**
 * \file
 * Auxiliary methods for PDF calculations.
 * \author mdejong
 */

namespace JPHYSICS {}
namespace JPP { using namespace JPHYSICS; }

namespace JPHYSICS {


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
   * Emission profile of photons from delta-rays.
   *
   * Profile is taken from reference ANTARES-SOFT-2002-015, J.\ Brunner (fig.\ 3).
   *
   * \param  x      cosine emission angle
   * \return        probability
   */
  inline double getDeltaRayProbability(const double x)
  {
    //return 1.0 / (4.0 * PI);
    return 0.188 * exp(-1.25 * pow(fabs(x - 0.90), 1.30));
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
}

#endif
