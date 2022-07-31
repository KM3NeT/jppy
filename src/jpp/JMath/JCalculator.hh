#ifndef __JMATH__JCALCULATOR__
#define __JMATH__JCALCULATOR__


/**
 * \author mdejong
 */

namespace JMATH {}
namespace JPP { using namespace JMATH; }

namespace JMATH {

  /**
   * Auxiliary class for arithmetic operations on objects.
   */  
  template<class T, int N = 0>
  struct JCalculator :
    public T                         // object
  {
    /**
     * Set calculator value.
     *
     * \param  value       value
     * \return             this calculator
     */
    JCalculator& set(const T& value) 
    {
      static_cast<T&>(*this) = value;

      return *this;
    }

    static JCalculator calculator;   // calculator
  };

  
  /**
   * Calculator.
   */
  template<class T, int N>
  JCalculator<T,N> JCalculator<T,N>::calculator;
  
  
  /** 
   * Product evaluation of objects.
   *
   * \param  first       first  object
   * \param  second      second object
   * \return             calculator
   */
  template<class T>
  inline const JCalculator<T, 1>& operator*(const T& first, const T& second)
  {
    JCalculator<T, 1>::calculator.mul(first, second);

    return JCalculator<T, 1>::calculator;
  }


  /**
   * Recursive product evaluation of objects.
   *
   * \param  first       first  object
   * \param  second      second object
   * \return             calculator
   */
  template<class T, int N>
  inline const JCalculator<T, N+1>& operator*(const T& first, const JCalculator<T, N>& second)
  {
    JCalculator<T, N+1>::calculator.mul(first, second);

    return JCalculator<T, N+1>::calculator;
  }


  /**
   * Recursive product evaluation of objects.
   *
   * \param  first       first  object
   * \param  second      second object
   * \return             calculator
   */
  template<class T, int N>
  inline const JCalculator<T, N+1>& operator*(const JCalculator<T, N>& first, const T& second)
  {
    JCalculator<T, N+1>::calculator.mul(first, second);

    return JCalculator<T, N+1>::calculator;
  }
}

#endif
