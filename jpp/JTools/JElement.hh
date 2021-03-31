#ifndef __JTOOLS__JELEMENT__
#define __JTOOLS__JELEMENT__

#include <cmath>

#include "JMath/JZero.hh"
#include "JMath/JMath.hh"
#include "JLang/JClass.hh"
#include "JIO/JSerialisable.hh"


/**
 * \file
 *
 * The elements in a collection are sorted according to their abscissa values and a given distance operator.
 * These elements should have the following type definitions and member methods:
 * <pre>
 *       typedef  \<abscissa type\>    abscissa_type;
 *       typedef  \<ordinate type\>    ordinate_type;
 *
 *       constructor();
 *       constructor(abscissa_type, ordinate_type);
 *
 *       abscissa_type        getX() const;
 *       const ordinate_type& getY() const;
 *       ordinate_type&       getY();
 * </pre>
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JIO::JReader;
  using JIO::JWriter;
  using JMATH::JMath;
  using JMATH::getZero;


  /**
   * 2D Element.
   */
  template<class JAbscissa_t, class JOrdinate_t>
  struct JElement2D {

    typedef JAbscissa_t                                          abscissa_type;
    typedef JOrdinate_t                                          ordinate_type;


    /**
     * Default constructor.
     */
    JElement2D() :
      __x(getZero<abscissa_type>()),
      __y(getZero<ordinate_type>())
    {}
    

    /**
     * Constructor.
     *
     * \param  x               abscissa value
     * \param  y               ordinate value
     */
    JElement2D(typename JLANG::JClass<abscissa_type>::argument_type x,
	       typename JLANG::JClass<ordinate_type>::argument_type y) :
      __x(x),
      __y(y)
    {}
    
    
    /**
     * Get abscissa value.
     *
     * \return                 abscissa value
     */
    abscissa_type getX() const 
    { 
      return __x;
    }
    
    
    /**
     * Get ordinate value.
     *
     * \return                 ordinate value
     */
    const ordinate_type& getY() const 
    { 
      return __y;
    }
    
    
    /**
     * Get ordinate value.
     *
     * \return                 ordinate value
     */
    ordinate_type& getY()
    { 
      return __y;
    }


    /**
     * Read element from input.
     *
     * \param  in              reader
     * \param  element         element
     * \return                 reader
     */
    friend inline JReader& operator>>(JReader& in, JElement2D& element)
    {
      in >> element.__x;
      in >> element.__y;

      return in;
    }


    /**
     * Write element to output.
     *
     * \param  out             writer
     * \param  element         element
     * \return                 writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JElement2D& element)
    {
      out << element.__x;
      out << element.__y;

      return out;
    }


  protected:    
    abscissa_type __x;
    ordinate_type __y;
  };


  /**
   * 2D Element for spline interpolations.
   *
   * Note that the internal data members needed for the calculation
   * of the 2nd derivatives are not subject to I/O, i.e.\ the I/O of 
   * this class is identical to that of the JElement2D class.
   */
  template<class JAbscissa_t, class JOrdinate_t>
  struct JSplineElement2D :
    public JElement2D<JAbscissa_t, JOrdinate_t>
  {

    typedef JElement2D<JAbscissa_t, JOrdinate_t>                 element_type;
    typedef typename element_type::abscissa_type                 abscissa_type;
    typedef typename element_type::ordinate_type                 ordinate_type;


    /**
     * Default constructor.
     */
    JSplineElement2D() :
      element_type(),
      __u(getZero<ordinate_type>())
    {}
    

    /**
     * Constructor.
     *
     * \param  x               abscissa value
     * \param  y               ordinate value
     */
    JSplineElement2D(typename JLANG::JClass<abscissa_type>::argument_type x,
		     typename JLANG::JClass<ordinate_type>::argument_type y) :
      element_type(x, y),
      __u(getZero<ordinate_type>())
    {}


    /**
     * Get derivative.
     *
     * \return                 derivative
     */
    ordinate_type getU() const
    { 
      return __u;
    }


    /**
     * Set derivative.
     *
     * \param  u               derivative
     */
    void setU(typename JLANG::JClass<ordinate_type>::argument_type u)
    { 
      __u= u;
    }


  protected:    
    ordinate_type __u;
  };


  /**
   * 2D Element for spline interpolations.
   *
   * Note that the internal data members needed for the calculation
   * of the integral values are not subject to I/O, i.e.\ the I/O of 
   * this class is identical to that of the JElement2D class.
   */
  template<class JAbscissa_t, class JOrdinate_t>
  struct JSplineElement2S :
    public JSplineElement2D<JAbscissa_t, JOrdinate_t>
  {

    typedef JSplineElement2D<JAbscissa_t, JOrdinate_t>           element_type;
    typedef typename element_type::abscissa_type                 abscissa_type;
    typedef typename element_type::ordinate_type                 ordinate_type;


    /**
     * Default constructor.
     */
    JSplineElement2S() :
      element_type(),
      __v(getZero<ordinate_type>())
    {}
    

    /**
     * Constructor.
     *
     * \param  x               abscissa value
     * \param  y               ordinate value
     */
    JSplineElement2S(typename JLANG::JClass<abscissa_type>::argument_type x,
		     typename JLANG::JClass<ordinate_type>::argument_type y) :
      element_type(x, y),
      __v(getZero<ordinate_type>())
    {}


    /**
     * Get integral.
     *
     * \return                 integral
     */
    ordinate_type getIntegral() const
    { 
      return __v;
    }


    /**
     * Set integral.
     *
     * \param  v               integral
     */
    void setIntegral(typename JLANG::JClass<ordinate_type>::argument_type v)
    { 
      __v = v;
    }


  protected:    
    ordinate_type __v;
  };


  /**
   * 2D Element for polynomial interpolations.
   *
   * Note that the internal data members needed for the calculation
   * of the integral values are not subject to I/O, i.e.\ the I/O of 
   * this class is identical to that of the JElement2D class.
   */
  template<class JAbscissa_t, class JOrdinate_t>
  struct JPolintElement2S :
    public JElement2D<JAbscissa_t, JOrdinate_t>
  {

    typedef JElement2D<JAbscissa_t, JOrdinate_t>                 element_type;
    typedef typename element_type::abscissa_type                 abscissa_type;
    typedef typename element_type::ordinate_type                 ordinate_type;


    /**
     * Default constructor.
     */
    JPolintElement2S() :
      element_type(),
      __v(getZero<ordinate_type>())
    {}
    

    /**
     * Constructor.
     *
     * \param  x               abscissa value
     * \param  y               ordinate value
     */
    JPolintElement2S(typename JLANG::JClass<abscissa_type>::argument_type x,
		     typename JLANG::JClass<ordinate_type>::argument_type y) :
      element_type(x, y),
      __v(getZero<ordinate_type>())
    {}


    /**
     * Get integral.
     *
     * \return                 integral
     */
    ordinate_type getIntegral() const
    { 
      return __v;
    }


    /**
     * Set integral.
     *
     * \param  v               integral
     */
    void setIntegral(typename JLANG::JClass<ordinate_type>::argument_type v)
    { 
      __v = v;
    }


  protected:    
    ordinate_type __v;
  };


  /**
   * 2D Binned element.
   *
   * Note that the internal data members needed for the calculation
   * of the bin center are not subject to I/O, i.e.\ the I/O of 
   * this class is identical to that of the JElement2D class.
   */
  template<class JAbscissa_t, class JOrdinate_t>
  struct JBin2D :
    public JElement2D<JAbscissa_t, JOrdinate_t>,
    public JMath< JBin2D<JAbscissa_t, JOrdinate_t> >
  {

    typedef JElement2D<JAbscissa_t, JOrdinate_t>                 element_type;
    typedef typename element_type::abscissa_type                 abscissa_type;
    typedef typename element_type::ordinate_type                 ordinate_type;
    

    /**
     * Default constructor.
     */
    JBin2D() :
      element_type(),
      __z (getZero<ordinate_type>()),
      __w2(getZero<ordinate_type>())
    {}
    

    /**
     * Constructor.
     *
     * \param  x               abscissa value
     * \param  y               ordinate value
     */
    JBin2D(typename JLANG::JClass<abscissa_type>::argument_type x,
	   typename JLANG::JClass<ordinate_type>::argument_type y) :
      element_type(x,y),
      __z (getZero<ordinate_type>()),
      __w2(getZero<ordinate_type>())
    {}

    
    /**
     * Add abscissa value.
     *
     * \param  x               abscissa value
     * \param  w               weight
     */
    void fill(typename JLANG::JClass<abscissa_type>::argument_type x,
	      typename JLANG::JClass<ordinate_type>::argument_type w)
    { 
      this->__y  += w;
      this->__z  += w * x;
      this->__w2 += w * w;
    }
    

    /**
     * Get bin center.
     *
     * \return                 center
     */
    abscissa_type getBinCenter() const
    { 
      if (this->__y != 0) 
	return this->__z / this->__y;
      else
	return this->__x;
    }
    

    /**
     * Get bin content.
     *
     * \return                 content
     */
    ordinate_type getBinContent() const
    { 
      return this->__y;
    }
    

    /**
     * Get bin error.
     *
     * \return                 error
     */
    ordinate_type getBinError() const
    { 
      return sqrt(this->__w2);
    }


    /**
     * Add bin.
     *
     * \param  bin             bin
     * \return                 this bin
     */
    JBin2D& add(const JBin2D& bin)
    {
      this->__y  += bin.__y;
      this->__z  += bin.__zl;
      this->__w2 += bin.__w2;

      return *this;
    }


    /**
     * Scale contents.
     *
     * \param  value           multiplication factor
     * \return                 this bin
     */
    JBin2D& mul(const double value)
    {
      this->__y  *= value;
      this->__z  *= value;
      this->__w2 *= value*value;

      return *this;
    }


    /**
     * Scale contents.
     *
     * \param  value           division factor
     * \return                 this bin
     */
    JBin2D& div(const double value)
    {
      this->__y  /= value;
      this->__z  /= value;
      this->__w2 /= value*value;

      return *this;
    }


  protected:    
    ordinate_type __z;
    ordinate_type __w2;
  };


  /**
   * 3D Element.
   */
  template<class JAbscissa_t, class JOrdinate_t>
  struct JElement3D {

    typedef JAbscissa_t                                          abscissa_type;
    typedef JOrdinate_t                                          ordinate_type;


    /**
     * Default constructor.
     */
    JElement3D() :
      __x(getZero<abscissa_type>()),
      __y(getZero<abscissa_type>()),
      __z(getZero<ordinate_type>())
    {}
    

    /**
     * Constructor.
     *
     * \param  x               abscissa value
     * \param  y               abscissa value
     * \param  z               ordinate value
     */
    JElement3D(typename JLANG::JClass<abscissa_type>::argument_type x,
	       typename JLANG::JClass<abscissa_type>::argument_type y,
	       typename JLANG::JClass<ordinate_type>::argument_type z) :
      __x(x),
      __y(y),
      __z(z)
    {}
    
    
    /**
     * Get abscissa value.
     *
     * \return                 abscissa value
     */
    abscissa_type getX() const 
    { 
      return __x;
    }
    
    
    /**
     * Get abscissa value.
     *
     * \return                 abscissa value
     */
    abscissa_type getY() const 
    { 
      return __y;
    }
    
    
    /**
     * Get ordinate value.
     *
     * \return                 ordinate value
     */
    const ordinate_type& getZ() const 
    { 
      return __y;
    }
    
    
    /**
     * Get ordinate value.
     *
     * \return                 ordinate value
     */
    ordinate_type& getZ()
    { 
      return __y;
    }


    /**
     * Read element from input.
     *
     * \param  in              reader
     * \param  element         element
     * \return                 reader
     */
    friend inline JReader& operator>>(JReader& in, JElement3D& element)
    {
      in >> element.__x;
      in >> element.__y;
      in >> element.__z;

      return in;
    }


    /**
     * Write element to output.
     *
     * \param  out             writer
     * \param  element         element
     * \return                 writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JElement3D& element)
    {
      out << element.__x;
      out << element.__y;
      out << element.__z;

      return out;
    }


  protected:    
    abscissa_type __x;
    abscissa_type __y;
    ordinate_type __z;
  };
}

#endif
