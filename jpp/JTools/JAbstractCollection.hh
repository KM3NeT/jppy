#ifndef __JTOOLS__JABSTRACTCOLLECTION__
#define __JTOOLS__JABSTRACTCOLLECTION__


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  /**
   * Abstract interface for abscissa values of a collection of elements.
   */
  template<class JAbscissa_t>
  struct JAbstractCollection {

    typedef JAbscissa_t                                                   abscissa_type;


    /**
     * Virtual destructor.
     */
    virtual ~JAbstractCollection()
    {}
    
    
    /**
     * Get number of elements.
     *
     * \return                 number of elements
     */
    virtual int getSize() const = 0;
    

    /**
     * Get abscissa value.
     *
     * \param  index           index
     * \return                 abscissa value
     */
    virtual abscissa_type getX(int index) const = 0;


    /**
     * Get minimal abscissa value.
     *
     * \return                 abscissa value
     */
    virtual abscissa_type getXmin() const = 0;


    /**
     * Get maximal abscissa value.
     *
     * \return                 abscissa value
     */
    virtual abscissa_type getXmax() const = 0;


    /**
     * Test whether abstract collections are equal.
     *
     * \param  collection      abstract collection
     * \return                 true if collections are equals; else false
     */
    bool is_equal(const JAbstractCollection& collection) const
    {
      if (this->getSize() == collection.getSize()) {

	for (int i = 0; i != this->getSize(); ++i) {

	  if (this->getX(i) != collection.getX(i)) {
	    return false;
	  }
	}

	return true;
      }

      return false;
    }
  };
}

#endif
