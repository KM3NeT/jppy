#ifndef __JMATH__JMATH__
#define __JMATH__JMATH__

#include <cmath>
#include <iterator>

#include "JLang/JNullType.hh"
#include "JLang/JClass.hh"
#include "JLang/JBool.hh"
#include "JLang/JVectorize.hh"
#include "JLang/JException.hh"
#include "JMath/JZero.hh"
#include "JMath/JCalculator.hh"


/**
 * \file
 * 
 * Base class for data structures with artithmetic capabilities.
 * \author mdejong
 */
namespace JMATH {}
namespace JPP { using namespace JMATH; }

namespace JGEOMETRY3D { class JQuaternion3D; }

namespace JMATH {

  using JLANG::JNullType;
  using JLANG::array_type;
  using JLANG::JDivisionByZero;
  
  
  /**
   * Power \f$ x^{y} \f$.
   *
   * \param  x              value
   * \param  y              power
   * \return                result
   */
  template<class T>
  inline T pow(const T& x, const double y);


  /**
   * Auxiliary class to hide data type specific methods.
   */
  struct JMath_t {
    /**
     * Friend declaration of global method.
     */
    template<class T>
    friend T JMATH::pow(const T&, const double y);

  private:
    /**
     * Power \f$ x^{y} \f$.
     * This method corresponds to primitive data types.
     *
     * \param  x              value
     * \param  y              power
     * \param  option         true
     * \return                result
     */
    template<class T>
    static inline T pow(const T& x, const double y, const JLANG::JBool<true> option)
    {
      return std::pow(x, y);
    }


    /**
     * Power \f$ x^{y} \f$.
     *
     * This method corresponds to non-primitive data types.
     *
     * \param  x              value
     * \param  y              power
     * \param  option         false
     * \return                result
     */
    template<class T>
    static inline T pow(const T& x, const double y, const JLANG::JBool<false> option)
    {
      return T(x).pow(y);
    }
  };


  /**
   * Power \f$ x^{y} \f$.
   *
   * \param  x              value
   * \param  y              power
   * \return                result
   */
  template<class T>
  inline T pow(const T& x, const double y)
  {
    using namespace JPP;

    return JMath_t::pow(x, y, JBool<JClass<T>::is_primitive>());
  }


  /**
   * Auxiliary base class for aritmetic operations of derived class types.
   */
  template<class JFirst_t, class JSecond_t = JNullType>
  struct JMath;

    
  /**
   * Template base class for data structures with arithmetic capabilities.
   *
   * This class provides for the operators <tt> - += -= *= /= + - * / </tt>.\n
   * To this end, the template parameter should privide for the corresponding member methods:
   * <pre>
   *       T& negate();
   *       T& add(const T& object);
   *       T& sub(const T& object);
   *       T& mul(const double factor);
   *       T& div(const double factor);
   * </pre>
   *
   * This class also provides for the object multiplication operators <tt>*= *</tt>.\n
   * To this end, the template parameter should then also provide for the member method:
   * <pre>
   *       T& mul(const T&, const T&);
   * </pre>
   *
   * This class adds interpolation functionality.\n
   * This class uses in-class friend operators (see Barton-Nackman trick).
   */
  template<class T>
  struct JMath<T, JNullType> {
    /**
     * Affirm operator.
     *
     * \param  object         this object
     * \return                affirmed object
     */
    friend T operator+(const T& object) 
    { 
      return T(object);
    }

    
    /**
     * Negate operator.
     *
     * \param  object         this object
     * \return                negated object
     */
    friend T operator-(const T& object) 
    { 
      return T(object).negate();
    }

    
    /**
     * Add object.
     *
     * \param  object         this object
     * \param  value          value
     * \return                this object
     */
    friend T& operator+=(T& object, const T& value)
    { 
      return object.add(value);
    }

    
    /**
     * Subtract object.
     *
     * \param  object         this object
     * \param  value          value
     * \return                this object
     */
    friend T& operator-=(T& object, const T& value)
    { 
      return object.sub(value);
    }


    /**
     * Scale object.
     *
     * \param  object         this object
     * \param  factor         factor
     * \return                this object
     */
    friend T& operator*=(T& object, const double factor)
    { 
      return object.mul(factor);
    }


    /**
     * Scale object.
     *
     * \param  object         this object
     * \param  factor         factor
     * \return                this object
     */
    friend T& operator/=(T& object, const double factor)
    { 
      return object.div(factor);
    }

    
    /**
     * Add objects.
     *
     * \param  first          first  object
     * \param  second         second object
     * \return                result object
     */
    friend T operator+(const T& first, const T& second)
    { 
      return JCalculator<T>::calculator.set(first).add(second);
    }

    
    /**
     * Subtract objects.
     *
     * \param  first          first  object
     * \param  second         second object
     * \return                result object
     */
    friend T operator-(const T& first, const T& second)
    { 
      return JCalculator<T>::calculator.set(first).sub(second);
    }


    /**
     * Scale object.
     *
     * \param  object         object
     * \param  factor         factor
     * \return                object
     */
    friend T operator*(const T& object, const double factor)
    { 
      return JCalculator<T>::calculator.set(object).mul(factor);
    }


    /**
     * Scale object.
     *
     * \param  factor         factor
     * \param  object         object
     * \return                object
     */
    friend T operator*(const double factor, const T& object)
    { 
      return JCalculator<T>::calculator.set(object).mul(factor);
    }


    /**
     * Scale object.
     *
     * \param  object         object
     * \param  factor         factor
     * \return                object
     */
    friend T operator/(const T& object, const double factor)
    { 
      return JCalculator<T>::calculator.set(object).div(factor);
    }


    /**
     * Multiply with object.
     *
     * \param  object         object
     * \return                result object
     */
    T& mul(const T& object)
    {
      return static_cast<T&>(*this) = JCalculator<T>::calculator.mul(static_cast<const T&>(*this), object);
    }


    /**
     * Multiply with object.
     *
     * \param  first          first  object
     * \param  second         second object
     * \return                result object
     */
    friend T& operator*=(T& first, const T& second)
    {
      return static_cast<JMath<T>&>(first).mul(second);
    }

    
    /**
     * Multiply objects.
     *
     * \param  first          first  object
     * \param  second         second object
     * \return                calculator
     */
    friend const JCalculator<T, 1>& operator*(const T& first, const T& second)
    { 
      JCalculator<T, 1>::calculator.mul(first, second);

      return JCalculator<T, 1>::calculator;
    }


    /**
     * Interpolation between objects.
     * The result is equal to <tt>*this = (1 - alpha) * (*this) + (alpha) * (object)</tt>.
     *
     * \param  object         object
     * \param  alpha          interpolation factor <tt>[0, 1]</tt>
     * \return                this object
     */
    T& interpolate(const T& object, const double alpha)
    {
      static_cast<T*>(this)->mul(1.0 - alpha);
      static_cast<T*>(this)->add(T(object).mul(alpha));

      return static_cast<T&>(*this);
    }
  };

  
  /**
   * Specialisation of JMath for two data types.
   *
   * This class provides for the object multiplication operators <tt>*= *</tt>.
   * The template parameter should then have the member method:
   * <pre>
   *       JFirst_t& mul(const JFirst_t&, const JSecond_t&);
   * </pre>
   * where <tt>JFirst_t</tt> and <tt>JSecond_t</tt> refer to the first and second template parameter, respectively.
   *
   * This class uses in-class friend operators (see Barton-Nackman trick).
   */
  template<class JFirst_t, class JSecond_t>
  struct JMath
  {
    /**
     * Multiply with object.
     *
     * \param  object         object
     * \return                result object
     */
    JFirst_t& mul(const JSecond_t& object)
    {
      return static_cast<JFirst_t&>(*this) = JCalculator<JFirst_t>::calculator.mul(static_cast<const JFirst_t&>(*this), object);
    }


    /**
     * Multiply with object.
     *
     * \param  first          first  object
     * \param  second         second object
     * \return                result object
     */
    friend JFirst_t& operator*=(JFirst_t& first, const JSecond_t& second)
    {
      return first.mul(second);
    }

    
    /**
     * Multiply objects.
     *
     * \param  first          first  object
     * \param  second         second object
     * \return                result object
     */
    friend JFirst_t operator*(const JFirst_t& first, const JSecond_t& second)
    { 
      return JFirst_t(first).mul(second);
    }
  };


  /**
   * Interpolation between objects.
   * The result is equal to <tt>result = (1 - alpha) * (first) + (alpha) * (second)</tt>.
   *
   * \param  first          first  object
   * \param  second         second object
   * \param  alpha          interpolation factor <tt>[0, 1]</tt>
   * \return                result
   */
  template<class T>
  inline T interpolate(const T&     first,
		       const T&     second, 
		       const double alpha)
  {
    return T(first).interpolate(second, alpha);
  }


  /**
   * Auxiliary class to determine average of set of values.
   */
  template<class JValue_t>
  struct JAverage {
    /**
     * Default constructor.
     */
    JAverage() :
      value (getZero<JValue_t>()),
      weight(0.0)
    {}

    
    /**
     * Constructor.
     *
     * \param  __begin        begin of data
     * \param  __end          end   of data
     */
    template<class T>
    JAverage(T __begin, T __end) :
      value (getZero<JValue_t>()),
      weight(0.0)
    {
      for (T i = __begin; i != __end; ++i) {
	value  += (*i);
	weight += 1.0;
      }
    }


    /**
     * Reset.
     */
    void reset()
    {
      this->value  = getZero<JValue_t>();
      this->weight = 0.0;
    }


    /**
     * Type conversion operator.
     *
     * \return                value
     */
    operator JValue_t () const
    {
      if (weight != 0.0)
	return value * (1.0 / weight);
      else
	THROW(JDivisionByZero, "Invalid weight.");
    }


    /**
     * Put value.
     *
     * \param  value         value
     * \param  w             weight
     */
    void put(const JValue_t& value, const double w = 1.0)
    {
      this->value  += value;
      this->weight += w;
    }

  private:
    JValue_t value;
    double   weight;
  };


  /**
   * Template definition so that compiler error is generated if implementation is missing (see JEigen3D.hh).
   */
  template<>
  struct JAverage<JGEOMETRY3D::JQuaternion3D>;


  /**
   * Get average.
   *
   * \param  __begin        begin of data
   * \param  __end          end   of data
   * \return                average value
   */
  template<class T>
  typename std::iterator_traits<T>::value_type getAverage(T __begin, T __end)
  {
    typedef typename std::iterator_traits<T>::value_type  value_type;

    return JAverage<value_type>(__begin, __end);
  }


  /**
   * Get average.
   *
   * \param  array          c-array of values
   * \return                average value
   */
  template<class JValue_t, size_t N>
  inline JValue_t getAverage(const JValue_t (&array)[N])
  {
    typedef JValue_t  value_type;
    
    return JAverage<value_type>((const value_type*) array, (const value_type*) array  +  N);
  }


  /**
   * Get average.
   *
   * \param  buffer         input data
   * \return                average value
   */
  template<class JElement_t, class JAllocator_t>
  JElement_t getAverage(const array_type<JElement_t, JAllocator_t>& buffer)
  {
    return JAverage<JElement_t>(buffer.begin(), buffer.end());
  }


  /**
   * Get average.
   *
   * \param  __begin        begin of data
   * \param  __end          end   of data
   * \param  value          default value
   * \return                average value
   */
  template<class T>
  typename std::iterator_traits<T>::value_type getAverage(T __begin, T __end, typename std::iterator_traits<T>::value_type value)
  {
    try {
      return getAverage(__begin, __end);
    }
    catch(const std::exception&) {
      return value;
    }
  }


  /**
   * Get average.
   *
   * \param  array          c-array of values
   * \param  value          default value
   * \return                average value
   */
  template<class JValue_t, size_t N>
  inline JValue_t getAverage(const JValue_t (&array)[N], typename JLANG::JClass<JValue_t>::argument_type value)
  {
    try {
      return getAverage(array);
    }
    catch(const std::exception&) {
      return value;
    }
  }


  /**
   * Get average.
   *
   * \param  buffer         input data
   * \param  value          default value
   * \return                average value
   */
  template<class JElement_t, class JAllocator_t>
  JElement_t getAverage(const array_type<JElement_t, JAllocator_t>& buffer, typename JLANG::JClass<JElement_t>::argument_type value)
  {
    try {
      return getAverage(buffer);
    }
    catch(const std::exception&) {
      return value;
    }
  }
}

#endif
