#ifndef __JMATHSUPPORTKIT__
#define __JMATHSUPPORTKIT__

#include <limits>
#include <cmath>

#include "JMath/JConstants.hh"
#include "JLang/JException.hh"


/**
 * \file
 *
 * Auxiliary methods for mathematics.
 * \author mdejong
 */

namespace JMATH {}
namespace JPP { using namespace JMATH; }

namespace JMATH {

  using JLANG::JValueOutOfRange;

  
  /**
   * Gauss function (normalised to 1 at x = 0).
   *
   * \param  x                    x
   * \param  sigma                sigma
   * \return                      function value
   */
  inline double gauss(const double x, const double sigma)
  {
    const double u = x / sigma;

    if (fabs(u) < 10.0)
      return exp(-0.5*u*u);
    else
      return 0.0;
  }


  /**
   * Gauss function (normalised to 1 at x = x0).
   *
   * \param  x                    x
   * \param  x0                   central value
   * \param  sigma                sigma
   * \return                      function value
   */
  inline double gauss(const double x, const double x0, const double sigma)
  {
    return gauss(x - x0, sigma);
  }


  /**
   * Normalised Gauss function.
   *
   * \param  x                    x
   * \param  sigma                sigma
   * \return                      function value
   */
  inline double Gauss(const double x, const double sigma)
  {
    return gauss(x, sigma) / sqrt(2.0*PI) / sigma;
  }


  /**
   * Normalised Gauss function.
   *
   * \param  x                    x
   * \param  x0                   central value
   * \param  sigma                sigma
   * \return                      function value
   */
  inline double Gauss(const double x, const double x0, const double sigma)
  {
    return Gauss(x - x0, sigma);
  }


  /**
   * Incomplete gamma function.
   *
   * \param  a                    a
   * \param  x                    x
   * \return                      function value
   */
  inline double Gamma(const double a, const double x)
  {
    using namespace std;

    const int max = 100;

    if (x <  0.0) { THROW(JValueOutOfRange, "x <  0 " << x); }
    if (a <= 0.0) { THROW(JValueOutOfRange, "a <= 0 " << a); }
 
    const double gln = lgamma(a);

    if (x <  a + 1.0) {

      if (x <= 0.0) { 
	THROW(JValueOutOfRange, "x <= 0 " << x);
      }

      double ap  = a;
      double sum = 1.0 /a;
      double del = sum;

      for (int i = 1; i != max; ++i) {

	ap  += 1.0;
	del *= x/ap;
	sum += del;

	if (fabs(del) < fabs(sum)*numeric_limits<double>::epsilon()) {
	  return sum*exp(-x + a*log(x) - gln);
	}
      }

    } else {

      double b = x + 1.0 - a;
      double c = numeric_limits<double>::epsilon() / numeric_limits<double>::min();
      double d = 1.0 / b;
      double h = d;

      for (int i = 1; i != max; ++i) {

	const double an = -i * (i-a);

	b += 2.0;
	d  = an*d + b;

	if (fabs(d) < numeric_limits<double>::min()) {
	  d = numeric_limits<double>::min();
	}

	c  = b + an/c;

	if (fabs(c) < numeric_limits<double>::min()) {
	  c = numeric_limits<double>::min();
	}

	d  = 1.0/d;

	const double del = d*c;
	
	h *= del;
	
	if (fabs(del - 1.0) < numeric_limits<double>::epsilon()) {
	  return 1.0 - exp(-x + a*log(x) - gln) * h;
	}
      }

      THROW(JValueOutOfRange, "a " << a);
    }

    return 0.0;
  }


  /**
   * Legendre polynome.
   *
   * \param  n                    degree 
   * \param  x                    x
   * \return                      function value
   */
  inline double legendre(const unsigned int n, const double x)
  {
    switch (n) {

    case 0:
      return 1.0;

    case 1:
      return x;

    default:
      {
	double p0;
	double p1 = 1.0;
	double p2 = x;

	for (unsigned int i = 2; i <= n; ++i) {
	  p0 = p1;
	  p1 = p2;
	  p2 = ((2*i-1) * x*p1  -  (i-1) * p0) / i;
	}

	return p2;
      }
    }
  }


  /**
   * Binomial function.
   *
   * \param  n                    n
   * \param  k                    k
   * \return                      function value
   */
  inline double binomial(const int n, const int k)
  {
    if (k == 0 || n == k) {
      return 1.0;
    }

    if (n <= 0 || k < 0 || n < k) {
      return 0.0;
    }

    const int k1 = std::min(k, n - k);
    const int k2 = n - k1;

    double value = k2 + 1;

    for (int i = k1; i != 1; --i) {
      value *= (double) (k2 + i) / (double) i;
    }

    return value;
  }
}

#endif
