#ifndef __JLANG__JTYPE__
#define __JLANG__JTYPE__


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * Auxiliary class for a type holder.
   * This class can be used to transfer a template class to a method argument.
   */
  template<class T>
  struct JType {

    typedef T  data_type;
  };


  /**
   * Get type.
   *
   * \return             type
   */
  template<class T>
  inline JType<T> getType()
  {
    return JType<T>();
  }
}

#endif
