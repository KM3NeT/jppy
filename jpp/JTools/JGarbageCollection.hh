#ifndef __JTOOLS__JGARBAGECOLLACTION__
#define __JTOOLS__JGARBAGECOLLACTION__

#include "JTools/JMappableCollection.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  /**
   * Garbage collection.
   *
   * This class implements the JMappableCollection interface but does nothing.
   */
  template<class JKey_t, class JValue_t>
  class JGarbageCollection :
    public JMappableCollection<JKey_t, JValue_t>
  {
  public:

    typedef JMappableCollection<JKey_t, JValue_t>                mappablecollection_type;

    typedef typename mappablecollection_type::key_type           key_type;
    typedef typename mappablecollection_type::mapped_type        mapped_type;


    /**
     * Clear.
     */
    virtual void clear() override 
    {}


    /**
     * Get mapped value.
     *
     * \param  key             key
     * \return                 value
     */
    virtual const mapped_type& get(typename JLANG::JClass<key_type>::argument_type key) const override 
    {
      return value;
    }


    /**
     * Get mapped value.
     *
     * \param  key             key
     * \return                 value
     */
    virtual mapped_type& get(typename JLANG::JClass<key_type>::argument_type key) override 
    {
      return value;
    }

  private:
    mapped_type value;
  };
}

#endif
