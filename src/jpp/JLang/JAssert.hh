#ifndef __JLANG__JASSERT__
#define __JLANG__JASSERT__

/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * Generation of compiler error.
   */
  template<bool, class T = void>
  struct JAssert;

  /**
   * Implementation of valid assertion.
   */
  template<class T>
  struct JAssert<true, T> 
  {
    static const bool value = true;

    typedef T type;
  };
}

#define STATIC_CHECK(expr) { JLANG::JAssert<expr>(); } 

#endif
