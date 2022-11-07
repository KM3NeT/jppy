#ifndef __JOSCPROB__JPPYOSCPROBINTERPOLATOR__
#define __JOSCPROB__JPPYOSCPROBINTERPOLATOR__


#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "JLang/JException.hh"

#include "JTools/JPolint.hh"
#include "JTools/JMapList.hh"
#include "JTools/JCollection.hh"
#include "JTools/JFunction1D_t.hh"
#include "JTools/JMultiFunction.hh"
#include "JTools/JFunctionalMap_t.hh"

#include "JOscProb/JOscChannel.hh"
#include "JOscProb/JOscParameters.hh"
#include "JOscProb/JOscProbInterpolator.hh"


/**
 * \author bjung, jbootsma, mdejong
 */

namespace JOSCPROB {};
namespace JPP { using namespace JOSCPROB; }

namespace JOSCPROB {

  namespace py = pybind11;

  using JLANG::JValueOutOfRange;
  

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
  struct JppyOscProbInterpolator :
    public JOscProbInterpolator<JCollection_t, JFunction1D_t, JFunctionalMaplist_t>
  {
    typedef JOscProbInterpolator<JCollection_t, JFunction1D_t, JFunctionalMaplist_t>                interpolator_type;

    typedef typename interpolator_type::multifunction_type                                         multifunction_type;

    enum { NUMBER_OF_DIMENSIONS = multifunction_type::NUMBER_OF_DIMENSIONS };

    typedef typename multifunction_type::abscissa_type                                                  abscissa_type;    
    typedef typename multifunction_type::argument_type                                                  argument_type;
    typedef typename multifunction_type::result_type                                                      result_type;
    typedef typename multifunction_type::value_type                                                        value_type;

    typedef typename multifunction_type::multimap_type                                                  multimap_type;

    typedef typename multifunction_type::super_const_iterator                                    super_const_iterator;
    typedef typename multifunction_type::super_iterator                                                super_iterator;
    typedef typename multifunction_type::function_type                                                  function_type;

    
    /**
     * Default constructor.
     */
    JppyOscProbInterpolator() :
      interpolator_type()
    {}


    /**
     * Constructor.
     *
     * \param  fileName           oscillation probability table filename
     */
    JppyOscProbInterpolator(const char* fileName) :
      interpolator_type(fileName)
    {}


    /**
     * Constructor.
     *
     * \param  fileName           oscillation probability table filename
     */
    JppyOscProbInterpolator(const char*           fileName,
			    const JOscParameters& parameters) :
      interpolator_type(fileName, parameters)
    {}


    /**
     * Get oscillation probability for given oscillation channel.
     *
     * \param  channel            oscillation channel
     * \param  E                  neutrino energies [GeV]
     * \param  costh              cosine zenith angles
     * \return                    oscillation probability
     */
    py::array_t<double> operator()(const JOscChannel&         channel,
				   const py::array_t<double>& E,
				   const py::array_t<double>& costh) const {

      using JLANG::JValueOutOfRange;

      py::buffer_info buffer1 = E.request();
      py::buffer_info buffer2 = costh.request();

      if (buffer1.ndim != 1 || buffer2.ndim != 1) {
	THROW(JValueOutOfRange, "JppyOscProbInterpolator<>::operator(): Input arrays are not one-dimensional.");
      }
 
      if (buffer1.shape != buffer2.shape) {
	THROW(JValueOutOfRange, "JppyOscProbInterpolator<>::operator(): Different input array shapes (" << buffer1.size << ", " << buffer2.size << ")");
      }

      py::array_t<double> result(buffer1.size);
      
      py::buffer_info buffer3 = result.request();

      double* ptr1 = static_cast<double*>(buffer1.ptr);
      double* ptr2 = static_cast<double*>(buffer2.ptr);
      double* ptr3 = static_cast<double*>(buffer3.ptr);      

      for (unsigned int i = 0; i < buffer1.size; ++i) {
	ptr3[i] = static_cast<const interpolator_type&>(*this)(channel, ptr1[i], ptr2[i]);
      }

      return result;
    }


    /**
     * Get oscillation probability for a given set of oscillation parameters\n
     * and a given oscillation channel.
     *
     * \param  parameters         oscillation parameters
     * \param  channel            oscillation channel
     * \param  E                  neutrino energies [GeV]
     * \param  costh              cosine zenith angles
     * \return                    oscillation probability
     */
    py::array_t<double> operator()(const JOscParameters&      parameters,
				   const JOscChannel&         channel,
				   const py::array_t<double>& E,
				   const py::array_t<double>& costh)
    {
      this->set(parameters);
      
      return (*this)(channel, E, costh);
    }
  };
}

#endif
