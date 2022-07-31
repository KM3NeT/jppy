#ifndef __JPHYSICS__JGEANT_T__
#define __JPHYSICS__JGEANT_T__

#include "JTools/JFunction1D_t.hh"
#include "JIO/JSerialisable.hh"

/**
 * \file
 * Base class for photon emission profile EM-shower.
 * \author mdejong
 */

namespace JPHYSICS {}
namespace JPP { using namespace JPHYSICS; }

namespace JPHYSICS {

  using JIO::JReader;
  using JIO::JWriter;

  typedef JTOOLS::JGridPolint1Function1D_t  JGeantFunction1D_t;


  /**
   * Base class for the probability density function of photon emission from EM-shower 
   * as a function of the index of refraction and the cosine of the emission angle.
   *
   * The implementation of this function is based on a linear interpolation of tabulated values.
   * In this, a linear approximation of the dependence of the normalisation constant on 
   * the index of refraction is assumed. This assumption is valid to within 10^-3.
   */
  class JGeant_t : 
    public JGeantFunction1D_t
  {
  public:
    /**
     * Default constructor.
     */
    JGeant_t() 
    {}
 

    /**
     * Number of photons from EM-shower as a function of emission angle.
     * The integral over full solid angle is normalised to one.
     *
     * \param  n       index of refraction
     * \param  ct      cosine angle of emmision
     * \return         d^2P/dcos()dphi
     */
    double operator()(const double n,
		      const double ct) const
    {
      const double y = JGeantFunction1D_t::operator()(ct - 1.0/n);

      return y * (a0 - a1*n);
    }


    /**
     * Integral number of photons from EM-shower between two emission angles.
     * The integral over full solid angle is normalised to one.
     *
     * \param  n       index of refraction
     * \param  xmin    minimal cosine angle of emmision
     * \param  xmax    maximal cosine angle of emmision
     * \return         dnpe/dphi 
     */
    double operator()(const double n, 
		      const double xmin, 
		      const double xmax) const
    {
      const double x_min = std::max(xmin - 1.0/n, buffer. begin()->getX());
      const double x_max = std::min(xmax - 1.0/n, buffer.rbegin()->getX());

      const double y = buffer(x_max) - buffer(x_min);

      return y * (a0 - a1*n);
    }


    /**
     * Read geant from input.
     *
     * \param  in       reader
     * \param  geant    geant
     * \return          reader
     */
    friend inline JReader& operator>>(JReader& in, JGeant_t& geant)
    {
      in >> geant.a0;
      in >> geant.a1;
      in >> static_cast<JGeantFunction1D_t&>(geant);

      geant.compile();

      return in;
    }


    /**
     * Write geant to output.
     *
     * \param  out      writer
     * \param  geant    geant
     * \return          writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JGeant_t& geant)
    {
      out << geant.a0;
      out << geant.a1;
      out << static_cast<const JGeantFunction1D_t&>(geant);

      return out;
    }


  protected:
    /**
     * Function compilation.
     */
    virtual void do_compile() override 
    {
      JGeantFunction1D_t::do_compile();

      buffer.clear();

      JTOOLS::integrate(*this, buffer);

      buffer.compile();
    }
 

    double a0;     //!< offset of the normalisation dependence
    double a1;     //!< slope  of the normalisation dependence
    JGeantFunction1D_t buffer;
  };
}

#endif
