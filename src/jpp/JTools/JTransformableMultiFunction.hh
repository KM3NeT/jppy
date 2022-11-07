#ifndef __JTOOLS__JTRANSFORMABLEMULTIFUNCTION__
#define __JTOOLS__JTRANSFORMABLEMULTIFUNCTION__

#include "JIO/JSerialisable.hh"
#include "JLang/JSharedPointer.hh"

#include "JTools/JMultiFunction.hh"
#include "JTools/JMultiMapTransformer.hh"
#include "JTools/JFunctional.hh"
#include "JTools/JArray.hh"
#include "JTools/JMultiHistogram.hh"
#include "JTools/JHistogramMap.hh"
#include "JTools/JHistogram1D.hh"
#include "JTools/JResultTransformer.hh"
#include "JTools/JTransformableMultiHistogram.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JIO::JReader;
  using JIO::JWriter;


  /**
   * Transformable multidimensional function.
   *
   * This class implements the JTransformable interface.
   */
  template<class JFunction_t, 
	   class JMaplist_t, 
	   class JDistance_t = JDistance<typename JFunction_t::argument_type> >
  class JTransformableMultiFunction :
    public JMultiFunction<JFunction_t, JMaplist_t, JDistance_t>,
    public JTransformable<JMapLength<JMaplist_t>::value, typename JFunction_t::argument_type>
  {
  public:

    typedef JMultiFunction<JFunction_t, 
			   JMaplist_t,
			   JDistance_t>                                         multifunction_type;

    enum { NUMBER_OF_DIMENSIONS = multifunction_type::NUMBER_OF_DIMENSIONS };

    typedef JFunction_t                                                         function_type;
    
    typedef typename multifunction_type::value_type                             value_type;
    typedef typename multifunction_type::argument_type                          argument_type;
    typedef typename multifunction_type::supervisor_type                        supervisor_type;

    typedef typename multifunction_type::abscissa_type                          abscissa_type;
    typedef typename multifunction_type::ordinate_type                          ordinate_type;
    typedef typename multifunction_type::result_type                            result_type;
    
    typedef typename multifunction_type::const_iterator                         const_iterator;
    typedef typename multifunction_type::const_reverse_iterator                 const_reverse_iterator;
    typedef typename multifunction_type::iterator                               iterator;
    typedef typename multifunction_type::reverse_iterator                       reverse_iterator;

    typedef typename multifunction_type::super_iterator                         super_iterator;
    typedef typename multifunction_type::super_const_iterator                   super_const_iterator;

    typedef JMultiMapTransformer<JMapLength<JMaplist_t>::value, argument_type>  transformer_type;
    typedef typename transformer_type::array_type                               array_type;

    using multifunction_type::insert;
    

    /**
     * Default constructor.
     */
    JTransformableMultiFunction() :
      transformer(transformer_type::getClone())
    {}


    /**
     * Constructor.
     *
     * \param  input                multidimensional input
     */
    template<class T>
    JTransformableMultiFunction(const T& input) 
    {
      insert(input);

      this->compile();
    }


    /**
     * Get transformable multidimensional function.
     *
     * \return                      this transformable multidimensional function
     */
    const JTransformableMultiFunction& getTransformableMultiFunction() const
    {
      return static_cast<const JTransformableMultiFunction&>(*this);
    }


    /**
     * Get transformable multidimensional function.
     *
     * \return                      this transformable multidimensional function
     */
    JTransformableMultiFunction& getTransformableMultiFunction()
    {
      return static_cast<JTransformableMultiFunction&>(*this);
    }


    /**
     * Insert multidimensional input.
     *
     * \param  input                multidimensional function
     */
    template<class __JFunction_t, class __JMaplist_t, class __JDistance_t>    
    void insert(const JTransformableMultiFunction<__JFunction_t, __JMaplist_t, __JDistance_t>& input) 
    {
      this->transformer.reset(input.transformer->clone());

      multifunction_type::insert(input);
    }

    
    /**
     * Insert multidimensional input.
     *
     * \param  input                multidimensional histogram
     */
    template<class JHistogram_t, class __JMaplist_t, class __JDistance_t>
    void insert(const JTransformableMultiHistogram<JHistogram_t, __JMaplist_t, __JDistance_t>& input)
    {
      this->transformer.reset(input.transformer->clone());

      multifunction_type::insert(input);
    }

    
    /**
     * Add function.
     *
     * Note that the summation is made via iteration of the elements in this multidimensional function.
     *
     * \param  input           multidimensional function
     */
    template<class JMultiFunction_t>
    void add(const JMultiFunction_t& input)
    {
      for (super_iterator i = this->super_begin(); i != this->super_end(); ++i) {
	
	const array_type array = (*i).getKey();
	function_type&   f1    = (*i).getValue();

	if (!f1.empty()) {

	  const JMultiMapGetTransformer<NUMBER_OF_DIMENSIONS - 1, value_type> get(*(this->transformer), array);
	  const JMultiMapPutTransformer<NUMBER_OF_DIMENSIONS - 1, value_type> put(*(this->transformer), array);

	  f1.transform(get);

	  for (typename function_type::iterator j = f1.begin(); j != f1.end(); ++j) {

	    const JArray<NUMBER_OF_DIMENSIONS, argument_type> buffer(array, j->getX());

	    j->getY() += get_value(input.evaluate(buffer.data()));
	  }

	  f1.transform(put);
	  f1.compile();
	}
      }
    }


    /**
     * Multidimensional interpolation method call.
     *
     * \param  args            comma seperated list of abscissa values
     * \return                 function value
     */
    template<class ...Args>
    result_type operator()(const Args& ...args) const
    {
      return this->evaluate(this->buffer.set(args...).data());
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

      this->buffer[NUMBER_OF_DIMENSIONS - 1] = transformer->putXn(this->buffer, this->buffer[NUMBER_OF_DIMENSIONS - 1]);

      const result_type y = multifunction_type::evaluate(this->buffer.data());
      const double      W = transformer->getWeight(this->buffer);

      return JResultTransformer<result_type>::transform(*transformer, this->buffer, y) * W;      
    }


    /**
     * Application of weight function and coordinate transformation.
     *
     * \param  transformer     function transformer
     */
    virtual void transform(const transformer_type& transformer) override 
    {
      for (super_iterator i = this->super_begin(); i != this->super_end(); ++i) {

	const array_type  array    = (*i).getKey();
	function_type&    function = (*i).getValue();

	const JMultiMapGetTransformer<NUMBER_OF_DIMENSIONS - 1, value_type> get(*(this->transformer), array);
	const JMultiMapPutTransformer<NUMBER_OF_DIMENSIONS - 1, value_type> put(        transformer,  array);
	
	function.transform(get);
	function.transform(put);
      }

      this->transformer.reset(transformer.clone());
      this->compile();
    }


    /**
     * Read transformable multidimensional function.
     *
     * \param  in              reader
     * \param  object          transformable multidimensional function.
     * \return                 reader
     */
    friend inline JReader& operator>>(JReader& in, JTransformableMultiFunction& object)
    {
      in >> static_cast<multifunction_type&>(object);

      return object.transformer->read(in);
    }


    /**
     * Write transformable multidimensional function.
     *
     * \param  out             writer
     * \param  object          transformable multidimensional function
     * \return                 writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JTransformableMultiFunction& object)
    {
      out << static_cast<const multifunction_type&>(object);

      return object.transformer->write(out);
    }


    JLANG::JSharedPointer<transformer_type> transformer;
  };
  

  /**
   * Conversion of multidimensional histogram to multidimensional function.
   *
   * \param  input             multidimensional histogram
   * \param  output            multidimensional function
   */
  template<class JHistogram_t,
	   class JHistogramMaplist_t,
	   class JHistogramDistance_t,
	   class JFunction_t,
	   class JFunctionMaplist_t,
	   class JFunctionDistance_t>
  inline void makePDF(const JTransformableMultiHistogram<JHistogram_t, JHistogramMaplist_t, JHistogramDistance_t>& input, 
			   JTransformableMultiFunction<JFunction_t, JFunctionMaplist_t, JFunctionDistance_t>&           output)
  {
    output.insert(input);
  }
}

#endif
