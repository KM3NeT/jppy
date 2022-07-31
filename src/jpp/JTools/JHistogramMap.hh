#ifndef __JTOOLS__JHISTOGRAMMAP__
#define __JTOOLS__JHISTOGRAMMAP__

#include "JLang/JClass.hh"
#include "JTools/JHistogram.hh"
#include "JTools/JDistance.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {


  /**
   * Histogram map.
   *
   * This class implements the JHistogram interface.
   */
  template<class JAbscissa_t,
	   class JContents_t,
	   template<class, class, class> class JMap_t,
	   class JDistance_t = JDistance<JAbscissa_t> >
  class JHistogramMap :
    public JMap_t<JAbscissa_t, JContents_t, JDistance_t>,
    public JHistogram<JAbscissa_t, typename JContents_t::contents_type>
  {
  public:

    typedef JMap_t<JAbscissa_t, JContents_t, JDistance_t>                        collection_type;
    typedef JHistogram<JAbscissa_t, typename JContents_t::contents_type>         histogram_type;

    typedef typename collection_type::abscissa_type                              abscissa_type;
    typedef typename collection_type::ordinate_type                              ordinate_type;
    typedef typename collection_type::value_type                                 value_type;

    typedef typename collection_type::const_iterator                             const_iterator;
    typedef typename collection_type::const_reverse_iterator                     const_reverse_iterator;
    typedef typename collection_type::iterator                                   iterator;
    typedef typename collection_type::reverse_iterator                           reverse_iterator;

    //typedef typename histogram_type::abscissa_type                               abscissa_type;
    typedef typename histogram_type::contents_type                               contents_type;


    /**
     * Default constructor.
     */
    JHistogramMap()
    {}


    /**
     * Fill histogram.
     *
     * \param  pX              pointer to abscissa values
     * \param  w               weight
     */
    virtual void evaluate(const abscissa_type* pX,
			  typename JLANG::JClass<contents_type>::argument_type w)
    {
      const abscissa_type x = *pX;

      iterator p = this->lower_bound(x);

      this->integral  += w;

      if      (p == this->begin()) 
	this->underflow += w;
      else if (p == this->end()) 
	this->overflow  += w;
      else 
	(--p)->getY().evaluate(++pX, w);
    }    


    /**
     * Scale contents.
     *
     * \param  factor          multiplication factor
     * \return                 this histogram
     */
    JHistogramMap& mul(typename JLANG::JClass<contents_type>::argument_type factor)
    {
      collection_type::mul(factor);
      histogram_type ::mul(factor);

      return *this;
    }


    /**
     * Scale contents.
     *
     * \param  factor          division factor
     * \return                 this histogram
     */
    JHistogramMap& div(typename JLANG::JClass<contents_type>::argument_type factor)
    {
      collection_type::div(factor);
      histogram_type ::div(factor);

      return *this;
    }


    /**
     * Read histogram map from input.
     *
     * \param  in              reader
     * \param  object          histogram map
     * \return                 reader
     */
    friend inline JReader& operator>>(JReader& in, JHistogramMap& object)
    {
      in >> static_cast<histogram_type&> (object);
      in >> static_cast<collection_type&>(object);

      return in;
    }


    /**
     * Write histogram map to output.
     *
     * \param  out             writer
     * \param  object          histogram map
     * \return                 writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JHistogramMap& object)
    {
      out << static_cast<const histogram_type&> (object);
      out << static_cast<const collection_type&>(object);

      return out;
    }
  };
}

#endif
