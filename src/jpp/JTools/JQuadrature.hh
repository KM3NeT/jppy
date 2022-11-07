#ifndef __JTOOLS__JQUADRATURE__
#define __JTOOLS__JQUADRATURE__

#include <cmath>

#include "JTools/JConstants.hh"
#include "JTools/JElement.hh"
#include "JTools/JCollection.hh"


/**
 * \file
 *
 * Auxiliary classes for numerical integration.
 * \author mdejong
 */
namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  /**
   * Type definition of basic element for quadratures.
   */
  typedef JElement2D<double, double>               JElement2D_t;
  

  /**
   * Type definition for numerical integration.
   *
   *          \f[\displaystyle \int_{x_1}^{x_2} f(x) dx = \sum_{i=1}^{N} w_i f(x_i) \f]
   *
   * The abscissa and ordinate values of the collection can be used
   * as abscissa and weight values of the summation to approximately
   * determine the integral of the function.
   */
  class JQuadrature :
    public JCollection<JElement2D_t> 
  {
  public:
    /**
     * Default constructor.
     */
    JQuadrature()
    {}


    /**
     * General purpose constructor.
     *
     * The template argument should correspond to a function requiring two arguments.\n
     * These two arguments should correspond to the lower and upper integration limit, respectively.\n
     * The given function should return the value of the integral between the two integration limits. 
     *
     * \param  xmin       minimal x
     * \param  xmax       maximal x
     * \param  nx         number of points
     * \param  integral   integral
     * \param  eps        precision
     */
    template<class JFunction_t>
    JQuadrature(const double xmin,
		const double xmax,
		const int    nx,
		JFunction_t  integral,
		const double eps = 1.0e-4)
    {
      double Xmin = xmin;
      double Xmax = xmax;

      const double Vmin = integral(Xmin, Xmax) / (double) nx;
    
      for (int i = 0; i != nx; ++i) {
      
	for (double xmin = Xmin, xmax = Xmax; ; ) {
	  
	  const double x = 0.5 * (xmin + xmax);
	  const double v = integral(Xmin, x);
	
	  if (fabs(Vmin - v)    < eps *  Vmin ||
	      fabs(xmax - xmin) < eps * (Xmax - Xmin)) {
	    
	    const double __x = 0.5 * (Xmin + x);
	    const double __y = Vmin / integral(__x);
	  
	    insert(JElement2D_t(__x,__y));
	  
	    Xmin = x;
	    xmax = Xmax;
	    
	    break;
	  }
	  
	  if (v < Vmin)
	    xmin = x;
	  else
	    xmax = x;
	}
      }
    }
  };


  /**
   * Numerical integrator for \f$ W(x) = 1 \f$.
   * 
   * Gauss-Legendre integration code is taken from reference:
   * Numerical Recipes in C++, W.H. Press, S.A. Teukolsky, W.T. Vetterling and B.P. Flannery, 
   * Cambridge University Press.
   */ 
  class JGaussLegendre : 
    public JQuadrature 
  {
  public:
    /**
     * Constructor.
     *
     * \param  n          number of points
     * \param  eps        precision
     */
    JGaussLegendre(const int    n,
		   const double eps = 1.0e-12) :
      JQuadrature()
    {
      resize(n);

      const int M = (n + 1) / 2;
      
      double p0, p1, p2, pp;
      
      for (int i = 0; i < M; ++i) {
	
	double z  = cos(PI * (i+0.75) / (n+0.5));
	double z1;
	
	do {
	  
	  p1 = 0.0;
	  p2 = 1.0;
	  
	  // recurrence relation
	  
	  for (int j = 0; j < n; ++j) {
	    p0 = p1;
	    p1 = p2;
	    p2 = ((2*j + 1) * z*p1  - j*p0) / (j+1);
	  }
	  
	  pp = n * (z*p2 - p1) / (z*z - 1.0);
	  
	  z1 = z;
	  z  = z1 - p2/pp;
	  
	} while (fabs(z-z1) > eps);

	const double y = 2.0 / ((1.0-z*z)*pp*pp);

	at(  i  ) = JElement2D_t(-z,y);
	at(n-i-1) = JElement2D_t(+z,y);
      }
    }
  };


  /**
   * Numerical integrator for \f$ W(x) = x^{a} \, e^{-x} \f$.
   *
   * Gauss-Laguerre integration code is taken from reference:
   * Numerical Recipes in C++, W.H. Press, S.A. Teukolsky, W.T. Vetterling and B.P. Flannery, 
   * Cambridge University Press.
   */ 
  class JGaussLaguerre : 
    public JQuadrature 
  {
  public:
    /**
     * Constructor.
     *
     * \param  n          number of points
     * \param  alf        power
     * \param  eps        precision
     */
    JGaussLaguerre(const int    n,
		   const double alf,
		   const double eps = 1.0e-12) :
      JQuadrature()
    {
      const int number_of_iterations = 100;

      double z1;
      double p0, p1, p2, pp;
      
      double z = (1.0 + alf) * (3.0 + 0.92*alf) / (1.0 + 2.4*n + 1.8*alf); 

      for (int i = 0; i < n; ++i) {
	
	switch (i) {

	case 0:
	  break;

	case 1:
	  z += (15.0 + 6.25*alf) / (1.0 + 0.9*alf + 2.5*n); 
	  break;

	default:
	  const double ai = i - 1;
	  z += ((1.0+2.55*ai)/(1.9*ai) + (1.26*ai*alf)/(1.0+3.5*ai)) * (z - at(i-2).getX()) / (1.0 + 0.3*alf);
	  break;
	}

	for (int k = 0; k != number_of_iterations; ++k) {
	  
	  p1 = 0.0;
	  p2 = 1.0;
	  
	  // recurrence relation
	  
	  for (int j = 0; j < n; ++j) {
	    p0 = p1;
	    p1 = p2;
	    p2 = ((2*j + 1 + alf - z) * p1  - (j + alf)*p0) / (j+1);
	  }
	  
	  pp = (n*p2 - (n+alf)*p1) / z;
	  
	  z1 = z;
	  z  = z1 - p2/pp;

	  if (fabs(z-z1) < eps)
	    break;
	}

	const double y = -tgamma(alf+n) / tgamma((double) n) / (pp*n*p1);
	
	insert(JElement2D_t(z,y));
      }
    }
  };


  /**
   * Numerical integrator for \f$ W(x) = e^{-x^{2}} \f$.
   *
   * Gauss-Hermite integration code is taken from reference:
   * Numerical Recipes in C++, W.H. Press, S.A. Teukolsky, W.T. Vetterling and B.P. Flannery, 
   * Cambridge University Press.
   */ 
  class JGaussHermite : 
    public JQuadrature 
  {
  public:
    /**
     * Constructor.
     *
     * \param  n          number of points
     * \param  eps        precision
     */
    JGaussHermite(const int    n,
		  const double eps = 1.0e-12) :
      JQuadrature()
    {
      resize(n);

      const double pii = 1.0 / pow(PI,0.25);

      const int number_of_iterations = 100;

      const int M = (n + 1) / 2;
      
      double p0, p1, p2, pp;
      double z = 0.0;
      double z1;
      
      for (int i = 0; i < M; ++i) {

	switch (i) {

	case 0:
	  z  = sqrt((double) (2*n+1))  -  1.85575 * pow((double) (2*n+1),-0.16667);
	  break;

	case 1:
	  z -= 1.14 * pow((double) n,0.426) / z;
	  break;

	case 2:
	  z  = 1.86*z + 0.86*at( 0 ).getX();
	  break;

	case 3:
	  z  = 1.91*z + 0.91*at( 1 ).getX();
	  break;

	default:
	  z  = 2.00*z + 1.00*at(i-2).getX();
	  break;
	}

	for (int k = 0; k != number_of_iterations; ++k) {
	  
	  p1 = 0.0;
	  p2 = pii;
	  
	  // recurrence relation
	  
	  for (int j = 0; j < n; ++j) {
	    p0 = p1;
	    p1 = p2;
	    p2 = z * sqrt(2.0/(double) (j+1)) * p1  -  sqrt((double) j / (double) (j+1)) * p0;
	  }
	  
	  pp = sqrt((double) (2*n)) * p1;
	  
	  z1 = z;
	  z  = z1 - p2/pp;

	  if (fabs(z-z1) < eps)
	    break;
	} 

	const double y = 2.0 / (pp*pp);

	at(  i  ) = JElement2D_t(-z,y);
	at(n-i-1) = JElement2D_t(+z,y);
      }
    }
  };


  /**
   * Numerical integrator for \f$ W(x) = (1 + g^{2} - 2gx)^{a} \f$, where \f$ g > 0 \f$.
   *
   * Henyey-Greenstein integration points and weights.
   */ 
  class JHenyeyGreenstein : 
    public JQuadrature 
  {
  public:
    /**
     * Constructor.
     *
     * \param  n          number of points
     * \param  g          angular dependence parameter
     * \param  a          power
     */
    JHenyeyGreenstein(const int    n,
		      const double g,
		      const double a) :
      JQuadrature()
    {
      const double b  = -2*g * (a + 1.0);
      const double ai =  1.0 / (a + 1.0);

      const double ymin = pow(1.0 + g, 2*(a + 1.0)) / b;
      const double ymax = pow(1.0 - g, 2*(a + 1.0)) / b;

      const double dy = (ymax - ymin) / (n + 1);
      
      for (double y = ymax - 0.5*dy; y > ymin; y -= dy) {
	
	const double v  = y*b;
	const double w  = pow(v, ai);
	const double x  = (1.0 + g*g - w) / (2*g);
	const double dx = pow(v, -a*ai)*dy;
	
	insert(JElement2D_t(x,dx));
      }
    }


    /**
     * Constructor.
     *
     * \param  n          number of points
     * \param  g          angular dependence parameter
     * \param  a          power
     * \param  xmin       minimal value
     * \param  xmax       maximal value
     */
    JHenyeyGreenstein(const int    n,
		      const double g,
		      const double a,
		      const double xmin,
		      const double xmax) :
      JQuadrature()
    {
      const double b  = -2*g * (a + 1.0);
      const double ai =  1.0 / (a + 1.0);

      const double ymin = pow(1.0 + g*g -2*g*xmin, a + 1.0) / b;
      const double ymax = pow(1.0 + g*g -2*g*xmax, a + 1.0) / b;

      const double dy = (ymax - ymin) / (n + 1);
      
      for (double y = ymax - 0.5*dy; y > ymin; y -= dy) {
	
	const double v  = y*b;
	const double w  = pow(v, ai);
	const double x  = (1.0 + g*g - w) / (2*g);
	const double dx = pow(v, -a*ai)*dy;
	
	insert(JElement2D_t(x,dx));
      }
    }


    /**
     * Constructor for special case where a = -1.
     *
     * \param  n          number of points
     * \param  g          angular dependence parameter
     */
    JHenyeyGreenstein(const int    n,
		      const double g) :
      JQuadrature()
    {
      const double dy = 1.0 / (n + 1);
      const double gi = log((1.0 + g*g) / (1.0 - g*g)) / (2.0*g);
      
      for (double y = 1.0 - 0.5*dy; y > 0.0; y -= dy) {
	
	const double v  = -y*2.0*g*gi  +  log(1.0 + g*g);
	const double w  = exp(v);
	const double x  = (1.0 + g*g - w) / (2.0*g);
	const double dx = w*gi*dy;
	
	insert(JElement2D_t(x,dx));
      }
    }
  };


  /**
   * Numerical integrator for \f$ W(x) = 1 + g \, x^{2} \f$, where \f$ g > 0 \f$.
   *
   * Rayleigh integration points and weights.
   */ 
  class JRayleigh : 
    public JQuadrature 
  {
  public:
    /**
     * Constructor.
     *
     * \param  n          number of points
     * \param  g          angular dependence parameter
     */
    JRayleigh(const int    n,
	      const double g) :
      JQuadrature()
    {
      const double dy = 1.0 / (n + 1);
      const double gi = 3.0/g  +  1.0;
      
      // t^3 + 3pt + 2q = 0
      
      const double p  = 1.0/g;
      
      for (double y = 0.5*dy; y < 1.0; y += dy) {
	
	const double q  = 0.5*gi - gi*y;
	
	const double b  = sqrt(q*q + p*p*p);
	const double u  = pow(-q + b, 1.0/3.0);
	const double v  = pow(+q + b, 1.0/3.0);
	
	const double x  =  u - v;
	const double dx = (u + v) / (3.0*b);
	
	insert(JElement2D_t(x, dx*gi*dy)); 
      }
    }
  };


  /**
   * Numerical integrator for \f$ W(x) = \left|x\right| / \sqrt{1 - x^{2}} \f$.
   *
   * Co-tangent integration points and weights.
   */ 
  class JCotangent : 
    public JQuadrature 
  {
  public:
    /**
     * Constructor.
     *
     * \param  n          number of points
     */
    JCotangent(const int n) :
      JQuadrature()
    {
      for (double ds = 1.0 / (n/2), sb = 0.5*ds; sb < 1.0; sb += ds) {

	const double cb = sqrt((1.0 + sb)*(1.0 - sb));
	const double dc = ds*sb/cb;
	  
	insert(JElement2D_t(+cb, dc));   
	insert(JElement2D_t(-cb, dc));   
      }
    }
  };


  /**
   * Numerical integrator for \f$ W(x) = \left|x\right| / \sqrt{1 - x^{2}} \f$ for \f$ x > 0 \f$ and \f$ W(x) = 1 \f$ for \f$ x \le 0 \f$.
   *
   * Bi-tangent integration points and weights.
   */ 
  class JBitangent : 
    public JQuadrature 
  {
  public:
    /**
     * Constructor.
     *
     * \param  n          number of points
     */
    JBitangent(const int n) :
      JQuadrature()
    {
      double sb, ds;
      double cb = 0.0;
      double dc = 0.0;
	  
      for (ds = 1.0 / (n/2), sb = 0.5*ds; sb < 1.0; sb += ds) {

	cb = sqrt((1.0 + sb)*(1.0 - sb));
	dc = ds*sb/cb;
	  
	insert(JElement2D_t(cb, dc));   
      }

      for (dc = (cb + 1.0) / (n/2), cb -= 0.5*dc ; cb > -1.0; cb -= dc) {
	insert(JElement2D_t(cb, dc));   
      }
    }
  };
}

#endif
