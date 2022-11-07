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

    if (fabs(u) < 20.0)
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
   * Source code is taken from reference:
   * Numerical Recipes in C++, W.H. Press, S.A. Teukolsky, W.T. Vetterling and B.P. Flannery,
   * Cambridge University Press.
   *
   * \param  a                    a
   * \param  x                    x
   * \return                      function value
   */
  inline double Gamma(const double a, const double x)
  {
    using namespace std;

    const int max = 1000000;

    if (x <  0.0) { THROW(JValueOutOfRange, "x <  0 " << x); }
    if (a <= 0.0) { THROW(JValueOutOfRange, "a <= 0 " << a); }
 
    const double gln = lgamma(a);

    if (x < a + 1.0) {

      if (x < 0.0) { 
	THROW(JValueOutOfRange, "x <= 0 " << x);
      }

      if (x == 0.0) {
	return 0.0;
      }

      double ap  = a;
      double sum = 1.0 / a;
      double del = sum;

      for (int i = 0; i != max; ++i) {

	ap  += 1.0;
	del *= x/ap;
	sum += del;

	if (fabs(del) < fabs(sum)*numeric_limits<double>::epsilon()) {
	  return sum*exp(-x + a*log(x) - gln);
	}
      }

      THROW(JValueOutOfRange, "i == " << max);

    } else {

      const double FPMIN = numeric_limits<double>::min() / numeric_limits<double>::epsilon();

      double b = x + 1.0 - a;
      double c = 1.0 / FPMIN;
      double d = 1.0 / b;
      double h = d;

      for (int i = 1; i != max; ++i) {

	const double an = -i * (i-a);

	b += 2.0;
	d  = an*d + b;

	if (fabs(d) < FPMIN) {
	  d = FPMIN;
	}

	c  = b + an/c;

	if (fabs(c) < FPMIN) {
	  c = FPMIN;
	}

	d  = 1.0/d;

	const double del = d*c;
	
	h *= del;
	
	if (fabs(del - 1.0) < numeric_limits<double>::epsilon()) {
	  return 1.0 - exp(-x + a*log(x) - gln) * h;
	}
      }

      THROW(JValueOutOfRange, "i == " << max);
    }
  }


  /**
   * Legendre polynome.
   *
   * \param  n                    degree 
   * \param  x                    x
   * \return                      function value
   */
  inline double legendre(const size_t n, const double x)
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

	for (size_t i = 2; i <= n; ++i) {
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
  inline double binomial(const size_t n, const size_t k)
  {
    if (n == 0 || n < k) {
      return 0.0;
    }

    if (k == 0 || n == k) {
      return 1.0;
    }

    const int k1 = std::min(k, n - k);
    const int k2 = n - k1;

    double value = k2 + 1;

    for (int i = k1; i != 1; --i) {
      value *= (double) (k2 + i) / (double) i;
    }

    return value;
  }


  /**
   * Poisson probability density distribition.
   *
   * \param  n                    number of occurences
   * \param  mu                   expectation value
   * \return                      probability
   */
  inline double poisson(const size_t n, const double mu)
  {
    using namespace std;

    if (mu > 0.0) {

      if (n > 0)
	return exp(n*log(mu) - lgamma(n+1) - mu);
      else
	return exp(-mu);
    } else if (mu == 0.0) {

      return (n == 0 ? 1.0 : 0.0);
    }
 
    THROW(JValueOutOfRange, "mu <= 0 " << mu);
  }


  /**
   * Poisson cumulative density distribition.
   *
   * \param  n                    number of occurences
   * \param  mu                   expectation value
   * \return                      probability
   */
  inline double Poisson(const size_t n, const double mu)
  {
    return 1.0 - Gamma(n + 1, mu);
  }
}

#endif
