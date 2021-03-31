#ifndef __JMATH__JCONSTANTS__
#define __JMATH__JCONSTANTS__

#include <math.h>


/**
 * \file
 * Mathematical constants. 
 * \author mdejong
 */
namespace JMATH {}
namespace JPP { using namespace JMATH; }

namespace JMATH {

  /**
   * Mathematical constants.
   */
  static const double PI                        = acos(-1.0);           //!< pi
  static const double EULER                     = 0.577215664901533;    //!< Euler number

  /**
   * Computing quantities.
   */
  static const long long int KILOBYTE   = 1024;                        //! Number of bytes in a kilo-byte
  static const long long int MEGABYTE   = KILOBYTE*KILOBYTE;           //! Number of bytes in a mega-byte
  static const long long int GIGABYTE   = MEGABYTE*KILOBYTE;           //! Number of bytes in a giga-byte
}

#endif
