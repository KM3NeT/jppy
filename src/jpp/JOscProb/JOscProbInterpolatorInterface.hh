#ifndef __JOSCPROB__JOSCPROBINTERPOLATORINTERFACE__
#define __JOSCPROB__JOSCPROBINTERPOLATORINTERFACE__


#include "JLang/JClonable.hh"

#include "JIO/JSerialisable.hh"

#include "JOscProb/JOscChannel.hh"
#include "JOscProb/JOscParameters.hh"
#include "JOscProb/JBaselineCalculator.hh"


/**
 * \author bjung, mdejong
 */

namespace JOSCPROB {

  using JLANG::JClonable;

  using JIO::JSerialisable;


  /**
   * Low-level interface for oscillation probability tables.
   */
  class JOscProbInterpolatorInterface :
    public JSerialisable,
    public JClonable<JOscProbInterpolatorInterface>
  {
  public:

    /**
     * Default constructor.
     */
    JOscProbInterpolatorInterface()
    {}


    /**
     * Virtual destructor.
     */
    virtual ~JOscProbInterpolatorInterface()
    {}
    

    /**
     * Load oscillation probability table from file.
     *
     * \param  fileName        oscillation probability table fileName
     */
    virtual void load(const char* fileName) = 0;
    
    
    /**
     * Get oscillation parameters.
     *
     * \return                 oscillation parameters
     */
    virtual const JOscParameters& getTableParameters() const = 0;


    /**
     * Get baseline calculator associated with this interpolation table.
     *
     * \return                    baseline calculator
     */
    virtual const JBaselineCalculator& getBaselineCalculator() const = 0;
    

    /**
     * Set oscillation parameters.
     *
     * \param  parameters      oscillation parameters
     */
    virtual void set(JOscParameters parameters) = 0;


    /**
     * Get oscillation probability for a given oscillation channel.
     *
     * \param  channel            oscillation channel
     * \param  E                  neutrino energy [GeV]
     * \param  costh              cosine zenith angle
     * \return                    oscillation probability
     */
    virtual double operator()(const JOscChannel&    channel,
			      const double          E,
			      const double          costh) const = 0;
    
    
    /**
     * Get oscillation probability for a given set of oscillation parameters\n
     * and a given oscillation channel.
     *
     * \param  channel            oscillation channel
     * \param  parameters         oscillation parameters
     * \param  E                  neutrino energy [GeV]
     * \param  costh              cosine zenith angle
     * \return                    oscillation probability
     */
    virtual double operator()(const JOscParameters& parameters,
			      const JOscChannel&    channel,
			      const double          E,
			      const double          costh)
    {
      set(parameters);

      return (*this)(channel, E, costh);      
    }
  };
}

#endif
