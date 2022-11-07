#ifndef __JTOOLS__JMULTIFUNCTION__
#define __JTOOLS__JMULTIFUNCTION__

#include "JTools/JMultiMap.hh"
#include "JTools/JFunctional.hh"
#include "JTools/JConstantFunction1D.hh"
#include "JTools/JArray.hh"
#include "JTools/JMultiHistogram.hh"
#include "JTools/JHistogramMap.hh"
#include "JTools/JHistogram1D.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {
 
  /**
   * Multidimensional interpolation method.
   *
   * The template parameters respectively refer to:
   * -# function object used for the lowest dimension(s);
   * -# list of functional maps used for the higher dimension(s); and  
   * -# distance operator.
   *
   * The number of dimensions of this function object is equal to the length of
   * the given map list plus the extra dimensions of the first function object.
   * When converting a multidimensional histogram to a multidimensional function for 
   * subsequent interpolations, the function object used for the lowest dimension(s) 
   * is treated as a probability density function.
   */
  template<class JFunction_t, 
	   class JMaplist_t, 
	   class JDistance_t = JDistance<typename JFunction_t::argument_type> >
  class JMultiFunction :
    public JMultiMap<typename JFunction_t::argument_type, JFunction_t, JMaplist_t, JDistance_t>
  {
  public:
    
    typedef JMultiMap<typename JFunction_t::argument_type, 
		      JFunction_t, 
		      JMaplist_t,
		      JDistance_t>                                        multimap_type;

    enum { NUMBER_OF_DIMENSIONS = JMapLength<JMaplist_t>::value + JFunction_t::NUMBER_OF_DIMENSIONS };

    typedef JFunction_t                                                   function_type;

    typedef typename function_type::value_type                            value_type;
    typedef typename function_type::argument_type                         argument_type;

    typedef typename multimap_type::abscissa_type                         abscissa_type;
    typedef typename multimap_type::ordinate_type                         ordinate_type;
    typedef typename multimap_type::result_type                           result_type;

    typedef typename multimap_type::const_iterator                        const_iterator;
    typedef typename multimap_type::const_reverse_iterator                const_reverse_iterator;
    typedef typename multimap_type::iterator                              iterator;
    typedef typename multimap_type::reverse_iterator                      reverse_iterator;

    typedef typename multimap_type::super_iterator                        super_iterator;
    typedef typename multimap_type::super_const_iterator                  super_const_iterator;

    using JFunctional<abscissa_type, result_type>::setExceptionHandler;
    using multimap_type::insert;


    /**
     * Default constructor.
     */
    JMultiFunction()
    {}


    /**
     * Constructor.
     *
     * \param  input                multidimensional input
     */
    template<class T>
    JMultiFunction(const T& input) 
    {
      insert(input);

      this->compile();
    }


    /**
     * Get multidimensional function.
     *
     * \return                      this multidimensional function
     */
    const JMultiFunction& getMultiFunction() const
    {
      return static_cast<const JMultiFunction&>(*this);
    }


    /**
     * Get multidimensional function.
     *
     * \return                      this multidimensional function
     */
    JMultiFunction& getMultiFunction() 
    {
      return static_cast<JMultiFunction&>(*this);
    }


    /**
     * Insert multidimensional input.
     *
     * \param  input                multidimensional function
     */
    template<class __JFunction_t, class __JMaplist_t, class __JDistance_t>    
    void insert(const JMultiFunction<__JFunction_t, __JMaplist_t, __JDistance_t>& input) 
    {
      copy(input, *this);
    }

    
    /**
     * Insert multidimensional input.
     *
     * \param  input                multidimensional histogram
     */
    template<class JHistogram_t, class __JMaplist_t, class __JDistance_t>
    void insert(const JMultiHistogram<JHistogram_t, __JMaplist_t, __JDistance_t>& input)
    {
      this->insert(JMultiKey<0, argument_type>(), input);
    }


    /**
     * Compilation.
     */
    void compile() 
    {
      this->for_each(compiler);

      for (super_iterator i = this->super_begin(); i != this->super_end(); ++i) {
	(*i).getValue().compile();
      }
    }


    /**
     * Set the supervisor for handling of exceptions.
     *
     * \param  supervisor      supervisor
     */
    void setExceptionHandler(const typename function_type::supervisor_type& supervisor)
    {
      this->for_each(supervisor);

      for (super_iterator i = this->super_begin(); i != this->super_end(); ++i) {
	(*i).getValue().setExceptionHandler(supervisor);
      }
    }


    /**
     * Multi-dimensional interpolation method call.
     *
     * \param  args            comma seperated list of abscissa values
     * \return                 function value
     */
    template<class ...Args>
    result_type operator()(const Args& ...args) const
    {
      buffer.set(args...);

      return this->evaluate(buffer.data());
    }


  protected:
    mutable JArray<NUMBER_OF_DIMENSIONS, argument_type> buffer;

    /**
     * Insert multidimensional histogram at multidimensional key.
     *
     * \param  key                  multidimensional key
     * \param  input                multidimensional histogram
     */
    template<unsigned int N,
	     class __JAbscissa_t,
	     class __JContents_t,
	     template<class, class, class> class __JMap_t,
	     class __JDistance_t>
    void insert(const JMultiKey<N, argument_type>&                                          key, 
		const JHistogramMap<__JAbscissa_t, __JContents_t, __JMap_t, __JDistance_t>& input)
    {
      if (input.size() > 1) {

	for (auto j = input.begin(), i = j++; j != input.end(); ++i, ++j) {
	  
	  const argument_type x = 0.5 * (i->getX() + j->getX());

	  insert(JMultiKey<N+1, argument_type>(key, x), i->getY());
	}
      }
    }


    /**
     * Convert one-dimensional histogram to PDF and insert result at given multidimensional key.
     *
     * \param  key                  multidimensional key
     * \param  input                histogram
     */
    template<class __JElement_t, template<class, class> class __JContainer_t, class __JDistance_t>
    void insert(const JMultiKey<JMapLength<JMaplist_t>::value, argument_type>&   key, 
		const JHistogram1D<__JElement_t, __JContainer_t, __JDistance_t>& input)
 
    {
      JFunction_t buffer;
      
      makePDF(input, buffer);
      
      multimap_type::insert(key, buffer);
    }    


    /**
     * Convert multidimensional histogram to PDF and insert result at given multidimensional key.
     *
     * \param  key                  multidimensional key
     * \param  input                multidimensional histogram
     */
    template<class JHistogram_t, class __JMaplist_t, class __JDistance_t>
    void insert(const JMultiKey<JMapLength<JMaplist_t>::value, argument_type>&    key, 
		const JMultiHistogram<JHistogram_t, __JMaplist_t, __JDistance_t>& input)
    {
      JFunction_t buffer;
      
      makePDF(input, buffer);
      
      multimap_type::insert(key, buffer);
    }    
  };

  
  /**
   * Template specialisation of JMultiFunction for a JConstantFunction.
   * The primary 2D function is reduced to a 1D function.
   */
  template<class JArgument_t,
           class JResult_t,
           template<class, class, class> class JMap_t,
           class JDistance_t>
  class JMultiFunction<JConstantFunction1D<JArgument_t, JResult_t>, JMapList<JMap_t>, JDistance_t> :
    public JMap_t<JArgument_t, JResult_t, JDistance_t>,
    public JFunction1D<JArgument_t, typename JMap_t<JArgument_t, JResult_t, JDistance_t>::result_type>
  {
  public:

    typedef JMap_t<JArgument_t, JResult_t, JDistance_t>                   multimap_type;

    typedef typename multimap_type::abscissa_type                         abscissa_type;
    typedef typename multimap_type::ordinate_type                         ordinate_type;

    typedef typename multimap_type::argument_type                         argument_type;
    typedef typename multimap_type::result_type                           result_type;

    typedef typename multimap_type::const_iterator                        const_iterator;
    typedef typename multimap_type::const_reverse_iterator                const_reverse_iterator;
    typedef typename multimap_type::iterator                              iterator;
    typedef typename multimap_type::reverse_iterator                      reverse_iterator;

    typedef JConstantFunction1D<JArgument_t, JResult_t>                   function_type;
    typedef JMap_t<JArgument_t, JResult_t, JDistance_t>                   map_type;
    
    enum { NUMBER_OF_DIMENSIONS = 1 };

    using multimap_type::insert;


    /**
     * Insert element.
     *
     * \param  key             multidimensional key
     * \param  function        function
     */
    void insert(const JMultiKey<1, abscissa_type>& key, const function_type& function)
    {
      this->insert(key.first, function.getY());
    }
  };

 
  /**
   * Template specialisation of JMultiFunction for a JConstantFunction.
   * The number of dimensions of the primary function is reduced by one.
   */
  template<class JArgument_t,
	   class JResult_t,
	   class JMaplist_t, 
	   class JDistance_t>
  class JMultiFunction<JConstantFunction1D<JArgument_t, JResult_t>, JMaplist_t, JDistance_t> :
    public JMultiFunction<JMultiFunction<JConstantFunction1D<JArgument_t, JResult_t>, 
					 typename JMaplist_t::tail_type, 
					 JDistance_t>,
			  typename JMaplist_t::head_list,
			  JDistance_t>
  {
  public:

    typedef JMultiFunction<JMultiFunction<JConstantFunction1D<JArgument_t, JResult_t>, 
					  typename JMaplist_t::tail_type, 
					  JDistance_t>,
			   typename JMaplist_t::head_list,
			   JDistance_t>                                   multifunction_type;

    typedef typename multifunction_type::multimap_type                    multimap_type;

    typedef typename multimap_type::abscissa_type                         abscissa_type;
    typedef typename multimap_type::ordinate_type                         ordinate_type;

    typedef typename multimap_type::argument_type                         argument_type;
    typedef typename multimap_type::result_type                           result_type;

    typedef typename multimap_type::const_iterator                        const_iterator;
    typedef typename multimap_type::const_reverse_iterator                const_reverse_iterator;
    typedef typename multimap_type::iterator                              iterator;
    typedef typename multimap_type::reverse_iterator                      reverse_iterator;

    typedef typename multimap_type::super_iterator                        super_iterator;
    typedef typename multimap_type::super_const_iterator                  super_const_iterator;

    typedef JConstantFunction1D<JArgument_t, JResult_t>                   function_type;
    typedef JMultiFunction<JConstantFunction1D<JArgument_t, JResult_t>, 
			   typename JMaplist_t::tail_type, 
			   JDistance_t>                                   map_type;

    enum { NUMBER_OF_DIMENSIONS = JMapLength<JMaplist_t>::value };

    using multimap_type::insert;


    /**
     * Insert element.
     *
     * \param  key             multidimensional key
     * \param  function        function
     */
    void insert(const JMultiKey<NUMBER_OF_DIMENSIONS, abscissa_type>& key, const function_type& function)
    {
      this->get(key.front()).put(key.back(), function.getY());
    }
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
  inline void makePDF(const JMultiHistogram<JHistogram_t, JHistogramMaplist_t, JHistogramDistance_t>& input, 
		      JMultiFunction<JFunction_t, JFunctionMaplist_t, JFunctionDistance_t>&           output)
  {
    output.insert(input);
  }
}

#endif
