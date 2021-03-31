#ifndef __JMATH__JLIMITS__
#define __JMATH__JLIMITS__

#include <limits>

/**
 * \file
 *
 * Definition of minimum and maximum values for any class.
 * \author mdejong
 */
namespace JMATH {}
namespace JPP { using namespace JMATH; }

namespace JMATH {

  /**
   * Auxiliary class for minimum and maximum values for any class.
   */
  template<class T, bool __is_specialized__ = std::numeric_limits<T>::is_specialized>
  struct JLimits;


  /**
   * Template spacialisation of JMATH::JLimits for numerical values.
   */
  template<class T>
  struct JLimits<T, true> {
    /**
     * Get minimum possible value.
     *
     * \return                  minimum possible value
     */
    static T min()
    {
      return std::numeric_limits<T>::min();
    }


    /**
     * Get maximum possible value.
     *
     * \return                  maximum possible value
     */
    static T max()
    {
      return std::numeric_limits<T>::max();
    }

    static const bool is_specialized = true; 
  };


  /**
   * Template spacialisation of JMATH::JRandom for other data types.
   *
   * The given template class should provide for the methods:
   * <pre>
   *      static T %min();
   *      static T %max();
   * </pre>
   */
  template<class T>
  struct JLimits<T, false> {
    /**
     * Get minimum possible value.
     *
     * \return                  minimum possible value
     */
    static T min()
    {
      return T::min();
    }


    /**
     * Get maximum possible value.
     *
     * \return                  maximum possible value
     */
    static T max()
    {
      return T::max();
    }

    static const bool is_specialized = false; 
  };

  
  /**
   * Get minimum possible value.
   *
   * \return                  minimum possible value
   */
  template<>
  inline float JLimits<float, true>::min()
  {
    return std::numeric_limits<float>::lowest();
  }

  
  /**
   * Get minimum possible value.
   *
   * \return                  minimum possible value
   */
  template<>
  inline double JLimits<double, true>::min()
  {
    return std::numeric_limits<double>::lowest();
  }

  
  /**
   * Get minimum possible value.
   *
   * \return                  minimum possible value
   */
  template<>
  inline long double JLimits<long double, true>::min()
  {
    return std::numeric_limits<long double>::lowest();
  }
}

#endif
