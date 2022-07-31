#ifndef __JTOOLS__JABSTRACTMULTIMAP__
#define __JTOOLS__JABSTRACTMULTIMAP__

#include "JTools/JAbstractCollection.hh"
#include "JTools/JMultiKey.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  /**
   * Abstract interface for abscissa values of a multidimensional map.
   *
   * The template parameters refer to the dimension of the multimap and
   * the data type of the abscissa values, respectively.
   */
  template<unsigned int N, class JAbscissa_t> 
  struct JAbstractMultiMap :
    public virtual JAbstractMultiMap<N - 1, JAbscissa_t>
  {
    typedef JAbscissa_t                                          abscissa_type;
    typedef JMultiKey<N - 1, abscissa_type>                      key_type;

    using JAbstractMultiMap<N - 1, JAbscissa_t>::operator();


    /**
     * Virtual destructor.
     */
    virtual ~JAbstractMultiMap()
    {}


    /**
     * Get abscissa values as a function of given key.
     * 
     * \param  key           key
     * \return               abscissa values
     */
    virtual const JAbstractCollection<abscissa_type>& operator()(const key_type& key) const = 0;
  };


  /**
   * Terminator class of recursive class JAbstractMultiMap.
   * This class provides for dummy implementations of interface methods.
   */
  template<class JAbscissa_t>
  struct JAbstractMultiMap<0, JAbscissa_t>
  {
  protected:
    void operator()() const {}
  };
}

#endif
