#ifndef __JTOOLS__JMAPPABLECOLLECTION__
#define __JTOOLS__JMAPPABLECOLLECTION__

#include "JLang/JClass.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JLANG::JClass;


  /**
   * Template interface definition for associative collection of elements.
   * The concrete associative collection should implement the following methods.
   * <pre> 
   *             void clear();
   *
   *             mapped_type& get(key_type key);
   * </pre>
   * This class implements the operator <tt>[key_type key]</tt> and method <tt>put(key_type key, mapped_type value)</tt>.
   */
  template<class JKey_t, class JValue_t>
  struct JMappableCollection
  {

    typedef JKey_t                                               key_type;
    typedef JValue_t                                             mapped_type;

    /**
     * Virtual destructor.
     */
    virtual ~JMappableCollection()
    {}


    /**
     * Clear.
     */
    virtual void clear() = 0;


    /**
     * Get mapped value.
     *
     * \param  key             key
     * \return                 value
     */
    virtual const mapped_type& get(typename JClass<key_type>::argument_type key) const = 0;


    /**
     * Get mapped value.
     *
     * \param  key             key
     * \return                 value
     */
    virtual mapped_type& get(typename JClass<key_type>::argument_type key) = 0;


    /**
     * Get mapped value.
     *
     * \param  key             key
     * \return                 value
     */
    const mapped_type& operator[](typename JClass<key_type>::argument_type key) const
    {
      return get(key);
    }


    /**
     * Get mapped value.
     *
     * \param  key             key
     * \return                 value
     */
    mapped_type& operator[](typename JClass<key_type>::argument_type key) 
    {
      return get(key);
    }


    /**
     * Put pair-wise element (key,value) into collection. 
     *
     * \param  key             key
     * \param  value           value
     */
    void put(typename JClass<key_type>   ::argument_type key,
	     typename JClass<mapped_type>::argument_type value)
    {
      get(key) = value;
    }
  };


  /**
   * Auxiliary class to define JMappableCollection for given template.
   */
  template<class T>
  struct JMappable {
    /**
     * Type definition of JMappableCollection.
     */
    typedef JMappableCollection<typename T::abscissa_type, 
				typename T::ordinate_type>   map_type;
  };
}

#endif
