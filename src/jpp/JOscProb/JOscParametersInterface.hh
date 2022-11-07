#ifndef __JOSCPROB__JOSCPARAMETERSINTERFACE__
#define __JOSCPROB__JOSCPARAMETERSINTERFACE__

#include <iostream>
#include <iomanip>
#include <string>

#include "JSystem/JStat.hh"

#include "Jeep/JPrint.hh"
#include "Jeep/JProperties.hh"

#include "JIO/JSerialisable.hh"

#include "JLang/JEquals.hh"
#include "JLang/JParameter.hh"
#include "JLang/JVectorize.hh"
#include "JLang/JStringStream.hh"
#include "JLang/JObjectStreamIO.hh"
#include "JLang/JEquationParameters.hh"


/**
 * \author bjung, mdejong
 */

namespace JOSCPROB {}
namespace JPP { using namespace JOSCPROB; }

namespace JOSCPROB {

  using JIO::JReader;
  using JIO::JWriter;
  using JIO::JSerialisable;

  using JLANG::JEquals;
  using JLANG::JParameter;
  using JLANG::JObjectStreamIO;
  using JLANG::JValueOutOfRange;
  using JLANG::JEquationParameters;

  
  /**
   * Abstract base class for sets of oscillation parameters.
   */
  template<class T>
  struct JOscParametersInterface :
    public JSerialisable,
    public JObjectStreamIO<JOscParametersInterface<T> >,
    public JEquals        <JOscParametersInterface<T> >
  {
    typedef JOscParametersInterface<T>                      JOscParameters_t;
    typedef JParameter<T>                                       JParameter_t;
    typedef typename JParameter_t::argument_type               argument_type;


    /**
     * Default constructor.
     */
    JOscParametersInterface() :
      dM21sq   (),
      dM31sq   (),
      deltaCP  (),
      sinsqTh12(),
      sinsqTh13(),
      sinsqTh23()
    {}


    /**
     * Constructor.
     *
     * \param  dM21sq          Squared mass difference between the first and second neutrino mass eigenstates               [eV2]
     * \param  dM31sq          Squared mass difference between the first and third neutrino mass eigenstates                [eV2]
     * \param  deltaCP         PMNS phase angle                                                                             [pi rad]
     * \param  sinsqTh12       Squared sine of the PMNS mixing angle between the first and second neutrino mass eigenstates [-]
     * \param  sinsqTh13       Squared sine of the PMNS mixing angle between the first and third neutrino mass eigenstates  [-]
     * \param  sinsqTh23       Squared sine of the PMNS mixing angle between the second and third neutrino mass eigenstates [-]
     */
    JOscParametersInterface(const T& dM21sq,
			    const T& dM31sq,
			    const T& deltaCP,
			    const T& sinsqTh12,
			    const T& sinsqTh13,
			    const T& sinsqTh23) :
      dM21sq   (dM21sq),
      dM31sq   (dM31sq),
      deltaCP  (deltaCP),
      sinsqTh12(sinsqTh12),
      sinsqTh13(sinsqTh13),
      sinsqTh23(sinsqTh23)
    {}


    /**
     * Constructor.
     * 
     * \param  name            parameter name
     * \param  value           parameter value
     * \param  args            remaining pairs of parameter names and values
     */
    template<class ...Args>
    JOscParametersInterface(const std::string& name,
			    const T&           value,
			    const Args&     ...args)
    {
      set(name, value, args...);
    }


    /**
     * Set value for a given oscillation parameter.
     *
     * \param  name            parameter name
     * \param  value           parameter value
     */
    void set(const std::string& name,
	     const T&           value)
    {
      JProperties properties = this->getProperties();

      JProperties::iterator i = properties.find(name);

      if (i != properties.end()) {

	i->second.setValue(JParameter_t(value));
	
      } else {
	
        THROW(JValueOutOfRange,
	      "template<class T> JOscParametersInterface<T>::set(const std::string&, const T&): " <<
	      "Invalid oscillation parameter name " << name << "; Valid options:\n"  << JLANG::get_keys(properties));
      }
    }


    /**
     * Set value for given list of oscillation parameters.
     *
     * \param  name            parameter name
     * \param  value           parameter value
     * \param  args            remaining pairs of parameter names and values
     */
    template<class ...Args>
    void set(const std::string& name,
	     const T&           value,
	     const Args&     ...args)
    {
      set(name, value);
      set(args...);
    }


    /**
     * Join the given oscillation parameters with these oscillation parameters.
     *
     * \param  parameters      oscillation parameters
     */
    JOscParameters_t& join(const JOscParameters_t& parameters)
    {
      if (parameters.dM21sq.isDefined())    { this->dM21sq    = parameters.dM21sq;    }
      if (parameters.dM31sq.isDefined())    { this->dM31sq    = parameters.dM31sq;    }
      if (parameters.deltaCP.isDefined())   { this->deltaCP   = parameters.deltaCP;   }
      if (parameters.sinsqTh12.isDefined()) { this->sinsqTh12 = parameters.sinsqTh12; }
      if (parameters.sinsqTh13.isDefined()) { this->sinsqTh13 = parameters.sinsqTh13; }
      if (parameters.sinsqTh23.isDefined()) { this->sinsqTh23 = parameters.sinsqTh23; }
      
      return *this;
    }
   

    /**
     * Get oscillation parameters.
     *
     * \return                 oscillation parameters
     */
    const JOscParameters_t& getOscParameters() const
    {
      return static_cast<const JOscParameters_t&>(*this);
    }


    /**
     * Set oscillation parameters.
     *
     * \param  parameters      oscillation parameters
     */
    void setOscParameters(const JOscParameters_t& parameters)
    {
      static_cast<JOscParameters_t&>(*this) = parameters;
    }


    /**
     * Check validity of oscillation parameters.
     *
     * \return                 true if valid; else false
     */
    virtual bool is_valid() const = 0;


    /**
     * Get size of this oscillation parameters set.
     *
     * \return                 size (= number of defined parameters)
     */
    virtual unsigned int size() const
    {
      return ((unsigned int) this->dM21sq.isDefined()    +
	      (unsigned int) this->dM31sq.isDefined()    +
	      (unsigned int) this->deltaCP.isDefined()   +
	      (unsigned int) this->sinsqTh12.isDefined() +
	      (unsigned int) this->sinsqTh13.isDefined() +
	      (unsigned int) this->sinsqTh23.isDefined());
    }


    /**
     * Check if this oscillations parameter set contains the given oscillation parameters.
     *
     * \param  parameters      oscillation parameters
     * \return                 true if all given oscillation parameters\n
     *                         are also defined in this oscillation parameters set
     */
    virtual bool contains(const JOscParameters_t& parameters) const
    {
      if ( (parameters.dM21sq.isDefined()    && !this->dM21sq.isDefined())    ||
	   (parameters.dM31sq.isDefined()    && !this->dM31sq.isDefined())    ||
	   (parameters.deltaCP.isDefined()   && !this->deltaCP.isDefined())   ||
	   (parameters.sinsqTh12.isDefined() && !this->sinsqTh12.isDefined()) ||
	   (parameters.sinsqTh13.isDefined() && !this->sinsqTh13.isDefined()) ||
	   (parameters.sinsqTh23.isDefined() && !this->sinsqTh23.isDefined()) ) {
	return false;
      } else {
	return true;
      }
    }


    bool equals(const JOscParameters_t& parameters) const
    {
      return (this->dM21sq    == parameters.dM21sq    &&
	      this->dM31sq    == parameters.dM31sq    &&
	      this->deltaCP   == parameters.deltaCP   &&
	      this->sinsqTh12 == parameters.sinsqTh12 &&
	      this->sinsqTh13 == parameters.sinsqTh13 &&
	      this->sinsqTh23 == parameters.sinsqTh23);
    }


    /**
     * Stream input of oscillation parameters.
     *
     * \param  in              input stream
     * \param  parameters      oscillation parameters
     * \return                 input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JOscParameters_t& parameters)
    {
      using namespace std;
      using namespace JPP;
      
      JStringStream is(in);

      if (getFileStatus(is.str().c_str())) {
	is.load();
      }

      JProperties properties(parameters.getProperties());

      is >> properties;

      parameters.setProperties(properties);

      return in;
    }


    /**
     * Stream output of oscillation parameters.
     *
     * \param  out             output stream
     * \param  parameters      oscillation parameters
     * \return                 output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JOscParameters_t& parameters)
    {
      return out << parameters.getProperties();
    }


    /**
     * Binary stream input of oscillation parameters.
     *
     * \param  in              input stream
     * \return                 input stream
     */
    JReader& read(JReader& in) override
    {
      JProperties properties = getProperties();

      for (JProperties::iterator i = properties.begin(); i != properties.end(); ++i) {
	
	bool is_defined;
	T    value;
	
	if ((in >> is_defined >> value) && is_defined) {

	  JParameter_t& parameter = i->second.getValue<JParameter_t>();
	  
	  parameter.setValue(value);
	}
      }

      return in;
    }
    

    /**
     * Binary stream output of oscillation parameters.
     *
     * \param  out             output stream
     * \return                 output stream
     */
    JWriter& write(JWriter& out) const override
    {
      const JProperties properties = getProperties();

      for (JProperties::const_iterator i = properties.cbegin(); i != properties.cend(); ++i) {

	const JParameter_t& parameter = i->second.getValue<const JParameter_t>();	

	out << parameter.isDefined() << parameter.getValue();
      }

      return out;
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
    JProperties getProperties(const JEquationParameters& equation = JOscParameters_t::getEquationParameters())
    {
      return JOscParametersHelper(*this, equation);
    }


    /**
     * Get properties of this class.
     *
     * \param  equation        equation parameters
     */
    JProperties getProperties(const JEquationParameters& equation = JOscParameters_t::getEquationParameters()) const
    {
      return JOscParametersHelper(*this, equation);
    }


    /**
     * Set properties of this class
     *
     * \param  properties      properties
     */
    void setProperties(const JProperties& properties)
    {
      this->dM21sq    = properties.getValue<JParameter_t>("dM21sq");
      this->dM31sq    = properties.getValue<JParameter_t>("dM31sq");
      this->deltaCP   = properties.getValue<JParameter_t>("deltaCP");
      this->sinsqTh12 = properties.getValue<JParameter_t>("sinsqTh12");
      this->sinsqTh13 = properties.getValue<JParameter_t>("sinsqTh13");
      this->sinsqTh23 = properties.getValue<JParameter_t>("sinsqTh23");
    }


    JParameter_t dM21sq;     //!< Squared mass difference between the first and second neutrino mass eigenstates               [eV2]
    JParameter_t dM31sq;     //!< Squared mass difference between the first and third neutrino mass eigenstates                [eV2]
    JParameter_t deltaCP;    //!< PMNS phase angle                                                                             [pi * rad]
    JParameter_t sinsqTh12;  //!< Squared sine of the PMNS mixing angle between the first and second neutrino mass eigenstates [-]
    JParameter_t sinsqTh13;  //!< Squared sine of the PMNS mixing angle between the first and third neutrino mass eigenstates  [-]
    JParameter_t sinsqTh23;  //!< Squared sine of the PMNS mixing angle between the second and third neutrino mass eigenstates [-]

    
  private:
    
    /**
     * Auxiliary class for I/O of oscillation parameters.
     */
    struct JOscParametersHelper :
      public JProperties
    {
      /**
       * Constructor.
       *
       * \param  parameters    oscillation parameters
       * \param  equation      equation    parameters 
       */
      template<class JOscParameters_t>
      JOscParametersHelper(JOscParameters_t&          parameters,
			   const JEquationParameters& equation) :
	JProperties(equation, 1)
      {
	this->insert(gmake_property(parameters.dM21sq));
	this->insert(gmake_property(parameters.dM31sq));
	this->insert(gmake_property(parameters.deltaCP));	
	this->insert(gmake_property(parameters.sinsqTh12));
	this->insert(gmake_property(parameters.sinsqTh13));
	this->insert(gmake_property(parameters.sinsqTh23));
      }
    };
  };
}

#endif
