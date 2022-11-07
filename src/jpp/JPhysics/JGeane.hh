#ifndef __JPHYSICS__JGEANE__
#define __JPHYSICS__JGEANE__

#include <cmath>
#include <map>

#include "JPhysics/JConstants.hh"


/**
 * \file
 * Energy loss of muon.
 * \author mdejong
 */

namespace JPHYSICS {}
namespace JPP { using namespace JPHYSICS; }

namespace JPHYSICS {

  /**
   * Equivalent muon track length per unit shower energy.
   *
   * See ANTARES internal note ANTARES-SOFT-2002-015, J.\ Brunner.
   *
   * \return        equivalent muon track length [m/Gev]
   */
  inline double geanc()
  {
    return 4.7319;                   // dx/dE [m/GeV]
  }


  /**
   * Interface for muon energy loss.
   *
   * This interface provides for the various function object operators.
   */
  class JGeane {
  public:
   /**
     * Get energy loss constant.
     *
     * \return         Energy loss due to ionisation [GeV/m]
     */
    virtual double getA() const = 0;


    /**
     * Get energy loss constant.
     *
     * \return         Energy loss due to pair production and bremsstrahlung [m^-1]
     */
    virtual double getB() const = 0;


    /**
     * Get energy of muon after specified distance.
     *
     * \param  E       Energy of muon [GeV]
     * \param  dx      distance traveled [m]
     * \return         Energy of muon [GeV]
     */
    virtual double getE(const double E, const double dx) const = 0;


    /**
     * Get distance traveled by muon.
     *
     * \param  E0      Energy of muon at start [GeV]
     * \param  E1      Energy of muon at end   [GeV]
     * \return         distance traveled [m]
     */
    virtual double getX(const double E0, 
			const double E1) const = 0;


    /**
     * Energy of muon after specified distance.
     *
     * \param  E       Energy of muon [GeV]
     * \param  dx      distance traveled [m]
     * \return         Energy of muon [GeV]
     */
    double operator()(const double E, const double dx) const
    {
      return this->getE(E, dx);
    }
    

    /**
     * Range of muon.
     *
     * \param  E       Energy of muon [GeV]
     * \return         range [m]
     */
    double operator()(const double E) const
    {
      return this->getX(E, 0.0);
    }


    /**
     * Equivalent unit track length per unit shower energy and per unit track length.
     *
     * \return        equivalent unit track length [Gev^-1]
     */
    double operator()() const
    {
      return this->getB() * geanc();
    }
  };


  /**
   * Function object for the energy loss of the muon.\n
   * The energy loss can be formulated as:
   *
   *     \f[ -\frac{dE}{dx}  =  a + bE \f]
   *
   * N.B:
   * \f$ a \f$ and \f$ b \f$ are assumed constant (internal units m and GeV, respectively).
   */
  class JGeane_t :
    public JGeane
  {
  public:
    /**
     * constructor
     * \param  __a     Energy loss due to ionisation [GeV/m]
     * \param  __b     Energy loss due to pair production and bremsstrahlung [m^-1]
     */ 
    JGeane_t(const double __a,
	     const double __b) :
      a(__a),
      b(__b)
    {}
    

    /**
     * Get energy loss constant.
     *
     * \return         Energy loss due to ionisation [GeV/m]
     */
    virtual double getA() const override 
    {
      return a;
    }


    /**
     * Get energy loss constant.
     *
     * \return         Energy loss due to pair production and bremsstrahlung [m^-1]
     */
    virtual double getB() const override 
    {
      return b;
    }


    /**
     * Get energy of muon after specified distance.
     *
     * \param  E       Energy of muon [GeV]
     * \param  dx      distance traveled [m]
     * \return         Energy of muon [GeV]
     */
    virtual double getE(const double E, const double dx) const override 
    {
      const double y = (a/b + E) * exp(-b*dx)  -  a/b;

      if (y > 0.0)
	return y;
      else
	return 0.0;
    }


    /**
     * Get distance traveled by muon.
     *
     * \param  E0      Energy of muon at start [GeV]
     * \param  E1      Energy of muon at end   [GeV]
     * \return         distance traveled [m]
     */
    virtual double getX(const double E0, 
			const double E1) const override
    {
      return -log((a + b*E1) / (a+b*E0)) / b;
    }

  protected:
    const double a;
    const double b;
  };


  /**
   * Function object for energy dependent energy loss of the muon.
   *
   * Approximate values of energy loss parameters taken from reference:
   * Proceedings of ICRC 2001, "Precise parametrizations of muon energy losses in water",
   * S. Klimushin, E. Bugaev and I. Sokalski.
   */
  class JGeaneWater : 
    public JGeane,
    protected std::map<double, JGeane_t>
  {    
  public:    
    /**
     * Default constructor.
     */
    JGeaneWater()
    {
      using namespace std;

      this->insert(make_pair( 0.0e0,  JGeane_t( 2.30e-1 * DENSITY_SEA_WATER, 15.50e-4 * DENSITY_SEA_WATER)));
      this->insert(make_pair(30.0e0,  JGeane_t( 2.67e-1 * DENSITY_SEA_WATER,  3.40e-4 * DENSITY_SEA_WATER)));
      this->insert(make_pair(35.3e3,  JGeane_t(-6.50e-1 * DENSITY_SEA_WATER,  3.66e-4 * DENSITY_SEA_WATER)));
    }
    

    /**
     * Get energy loss constant.
     *
     * N.B. The return value corresponds to the low-energy regime.
     *
     * \return         Energy loss due to ionisation [GeV/m]
     */
    virtual double getA() const override 
    {
      return 2.30e-1 * DENSITY_SEA_WATER; //This is the value for low energy (<30 GeV), the value used for step(ds) and getRange())
    }


    /**
     * Get energy loss constant.
     *
     * N.B. The return value corresponds to the medium-energy regime.
     *
     * \return         Energy loss due to pair production and bremsstrahlung [m^-1]
     */
    virtual double getB() const override 
    {
      return 3.40e-4 * DENSITY_SEA_WATER;
    }


    /**
     * Get energy of muon after specified distance.
     *
     * \param  E       Energy of muon [GeV]
     * \param  dx      distance traveled [m]
     * \return         Energy of muon [GeV]
     */
    virtual double getE(const double E, const double dx) const override 
    {
      double E1 = E;
      double x1 = dx;

      if (E1 > MASS_MUON / getSinThetaC()) {

	const_iterator p = this->lower_bound(E1);

	do {

	  --p;

	  const double x2 = p->second.getX(E1, p->first);

	  if (x2 > x1) {
	    return p->second.getE(E1, x1);
	  }

	  E1  = p->first;
	  x1 -= x2;

	} while (p != this->begin());
      }

      return E1;
    }




    /**
     * Get energy loss due to ionisation.
     *
     * \param  E           initial energy                [GeV]
     * \param  dx          distance traveled             [m]
     * \return             energy loss due to ionisation [GeV]
     */
    double getEa(const double E, const double dx) const
    {
      using namespace std;
      using namespace JPP;
      
      double Ea = 0.0;
      
      double E1 = E;
      double x1 = dx;

      if (E1 > MASS_MUON / getSinThetaC()) {

	map<double, JGeane_t>::const_iterator p = this->lower_bound(E1);

	do {

	  --p;

	  const double x2 = p->second.getX(E1, p->first);

	  Ea += (x2 > x1 ? x1 : x2) * p->second.getA();
	  E1  = p->first;
	  
	  x1 -= x2;
	  
	} while (p != this->cbegin() && x1 > 0.0);
      }

      return Ea;
    }


    /**
     * Get energy loss due to pair production and bremsstrahlung.
     *
     * \param  E           initial energy                                        [GeV]
     * \param  dx          distance traveled                                     [m]
     * \return             energy loss due to pair production and bremsstrahlung [GeV]
     */
    double getEb(const double E, const double dx) const
    {
      const double dE = E - getE(E, dx);
      
      return dE - getEa(E, dx);
    }


    /**
     * Get distance traveled by muon.
     *
     * \param  E0      Energy of muon at start [GeV]
     * \param  E1      Energy of muon at end   [GeV]
     * \return         distance traveled       [m]
     */
    virtual double getX(const double E0, 
			const double E1) const override
    {
      double E  = E0;
      double dx = 0.0;

      if (E > MASS_MUON / getSinThetaC()) {

	const_iterator p = this->lower_bound(E);

	do {

	  --p;

	  if (E1 > p->first) {
	    return dx + p->second.getX(E, E1);
	  }

	  dx += p->second.getX(E, p->first);
	  E   = p->first;

	} while (p != this->begin());
      }

      return dx;
    }
  };


  /**
   * Function object for energy loss of muon in sea water.
   */
  static const JGeaneWater gWater;


  /**
   * Function object for energy loss of muon in rock.
   */
  static const JGeane_t    gRock(2.67e-1 * 0.9 * DENSITY_ROCK, 
				 3.40e-4 * 1.2 * DENSITY_ROCK);

}

#endif
