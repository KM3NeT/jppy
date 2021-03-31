#ifndef __JFUNCTION1D_T__
#define __JFUNCTION1D_T__

#include "JTools/JElement.hh"
#include "JTools/JCollection.hh"
#include "JTools/JGridCollection.hh"
#include "JTools/JSpline.hh"
#include "JTools/JHermiteSpline.hh"
#include "JTools/JPolint.hh"
#include "JTools/JResult.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {


  /**
   * Type definition of a spline interpolation based on a JGridCollection.
   */
  template<class JElement_t, class JDistance_t = JDistance<typename JElement_t::abscissa_type> >
  struct JGridSplineFunction1D : 
    public JSplineFunction1D<JElement_t, JGridCollection, JDistance_t>
  {};


  /**
   * Type definition of a spline interpolation method based on a JCollection with double result type.
   */
  struct JSplineFunction1D_t : 
    public JSplineFunction1D<JSplineElement2D<double, double>, JCollection, double>
  {};


  /**
   * Type definition of a spline interpolation method based on a JCollection with JResultDerivative result type.
   */
  struct JSplineFunction1H_t : 
    public JSplineFunction1D<JSplineElement2S<double, double>, JCollection, JResultDerivative<double> >
  {};


  /**
   * Type definition of a spline interpolation method based on a JCollection with JResultPDF result type.
   */
  struct JSplineFunction1S_t : 
    public JSplineFunction1D<JSplineElement2S<double, double>, JCollection, JResultPDF<double> >
  {};


  /**
   * Type definition of a spline interpolation based on a JGridCollection with result type double.
   */
  struct JGridSplineFunction1D_t : 
    public JSplineFunction1D<JSplineElement2D<double, double>, JGridCollection, double>
  {};


  /**
   * Type definition of a spline interpolation based on a JGridCollection with JResultDerivative result type.
   */
  struct JGridSplineFunction1H_t :
    public JSplineFunction1D<JSplineElement2S<double, double>, JGridCollection, JResultDerivative<double> >
  {};


  /**
   * Type definition of a spline interpolation based on a JGridCollection with JResultPDF result type.
   */
  struct JGridSplineFunction1S_t :
    public JSplineFunction1D<JSplineElement2S<double, double>, JGridCollection, JResultPDF<double> >
  {};


  /**
   * Type definition of a spline interpolation based on a JGridCollection.
   */
  template<class JElement_t, class JDistance_t = JDistance<typename JElement_t::abscissa_type> >
  struct JGridHermiteSplineFunction1D : 
    public JHermiteSplineFunction1D<JElement_t, JGridCollection, JDistance_t>
  {};


  /**
   * Type definition of a spline interpolation method based on a JCollection with double result type.
   */
  struct JHermiteSplineFunction1D_t : 
    public JHermiteSplineFunction1D<JSplineElement2D<double, double>, JCollection, double>
  {};


  /**
   * Type definition of a spline interpolation method based on a JCollection with JResultDerivative result type.
   */
  struct JHermiteSplineFunction1H_t : 
    public JHermiteSplineFunction1D<JSplineElement2S<double, double>, JCollection, JResultDerivative<double> >
  {};


  /**
   * Type definition of a spline interpolation method based on a JCollection with JResultPDF result type.
   */
  struct JHermiteSplineFunction1S_t : 
    public JHermiteSplineFunction1D<JSplineElement2S<double, double>, JCollection, JResultPDF<double> >
  {};


  /**
   * Type definition of a spline interpolation based on a JGridCollection with result type double.
   */
  struct JGridHermiteSplineFunction1D_t : 
    public JHermiteSplineFunction1D<JSplineElement2D<double, double>, JGridCollection, double>
  {};


  /**
   * Type definition of a spline interpolation based on a JGridCollection with JResultDerivative result type.
   */
  struct JGridHermiteSplineFunction1H_t :
    public JHermiteSplineFunction1D<JSplineElement2S<double, double>, JGridCollection, JResultDerivative<double> >
  {};


  /**
   * Type definition of a zero degree polynomial interpolation.
   */
  template<class JElement_t, class JDistance_t = JDistance<typename JElement_t::abscissa_type> >
  struct JPolint0Function1D :
    public JPolintFunction1D<0, JElement_t, JCollection, JDistance_t>
  {};


  /**
   * Type definition of a 1st degree polynomial interpolation.
   */
  template<class JElement_t, class JDistance_t = JDistance<typename JElement_t::abscissa_type> >
  struct JPolint1Function1D :
    public JPolintFunction1D<1, JElement_t, JCollection, JDistance_t>
  {};


  /**
   * Type definition of a 2nd degree polynomial interpolation.
   */
  template<class JElement_t, class JDistance_t = JDistance<typename JElement_t::abscissa_type> >
  struct JPolint2Function1D :
    public JPolintFunction1D<2, JElement_t, JCollection, JDistance_t>
  {};


  /**
   * Type definition of a 3rd degree polynomial interpolation.
   */
  template<class JElement_t, class JDistance_t = JDistance<typename JElement_t::abscissa_type> >
  struct JPolint3Function1D :
    public JPolintFunction1D<3, JElement_t, JCollection, JDistance_t>
  {};


  /**
   * Polynomial interpolation method with result type double.
   */
  template<int N>
  struct JPolintFunction1D_t :
    public JPolintFunction1D<N, JElement2D<double, double>, JCollection, double>
  {};


  /**
   * Type definition of a zero degree polynomial interpolation with result type double.
   */
  struct JPolint0Function1D_t :
    public JPolintFunction1D_t<0>
  {};


  /**
   * Type definition of a 1st degree polynomial interpolation with result type double.
   */
  struct JPolint1Function1D_t :
    public JPolintFunction1D_t<1>
  {};


  /**
   * Type definition of a 2nd degree polynomial interpolation with result type double.
   */
  struct JPolint2Function1D_t :
    public JPolintFunction1D_t<2>
  {};


  /**
   * Type definition of a 3rd degree polynomial interpolation with result type double.
   */
  struct JPolint3Function1D_t :
    public JPolintFunction1D_t<3>
  {};


  /**
   * Polynomial interpolation method with result type JResultDerivative.
   */
  template<int N>
  struct JPolintFunction1H_t :
    public JPolintFunction1D<N, JElement2D<double, double>, JCollection, JResultDerivative<double> >
  {};


  /**
   * Type definition of a 1st degree polynomial interpolation with result type JResultDerivative.
   */
  struct JPolint1Function1H_t :
    public JPolintFunction1H_t<1>
  {};


  /**
   * Type definition of a 2nd degree polynomial interpolation with result type JResultDerivative.
   */
  struct JPolint2Function1H_t :
    public JPolintFunction1H_t<2>
  {};


  /**
   * Type definition of a 3rd degree polynomial interpolation with result type JResultDerivative.
   */
  struct JPolint3Function1H_t :
    public JPolintFunction1H_t<3>
  {};


  /**
   * Polynomial interpolation method with result type JResultPDF.
   */
  template<int N>
  struct JPolintFunction1S_t :
    public JPolintFunction1D<N, JPolintElement2S<double, double>, JCollection, JResultPDF<double> >
  {};


  /**
   * Type definition of a 1st degree polynomial interpolation with result type JResultPDF.
   */
  struct JPolint1Function1S_t : 
    public JPolintFunction1S_t<1>
  {};


  /**
   * Type definition of a 2nd degree polynomial interpolation with result type JResultPDF.
   */
  struct JPolint2Function1S_t : 
    public JPolintFunction1S_t<2>
  {};


  /**
   * Type definition of a 3rd degree polynomial interpolation with result type JResultPDF.
   */
  struct JPolint3Function1S_t : 
    public JPolintFunction1S_t<3>
  {};


  /**
   * Polynomial interpolation method based on a JGridCollection with result type double.
   */
  template<int N>
  struct JGridPolintFunction1D_t :
    public JPolintFunction1D<N, JElement2D<double, double>, JGridCollection, double>
  {};


  /**
   * Type definition of a zero degree polynomial interpolation based on a JGridCollection with result type double.
   */
  struct JGridPolint0Function1D_t :
    public JGridPolintFunction1D_t<0>
  {};


  /**
   * Type definition of a 1st degree polynomial interpolation based on a JGridCollection with result type double.
   */
  struct JGridPolint1Function1D_t :
    public JGridPolintFunction1D_t<1>
  {};


  /**
   * Type definition of a 2nd degree polynomial interpolation based on a JGridCollection with result type double.
   */
  struct JGridPolint2Function1D_t :
    public JGridPolintFunction1D_t<2>
  {};


  /**
   * Type definition of a 3rd degree polynomial interpolation based on a JGridCollection with result type double.
   */
  struct JGridPolint3Function1D_t :
    public JGridPolintFunction1D_t<3>
  {};


  /**
   * Polynomial interpolation method based on a JGridCollection with result type JResultDerivative.
   */
  template<int N>
  struct JGridPolintFunction1H_t :
    public JPolintFunction1D<N, JElement2D<double, double>, JGridCollection, JResultDerivative<double> >
  {};


  /**
   * Type definition of a 1st degree polynomial interpolation with result type JResultDerivative.
   */
  struct JGridPolint1Function1H_t :
    public JGridPolintFunction1H_t<2>
  {};



  /**
   * Type definition of a 2nd degree polynomial interpolation with result type JResultDerivative.
   */
  struct JGridPolint2Function1H_t :
    public JGridPolintFunction1H_t<2>
  {};


  /**
   * Type definition of a 3rd degree polynomial interpolation with result type JResultDerivative.
   */
  struct JGridPolint3Function1H_t :
    public JGridPolintFunction1H_t<3>
  {};


  /**
   * Polynomial interpolation method with result type JResultPDF.
   */
  template<int N>
  struct JGridPolintFunction1S_t :
    public JPolintFunction1D<N, JPolintElement2S<double, double>, JGridCollection, JResultPDF<double> >
  {};


  /**
   * Type definition of a 1st degree polynomial interpolation with result type JResulPDF.
   */
  struct JGridPolint1Function1S_t : 
    public JGridPolintFunction1S_t<1>
  {};


  /**
   * Type definition of a 2nd degree polynomial interpolation with result type JResulPDF.
   */
  struct JGridPolint2Function1S_t : 
    public JGridPolintFunction1S_t<2>
  {};


  /**
   * Type definition of a 3rd degree polynomial interpolation with result type JResulPDF.
   */
  struct JGridPolint3Function1S_t : 
    public JGridPolintFunction1S_t<3>
  {};
}

#endif
