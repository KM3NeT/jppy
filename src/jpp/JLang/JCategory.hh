#ifndef __JLANG__JCATEGORY
#define __JLANG__JCATEGORY


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * Auxiliary class to define value, reference and pointer types for given data type and category.
   */
  template<class T, bool is_constant>
  struct JCategory;

  /**
   * Specialisation of JCategory for constant (i.e.\ non-modifiable) data type.
   */
  template<class T>
  struct JCategory<T, true> {
    typedef const T       value_type;
    typedef const T&      reference_type;
    typedef const T*      pointer_type;
  };

  /**
   * Specialisation of JCategory for modifiable (i.e.\ non-constant) data type.
   */
  template<class T>
  struct JCategory<T, false> {
    typedef T             value_type;
    typedef T&            reference_type;
    typedef T*            pointer_type;
  };
}

#endif
