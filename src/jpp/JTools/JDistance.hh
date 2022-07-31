#ifndef __JTOOLS__JDISTANCE__
#define __JTOOLS__JDISTANCE__

#include <limits>

#include "JLang/JClass.hh"

/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  /**
   * Template class for distance evaluation.
   *
   * This class should be specialised for data types when the distance
   * between two values is not equal to the arithmetic minus operation.
   */
  template<class JAbscissa_t>
  struct JDistance {

    typedef typename JLANG::JClass<JAbscissa_t>::argument_type    argument_type;


    /**
     * Get distance between values.
     *
     * The distance should be negative, zero or positive if the first value
     * is respectively larger, equal or smaller compared to the second value.
     *
     * \param  first           first  value
     * \param  second          second value
     * \return                 distance
     */
    inline double operator()(argument_type first, argument_type second) const
    {
      return second - first;
    }

    static double precision;
  };


  /**
   * Default precision.
   */
  template<class JAbscissa_t>
  double JDistance<JAbscissa_t>::precision = std::numeric_limits<double>::min();
}

#endif
