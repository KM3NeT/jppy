#ifndef __JHISTOGRAM1D__
#define __JHISTOGRAM1D__

#include "JTools/JHistogram.hh"
#include "JTools/JDistance.hh"
#include "JTools/JAbstractHistogram.hh"
#include "JTools/JAbstractCollection.hh"
#include "JTools/JCollection.hh"
#include "JTools/JElement.hh"
#include "JMath/JMath.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JMATH::JMath;
  using JLANG::JClass;


  /**
   * Auxiliary class for merging of fixed number of consecutive bins.
   */
  template<class JElement_t>
  struct JRebin {

    typedef JElement_t                                   value_type;
    typedef typename JElement_t::ordinate_type           contents_type;


    /**
     * Constructor.
     *
     * \param  n               number of bins to merge
     */
    JRebin(const int n) :
      __n(n > 1 ? n : 1),
      __i(0)
      {}
    
    
    /**
     * Test whether bins should be merged.
     *
     * \param  first           first  bin
     * \param  second          second bin
     * \return                 true if bins should be merged; else false
     */
    bool operator()(const value_type& first, const value_type& second) const
    {
      return (__n != 1 && ++__i%__n != 0);
    }
    
  private:
    const   int __n;
    mutable int __i;
  };
  
  
  /**
   * Auxiliary class for merging of consecutive bins until minimal content is reached.
   */
  template<class JElement_t>
  struct JContent {

    typedef JElement_t                                   value_type;
    typedef typename JElement_t::ordinate_type           contents_type;


    /**
     * Constructor.
     *
     * \param  y               minimal content
     */
    JContent(const contents_type y) :
      __y(y)
    {}
    
    
    /**
     * Test whether bins should be merged.
     *
     * \param  first           first  bin
     * \param  second          second bin
     * \return                 true if bins should be merged; else false
     */
    bool operator()(const value_type& first, const value_type& second) const
    {
      return (first.getY() + second.getY() < __y);
    }
    
  private:
    const contents_type __y;
  };
  


  /**
   * Histogram in 1D.
   *
   * This class implements the JHistogram interface.
   */
  template<class JElement_t,
	   template<class, class> class JContainer_t,
	   class JDistance_t = JDistance<typename JElement_t::abscissa_type> >
  class JHistogram1D :
    public JContainer_t<JElement_t, JDistance_t>,
    public JHistogram<typename JElement_t::abscissa_type, typename JElement_t::ordinate_type>,
    public JMath< JHistogram1D<JElement_t, JContainer_t, JDistance_t> >
  {
  public:

    enum { NUMBER_OF_DIMENSIONS = 1 };

    typedef JContainer_t<JElement_t, JDistance_t>                                collection_type;

    typedef typename collection_type::abscissa_type                              abscissa_type;
    typedef typename collection_type::ordinate_type                              ordinate_type;
    typedef typename collection_type::value_type                                 value_type;

    typedef typename collection_type::const_iterator                             const_iterator;
    typedef typename collection_type::const_reverse_iterator                     const_reverse_iterator;
    typedef typename collection_type::iterator                                   iterator;
    typedef typename collection_type::reverse_iterator                           reverse_iterator;

    typedef JHistogram<abscissa_type, ordinate_type>                             histogram_type;

    typedef typename histogram_type::contents_type                               contents_type;

    typedef JTOOLS::JRebin  <value_type>                                         JRebin;
    typedef JTOOLS::JContent<value_type>                                         JContent;


    /**
     * Default constructor.
     */
    JHistogram1D()
    {}


    /**
     * Constructor.
     *
     * \param  bounds          bounds
     */
    JHistogram1D(const JAbstractHistogram<abscissa_type>& bounds)
    {
      this->configure(bounds);
    }


    /**
     * Constructor.
     *
     * \param  bounds          bounds
     */
    JHistogram1D(const JAbstractCollection<abscissa_type>& bounds)
    {
      this->configure(bounds);
    }


    /**
     * Reset.
     */
    void reset()
    {
      histogram_type::reset();

      for (iterator i = this->begin(); i != this->end(); ++i) {
	i->getY() = JMATH::zero;
      }
    }


    /**
     * Fill histogram.
     *
     * \param  pX              pointer to abscissa values
     * \param  w               weight
     */
    virtual void evaluate(const abscissa_type* pX,
			  typename JClass<contents_type>::argument_type w)
    {
      this->fill(*pX, w);
    }


    /**
     * Fill histogram.
     *
     * \param  x               abscissa value
     * \param  w               weight
     */
    void fill(typename JClass<abscissa_type>::argument_type x, 
	      typename JClass<contents_type>::argument_type w)
    {
      this->integral  += w;

      iterator p = this->lower_bound(x);
	
      if      (p == this->begin())
	this->underflow += w;
      else if (p == this->end())
	this->overflow  += w;
      else
	(--p)->getY()   += w;
    }

    
    /**
     * Rebin histogram.
     *
     * \param  merge           rebin evaluator
     */
    template<class JRebin_t>
    void rebin(JRebin_t merge)
    {
      if (this->size() > 1u) {

	iterator out = this->begin();

	for (const_iterator i = this->begin(); i != this->end(); ) {

	  *out = *i;

	  while (++i != this->end() && merge(*out,*i)) {
	    out->getY() += i->getY();
	  }

	  ++out;
	}

	const_reverse_iterator __rbegin(out);

	if (this->getDistance(__rbegin->getX(), this->rbegin()->getX()) > 0.0) {

	  *out = *(this->rbegin());

	  ++out;
	}

	this->resize(std::distance(this->begin(), out));
      }
    }


    /**
     * Add histogram.
     *
     * \param  histogram       histogram
     * \return                 this histogram
     */
    JHistogram1D& add(const JHistogram1D& histogram)
    {
      collection_type::add(static_cast<const collection_type&>(histogram));
      histogram_type ::add(static_cast<const histogram_type&> (histogram));

      return *this;
    }


    /**
     * Subtract histogram.
     *
     * \param  histogram       histogram
     * \return                 this histogram
     */
    JHistogram1D& sub(const JHistogram1D& histogram)
    {
      collection_type::sub(static_cast<const collection_type&>(histogram));
      histogram_type ::sub(static_cast<const histogram_type&> (histogram));

      return *this;
    }


    /**
     * Scale contents.
     *
     * \param  value           multiplication factor
     * \return                 this histogram
     */
    JHistogram1D& mul(const double value)
    {
      collection_type::mul(value);
      histogram_type ::mul(value);

      return *this;
    }


    /**
     * Scale contents.
     *
     * \param  value           division factor
     * \return                 this histogram
     */
    JHistogram1D& div(const double value)
    {
      collection_type::div(value);
      histogram_type ::div(value);

      return *this;
    }
    

    /**
     * Read histogram from input.
     *
     * \param  in              reader
     * \param  object          histogram
     * \return                 reader
     */
    friend inline JReader& operator>>(JReader& in, JHistogram1D& object)
    {
      in >> static_cast<histogram_type&> (object);
      in >> static_cast<collection_type&>(object);
      
      return in;
    }


    /**
     * Write histogram to output.
     *
     * \param  out             writer
     * \param  object          histogram
     * \return                 writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JHistogram1D& object)
    {
      out << static_cast<const histogram_type&> (object);
      out << static_cast<const collection_type&>(object);

      return out;
    }
  };


  /**
   * Template specialisation if JHistogram1D class with bin centering.
   *
   * This class implements the JHistogram interface.
   */
  template<class JAbscissa_t,
	   class JContents_t,
	   template<class, class> class JContainer_t,
	   class JDistance_t>
  class JHistogram1D<JBin2D<JAbscissa_t, JContents_t>, JContainer_t, JDistance_t> :
    public JContainer_t<JBin2D<JAbscissa_t, JContents_t>, JDistance_t>,
    public JHistogram<JAbscissa_t, JContents_t>,
    public JMath< JHistogram1D<JBin2D<JAbscissa_t, JContents_t>, JContainer_t, JDistance_t> >
  {
  public:

    enum { NUMBER_OF_DIMENSIONS = 1 };

    typedef JBin2D<JAbscissa_t, JContents_t>                                     element_type;
    typedef JContainer_t<element_type, JDistance_t>                              collection_type;

    typedef typename collection_type::abscissa_type                              abscissa_type;
    typedef typename collection_type::ordinate_type                              ordinate_type;
    typedef typename collection_type::value_type                                 value_type;

    typedef typename collection_type::const_iterator                             const_iterator;
    typedef typename collection_type::const_reverse_iterator                     const_reverse_iterator;
    typedef typename collection_type::iterator                                   iterator;
    typedef typename collection_type::reverse_iterator                           reverse_iterator;

    typedef JHistogram<abscissa_type, ordinate_type>                             histogram_type;

    typedef typename histogram_type::contents_type                               contents_type;

    typedef JTOOLS::JRebin  <value_type>                                         JRebin;
    typedef JTOOLS::JContent<value_type>                                         JContent;


    /**
     * Default constructor.
     */
    JHistogram1D()
    {}


    /**
     * Constructor.
     *
     * \param  bounds          bounds
     */
    JHistogram1D(const JAbstractCollection<abscissa_type>& bounds)
    {
      this->set(bounds);
    }


    /**
     * Fill histogram.
     *
     * \param  pX              pointer to abscissa values
     * \param  w               weight
     */
    virtual void evaluate(const abscissa_type* pX,
			  typename JClass<contents_type>::argument_type w)
    {
      this->fill(*pX, w);
    }


    /**
     * Fill histogram.
     *
     * \param  x               abscissa value
     * \param  w               weight
     */
    void fill(typename JClass<abscissa_type>::argument_type x, 
	      typename JClass<contents_type>::argument_type w)
    {
      this->integral  += w;

      iterator p = this->lower_bound(x);
	
      if      (p == this->begin())
	this->underflow += w;
      else if (p == this->end())
	this->overflow  += w;
      else
	(--p)->fill(x, w);
    }

    
    /**
     * Rebin histogram.
     *
     * \param  merge           rebin evaluator
     */
    template<class JRebin_t>
    void rebin(JRebin_t merge)
    {
      if (this->size() > 1u) {
      
	iterator out = this->begin();

	for (const_iterator i = this->begin(); i != this->end(); ) {

	  *out = *i;

	  while (++i != this->end() && merge(*out,*i)) {
	    out->add(*i);
	  }

	  ++out;
	}

	const_reverse_iterator __rbegin(out);

	if (getDistance(__rbegin->getX(), this->rbegin()->getX()) > 0.0) {

	  *out = *(this->rbegin());

	  ++out;
	}

	this->resize(std::distance(this->begin(), out));
      }
    }


    /**
     * Scale contents.
     *
     * \param  value           multiplication factor
     */
    JHistogram1D& mul(const double value)
    {
      for (iterator i = this->begin(); i != this->end(); ++i) {
	i->mul(value);
      }

      histogram_type::mul(value);

      return *this;
    }


    /**
     * Scale contents.
     *
     * \param  value           division factor
     */
    JHistogram1D& div(const double value)
    {
      for (iterator i = this->begin(); i != this->end(); ++i) {
	i->div(value);
      }

      histogram_type::div(value);

      return *this;
    }


    /**
     * Read histogram from input.
     *
     * \param  in              reader
     * \param  object          histogram
     * \return                 reader
     */
    friend inline JReader& operator>>(JReader& in, JHistogram1D& object)
    {
      in >> static_cast<histogram_type&> (object);
      in >> static_cast<collection_type&>(object);
      
      return in;
    }


    /**
     * Write histogram to output.
     *
     * \param  out             writer
     * \param  object          histogram
     * \return                 writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JHistogram1D& object)
    {
      out << static_cast<const histogram_type&> (object);
      out << static_cast<const collection_type&>(object);

      return out;
    }

  private:
    /**
     * Make methods inaccessible.
     */
    JHistogram1D& add(const JHistogram1D& histogram);   //!< addition    not allowed with bin centering
    JHistogram1D& sub(const JHistogram1D& histogram);   //!< subtraction not allowed with bin centering
  };


  /**
   * Conversion of histogram to probability density function (PDF).
   *
   * The PDF abscissa and contents are set to the bin center and contents divided the bin width, respectively.
   *
   * \param  input             histogram
   * \param  output            mappable collection
   */
  template<class JElement_t, 
	   template<class, class> class JContainer_t,
	   class JDistance_t>
  inline void makePDF(const JHistogram1D<JElement_t, JContainer_t, JDistance_t>& input, 
		      typename JMappable<JElement_t>::map_type&                  output)
  {
    typedef typename JElement_t::abscissa_type                                            abscissa_type;
    typedef typename JElement_t::ordinate_type                                            ordinate_type;
    typedef typename JHistogram1D<JElement_t, JContainer_t, JDistance_t>::const_iterator  const_iterator;
    
    if (input.getSize() > 1) {
      
      for (const_iterator j = input.begin(), i = j++; j != input.end(); ++i, ++j) {
	
	const abscissa_type x = 0.5 * (i->getX() + j->getX());
	const ordinate_type y = i->getY();
	const double        w = input.getDistance(i->getX(), j->getX());
	
	output.put(x, y/w);
      }
    }
  }
  

  /**
   * Conversion of histogram to probability density function (PDF).
   *
   * The PDF abscissa and contents are set to the bin center and contents divided the bin width, respectively.
   *
   * \param  input             histogram
   * \param  output            mappable collection
   */
  template<class JAbscissa_t,
	   class JContents_t,
	   template<class, class> class JContainer_t,
	   class JDistance_t>
  inline void makePDF(const JHistogram1D<JBin2D<JAbscissa_t, JContents_t>, JContainer_t, JDistance_t>& input, 
		      JMappableCollection<JAbscissa_t, JContents_t>&                                   output)
  {
    typedef JAbscissa_t                                                                                         abscissa_type;
    typedef JContents_t                                                                                         contents_type;
    typedef typename JHistogram1D<JBin2D<JAbscissa_t, JContents_t>, JContainer_t, JDistance_t>::const_iterator  const_iterator;
    
    if (input.getSize() > 1) {
      
      for (const_iterator j = input.begin(), i = j++; j != input.end(); ++i, ++j) {
	
	const abscissa_type x = i->getBinCenter();
	const contents_type y = i->getY();
	const double        w = input.getDistance(i->getX(), j->getX());
	
	output.put(x, y/w);
      }
    }
  }


  /**
   * Conversion of data points to integral values.
   *
   * The integration is based on the sum of bin contents of the input data points.
   *
   * \param  input             histogram
   * \param  output            mappable collection
   * \return                   integral
   */
  template<class JElement_t,
           template<class, class> class JContainer_t,
           class JDistance_t>
  inline typename JElement_t::ordinate_type
  integrate(const JHistogram1D<JElement_t, JContainer_t, JDistance_t>& input, 
	    typename JMappable<JElement_t>::map_type&                  output)
  {
    typedef typename JElement_t::ordinate_type                                            ordinate_type;
    typedef typename JHistogram1D<JElement_t, JContainer_t, JDistance_t>::const_iterator  const_iterator;
    
    ordinate_type V(JMATH::zero);
    
    if (input.getSize() > 1) {
      
      output.put(input.begin()->getX(), V);
      
      for (const_iterator j = input.begin(), i = j++; j != input.end(); ++i, ++j) {
        
        V += i->getY();

        output.put(j->getX(), V);
      }
    }
    
    return V;
  }
}

#endif
