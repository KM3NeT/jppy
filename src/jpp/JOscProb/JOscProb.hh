#ifndef __JOSCPROB__JOSCPROB__
#define __JOSCPROB__JOSCPROB__

#include "JLang/JClonable.hh"

#include "JOscProb/JOscChannel.hh"


/**
 * \author bjung
 */

namespace JOSCPROB {

  using JLANG::JClonable;
  

  /**
   * Low-level interface for retrieving the oscillation probability\n
   * corresponding to a given oscillation channel, neutrino energy and zenith angle.
   */
  struct JOscProb :
    public JClonable<JOscProb>
  {
    /**
     * Virtual destructor.
     */
    virtual ~JOscProb()
    {}

    
    /**
     * Get oscillation probability for given oscillation channel.
     *
     * \param  channel            oscillation channel
     * \param  energy             neutrino energy     [GeV]
     * \param  costh              cosine zenith angle
     * \return                    oscillation probability
     */
    virtual double getOscProb(const JOscChannel& channel,
			      const double       energy,
			      const double       costh) const = 0;
  };
}

#endif
