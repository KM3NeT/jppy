#ifndef __JLANG__JCLONABLE__
#define __JLANG__JCLONABLE__

#include "JLang/JNullType.hh"

/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Template class for object cloning.
   */
  template<class JClonable_t, class JDerived_t = JNullType>
  struct JClonable;


  /**
   * Template specialisation to define base class for interface of object cloning.
   */
  template<class JClonable_t>
  struct JClonable<JClonable_t, JNullType>
  {
    /**
     * Type definition of return value of method clone().
     */
    typedef JClonable_t*                                      clone_type;


    /**
     * Virtual destructor.
     */
    virtual ~JClonable() 
    {}


    /**
     * Get clone of this object.
     *
     * \return              pointer to newly created object
     */
    virtual clone_type clone() const = 0;
  };


  /**
   * Template specialisation to define base class for implementation of object cloning.
   *
   * This class derives from the specified clonable class and implements the method clone.
   */
  template<class JClonable_t, class JDerived_t>
  struct JClonable :
    public JClonable_t
  {

    typedef typename JClonable<JClonable_t>::clone_type       clone_type;
    

    /**
     * Get clone of this object.
     *
     * \return              pointer to newly created object
     */
    virtual clone_type clone() const override 
    {
      return new JDerived_t(static_cast<const JDerived_t&>(*this));
    }
  };
}


#endif
