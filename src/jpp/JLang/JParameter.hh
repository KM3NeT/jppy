#ifndef __JLANG__JPARAMETER__
#define __JLANG__JPARAMETER__

#include <istream>
#include <ostream>

#include "JLang/JClass.hh"
#include "JLang/JComparable.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {
  
  
  /**
   * Parameter class.
   *
   * This class is a simple wrapper around the template parameter with an additional status value.\n
   * The status value indicates whether the parameter has been defined or not.\n
   * A parameter is defined when a value has been assigned or correctly read.\n
   * Note that the comparison between parameter objects is based on the philosophy "undefined = any value".\n
   * Hence, if any of the two parameter values is undefined, they are considered equal.\n
   * The comparison between a parameter object with a template value is based 
   * on the internal value of the parameter object via implicit type conversion,
   * regardless of its state.
   */
  template<class T>
  class JParameter :
    public JComparable< JParameter<T> >
  {
  public:

    typedef typename JClass<T>::argument_type  argument_type;

    /**
     * Default constructor.
     */
    JParameter() :
      __value(),
      is_defined(false)
    {}

  
    /**
     * Constructor.
     *
     * \param  value        value
     */
    explicit JParameter(const argument_type& value) :
      __value(value),
      is_defined(true)
    {}


    /**
     * Assignment operator.
     *
     * \param  value        value
     * \return              this parameter
     */
    JParameter<T>& operator=(const argument_type& value)
    {
      setValue(value);

      return *this;
    }


    /**
     * Get value of parameter.
     *
     * \return              value
     */
    const T& getValue() const 
    { 
      return __value; 
    }


    /**
     * Get value of parameter.
     *
     * \return              value
     */
    T& getValue() 
    { 
      return __value; 
    }


    /**
     * Set value.
     *
     * \param  value        value
     */
    void setValue(const argument_type& value)
    {
      __value    = value;
      is_defined = true;
    }


    /**
     * Type conversion operator.
     *
     * \return              value
     */
    operator const T&() const
    { 
      return getValue();
    }


    /**
     * Type conversion operator.
     *
     * \return              value
     */
    operator T&()       
    {
      return getValue();
    }


    /**
     * Get status of parameter.
     *
     * \return              true if value has been defined (by read or assignment); else false
     */
    const bool isDefined() const
    {
      return is_defined;
    }


    /**
     * Less than method.
     *
     * This method evaluates to true if both parameter values are defined and
     * this value is less than the value of the given parameter object.
     *
     * \param   parameter   parameter
     * \return              true if both defined and first value less than second value; else false
     */
    inline bool less(const JParameter<T>& parameter) const
    {
      return this->isDefined() && parameter.isDefined() && this->getValue() < parameter.getValue();
    }
    

    /**
     * Stream input.
     *
     * \param  in           input stream
     * \param  parameter    parameter
     * \return              input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JParameter<T>& parameter)
    {
      in >> parameter.__value;

      parameter.is_defined = (bool) in;

      return in;
    }


    /**
     * Stream output.
     *
     * \param  out          output stream
     * \param  parameter    parameter
     * \return              output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JParameter<T>& parameter)
    {
      if (parameter.is_defined) {
	out << parameter.__value;
      }
      
      return out;
    }


  protected:
    T    __value;
    bool is_defined;
  };
}

#endif
