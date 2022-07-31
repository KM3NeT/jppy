#ifndef __JTOOLS__JRESULTTRANSFORMER__
#define __JTOOLS__JRESULTTRANSFORMER__

#include "JTools/JMultiMapTransformer.hh"
#include "JTools/JResult.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  /**
   * Auxiliary class to handle multidimensional map transformations for given result type.
   */
  template<class JResult_t>
  struct JResultTransformer {

    typedef JResult_t                    result_type;

    template<unsigned int N, class JArgument_t>
    static result_type transform(const JMultiMapTransformer<N, JArgument_t>&                     transformer,
				 typename JMultiMapTransformer<N, JArgument_t>::const_array_type array,
				 const result_type&                                              result) 
    {
      return result;
    }
  };


  /**
   * Auxiliary class to handle multidimensional map transformations for given result type.
   */
  template<class JResult_t>
  struct JResultTransformer< JResultHesse<JResult_t> > {

    typedef JResultHesse<JResult_t>      result_type;

    template<unsigned int N, class JArgument_t>
    static result_type transform(const JMultiMapTransformer<N, JArgument_t>&                     transformer,
				 typename JMultiMapTransformer<N, JArgument_t>::const_array_type array,
				 const result_type&                                              result) 
    {
      const JArgument_t z = transformer.getXn(array, 1.0) - transformer.getXn(array, 0.0);
       
      result_type value(result);

      value.fp /= z;

      return value;
    }
  };


  /**
   * Auxiliary class to handle multidimensional map transformations for given result type.
   */
  template<class JResult_t>
  struct JResultTransformer< JResultPDF<JResult_t> > {

    typedef JResultPDF<JResult_t>        result_type;

    template<unsigned int N, class JArgument_t>
    static result_type transform(const JMultiMapTransformer<N, JArgument_t>&                     transformer,
				 typename JMultiMapTransformer<N, JArgument_t>::const_array_type array,
				 const result_type&                                              result) 
    {
      typedef JArgument_t argument_type;

      const argument_type z = transformer.getXn(array, 1.0) - transformer.getXn(array, 0.0);

      result_type value(result);

      value.fp /= z;
      value.v  *= z;
      value.V  *= z;

      return value;
    }
  };
}

#endif
