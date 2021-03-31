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
  template<bool>
  struct JAssert;

  /**
   * Implementation of valid assertion.
   */
  template<>
  struct JAssert<true> 
  {
    static const bool value = true;
  };
}

#define STATIC_CHECK(expr) { JLANG::JAssert<expr>(); } 

#endif
