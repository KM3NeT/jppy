#ifndef __JLANG__JVOID__
#define __JLANG__JVOID__


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Auxiliary class for void type definition.
   * This class can be used to evaluate the validity of a type definition.
   */
  template<class T>
  struct JVoid 
  { 
    typedef void type;
  };
}

#endif
