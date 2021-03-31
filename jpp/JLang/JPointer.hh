#ifndef __JLANG__JPOINTER__
#define __JLANG__JPOINTER__

#include "JLang/JAbstractPointer.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * Template implementation of class that holds pointer to object(s).
   *
   * This class implements the JAbstractPointer interface.
   */
  template<class JClass_t>
  class JPointer :
    public JAbstractPointer<JClass_t>
  {
  public:

    using JAbstractPointer<JClass_t>::set;
    using JAbstractPointer<JClass_t>::reset;
    
    /**
     * Default constructor.
     */
    JPointer() :
      __p(NULL)
    {}


    /**
     * Constructor.
     *
     * \param  p        pointer to object
     */
    JPointer(JClass_t* p) :
      __p(p)
    {}


    /**
     * Constructor.
     *
     * \param  pointer  pointer to object
     */
    template<class T>
    JPointer(JPointer<T> pointer) :
      __p(pointer.get())
    {}


    /**
     * Get pointer.
     *
     * \return          pointer to object
     */
    virtual JClass_t* get() const override 
    {
      return this->__p;
    }


    /**
     * Set pointer.
     *
     * \param  p        pointer to object
     */
    virtual void set(JClass_t* p) override 
    {
      this->__p = p;
    }


    /**
     * Reset pointer.
     */
    virtual void reset() override 
    { 
      this->__p = NULL;
    }


    /**
     * Set pointer.
     *
     * \param  pointer  pointer to object
     */
    template<class T>
    void set(const JPointer<T>& pointer)
    {
      this->set(pointer.get());
    }


    /**
     * Reset pointer.
     *
     * \param  pointer  pointer to object
     */
    template<class T>
    void reset(const JPointer<T>& pointer)
    {
      this->reset(pointer.get());
    }


    /**
     * Get rereference to internal pointer.
     *
     * \return          reference to internal pointer
     */
    JClass_t* const& getReference() const
    {
      return __p;
    }


    /**
     * Get rereference to internal pointer.
     *
     * \return          reference to internal pointer
     */
    JClass_t* & getReference()
    {
      return __p;
    }
    
  protected:
    JClass_t* __p;      //!< pointer to object
  };
}

#endif
