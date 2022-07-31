#ifndef __JLANG__JSTORAGE__
#define __JLANG__JSTORAGE__

#include "JLang/JPointer.hh"
#include "JLang/JMemory.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * Template storage class.
   * This class extends the JPointer class with storage capabilities.
   * The first template argument refers to the data type pointed to
   * and the second (optional argument) to the memory management policy (see e.g.\ JMemory.hh).
   * The method create() can be used to allocate memory;
   * the method reset() releases the allocated memory.
   */
  template<class JClass_t, template<class> class JMemory_t = JNew>
  class JStorage :
    public JPointer <JClass_t>,
    public JMemory_t<JClass_t>
  {
  public:

    typedef JPointer <JClass_t>                   pointer_type;
    typedef JMemory_t<JClass_t>                   memory_type;

    using pointer_type::reset;


    /**
     * Reset pointer.
     * The allocated memory is released.
     */
    virtual void reset() override 
    { 
      if (this->is_valid()) {
	this->release();
      }

      pointer_type::reset();
    }


    /**
     * Recreate object in memory.
     * A new object is created if no memory is allocated yet, 
     * else the previously created object is maintained.
     */
    void recreate()
    {
      if (!this->is_valid()) {
	this->set(memory_type::create());
      }
    }


    /**
     * Create object in memory.
     * The memory allocated by a previously created object will be released.
     */
    void create()
    {
      this->reset(memory_type::create());
    }


    /**
     * Create array of objects in memory.
     * The memory allocated by previously created objects will be released.
     *
     * \param  size      number of elements
     */
    void create(const unsigned int size)
    {
      this->reset(memory_type::create(size));
    }


  protected:
    /**
     * Release memory.
     */
    void release()
    { 
      memory_type::release(this->get());
    }
  };
}

#endif
