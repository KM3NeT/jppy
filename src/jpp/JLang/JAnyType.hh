#ifndef __JLANG__JANYTYPE__
#define __JLANG__JANYTYPE__


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Auxiliary class for any type definition.
   * This class can be used to test the validity of an expression through type conversion.
   */
  struct JAnyType {
    /**
     * Type conversion operation.
     *
     * \param  any_type         any type
     */
    template<class T>
    JAnyType(const T& any_type)
    {}
  };
}

#endif
