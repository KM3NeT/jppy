#ifndef __JLANG__JNULLTYPE__
#define __JLANG__JNULLTYPE__


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Auxiliary class for no type definition.
   * This class can be used to terminate a type list, define a default template argument, etc.
   */
  struct JNullType {};
}

#endif
