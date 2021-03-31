#ifndef __JTOOLS__JMAPLIST__
#define __JTOOLS__JMAPLIST__

#include "JLang/JNullType.hh"
#include "JTools/JDistance.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JLANG::JNullType;
  

  /**
   * Map list.
   */
  template<template<class, class, class> class JHead_t, class JTail_t = JNullType>
  struct JMapList
  {
    typedef JMapList<JHead_t, typename JTail_t::head_list>   head_list;   //!< first N-1 maps
    typedef typename JTail_t::tail_type                      tail_type;   //!< last  map
  };


  /**
   * Terminator class of map list.
   */
  template<template<class, class, class> class JHead_t>
  struct JMapList<JHead_t, JNullType>
  {
    typedef JNullType                                        head_list;   //!< first N-1 maps
    typedef JMapList<JHead_t>                                tail_type;   //!< last  map
  };


  /**
   * Length of map list.
   */
  template<class JMaplist_t> struct JMapLength {};


  /**
   * Recursive length of map list.
   */
  template<template<class, class, class> class JHead_t, class JTail_t>
  struct JMapLength< JMapList<JHead_t, JTail_t> >
  {
    enum { value = 1 + JMapLength<JTail_t>::value };
  };


  /**
   * Terminator class of length of map list.
   */
  template<template<class, class, class> class JHead_t>
  struct JMapLength< JMapList<JHead_t, JNullType> >
  {
    enum { value = 1 };
  };


  /**
   * Auxiliary class for no map definition.
   * This class can be used to terminate a map list, define a default template argument, etc.
   */
  template<class JKey_t,
	   class JValue_t,
	   class JDistance_t = JDistance<JKey_t> >
  struct JNullMap {};

  
  /**
   * Auxiliary class for recursive map list generation.
   * This class accepts up to 26 map types.
   */
  template<template<class, class, class> class A = JNullMap,
	   template<class, class, class> class B = JNullMap,
	   template<class, class, class> class C = JNullMap,
	   template<class, class, class> class D = JNullMap,
	   template<class, class, class> class E = JNullMap,
	   template<class, class, class> class F = JNullMap,
	   template<class, class, class> class G = JNullMap,
	   template<class, class, class> class H = JNullMap,
	   template<class, class, class> class I = JNullMap,
	   template<class, class, class> class J = JNullMap,
	   template<class, class, class> class K = JNullMap,
	   template<class, class, class> class L = JNullMap,
	   template<class, class, class> class M = JNullMap,
	   template<class, class, class> class N = JNullMap,
	   template<class, class, class> class O = JNullMap,
	   template<class, class, class> class P = JNullMap,
	   template<class, class, class> class Q = JNullMap,
	   template<class, class, class> class R = JNullMap,
	   template<class, class, class> class S = JNullMap,
	   template<class, class, class> class T = JNullMap,
	   template<class, class, class> class U = JNullMap,
	   template<class, class, class> class V = JNullMap,
	   template<class, class, class> class W = JNullMap,
	   template<class, class, class> class X = JNullMap,
	   template<class, class, class> class Y = JNullMap,
	   template<class, class, class> class Z = JNullMap>
  struct JMAPLIST
  {
    typedef JMapList<A, typename JMAPLIST<B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z>::maplist>  maplist;  //!< Map list
  };

  
  /**
   * Template specialisation to terminate recursive map list generation.
   */
  template<template<class, class, class> class A>
  struct JMAPLIST<A,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap,
		  JNullMap>
  {
    typedef JMapList<A>  maplist;  //!< Map list
  };
}

#endif
