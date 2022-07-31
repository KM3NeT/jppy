#ifndef __JTOOLS__JMULTIHISTOGRAM__
#define __JTOOLS__JMULTIHISTOGRAM__

#include "JTools/JMultiMap.hh"
#include "JTools/JHistogram.hh"
#include "JTools/JArray.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {


  /**
   * Multidimensional histogram.
   *
   * The template parameters respectively refer to:
   * -# histogram object used for the lowest dimension(s);
   * -# list of histogram maps used for the higher dimension(s); and
   * -# distance operator.
   *
   * The number of dimensions of this histogram object is equal to the length of
   * the given map list plus the extra dimensions of the first histogram object.
   */
  template<class JHistogram_t,
	   class JMaplist_t,
	   class JDistance_t = JDistance<typename JHistogram_t::abscissa_type> >
  class JMultiHistogram :
    public JMultiMap<typename JHistogram_t::abscissa_type, JHistogram_t, JMaplist_t, JDistance_t>
  {
  public:

    typedef JMultiMap<typename JHistogram_t::abscissa_type,
                      JHistogram_t,
                      JMaplist_t,
                      JDistance_t>                                        multimap_type;

    enum { NUMBER_OF_DIMENSIONS = JMapLength<JMaplist_t>::value + JHistogram_t::NUMBER_OF_DIMENSIONS };

    typedef typename multimap_type::abscissa_type                         abscissa_type;
    typedef typename multimap_type::ordinate_type                         ordinate_type;

    typedef typename multimap_type::const_iterator                        const_iterator;
    typedef typename multimap_type::const_reverse_iterator                const_reverse_iterator;
    typedef typename multimap_type::iterator                              iterator;
    typedef typename multimap_type::reverse_iterator                      reverse_iterator;

    typedef typename multimap_type::super_iterator                        super_iterator;
    typedef typename multimap_type::super_const_iterator                  super_const_iterator;

    typedef typename JHistogram_t::contents_type                          contents_type;


    /**
     * Default constructor.
     */
    JMultiHistogram()
    {}


    /**
     * Get multidimensional histogram.
     *
     * \return                 this multidimensional histogram
     */
    const JMultiHistogram& getMultiHistogram() const
    {
      return static_cast<const JMultiHistogram&>(*this);
    }


    /**
     * Get multidimensional histogram.
     *
     * \return                 this multidimensional histogram
     */
    JMultiHistogram& getMultiHistogram()
    {
      return static_cast<JMultiHistogram&>(*this);
    }


    /**
     * Multi-dimensional fill method call.
     *
     * \param  args            comma seperated list of abscissa values and weight
     */
    template<class ...Args>
    void fill(const Args& ...args)
    {
      __fill__(0, args...);
    }

    
  protected:
    /**
     * Recursive method for filling histogram.
     *
     * \param  i                index
     * \param  x0               value at given index
     * \param  x1               value at following index
     * \param  args             remaining values and weight
     */
    template<class ...Args>
    void __fill__(const int i, const abscissa_type x0, const abscissa_type x1, const Args& ...args)
    {
      this->buffer[i] = x0;

      __fill__(i + 1, x1, args...);
    }

    /**
     * Termination method for filling histogram.
     *
     * \param  i                index
     * \param  x                value at given index
     * \param  w                weight
     */
    virtual void __fill__(const int i, const abscissa_type x, const contents_type w)
    {
      this->buffer[i] = x;

      this->evaluate(this->buffer.data(), w);
    }

    mutable JArray<NUMBER_OF_DIMENSIONS, abscissa_type> buffer;
  };
}

#endif
