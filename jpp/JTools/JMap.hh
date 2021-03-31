#ifndef __JTOOLS__JMAP__
#define __JTOOLS__JMAP__

#include "JTools/JCollection.hh"
#include "JTools/JMapCollection.hh"
#include "JTools/JElement.hh"
#include "JMath/JMath.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JMATH::JMath;


  /**
   * Map of pair-wise elements.
   *
   * The key_type and mapped_type refer to the pair-wise element of this map, respectively.
   */
  template<class JKey_t,
	   class JValue_t,
	   class JDistance_t = JDistance<JKey_t> >
  class JMap :
    public JCollection<JElement2D<JKey_t, JValue_t>, JDistance_t>,
    public JMath< JMap<JKey_t, JValue_t, JDistance_t> >
  {
  public:

    typedef JKey_t                                                               key_type;
    typedef JValue_t                                                             mapped_type;
    
    typedef JCollection<JElement2D<JKey_t, JValue_t>, JDistance_t>               container_type;

    typedef typename container_type::const_iterator                              const_iterator;
    typedef typename container_type::const_reverse_iterator                      const_reverse_iterator;
    typedef typename container_type::iterator                                    iterator;
    typedef typename container_type::reverse_iterator                            reverse_iterator;


    /**
     * Default constructor.
     */
    JMap()
    {}


    /**
     * Add map.
     *
     * \param  map             map
     * \return                 this map
     */
    JMap& add(const JMap& map)
    {
      static_cast<container_type&>(*this).add(static_cast<const container_type&>(map));

      return *this;
    }
 

    /**
     * Subtract map.
     *
     * \param  map             map
     * \return                 this map
     */
    JMap& sub(const JMap& map)
    {
      static_cast<container_type&>(*this).sub(static_cast<const container_type&>(map));

      return *this;
    }


    /**
     * Scale contents.
     *
     * \param  value           multiplication factor
     * \return                 this map
     */
    JMap& mul(const double value)
    {
      static_cast<container_type&>(*this).mul(value);

      return *this;
    }

    /**
     * Scale contents.
     *
     * \param  value           division factor
     * \return                 this map
     */
    JMap& div(const double value)
    {
      static_cast<container_type&>(*this).div(value);

      return *this;
    }
  };


  /**
   * Specialisation of JMapCollection for JMap.
   */
  template<>
  struct JMapCollection<JMap> {
    /**
     * Collection of elements.
     */
    template<class JElement_t, 
	     class JDistance_t = JDistance<typename JElement_t::abscissa_type> >
    struct collection_type :
      public JCollection<JElement_t, JDistance_t>
    {};
  };
}

#endif
