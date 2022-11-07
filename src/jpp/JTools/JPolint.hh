#ifndef __JTOOLS__JPOLINT__
#define __JTOOLS__JPOLINT__

#include <cmath>
#include <iterator>
#include <algorithm>

#include "JLang/JException.hh"
#include "JLang/JAssert.hh"
#include "JLang/JBool.hh"
#include "JLang/JStreamAvailable.hh"
#include "JTools/JFunctional.hh"
#include "JTools/JDistance.hh"
#include "JTools/JResult.hh"
#include "JTools/JElement.hh"
#include "JTools/JMapCollection.hh"
#include "JTools/JQuadrature.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JLANG::JException;
  using JLANG::JFunctionalException;
  using JLANG::JNumericalPrecision;
  using JLANG::JValueOutOfRange;


  /**
   * Template definition for functional collection with polynomial interpolation.
   */
  template<unsigned int N, 
	   class JElement_t,
	   template<class, class> class JCollection_t,
	   class JResult_t,
	   class JDistance_t>
  class JPolintFunction;


  /**
   * Template specialisation for functional collection with polynomial interpolation.
   *
   * Polynomial interpolation code is taken from reference:
   * Numerical Recipes in C++, W.H. Press, S.A. Teukolsky, W.T. Vetterling and B.P. Flannery,
   * Cambridge University Press.
   */
  template<unsigned int N, class JElement_t, template<class, class> class JCollection_t, class JDistance_t>
  class JPolintFunction<N, 
			JElement_t, 
			JCollection_t, 
			typename JResultType<typename JElement_t::ordinate_type>::result_type,
			JDistance_t> :
    public JCollection_t<JElement_t, JDistance_t>,
    public JFunction<typename JElement_t::abscissa_type,
		     typename JResultType<typename JElement_t::ordinate_type>::result_type>
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

    typedef typename JResultType<ordinate_type>::result_type                     data_type;
    typedef JFunction<abscissa_type, data_type>                                  function_type;           

    typedef typename function_type::argument_type                                argument_type;
    typedef typename function_type::result_type                                  result_type;
    typedef typename function_type::JExceptionHandler                            exceptionhandler_type;


    /**
     * Default constructor.			
     */
    JPolintFunction()
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

	++pX;  // next argument value


	const int n = std::min((int) (N + 1), (int) this->size());         // number of points to interpolate      

	for (int i = n/2; i != 0 && p != this->end();   --i, ++p) {}       // move p to begin of data
	for (int i = n  ; i != 0 && p != this->begin(); --i, --p) {}


	int j = 0;

	for (int i = 0; i != n; ++p, ++i) {
	
	  u[i] = this->getDistance(x, p->getX());
	  v[i] = function_type::getValue(p->getY(), pX);
	  w[i] = v[i];

	  if (fabs(u[i]) < fabs(u[j])) {
	    j = i;
	  }
	}

      
	result_type y = v[j];

	--j;

	for (int m = 1; m != n; ++m) {

	  for (int i = 0; i != n-m; ++i) {

	    const double ho = u[ i ];
	    const double hp = u[i+m];
	    const double dx = ho - hp;

	    v[i]  = v[i+1];
	    v[i] -= w[ i ];
	    w[i]  = v[ i ];

	    v[i] *= ho/dx;
	    w[i] *= hp/dx;
	  }

	  if (2*(j+1) < n - m)
	    y += v[j+1];
	  else
	    y += w[j--];
	}

	return y;
	
      } else if (this->size() == 1u && this->getDistance(x, this->begin()->getX()) <= distance_type::precision) {
			
	return function_type::getValue(this->begin()->getY(), ++pX);
	
      } else {

        return this->getExceptionHandler().action(MAKE_EXCEPTION(JFunctionalException, "not enough data " << STREAM("?") << x));	
      }
    }

  protected: 
    /**
     * Function compilation.
     */
    virtual void do_compile() override 
    {}


  private: 
    mutable double      u[N+1];
    mutable result_type v[N+1];
    mutable result_type w[N+1];
  };


  /**
   * Template specialisation for zero-degree polynomial interpolation method.\n
   * The interpolation is based on a simple lookup table.
   */
  template<class JElement_t, template<class, class> class JCollection_t, class JDistance_t>
  class JPolintFunction<0, 
			JElement_t,
			JCollection_t,
			typename JResultType<typename JElement_t::ordinate_type>::result_type,
			JDistance_t> :
    public JCollection_t<JElement_t, JDistance_t>,
    public JFunction<typename JElement_t::abscissa_type,
		     typename JResultType<typename JElement_t::ordinate_type>::result_type>
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

    typedef typename JResultType<ordinate_type>::result_type                     data_type;
    typedef JFunction<abscissa_type, data_type>                                  function_type;           

    typedef typename function_type::argument_type                                argument_type;
    typedef typename function_type::result_type                                  result_type;
    typedef typename function_type::JExceptionHandler                            exceptionhandler_type;


    /**
     * Default constructor.			
     */
    JPolintFunction()
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

	++pX;  // next argument value


	const_iterator q = p--;

	if (q == this->begin() || this->getDistance(x, q->getX()) < this->getDistance(p->getX(), x)) 
	  return function_type::getValue(q->getY(), pX);
	else
	  return function_type::getValue(p->getY(), pX);

      } else if (this->size() == 1u && this->getDistance(x, this->begin()->getX()) <= distance_type::precision) {
			
	return function_type::getValue(this->begin()->getY(), ++pX);
	
      } else {

        return this->getExceptionHandler().action(MAKE_EXCEPTION(JFunctionalException, "not enough data " << STREAM("?") << x));	
      }
    }

  protected:
    /**
     * Function compilation.
     */
    virtual void do_compile() override 
    {}
  };


  /**
   * Template specialisation for first-degree polynomial interpolation method.\n
   * The interpolation is based on a simple linear interpolation.
   */
  template<class JElement_t, template<class, class> class JCollection_t, class JDistance_t>
  class JPolintFunction<1, 
			JElement_t,
			JCollection_t,
			typename JResultType<typename JElement_t::ordinate_type>::result_type,
			JDistance_t> :
    public JCollection_t<JElement_t, JDistance_t>,
    public JFunction<typename JElement_t::abscissa_type,
		     typename JResultType<typename JElement_t::ordinate_type>::result_type>
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

    typedef typename JResultType<ordinate_type>::result_type                     data_type;
    typedef JFunction<abscissa_type, data_type>                                  function_type;           

    typedef typename function_type::argument_type                                argument_type;
    typedef typename function_type::result_type                                  result_type;
    typedef typename function_type::JExceptionHandler                            exceptionhandler_type;


    /**
     * Default constructor.			
     */
    JPolintFunction() 
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

	++pX;  // next argument value


	const_iterator q = p--;

	const double dx = this->getDistance(p->getX(), q->getX());
	const double a  = this->getDistance(x, q->getX()) / dx;
	const double b  = 1.0 - a;

	ya = function_type::getValue(p->getY(), pX);
	yb = function_type::getValue(q->getY(), pX);

	ya *= a;
	yb *= b;

	ya += yb;

	return ya;
	
      } else if (this->size() == 1u && this->getDistance(x, this->begin()->getX()) <= distance_type::precision) {
	
	return function_type::getValue(this->begin()->getY(), ++pX);
	
      } else {

        return this->getExceptionHandler().action(MAKE_EXCEPTION(JFunctionalException, "not enough data " << STREAM("?") << x));	
      }
    }

  protected:
    /**
     * Function compilation.
     */
    virtual void do_compile() override 
    {}


  private:
    mutable result_type ya;
    mutable result_type yb;
  };


  /**
   * Template specialisation for polynomial interpolation method with returning JResultPDF data structure.
   *
   * Note that the data structure of the elements in the collection should have the additional methods:
   * <pre>
   *     ordinate_type getIntegral() const;
   *     void setIntegral(ordinate_type v);
   * </pre>
   * to get and set the integral values, respectively.
   */
  template<unsigned int N, class JElement_t, template<class, class> class JCollection_t, class JDistance_t>
  class JPolintFunction<N,
			JElement_t,
			JCollection_t,
			JResultPDF<typename JResultType<typename JElement_t::ordinate_type>::result_type>,
			JDistance_t> :
    public JCollection_t<JElement_t, JDistance_t>,
    public JFunction<typename JElement_t::abscissa_type, 
		     JResultPDF<typename JResultType<typename JElement_t::ordinate_type>::result_type> >
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

    typedef typename JResultType<ordinate_type>::result_type                     data_type;
    typedef JFunction<abscissa_type, JResultPDF<data_type> >                     function_type;

    typedef typename function_type::argument_type                                argument_type;
    typedef typename function_type::result_type                                  result_type;
    typedef typename function_type::JExceptionHandler                            exceptionhandler_type;

    using JFunctional<>::compile;


    /**
     * Default contructor.
     */
    JPolintFunction()
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

      ++pX;  // next argument value

      {
	const int n = std::min((int) (N + 1), (int) this->size());         // number of points to interpolate

	for (int i = n/2; i != 0 && p != this->end();   --i, ++p) {}       // move p to begin of data
	for (int i = n  ; i != 0 && p != this->begin(); --i, --p) {}

	
	int j = 0;
	
	for (int i = 0; i != n; ++p, ++i) {

	  u[i] = this->getDistance(x, p->getX());

	  w[i][0] = v[i][0] = JFunctional<argument_type, data_type>::getValue(p->getY(), pX);
	  w[i][1] = v[i][1] = JMATH::zero;
	  w[i][2] = v[i][2] = p->getIntegral();
	  
	  if (fabs(u[i]) < fabs(u[j])) {
	    j = i;
	  }
	}


	result.f  = v[j][0];
	result.fp = v[j][1];
	result.v  = v[j][2];
	result.V  = this->rbegin()->getIntegral();

	--j;
	
	for (int m = 1; m != n; ++m) {
	  
	  for (int i = 0; i != n-m; ++i) {
	    
	    const double ho = u[ i ];
	    const double hp = u[i+m];
	    const double dx = ho - hp;
	    
	    for (int k = 0; k != 3; ++k) {
	      r[k] = (v[i+1][k] - w[i][k]) / dx;
	    }
	    
	    v[i][0] = ho * r[0];
	    w[i][0] = hp * r[0];
	    v[i][1] = ho * r[1] - r[0];
	    w[i][1] = hp * r[1] - r[0];
	    v[i][2] = ho * r[2];
	    w[i][2] = hp * r[2];
	  }

	  if (2*(j+1) < n - m) {
	    
	    result.f  += v[j+1][0];
	    result.fp += v[j+1][1];
	    result.v  += v[j+1][2];
	    
	  } else {
	    
	    result.f  += w[j][0];
	    result.fp += w[j][1];
	    result.v  += w[j][2];
	  
	    --j;
	  }
	}
      }
      
      return result;
    }

  protected: 
    /**
     * Function compilation.
     */
    virtual void do_compile() override 
    {
      ordinate_type V(JMATH::zero);

      if (this->getSize() > 1) {

	const JGaussLegendre engine(N);

	this->begin()->setIntegral(V);

	for (iterator j = this->begin(), i = j++; j != this->end(); ++i, ++j) {
	  
	  const abscissa_type xmin = i->getX();
	  const abscissa_type xmax = j->getX();
	  
	  for (JGaussLegendre::const_iterator m = engine.begin(); m != engine.end(); ++m) {
	    
	    const abscissa_type x = 0.5 * (xmax + xmin  +  m->getX() * (xmax - xmin));
	    const ordinate_type v = 0.5 * (xmax - xmin) *  m->getY() * get_value(this->evaluate(&x));
	    
	    V += v;
	  }
	  
	  j->setIntegral(V);
	}
      }
    }


    mutable double    u[N+1];
    mutable data_type v[N+1][3];
    mutable data_type w[N+1][3];
    mutable data_type r[3];

    mutable result_type result;
  };


  /**
   * Template definition of base class for polynomial interpolations with polynomial result.
   */
  template<unsigned int N,
	   class JElement_t,
	   template<class, class> class JCollection_t,
	   class JResult_t,
	   class JDistance_t>
  class JPolintCollection;


  /**
   * Template base class for polynomial interpolations with polynomial result.
   *
   * This class partially implements the JFunctional interface.
   */
  template<unsigned int N, class JElement_t, template<class, class> class JCollection_t, class JDistance_t, unsigned int M>
  class JPolintCollection<N,
			  JElement_t,
			  JCollection_t,
			  JResultPolynome<M, typename JResultType<typename JElement_t::ordinate_type>::result_type>,
			  JDistance_t> :
    public JCollection_t<JElement_t, JDistance_t>,
    public virtual JFunctional<>,
    private JLANG::JAssert<M <= N>
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

    typedef typename JResultType<ordinate_type>::result_type                     data_type;
    typedef JFunction<abscissa_type, JResultPolynome<M, data_type> >             function_type;

    typedef typename function_type::argument_type                                argument_type;
    typedef typename function_type::result_type                                  result_type;

    using JFunctional<>::compile;


    /**
     * Default contructor.
     */
    JPolintCollection()
    {}


    /**
     * Recursive interpolation method implementation.
     *
     * \param  pX              pointer to abscissa values
     * \return                 function value
     */
    result_type evaluate(const argument_type* pX) const
    {
      const argument_type x = *pX;

      if (this->size() <= N) {
        THROW(JFunctionalException, "not enough data " << STREAM("?") << x);
      }

      const_iterator p = this->lower_bound(x);

      if ((p == this->begin() && this->getDistance(x, (p++)->getX()) > distance_type::precision) ||
          (p == this->end()   && this->getDistance((--p)->getX(), x) > distance_type::precision)) {

        THROW(JValueOutOfRange, "abscissa out of range " 
	      << STREAM("?") << x                      << " <> " 
	      << STREAM("?") << this->begin() ->getX() << ' '
	      << STREAM("?") << this->rbegin()->getX());
      }

      ++pX;  // next argument value


      const int n = std::min((int) (N + 1), (int) this->size());         // number of points to interpolate      

      for (int i = n/2; i != 0 && p != this->end();   --i, ++p) {}       // move p to begin of data
      for (int i = n  ; i != 0 && p != this->begin(); --i, --p) {}


      int j = 0;

      for (int i = 0; i != n; ++p, ++i) {
	
	u[i] = this->getDistance(x, p->getX());

	w[i][0] = v[i][0] = JFunctional<argument_type, data_type>::getValue(p->getY(), pX);
	  
	for (unsigned int k = 1; k != M+1; ++k) {
	  w[i][k] = v[i][k] = JMATH::zero;
	}

	if (fabs(u[i]) < fabs(u[j])) {
	  j = i;
	}
      }


      for (unsigned int k = 0; k != M+1; ++k) {
	result.y[k] = v[j][k];
      }

      --j;

      for (int m = 1; m != n; ++m) {

	for (int i = 0; i != n-m; ++i) {

	  const double ho = u[ i ];
	  const double hp = u[i+m];
	  const double dx = ho - hp;

	  for (int k = 0; k != M+1; ++k) {
	    r[k] = (v[i+1][k] - w[i][k]) / dx;
	  }

	  v[i][0] = ho * r[0];
	  w[i][0] = hp * r[0];

	  for (int k = 1; k != M+1; ++k) {
	    v[i][k] = ho * r[k]  -  k * r[k-1];
	    w[i][k] = hp * r[k]  -  k * r[k-1];
	  }
	}

	if (2*(j+1) < n - m) {

	  for (int k = 0; k != M+1; ++k) {
	    result.y[k] += v[j+1][k];
	  }

	} else {

	  for (int k = 0; k != M+1; ++k) {
	    result.y[k] += w[j][k];
	  }

	  --j;
	}
      }

      return result;
    }

  protected: 
    /**
     * Function compilation.
     */
    virtual void do_compile() override 
    {}


  private: 
    mutable double    u[N+1];
    mutable data_type v[N+1][M+1];
    mutable data_type w[N+1][M+1];
    mutable data_type r[M+1];

    mutable result_type result;
  };


  /**
   * Template specialisation for polynomial interpolation method with returning JResultPolynome data structure.
   */
  template<unsigned int N, class JElement_t, template<class, class> class JCollection_t, class JDistance_t, unsigned int M>
  class JPolintFunction<N,
			JElement_t,
			JCollection_t,
			JResultPolynome<M, typename JResultType<typename JElement_t::ordinate_type>::result_type>,
			JDistance_t> :
    public JPolintCollection<N,
			     JElement_t,
			     JCollection_t,
			     JResultPolynome<M, typename JResultType<typename JElement_t::ordinate_type>::result_type>,
			     JDistance_t>,
    public JFunction<typename JElement_t::abscissa_type, 
		     JResultPolynome<N, typename JResultType<typename JElement_t::ordinate_type>::result_type> >
  {
  public:

    typedef JPolintCollection<N,
			      JElement_t,
			      JCollection_t,
			      JResultPolynome<M, typename JResultType<typename JElement_t::ordinate_type>::result_type>,
			      JDistance_t>                                       collection_type;

    typedef typename collection_type::abscissa_type                              abscissa_type;
    typedef typename collection_type::ordinate_type                              ordinate_type;
    typedef typename collection_type::value_type                                 value_type;
    typedef typename collection_type::distance_type                              distance_type;

    typedef typename collection_type::const_iterator                             const_iterator;
    typedef typename collection_type::const_reverse_iterator                     const_reverse_iterator;
    typedef typename collection_type::iterator                                   iterator;
    typedef typename collection_type::reverse_iterator                           reverse_iterator;

    typedef typename JResultType<ordinate_type>::result_type                     data_type;
    typedef JFunction<abscissa_type, JResultPolynome<M, data_type> >             function_type;

    typedef typename function_type::argument_type                                argument_type;
    typedef typename function_type::result_type                                  result_type;
    typedef typename function_type::JExceptionHandler                            exceptionhandler_type;


    /**
     * Default contructor.
     */
    JPolintFunction()
    {}


    /**
     * Recursive interpolation method implementation.
     *
     * \param  pX              pointer to abscissa values
     * \return                 function value
     */
    virtual result_type evaluate(const argument_type* pX) const override 
    {
      try {
	return collection_type::evaluate(pX);
      }
      catch(const JException& error) {
        return this->getExceptionHandler().action(error);
      }
    }
  };


  /**
   * Template specialisation for polynomial interpolation method with returning JResultDerivative data structure.
   */
  template<unsigned int N, class JElement_t, template<class, class> class JCollection_t, class JDistance_t>
  class JPolintFunction<N,
			JElement_t,
			JCollection_t,
			JResultDerivative<typename JResultType<typename JElement_t::ordinate_type>::result_type>,
			JDistance_t> :
    public JPolintCollection<N,
			     JElement_t,
			     JCollection_t,
			     JResultPolynome<1, typename JResultType<typename JElement_t::ordinate_type>::result_type>,
			     JDistance_t>,
    public JFunction<typename JElement_t::abscissa_type,
		     JResultDerivative<typename JResultType<typename JElement_t::ordinate_type>::result_type> >
  {
  public:

    typedef JPolintCollection<N,
			      JElement_t,
			      JCollection_t,
			      JResultPolynome<1, typename JResultType<typename JElement_t::ordinate_type>::result_type>,
			      JDistance_t>                                       collection_type;

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
     * Default contructor.
     */
    JPolintFunction()
    {}


    /**
     * Recursive interpolation method implementation.
     *
     * \param  pX              pointer to abscissa values
     * \return                 function value
     */
    virtual result_type evaluate(const argument_type* pX) const override 
    {
      try {
	return collection_type::evaluate(pX);
      }
      catch(const JException& error) {
        return this->getExceptionHandler().action(error);
      }
    }
  };


  /**
   * Template specialisation for polynomial interpolation method with returning JResultHesse data structure.
   */
  template<unsigned int N, class JElement_t, template<class, class> class JCollection_t, class JDistance_t>
  class JPolintFunction<N,
			JElement_t,
			JCollection_t,
			JResultHesse<typename JResultType<typename JElement_t::ordinate_type>::result_type>,
			JDistance_t> :
    public JPolintCollection<N,
			     JElement_t,
			     JCollection_t,
			     JResultPolynome<2, typename JResultType<typename JElement_t::ordinate_type>::result_type>,
			     JDistance_t>,
    public JFunction<typename JElement_t::abscissa_type,
		     JResultHesse<typename JResultType<typename JElement_t::ordinate_type>::result_type> >
  {
  public:

    typedef JPolintCollection<N,
			      JElement_t,
			      JCollection_t,
			      JResultPolynome<2, typename JResultType<typename JElement_t::ordinate_type>::result_type>,
			      JDistance_t>                                       collection_type;

    typedef typename collection_type::abscissa_type                              abscissa_type;
    typedef typename collection_type::ordinate_type                              ordinate_type;
    typedef typename collection_type::value_type                                 value_type;
    typedef typename collection_type::distance_type                              distance_type;

    typedef typename collection_type::const_iterator                             const_iterator;
    typedef typename collection_type::const_reverse_iterator                     const_reverse_iterator;
    typedef typename collection_type::iterator                                   iterator;
    typedef typename collection_type::reverse_iterator                           reverse_iterator;

    typedef typename JResultType<ordinate_type>::result_type                     data_type;
    typedef JFunction<abscissa_type, JResultHesse<data_type> >                   function_type;

    typedef typename function_type::argument_type                                argument_type;
    typedef typename function_type::result_type                                  result_type;
    typedef typename function_type::JExceptionHandler                            exceptionhandler_type;

    using JFunctional<>::compile;


    /**
     * Default contructor.
     */
    JPolintFunction()
    {}


    /**
     * Recursive interpolation method implementation.
     *
     * \param  pX              pointer to abscissa values
     * \return                 function value
     */
    virtual result_type evaluate(const argument_type* pX) const override 
    {
      try {
	return collection_type::evaluate(pX);
      }
      catch(const JException& error) {
        return this->getExceptionHandler().action(error);
      }
    }
  };


  /**
   * Template class for polynomial interpolation in 1D
   *
   * This class implements the JFunction1D interface.
   */
  template<unsigned int N,
	   class JElement_t,
	   template<class, class> class JCollection_t,
	   class JResult_t   = typename JElement_t::ordinate_type,
	   class JDistance_t = JDistance<typename JElement_t::abscissa_type> >
  class JPolintFunction1D :
    public JPolintFunction<N, JElement_t, JCollection_t, JResult_t, JDistance_t>,
    public JFunction1D<typename JElement_t::abscissa_type, JResult_t> 
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

    typedef JFunction1D<abscissa_type, JResult_t>                                function_type;           

    typedef typename function_type::argument_type                                argument_type;
    typedef typename function_type::result_type                                  result_type;
    typedef typename function_type::JExceptionHandler                            exceptionhandler_type;


    /**
     * Default contructor.
     */
    JPolintFunction1D()
    {}
  };


  /**
   * \cond NEVER
   * Forward declarations.
   * \endcond
   */
  template<class JAbscissa_t, class JOrdinate_t>
  struct JElement2D;

  template<template<class, class, class> class JMap_t>
  struct JMapCollection;


  /**
   * Functional map with polynomial interpolation.
   */
  template<unsigned int N,
	   class JKey_t,
	   class JValue_t,
	   template<class, class, class> class JMap_t,
	   class JResult_t,
	   class JDistance_t = JDistance<JKey_t> >
  class JPolintMap :
    public JPolintFunction<N, 
			   JElement2D<JKey_t, JValue_t>, 
			   JMapCollection<JMap_t>::template collection_type, 
			   JResult_t,
			   JDistance_t>
  {
  public:

    typedef JElement2D<JKey_t, JValue_t>                                         element_type;
    typedef JPolintFunction<N, 
			    element_type,
			    JMapCollection<JMap_t>::template collection_type,
			    JResult_t,
			    JDistance_t>                                         JPolintFunction_t;

    typedef typename JPolintFunction_t::abscissa_type                            abscissa_type;
    typedef typename JPolintFunction_t::ordinate_type                            ordinate_type;
    typedef typename JPolintFunction_t::value_type                               value_type;
    typedef typename JPolintFunction_t::distance_type                            distance_type;

    typedef typename JPolintFunction_t::const_iterator                           const_iterator;
    typedef typename JPolintFunction_t::const_reverse_iterator                   const_reverse_iterator;
    typedef typename JPolintFunction_t::iterator                                 iterator;
    typedef typename JPolintFunction_t::reverse_iterator                         reverse_iterator;

    typedef typename JPolintFunction_t::argument_type                            argument_type;
    typedef typename JPolintFunction_t::result_type                              result_type;
    typedef typename JPolintFunction_t::JExceptionHandler                        exceptionhandler_type;


    /**
     * Default constructor.
     */
    JPolintMap()
    {}
  };

                    
  /**
   * Conversion of data points to integral values.
   *
   * This method transfers the integration to the corresponding specialised function.
   *
   * \param  input             collection   
   * \param  output            mappable collection
   * \return                   integral
   */
  template<unsigned int N,
           class JElement_t,
           template<class, class> class JCollection_t,
           class JResult_t,
           class JDistance_t>
  inline typename JElement_t::ordinate_type
  integrate(const JPolintFunction1D<N, JElement_t, JCollection_t, JResult_t, JDistance_t>& input, 
	    typename JMappable<JElement_t>::map_type&                                      output)
  {
    return integrate(input, output, JLANG::JBool<N == 0 || N == 1>());
  }


  /**
   * Conversion of data points to integral values.
   *
   * The integration uses the Gauss-Legendre quadratures with the number of points set
   * to the degree of the input polynomial interpolating function.
   *
   * \param  input             collection   
   * \param  output            mappable collection
   * \param  option            false
   * \return                   integral
   */
  template<unsigned int N,
           class JElement_t,
           template<class, class> class JCollection_t,
           class JResult_t,
           class JDistance_t>
  inline typename JElement_t::ordinate_type
  integrate(const JPolintFunction1D<N, JElement_t, JCollection_t, JResult_t, JDistance_t>& input, 
	    typename JMappable<JElement_t>::map_type&                                      output,
	    const JLANG::JBool<false>&                                                     option)
  {
    typedef typename JElement_t::abscissa_type                                                                abscissa_type;
    typedef typename JElement_t::ordinate_type                                                                ordinate_type;
    typedef typename JPolintFunction1D<N, JElement_t, JCollection_t, JResult_t, JDistance_t>::const_iterator  const_iterator;
    
    ordinate_type V(JMATH::zero);

    if (input.getSize() > 1) {
      
      output.put(input.begin()->getX(), V);
      
      const JGaussLegendre engine(N);
      
      for (const_iterator j = input.begin(), i = j++; j != input.end(); ++i, ++j) {
        
        const abscissa_type xmin = i->getX();
        const abscissa_type xmax = j->getX();
        
        for (JGaussLegendre::const_iterator m = engine.begin(); m != engine.end(); ++m) {
          
          const abscissa_type x = 0.5 * (xmax + xmin  +  m->getX() * (xmax - xmin));
          const ordinate_type v = 0.5 * (xmax - xmin) *  m->getY() * get_value(input(x));
          
          V += v;
        }
        
        output.put(xmax, V);
      }
    }
    
    return V;
  }
  
  
  /**
   * Conversion of data points to integral values.
   *
   * The integration is based on the sum of ordinates of the input data points.
   *
   * \param  input             collection   
   * \param  output            mappable collection
   * \param  option            true
   * \return                   integral
   */
  template<class JElement_t,
           template<class, class> class JCollection_t,
           class JResult_t,
           class JDistance_t>
  inline typename JElement_t::ordinate_type
  integrate(const JPolintFunction1D<0, JElement_t, JCollection_t, JResult_t, JDistance_t>& input, 
	    typename JMappable<JElement_t>::map_type&                                      output,
	    const JLANG::JBool<true>&                                                      option)
  {
    typedef typename JElement_t::ordinate_type                                                                ordinate_type;
    typedef typename JPolintFunction1D<0, JElement_t, JCollection_t, JResult_t, JDistance_t>::const_iterator  const_iterator;
    
    ordinate_type V(JMATH::zero);
    
    if (input.getSize() > 1) {
      
      output.put(input.begin()->getX(), V);
      
      for (const_iterator j = input.begin(), i = j++; j != input.end(); ++i, ++j) {
        
	V += input.getDistance(i->getX(), j->getX()) * j->getY();
        
	output.put(j->getX(), V);
      }
    }
    
    return V;
  }
  

  /**
   * Conversion of data points to integral values.
   *
   * The integration is based on the trapezoidal rule applied to the input data points.
   *
   * \param  input             collection   
   * \param  output            mappable collection
   * \param  option            true
   * \return                   integral
   */
  template<class JElement_t,
           template<class, class> class JCollection_t,
           class JResult_t,
	   class JDistance_t>
  inline typename JElement_t::ordinate_type
  integrate(const JPolintFunction1D<1, JElement_t, JCollection_t, JResult_t, JDistance_t>& input, 
	    typename JMappable<JElement_t>::map_type&                                      output,
	    const JLANG::JBool<true>&                                                      option)
  {
    typedef typename JElement_t::ordinate_type                                                                ordinate_type;
    typedef typename JPolintFunction1D<1, JElement_t, JCollection_t, JResult_t, JDistance_t>::const_iterator  const_iterator;
    
    ordinate_type V(JMATH::zero);
    
    if (input.getSize() > 1) {
      
      output.put(input.begin()->getX(), V);
      
      for (const_iterator j = input.begin(), i = j++; j != input.end(); ++i, ++j) {
	
	V += 0.5 * input.getDistance(i->getX(), j->getX()) * (i->getY() + j->getY());
        
	output.put(j->getX(), V);
      }
    }
    
    return V;
  }
}

#endif
