#ifndef __JTOOLS__JQUANTILES__
#define __JTOOLS__JQUANTILES__

#include <limits>
#include <cmath>

#include "JLang/JException.hh"
#include "JTools/JRange.hh"
#include "JTools/JElement.hh"
#include "JTools/JResult.hh"
#include "JTools/JAbstractCollection.hh"
#include "JTools/JToolsToolkit.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JLANG::JException;
  using JLANG::JEmptyCollection;


  /**
   * Locate maximum or minimun of function.
   * 
   * Golden section search code is taken from reference:
   * Numerical Recipes in C++, W.H. Press, S.A. Teukolsky, W.T. Vetterling and B.P. Flannery,
   * Cambridge University Press.
   *
   *             xa < xb < xc
   *
   *             is = +1  ->  There is a minimum, i.e:  f(xb) < min(f(xa),f(xc))
   *             is = -1  ->  There is a maximum, i.e:  f(xb) > max(f(xa),f(xc))
   *
   * \param  xa       
   * \param  xb       
   * \param  xc       
   * \param  f        function
   * \param  is       sign (+1 -> minimim, -1 -> maximum)
   * \param  eps      relative precision
   */
  template<class JFunction1D_t>
  double search(const double xa,
		const double xb,
		const double xc,
		const JFunction1D_t& f,
		const int    is,
		const double eps = 1.0e-6)
  {
    static const double R = 0.61803399;
    static const double C = 1.0 - R;

    double x0 = xa;
    double x3 = xc;
    double x1, x2;

    if (fabs(xc-xb) > fabs(xb-xa)) {
      x1 = xb;
      x2 = xb + C*(xc-xb);
    } else {
      x2 = xb;
      x1 = xb - C*(xb-xa);
    }

    double f1 = is * get_value(f(x1));
    double f2 = is * get_value(f(x2));

    while (fabs(x3-x0) > eps*(fabs(x1)+fabs(x2))) {

      if (f2 < f1) {

	x0 = x1;
	x1 = x2;
	x2 = R*x2 + C*x3;

	f1 = f2;
	f2 = is * get_value(f(x2));

      } else {

	x3 = x2;
	x2 = x1;
	x1 = R*x1 + C*x0;

	f2 = f1;
	f1 = is * get_value(f(x1));
      }
    }

    if (f1 < f2)
      return x1;
    else
      return x2;
  }


  /**
   * Quantile calculator for a given function.
   * It is assumed that the function has a single maximum.
   */
  class JQuantiles :
    public JRange<double> 
  {
  public:

    typedef JAbstractCollection<double>     JAbscissa_t;

    /**
     * Default constructor.
     */
    JQuantiles() :
      Xmax(0.0),
      Ymax(0.0),
      fwhm(0.0),
      sum (0.0)
    {}
   

    /**
     * Constructor.
     *
     * \param   f1        functional collection
     * \param   Q         quantile
     * \param   eps       relative precision
     */
    template<class JFunction1D_t>
    JQuantiles(const JFunction1D_t& f1,
	       const double         Q   = 1.0,
	       const double         eps = 1.0e-6) :
      Xmax(0.0),
      Ymax(0.0),
      fwhm(0.0),
      sum (0.0)
    {
      set(f1, Q, eps);
    }
   

    /**
     * Constructor.
     *
     * \param   abscissa  abscissa
     * \param   f1        function
     * \param   Q         quantile
     * \param   eps       relative precision
     */
    template<class JFunction1D_t>
    JQuantiles(const JAbscissa_t&   abscissa,
	       const JFunction1D_t& f1,
	       const double         Q   = 1.0,
	       const double         eps = 1.0e-6) :
      Xmax(0.0),
      Ymax(0.0),
      fwhm(0.0),
      sum (0.0)
    {
      set(abscissa, f1, Q, eps);
    }


    /**
     * Set quantiles.
     *
     * \param   f1        functional collection
     * \param   Q         quantile
     * \param   eps       relative precision
     */
    template<class JFunction1D_t>
    void set(const JFunction1D_t& f1,
	     const double         Q   = 1.0,
	     const double         eps = 1.0e-6) 
    {
      typedef typename JFunction1D_t::const_iterator  const_iterator;

      if (f1.empty()) {
	throw JEmptyCollection("JQuantiles() no data.");
      }


      // maximum

      const_iterator p = f1.begin();

      for (const_iterator i = f1.begin(); i != f1.end(); ++i) {
	if (i->getY() > p->getY()) {
	  p = i;
	}
      }
      

      // x at maximum
      
      Xmax = p->getX();
	
      if (p != f1.begin()) {
	  
	const double xa = (--p)->getX();
	const double xb = (++p)->getX();
	  
	if (++p != f1.end()) {
	    
	  const double xc = p->getX();
	    
	  Xmax = search(xa, xb, xc, f1, -1, eps);
	}
      }
	
      Ymax = get_value(f1(Xmax));


      // integral & quantile

      if (Q > 0.0 && Q <= 1.0) {

	JSplineFunction1D<JSplineElement2D<double, double>, JCollection, double> buffer;

	try {

	  sum = makeCDF(f1, buffer);

	  setLowerLimit(buffer(0.5 * (1.0 - Q)));
	  setUpperLimit(buffer(0.5 * (1.0 + Q)));
	}
	catch(const JException& error) {
	  sum = 0.0;
	}

      } else {

	sum = JTOOLS::getIntegral(f1);

	if        (Q >  1.0) {
	  setLowerLimit(f1. begin()->getX());
	  setUpperLimit(f1.rbegin()->getX());
	} else if (Q <= 0.0) {
	  setLowerLimit(Xmax);
	  setUpperLimit(Xmax);
	}
      }


      // FWHM
	
      fwhm = 0.0;
	
      for (double xmin = f1.begin()->getX(), xmax = Xmax, v = 0.5*Ymax; ; ) {
	
	const double x = 0.5 * (xmin + xmax);
	const double y = get_value(f1(x));
	
	if (fabs(y - v) < eps*v || xmax - xmin < eps) {
	  fwhm -= x;
	  break;
	}
	
	if (y > v)
	  xmax = x;
	else
	  xmin = x;
      }
      
      for (double xmin = Xmax, xmax = f1.rbegin()->getX(), v = 0.5*Ymax; ; ) {
	
	const double x = 0.5 * (xmin + xmax);
	const double y = get_value(f1(x));
	  
	if (fabs(y - v) < eps*v || xmax - xmin < eps) {
	  fwhm += x;
	  break;
	}
	
	if (y > v)
	  xmin = x;
	else
	  xmax = x;
      }
    }


    /**
     * Set quantiles.
     *
     * \param   abscissa  abscissa
     * \param   f1        function
     * \param   Q         quantile
     * \param   eps       relative precision
     */
    template<class JFunction1D_t>
    void set(const JAbscissa_t&   abscissa,
	     const JFunction1D_t& f1,
	     const double         Q   = 1.0,
	     const double         eps = 1.0e-6) 
    {
      JSplineFunction1D<JSplineElement2D<double, double>, JCollection, double> buffer;

      buffer.configure(abscissa, f1);
      buffer.compile();

      set(buffer, Q, eps);
    }


    /**
     * Get position of maximum.
     *
     * \return     x value at maximum
     */
    double getX() const 
    { 
      return Xmax;
    }

 
    /**
     * Get value of maximum.
     *
     * \return     y value at maximum
     */
    double getY() const 
    {
      return Ymax;
    }


    /**
     * Get Full Width at Half Maximum.
     *
     * \return     FWHM
     */
    double getFWHM() const
    {
      return fwhm;
    }
    

    /**
     * Get integral of function.
     *
     * \return     integral
     */
    double getIntegral() const
    {
      return sum;
    }


  protected:
    double Xmax;
    double Ymax;
    double fwhm;
    double sum;
  };
}

#endif
