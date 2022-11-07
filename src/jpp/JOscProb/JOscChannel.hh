#ifndef __JOSCPROB__JOSCCHANNEL__
#define __JOSCPROB__JOSCCHANNEL__

#include <iostream>

#include "JLang/JComparable.hh"

#include "Jeep/JProperties.hh"


/**
 * \author bjung
 * \file
 * Oscillation channels and auxiliary methods.
 */

namespace JOSCPROB {}
namespace JPP { using namespace JOSCPROB; }

namespace JOSCPROB {

  using JLANG::JEquationParameters;


  /**
   * Neutrino flavours.
   */
  enum class JFlavour_t      { ELECTRON           = 12,
			       MUON               = 14,
			       TAU                = 16,
			       FLAVOUR_UNDEFINED  =  0 };


  /**
   * Charge parities.
   */
  enum class JChargeParity_t { ANTIPARTICLE       = -1,
			       PARTICLE           = +1,
			       CPARITY_UNDEFINED  =  0 };
  
  
  /**
   * Auxiliary function for retrieving the flavour corresponding to a given PDG identifier.
   *
   * \param  pdgType             PDG particle identifier
   */
  inline JFlavour_t getFlavour(const int pdgType)
  {
    const int type = abs(pdgType);
      
    switch (type) {
	
    case (int) JFlavour_t::ELECTRON:
    case (int) JFlavour_t::MUON:
    case (int) JFlavour_t::TAU:
      return static_cast<JFlavour_t>(type);
    default:
      return JFlavour_t::FLAVOUR_UNDEFINED;
    }
  }


  /**
   * Auxiliary function for retrieving the charge-parity of a given PDG type.
   *
   * \param  pdgType             PDG particle identifier
   * \return                     charge-parity (1 for neutrinos; -1 for anti-neutrinos)
   */
  inline JChargeParity_t getChargeParity(const int pdgType)
  {
    if        (pdgType < 0) {
      return JChargeParity_t::ANTIPARTICLE;
    } else if (pdgType > 0) {
      return JChargeParity_t::PARTICLE;
    } else {
      return JChargeParity_t::CPARITY_UNDEFINED;
    }
  }
  
  
  /**
   * Neutrino oscillation channel.
   */  
  struct JOscChannel :
    public JLANG::JComparable<JOscChannel>
  {
    /**
     * Default constructor.
     */
    JOscChannel() :
      in     (JFlavour_t::FLAVOUR_UNDEFINED),
      out    (JFlavour_t::FLAVOUR_UNDEFINED),
      Cparity(JChargeParity_t::CPARITY_UNDEFINED)
    {}


    /**
     * Constructor.				
     *
     * \param  in              input  flavour
     * \param  out             output flavour
     * \param  Cparity         charge parity
     */
    JOscChannel(const JFlavour_t      in,
		const JFlavour_t      out,
		const JChargeParity_t Cparity) :
      in (in),
      out(out),
      Cparity(Cparity)
    {}


    /**
     * Constructor.
     *
     * \param  in              input  flavour
     * \param  out             output flavour
     * \param  Cparity         charge parity
     */
    JOscChannel(const int in,
		const int out,
		const int Cparity) :
      in (getFlavour(in)),
      out(getFlavour(out)),
      Cparity(getChargeParity(Cparity))
    {}


    /**
     * Check validity of this oscillation channel.
     *
     * \return                 true if this oscillation channel is valid; else false.
     */
    inline bool is_valid() const
    {
      return (in      != JFlavour_t::FLAVOUR_UNDEFINED &&
	      out     != JFlavour_t::FLAVOUR_UNDEFINED &&
	      Cparity != JChargeParity_t::CPARITY_UNDEFINED);
    }


    /**
     * Less-than method
     *
     * \param  channel         channel
     * \return                 true this channel less than given channel; else false 
     */
    inline bool less(const JOscChannel& channel) const
    {
      if (this->Cparity == channel.Cparity) {
	
	if (this->in == channel.in) {
	  
	  return this->out < channel.out;
	  
	} else {
	  
	  return this->in < channel.in;
	}
	
      } else {
	
	return this->Cparity < channel.Cparity;
      }
    }    


    /**
     * Write channel to output.
     *
     * \param  out           output stream
     * \param  object        oscillation channel
     * \return               output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JOscChannel& object)
    {
      return out << object.getProperties();
    }


    /** 
     * Read channel from input.
     *
     * \param  in            input stream
     * \param  object        oscillation channel
     * \return               input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JOscChannel& object)
    {
      JProperties properties(object.getProperties());

      in >> properties;

      object.setProperties(properties);
      
      return in;
    }


    /**
     * Get equation parameters.
     *
     * \return                 equation parameters
     */
    static inline JEquationParameters& getEquationParameters()
    {
      static JEquationParameters equation("=", "\n\r;,", "./", "#");

      return equation;
    }
      

    /**
     * Set equation parameters.
     *
     * \param  equation        equation parameters
     */
    static inline void setEquationParameters(const JEquationParameters& equation)
    {
      getEquationParameters() = equation;
    }

    
    /**
     * Get properties of this class.
     *
     * \param  equation        equation parameters
     */
    JProperties getProperties(const JEquationParameters& equation = JOscChannel::getEquationParameters())
    {
      return JOscChannelHelper(*this, equation);
    }


    /**
     * Get properties of this class.
     *
     * \param  equation        equation parameters
     */
    JProperties getProperties(const JEquationParameters& equation = JOscChannel::getEquationParameters()) const
    {
      return JOscChannelHelper(*this, equation);
    }


    /**
     * Set properties of this class
     *
     * \param  properties      properties
     */
    void setProperties(const JProperties& properties)
    {
      this->in      = getFlavour     (properties.getValue<const int>("in"));
      this->out     = getFlavour     (properties.getValue<const int>("out"));
      this->Cparity = getChargeParity(properties.getValue<const int>("Cparity"));
    }


    JFlavour_t      in;      //!< Incoming  flavour
    JFlavour_t      out;     //!< Outcoming flavour
    JChargeParity_t Cparity; //!< Charge-parity

    
  private:
    /**
     * Auxiliary class for I/O of oscillation channel.
     */
    struct JOscChannelHelper :
      public JProperties
    {
      /**
       * Constructor.
       *
       * \param  object        oscillation channel
       * \param  equation      equation    parameters 
       */
      template<class JOscChannel_t>
      JOscChannelHelper(JOscChannel_t&             object,
			const JEquationParameters& equation) :
	JProperties(equation, 1),
	in         ((int) object.in),
	out        ((int) object.out),
	Cparity    ((int) object.Cparity)
      {
	this->insert(gmake_property(in));
	this->insert(gmake_property(out));
	this->insert(gmake_property(Cparity));
      }

      int in;
      int out;
      int Cparity;
    };
  };


  /**
   * Declare group of neutrino oscillation channels.
   */
  static const JOscChannel getOscChannel[]  =  {
    JOscChannel(JFlavour_t::ELECTRON,  JFlavour_t::ELECTRON,  JChargeParity_t::PARTICLE),
    JOscChannel(JFlavour_t::ELECTRON,  JFlavour_t::MUON,      JChargeParity_t::PARTICLE),
    JOscChannel(JFlavour_t::ELECTRON,  JFlavour_t::TAU,       JChargeParity_t::PARTICLE),
    JOscChannel(JFlavour_t::MUON,      JFlavour_t::ELECTRON,  JChargeParity_t::PARTICLE),
    JOscChannel(JFlavour_t::MUON,      JFlavour_t::MUON,      JChargeParity_t::PARTICLE),
    JOscChannel(JFlavour_t::MUON,      JFlavour_t::TAU,       JChargeParity_t::PARTICLE),
    JOscChannel(JFlavour_t::TAU,       JFlavour_t::ELECTRON,  JChargeParity_t::PARTICLE),
    JOscChannel(JFlavour_t::TAU,       JFlavour_t::MUON,      JChargeParity_t::PARTICLE),
    JOscChannel(JFlavour_t::TAU,       JFlavour_t::TAU,       JChargeParity_t::PARTICLE),
    JOscChannel(JFlavour_t::ELECTRON,  JFlavour_t::ELECTRON,  JChargeParity_t::ANTIPARTICLE),
    JOscChannel(JFlavour_t::ELECTRON,  JFlavour_t::MUON,      JChargeParity_t::ANTIPARTICLE),
    JOscChannel(JFlavour_t::ELECTRON,  JFlavour_t::TAU,       JChargeParity_t::ANTIPARTICLE),
    JOscChannel(JFlavour_t::MUON,      JFlavour_t::ELECTRON,  JChargeParity_t::ANTIPARTICLE),
    JOscChannel(JFlavour_t::MUON,      JFlavour_t::MUON,      JChargeParity_t::ANTIPARTICLE),
    JOscChannel(JFlavour_t::MUON,      JFlavour_t::TAU,       JChargeParity_t::ANTIPARTICLE),
    JOscChannel(JFlavour_t::TAU,       JFlavour_t::ELECTRON,  JChargeParity_t::ANTIPARTICLE),
    JOscChannel(JFlavour_t::TAU,       JFlavour_t::MUON,      JChargeParity_t::ANTIPARTICLE),
    JOscChannel(JFlavour_t::TAU,       JFlavour_t::TAU,       JChargeParity_t::ANTIPARTICLE)
  };


  /**
   * Number of neutrino oscillation channels.
   */
  static const unsigned int NUMBER_OF_OSCCHANNELS = sizeof(getOscChannel) / sizeof(JOscChannel);
}

#endif
