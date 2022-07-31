#ifndef __JTOOLS__JTRANSFORMABLEMULTIHISTOGRAM__
#define __JTOOLS__JTRANSFORMABLEMULTIHISTOGRAM__

#include "JIO/JSerialisable.hh"
#include "JLang/JSharedPointer.hh"

#include "JTools/JMultiMap.hh"
#include "JTools/JMultiHistogram.hh"
#include "JTools/JMultiMapTransformer.hh"
#include "JTools/JArray.hh"
#include "JIO/JSerialisable.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JIO::JReader;
  using JIO::JWriter;


  /**
   * Transformable multidimensional histogram.
   *
   * This class implements the JTransformable interface.
   */
  template<class JHistogram_t,
	   class JMaplist_t,
	   class JDistance_t = JDistance<typename JHistogram_t::abscissa_type> >
  class JTransformableMultiHistogram :
    public JMultiHistogram<JHistogram_t, JMaplist_t, JDistance_t>,
    public JTransformable<JMapLength<JMaplist_t>::value, typename JHistogram_t::abscissa_type>
  {
  public:

    typedef JMultiHistogram<JHistogram_t,
			    JMaplist_t,
			    JDistance_t>                                        JMultiHistogram_t;

    enum { NUMBER_OF_DIMENSIONS = JMultiHistogram_t::NUMBER_OF_DIMENSIONS };

    typedef JHistogram_t                                                        histogram_type;

    typedef typename histogram_type::contents_type                              contents_type;
    typedef typename histogram_type::value_type                                 value_type;

    typedef typename JMultiHistogram_t::abscissa_type                           abscissa_type;
    typedef typename JMultiHistogram_t::ordinate_type                           ordinate_type;

    typedef typename JMultiHistogram_t::const_iterator                          const_iterator;
    typedef typename JMultiHistogram_t::const_reverse_iterator                  const_reverse_iterator;
    typedef typename JMultiHistogram_t::iterator                                iterator;
    typedef typename JMultiHistogram_t::reverse_iterator                        reverse_iterator;

    typedef typename JMultiHistogram_t::super_iterator                          super_iterator;
    typedef typename JMultiHistogram_t::super_const_iterator                    super_const_iterator;

    typedef JMultiMapTransformer<JMapLength<JMaplist_t>::value, abscissa_type>  transformer_type;
    typedef typename transformer_type::array_type                               array_type;


    /**
     * Default constructor.
     */
    JTransformableMultiHistogram() :
      transformer(transformer_type::getClone())
    {}


    /**
     * Get transformable multidimensional histogram.
     *
     * \return                 this transformable multidimensional histogram
     */
    const JTransformableMultiHistogram& getTransformableMultiHistogram() const
    {
      return static_cast<const JTransformableMultiHistogram&>(*this);
    }


    /**
     * Get transformable multidimensional histogram.
     *
     * \return                 this transformable multidimensional histogram
     */
    JTransformableMultiHistogram& getTransformableMultiHistogram()
    {
      return static_cast<JTransformableMultiHistogram&>(*this);
    }


    /**
     * Application of weight function and coordinate transformation.
     *
     * \param  transformer     function transformer
     */
    virtual void transform(const transformer_type& transformer) override 
    {
      typedef typename transformer_type::array_type array_type;

      for (super_iterator i = this->super_begin(); i != this->super_end(); ++i) {

	const array_type  array     = (*i).getKey();
	histogram_type&   histogram = (*i).getValue();

	const JMultiMapGetTransformer<NUMBER_OF_DIMENSIONS - 1, value_type> get(*(this->transformer), array);
	const JMultiMapPutTransformer<NUMBER_OF_DIMENSIONS - 1, value_type> put(        transformer,  array);
	
	histogram.transform(get);
	histogram.transform(put);
      }

      this->transformer.reset(transformer.clone());
    }


    /**
     * Read transformable multidimensional histogram.
     *
     * \param  in              reader
     * \param  object          transformable multidimensional histogram.
     * \return                 reader
     */
    friend inline JReader& operator>>(JReader& in, JTransformableMultiHistogram& object)
    {
      in >> static_cast<JMultiHistogram_t&>(object);

      return object.transformer->read(in);
    }


    /**
     * Write transformable multidimensional histogram.
     *
     * \param  out             writer
     * \param  object          transformable multidimensional histogram
     * \return                 writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JTransformableMultiHistogram& object)
    {
      out << static_cast<const JMultiHistogram_t&>(object);

      return object.transformer->write(out);
    }


    JLANG::JSharedPointer<transformer_type> transformer;

  protected:
    /**
     * Termination method for filling histogram.
     *
     * \param  i                index
     * \param  x                abscissa value
     * \param  w                weight
     */
    virtual void __fill__(const int i, const abscissa_type x, const contents_type w) override 
    {
      this->buffer[NUMBER_OF_DIMENSIONS - 1] = transformer->putXn(this->buffer, this->buffer[NUMBER_OF_DIMENSIONS - 1]);

      const double z = transformer->putXn(this->buffer, 1.0) - transformer->putXn(this->buffer, 0.0);
      const double W = transformer->getWeight(this->buffer);

      this->evaluate(this->buffer.begin(), w * z / W);

      this->evaluate(this->buffer.begin(), w);
    }    
  };
}

#endif
