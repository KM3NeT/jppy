#ifndef __JOSCPROB__JBASELINECALCULATOR__
#define __JOSCPROB__JBASELINECALCULATOR__

#include "JIO/JSerialisable.hh"


/**
 * \author bjung
 * Auxiliary data structure for storing and computing oscillation baselines.
 */

namespace JOSCPROB {}
namespace JPP { using namespace JOSCPROB; }

namespace JOSCPROB {

  using JIO::JReader;
  using JIO::JWriter;
  using JIO::JSerialisable;


  /**
   * Auxiliary data structure for storing and calculating baselines.
   */
  struct JBaselineCalculator :
    public JSerialisable
  {
    /**
     * Default constructor.
     */
    JBaselineCalculator() :
      Lmin(0.0),
      Lmax(0.0)
    {}


    /**
     * Constructor.
     *
     * \param  Lmin     Minimum baseline [km]
     * \param  Lmax     Maximum baseline [km]
     */
    JBaselineCalculator(const double Lmin,
			const double Lmax) :
      Lmin(Lmin),
      Lmax(Lmax)
    {}


    /**
     * Get minimum baseline.
     *
     * \return                 maximum baseline [km]
     */
    double getMinimumBaseline() const
    {
      return Lmin;
    }

    
    /**
     * Get maximum baseline.
     *
     * \return                 maximum baseline [km]
     */
    double getMaximumBaseline() const
    {
      return Lmax;
    }


    /**
     * Get inner radius.
     *
     * \return                inner radius [km]
     */
    double getInnerRadius() const
    {
      return 0.5 * (Lmax - Lmin);
    }


    /**
     * Get outer radius.
     *
     * \return                outer radius [km]
     */
    double getOuterRadius() const
    {
      return 0.5 * (Lmax + Lmin);;
    }
    

    /**
     * Get cosine zenith angle for a given baseline.
     *
     * \param  L              baseline [km]
     * \return                cosine zenith angle
     */
    double getCosth(const double L) const
    {
      static const double r = getInnerRadius();
      static const double R = getOuterRadius();
      
      return (R*R - r*r - L*L) / (2*L*r);
    }

    
    /**
     * Get baseline for a given cosine zenith angle.
     *
     * \param  costh           cosine zenith angle
     * \return                 baseline [km]
     */
    double getBaseline(const double costh) const
    {
      static const double r = getInnerRadius();
      static const double R = getOuterRadius();

      const double ct = (fabs(costh) < 1.0 ? costh : (costh < 0 ? -1.0 : 1.0));
    
      return (-r * ct + sqrt(R*R - r*r * (1 - ct) * (1 + ct)));
    }


    /**
     * Get baseline for a given cosine zenith angle.
     *
     * \param  costh           cosine zenith angle
     * \return                 baseline [km]
     */
    double operator()(const double costh) const
    {
      return getBaseline(costh);
    }


    /**
     * Binary stream input of baseline extrema.
     *
     * \param  in              input stream
     * \return                 input stream
     */
    JReader& read(JReader& in) override
    {
      return in >> Lmin >> Lmax;
    }

    
    /**
     * Binary stream output of oscillation parameters.
     *
     * \param  out             output stream
     * \return                 output stream
     */
    JWriter& write(JWriter& out) const override
    {
      return out << Lmin << Lmax;
    }


    /**
     * Stream input of baseline calculator.
     *
     * \param  in              input stream
     * \param  object          object
     * \return                 input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JBaselineCalculator& object)
    {
      return in >> object.Lmin >> object.Lmax;
    }


    /**
     * Stream output of baseline calculator.
     *
     * \param  out             output stream
     * \param  object          object
     * \return                 output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JBaselineCalculator& object)
    {
      return out << FIXED(15,5) << object.Lmin << FIXED(15,5) << object.Lmax;
    }

    
  protected:
    
    double Lmin; //!< Minimum baseline [km]
    double Lmax; //!< Maximum baseline [km]
  };
}

#endif
