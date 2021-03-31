#ifndef __JLANG__JABSTRACTPOINTER__
#define __JLANG__JABSTRACTPOINTER__

#include <cstdlib>

#include "JLang/JException.hh"
#include "JLang/JEquals.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Template interface for pointer to object(s).
   */
  template<class JClass_t>
  class JAbstractPointer :
    public JEquals< JAbstractPointer<JClass_t> >
  {
  protected:
    /**
     * Default constructor.
     */
    JAbstractPointer()
    {}


  public:
    /**
     * Virtual destructor.
     */
    virtual ~JAbstractPointer()
    {}


    /**
     * Equals.
     * The equality is evaluated by comparison of the internal pointers.
     *
     * \param  object   abstract pointer
     * \return          true if equals; else false
     */
    virtual bool equals(const JAbstractPointer& object) const
    {
      return this->get() == object.get();
    }


    /**
     * Get pointer.
     *
     * \return          pointer to object
     */
    virtual JClass_t* get() const = 0;


    /**
     * Set pointer.
     *
     * \param  p        pointer to object
     */
    virtual void set(JClass_t* p) = 0;


    /**
     * Reset pointer.
     */
    virtual void reset() = 0;


    /**
     * Check validity of pointer.
     *
     * \return          true if pointer not null; else false
     */
    bool is_valid() const 
    { 
      return this->get() != NULL; 
    }


    /**
     * Reset pointer.
     *
     * \param  p        pointer to object
     */
    void reset(JClass_t* p)
    {
      if (this->get() != p) {

        this->reset();

        if (p != NULL) {
          this->set(p);
	}
      }
    }


    /**
     * Smart pointer operator.
     *
     * \return          pointer to object
     */
    JClass_t* operator->() const 
    { 
      if (!is_valid())
	throw JNullPointerException("JAbstractPointer::operator->()");
      else
	return this->get();
    }

    
    /**
     * Type conversion operator.
     *
     * \return          pointer to object
     */
    operator JClass_t*() const
    {
      return this->get();
    }
  };
}

#endif
