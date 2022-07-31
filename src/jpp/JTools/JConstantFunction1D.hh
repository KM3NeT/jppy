#ifndef __JCONSTANTFUNCTION1D__
#define __JCONSTANTFUNCTION1D__

#include "JIO/JSerialisable.hh"
#include "JTools/JFunctional.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JIO::JReader;
  using JIO::JWriter;

  /**
   * Template implementation of function object in one dimension returning a constant value.
   *
   * This class implements the JFunction1D interface.
   */
  template<class JArgument_t, class JResult_t>
  class JConstantFunction1D : 
    public JFunction1D<JArgument_t, JResult_t>
  {
  public:

    typedef JFunction1D<JArgument_t, JResult_t>                         function_type;
    typedef typename function_type::argument_type                       argument_type;
    typedef typename function_type::result_type                         result_type;


    /**
     * Default constructor.
     */
    JConstantFunction1D() :
      function_type(),
      __y()
    {}


    /**
     * Constructor.
     *
     * \param  y               value
     */
    JConstantFunction1D(const result_type y) :
      function_type(),
      __y(y)
    {}


    /**
     * Constructor.
     *
     * \param  y               zero
     */
    JConstantFunction1D(const JMATH::JZero& y) :
      function_type(),
      __y(y)
    {}


    /**
     * Add offset.
     *
     * \param  value           offset
     */
    JConstantFunction1D& add(typename JLANG::JClass<result_type>::argument_type value)
    {
      __y += value;

      return *this;
    }


    /**
     * Subtract offset.
     *
     * \param  value            offset
     */
    JConstantFunction1D& sub(typename JLANG::JClass<result_type>::argument_type value)
    {
      __y -= value;

      return *this;
    }


    /**
     * Scale contents.
     *
     * \param  value           multiplication factor
     */
    JConstantFunction1D& mul(const double value)
    {
      __y *= value;

      return *this;
    }


    /**
     * Scale contents.
     *
     * \param  value           division factor
     */
    JConstantFunction1D& div(const double value)
    {
      __y /= value;

      return *this;
    }


    /**
     * Add function.
     *
     * \param  function        function
     */
    JConstantFunction1D& add(const JConstantFunction1D& function)
    {
      __y += function.getY();

      return *this;
    }


    /**
     * Subtract function.
     *
     * \param  function        function
     */
    JConstantFunction1D& sub(const JConstantFunction1D& function)
    {
      __y -= function.getY();

      return *this;
    }


    /**
     * Function value.
     *
     * \return                 function value
     */
    result_type getY() const 
    {
      return __y;
    }


    /**
     * Function value evaluation.
     *
     * \param  pX              pointer to abscissa values
     * \return                 function value
     */
    virtual result_type evaluate(const argument_type* pX) const override 
    {
      return __y;
    }


    /**
     * Read function from input.
     *
     * \param  in              reader
     * \param  function        function
     * \return                 reader
     */
    friend inline JReader& operator>>(JReader& in, JConstantFunction1D& function)
    {
      return in >> function.__y;
    }


    /**
     * Write function to output.
     *
     * \param  out             writer
     * \param  function        function
     * \return                 writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JConstantFunction1D& function)
    {
      return out << function.__y;
    }


  protected:
    /**
     * Function compilation.
     */
    virtual void do_compile() override 
    {}

  private:
    result_type __y;
  };
}

#endif
