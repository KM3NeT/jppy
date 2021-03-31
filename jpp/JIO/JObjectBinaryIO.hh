#ifndef __JIO__JOBJECTBINARYIO__
#define __JIO__JOBJECTBINARYIO__

#include "JLang/JObjectIO.hh"
#include "JIO/JFileStreamIO.hh"


/**
 * \author mdejong
 */

namespace JIO {}
namespace JPP { using namespace JIO; }

namespace JIO {
  
  /**
   * Auxiliary base class for storing and loading a single object to and from a binary file, respectively. 
   * The implementation of this functionality is based on a static cast of 
   * the given template parameter (known as "curiously recurring template pattern").
   */
  template<class T>
  struct JObjectBinaryIO {
    /**
     * Load from input file.
     *
     * \param  file_name               file name
     */
    void load(const char* file_name)
    {
      JLANG::load<JFileStreamReader>(file_name, static_cast<T&>(*this));
    }


    /**
     * Store to output file.
     *
     * \param  file_name               file name
     */
    void store(const char* file_name) const
    {
      JLANG::store<JFileStreamWriter>(file_name, static_cast<const T&>(*this));
    }
  };
}

#endif
