#ifndef __JTOOLS__JGRIDCOLLECTION__
#define __JTOOLS__JGRIDCOLLECTION__

#include "JLang/JClass.hh"
#include "JTools/JCollection.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JLANG::JClass;

  
  /**
   * General purpose class for collection of equidistant elements.
   *
   * For a collection with equidistant elements, the index of the nearest element can directly be computed
   * based on the minimal abscissa value, the maximal abscissa value and the number of elements in the collection.
   * The lower_bound methods are re-implemented in this class which otherwise simply derives from JCollection.
   *
   * For convenience, the implementation of the standard map operator <tt>[]</tt> of the JCollection class is maintained.
   */
  template<class JElement_t, class JDistance_t = JDistance<typename JElement_t::abscissa_type> >
  class JGridCollection :
    public JCollection<JElement_t, JDistance_t>
  {
  public:

    typedef JCollection<JElement_t, JDistance_t>                                 collection_type;

    typedef typename collection_type::abscissa_type                              abscissa_type;
    typedef typename collection_type::ordinate_type                              ordinate_type;
    typedef typename collection_type::value_type                                 value_type;

    typedef typename collection_type::const_iterator                             const_iterator;
    typedef typename collection_type::const_reverse_iterator                     const_reverse_iterator;
    typedef typename collection_type::iterator                                   iterator;
    typedef typename collection_type::reverse_iterator                           reverse_iterator;

    typedef typename collection_type::pair_type                                  pair_type;


    /**
     * Default constructor.
     */
    JGridCollection()
    {}
    
     
    /**
     * Get index of given abscissa value.
     *
     * For values within the range of this collection, the index starts at zero and ends at number of elements minus one. 
     * Note that the index could be less than zero or larger than (or equal to) the number of elements,
     * if the given abscissa value is outside the range of this collection.
     *
     * \param  x               abscissa value
     * \return                 index
     */
    int getIndex(typename JClass<abscissa_type>::argument_type x) const
    {
      return (int) ((this->size() - 1) * (x - this->begin()->getX()) / (this->rbegin()->getX() - this->begin()->getX()));
    }


    /**
     * Get first position of element <tt>i</tt>, where <tt>x >= i->getX()</tt>.
     *
     * \param  x               abscissa value
     * \return                 position of corresponding element
     */
    const_iterator lower_bound(typename JClass<abscissa_type>::argument_type x) const
    {
      const int index = getIndex(x) + 1;

      if      (index <= 0)
	return this->begin();
      else if (index >= (int) this->size())
        return this->end();
      else
	return this->begin() + index;
    }


    /**
     * Get first position of element <tt>i</tt>, where <tt>x >= i->getX()</tt>.
     *
     * \param  x               abscissa value
     * \return                 position of corresponding element
     */
    iterator lower_bound(typename JClass<abscissa_type>::argument_type x)
    {
      const int index = getIndex(x) + 1;

      if      (index <= 0)
	return this->begin();
      else if (index >= (int) this->size())
        return this->end();
      else
	return this->begin() + index;
    }
  };
}

#endif
