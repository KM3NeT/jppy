#ifndef __JTOOLS__JASSEMBLER__
#define __JTOOLS__JASSEMBLER__

#include "JLang/JVoid.hh"

/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JLANG::JVoid;
  
  
  /**
   * Auxiliary class to check whether given template is a collection,
   * i.e.\ has a defined type <tt>collection_type</tt>.
   */
  template<class T, class JType_t = void>
  struct JAssembler
  {
    static const bool is_collection = false;
  };

  
  /**
   * Template specialisation of class JAssembler for classes
   * with a defined type <tt>collection_type</tt>.
   */
  template<class T>
  struct JAssembler<T, typename JVoid<typename T::collection_type>::type>
  {
    static const bool is_collection = true;
  };
}

#endif
