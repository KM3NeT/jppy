#ifndef __JTOOLS__JTOOLSTOOLKIT__
#define __JTOOLS__JTOOLSTOOLKIT__

#include "JLang/JException.hh"
#include "JLang/JBool.hh"
#include "JMath/JZero.hh"
#include "JTools/JAssembler.hh"
#include "JTools/JElement.hh"
#include "JTools/JCollection.hh"
#include "JTools/JGarbageCollection.hh"
#include "JTools/JMultiFunction.hh"
#include "JTools/JMultiHistogram.hh"
#include "JTools/JFunctionalMap.hh"


/**
 * \file
 *
 * This include file contains various recursive methods to operate on multi-dimensional collections.
 * \author mdejong
 */
namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

/**
 * Auxiliary classes and methods for multi-dimensional interpolations and histograms.
 */
namespace JTOOLS {

  using JLANG::JException;
  using JLANG::JEmptyCollection;
  using JLANG::JDivisionByZero;
  using JLANG::JBool;

  
  /**
   * Conversion of data points to cumulative probability distribition (CDF).
   *
   * Note that the data type of the input container should be preserved 
   * so that the corresponding method <tt>integrate()</tt> is used.
   *
   * \param  input             collection   
   * \param  output            mappable collection
   * \param  eps               minimal step size
   * \return                   integral value
   */
  template<class JContainer_t,
	   class JKey_t,
	   class JValue_t>
  inline typename JContainer_t::ordinate_type
  makeCDF(const JContainer_t&                        input,
	  JMappableCollection<JKey_t, JValue_t>&     output, 
	  const typename JContainer_t::ordinate_type eps = JMATH::zero)
  {
    typedef typename JContainer_t::ordinate_type       ordinate_type;
    typedef typename JContainer_t::abscissa_type       abscissa_type;
    typedef JElement2D<abscissa_type, ordinate_type>   element_type;
    typedef JCollection<element_type>                  buffer_type;


    if (input.getSize() > 1) {
      
      buffer_type buffer;

      const ordinate_type V = integrate(input, buffer);
      
      if (V == ordinate_type(JMATH::zero)) {
	THROW(JDivisionByZero, "Method makeCDF(): integral equals zero.");
      }
      
      output.clear();
      
      typename buffer_type::const_iterator i = buffer.begin();
	    
      for ( ; i != buffer.end() && i->getY() <= 0.5 * eps * V; ++i) {}
      
      if (i != buffer.end()) {
	
	// force first point at (0, ymin)
      
	JKey_t   x = 0.0;
	JValue_t y = i->getX();
	
	output.put(x, y);
      
	JKey_t   xmax = x;
	JValue_t ymax = y;
      
	for (++i; i != buffer.end(); ++i) {
	  
	  x = i->getY() / V;
	  y = i->getX();
	  
	  if (x > xmax) {
	  
	    ymax = y;
	    
	    if (x > xmax + eps) {
	      
	      output.put(x, y);
	      
	      xmax = x;
	    }
	  }
	}
	
	// force last point at (1, ymax)
	
	x = 1.0;
	y = ymax;
	
	output.put(x,y);
	
      } else {
	THROW(JDivisionByZero, "Method makeCDF(): no remaining data.");
      }

      return V;

    } else {
      THROW(JEmptyCollection, "Method makeCDF(): not sufficient input data.");
    }
  }

  
  /**
   * Get integral of input data points.
   *
   * \param  input           input data
   * \return                 integral value
   */
  template<class JContainer_t>
  inline typename JContainer_t::ordinate_type getIntegral(const JContainer_t& input)
  { 
    JGarbageCollection<typename JContainer_t::abscissa_type, typename JContainer_t::ordinate_type> garbage;
    
    return integrate(input, garbage);
  }


  /**
   * Auxiliary method to get integral of input data points.
   *
   * \param  input           input data
   * \return                 integral value
   */
  template<class JFunction_t,
	   template<class, class, class> class JMap_t,
	   class JTail_t>
  inline typename JFunction_t::ordinate_type
  getIntegral(const JMultiMap<typename JFunction_t::abscissa_type, JFunction_t, JMapList<JMap_t, JTail_t>, typename JFunction_t::distance_type>& input)
  { 
    typedef typename JFunction_t::abscissa_type                                               abscissa_type;
    typedef typename JFunction_t::ordinate_type                                               ordinate_type;
    typedef typename JFunction_t::distance_type                                               distance_type;
    typedef typename JFunctionalMap<JMap_t>::template function_type<abscissa_type,
								    ordinate_type,
								    ordinate_type,
								    distance_type>            buffer_type;
    typedef JMultiMap<abscissa_type, JFunction_t, JMapList<JMap_t, JTail_t>, distance_type>   multimap_type;
    
    static buffer_type buffer;
    
    buffer.configure(input);
    
    typename buffer_type::iterator out = buffer.begin();
    
    for (typename multimap_type::const_iterator in = input.begin(); in != input.end(); ++in, ++out) {
      out->getY() = getIntegral(in->getY());
    }
    
    buffer.compile();
    
    return getIntegral(buffer);
  }
  

  /**
   * Get integral of input data points.
   *
   * \param  input           input data
   * \return                 integral value
   */
  template<class JFunction_t,
	   template<class, class, class> class JMap_t,
	   class JTail_t>
  inline typename JFunction_t::ordinate_type
  getIntegral(const JMultiFunction<JFunction_t, JMapList<JMap_t, JTail_t>, typename JFunction_t::distance_type>& input)
  { 
    typedef typename JFunction_t::abscissa_type                                      abscissa_type;
    typedef typename JFunction_t::ordinate_type                                      ordinate_type;
    typedef typename JFunction_t::distance_type                                      distance_type;
    typedef typename JFunctionalMap<JMap_t>::template function_type<abscissa_type,
								    ordinate_type,
								    ordinate_type,
								    distance_type>   buffer_type;
    typedef JMultiFunction<JFunction_t, JMapList<JMap_t, JTail_t>, distance_type>    multifunction_type;
    
    buffer_type buffer;
    
    for (typename multifunction_type::const_iterator i = input.begin(); i != input.end(); ++i) {      
      buffer.put(i->getX(), getIntegral(i->getY()));
    }
    
    buffer.compile();
    
    return getIntegral(buffer);
  }


  /**
   * Reset value.
   *
   * The value is set to (the equivalent of) zero, see method JMATH::getZero.
   *
   * \param  value           value
   */
  template<class T>
  inline void reset(T& value)
  {
    value = JMATH::getZero<T>();
  }
  

  /**
   * Recursive reset of collection.
   *
   * \param  collection      collection
   */
  template<class JElement_t, class JDistance_t>
  inline void reset(JCollection<JElement_t, JDistance_t>& collection)
  {
    typedef typename JCollection<JElement_t, JDistance_t>::iterator  iterator;

    for (iterator i = collection.begin(); i != collection.end(); ++i) {
      reset(i->getY());
    }
  }

  
  /**
   * Copy of input to output.
   *
   * The output value is set to the input value.
   *
   * \param  input           input
   * \param  output          output
   */
  template<class T>
  inline void copy(const T& input, T& output)
  {
    output = input;
  }


  /**
   * Recursive copy of input collection to output collection.
   *
   * \param  input           input
   * \param  output          output
   */
  template<class JElement_t, class JDistance_t, class JKey_t, class JValue_t>
  inline void copy(const JCollection<JElement_t, JDistance_t>& input, JMappableCollection<JKey_t, JValue_t>& output)
  {
    typedef typename JCollection<JElement_t, JDistance_t>::const_iterator  const_iterator;

    output.clear();

    for (const_iterator i = input.begin(); i != input.end(); ++i) {
      copy(i->getY(), output.get(i->getX()));
    }
  }


  /**
   * Configuration of value.
   *
   * This is a fall-back method; it does nothing.
   *
   * \param  value           value
   * \param  bounds          bounds
   * \param  option          false
   */
  template<class T, class JAbscissa_t>
  inline void configure(const T& value, const JAbstractCollection<JAbscissa_t>& bounds, JBool<false> option)
  {}


  /**
   * Recursive configuration of collection.
   *
   * \param  collection      collection
   * \param  bounds          bounds
   * \param  option          true
   */
  template<class JElement_t, class JDistance_t>
  inline void configure(JCollection<JElement_t, JDistance_t>&                          collection,
			const JAbstractCollection<typename JElement_t::abscissa_type>& bounds,
			JBool<true>                                                    option = JBool<true>())
  {
    typedef typename JCollection<JElement_t, JDistance_t>::iterator       iterator;
    typedef typename JCollection<JElement_t, JDistance_t>::ordinate_type  ordinate_type;

    collection.configure(bounds);

    for (iterator i = collection.begin(); i != collection.end(); ++i) {
      configure(i->getY(), bounds, JBool<JAssembler<ordinate_type>::is_collection>());
    }
  }


  /**
   * Accumulation of value.
   *
   * This is a fall-back method; it does nothing.
   *
   * \param  value           value
   * \param  option          false
   */
  template<class T>
  inline void accumulate(T& value, JBool<false> option)
  {}


  /**
   * Recursive accumulation of collection.
   *
   * \param  collection      collection
   * \param  option          true
   */
  template<class JElement_t, class JDistance_t>
  inline void accumulate(JCollection<JElement_t, JDistance_t>& collection, JBool<true> option = JBool<true>())
  {
    typedef typename JCollection<JElement_t, JDistance_t>::iterator       iterator;
    typedef typename JCollection<JElement_t, JDistance_t>::ordinate_type  ordinate_type;

    for (iterator i = collection.begin(); i != collection.end(); ++i) {
      accumulate(i->getY(), JBool<JAssembler<ordinate_type>::is_collection>());
    }

    if (collection.getSize() > 1) {

      for (iterator j = collection.begin(), i = j++; j != collection.end(); ++i, ++j) {
	j->getY() += i->getY();
      }

      reset(collection.begin()->getY());
    }
  }
}

#endif
