#ifndef __JLANG__JEQUALS__
#define __JLANG__JEQUALS__

#include "JLang/JNullType.hh"
#include "JLang/JClass.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

   
  /**
   * Template definition of auxiliary base class for comparison of data structures.
   *
   * The various specialisations of this class implement the operators <tt> == != </tt>.
   */
  template<class JFirst_t, class JSecond_t = JNullType> 
  struct JEquals;


  /**
   * General purpose specialisation of class JEquals for any data type.
   *
   * The template parameter should have the corresponding member method:
   * <pre>
   *       bool equals(const JClass_t&) const;
   * </pre>
   *
   * This class uses in-class friend operators (see Barton-Nackman trick).
   */
  template<class JClass_t>
  struct JEquals<JClass_t, JNullType> 
  {
    /**
     * Equal operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if two objects are equal; else false
     */
    friend bool operator==(const JClass_t& first,
			   const JClass_t& second)
    { 
      return first.equals(second);
    }


    /**
     * Not equal operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if two objects are not equal; else false
     */
    friend bool operator!=(const JClass_t& first,
			   const JClass_t& second)
    {
      return !first.equals(second);
    }
  };
  

  /**
   * Specialisation of class JEquals for two data types.
   *
   * The first template parameter should have the corresponding member methods:
   * <pre>
   *       bool equals(const JFirst_t&) const;
   *       bool equals(const JSecond_t&) const;
   * </pre>
   * where <tt>JFirst_t</tt> and <tt>JSecond_t</tt> refer to the first and second template parameter, respectively.
   * The second template parameter may be a primitive data type.
   * This class uses in-class friend operators (see Barton-Nackman trick).
   */
  template<class JFirst_t, class JSecond_t>
  struct JEquals :
    public JEquals<JFirst_t>
  {
    /**
     * Equal operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if two objects are equal; else false
     */
    friend bool operator==(const JFirst_t& first,
			   typename JClass<JSecond_t>::argument_type second)
    { 
      return first.equals(second);
    }


    /**
     * Equal operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if two objects are equal; else false
     */
    friend bool operator==(typename JClass<JSecond_t>::argument_type first,
			   const JFirst_t& second)
    { 
      return second.equals(first);
    }


    /**
     * Not equal operator.
     *
     * \param  first        first object
     * \param  second       second object
     * \return              true if two objects are not equal; else false
     */
    friend bool operator!=(const JFirst_t& first,
			   typename JClass<JSecond_t>::argument_type second)
    {
      return !first.equals(second);
    }


    /**
     * Not equal operator.
     *
     * \param  first        first object
     * \param  second       second object
     * \return              true if two objects are not equal; else false
     */
    friend bool operator!=(typename JClass<JSecond_t>::argument_type first,
			   const JFirst_t& second)
    {
      return !second.equals(first);
    }
  };
}

#endif
