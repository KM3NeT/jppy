#ifndef __JOSCPROB__JOSCPROBTOOLKIT__
#define __JOSCPROB__JOSCPROBTOOLKIT__

#include <string>

#include "JLang/JException.hh"

#include "JOscProb/JOscChannel.hh"


/**
 * \author bjung
 * Auxiliary methods for oscillation probabilities.
 */

namespace JOSCPROB {}
namespace JPP { using namespace JOSCPROB; }

namespace JOSCPROB {

  using JLANG::JValueOutOfRange;
  

  /**
   * OscProb neutrino flavour identifiers.
   */
  enum class OscProbFlavour_t { ELECTRON,
				MUON,
				TAU };
  
  
  /**
   * Auxiliary function for retrieving the OscProb flavour identifier corresponding to a JOscProb flavour identifier.
   *
   * \param  flavour             flavour identifier
   * \return                     OscProb flavour identifier
   */
  inline OscProbFlavour_t getOscProbFlavour(const JFlavour_t flavour)
  {
    switch(flavour) {
    case JFlavour_t::ELECTRON:
      return OscProbFlavour_t::ELECTRON;
    case JFlavour_t::MUON:
      return OscProbFlavour_t::MUON;
    case JFlavour_t::TAU:
      return OscProbFlavour_t::TAU;
    default:
      THROW(JLANG::JValueOutOfRange, "getOscProbFlavour(...): Invalid flavour " << (int) flavour);
    }
  }


  /**
   * Auxiliary function for retrieving the OscProb flavour identifier corresponding to a JOscProb flavour identifier.
   *
   * \param  flavour             flavour identifier
   * \return                     OscProb flavour identifier
   */
  inline OscProbFlavour_t getOscProbFlavour(const int pdgType)
  {
    JFlavour_t flavour = getFlavour(pdgType);
    
    return getOscProbFlavour(flavour);
  }


  /**
   * Auxiliary data structure to hold oscillation variable names.
   */
  struct JOscVars
  {
    /**
     * Oscillation variable types.
     */
    enum type {
      COSTH,
      SINTH,
      ENERGY,
      LOG10E,
      LOE,
      BASELINE,
      UNDEFINED
    };

    
    static const char* const energy() { return "energy"; }   //!< energy [GeV]
    static const char* const log10E() { return "log10E"; }   //!< logarithmic energy [GeV]
    static const char* const LoE()    { return "LoE"; }      //!< L/E [km GeV-1]
    
    static const char* const costh()  { return "costh"; }    //!< cosine of zenith-angle
    static const char* const sinth()  { return "sinth"; }    //!< sine of zenith-angle
    static const char* const L()      { return "L"; }        //!< sine of zenith-angle

    
    /**
     * Get oscillation variable type.
     *
     * \param  name                              oscillation variable name
     * \return                                   oscillation variable type
     */
    static inline type getType(const std::string& name)
    {
      if        (name == energy()) {
	return ENERGY;
      } else if (name == costh()) {
	return COSTH;
      } else if (name == sinth()) {
	return SINTH;
      } else if (name == log10E()) {
	return LOG10E;	
      } else if (name == LoE()) {
	return LOE;
      } else if (name == L()) {
	return BASELINE;	
      } else {
	THROW(JValueOutOfRange, "JOscVars::getType(): Invalid oscillation variable " << name);
      }
    }
  };
}

#endif
