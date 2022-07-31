#ifndef __JPHYSICS__JGEANZ__
#define __JPHYSICS__JGEANZ__

#include <cmath>

#include "JMath/JMathSupportkit.hh"

/**
 * \file
 * Longitudinal emission profile EM-shower.
 * \author mdejong
 */

namespace JPHYSICS {}
namespace JPP { using namespace JPHYSICS; }

namespace JPHYSICS {

  /**
   * Function object for longitudinal profile of EM-shower.
   *
   *
   *         \f[P(z)  \propto  z^{a-1} \times e^{-z/b}\f]
   *
   * where: \f[a = a_{0} + a_{1} \times \ln(E)\f]
   *
   * The parametrisation is taken from reference:
   * C. Kopper, "Performance Studies for the KM3NeT Neutrino Telescope.",
   * PhD thesis, University of Erlangen.
   */
  class JGeanz {
  public:
    /**
     * constructor
     * \param  __a0    power term (constant)
     * \param  __a1    power term (E dependence)
     * \param  __b     expontial slope
     */ 
    JGeanz(const double __a0,
	   const double __a1,
	   const double __b) :
      a0(__a0),
      a1(__a1),
      b (__b),
      Emin(exp(-a0/a1))
    {}


    /**
     * Probability Density Function
     *
     * \param  E       EM-shower energy [GeV]
     * \param  z       z position of light emission point relative to vertex location (z >= 0) [m]
     * \return         dP/dz
     */
    double getProbability(const double E, 
			  const double z) const
    {
      if (E > Emin) {

	const double a = a0 + a1 * log(E);
	const double y = pow(z,a-1.0) * exp(-z/b) / (pow(b,a) * std::tgamma(a));
	
	return y;
      } 
	  
      if (z <= getMinimalShowerSize()) 
	return 1.0 / getMinimalShowerSize();
      else
	return 0.0;
    }
    

    /**
     * Probability Density Function
     *
     * \param  E       EM-shower energy [GeV]
     * \param  z       z position of light emission point relative to vertex location (z >= 0) [m]
     * \return         dP/dz
     */
    double operator()(const double E, 
		      const double z) const
    {
      return getProbability(E, z);
    }
   

    /**
     * Integral of PDF (starting from 0).
     *
     * \param  E       EM-shower energy [GeV]
     * \param  z       z position [m] (>= 0)
     * \return         dP
     */
    double getIntegral(const double E, 
		       const double z) const
    {
      if (E > Emin) {

	const double a = a0 + a1 * log(E);
	const double x = z / b;
	const double y = JMATH::Gamma(a,x);
	
	return y;
      }
      
      if (z <= getMinimalShowerSize())
	return z / getMinimalShowerSize();
      else
	return 1.0;
    }
    

    /**
     * Get shower length for a given integrated probability.
     *
     * \param  E       EM-shower energy [GeV]
     * \param  P       integrated probability [0,1]
     * \param  eps     relative precision
     * \return         shower length [m]
     */
    double getLength(const double E, 
		     const double P,
		     const double eps = 1.0e-3) const
    {
      double zmin  =   0.0;     // [m]
      double zmax  =  30.0;     // [m]

      if (E > Emin) {

	const double Q = P * (1.0 - eps);
	
	for (int i = 100; i != 0; --i) {
	  
	  const double z = 0.5 * (zmin + zmax);
	  const double p = getIntegral(E, z);
	  
	  if (fabs(p-Q) < p*eps) {
	    return z;
	  }
	  
	  if (p > P)
	    zmax = z;
	  else
	    zmin = z;
	}
	
	return 0.5 * (zmin + zmax);

      } else

	return 0.0;
    }

    /**
     * Get depth of shower maximum
     *
     * \param  E       EM-shower energy[GeV]
     * \return         depth of maximum [m]
     */
    
    double getMaximum(const double E) const
    {
      const double a = a0 + a1 * log(E); 

      return (a-1)*b;
    }


    /**
     * Get minimal shower size.
     *
     * \return         size [m]
     */
    static double getMinimalShowerSize()
    {
      return 1e-6;
    }

  protected:
    const double a0;
    const double a1;
    const double b;
    const double Emin;
  };


  /**
   * Function object for longitudinal EM-shower profile
   */
  static const JGeanz geanz(1.85, 0.62, 0.54);
}

#endif
