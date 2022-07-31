#ifndef __JLANG__JSINGLEPOINTER__
#define __JLANG__JSINGLEPOINTER__

#include "JLang/JStorage.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * The template JSinglePointer class can be used to hold a pointer to an object.
   * The object pointed to is deleted at destruction.
   * It is possible to create a container with single pointers.
   * The first template argument refers to the data type pointed to
   * and the second to the memory management policy.
   */
  template<class JClass_t, template<class> class JMemory_t = JNew> 
  class JSinglePointer :
    public JStorage<JClass_t, JMemory_t>
  {
  public:

    /**
     * Default constructor.
     */
    JSinglePointer()
    {}


    /**
     * Copy constructor.
     * The ownership of the object pointed to is transferred to this single pointer.
     *
     * \param  object   single pointer
     */
    JSinglePointer(const JSinglePointer& object)
    {
      this->set(object.get());

      const_cast<JSinglePointer&>(object).set(NULL);
    }


    /**
     * Constructor.
     *
     * \param  p        pointer to object
     */
    JSinglePointer(JClass_t* p)
    {
      this->set(p);
    }


    /**
     * Destructor.
     */
    ~JSinglePointer()
    {
      this->reset();
    }


    /**
     * Assignment operator.
     * The object pointed to previously is deleted.
     * The ownership of the object pointed to is transferred to this single pointer.
     *
     * \param  object   single pointer
     * \return          this single pointer
     */
    JSinglePointer& operator=(const JSinglePointer& object)
    {
      if (this->get() != object.get()) {

	this->reset();

	if (object.is_valid()) {

	  this->set(object.get());

	  const_cast<JSinglePointer&>(object).set(NULL);
	}
      }

      return *this;
    }


    /**
     * Assignment operator.
     * The object pointed to previously is deleted.
     *
     * \param  p        pointer to derived class object
     * \return          this single pointer
     */
    template<class JDerived_t>
    JSinglePointer& operator=(JDerived_t* p)
    {
      this->reset(p);

      return *this;
    }
  };
}

#endif
