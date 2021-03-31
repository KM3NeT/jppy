#ifndef __JMATH__JZERO__
#define __JMATH__JZERO__

/**
 * \file
 *
 * Definition of zero value for any class.
 * \author mdejong
 */
namespace JMATH {}
namespace JPP { using namespace JMATH; }

namespace JMATH {

  /**
   * Get zero value for a given data type.
   *
   * The default implementation of this method returns an object which 
   * is created with the default constructor.
   * This method should be specialised if this value does not correspond
   * to the equivalent of a zero result.
   *
   * \return                     zero
   */
  template<class T>
  inline T getZero() 
  {
    return T();
  }


  /**
   * Get zero value for <tt>bool</tt>.
   *
   * \return                     false
   */
  template<> inline bool getZero<bool>()
  {
    return false;
  }


  /**
   * Get zero value for <tt>float</tt>.
   *
   * \return                     zero
   */
  template<> inline float getZero<float>()
  { 
    return float(0.0);
  }


  /**
   * Get zero value for <tt>double</tt>.
   *
   * \return                     zero
   */
  template<>
  inline double getZero<double>() 
  {
    return double(0.0);
  }


  /**
   * Get zero value for <tt>long double</tt>.
   *
   * \return                     zero
   */
  template<>
  inline long double getZero<long double>() 
  {
    return (long double)(0.0);
  }


  /**
   * Auxiliary class to assign zero value.
   */
  struct JZero {
    /** 
     * Default constructor.
     */
    JZero()
    {}


    /**
     * Type conversion operator.
     *
     * \return                     zero
     */
    template<class T>
    operator T() const
    {
      return getZero<T>();
    }
  };


  /**
   * Function object to assign zero value.
   */
  static const JZero zero;
}

#endif
