#ifndef __JTOOLS__JSPLINE__
#define __JTOOLS__JSPLINE__

#include <utility>

#include "JMath/JZero.hh"
#include "JLang/JException.hh"
#include "JLang/JClass.hh"
#include "JLang/JStreamAvailable.hh"
#include "JTools/JFunctional.hh"
#include "JTools/JDistance.hh"
#include "JTools/JResult.hh"
#include "JTools/JMapCollection.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JLANG::JNoValue;
  using JLANG::JDivisionByZero;
  using JLANG::JFunctionalException;
  using JLANG::JValueOutOfRange;


  /**
   * Auxiliary class to define first derivates of the spline function at the two extrema.
   */
  template<class JOrdinate_t>
  class JSplineBounds {
  public:

    typedef JOrdinate_t                                                          ordinate_type;
    typedef typename JLANG::JClass<ordinate_type>::argument_type                 argument_type;


    /** 
     * Default constructor.
     */
    JSplineBounds() :
      fp_at_xmin(false, ordinate_type()),
      fp_at_xmax(false, ordinate_type())
    {}


    /** 
     * Constructor.
     *
     * \param fpAtXmin          1st derivative at minimal abscissa value
     * \param fpAtXmax          1st derivative at maximal abscissa value
     */
    JSplineBounds(argument_type fpAtXmin,
		  argument_type fpAtXmax) :
      fp_at_xmin(true, fpAtXmin),
      fp_at_xmax(true, fpAtXmax)
    {}


    /**
     * Set first derivative of function at minimal abscissa value.
     * 
     * \param fp                1st derivative
     */
    void setFirstDerivativeAtXmin(argument_type fp)
    {
      fp_at_xmin.first  = true;
      fp_at_xmin.second = fp;
    }


    /**
     * Set first derivative of function at maximal abscissa value.
     * 
     * \param fp                1st derivative
     */
    void setFirstDerivativeAtXmax(argument_type fp)
    {
      fp_at_xmax.first  = true;
      fp_at_xmax.second = fp;
    }


    /**
     * Has first derivative of function at minimal abscissa value.
     * 
     * \return                  true if 1st derivative is set; else false
     */
    const bool& hasFirstDerivativeAtXmin() const 
    { 
      return fp_at_xmin.first;
    }


    /**
     * Has first derivative of function at maximal abscissa value.
     * 
     * \return                  true if 1st derivative is set; else false
     */
    const bool& hasFirstDerivativeAtXmax() const 
    { 
      return fp_at_xmax.first;
    }


    /**
     * Get first derivative of function at minimal abscissa value.
     * 
     * \return                  1st derivative
     */
    ordinate_type getFirstDerivativeAtXmin() const 
    { 
      if (fp_at_xmin.first)
	return fp_at_xmin.second;
      else
	throw JNoValue("JSplineBounds: missing 1st derivative.");
    }


    /**
     * Get first derivative of function at maximal abscissa value.
     * 
     * \return                  1st derivative
     */
    ordinate_type getFirstDerivativeAtXmax() const 
    { 
      if (fp_at_xmax.first)
	return fp_at_xmax.second;
      else
	throw JNoValue("JSplineBounds: missing 1st derivative.");
    }

  protected:
    std::pair<bool, ordinate_type> fp_at_xmin;
    std::pair<bool, ordinate_type> fp_at_xmax;
  };


  /**
   * Helper method for JSplineBounds.
   *
   * \param fpAtXmin          1st derivative at minimal abscissa value
   * \param fpAtXmax          1st derivative at maximal abscissa value
   * \return                  spline bounds
   */
  template<class JOrdinate_t>
  inline JSplineBounds<JOrdinate_t> make_spline_bounds(const JOrdinate_t fpAtXmin,
						       const JOrdinate_t fpAtXmax)
  {
    return JSplineBounds<JOrdinate_t>(fpAtXmin, fpAtXmax);
  }


  /**
   * Template base class for spline interpolations.
   *
   * This class partially implements the JFunctional interface.
   *
   * Note that the data structure of the elements in the collection should have the additional methods:
   * <pre>
   *     ordinate_type getU() const;
   *     void setU(ordinate_type u);
   * </pre>
   * to get and set the second derivatives, respectively.
   *
   * Spline interpolation code is taken from reference:
   * Numerical Recipes in C++, W.H. Press, S.A. Teukolsky, W.T. Vetterling and B.P. Flannery,
   * Cambridge University Press.
   */
  template<class JElement_t, template<class, class> class JCollection_t, class JDistance_t>
  class JSplineCollection :
    public JCollection_t<JElement_t, JDistance_t>,
    public virtual JFunctional<>
  {
  public:

    typedef JCollection_t<JElement_t, JDistance_t>                               collection_type;

    typedef typename collection_type::abscissa_type                              abscissa_type;
    typedef typename collection_type::ordinate_type                              ordinate_type;
    typedef typename collection_type::value_type                                 value_type;

    typedef typename collection_type::const_iterator                             const_iterator;
    typedef typename collection_type::const_reverse_iterator                     const_reverse_iterator;
    typedef typename collection_type::iterator                                   iterator;
    typedef typename collection_type::reverse_iterator                           reverse_iterator;
 
    using JFunctional<>::compile;


    /**
     * Determination of second derivatives with specified bounds.
     *
     * \param bounds            1st derivatives at two extrema.
     */
    void compile(const JSplineBounds<ordinate_type>& bounds) 
    {
      const int numberOfElements = this->size();

      using namespace std;

      if (numberOfElements > 2) {
	
	std::vector<double> buffer(numberOfElements);

	if (bounds.hasFirstDerivativeAtXmin()) {
	  
	  iterator j = this->begin();
	  iterator i = j++;
	   
	  const double        dx = this->getDistance(i->getX(),  j->getX());
	  const ordinate_type dy =                  (j->getY() - i->getY());
	   
	  buffer[0] = -0.5;

	  i->setU((3.0/dx) * (dy/dx - bounds.getFirstDerivativeAtXmin()));

	} else {

	  buffer[0] = 0.0;

	  this->begin()->setU(JMATH::zero);
	}

	int index = 1;
       
	for (iterator k = this->begin(), i = k++, j = k++; k != this->end(); ++i, ++j, ++k, ++index) {

	  const abscissa_type  x1 = i->getX();
	  const abscissa_type  x2 = j->getX();
	  const abscissa_type  x3 = k->getX();

	  const ordinate_type& y1 = i->getY();
	  const ordinate_type& y2 = j->getY();
	  const ordinate_type& y3 = k->getY();

	  const double sig = this->getDistance(x1, x2) / this->getDistance(x1, x3);
	  const double h   = sig * buffer[index-1] + 2.0;
	 
	  buffer[index]    = (sig - 1.0) / h;
	 
	  j->setU((y3 - y2) / this->getDistance(x2, x3)  -  
		  (y2 - y1) / this->getDistance(x1, x2));

	  j->setU((6.0 * j->getU() / this->getDistance(x1, x3)  -  sig * i->getU()) / h);
	}
       

	if (bounds.hasFirstDerivativeAtXmax()) {

	  reverse_iterator j = this->rbegin();
	  reverse_iterator i = j++;

	  index = numberOfElements - 2;
	  
	  const double        dx = this->getDistance(i->getX(),  j->getX());
	  const ordinate_type dy =                  (j->getY() - i->getY());
	   
	  i->setU((3.0/dx) * (bounds.getFirstDerivativeAtXmax() - dy/dx));

	  i->setU((i->getU() - 0.5*j->getU()) / (0.5*buffer[index] + 1.0));

	} else {

	  this->rbegin()->setU(JMATH::zero);
	}


	reverse_iterator j = this->rbegin();
	reverse_iterator i = j++;
	
	index = numberOfElements - 2;
	
	for ( ; j != this->rend(); ++i, ++j, --index) {
	  j->setU(j->getU() + i->getU() * buffer[index]);
	}

      } else {

	for (iterator i = this->begin(); i != this->end(); ++i) {
	  i->setU(JMATH::zero);
	}
      }
    }


  protected:
    /**
     * Default constructor.
     */
    JSplineCollection()
    {}


    /**
     * Determination of second derivatives with no bounds.
     */
    virtual void do_compile() override 
    {
      compile(JSplineBounds<ordinate_type>());
    }
  };


  /**
   * Template definition for functional collection with spline interpolation.
   */
  template<class JElement_t, 
	   template<class, class> class JCollection_t,
	   class JResult_t,
           class JDistance_t>
  class JSplineFunction;


  /**
   * Template specialisation for functional collection with spline interpolation.
   */
  template<class JElement_t, template<class, class> class JCollection_t, class JDistance_t>
  class JSplineFunction<JElement_t, 
			JCollection_t, 
			typename JResultType<typename JElement_t::ordinate_type>::result_type,
			JDistance_t> :
    public JSplineCollection<JElement_t, JCollection_t, JDistance_t>,
    public JFunction<typename JElement_t::abscissa_type, 
		     typename JResultType<typename JElement_t::ordinate_type>::result_type>
  {
  public:

    typedef JSplineCollection<JElement_t, JCollection_t, JDistance_t>            collection_type;

    typedef typename collection_type::abscissa_type                              abscissa_type;
    typedef typename collection_type::ordinate_type                              ordinate_type;
    typedef typename collection_type::value_type                                 value_type;
    typedef typename collection_type::distance_type                              distance_type;

    typedef typename collection_type::const_iterator                             const_iterator;
    typedef typename collection_type::const_reverse_iterator                     const_reverse_iterator;
    typedef typename collection_type::iterator                                   iterator;
    typedef typename collection_type::reverse_iterator                           reverse_iterator;

    typedef typename JResultType<ordinate_type>::result_type                     data_type;
    typedef JFunction<abscissa_type, data_type>                                  function_type;  

    typedef typename function_type::argument_type                                argument_type;
    typedef typename function_type::result_type                                  result_type;
    typedef typename function_type::JExceptionHandler                            exceptionhandler_type;


    /**
     * Default constructor.			
     */
    JSplineFunction()
    {}


    /**
     * Recursive interpolation method implementation.
     *
     * \param  pX              pointer to abscissa values
     * \return                 function value
     */
    virtual result_type evaluate(const argument_type* pX) const override 
    {
      const argument_type x = *pX;

      if (this->size() > 1u) {

	const_iterator p = this->lower_bound(x);

	if ((p == this->begin() && this->getDistance(x, (p++)->getX()) > distance_type::precision) ||
	    (p == this->end()   && this->getDistance((--p)->getX(), x) > distance_type::precision)) {

	  return this->getExceptionHandler().action(MAKE_EXCEPTION(JValueOutOfRange, "abscissa out of range " 
								   << STREAM("?") << x                      << " <> " 
								   << STREAM("?") << this->begin() ->getX() << ' '
								   << STREAM("?") << this->rbegin()->getX()));
	}

	const_iterator q = p--;

	const double dx = this->getDistance(p->getX(), q->getX());
	const double a  = this->getDistance(x, q->getX()) / dx;
	const double b  = 1.0 - a;

	return (a * p->getY() + b * q->getY()
		- a*b * ((a + 1.0)*p->getU() + (b + 1.0)*q->getU()) * dx*dx/6);
	
      } else if (this->size() == 1u && this->getDistance(x, this->begin()->getX()) <= distance_type::precision) {

	return this->begin()->getY();
	
      } else {

        return this->getExceptionHandler().action(MAKE_EXCEPTION(JFunctionalException, "not enough data " << STREAM("?") << x));
      }
    }
  };


  /**
   * Template specialisation for spline interpolation method with returning JResultDerivative data structure.
   */
  template<class JElement_t, template<class, class> class JCollection_t, class JDistance_t>
  class JSplineFunction<JElement_t,
			JCollection_t,
			JResultDerivative<typename JResultType<typename JElement_t::ordinate_type>::result_type>,
			JDistance_t> :
    public JSplineCollection<JElement_t, JCollection_t, JDistance_t>,
    public JFunction<typename JElement_t::abscissa_type, 
		     JResultDerivative<typename JResultType<typename JElement_t::ordinate_type>::result_type> >
  {
  public:

    typedef JSplineCollection<JElement_t, JCollection_t, JDistance_t>            collection_type;

    typedef typename collection_type::abscissa_type                              abscissa_type;
    typedef typename collection_type::ordinate_type                              ordinate_type;
    typedef typename collection_type::value_type                                 value_type;
    typedef typename collection_type::distance_type                              distance_type;

    typedef typename collection_type::const_iterator                             const_iterator;
    typedef typename collection_type::const_reverse_iterator                     const_reverse_iterator;
    typedef typename collection_type::iterator                                   iterator;
    typedef typename collection_type::reverse_iterator                           reverse_iterator;

    typedef typename JResultType<ordinate_type>::result_type                     data_type;
    typedef JFunction<abscissa_type, JResultDerivative<data_type> >              function_type;

    typedef typename function_type::argument_type                                argument_type;
    typedef typename function_type::result_type                                  result_type;
    typedef typename function_type::JExceptionHandler                            exceptionhandler_type;
 
    using JFunctional<>::compile;


    /**
     * Default constructor.
     */
    JSplineFunction() 
    {}


    /**
     * Recursive interpolation method implementation.
     *
     * \param  pX              pointer to abscissa values
     * \return                 function value
     */
    virtual result_type evaluate(const argument_type* pX) const override 
    {
      const argument_type x = *pX;

      if (this->size() <= 1u) {
        return this->getExceptionHandler().action(MAKE_EXCEPTION(JFunctionalException, "not enough data " << STREAM("?") << x));
      }

      const_iterator p = this->lower_bound(x);


      if ((p == this->begin() && this->getDistance(x, (p++)->getX()) > distance_type::precision) ||
          (p == this->end()   && this->getDistance((--p)->getX(), x) > distance_type::precision)) {

        return this->getExceptionHandler().action(MAKE_EXCEPTION(JValueOutOfRange, "abscissa out of range " 
								 << STREAM("?") << x                      << " <> " 
								 << STREAM("?") << this->begin() ->getX() << ' '
								 << STREAM("?") << this->rbegin()->getX()));
      }

      const_iterator q = p--;

      const double dx = this->getDistance(p->getX(), q->getX());
      const double a  = this->getDistance(x, q->getX()) / dx;
      const double b  = 1.0 - a;

      result.f   = a * p->getY() + b * q->getY()
	- a*b * ((a + 1.0)*p->getU() + (b + 1.0)*q->getU()) * dx*dx/6;

      result.fp  = (q->getY() - p->getY() + (p->getU()*(1.0 - 3*a*a) -
					     q->getU()*(1.0 - 3*b*b)) * dx*dx/6) / dx;

      return result;
    }


  private:
    mutable result_type result;
  };


  /**
   * Template specialisation for spline interpolation method with returning JResultPDF data structure.
   *
   * Note that the data structure of the elements in the collection should have the additional methods:
   * <pre>
   *     ordinate_type getIntegral() const;
   *     void setIntegral(ordinate_type v);
   * </pre>
   * to get and set the integral values, respectively.
   */
  template<class JElement_t, template<class, class> class JCollection_t, class JDistance_t>
  class JSplineFunction<JElement_t,
			JCollection_t,
			JResultPDF<typename JResultType<typename JElement_t::ordinate_type>::result_type>,
			JDistance_t> :
    public JSplineCollection<JElement_t, JCollection_t, JDistance_t>,
    public JFunction<typename JElement_t::abscissa_type, 
		     JResultPDF<typename JResultType<typename JElement_t::ordinate_type>::result_type> >
  {
  public:

    typedef JSplineCollection<JElement_t, JCollection_t, JDistance_t>            collection_type;

    typedef typename collection_type::abscissa_type                              abscissa_type;
    typedef typename collection_type::ordinate_type                              ordinate_type;
    typedef typename collection_type::value_type                                 value_type;
    typedef typename collection_type::distance_type                              distance_type;

    typedef typename collection_type::const_iterator                             const_iterator;
    typedef typename collection_type::const_reverse_iterator                     const_reverse_iterator;
    typedef typename collection_type::iterator                                   iterator;
    typedef typename collection_type::reverse_iterator                           reverse_iterator;

    typedef typename JResultType<ordinate_type>::result_type                     data_type;
    typedef JFunction<abscissa_type, JResultPDF<data_type> >                     function_type;

    typedef typename function_type::argument_type                                argument_type;
    typedef typename function_type::result_type                                  result_type;
    typedef typename function_type::JExceptionHandler                            exceptionhandler_type;
 
    using JFunctional<>::compile;


    /**
     * Default constructor.			
     */
    JSplineFunction() 
    {}


    /**
     * Determination of second derivatives with specified bounds.
     *
     * \param bounds            1st derivatives at two extrema.
     */
    void compile(const JSplineBounds<ordinate_type>& bounds) 
    {
      if (this->size() >= 2u) {

	collection_type::compile(bounds);
 
	this->begin()->setIntegral(JMATH::zero);

	for (iterator j = this->begin(), i = j++; j != this->end(); ++i, ++j) {
	  
	  const double        dx = this->getDistance(i->getX(), j->getX());
	  const ordinate_type y  = i->getY() + j->getY();
	  const ordinate_type z  = i->getU() + j->getU();
	
	  const ordinate_type v = dx * 0.50 * y;
	  const ordinate_type w = dx * 0.25 * z*dx*dx/6;

	  j->setIntegral(i->getIntegral() + v - w);
	}
      }
    }


    /**
     * Recursive interpolation method implementation.
     *
     * \param  pX              pointer to abscissa values
     * \return                 function value
     */
    virtual result_type evaluate(const argument_type* pX) const override 
    {
      const argument_type x = *pX;

      if (this->size() <= 1u) {
        return this->getExceptionHandler().action(MAKE_EXCEPTION(JFunctionalException, "not enough data " << STREAM("?") << x));
      }

      const_iterator p = this->lower_bound(x);

      if        (p == this->begin() && this->getDistance(x, (p++)->getX()) > distance_type::precision) {

	try {

          result   = this->getExceptionHandler().action(MAKE_EXCEPTION(JValueOutOfRange, "abscissa out of range " 
								       << STREAM("?") << x << " < " << STREAM("?") << this->begin() ->getX()));

	  // overwrite integral values

	  result.v = 0;
	  result.V = this->rbegin()->getIntegral();

	} catch(const JValueOutOfRange& exception) {
	  throw exception;
	}

	return result;

      } else if (p == this->end()   && this->getDistance((--p)->getX(), x) > distance_type::precision) {

	try {

          result   = this->getExceptionHandler().action(MAKE_EXCEPTION(JValueOutOfRange, "abscissa out of range " 
								       << STREAM("?") << x << " > " << STREAM("?") << this->rbegin() ->getX()));

	  // overwrite integral values

	  result.v = this->rbegin()->getIntegral();
	  result.V = this->rbegin()->getIntegral();

	} catch(const JValueOutOfRange& exception) {
	  throw exception;
	}

	return result;
      }

      const_iterator q = p--;

      const double dx = this->getDistance(p->getX(), q->getX());
      const double a  = this->getDistance(x, q->getX()) / dx;
      const double b  = 1.0 - a;

      result.f  = a * p->getY() + b * q->getY()
	- a*b * ((a + 1.0)*p->getU() + (b + 1.0)*q->getU()) * dx*dx/6;

      result.fp = (q->getY() - p->getY() + (p->getU()*(1.0 - 3*a*a) -
					    q->getU()*(1.0 - 3*b*b)) * dx*dx/6) / dx;
      
      result.v  = p->getIntegral()
	+ 0.5*dx * (p->getY() - 0.5*p->getU()*dx*dx/6)
	- 0.5*dx * ((a*a*p->getY() - b*b*q->getY()) +
		    (p->getU() * a*a*(0.5*a*a - 1.0) -
		     q->getU() * b*b*(0.5*b*b - 1.0)) * dx*dx/6);
      
      result.V  = this->rbegin()->getIntegral();

      return result;
    }


  protected:
    /**
     * Determination of second derivatives with no bounds.
     */
    virtual void do_compile() override 
    {
      compile(JSplineBounds<ordinate_type>());
    }


  private:
    mutable result_type result;
  };


  /**
   * Template class for spline interpolation in 1D
   *
   * This class implements the JFunction1D interface.
   */
  template<class JElement_t,
	   template<class, class> class JCollection_t,
	   class JResult_t   = typename JElement_t::ordinate_type,
	   class JDistance_t = JDistance<typename JElement_t::abscissa_type> >
  class JSplineFunction1D :
    public JSplineFunction<JElement_t, JCollection_t, JResult_t, JDistance_t>,
    public JFunction1D<typename JElement_t::abscissa_type, JResult_t> 
  {
  public:

    typedef JSplineCollection<JElement_t, JCollection_t, JDistance_t>            collection_type;

    typedef typename collection_type::abscissa_type                              abscissa_type;
    typedef typename collection_type::ordinate_type                              ordinate_type;
    typedef typename collection_type::value_type                                 value_type;
    typedef typename collection_type::distance_type                              distance_type;

    typedef typename collection_type::const_iterator                             const_iterator;
    typedef typename collection_type::const_reverse_iterator                     const_reverse_iterator;
    typedef typename collection_type::iterator                                   iterator;
    typedef typename collection_type::reverse_iterator                           reverse_iterator;

    typedef JFunction1D<abscissa_type, JResult_t>                                function_type;           

    typedef typename function_type::argument_type                                argument_type;
    typedef typename function_type::result_type                                  result_type;
    typedef typename function_type::JExceptionHandler                            exceptionhandler_type;


    /**
     * Default contructor.
     */
    JSplineFunction1D()
    {}
  };


  /**
   * \cond NEVER
   * Forward declarations.
   * \endcond
   */
  template<class JAbscissa_t, class JOrdinate_t>
  struct JSplineElement2D;

  template<template<class, class, class> class JMap_t>
  struct JMapCollection;


  /**
   * Functional map with spline interpolation.
   */
  template<class JKey_t,
           class JValue_t,
           template<class, class, class> class JMap_t,
           class JResult_t,
           class JDistance_t = JDistance<JKey_t> >
  class JSplineMap :
    public JMap_t<JKey_t, JValue_t, JDistance_t>,
    public JFunction<JKey_t, JResult_t>
  {
  public:

    typedef JMap_t<JKey_t, JValue_t, JDistance_t>                                collection_type;
    typedef JFunction<JKey_t, JResult_t>                                         function_type;

    typedef typename collection_type::abscissa_type                              abscissa_type;
    typedef typename collection_type::ordinate_type                              ordinate_type;
    typedef typename collection_type::value_type                                 value_type;
    typedef typename collection_type::distance_type                              distance_type;

    typedef typename collection_type::const_iterator                             const_iterator;
    typedef typename collection_type::const_reverse_iterator                     const_reverse_iterator;
    typedef typename collection_type::iterator                                   iterator;
    typedef typename collection_type::reverse_iterator                           reverse_iterator;

    typedef typename function_type::argument_type                                argument_type;
    typedef typename function_type::result_type                                  result_type;
    typedef typename function_type::JExceptionHandler                            exceptionhandler_type;

    typedef typename JResultType<ordinate_type>::result_type                     data_type;
    typedef JSplineFunction1D<JSplineElement2D<argument_type, data_type>, 
			      JMapCollection<JMap_t>::template collection_type,
			      result_type>                                       JSplineFunction1D_t;


    /**
     * Default constructor.
     */
    JSplineMap()
    {}


    /**
     * Recursive interpolation method implementation.
     *
     * \param  pX              pointer to abscissa values
     * \return                 function value
     */
    virtual result_type evaluate(const argument_type* pX) const override 
    {
      const argument_type x = *pX;

      ++pX;  // next argument value

      const_iterator p = this->begin();

      for (typename JSplineFunction1D_t::iterator q = buffer.begin(); q != buffer.end(); ++q, ++p) {
	q->getY() = JFunction<argument_type, data_type>::getValue(p->getY(), pX);
      }

      buffer.compile();

      return buffer(x);
    }


  private:
    /**
     * Function compilation.
     */
    virtual void do_compile() override 
    {
      buffer.clear();

      for (iterator i = this->begin(); i != this->end(); ++i) {
	buffer.put(i->getX(), data_type());
      }
    }


    mutable JSplineFunction1D_t buffer;
  };


  /**
   * Conversion of data points to integral values.
   *
   * The integration includes the use of 2nd derivatives of the data points of the input spline interpolating function.
   *
   * \param  input             collection   
   * \param  output            mappable collection
   * \return                   integral
   */
  template<class JElement_t,
           template<class, class> class JCollection_t,
           class JResult_t,
           class JDistance_t>
  inline typename JElement_t::ordinate_type 
  integrate(const JSplineFunction1D<JElement_t, JCollection_t, JResult_t, JDistance_t>& input, 
	    typename JMappable<JElement_t>::map_type&                                   output)
  {
    typedef typename JElement_t::ordinate_type                                                             ordinate_type;
    typedef typename JSplineFunction1D<JElement_t, JCollection_t, JResult_t, JDistance_t>::const_iterator  const_iterator;
    
    ordinate_type V(JMATH::zero);
    
    if (input.getSize() > 1) {
      
      output.put(input.begin()->getX(), V);
      
      for (const_iterator j = input.begin(), i = j++; j != input.end(); ++i, ++j) {
	
	const double        dx = input.getDistance(i->getX(), j->getX());
	const ordinate_type y  = i->getY()  +  j->getY();
	const ordinate_type z  = i->getU()  +  j->getU();             
	const ordinate_type v  = dx * 0.50 * y;
	const ordinate_type w  = dx * 0.25 * z*dx*dx/6;
	
	V += v - w;
	
	output.put(j->getX(), V);
      }
    }
    
    return V;
  }
  

  /**
   * Conversion of data points to integral values.
   *
   * The integration directly uses the integral values of the input spline interpolating function.
   *
   * \param  input             collection   
   * \param  output            mappable collection
   * \return                   integral
   */
  template<class JElement_t,
           template<class, class> class JCollection_t,
           class JDistance_t>
  inline typename JElement_t::ordinate_type
  integrate(const JSplineFunction1D<JElement_t, JCollection_t, JResultPDF<typename JElement_t::ordinate_type>, JDistance_t>& input,
	    typename JMappable<JElement_t>::map_type&                                                                        output)
  {
    typedef typename JElement_t::ordinate_type                                                               ordinate_type;
    typedef JResultPDF<ordinate_type>                                                                        result_type;
    typedef typename JSplineFunction1D<JElement_t, JCollection_t, result_type, JDistance_t>::const_iterator  const_iterator;
    
    if (input.getSize() > 1) {
      
      for (const_iterator i = input.begin(); i != input.end(); ++i) {
        output.put(i->getX(), i->getIntegral());
      }

      return input.rbegin()->getIntegral();
    }
    
    return JMATH::zero;
  }
}

#endif
