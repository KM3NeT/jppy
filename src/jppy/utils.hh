#ifndef __JPPY_UTILS__
#define __JPPY_UTILS__

#include <iostream>
#include <sstream>
#include <string>

namespace UTILS {

  /**
   * Auxiliary function for defining the representation of a class object.\n
   * The stream output operator must be defined for the given class.
   *
   * \param  object           class object
   */
  template<class T>
  std::string get_representation(const T& object) {

    using namespace std;

    stringstream stream;
    stream << object;
    
    return stream.str();
  }
}

#endif
