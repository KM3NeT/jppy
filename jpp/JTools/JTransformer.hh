#ifndef __JTOOLS__JTRANSFORMER__
#define __JTOOLS__JTRANSFORMER__


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  /**
   * Interface for transformation of collection of elements.
   */
  template<class JElement_t>
  struct JCollectionElementTransformer {
    /**
     * Virtual destructor.
     */
    virtual ~JCollectionElementTransformer()
    {}


    /**
     * Transform element.
     *
     * \param  element         input  element
     * \return                 output element
     */
    virtual JElement_t operator()(const JElement_t& element) const = 0;
  };
}

#endif
