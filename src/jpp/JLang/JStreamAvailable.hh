#ifndef __JLANG__JSTREAMAVAILABLE__
#define __JLANG__JSTREAMAVAILABLE__

#include <istream>
#include <ostream>

#include "JLang/JAnyType.hh"
#include "JLang/JNullType.hh"
#include "JLang/JTest.hh"
#include "JLang/JBool.hh"
#include "JLang/JPrintHelper.hh"


/**
 * \author mdejong
 */

/**
 * Fallback implementation for <tt>std::istream& operator>>(std::istream, T&)</tt> 
 * for types that don't support the stream operator.
 *
 * \param  in          input stream
 * \param  any_type    any  type
 * \return             null type
 */
JLANG::JNullType operator>>(std::istream& in, JLANG::JAnyType any_type);
  
  
/**
 * \cond NEVER
 * Fallback implementation for <tt>std::ostream& operator<<(std::ostream, const T&)</tt>
 * for types that don't support the stream operator.
 *
 * \param  out         output stream
 * \param  any_type    any  type
 * \return             null type
 * \endcond
 */
//JLANG::JNullType operator<<(std::ostream& out, JLANG::JAnyType any_type);


/**
 * Test availability of stream operators.
 */
template<class T, bool __str__ = JLANG::JPrintHelper::JMemberMethod<T>::__str__>
class JStreamAvailable :
  public JLANG::JTest
{
  using JLANG::JTest::test;
  
  static JTrue test(std::istream&);
  static JTrue test(std::ostream&);
    
  static T& getReference();

  static std::istream& is();
  static std::ostream& os();
    
public:
  static const bool has_istream = JTEST(test(is() >> getReference()));  //!< true if <tt>std::istream& operator>>(std::istream&, T&)</tt> is defined; else false
  static const bool has_ostream = JTEST(test(os() << getReference()));  //!< true if <tt>std::ostream& operator<<(std::ostream&, const T&)</tt> is defined; else false
};


/**
 * Specialisation of JStreamAvailable for class without member method <tt>__str__</tt>.
 */
template<class T>
class JStreamAvailable<T, true> :
  public JLANG::JTest
{
public:
  static const bool has_istream = JStreamAvailable<T, false>::has_istream;
  static const bool has_ostream = true;
};


/**
 * Auxiliary data structure for handling std::ostream.
 */
struct STREAM {
protected:
  /**
   * Auxiliary class for format stream.
   */
  struct JStream
  {
    /**
     * Constructor.
     *
     * \param  out         output stream
     * \param  message     message printed in case operator std::ostream<< is unavailable
     */
    JStream(std::ostream& out, const std::string& message) :
      out    (out),
      message(message)
    {}


    /**
     * Write value to output stream.
     *
     * \param  value       value
     * \return             this JStream
     */
    template<class T>
    std::ostream& operator<<(const T& value)
    {
      using namespace JPP;

      return print(out, value, JBool<JStreamAvailable<T>::has_ostream>());
    }

  private:
    /**
     * Print value if given option is true.
     *
     * \param  out         output stream
     * \param  value       value
     * \param  option      true
     * \return             output stream
     */
    template<class T>
    std::ostream& print(std::ostream& out, const T& value, const JLANG::JBool<true>& option)
    {
      return out << value;
    }

    /**
     * Print value if given option is true.
     *
     * \param  out         output stream
     * \param  value       value
     * \param  option      false
     * \return             output stream
     */
    template<class T>
    std::ostream& print(std::ostream& out, const T& value, const JLANG::JBool<false>& option)
    {
      return out << message;
    }

    std::ostream& out; 
    std::string   message;
  };

  std::string message;

public:
  /**
   * Constructor.
   *
   * \param  message     message printed in case operator std::ostream<< is unavailable
   */
  STREAM(const std::string& message = "") :
    message(message)
  {}


  /**
   * Format specifier.
   *
   * \param  out           output stream
   * \param  format        format
   * \return               output stream
   */
  friend inline JStream operator<<(std::ostream& out, const STREAM& format)
  {
    return JStream(out, format.message);
  }
};

#endif
