#ifndef __JLANG__JSTDTYPES__
#define __JLANG__JSTDTYPES__

/**
 * \file
 *
 * Forward declarations of STD containers.
 * \author mdejong
 */

namespace std {
  template<class JElement_t, class JAllocator_t>                                    class vector;
  template<class JElement_t, class JComparator_t, class JAllocator_t>               class set;
  template<class JElement_t, class JComparator_t, class JAllocator_t>               class multiset;
  template<class JFirst_t, class JSecond_t>                                         class pair;
  template<class JKey_t, class JValue_t, class JComparator_t, class JAllocator_t>   class map;
  template<class JKey_t, class JValue_t, class JComparator_t, class JAllocator_t>   class multimap;
  template<class Category, class T, class Distance, class Pointer, class Reference> struct iterator;
#ifdef _GLIBCXX_USE_CXX11_ABI
  inline namespace __cxx11 {
    template<class JElement_t, class JAllocator_t>                                  class list;
  }
#else
  template<class JElement_t, class JAllocator_t>                                    class list;
#endif 
}

#endif

