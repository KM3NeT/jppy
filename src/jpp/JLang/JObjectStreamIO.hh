#ifndef __JLANG__JOBJECTSTREAMIO__
#define __JLANG__JOBJECTSTREAMIO__

#include <fstream>

#include "JLang/JObjectIO.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * Auxiliary base class for storing and loading a single object to and from an ASCII file, respectively. 
   * The implementation of this functionality is based on a static cast of 
   * the given template parameter (known as "curiously recurring template pattern").
   */
  template<class T>
  struct JObjectStreamIO {
    /**
     * Load from input file.
     *
     * \param  file_name               file name
     */
    void load(const char* file_name)
    {
      JLANG::load<std::ifstream>(file_name, static_cast<T&>(*this));
    }


    /**
     * Store to output file.
     *
     * \param  file_name               file name
     */
    void store(const char* file_name) const
    {
      JLANG::store<std::ofstream>(file_name, static_cast<const T&>(*this));
    }
  };
}

#endif
