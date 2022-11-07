#ifndef __JPHYSICS__JPDFTABLE__
#define __JPHYSICS__JPDFTABLE__

#include <cmath>

#include "JIO/JObjectBinaryIO.hh"
#include "JTools/JTransformableMultiFunction.hh"
#include "JTools/JQuantiles.hh"
#include "JTools/JSet.hh"
#include "JTools/JRange.hh"
#include "JMath/JMathSupportkit.hh"
#include "JPhysics/JConstants.hh"
#include "JPhysics/JPDFTransformer.hh"


/**
 * \author mdejong
 */

namespace JPHYSICS {}
namespace JPP { using namespace JPHYSICS; }

namespace JPHYSICS {

  using JIO::JSerialisable;
  using JIO::JReader;
  using JIO::JWriter;
  using JIO::JObjectBinaryIO;
  using JTOOLS::JTransformableMultiFunction;
  using JTOOLS::JTransformableMultiHistogram;
  using JTOOLS::JRange;
  

  /**
   * Multi-dimensional PDF table for arrival time of Cherenkov light.
   */
  template<class JFunction1D_t,
	   class JMaplist_t,
           class JDistance_t = JTOOLS::JDistance<typename JFunction1D_t::argument_type> >
  class JPDFTable :
    public JTransformableMultiFunction<JFunction1D_t, JMaplist_t, JDistance_t>,
    public JSerialisable,
    public JObjectBinaryIO< JPDFTable<JFunction1D_t, JMaplist_t, JDistance_t> >
  {
  public:

    typedef JTransformableMultiFunction<JFunction1D_t, JMaplist_t, JDistance_t>      transformablemultifunction_type;

    typedef typename transformablemultifunction_type::argument_type                  argument_type;
    typedef typename transformablemultifunction_type::result_type                    result_type;
    typedef typename transformablemultifunction_type::value_type                     value_type;

    typedef typename transformablemultifunction_type::multimap_type                  multimap_type;
    typedef typename transformablemultifunction_type::transformer_type               transformer_type;

    enum { NUMBER_OF_DIMENSIONS = transformablemultifunction_type::NUMBER_OF_DIMENSIONS };

    typedef typename transformablemultifunction_type::super_const_iterator           super_const_iterator;
    typedef typename transformablemultifunction_type::super_iterator                 super_iterator;
    typedef typename transformablemultifunction_type::function_type                  function_type;

    using transformablemultifunction_type::insert;


    /**
     * Default constructor.
     */
    JPDFTable() :
      transformablemultifunction_type()
    {}


    /**
     * Constructor.
     *
     * \param  input                multi-dimensional function
     */
    template<class __JFunction_t, class __JMaplist_t, class __JDistance_t>    
    JPDFTable(const JTransformableMultiFunction<__JFunction_t, __JMaplist_t, __JDistance_t>& input) :
      transformablemultifunction_type(input)
    {}

    
    /**
     * Constructor.
     *
     * \param  input                multi-dimensional histogram
     */
    template<class JHistogram_t, class __JMaplist_t, class __JDistance_t>
    JPDFTable(const JTransformableMultiHistogram<JHistogram_t, __JMaplist_t, __JDistance_t>& input) :
      transformablemultifunction_type(input)
    {}


    /**
     * Blur PDF.
     *
     * The arrival times of Cherenkov light are smeared according to a Gaussian distribution
     * with the specified width (i.e.\ TTS) using Gauss-Hermite integration.
     * An exception is made when the time range according the specified quantile is 
     * smaller than the specified width (TTS) of the Gaussian distribution.
     * In that case, the resulting PDF is a Gaussian distribution with the specified width (TTS)
     * and normalisation according to the integral value of the input PDF.
     * A smooth transition is imposed between the normal regime and this exeption.
     *
     * \param  TTS                  TTS [ns]
     * \param  numberOfPoints       number of points for Gauss-Hermite integration
     * \param  epsilon              precision
     * \param  quantile             quantile
     */
    void blur(const double TTS, 
	      const int    numberOfPoints = 25,
	      const double epsilon        = 1.0e-10,
	      const double quantile       = 0.99)
    {
      using namespace std;
      using namespace JPP;

      typedef typename transformer_type::array_type  array_type;

      const JGaussHermite engine(numberOfPoints, epsilon);

      for (super_iterator i = this->super_begin(); i != this->super_end(); ++i) {
	
	const array_type array = (*i).getKey();
	function_type&   f1    = (*i).getValue();

	if (!f1.empty()) {

	  const typename function_type::supervisor_type& supervisor = f1.getSupervisor();

	  const JMultiMapGetTransformer<NUMBER_OF_DIMENSIONS - 1, value_type> get(*(this->transformer), array);
	  const JMultiMapPutTransformer<NUMBER_OF_DIMENSIONS - 1, value_type> put(*(this->transformer), array);

	  f1.transform(get);
	  f1.compile();

	  const JQuantiles Q(f1, quantile);

	  // abscissa 

	  JSet<double> X;

	  for (JGaussHermite::const_iterator j = engine.begin(); j != engine.end(); ++j) {
	    X.insert(Q.getX() + TTS*sqrt(2.0)*j->getX());
	  }

	  for (typename function_type::const_iterator j = f1.begin(); j != f1.end(); ++j) {

	    if (j->getX() - TTS < X.getXmin()) {
	      X.insert(j->getX() - TTS);
	    }
	    
	    if (j->getX() + TTS > X.getXmax()) {
	      X.insert(j->getX() + TTS);
	    }
	  }


	  const double W = gauss(Q.getUpperLimit() - Q.getLowerLimit(), TTS);

	  function_type buffer;

	  for (JSet<double>::const_iterator x = X.begin(); x != X.end(); ++x) {

	    double y = 0.0;

	    for (JGaussHermite::const_iterator j = engine.begin(); j != engine.end(); ++j) {

	      const double u = j->getX();
	      const double v = j->getY() / sqrt(PI);
	      const double w = get_value(f1(*x + u*TTS*sqrt(2.0)));
	
	      y += v * w;
	    }

	    buffer[*x]  =  W * Q.getIntegral() * Gauss(*x - Q.getX(), TTS)  +  (1.0 - W) * y;
	  }

	  buffer.transform(put);
	  buffer.compile();

	  f1 = buffer;

	  f1.setExceptionHandler(supervisor);
	}
      }
    }


    /**
     * Compresses PDF to given abscissa range.
     *
     * \param   range               abscissa range
     */
    void compress(const JRange<typename function_type::abscissa_type>& range)
    {        
      for (super_iterator i = this->super_begin(); i != this->super_end(); ++i) {

	function_type& f1 = i.getValue();
	
	typename function_type::iterator p = f1.lower_bound(range.getLowerLimit());

	f1.function_type::container_type::erase(f1.begin(), p);

	typename function_type::iterator q = f1.lower_bound(range.getUpperLimit());

	f1.function_type::container_type::erase(++q, f1.end());
      }

      this->compile();
    }


    /**
     * Read from input.
     *
     * \param  in                   reader
     * \return                      reader
     */
    virtual JReader& read(JReader& in) override 
    {
      if (in >> static_cast<transformablemultifunction_type&>(*this)) {

	// read optional transformer

	JPDFTransformer<NUMBER_OF_DIMENSIONS - 1, argument_type> buffer;

	if (buffer.read(in)) {

	  this->transformer.reset(buffer.clone());

	} else {

	  in.clear();
	  
	  this->transformer.reset(transformer_type::getClone());
	}
      }
      
      this->compile();
      
      return in;
    }
    
    
    /**
     * Write from input.
     *
     * \param  out                  writer
     * \return                      writer
     */
    virtual JWriter& write(JWriter& out) const override 
    {
      out << static_cast<const transformablemultifunction_type&>(*this);

      this->transformer->write(out);

      return out;
    }
  };
}

#endif
