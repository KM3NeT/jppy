#ifndef __JTOOLS__JRESULT__
#define __JTOOLS__JRESULT__

#include <cmath>

#include "JTools/JRange.hh"
#include "JLang/JClass.hh"
#include "JLang/JAssert.hh"
#include "JMath/JMath.hh"
#include "JMath/JZero.hh"


/**
 * \file
 *
 * This include file containes various data structures that
 * can be used as specific return types for the interpolating methods.
 * These data structures have standard arithmetic capabilities and
 * are templated so that they can be expanded in higher dimensions.
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JMATH::JMath;


  /**
   * Data structure for result including value and first derivative of function.
   *
   * This data structure contains the following data mambers:
   * <pre>
   *    JResultDerivative::f   = function value;
   *    JResultDerivative::fp  = first  derivative;
   * </pre>
   *
   * This class implements the JMATH::JMath interface.
   */
  template<class JResult_t>
  struct JResultDerivative :
    public JMath< JResultDerivative<JResult_t> >
  {

    typedef typename JLANG::JClass<JResult_t>::argument_type                     argument_type;


    /**
     * Default constructor.
     */
    JResultDerivative() :
      f  (JMATH::zero),
      fp (JMATH::zero)
    {}


    /**
     * Constructor.
     *
     * \param  f               function value
     * \param  fp              first  derivative
     */
    JResultDerivative(argument_type f,
		      argument_type fp) :
      f  (f ),
      fp (fp)
    {}


    /**
     * Prefix unary minus for function value of PDF.
     *
     * \return                  function value of PDF
     */
    JResultDerivative& negate()
    {
      f   = -f;
      fp  = -fp;

      return *this;
    }


    /**
     * Addition operator for function value of PDF.
     *
     * \param  value            function value of PDF
     * \return                  function value of PDF
     */
    JResultDerivative& add(const JResultDerivative& value)
    {
      f   += value.f;
      fp  += value.fp;

      return *this;
    }


    /**
     * Subtraction operator for function value of PDF.
     *
     * \param  value            function value of PDF
     * \return                  function value of PDF
     */
    JResultDerivative& sub(const JResultDerivative& value)
    {
      f   -= value.f;
      fp  -= value.fp;

      return *this;
    }


    /**
     * Multiplication operator for function value of PDF.
     *
     * \param  value            multiplication factor
     * \return                  function value of PDF
     */
    JResultDerivative& mul(const double value)
    {
      f   *= value;
      fp  *= value;

      return *this;
    }


    /**
     * Division operator for function value of PDF.
     *
     * \param  value            multiplication factor
     * \return                  function value of PDF
     */
    JResultDerivative& div(const double value)
    {
      f   /= value;
      fp  /= value;

      return *this;
    }


    /**
     * Get probability.\n
     * If the given hit is false (true), the return value corresponds to the Poisson probability
     * that zero (one or more) hits occur for the given expectation value JResultDerivative::f.
     *
     * \param  hit              hit (or not)
     * \return                  probability
     */
    double getP(const bool hit) const
    {
      if (!hit)
	return exp(-f);                  // probability of 0 hits
      else
	return 1.0 - getP(false);        // probability of 1 or more hits
    }


    /**
     * Get chi2.
     * The chi2 corresponds to <tt>-log(P)</tt>, where <tt>P</tt> is the probability JResultDerivative::f.
     *
     * \param  hit              hit (or not)
     * \return                  chi2
     */
    double getChi2(const bool hit) const
    {
      if (!hit)
	return  f;                       // = -log(getP(false))
      else
	return -log(getP(true));
    }


    /**
     * Get derivative of chi2.
     *
     * \param  hit              hit (or not)
     * \return                  derivative
     */
    double getDerivativeOfChi2(const bool hit) const
    {
      if (!hit)
	return  fp;
      else
	return -fp * getP(false) / getP(true);
    }


    JResult_t f;      //!< function value
    JResult_t fp;     //!< first  derivative
  };


  /**
   * Data structure for result including value and first derivative of function.
   *
   * This data structure contains the following data mambers:
   * <pre>
   *    JResultHesse::f   = function value;
   *    JResultHesse::fp  = first  derivative;
   *    JResultHesse::fpp = second derivative;
   * </pre>
   *
   * This class implements the JMATH::JMath interface.
   */
  template<class JResult_t>
  struct JResultHesse :
    public JResultDerivative<JResult_t>,
    public JMath< JResultHesse<JResult_t> >
  {

    typedef typename JResultDerivative<JResult_t>::argument_type                 argument_type;


    /**
     * Default constructor.
     */
    JResultHesse() :
      JResultDerivative<JResult_t>(),
      fpp(JMATH::zero)
    {}


    /**
     * Constructor.
     *
     * \param  f               function value
     * \param  fp              first  derivative
     * \param  fpp             second derivative
     */
    JResultHesse(argument_type f,
		 argument_type fp,
		 argument_type fpp) :
      JResultDerivative<JResult_t>(f, fp),
      fpp(fpp)
    {}


    /**
     * Prefix unary minus for function value of PDF.
     *
     * \return                  function value of PDF
     */
    JResultHesse& negate()
    {
      static_cast<JResultDerivative<JResult_t>&>(*this).negate();
      fpp = -fpp;

      return *this;
    }


    /**
     * Addition operator for function value of PDF.
     *
     * \param  value            function value of PDF
     * \return                  function value of PDF
     */
    JResultHesse& add(const JResultHesse& value)
    {
      static_cast<JResultDerivative<JResult_t>&>(*this).add(value);
      fpp += value.fpp;

      return *this;
    }


    /**
     * Subtraction operator for function value of PDF.
     *
     * \param  value            function value of PDF
     * \return                  function value of PDF
     */
    JResultHesse& sub(const JResultHesse& value)
    {
      static_cast<JResultDerivative<JResult_t>&>(*this).sub(value);
      fpp -= value.fpp;

      return *this;
    }


    /**
     * Multiplication operator for function value of PDF.
     *
     * \param  value            multiplication factor
     * \return                  function value of PDF
     */
    JResultHesse& mul(const double value)
    {
      static_cast<JResultDerivative<JResult_t>&>(*this).mul(value);
      fpp *= value;

      return *this;
    }


    /**
     * Division operator for function value of PDF.
     *
     * \param  value            multiplication factor
     * \return                  function value of PDF
     */
    JResultHesse& div(const double value)
    {
      static_cast<JResultDerivative<JResult_t>&>(*this).div(value);
      fpp /= value;

      return *this;
    }


    JResult_t fpp;    //!< second derivative
  };


  /**
   * Data structure for result including value, first derivative and integrals of function.
   *
   * This data structure contains the following data mambers:
   * <pre>
   *    JResultPDF::f   = function value;
   *    JResultPDF::fp  = first derivative;
   *    JResultPDF::v   = partial  integral;
   *    JResultPDF::V   = complete integral.
   * </pre>
   * The partial and complete integrals are used to evaluate the probability of the first hit. 
   *
   * This class implements the JMATH::JMath interface.
   */
  template<class JResult_t>
  struct JResultPDF : 
    public JMath< JResultPDF<JResult_t> >
  {

    typedef typename JLANG::JClass<JResult_t>::argument_type                     argument_type;


    /**
     * Default constructor.
     */
    JResultPDF() :
      f (JMATH::zero),
      fp(JMATH::zero),
      v (JMATH::zero),
      V (JMATH::zero)
    {}


    /**
     * Constructor.
     *
     * \param  f               function value
     * \param  fp              first derivative
     * \param  v               integral <xmin,x]
     * \param  V               integral <xmin,xmax>
     */
    JResultPDF(argument_type f,
	       argument_type fp,
	       argument_type v,
	       argument_type V) :
      f (f ),
      fp(fp),
      v (v),
      V (V)
    {}


    /**
     * Constructor.\n
     * This constructor refers to the result of a signal with a constant rate <tt>R</tt>
     * to produce an event occurring at the given moment <tt>x</tt> within the fixed range <tt>X</tt>.
     *
     * \param  R               rate
     * \param  x               abscissa value
     * \param  X               abscissa range
     */
    JResultPDF(argument_type R,
	       argument_type x,
	       const JRange<JResult_t>& X) :
      f (R),
      fp(JMATH::zero),
      v (R * (X.constrain(x)    - X.getLowerLimit())),
      V (R * (X.getUpperLimit() - X.getLowerLimit()))
    {}


    /**
     * Prefix unary minus for function value of PDF.
     *
     * \return                  function value of PDF
     */
    JResultPDF& negate()
    {
      f  = -f;
      fp = -fp;
      v  = -v;
      V  = -V;

      return *this;
    }


    /**
     * Addition operator for function value of PDF.
     *
     * \param  value            function value of PDF
     * \return                  function value of PDF
     */
    JResultPDF& add(const JResultPDF& value)
    {
      f  += value.f;
      fp += value.fp;
      v  += value.v;
      V  += value.V;

      return *this;
    }


    /**
     * Subtraction operator for function value of PDF.
     *
     * \param  value            function value of PDF
     * \return                  function value of PDF
     */
    JResultPDF& sub(const JResultPDF& value)
    {
      f  -= value.f;
      fp -= value.fp;
      v  -= value.v;
      V  -= value.V;

      return *this;
    }


    /**
     * Multiplication operator for function value of PDF.
     *
     * \param  value            multiplication factor
     * \return                  function value of PDF
     */
    JResultPDF& mul(const double value)
    {
      f  *= value;
      fp *= value;
      v  *= value;
      V  *= value;

      return *this;
    }


    /**
     * Division operator for function value of PDF.
     *
     * \param  value            division factor
     * \return                  function value of PDF
     */
    JResultPDF& div(const double value)
    {
      f  /= value;
      fp /= value;
      v  /= value;
      V  /= value;

      return *this;
    }


    /**
     * Get probability of first hit.\n
     * The probability is defined at the moment JResultPDF::f and JResultPDF::v have been evaluated
     * and it is normalised to the total interval corresponding to JResultPDF::V.
     *
     * \return                  probability
     */
    double getP() const
    {
      return exp(-v) * f / (1.0 - exp(-V));
    }


    /**
     * Get chi2 of first hit.\n
     * The chi2 corresponds to <tt>-log(P)</tt>, where <tt>P</tt> is the probability JResultPDF::f.
     *
     * \return                  chi2
     */
    double getChi2() const
    {
      return -log(getP());
    }


    /**
     * Get derivative of chi2 of first hit.
     *
     * \return                  derivative
     */
    double getDerivativeOfChi2() const
    {
      return fp/f - f;
    }


    JResult_t f;      //!< function value
    JResult_t fp;     //!< first derivative
    JResult_t v;      //!< integral <xmin,x]
    JResult_t V;      //!< integral <xmin,xmax>
  };


  /**
   * Data structure for result including value and <tt>N</tt> derivatives of function.
   *
   * This data structure contains the following data mambers:
   * <pre>
   *    JResultPolynome::y[0] = function value;
   *    JResultPolynome::y[i] = ith derivative; 
   *    JResultPolynome::y[N] = Nth derivative; 
   * </pre>
   *
   * This class implements the JMATH::JMath interface.
   */
  template<unsigned int N, class JResult_t>
  struct JResultPolynome :
    public JMath< JResultPolynome<N, JResult_t> >
  {

    typedef typename JLANG::JClass<JResult_t>::argument_type                     argument_type;

    static const int NUMBER_OF_POINTS = N + 1;        // number of points (starting at 0)


    /**
     * Default constructor.
     */
    JResultPolynome()
    {
      for (int i = 0; i != NUMBER_OF_POINTS; ++i) {
	y[i] = JMATH::zero;
      }
    }


    /**
     * Type conversion operator.
     *
     * \return                 result
     */
    operator JResultDerivative<JResult_t>() const
    {
      STATIC_CHECK(NUMBER_OF_POINTS >= 2);
      return JResultDerivative<JResult_t>(y[0], y[1]);
    }


    /**
     * Type conversion operator.
     *
     * \return                 result
     */
    operator JResultHesse<JResult_t>() const
    {
      STATIC_CHECK(NUMBER_OF_POINTS >= 3);
      return JResultHesse<JResult_t>(y[0], y[1], y[2]);
    }


    /**
     * Prefix unary minus for function value of PDF.
     *
     * \return                  function value of PDF
     */
    JResultPolynome& negate()
    {
      for (int i = 0; i != NUMBER_OF_POINTS; ++i) {
	y[i] = -y[i];
      }

      return *this;
    }


    /**
     * Addition operator for function value of PDF.
     *
     * \param  value            function value of PDF
     * \return                  function value of PDF
     */
    JResultPolynome& add(const JResultPolynome& value)
    {
      for (int i = 0; i != NUMBER_OF_POINTS; ++i) {
	y[i] += value.y[i];
      }

      return *this;
    }


    /**
     * Subtraction operator for function value of PDF.
     *
     * \param  value            function value of PDF
     * \return                  function value of PDF
     */
    JResultPolynome& sub(const JResultPolynome& value)
    {
      for (int i = 0; i != NUMBER_OF_POINTS; ++i) {
	y[i] -= value.y[i];
      }

      return *this;
    }


    /**
     * Multiplication operator for function value of PDF.
     *
     * \param  value            multiplication factor
     * \return                  function value of PDF
     */
    JResultPolynome& mul(const double value)
    {
      for (int i = 0; i != NUMBER_OF_POINTS; ++i) {
	y[i] *= value;
      }

      return *this;
    }


    /**
     * Division operator for function value of PDF.
     *
     * \param  value            multiplication factor
     * \return                  function value of PDF
     */
    JResultPolynome& div(const double value)
    {
      for (int i = 0; i != NUMBER_OF_POINTS; ++i) {
	y[i] /= value;
      }

      return *this;
    }


    /**
     * Function value.
     *
     * \param  x          abscissa value
     * \return            function value
     */
    double getValue(const double x) const
    {
      double w = 0.0;
      double z = 1.0;

      for (int i = 0; i != NUMBER_OF_POINTS; ++i, z *= x / i) {
        w += y[i] * z;
      }

      return w;
    }

    
    /**
     * Function value.
     *
     * \param  x          abscissa value
     * \return            function value
     */
    double operator()(const double x) const
    {
      return getValue(x);
    }


    JResult_t y[NUMBER_OF_POINTS];   //!< function and derivative values
  };


  /**
   * Auxiliary class to recursively evaluate to a result.
   */
  template<class T>
  struct JResultEvaluator
  {
    typedef T                                result_type;
    
    /**
     * Get function value.
     *
     * \return                  result
     */
    static result_type get_value(typename JLANG::JClass<T>::argument_type value)
    {
      return value;
    }
    
    /**
     * Get derivative value.
     *
     * \return                  result
     */
    static result_type get_derivative(typename JLANG::JClass<T>::argument_type value)
    {
      return value;
    }
    
    /**
     * Get partial integral value.
     *
     * \return                  result
     */
    static result_type get_integral(typename JLANG::JClass<T>::argument_type value)
    {
      return value;
    }
    
    /**
     * Get total integral value.
     *
     * \return                  result
     */
    static result_type get_total_integral(typename JLANG::JClass<T>::argument_type value)
    {
      return value;
    }
  };


  /**
   * Template specialisation of JResultEvaluator for JResultDerivative.
   */
  template<class T>
  struct JResultEvaluator< JResultDerivative<T> >
  {
    typedef typename JResultEvaluator<T>::result_type  result_type;

    /**
     * Get function value.
     *
     * \return                  result
     */
    static result_type get_value(const JResultDerivative<T>& value)
    {
      return JResultEvaluator<T>::get_value(value.f);
    }

    /**
     * Get derivative value.
     *
     * \return                  result
     */
    static result_type get_derivative(const JResultDerivative<T>& value)
    {
      return JResultEvaluator<T>::get_value(value.fp);
    }

    /**
     * Get partial integral value.
     *
     * \return                  result
     */
    static result_type get_integral(const JResultDerivative<T>& value)
    {
      return JMATH::zero;
    }
    
    /**
     * Get total integral value.
     *
     * \return                  result
     */
    static result_type get_total_integral(const JResultDerivative<T>& value)
    {
      return JMATH::zero;
    }
  };


  /**
   * Template specialisation of JResultEvaluator for JResultHesse.
   */
  template<class T>
  struct JResultEvaluator< JResultHesse<T> >
  {
    typedef typename JResultEvaluator<T>::result_type  result_type;
    
    /**
     * Get function value.
     *
     * \return                  result
     */
    static result_type get_value(const JResultHesse<T>& value)
    {
      return JResultEvaluator<T>::get_value(value.f);
    }
    
    /**
     * Get derivative value.
     *
     * \return                  result
     */
    static result_type get_derivative(const JResultHesse<T>& value)
    {
      return JResultEvaluator<T>::get_value(value.fp);
    }

    /**
     * Get partial integral value.
     *
     * \return                  result
     */
    static result_type get_integral(const JResultHesse<T>& value)
    {
      return JMATH::zero;
    }
    
    /**
     * Get total integral value.
     *
     * \return                  result
     */
    static result_type get_total_integral(const JResultHesse<T>& value)
    {
      return JMATH::zero;
    }
  };


  /**
   * Template specialisation of JResultEvaluator for JResultPDF.
   */
  template<class T>
  struct JResultEvaluator< JResultPDF<T> >
  {
    typedef typename JResultEvaluator<T>::result_type  result_type;
    
    /**
     * Get function value.
     *
     * \return                  result
     */
    static result_type get_value(const JResultPDF<T>& value)
    {
      return JResultEvaluator<T>::get_value(value.f);
    }

    /**
     * Get derivative value.
     *
     * \return                  result
     */
    static result_type get_derivative(const JResultPDF<T>& value)
    {
      return JResultEvaluator<T>::get_value(value.fp);
    }
    
    /**
     * Get partial integral value.
     *
     * \return                  result
     */
    static result_type get_integral(const JResultPDF<T>& value)
    {
      return JResultEvaluator<T>::get_value(value.v);
    }
    
    /**
     * Get total integral value.
     *
     * \return                  result
     */
    static result_type get_total_integral(const JResultPDF<T>& value)
    {
      return JResultEvaluator<T>::get_value(value.V);
    }
  };


  /**
   * Template specialisation of JResultEvaluator for JResultPolynome.
   */
  template<unsigned int N, class T>
  struct JResultEvaluator< JResultPolynome<N, T> >
  {
    typedef typename JResultEvaluator<T>::result_type  result_type;
    
    /**
     * Get function value.
     *
     * \return                  result
     */
    static result_type get_value(const JResultPolynome<N, T>& value)
    {
      return JResultEvaluator<T>::get_value(value.y[0]);
    }

    /**
     * Get derivative value.
     *
     * \return                  result
     */
    static result_type get_derivative(const JResultPolynome<N, T>& value)
    {
      return JResultEvaluator<T>::get_value(value.y[1]);
    }
    
    /**
     * Get partial integral value.
     *
     * \return                  result
     */
    static result_type get_integral(const JResultPolynome<N, T>& value)
    {
      return JMATH::zero;
    }
    
    /**
     * Get total integral value.
     *
     * \return                  result
     */
    static result_type get_total_integral(const JResultPolynome<N, T>& value)
    {
      return JMATH::zero;
    }
  };


  /**
   * Template specialisation of JResultEvaluator for JResultPolynome.
   */
  template<class T>
  struct JResultEvaluator< JResultPolynome<0, T> >
  {
    typedef typename JResultEvaluator<T>::result_type  result_type;
    
    /**
     * Get function value.
     *
     * \return                  result
     */
    static result_type get_value(const JResultPolynome<0, T>& value)
    {
      return JResultEvaluator<T>::get_value(value.y[0]);
    }

    /**
     * Get derivative value.
     *
     * \return                  result
     */
    static result_type get_derivative(const JResultPolynome<0, T>& value)
    {
      return JMATH::zero;
    }
    
    /**
     * Get partial integral value.
     *
     * \return                  result
     */
    static result_type get_integral(const JResultPolynome<0, T>& value)
    {
      return JMATH::zero;
    }
    
    /**
     * Get total integral value.
     *
     * \return                  result
     */
    static result_type get_total_integral(typename JLANG::JClass<T>::argument_type value)
    {
      return JMATH::zero;
    }
  };

  
  /**
   * Helper method to recursively evaluate a to function value.
   *
   * \param  value            result
   * \return                  function value
   */
  template<class JResult_t>
  inline typename JResultEvaluator<JResult_t>::result_type get_value(const JResult_t& value)
  { 
    return JResultEvaluator<JResult_t>::get_value(value);
  }

  
  /**
   * Helper method to convert function value to derivative value.
   *
   * \param  value            result
   * \return                  derivative value
   */
  template<class JResult_t>
  inline typename JResultEvaluator<JResult_t>::result_type get_derivative(const JResult_t& value)
  { 
    return JResultEvaluator<JResult_t>::get_derivative(value);
  }

  
  /**
   * Helper method to convert function value to partial integral value.
   *
   * \param  value            result
   * \return                  partial integral value
   */
  template<class JResult_t>
  inline typename JResultEvaluator<JResult_t>::result_type get_integral(const JResult_t& value)
  { 
    return JResultEvaluator<JResult_t>::get_integral(value);
  }

  
  /**
   * Helper method to convert function value to total integral value.
   *
   * \param  value            result
   * \return                  total integral value
   */
  template<class JResult_t>
  inline typename JResultEvaluator<JResult_t>::result_type get_total_integral(const JResult_t& value)
  { 
    return JResultEvaluator<JResult_t>::get_total_integral(value);
  }
}

#endif
