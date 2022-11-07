#ifndef __JTOOLS__JMULTIMAPTRANSFORMER__
#define __JTOOLS__JMULTIMAPTRANSFORMER__

#include "JLang/JClonable.hh"
#include "JIO/JSerialisable.hh"
#include "JTools/JArray.hh"
#include "JTools/JTransformer.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JLANG::JClonable;
  using JIO::JSerialisable;
  using JIO::JReader;
  using JIO::JWriter;


  /**
   * Interface for weight application and coordinate transformation of function.
   *
   * The template parameters refer to the dimension of the map and the data type of the argument, respectively.
   *
   * This class extends the JClonable and JSerialisable interfaces.
   */
  template<unsigned int N, class JArgument_t>
  class JMultiMapTransformer :
    public JClonable< JMultiMapTransformer<N, JArgument_t> >,
    public JSerialisable 
  {
  public:

    typedef JMultiMapTransformer<N, JArgument_t>                      multimaptransformer_type;

    typedef typename JClonable<multimaptransformer_type>::clone_type  clone_type;
    typedef JArgument_t                                               argument_type;
    typedef JArray<N, argument_type>                                  array_type;
    typedef const JArray<N, const argument_type>                      const_array_type;


    /**
     * Evaluate xn value as a function of {x0, ..., xn-1}
     *
     * \param  buffer               x0 - xn-1 values
     * \param  xn                   xn value
     * \return                      xn value
     */
    virtual argument_type putXn(const_array_type& buffer, const argument_type xn) const = 0;


    /**
     * Evaluate xn value as a function of {x0, ..., xn-1}
     *
     * \param  buffer               x0 - xn-1 values
     * \param  xn                   xn value
     * \return                      xn value
     */
    virtual argument_type getXn(const_array_type& buffer, const argument_type xn) const = 0;


    /**
     * Weight function.
     *
     * \param  buffer               x0 - xn-1 values
     * \return                      weight
     */
    virtual double getWeight(const_array_type& buffer) const = 0;


    /**
     * Weight function.
     *
     * \param  x                    first abscissa values
     * \param  args                 comma seperated list of remaining abscissa values
     * \return                      weight
     */
    template<class ...Args>
    double getWeight(const argument_type x, const Args& ...args) const
    {
      return getWeight(array_type(x, args...));
    }


    /**
     * Default implementation of weight application and coordinate transformation of function.
     */
    class JMultiMapDefaultTransformer;


    /**
     * Get default transformer.
     *
     * \return                      default transformer
     */
    static const JMultiMapTransformer& getDefaultTransformer()
    {
      static const JMultiMapDefaultTransformer transformer;
      
      return transformer;
    }


    /**
     * Get clone of default transformer.
     *
     * \return                      pointer to newly created transformer
     */
    static JMultiMapTransformer* getClone()
    {
      return new JMultiMapDefaultTransformer(); 
    }
  };

    
  /**
   * Default implementation of weight application and coordinate transformation of function.
   *
   * The coordiniate transformation has no effect and the weight is equal to one.
   * 
   * This class implements the JMultiMapTransformer, JClonable and JSerialisable interfaces.
   */
  template<unsigned int N, class JArgument_t>
  class JMultiMapTransformer<N, JArgument_t>::JMultiMapDefaultTransformer : 
    public JMultiMapTransformer<N, JArgument_t>
  {
  public:

    /**
     * Default constructor.
     */
    JMultiMapDefaultTransformer()
    {}


    /**
     * Clone object.
     * 
     * \return                      pointer to newly created transformer
     */
    virtual clone_type clone() const override 
    {
      return new JMultiMapDefaultTransformer(*this);
    }


    /**
     * Evaluate xn value as a function of {x0, ..., xn-1}
     *
     * \param  buffer               x0 - xn-1 values
     * \param  xn                   xn value
     * \return                      xn value
     */
    virtual argument_type putXn(const_array_type& buffer, const argument_type xn) const override 
    {
      return xn;
    }


    /**
     * Evaluate xn value as a function of {x0, ..., xn-1}
     *
     * \param  buffer               x0 - xn-1 values
     * \param  xn                   xn value
     * \return                      xn value
     */
    virtual argument_type getXn(const_array_type& buffer, const argument_type xn) const override 
    {
      return xn;
    }


    /**
     * Weight function.
     *
     * \param  buffer               x0 - xn-1 values
     * \return                      weight
     */
    virtual double getWeight(const_array_type& buffer) const override 
    {
      return 1.0;
    }


    /**
     * Read from input.
     *
     * This method reads nothing.
     *
     * \param  in              reader
     * \return                 reader
     */
    virtual JReader& read(JReader& in) override 
    { 
      return in;
    }


    /**
     * Write to output.
     *
     * This method writes nothing.
     *
     * \param  out             writer
     * \return                 writer
     */
    virtual JWriter& write(JWriter& out) const override 
    { 
      return out; 
    }
  };

  
  /**
   * Auxiliary class to convert JMultiMapTransformer to JCollectionElementTransformer.
   *
   * This class implements the JCollectionElementTransformer interface.
   */
  template<unsigned int N, class JElement_t>
  class JMultiMapPutTransformer : 
    public JCollectionElementTransformer<JElement_t> 
  {
  public:

    typedef typename JElement_t::abscissa_type                     abscissa_type;
    typedef typename JElement_t::ordinate_type                     ordinate_type;

    typedef JMultiMapTransformer<N, abscissa_type>                 transformer_type;

    typedef typename transformer_type::const_array_type            const_array_type;


    /**
     * Constructor.
     *
     * \param  __transformer        multidimensional map transformer
     * \param  __buffer             x0 - xn-1 values
     */
    JMultiMapPutTransformer(const transformer_type& __transformer,
			    const_array_type&       __buffer) :
      transformer(__transformer),
      buffer     (__buffer),
      W(transformer.getWeight(buffer))
    {}


    /**
     * Transform element.
     *
     * \param  element              input  element
     * \return                      output element
     */
    virtual JElement_t operator()(const JElement_t& element) const override 
    {
      return JElement_t(transformer.putXn(buffer, element.getX()), element.getY() / W);
    }
    
  private:    
    const transformer_type& transformer;
    const_array_type        buffer;
    const double            W;
  };


  /**
   * Auxiliary class to convert JMultiMapTransformer to JCollectionElementTransformer.
   *
   * This class implements the JCollectionElementTransformer interface.
   */
  template<unsigned int N, class JElement_t>
  class JMultiMapGetTransformer : 
    public JCollectionElementTransformer<JElement_t> 
  {
  public:

    typedef typename JElement_t::abscissa_type                     abscissa_type;
    typedef typename JElement_t::ordinate_type                     ordinate_type;

    typedef JMultiMapTransformer<N, abscissa_type>                 transformer_type;

    typedef typename transformer_type::const_array_type            const_array_type;


    /**
     * Constructor.
     * 
     * \param  __transformer        multidimensional map transformer
     * \param  __buffer             x0 - xn-1 values
     */
    JMultiMapGetTransformer(const transformer_type& __transformer,
			    const_array_type&       __buffer) :
      transformer(__transformer),
      buffer     (__buffer),
      W(transformer.getWeight(buffer))
    {}


    /**
     * Transform element.
     *
     * \param  element              input  element
     * \return                      output element
     */
    virtual JElement_t operator()(const JElement_t& element) const override 
    {
      return JElement_t(transformer.getXn(buffer, element.getX()), element.getY() * W);
    }
    
  private:
    const transformer_type& transformer;
    const_array_type        buffer;
    const double            W;
  };


  /**
   * Abstract interface for transformable multidimensional map.
   *
   * The template parameters refer to the dimension of the map and the data type of the argument, respectively.
   */
  template<unsigned int N, class JArgument_t>
  struct JTransformable
  {
    typedef JArgument_t                                            argument_type;
    typedef JMultiMapTransformer<N, argument_type>                 transformer_type;

    /**
     * Application of transformation.
     *
     * \param  transformer          function transformer
     */
    virtual void transform(const transformer_type& transformer) = 0;
  };
}

#endif
