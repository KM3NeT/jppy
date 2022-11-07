#ifndef __JLANG__JPRINTHELPER__
#define __JLANG__JPRINTHELPER__

#include <ostream>

#include "JLang/JType.hh"
#include "JLang/JBool.hh"
#include "JLang/JTest.hh"
#include "JLang/JClass.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {
  
  /**
   * Auxiliary class to print via member method <tt>const char* __str__() const;</tt>.
   */
  class JPrintHelper {
    /**
     * Print interface.
     */
    struct JTypeout {
      /**
       * Virtual destructor.
       */
      virtual ~JTypeout()
      {}

      
      /**
       * Print object.
       *
       * \param  out           output stream
       * \param  p             pointer to object
       * \return               output stream
       */
      virtual std::ostream& print(std::ostream& out, const void* p) const = 0;
    };

  
    /**
     * Type writer implementation of interface JTypeout based on member method <tt>const char* __str__() const;</tt>
     */
    template<class T>
    struct JTypewriter :
      public JTypeout
    {
      /**
       * Print object.
       *
       * \param  out           output stream
       * \param  p             pointer to object
       * \return               output stream
       */
      virtual std::ostream& print(std::ostream& out, const void* p) const override 
      {
	return out << ((const T*) p)->__str__();;
      }
    };


    /**
     * Get type writer.
     *
     * \param  type          type
     * \param  option        true
     */
    template<class T>
    static JTypeout* get(JType<T> type, JBool<true> option)
    {
      return new JTypewriter<T>();
    }
  
    const void* p;             //!< pointer to object
    JTypeout*   typeout;       //!< pointer to printer interface
  

  public:
    /**
     * Test for availability of member method <tt>const char* __str__() const;</tt>.
     */
    template<class T, bool is_primitive = JClass<T>::is_primitive>
    class JMemberMethod :
      public JTest
    {
      using JTest::test;
  
      template<class U>
      static JTrue test(JTypecheck<const char* (U::*)() const, &U::__str__>*);
  
    public:
      static const bool __str__ = JTEST(test<T>(0));
    };
    

    /**
     * Specialisation of JMemberMethod for primitive data types. 
     */
    template<class T>
    struct JMemberMethod<T, true>
    {
      static const bool __str__ = false;
    };

      
    /**
     * Constructor
     *
     * \param  object        object
     */
    template<class T>
    JPrintHelper(const T& object) :
      p(&object),
      typeout(get(JType<T>(), JBool<JMemberMethod<T>::__str__>()))
    {}


    /**
     * Destructor.
     */
    ~JPrintHelper()
    {
      delete typeout;
    }

    
    /**
     * Print object.
     *
     * \param  out           output stream
     * \return               output stream
     */
    inline std::ostream& print(std::ostream& out) const
    {
      return typeout->print(out, p);
    }

  private:
    JPrintHelper(const JPrintHelper&);
    JPrintHelper(JPrintHelper&&);
    JPrintHelper& operator=(const JPrintHelper&);
    JPrintHelper& operator=(JPrintHelper&&);
  };

  
  /**
   * Auxiliary class to temporarily replace std::ostream. 
   */
  struct JPrinter {
    /**
     * Constructor.
     *
     * \param  out           output stream
     */
    JPrinter(std::ostream& out) :
      __out(out)
    {}


    /**
     * Type definition of I/O operator.
     */
    typedef std::ostream& (*io_manip) (std::ostream&);
    

    /**
     * Parse I/O manipulator.
     *
     * \param  manip         I/O manipulator
     * \return               output stream
     */
    inline std::ostream& operator<<(io_manip manip)
    { 
      return __out << manip;
    }


    /**
     * Parse object.
     *
     * \param  object        object
     * \return               output stream
     */
    template<class T>
    inline std::ostream& operator<<(const T& object)
    {
      return __out << object;
    }   
    
  private:
    std::ostream& __out;
  };
}


/**
 * Print object via helper.
 *
 * \param  out           output stream
 * \param  object        object
 * \return               output stream
 */
inline JLANG::JPrinter operator<<(std::ostream& out, const JLANG::JPrintHelper& object)
{
  return object.print(out);
}

#endif
