#ifndef __JTOOLS__JHERMITESPLINE__
#define __JTOOLS__JHERMITESPLINE__

#include <utility>
#include <cmath>

#include "JMath/JZero.hh"
#include "JLang/JException.hh"
#include "JLang/JClass.hh"
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
   * Template base class spline interpolations.
   *
   * This class implements the JFunctional interface.
   *
   * Note that the data structure of the elements in the collection should have the additional methods:
   * <pre>
   *     ordinate_type getU() const;
   *     void setU(ordinate_type u);
   * </pre>
   * to get and set the derivatives, respectively.
   *
   * Note that -by default- the compilation is for a monotonous interpolation.
   */
  template<class JElement_t, template<class, class> class JCollection_t, class JDistance_t>
  class JHermiteSplineCollection :
    public JCollection_t<JElement_t, JDistance_t>,
    public virtual JFunctional<>
  {
  public:

    typedef JCollection_t<JElement_t, JDistance_t>                               collection_type;

    typedef typename collection_type::abscissa_type                              abscissa_type;
    typedef typename collection_type::ordinate_type                              ordinate_type;
    typedef typename collection_type::value_type                                 value_type;
    typedef typename collection_type::distance_type                              distance_type;

    typedef typename collection_type::const_iterator                             const_iterator;
    typedef typename collection_type::const_reverse_iterator                     const_reverse_iterator;
    typedef typename collection_type::iterator                                   iterator;
    typedef typename collection_type::reverse_iterator                           reverse_iterator;
 
    using JFunctional<>::compile;


    /**
     * Determination of derivatives.
     *
     * \param  monotone        monotone
     */
    void compile(const bool monotone)
    {
      using namespace std;
      
      if (this->size() >= 2u) {

	{
	  iterator         j = this->begin(), i = j++;

	  i->setU((j->getY() - i->getY()) / this->getDistance(i->getX(), j->getX()));
	}		

	{
	  reverse_iterator j = this->rbegin(), i = j++;

	  i->setU((j->getY() - i->getY()) / this->getDistance(i->getX(), j->getX()));
	}	
	
	for (iterator k = this->begin(), i = k++, j = k++; k != this->end(); ++i, ++j, ++k) {
	  j->setU(0.5 * ((j->getY() - i->getY()) / this->getDistance(i->getX(), j->getX()) +
			 (k->getY() - j->getY()) / this->getDistance(j->getX(), k->getX())));
	}

	if (monotone) {
	  
	  for (iterator j = this->begin(), i = j++; j != this->end(); ++i, ++j) {
	    if (i->getY() == j->getY()) {
	      j->setU(JMATH::zero);
	    }
	  }

	  for (iterator j = this->begin(), i = j++; j != this->end(); ++i, ++j) {

	    const ordinate_type u = (j->getY() - i->getY()) / this->getDistance(i->getX(), j->getX());
	    const ordinate_type w = (i->getU()*i->getU() + j->getU()*j->getU());
	    
	    if (w > 9.0*u*u) {

	      const ordinate_type v = 3.0*u/sqrt(w);

	      i->setU(v*i->getU());
	      j->setU(v*j->getU());
	    }
	  }
	}
      }
    }
    

  protected:
    
    static abscissa_type h00 (abscissa_type t)  { return (1.0 + 2*t) * (1.0 - t) * (1.0 - t); }
    static abscissa_type h10 (abscissa_type t)  { return t * (1.0 - t) * (1.0 - t); }
    static abscissa_type h01 (abscissa_type t)  { return t * t * (3.0 - 2*t); }
    static abscissa_type h11 (abscissa_type t)  { return t * t * (t - 1.0); }

    static abscissa_type h00p(abscissa_type t)  { return 6 * t * (t - 1.0); }
    static abscissa_type h10p(abscissa_type t)  { return t * (3*t - 4.0) + 1.0; }
    static abscissa_type h01p(abscissa_type t)  { return 6 * t * (1.0 -t); }
    static abscissa_type h11p(abscissa_type t)  { return t * (3*t - 2.0); }
    
    static abscissa_type H00 (abscissa_type t)  { return t * (t * t * (0.5*t - 1.0) + 1.0); } 
    static abscissa_type H10 (abscissa_type t)  { return t * t * (t * (0.25*t - 2.0/3.0) + 0.5); }
    static abscissa_type H01 (abscissa_type t)  { return t * t * t * (1.0 - 0.5*t); }
    static abscissa_type H11 (abscissa_type t)  { return t * t * t * (0.25*t - 1.0/3.0); }

    
    /**
     * Default constructor.
     */
    JHermiteSplineCollection()
    {}


    /**
     * Determination of derivatives.
     */
    virtual void do_compile() override 
    {
      compile(true);
    }
  };


  /**
   * Template definition for functional collection with spline interpolation.
   */
  template<class JElement_t, 
	   template<class, class> class JCollection_t,
	   class JResult_t,
           class JDistance_t>
  class JHermiteSplineFunction;


  /**
   * Template specialisation for functional collection with spline interpolation.
   */
  template<class JElement_t, template<class, class> class JCollection_t, class JDistance_t>
  class JHermiteSplineFunction<JElement_t, 
			       JCollection_t, 
			       typename JResultType<typename JElement_t::ordinate_type>::result_type,
			       JDistance_t> :
    public JHermiteSplineCollection<JElement_t, JCollection_t, JDistance_t>,
    public JFunction<typename JElement_t::abscissa_type, 
		     typename JResultType<typename JElement_t::ordinate_type>::result_type>
  {
  public:

    typedef JHermiteSplineCollection<JElement_t, JCollection_t, JDistance_t>     collection_type;

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
    JHermiteSplineFunction()
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

        return this->getExceptionHandler().action(JValueOutOfRange("JHermiteSplineFunction::evaluate() x out of range."));
      }

      const_iterator q = p--;

      const double dx = this->getDistance(p->getX(), q->getX());
      const double t  = this->getDistance(p->getX(), x) / dx;

      return h00(t)*p->getY() + h10(t)*p->getU()*dx + h01(t)*q->getY() + h11(t)*q->getU()*dx;
    }

  protected:
    
    using collection_type::h00;
    using collection_type::h10;
    using collection_type::h01;
    using collection_type::h11;
  };


  /**
   * Template specialisation for spline interpolation method with returning JResultDerivative data structure.
   */
  template<class JElement_t, template<class, class> class JCollection_t, class JDistance_t>
  class JHermiteSplineFunction<JElement_t,
			       JCollection_t,
			       JResultDerivative<typename JResultType<typename JElement_t::ordinate_type>::result_type>,
			       JDistance_t> :
    public JHermiteSplineCollection<JElement_t, JCollection_t, JDistance_t>,
    public JFunction<typename JElement_t::abscissa_type, 
		     JResultDerivative<typename JResultType<typename JElement_t::ordinate_type>::result_type> >
  {
  public:

    typedef JHermiteSplineCollection<JElement_t, JCollection_t, JDistance_t>     collection_type;

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


    /**
     * Default constructor.
     */
    JHermiteSplineFunction() 
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

        return this->getExceptionHandler().action(JValueOutOfRange("JHermiteSplineFunction::evaluate() x out of range."));
      }

      const_iterator q = p--;

      const double dx = this->getDistance(p->getX(), q->getX());
      const double t  = this->getDistance(p->getX(), x) / dx;

      result.f  = h00 (t)*p->getY()    + h10 (t)*p->getU()*dx + h01 (t)*q->getY()    + h11 (t)*q->getU()*dx;
      result.fp = h00p(t)*p->getY()/dx + h10p(t)*p->getU()    + h01p(t)*q->getY()/dx + h11p(t)*q->getU();

      return result;
    }

    
  protected:
    
    using collection_type::h00;
    using collection_type::h10;
    using collection_type::h01;
    using collection_type::h11;

    using collection_type::h00p;
    using collection_type::h10p;
    using collection_type::h01p;
    using collection_type::h11p;
    
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
  class JHermiteSplineFunction<JElement_t,
			       JCollection_t,
			       JResultPDF<typename JResultType<typename JElement_t::ordinate_type>::result_type>,
			       JDistance_t> :
    public JHermiteSplineCollection<JElement_t, JCollection_t, JDistance_t>,
    public JFunction<typename JElement_t::abscissa_type, 
		     JResultPDF<typename JResultType<typename JElement_t::ordinate_type>::result_type> >
  {
  public:

    typedef JHermiteSplineCollection<JElement_t, JCollection_t, JDistance_t>     collection_type;

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
 

    /**
     * Default constructor.			
     */
    JHermiteSplineFunction() 
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

      if        (p == this->begin() && this->getDistance(x, (p++)->getX()) > distance_type::precision) {

	try {

	  result   = this->getExceptionHandler().action(JValueOutOfRange("JHermiteSplineFunction1D<>::operator() x < xmin."));

	  // overwrite integral values

	  result.v = 0;
	  result.V = this->rbegin()->getIntegral();

	} catch(const JValueOutOfRange& exception) {
	  throw exception;
	}

	return result;

      } else if (p == this->end()   && this->getDistance((--p)->getX(), x) > distance_type::precision) {

	try {

	  result   = this->getExceptionHandler().action(JValueOutOfRange("JHermiteSplineFunction1D<>::operator() x > xmax."));

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
      const double t  = this->getDistance(p->getX(), x) / dx;

      result.f   =   h00 (t)*p->getY()    + h10 (t)*p->getU()*dx + h01 (t)*q->getY()    + h11 (t)*q->getU()*dx;
      result.fp  =   h00p(t)*p->getY()/dx + h10p(t)*p->getU()    + h01p(t)*q->getY()/dx + h11p(t)*q->getU();
      result.v   = (p->getIntegral() +
		    (H00 (t)*p->getY()    + H10 (t)*p->getU()*dx + H01 (t)*q->getY()    + H11 (t)*q->getU()*dx)*dx);
      result.V   =   this->rbegin()->getIntegral();

      return result;
    }


  protected:

    using collection_type::h00;
    using collection_type::h10;
    using collection_type::h01;
    using collection_type::h11;

    using collection_type::h00p;
    using collection_type::h10p;
    using collection_type::h01p;
    using collection_type::h11p;

    using collection_type::H00;
    using collection_type::H10;
    using collection_type::H01;
    using collection_type::H11;

    /**
     * Determination of derivatives.
     */
    virtual void do_compile() override 
    {
      if (!this->empty()) {

	collection_type::do_compile();
 
	this->begin()->setIntegral(JMATH::zero);

	for (iterator j = this->begin(), i = j++; j != this->end(); ++i, ++j) {
	  
	  const double        dx = this->getDistance(i->getX(), j->getX());
	  const ordinate_type y  = i->getY() + j->getY();
	  const ordinate_type z  = i->getU() - j->getU();
	
	  const ordinate_type v = dx * 0.50 * y;
	  const ordinate_type w = dx * 1.00 * z*dx/12.0;

	  j->setIntegral(i->getIntegral() + v + w);
	}
      }
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
  class JHermiteSplineFunction1D :
    public JHermiteSplineFunction<JElement_t, JCollection_t, JResult_t, JDistance_t>,
    public JFunction1D<typename JElement_t::abscissa_type, JResult_t> 
  {
  public:

    typedef JHermiteSplineCollection<JElement_t, JCollection_t, JDistance_t>     collection_type;

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
    JHermiteSplineFunction1D()
    {}
  };


  /**
   * Functional map with spline interpolation.
   */
  template<class JKey_t,
           class JValue_t,
           template<class, class, class> class JMap_t,
           class JResult_t,
           class JDistance_t = JDistance<JKey_t> >
  class JHermiteSplineMap :
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
    typedef JHermiteSplineFunction1D<JSplineElement2D<argument_type, data_type>, 
				     JMapCollection<JMap_t>::template collection_type,
				     result_type>                                JHermiteSplineFunction1D_t;


    /**
     * Default constructor.
     */
    JHermiteSplineMap() 
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

      for (typename JHermiteSplineFunction1D_t::iterator q = buffer.begin(); q != buffer.end(); ++q, ++p) {
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


    mutable JHermiteSplineFunction1D_t buffer;
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
  integrate(const JHermiteSplineFunction1D<JElement_t, JCollection_t, JResult_t, JDistance_t>& input, 
	    typename JMappable<JElement_t>::map_type&                                   output)
  {
    typedef typename JElement_t::ordinate_type                                                             ordinate_type;
    typedef typename JHermiteSplineFunction1D<JElement_t, JCollection_t, JResult_t, JDistance_t>::const_iterator  const_iterator;
    
    ordinate_type V(JMATH::zero);
    
    if (input.getSize() > 1) {
      
      output.put(input.begin()->getX(), V);
      
      for (const_iterator j = input.begin(), i = j++; j != input.end(); ++i, ++j) {
	
	const double        dx = input.getDistance(i->getX(), j->getX());
	const ordinate_type y  = i->getY() + j->getY();
	const ordinate_type z  = i->getU() - j->getU();
	
	const ordinate_type v = dx * 0.50 * y;
	const ordinate_type w = dx * 1.00 * z*dx/12.0;
	
	V += v + w;
	
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
  integrate(const JHermiteSplineFunction1D<JElement_t, JCollection_t, JResultPDF<typename JElement_t::ordinate_type>, JDistance_t>& input,
	    typename JMappable<JElement_t>::map_type&                                                                               output)
  {
    typedef typename JElement_t::ordinate_type                                                                      ordinate_type;
    typedef JResultPDF<ordinate_type>                                                                               result_type;
    typedef typename JHermiteSplineFunction1D<JElement_t, JCollection_t, result_type, JDistance_t>::const_iterator  const_iterator;
    
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
