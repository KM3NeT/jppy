#ifndef __JTOOLS__JFUNCTIONALMAP__
#define __JTOOLS__JFUNCTIONALMAP__

#include "JTools/JFunctionalMap_t.hh"
#include "JTools/JSpline.hh"
#include "JTools/JPolint.hh"
#include "JTools/JCollection.hh"
#include "JTools/JGridCollection.hh"
#include "JTools/JElement.hh"
#include "JTools/JDistance.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {


  /**
   * Auxiliary class to define corresponding one-dimensional function interpolator <tt>function_type</tt>. 
   */
  template<template<class JKey_t, class JValue_t, class JDistance_t> class JFunctionalMap_t>
  struct JFunctionalMap;


  /**
   * Specialisation of JFunctionalMap for JSplineFunctionalMap.
   */
  template<>
  struct JFunctionalMap<JSplineFunctionalMap>
  {
    /**
     * Corresponding one-dimensional function interpolator.
     */
    template<class JAbscissa_t, 
	     class JOrdinate_t, 
	     class JResult_t, 
	     class JDistance_t = JDistance<JAbscissa_t> >
    struct function_type : 
      public JSplineFunction1D<JSplineElement2D<JAbscissa_t, JOrdinate_t>, JCollection, JResult_t, JDistance_t>
    {}; 
  };


  /**
   * Specialisation of JFunctionalMap for JSplineFunctionalGridMap.
   */
  template<>
  struct JFunctionalMap<JSplineFunctionalGridMap>
  {
    /**
     * Corresponding one-dimensional function interpolator.
     */
    template<class JAbscissa_t, 
	     class JOrdinate_t, 
	     class JResult_t, 
	     class JDistance_t = JDistance<JAbscissa_t> >
    struct function_type : 
      public JSplineFunction1D<JSplineElement2D<JAbscissa_t, JOrdinate_t>, JGridCollection, JResult_t, JDistance_t>
    {}; 
  };


  /**
   * Specialisation of JFunctionalMap for JPolint0FunctionalMap.
   */
  template<>
  struct JFunctionalMap<JPolint0FunctionalMap>
  {
    /**
     * Corresponding one-dimensional function interpolator.
     */
    template<class JAbscissa_t, 
	     class JOrdinate_t, 
	     class JResult_t, 
	     class JDistance_t = JDistance<JAbscissa_t> >
    struct function_type : 
      public JPolintFunction1D<0, JElement2D<JAbscissa_t, JOrdinate_t>, JCollection, JResult_t, JDistance_t>
    {}; 
  };


  /**
   * Specialisation of JFunctionalMap for JPolint1FunctionalMap.
   */
  template<>
  struct JFunctionalMap<JPolint1FunctionalMap>
  {
    /**
     * Corresponding one-dimensional function interpolator.
     */
    template<class JAbscissa_t, 
	     class JOrdinate_t, 
	     class JResult_t, 
	     class JDistance_t = JDistance<JAbscissa_t> >
    struct function_type : 
      public JPolintFunction1D<1, JElement2D<JAbscissa_t, JOrdinate_t>, JCollection, JResult_t, JDistance_t>
    {}; 
  };


  /**
   * Specialisation of JFunctionalMap for JPolint2FunctionalMap.
   */
  template<>
  struct JFunctionalMap<JPolint2FunctionalMap>
  {
    /**
     * Corresponding one-dimensional function interpolator.
     */
    template<class JAbscissa_t, 
	     class JOrdinate_t, 
	     class JResult_t, 
	     class JDistance_t = JDistance<JAbscissa_t> >
    struct function_type : 
      public JPolintFunction1D<2, JElement2D<JAbscissa_t, JOrdinate_t>, JCollection, JResult_t, JDistance_t>
    {}; 
  };


  /**
   * Specialisation of JFunctionalMap for JPolint3FunctionalMap.
   */
  template<>
  struct JFunctionalMap<JPolint3FunctionalMap>
  {
    /**
     * Corresponding one-dimensional function interpolator.
     */
    template<class JAbscissa_t, 
	     class JOrdinate_t, 
	     class JResult_t, 
	     class JDistance_t = JDistance<JAbscissa_t> >
    struct function_type : 
      public JPolintFunction1D<3, JElement2D<JAbscissa_t, JOrdinate_t>, JCollection, JResult_t, JDistance_t>
    {}; 
  };


  /**
   * Specialisation of JFunctionalMap for JPolint0FunctionalGridMap.
   */
  template<>
  struct JFunctionalMap<JPolint0FunctionalGridMap>
  {
    /**
     * Corresponding one-dimensional function interpolator.
     */
    template<class JAbscissa_t, 
	     class JOrdinate_t, 
	     class JResult_t, 
	     class JDistance_t = JDistance<JAbscissa_t> >
    struct function_type : 
      public JPolintFunction1D<0, JElement2D<JAbscissa_t, JOrdinate_t>, JGridCollection, JResult_t, JDistance_t>
    {}; 
  };


  /**
   * Specialisation of JFunctionalMap for JPolint1FunctionalGridMap.
   */
  template<>
  struct JFunctionalMap<JPolint1FunctionalGridMap>
  {
    /**
     * Corresponding one-dimensional function interpolator.
     */
    template<class JAbscissa_t, 
	     class JOrdinate_t, 
	     class JResult_t, 
	     class JDistance_t = JDistance<JAbscissa_t> >
    struct function_type : 
      public JPolintFunction1D<1, JElement2D<JAbscissa_t, JOrdinate_t>, JGridCollection, JResult_t, JDistance_t>
    {}; 
  };


  /**
   * Specialisation of JFunctionalMap for JPolint2FunctionalGridMap.
   */
  template<>
  struct JFunctionalMap<JPolint2FunctionalGridMap>
  {
    /**
     * Corresponding one-dimensional function interpolator.
     */
    template<class JAbscissa_t, 
	     class JOrdinate_t, 
	     class JResult_t, 
	     class JDistance_t = JDistance<JAbscissa_t> >
    struct function_type : 
      public JPolintFunction1D<2, JElement2D<JAbscissa_t, JOrdinate_t>, JGridCollection, JResult_t, JDistance_t>
    {}; 
  };


  /**
   * Specialisation of JFunctionalMap for JPolint3FunctionalGridMap.
   */
  template<>
  struct JFunctionalMap<JPolint3FunctionalGridMap>
  {
    /**
     * Corresponding one-dimensional function interpolator.
     */
    template<class JAbscissa_t, 
	     class JOrdinate_t, 
	     class JResult_t, 
	     class JDistance_t = JDistance<JAbscissa_t> >
    struct function_type : 
      public JPolintFunction1D<3, JElement2D<JAbscissa_t, JOrdinate_t>, JGridCollection, JResult_t, JDistance_t>
    {}; 
  };
}

#endif
