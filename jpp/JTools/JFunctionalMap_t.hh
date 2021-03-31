#ifndef __JFUNCTIONALMAP_T__
#define __JFUNCTIONALMAP_T__

#include "JTools/JDistance.hh"
#include "JTools/JMap.hh"
#include "JTools/JGridMap.hh"
#include "JTools/JPolint.hh"
#include "JTools/JSpline.hh"
#include "JTools/JResult.hh"


/**
 * \file
 *
 * Various implementations of functional maps.
 * \author mdejong
 */
namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {


  /**
   * Type definition of a spline interpolation based on a JMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JSplineFunctionalMap :
    public JSplineMap<JKey_t, JValue_t, JMap, typename JResultType<JValue_t>::result_type, JDistance_t>
  {};


  /**
   * Type definition of a spline interpolation based on a JGridMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JSplineFunctionalGridMap :
    public JSplineMap<JKey_t, JValue_t, JGridMap, typename JResultType<JValue_t>::result_type, JDistance_t>
  {};


  /**
   * Type definition of a zero degree polynomial interpolation based on a JMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint0FunctionalMap : 
    public JPolintMap<0, JKey_t, JValue_t, JMap, typename JResultType<JValue_t>::result_type, JDistance_t>
  {};


  /**
   * Type definition of a 1st degree polynomial interpolation based on a JMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint1FunctionalMap :
    public JPolintMap<1, JKey_t, JValue_t, JMap, typename JResultType<JValue_t>::result_type, JDistance_t>
  {};


  /**
   * Type definition of a 2nd degree polynomial interpolation based on a JMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint2FunctionalMap :
    public JPolintMap<2, JKey_t, JValue_t, JMap, typename JResultType<JValue_t>::result_type, JDistance_t>
  {};


  /**
   * Type definition of a 3rd degree polynomial interpolation based on a JMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint3FunctionalMap :
    public JPolintMap<3, JKey_t, JValue_t, JMap, typename JResultType<JValue_t>::result_type, JDistance_t>
  {};


  /**
   * Type definition of a zero degree polynomial interpolation based on a JGridMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint0FunctionalGridMap :
    public JPolintMap<0, JKey_t, JValue_t, JGridMap, typename JResultType<JValue_t>::result_type, JDistance_t>
  {};


  /**
   * Type definition of a 1st degree polynomial interpolation based on a JGridMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint1FunctionalGridMap :
    public JPolintMap<1, JKey_t, JValue_t, JGridMap, typename JResultType<JValue_t>::result_type, JDistance_t>
  {};


  /**
   * Type definition of a 2nd degree polynomial interpolation based on a JGridMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint2FunctionalGridMap :
    public JPolintMap<2, JKey_t, JValue_t, JGridMap, typename JResultType<JValue_t>::result_type, JDistance_t>
  {};


  /**
   * Type definition of a 3rd degree polynomial interpolation based on a JGridMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint3FunctionalGridMap :
    public JPolintMap<3, JKey_t, JValue_t, JGridMap, typename JResultType<JValue_t>::result_type, JDistance_t>
  {};


  /**
   * Type definition of a spline interpolation based on a JMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JSplineFunctionalMapH :
    public JSplineMap<JKey_t, JValue_t, JMap, JResultDerivative<typename JResultType<JValue_t>::result_type>, JDistance_t>
  {};


  /**
   * Type definition of a spline interpolation based on a JGridMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JSplineFunctionalGridMapH :
    public JSplineMap<JKey_t, JValue_t, JGridMap, JResultDerivative<typename JResultType<JValue_t>::result_type>, JDistance_t>
  {};


  /**
   * Type definition of a zero degree polynomial interpolation based on a JMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint0FunctionalMapH : 
    public JPolintMap<0, JKey_t, JValue_t, JMap, JResultDerivative<typename JResultType<JValue_t>::result_type>, JDistance_t>
  {};


  /**
   * Type definition of a 1st degree polynomial interpolation based on a JMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint1FunctionalMapH :
    public JPolintMap<1, JKey_t, JValue_t, JMap, JResultDerivative<typename JResultType<JValue_t>::result_type>, JDistance_t>
  {};


  /**
   * Type definition of a 2nd degree polynomial interpolation based on a JMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint2FunctionalMapH :
    public JPolintMap<2, JKey_t, JValue_t, JMap, JResultDerivative<typename JResultType<JValue_t>::result_type>, JDistance_t>
  {};


  /**
   * Type definition of a 3rd degree polynomial interpolation based on a JMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint3FunctionalMapH :
    public JPolintMap<3, JKey_t, JValue_t, JMap, JResultDerivative<typename JResultType<JValue_t>::result_type>, JDistance_t>
  {};


  /**
   * Type definition of a zero degree polynomial interpolation based on a JGridMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint0FunctionalGridMapH :
    public JPolintMap<0, JKey_t, JValue_t, JGridMap, JResultDerivative<typename JResultType<JValue_t>::result_type>, JDistance_t>
  {};


  /**
   * Type definition of a 1st degree polynomial interpolation based on a JGridMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint1FunctionalGridMapH :
    public JPolintMap<1, JKey_t, JValue_t, JGridMap, JResultDerivative<typename JResultType<JValue_t>::result_type>, JDistance_t>
  {};


  /**
   * Type definition of a 2nd degree polynomial interpolation based on a JGridMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint2FunctionalGridMapH :
    public JPolintMap<2, JKey_t, JValue_t, JGridMap, JResultDerivative<typename JResultType<JValue_t>::result_type>, JDistance_t>
  {};


  /**
   * Type definition of a 3rd degree polynomial interpolation based on a JGridMap implementation.
   */
  template<class JKey_t, class JValue_t, class JDistance_t = JDistance<JKey_t> >
  struct JPolint3FunctionalGridMapH :
    public JPolintMap<3, JKey_t, JValue_t, JGridMap, JResultDerivative<typename JResultType<JValue_t>::result_type>, JDistance_t>
  {};
}

#endif
