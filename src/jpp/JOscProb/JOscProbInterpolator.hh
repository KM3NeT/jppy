#ifndef __JOSCPROB__JOSCPROBINTERPOLATOR__
#define __JOSCPROB__JOSCPROBINTERPOLATOR__

#include "Jeep/JMessage.hh"
#include "Jeep/JProperties.hh"

#include "JIO/JSerialisable.hh"
#include "JIO/JFileStreamIO.hh"

#include "JLang/JClonable.hh"
#include "JLang/JObjectIO.hh"
#include "JLang/JException.hh"

#include "JTools/JPolint.hh"
#include "JTools/JMapList.hh"
#include "JTools/JCollection.hh"
#include "JTools/JFunction1D_t.hh"
#include "JTools/JMultiFunction.hh"
#include "JTools/JFunctionalMap_t.hh"

#include "JOscProb/JOscChannel.hh"
#include "JOscProb/JOscParameters.hh"
#include "JOscProb/JOscProbToolkit.hh"
#include "JOscProb/JBaselineCalculator.hh"
#include "JOscProb/JOscProbInterpolatorInterface.hh"


/**
 * \author bjung, mdejong
 */

namespace JOSCPROB {}
namespace JPP { using namespace JOSCPROB; }

namespace JOSCPROB {

  using JEEP::JMessage;

  using JLANG::JClonable;

  using JTOOLS::JMultiFunction;
  

  /** 
   * Template definition of a multi-dimensional oscillation probability interpolation table.
   */
  template<template<class, class> class JCollection_t = JTOOLS::JCollection,
	   class JFunction1D_t                        = JTOOLS::JPolintFunction1D <1,
										   JTOOLS::JElement2D<double, JTOOLS::JArray<NUMBER_OF_OSCCHANNELS, double> >,
										   JCollection_t,
										   JTOOLS::JArray<NUMBER_OF_OSCCHANNELS, double> >,
	   class JFunctionalMaplist_t                 = JTOOLS::JMAPLIST          <JTOOLS::JPolint1FunctionalMap,
										   JTOOLS::JPolint1FunctionalMap,
										   JTOOLS::JPolint1FunctionalMap,
										   JTOOLS::JPolint1FunctionalMap,
										   JTOOLS::JPolint1FunctionalMap,
										   JTOOLS::JPolint1FunctionalMap,
										   JTOOLS::JPolint2FunctionalMap>::maplist >
  class JOscProbInterpolator :
    public JMultiFunction                                                               <JFunction1D_t, JFunctionalMaplist_t>,
    public JClonable<JOscProbInterpolatorInterface, JOscProbInterpolator <JCollection_t, JFunction1D_t, JFunctionalMaplist_t> >,
    public JMessage                                <JOscProbInterpolator <JCollection_t, JFunction1D_t, JFunctionalMaplist_t> >
  {
  public:

    typedef JOscProbInterpolator<JCollection_t, JFunction1D_t, JFunctionalMaplist_t>                interpolator_type;
    typedef JMultiFunction<JFunction1D_t, JFunctionalMaplist_t>                                    multifunction_type;

    enum { NUMBER_OF_DIMENSIONS = multifunction_type::NUMBER_OF_DIMENSIONS };

    typedef typename multifunction_type::abscissa_type                                                  abscissa_type;    
    typedef typename multifunction_type::argument_type                                                  argument_type;
    typedef typename multifunction_type::result_type                                                      result_type;
    typedef typename multifunction_type::value_type                                                        value_type;

    typedef typename multifunction_type::multimap_type                                                  multimap_type;

    typedef typename multifunction_type::super_const_iterator                                    super_const_iterator;
    typedef typename multifunction_type::super_iterator                                                super_iterator;
    typedef typename multifunction_type::function_type                                                  function_type;

    using JMessage<interpolator_type>::debug;

    
    /**
     * Default constructor.
     */
    JOscProbInterpolator() :
      multifunction_type(),
      parameters(),
      getBaseline()
    {
      this->set(JOscParameters(false)); // Initialize buffer with NuFIT NO best fit parameters
    }


    /**
     * Constructor.
     *
     * \param  fileName             oscillation probability table filename
     */
    JOscProbInterpolator(const char* fileName) :
      multifunction_type(),
      parameters(),
      getBaseline()
    {
      this->load(fileName);
      this->set(JOscParameters(false)); // Initialize buffer with NuFIT NO best fit parameters      
    }


    /**
     * Constructor.
     *
     * \param  fileName             oscillation probability table filename
     * \param  parameters           oscillation parameters
     */
    JOscProbInterpolator(const char*           fileName,
			 const JOscParameters& parameters) :
      multifunction_type(),
      parameters(),
      getBaseline()
    {
      this->load(fileName);
      this->set(parameters);
    }


    /**
     * Load oscillation probability table from file.
     *
     * \param  fileName             oscillation probability table filename
     */
    void load(const char* fileName) override
    {
      using namespace std;
      using namespace JPP;
      
      try {

	NOTICE("loading oscillation probability table from file " << fileName << "... " << flush);
	
	JLANG::load<JIO::JFileStreamReader>(fileName, *this);

	NOTICE("OK" << endl);
      }
      catch(const JException& error) {
	THROW(JFileReadException, "JOscProbInterpolator::load(): Error reading file " << fileName);
      }
    }    


    /**
     * Get fixed oscillation parameters associated with this interpolation table.
     *
     * \return                    oscillation parameters
     */
    const JOscParameters& getTableParameters() const override
    {
      return parameters;
    }


    /**
     * Get baseline calculator associated with this interpolation table.
     *
     * \return                    baseline calculator
     */
    const JBaselineCalculator& getBaselineCalculator() const override
    {
      return getBaseline;
    }


    /**
     * Set oscillation parameters for interpolation.
     *
     * \return                      oscillation parameters
     */
    void set(JOscParameters parameters) override
    {
      using namespace JPP;
      
      parameters.join(this->parameters);
      
      const JProperties properties = parameters.getProperties();

      for (JProperties::const_iterator i = properties.cbegin(); i != properties.cend(); ++i) {
	
	const JOscParameters::JParameter_t& parameter = i->second.getValue<JOscParameters::JParameter_t>();
	
	if (parameter.isDefined()) {
	    
	  const int index = std::distance(properties.cbegin(), i);

	  this->buffer[index] = parameter.getValue();
	  
	} else {

	  THROW(JNoValue,
		"JOscProbInterpolator<...>::set(JOscParameters): " <<
		"No value for parameter "                          << i->first);
	}
      }
    }


    /**
     * Get oscillation probability for a given oscillation channel.
     *
     * \param  channel            oscillation channel
     * \param  E                  neutrino energy [GeV]
     * \param  costh              cosine zenith angle
     * \return                    oscillation probability
     */
    double operator()(const JOscChannel&    channel,
		      const double          E,
		      const double          costh) const override
    {
      using namespace std;
      using namespace JPP;

      const JOscChannel* p = find(getOscChannel, getOscChannel + NUMBER_OF_OSCCHANNELS, channel);
	
      if (p != end(getOscChannel)) {
	
	const double L = getBaseline(costh);

	this->buffer[NUMBER_OF_DIMENSIONS-2] = L/E;
	this->buffer[NUMBER_OF_DIMENSIONS-1] = costh;

	const argument_type* arguments = this->buffer.data();
	
	const size_t       index         = distance(getOscChannel, p);
	const result_type& probabilities = this->evaluate(arguments);

	return probabilities[index];
	
      } else {
	
	THROW(JValueOutOfRange, "JOscProbInterpolator<...>::operator(): Invalid oscillation channel " << channel << endl);
      }
    }


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
    double operator()(const JOscParameters& parameters,
		      const JOscChannel&    channel,
		      const double          E,
		      const double          costh) override
    {
      set(parameters);

      return (*this)(channel, E, costh);      
    }
    
    
    /**
     * Read from input.
     *
     * \param  in                   reader
     * \return                      reader
     */
    JReader& read(JReader& in) override 
    {
      parameters.read(in);
      getBaseline.read(in);
      
      in >> static_cast<multifunction_type&>(*this);
      
      this->compile();
      
      return in;
    }
    
    
    /**
     * Write from input.
     *
     * \param  out                  writer
     * \return                      writer
     */
    JWriter& write(JWriter& out) const override 
    {
      parameters.write(out);
      getBaseline.write(out);
      
      out << static_cast<const multifunction_type&>(*this);

      return out;
    }

    
  private:

    JOscParameters      parameters;  //!< Fixed oscillation parameters corresponding to the oscillation probability table
    JBaselineCalculator getBaseline; //!< Baseline functor
  };
}


namespace JEEP {

  /**
   * JMessage template specialization for oscillation probability interpolators.
   */
  template<template<class, class> class JCollection_t, class JFunction1D_t, class JFunctionalMaplist_t>
  struct JMessage<JOSCPROB::JOscProbInterpolator<JCollection_t, JFunction1D_t, JFunctionalMaplist_t> >
  {
    static int debug;
  };


  /**
   * Default verbosity for oscillation probability interpolators.
   */
  template<template<class, class> class JCollection_t, class JFunction1D_t, class JFunctionalMaplist_t>
  int JMessage<JOSCPROB::JOscProbInterpolator<JCollection_t, JFunction1D_t, JFunctionalMaplist_t> >::debug = (int) notice_t;
  
}

#endif
