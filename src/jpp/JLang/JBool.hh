#ifndef __JLANG__JBOOL__
#define __JLANG__JBOOL__


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Auxiliary template class for type bool.
   * This class can be used for boolean expressions at compile time.
   */ 
  template<bool __value__> 
  struct JBool 
  {
    /**
     * Type definition of bool value.
     */
    typedef JBool<__value__>  bool_type;


    /**
     * Default construcor.
     */
    JBool()
    {}

    
    /**
     * Value.
     */
    static const bool value = __value__;


    /**
     * Type conversion operator.
     *
     * \return             value
     */
    operator bool() const
    {
      return value;
    }
    

    /**
     * Make logical NOT.
     *
     * \return             logical NOT
     */
    static JBool<!value> c_not()
    {
      return JBool<!value>();
    }


    /**
     * Make logical EQUALS.
     *
     * \return             logical EQUALS
     */
    template<bool option>
    static JBool<value == option> c_equals()
    {
      return JBool<value == option>();
    }


    /**
     * Make logical EQUALS.
     *
     * \param  object      value
     * \return             logical EQUALS
     */
    template<bool option>
    static JBool<value == option> c_equals(const JBool<option>& object)
    {
      return bool_type::c_equals<option>();
    }


    /**
     * Make logical AND.
     *
     * \return             logical AND
     */
    template<bool option>
    static JBool<value && option> c_and()
    {
      return JBool<value && option>();
    }


    /**
     * Make logical AND.
     *
     * \param  object      value
     * \return             logical AND
     */
    template<bool option>
    static JBool<value && option> c_and(const JBool<option>& object)
    {
      return bool_type::c_and<option>();
    }


    /**
     * Make logical OR.
     *
     * \return             logical OR
     */
    template<bool option>
    static JBool<value || option> c_or()
    {
      return JBool<value || option>();
    }


    /**
     * Make logical OR.
     *
     * \param  object      value
     * \return             logical OR
     */
    template<bool option>
    static JBool<value || option> c_or(const JBool<option>& object)
    {
      return bool_type::c_or<option>();
    }


    /**
     * Make logical XOR.
     *
     * \return             logical XOR
     */
    template<bool option>
    static JBool<value != option> c_xor()
    {
      return JBool<value != option>();
    }


    /**
     * Make logical XOR.
     *
     * \param  object      value
     * \return             logical XOR
     */
    template<bool option>
    static JBool<value != option> c_xor(const JBool<option>& object)
    {
      return bool_type::c_xor<option>();
    }


    /**
     * Make logical SWITCH.
     * If value is true, select first, else select second.
     *
     * \return             logical SWITCH
     */
    template<bool __first__, bool __second__>
    static JBool<(value && __first__) || (!value && __second__)> c_switch()
    {
      return JBool<(value && __first__) || (!value && __second__)>();
    }


    /**
     * Make logical SWITCH.
     * If value is true, select first, else select second.
     *
     * \param  first       first  value
     * \param  second      second value
     * \return             logical SWITCH
     */
    template<bool __first__, bool __second__>
    static JBool<(value && __first__) || (!value && __second__)> c_switch(const JBool<__first__>&  first,
									  const JBool<__second__>& second)
    {
      return bool_type::c_switch<__first__, __second__>();
    }
  };


  /**
   * Make logical NOT.
   *
   * \param  value       value
   * \return             logical NOT
   */
  template<bool __value__>
  static JBool<!__value__> c_not(const JBool<__value__>& value)
  {
    return value.c_not();
  }
  

  /**
   * Make logical EQUALS.
   *
   * \param  first       first  value
   * \param  second      second value
   * \return             logical EQUALS
   */
  template<bool __first__, bool __second__>
  inline JBool<__first__ == __second__> c_equals(const JBool<__first__>&  first,
						 const JBool<__second__>& second)
  {
    return first.c_equals(second);
  }


  /**
   * Make logical AND.
   *
   * \param  first       first  value
   * \param  second      second value
   * \return             logical AND
   */
  template<bool __first__, bool __second__>
  inline JBool<__first__ == __second__> c_and(const JBool<__first__>&  first,
					      const JBool<__second__>& second)
  {
    return first.c_and(second);
  }


  /**
   * Make logical OR.
   *
   * \param  first       first  value
   * \param  second      second value
   * \return             logical OR
   */
  template<bool __first__, bool __second__>
  inline JBool<__first__ == __second__> c_or(const JBool<__first__>&  first,
					     const JBool<__second__>& second)
  {
    return first.c_or(second);
  }


  /**
   * Make logical XOR.
   *
   * \param  first       first  value
   * \param  second      second value
   * \return             logical XOR
   */
  template<bool __first__, bool __second__>
  inline JBool<__first__ == __second__> c_xor(const JBool<__first__>&  first,
					      const JBool<__second__>& second)
  {
    return first.c_xor(second);
  }


  typedef JBool<true>    JTRUE;         //!< True  type
  typedef JBool<false>   JFALSE;        //!< False type
  
  static const JTRUE     JTrue_t;       //!< True  value
  static const JFALSE    JFalse_t;      //!< False value

  
  /**
   * Type definition of logical EQUALS.
   */
  template<class JFirst_t, class JSecond_t>
  struct EQUALS;

  /**
   * Template specialisation for logical EQUALS.
   */
  template<bool first, bool second>
  struct EQUALS< JBool<first>, JBool<second> > :
    public JBool<first == second>
  {};
  

  /**
   * Type definition of logical NOT.
   */
  template<class T>
  struct NOT;

  /**
   * Template specialisation for logical NOT.
   */
  template<bool __value__>
  struct NOT< JBool<__value__> > :
    public JBool<!__value__>
  {};
  
  
  /**
   * Type definition of logical AND.
   */
  template<class JFirst_t, class JSecond_t>
  struct AND;

  /**
   * Template specialisation for logical AND.
   */
  template<bool first, bool second>
  struct AND< JBool<first>, JBool<second> > :
    public JBool<first && second>
  {};


  /**
   * Type definition of logical OR.
   */
  template<class JFirst_t, class JSecond_t>
  struct OR;
  
  /**
   * Template specialisation for logical OR.
   */  
  template<bool first, bool second>
  struct OR< JBool<first>, JBool<second> > :
    public JBool<first || second>
  {};


  /**
   * Type definition of logical XOR.
   */
  template<class JFirst_t, class JSecond_t>
  struct XOR;
  
  /**
   * Template specialisation for logical XOR.
   */  
  template<bool first, bool second>
  struct XOR< JBool<first>, JBool<second> > :
    public JBool<first != second>
  {};
}

#endif
