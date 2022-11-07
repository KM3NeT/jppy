#ifndef __JTOOLS__JSET__
#define __JTOOLS__JSET__

#include <set>
#include <iterator>

#include "JTools/JAbstractCollection.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  template<class JElement_t, class JDistance_t>
  class JCollection;

  
  /**
   * Simple data structure for an abstract collection of non-equidistant abscissa values.
   *
   * This class implements the JAbstractCollection interface.
   */
  template<class JAbscissa_t>
  struct JSet : 
    public JAbstractCollection<JAbscissa_t>,
    std::set                  <JAbscissa_t>
  {
    typedef typename JAbstractCollection<JAbscissa_t>::abscissa_type      abscissa_type;


    /**
     * Default constructor.
     */
    JSet()
    {}


    /**
     * Constructor.
     *
     * \param  collection      abstract collection
     */
    JSet(const JAbstractCollection<abscissa_type>& collection)
    {
      for (int i = 0; i != collection.getSize(); ++i) {
	this->insert(collection.getX(i));
      }
    }


    /**
     * Constructor.
     *
     * \param  __begin         begin of abscissa values
     * \param  __end           end   of abscissa values
     */
    template<class T>
    JSet(T __begin, T __end)
    {
      for (T i = __begin; i != __end; ++i) {
	this->insert(*i);
      }
    }


    /**
     * Get number of elements.
     *
     * \return                 number of elements
     */
    virtual int getSize() const override 
    { 
      return (int) this->size();
    }
    

    /**
     * Get abscissa value.
     *
     * \param  index           index
     * \return                 abscissa value
     */
    virtual abscissa_type getX(int index) const override 
    {
      typename std::set<abscissa_type>::const_iterator i = this->begin();

      std::advance(i, index);

      return *i;
    }


    /**
     * Get minimal abscissa value.
     *
     * \return                 abscissa value
     */
    virtual abscissa_type getXmin() const override 
    {
      return *(this->begin());
    }


    /**
     * Get maximal abscissa value.
     *
     * \return                 abscissa value
     */
    virtual abscissa_type getXmax() const override 
    {
      return *(this->rbegin());
    }
    

    /**
     * Configure collection.
     *
     * \param  collection        collection
     * \return                   this set
     */
    template<class JElement_t, class JDistance_t>
    const JSet& operator()(JCollection<JElement_t, JDistance_t>& collection) const
    {
      collection.configure(*this);

      return *this;
    }


    /**
     * Read set from input.
     *
     * \param  in       input stream
     * \param  object   object
     * \return          input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JSet<JAbscissa_t>& object)
    {
      for (JAbscissa_t value; in >> value; ) {
	object.insert(value);
      }

      return in;
    }


    /**
     * Write set to output.
     *
     * \param  out      output stream
     * \param  object   object
     * \return          output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JSet<JAbscissa_t>& object)
    {
      for (typename JSet<JAbscissa_t>::const_iterator i = object.begin(); i != object.end(); ++i) {
	out << ' ' << *i;
      }

      return out;
    }
  };


  /**
   * Helper method for JSet.
   *
   * \param  __begin         begin of abscissa values
   * \param  __end           end   of abscissa values
   * \return                 set
   */
  template<class T>
  inline JSet<typename std::iterator_traits<T>::value_type> make_set(T __begin, T __end)
  {
    return JSet<typename std::iterator_traits<T>::value_type>(__begin, __end);
  }


  /**
   * Helper method for JSet.
   *
   * \param  input           abstract collection
   * \return                 set
   */
  template<class JAbscissa_t>
  inline JSet<JAbscissa_t> make_set(const JAbstractCollection<JAbscissa_t>& input)
  {
    return JSet<JAbscissa_t>(input);
  }
}

#endif
