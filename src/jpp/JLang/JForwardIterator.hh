#ifndef __JLANG__JFORWARDITERATOR__
#define __JLANG__JFORWARDITERATOR__

#include <cstddef>

/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * Template interface for method bool increment().
   * This interface implements the pre-fix and post-fix operators <tt>++</tt>.
   */
  template<class T> 
  class JForwardIterator {
  public:
    /**
     * Virtual destructor.
     */
    virtual ~JForwardIterator()
    {}

    
    /**
     * Increment iterator.
     *
     * \return                 this iterator
     */
    T& operator++()
    {
      this->increment();
      
      return static_cast<T&>(*this);
    }


    /**
     * Increment iterator.
     *
     * \return                 previous iterator
     */
    T operator++(int)
    {
      const T tmp(static_cast<const T&>(*this));

      this->increment();

      return tmp;
    }


    /**
     * Advance iterator.
     *
     * \param  offset          offset
     * \return                 iterator
     */
    T& operator+=(const size_t offset)
    {
      this->increment(offset);

      return static_cast<T&>(*this);
    }


    /**
     * Advance operator.
     *
     * \param  object          iterator
     * \param  offset          offset
     * \return                 iterator
     */
    friend inline T operator+(const T& object, const size_t offset)
    {
      T tmp(object);

      tmp.increment(offset);

      return tmp;
    }
    

    /**
     * Increment iterator.
     *
     * \return                 true if incremented; else false
     */
    virtual bool increment() = 0;


    /**
     * Increment iterator.
     *
     * \param  offset          offset
     * \return                 true if incremented; else false
     */
    virtual bool increment(const size_t offset)
    {
      size_t i = 0;

      while (i != offset && this->increment()) {
	++i;
      }

      return i == offset;
    }
  };
}

#endif

