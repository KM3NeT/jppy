#ifndef __JLANG__JTEST__
#define __JLANG__JTEST__


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * Auxiliary base class for compile time evaluation of test.
   *
   * The derived class should implement the static method JTest::test,
   * returning JTest::JTrue.
   * The macro JTest::JTEST will then return true if the given test is successful.
   */
  struct JTest {
  protected:
    
    struct JTrue  { char buffer; };  //!< definition of true
    struct JFalse { int  buffer; };  //!< definition of false

    static JFalse test(...);         //!< default false
    
    template<class __T__>
    static JFalse test(...);         //!< default false

    
    /**
     * Auxiliary class for type checking.
     */
    template<class U, U> struct JTypecheck;

    
    /**
     * Test macro.
     *
     * \param  __A__        test
     * \return              true if test successful; else false
     */
#define JTEST(__A__) (sizeof(__A__) == sizeof(JTrue))
  };
}

#endif
