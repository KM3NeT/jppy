#ifndef __JPHYSICS__JNPETABLE__
#define __JPHYSICS__JNPETABLE__

#include "JLang/JSharedPointer.hh"
#include "JLang/JException.hh"

#include "JTools/JConstantFunction1D.hh"
#include "JTools/JMultiFunction.hh"
#include "JTools/JTransformableMultiFunction.hh"
#include "JTools/JToolsToolkit.hh"


/**
 * \author mdejong
 */

namespace JPHYSICS {}
namespace JPP { using namespace JPHYSICS; }

namespace JPHYSICS {

  using JTOOLS::JConstantFunction1D;
  using JTOOLS::JMapList;
  using JTOOLS::JMultiFunction;
  using JTOOLS::JMultiMapTransformer;
  using JTOOLS::JTransformableMultiFunction;


  /**
   * Custom class for integrated values of the PDF of the arrival time of Cherenkov light.
   *
   * This class provides for the number of photo-electrons as a function
   * of the leading <tt>(n - 1)</tt> parameter values. 
   */
  template<class JArgument_t, 
	   class JResult_t,
	   class JMaplist_t,
	   class JDistance_t = JTOOLS::JDistance<JArgument_t> >
  class JNPETable :
    public JMultiFunction<JConstantFunction1D<JArgument_t, JResult_t>,
			  JMaplist_t, 
			  JDistance_t>
  {
  public:

    typedef JMultiFunction<JConstantFunction1D<JArgument_t, JResult_t>,
			   JMaplist_t, 
			   JDistance_t>                                   multifunction_t;

    using multifunction_t::NUMBER_OF_DIMENSIONS;

    typedef JConstantFunction1D<JArgument_t, JResult_t>                   function_type;
    typedef typename multifunction_t::map_type                            map_type;

    typedef typename multifunction_t::value_type                          value_type;
    typedef typename multifunction_t::argument_type                       argument_type;
    typedef typename multifunction_t::supervisor_type                     supervisor_type;

    typedef typename multifunction_t::abscissa_type                       abscissa_type;
    typedef typename multifunction_t::ordinate_type                       ordinate_type;
    typedef typename multifunction_t::result_type                         result_type;

    typedef typename multifunction_t::const_iterator                      const_iterator;
    typedef typename multifunction_t::const_reverse_iterator              const_reverse_iterator;
    typedef typename multifunction_t::iterator                            iterator;
    typedef typename multifunction_t::reverse_iterator                    reverse_iterator;

    typedef typename multifunction_t::super_iterator                      super_iterator;
    typedef typename multifunction_t::super_const_iterator                super_const_iterator;

    typedef JMultiMapTransformer<NUMBER_OF_DIMENSIONS, argument_type>     transformer_type;


    /**
     * Default constructor.
     */
    JNPETable() :
      transformer(transformer_type::getClone())
    {}


    /**
     * Constructor.
     *
     * \param  input               multi-dimensional PDF
     */
    template<class JPDF_t, class JPDFMaplist_t, class JPDFDistance_t>
    JNPETable(const JTransformableMultiFunction<JPDF_t, JPDFMaplist_t, JPDFDistance_t>& input) :
      transformer(transformer_type::getClone())
    {
      using namespace JTOOLS;

      typedef JTransformableMultiFunction<JPDF_t, JPDFMaplist_t, JPDFDistance_t>                       JTransformableMultiFunction_t;
      typedef JMultiKey<JTransformableMultiFunction_t::NUMBER_OF_DIMENSIONS - 1, argument_type>        JMultiKey_t;
      typedef typename JTransformableMultiFunction_t::transformer_type                                 transformer_type;


      this->transformer.reset(input.transformer->clone());

      for (typename JTransformableMultiFunction_t::super_const_iterator i = input.super_begin(); i != input.super_end(); ++i) {

	const JMultiKey_t& key   = (*i).getKey();
	const JPDF_t&      value = (*i).getValue();

        const typename transformer_type::array_type array(key);

        const double        V = getIntegral(value);
        const argument_type z = input.transformer->getXn(array, 1.0) - input.transformer->getXn(array, 0.0);

        this->insert(key, function_type(z*V));
      }

      this->compile();
    }


    /**
     * Add NPE table.
     *
     * Note that the summation is made via iteration of the elements in this multidimensional table.
     *
     * \param  input               NPE table
     */
    void add(const JNPETable& input)
    {
      using namespace JTOOLS;

      for (super_iterator i = this->super_begin(); i != this->super_end(); ++i) {

        map_type& f1 = (*i).getValue();

	for (typename map_type::iterator j = f1.begin(); j != f1.end(); ++j) {

	  try {

	    const JArray<NUMBER_OF_DIMENSIONS, argument_type> buffer((*i).getKey(), j->getX());

	    const double npe = get_value(input.evaluate(buffer.data()));
	    const double W   = this->transformer->getWeight(buffer);
	  
	    j->getY() += npe/W;
	  }
	  catch(JLANG::JException& error) {}
	}
      }
    }


    /**
     * Get number of photo-electrons.
     *
     * \param  args               comma separated argument list
     * \return                    number of photo-electrons
     */
    template<class ...Args>
    result_type operator()(const Args& ...args) const
    {
      this->buffer.set(args...);

      return this->evaluate(this->buffer.data());
    }


    /**
     * Recursive function value evaluation.
     *
     * \param  pX              pointer to abscissa values
     * \return                 function value
     */
    virtual result_type evaluate(const argument_type* pX) const override 
    {
      for (int i = 0; i != NUMBER_OF_DIMENSIONS; ++i) {
        this->buffer[i] = pX[i];
      }

      const double      W   = transformer->getWeight(buffer);
      const result_type npe = multifunction_t::evaluate(buffer.data());

      return W * npe;
    }


    /**
     * Application of weight function.
     *
     * \param  transformer     function transformer
     */
    void transform(const transformer_type& transformer)
    {
      using namespace JTOOLS;

      for (super_iterator i = this->super_begin(); i != this->super_end(); ++i) {

        map_type& f1 = (*i).getValue();

	for (typename map_type::iterator j = f1.begin(); j != f1.end(); ++j) {

	  const JArray<NUMBER_OF_DIMENSIONS, argument_type> array((*i).getKey(), j->getX());

	  j->getY() *= this->transformer->getWeight(array) / transformer.getWeight(array);
	}
      }

      this->transformer.reset(transformer.clone());
      this->compile();
    }


    JLANG::JSharedPointer<transformer_type> transformer;

  protected:
    mutable JTOOLS::JArray<NUMBER_OF_DIMENSIONS, argument_type> buffer;
  };
}

#endif
