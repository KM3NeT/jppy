#ifndef __JTOOLS__JMULTIPAIR__
#define __JTOOLS__JMULTIPAIR__

#include "JTools/JMultiKey.hh"
#include "JLang/JClass.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  /**
   * Multidimensional pair.
   *
   * This class reproduces the element of a multidimensional map.
   * The individual data members can be accessed as:
   * <pre>
   *           JMultiPair<3, key_type, mapped_type>  pair;
   *
   *           pair[[.second].second].first;
   *           pair[[.second].second].second;
   * </pre>
   */
  template<unsigned int N, class JKey_t, class JValue_t>
  class JMultiPair 
  {
  public:

    typedef JKey_t                                              key_type;
    typedef JValue_t                                            value_type;
    typedef JMultiPair<N-1, key_type, value_type>               mapped_type;
    typedef typename JLANG::JClass<value_type>::reference_type  reference_type;
    typedef JMultiKey<N, key_type>                              multikey_type;


    /**
     * Default constructor.
     */
    JMultiPair() :
      first (),
      second()
    {}

    
    /**
     * Constructor.
     * The primary key is inserted at the start of the secondary keys.
     *
     * \param   __first      primary   key
     * \param   __second     secondary keys and value
     */
    JMultiPair(typename JClass<key_type>   ::argument_type __first,
	       typename JClass<mapped_type>::argument_type __second) :
      first (__first),
      second(__second)
    {}

    
    /**
     * Constructor.
     * The secondary key is appended to the end of the primary keys.
     *
     * \param   __first      primary   keys and value
     * \param   __second     secondary key
     */
    JMultiPair(typename JClass<mapped_type>::argument_type __first,
	       typename JClass<key_type>   ::argument_type __second) :
      first (__first.first),
      second(mapped_type(__first.second, __second))
    {}

    
    /**
     * Constructor.
     *
     * \param   key          multidimensional key
     * \param   value        value
     */
    JMultiPair(typename JClass<multikey_type>::argument_type key,
	       typename JClass<value_type>   ::argument_type value) :
      first (key.first),
      second(mapped_type(key.second, value))
    {}


    /**
     * Get multidimensional key.
     *
     * \return               multidimensional key
     */
    multikey_type getKey() const
    {
      return multikey_type(this->first, this->second.getKey());
    }


    /**
     * Get value.
     *
     * \return               value
     */
    reference_type getValue()
    { 
      return this->second.getValue();
    }


    /**
     * Get value.
     *
     * \return               value
     */
    const reference_type getValue() const 
    { 
      return this->second.getValue();
    }


    key_type    first;
    mapped_type second;
  };


  /**
   * Two-dimensional pair.
   */
  template<class JKey_t, class JValue_t>
  class JMultiPair<2, JKey_t, JValue_t> 
  {
  public:

    typedef JKey_t                                       key_type;
    typedef JValue_t                                     value_type;
    typedef JMultiPair<1, key_type, value_type>          mapped_type;
    typedef typename JClass<value_type>::reference_type  reference_type;
    typedef JMultiKey<2, key_type>                       multikey_type;


    /**
     * Default constructor.
     */
    JMultiPair() :
      first (),
      second()
    {}


    /**
     * Constructor.
     * The primary key is inserted at the start of the secondary key.
     *
     * \param   __first      primary   key
     * \param   __second     secondary key and value
     */
    JMultiPair(typename JClass<key_type>   ::argument_type __first,
	       typename JClass<mapped_type>::argument_type __second) :
      first (__first),
      second(__second)
    {}


    /**
     * Constructor.
     * The secondary key is appended to the end of the primary key.
     *
     * \param   __first      primary   keys and value
     * \param   __second     secondary key
     */
    JMultiPair(typename JClass<mapped_type>::argument_type __first,
	       typename JClass<key_type>   ::argument_type __second) :
      first (__first.first),
      second(mapped_type(__second, __first.second))
    {}

    
    /**
     * Constructor.
     *
     * \param   key          multidimensional key
     * \param   value        value
     */
    JMultiPair(typename JClass<multikey_type>::argument_type key,
	       typename JClass<value_type>   ::argument_type value) :
      first (key.first), 
      second(mapped_type(key.second, value))
    {}


    /**
     * Get multidimensional key.
     *
     * \return               multidimensional key
     */
    multikey_type getKey() const
    { 
      return multikey_type(this->first, this->second.getKey());
    }


    /**
     * Get value.
     *
     * \return               value
     */
    reference_type getValue()
    {
      return this->second.getValue();
    }


    /**
     * Get value.
     *
     * \return               value
     */
    const reference_type getValue() const
    { 
      return this->second.getValue();
    }


    key_type    first;
    mapped_type second;
  };


  /**
   * One-dimensional pair.
   */  
  template<class JKey_t, class JValue_t>
  class JMultiPair<1, JKey_t, JValue_t> 
  {
  public:

    typedef JKey_t                                       key_type;
    typedef JValue_t                                     value_type;
    typedef JValue_t                                     mapped_type;
    typedef typename JClass<value_type>::reference_type  reference_type;
    typedef JMultiKey<1, key_type>                       multikey_type;


    /**
     * Default constructor.
     */
    JMultiPair() :
      first (),
      second()
    {}


    /**
     * Constructor.
     *
     * \param   __first      key
     * \param   __second     value
     */
    JMultiPair(typename JClass<key_type>  ::argument_type __first,
	       typename JClass<value_type>::argument_type __second) :
      first (__first),
      second(__second)
    {}

    
    /**
     * Constructor.
     *
     * \param   key          multidimensional key
     * \param   value        value
     */
    JMultiPair(typename JClass<multikey_type>::argument_type key,
	       typename JClass<value_type>   ::argument_type value) :
      first (key.first),
      second(value)
    {}

    
    /**
     * Get multidimensional key.
     *
     * \return               multidimensional key
     */
    multikey_type getKey() const
    { 
      return JMultiKey<1, key_type>(this->first);
    }


    /**
     * Get value.
     *
     * \return               value
     */
    reference_type getValue()
    {
      return this->second;
    }


    /**
     * Get value.
     *
     * \return               value
     */
    const reference_type getValue() const
    { 
      return this->second;
    }


    key_type    first;
    mapped_type second;
  };


  /**
   * Empty pair.
   */
  template<class JKey_t, class JValue_t>
  class JMultiPair<0, JKey_t, JValue_t>
  {};
}

#endif
