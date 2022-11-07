#ifndef __JEEP__JCOMPARISONTOOLKIT__
#define __JEEP__JCOMPARISONTOOLKIT__

#include "JLang/JComparisonAvailable.hh"
#include "JLang/JBool.hh"


/**
 * \author mdejong
 */

namespace JEEP {}
namespace JPP { using namespace JEEP; }

namespace JEEP {

  using JLANG::JComparisonAvailable;
  using JLANG::JBool;

    

  /**
   * Comparison of comparable objects.
   *
   * \param  first         first  object
   * \param  second        second object
   * \return               true if objects are equal; else false
   */
  template<class T>
  inline bool compareObjects(const T& first,
			     const T& second,
			     JBool<true>)
  {
    return first == second;
  }
    

  /**
   * Comparison of uncomparable objects.
   *
   * \param  first         first  object
   * \param  second        second object
   * \return               false
   */
  template<class T>
  inline bool compareObjects(const T& first,
			     const T& second,
			     JBool<false>)
  {
    return false;
  }
    

  /**
   * Comparison of objects.
   *
   * \param  first         first  object
   * \param  second        second object
   * \return               true if objects are comparable and equal; else false
   */
  template<class T>
  inline bool compareObjects(const T& first,
			     const T& second)
  { 
    return compareObjects(first, second, JBool<JComparisonAvailable<T>::has_eq>());
  }
}

#endif
