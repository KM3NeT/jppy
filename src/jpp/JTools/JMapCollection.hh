#ifndef __JTOOLS__JMAPCOLLECTION__
#define __JTOOLS__JMAPCOLLECTION__

/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  /**
   * Template class to define the corresponding JCollection for a given template JMap.
   *
   * This class should be specialised for each map type so that the implementation
   * of an interpolation method based on a collection can directly be transferred to a map.
   */
  template<template<class, class, class> class JMap_t>
  struct JMapCollection;
}

#endif
