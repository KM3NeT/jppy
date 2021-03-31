#ifndef __JLANG__JCOMPARABLE__
#define __JLANG__JCOMPARABLE__

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
   * The various specialisations of this class implement the operators <tt> < > <= >= == != </tt>.
   */
  template<class JFirst_t, class JSecond_t = JNullType> 
  struct JComparable;


  /**
   * General purpose specialisation of class JComparable for any data type.
   *
   * The template parameter should have the corresponding member method:
   * <pre>
   *       bool less(const JClass_t&) const;
   * </pre>
   *
   * This class uses in-class friend operators (see Barton-Nackman trick).
   */
  template<class JClass_t>
  struct JComparable<JClass_t, JNullType>
  {
    /**
     * Less than operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if first object is less than second object; else false
     */
    friend bool operator<(const JClass_t& first,
			  const JClass_t& second)
    { 
      return first.less(second);
    }

 
    /**
     * Greater than operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if first object is greater than second object; else false
     */
    friend bool operator>(const JClass_t& first,
			  const JClass_t& second)
    { 
      return second.less(first);
    }
    

    /**
     * Less than or equal operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if first object is less than or equal to second object; else false
     */
    friend bool operator<=(const JClass_t& first,
			   const JClass_t& second)
    { 
      return !second.less(first);
    }
    

    /**
     * Greater than or equal operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if first object is greater than or equal to second object; else false
     */
    friend bool operator>=(const JClass_t& first,
			   const JClass_t& second)
    { 
      return !first.less(second);
    }
    

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
      return !first.less(second) && !second.less(first);
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
      return first.less(second) || second.less(first);
    }
  };
  

  /**
   * Specialisation of class JComparable for two data types.
   *
   * The first template parameter should have the corresponding member methods:
   * <pre>
   *       bool less(const JSecond_t&) const;
   *       bool more(const JSecond_t&) const;
   * </pre>
   * where <tt>JFirst_t</tt> andd <tt>JSecond_t</tt> refers to the first and second template parameter, respectively.
   * The second template parameter may be a primitive data type.
   * This class uses in-class friend operators (see Barton-Nackman trick).
   */
  template<class JFirst_t, class JSecond_t>
  struct JComparable
  {
    /**
     * Less than operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if first object is less than second object; else false
     */
    friend bool operator<(const JFirst_t& first,
			  typename JClass<JSecond_t>::argument_type second)
    { 
      return first.less(second);
    }


    /**
     * Less than operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if first object is less than second object; else false
     */
    friend bool operator<(typename JClass<JSecond_t>::argument_type first,
			  const JFirst_t& second)			  
    { 
      return second.more(first);
    }


    /**
     * Greater than operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if first object is greater than second object; else false
     */
    friend bool operator>(const JFirst_t& first,
			  typename JClass<JSecond_t>::argument_type second)
    { 
      return first.more(second);
    }


    /**
     * Greater than operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if first object is greater than second object; else false
     */
    friend bool operator>(typename JClass<JSecond_t>::argument_type first,
			  const JFirst_t& second)			  
    { 
      return second.less(first);
    }


    /**
     * Less than or equal operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if first object is less than or equal to second object; else false
     */
    friend bool operator<=(const JFirst_t& first,
			   typename JClass<JSecond_t>::argument_type second)
    { 
      return !first.more(second);
    }


    /**
     * Less than or equal operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if first object is less than or equal to second object; else false
     */
    friend bool operator<=(typename JClass<JSecond_t>::argument_type first,
			   const JFirst_t& second)
    { 
      return second.more(first);
    }
    

    /**
     * Greater than or equal operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if first object is greater than or equal to second object; else false
     */
    friend bool operator>=(const JFirst_t& first,
			   typename JClass<JSecond_t>::argument_type second)
    { 
      return !first.less(second);
    }
    

    /**
     * Greater than or equal operator.
     *
     * \param  first        first  object
     * \param  second       second object
     * \return              true if first object is greater than or equal to second object; else false
     */
    friend bool operator>=(typename JClass<JSecond_t>::argument_type first,
			   const JFirst_t& second)
			   
    { 
      return second.less(first);
    }
    

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
      return !first.less(second) && !first.more(second);
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
      return !second.less(first) && !second.more(first);
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
      return first.less(second) || first.more(second);
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
      return second.less(first) || second.more(first);
    }
  };
}

#endif
