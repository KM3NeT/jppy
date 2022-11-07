#ifndef __JTOOLS__JGRIDMAP__
#define __JTOOLS__JGRIDMAP__

#include "JTools/JGridCollection.hh"
#include "JTools/JMapCollection.hh"
#include "JTools/JElement.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  
  /**
   * Map of equidistant pair-wise elements.
   *
   * The key_type and mapped_type refer to the pair-wise element of this map, respectively.
   */
  template<class JKey_t, 
	   class JValue_t, 
	   class JDistance_t = JDistance<JKey_t> >
  class JGridMap :
    public JGridCollection<JElement2D<JKey_t, JValue_t>, JDistance_t>
  {
  public:

    typedef JKey_t                                                               key_type;
    typedef JValue_t                                                             mapped_type;

    typedef JGridCollection<JElement2D<JKey_t, JValue_t>, JDistance_t>           collection_type;

    typedef typename collection_type::abscissa_type                              abscissa_type;
    typedef typename collection_type::ordinate_type                              ordinate_type;
    typedef typename collection_type::value_type                                 value_type;

    typedef typename collection_type::const_iterator                             const_iterator;
    typedef typename collection_type::const_reverse_iterator                     const_reverse_iterator;
    typedef typename collection_type::iterator                                   iterator;
    typedef typename collection_type::reverse_iterator                           reverse_iterator;


    /**
     * Default constructor.
     */
    JGridMap()
    {}
  };


  /**
   * Specialisation of JMapCollection for JGridMap.
   */
  template<>
  struct JMapCollection<JGridMap> {
    /**
     * Collection of elements.
     */
    template<class JElement_t, 
	     class JDistance_t = JDistance<typename JElement_t::abscissa_type> >
    struct collection_type :
      public JGridCollection<JElement_t, JDistance_t>
    {};
  };
}

#endif
