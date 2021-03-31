#ifndef __JLANG__JSHAREDPOINTER__
#define __JLANG__JSHAREDPOINTER__

#include "JLang/JSharedCounter.hh"
#include "JLang/JMemory.hh"
#include "JLang/JStorage.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * The template JSharedPointer class can be used to share a pointer to an object. 
   * The object pointed to is deleted when the shared counter is zero, 
   * i.e.\ when no-one shares the object.
   * It is possible to create a container with shared pointers. 
   * The first template argument refers to the data type pointed to
   * and the second to the memory management policy.
   */
  template<class JClass_t, template<class> class JMemory_t = JNew> 
  class JSharedPointer :
    public JSharedCounter,
    public JStorage<JClass_t, JMemory_t>
  {
  public:

    typedef JPointer<JClass_t>                    pointer_type;
    typedef JStorage<JClass_t, JMemory_t>         storage_type;
  
    using   JAbstractPointer<JClass_t>::reset;


    /**
     * Default constructor.
     */
    JSharedPointer() 
    {}


    /**
     * Copy constructor.
     * The reference counter of the shared object is incremented by one.
     *
     * \param  object   shared pointer
     */
    JSharedPointer(const JSharedPointer& object)
    {
      if (object.is_valid()) {
	this->set(object);
      }
    }

  
    /**
     * Assignment constructor.
     * If the pointer is valid, the reference counter of the shared object pointed to 
     * is initialised to one.
     *
     * \param  p        pointer to derived class object
     */
    template<class JDerived_t>
    JSharedPointer(JDerived_t* p)
    {
      if (p != NULL) {
	this->set(p);
      }
    }


    /**
     * Destructor.
     * The reference counter is decremented by one and 
     * the object pointed to is deleted when the reference counter is zero.
     */
    ~JSharedPointer()
    {
      if (this->detach()) {
	storage_type::reset();
      }
    }


    /**
     * Get shared pointer.
     *
     * \return          this shared pointer
     */
    const JSharedPointer& getSharedPointer() const
    {
      return static_cast<const JSharedPointer&>(*this);
    }


    /**
     * Get shared pointer.
     *
     * \return          this shared pointer
     */
    JSharedPointer& getSharedPointer()
    {
      return static_cast<JSharedPointer&>(*this);
    }


    /**
     * Set shared pointer.
     *
     * \param  object   shared pointer
     */
    void setSharedPointer(const JSharedPointer& object)
    {
      if (this->get() != object.get()) {

	this->reset();

	if (object.is_valid()) {
	  this->set(object);
	}
      }
    }


    /**
     * Assignment operator.
     * The reference counter is decremented by one and 
     * the object pointed to previously is deleted when its reference counter is zero.
     * The reference counter of the shared object is incremented by one.
     *
     * \param  object   shared pointer 
     * \return          this shared pointer
     */
    JSharedPointer& operator=(const JSharedPointer& object) 
    {
      this->setSharedPointer(object);

      return *this;
    }

  
    /**
     * Assignment operator.
     * The reference counter is decremented by one and 
     * the object pointed to previously is deleted when its reference counter is zero.
     * If the pointer is valid, the reference counter of the shared object pointed to 
     * is initialised to one.
     *
     * \param  p        pointer to derived class object
     * \return          this shared pointer
     */
    template<class JDerived_t>
    JSharedPointer& operator=(JDerived_t* p)
    {
      this->reset(p);

      return *this;
    }


    /**
     * Reset pointer.
     * The reference counter is decremented by one and 
     * the object pointed to previously is deleted when its reference counter is zero.
     */
    virtual void reset() override 
    {
      if (this->detach()) {
	storage_type::reset();
      }

      pointer_type::reset();
    }


  protected:
    /**
     * Set pointer.
     * The reference counter of the shared object pointed to is incremented by one.
     *
     * \param  object   shared pointer 
     */
    void set(const JSharedPointer& object) 
    {
      pointer_type::set(object.get());

      this->attach(object);
    }


    /**
     * Set pointer.
     * The reference counter of the shared object pointed to is initialised to one.
     *
     * \param  p        pointer to derived class object
     */
    virtual void set(JClass_t* p) override 
    {
      pointer_type::set(p);

      this->initialise();
    }
  };
}

#endif
