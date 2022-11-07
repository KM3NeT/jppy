
#ifndef __JLANG__JCOMPARISONAVAILABLE__
#define __JLANG__JCOMPARISONAVAILABLE__

#include "JLang/JNullType.hh"
#include "JLang/JAnyType.hh"
#include "JLang/JVoid.hh"
#include "JLang/JTest.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * Local namespace for fallback implementations for comparison operators.
   */  
  namespace JLOCAL {
    
    /**
     * Fallback implementations for comparison operators.
     */
    inline JNullType operator==(JAnyType, JAnyType) { return JNullType(); }
    inline JNullType operator!=(JAnyType, JAnyType) { return JNullType(); }
    inline JNullType operator< (JAnyType, JAnyType) { return JNullType(); }
    inline JNullType operator> (JAnyType, JAnyType) { return JNullType(); }
    inline JNullType operator<=(JAnyType, JAnyType) { return JNullType(); }
    inline JNullType operator>=(JAnyType, JAnyType) { return JNullType(); }

    
    /**
     * Test availability of comparison operators of non-composite data types.
     */
    template<class T, class JType_t = void>
    class JComparisonAvailable :
      public JTest
    {
      using JTest::test;
    
      static JTrue test(const bool&);
    
      static T& getReference();
    
    public:
      static const bool has_eq = JTEST(test(getReference() == getReference()));  //!< true if operator== available; else false
      static const bool has_ne = JTEST(test(getReference() != getReference()));  //!< true if operator!= available; else false
      static const bool has_lt = JTEST(test(getReference() <  getReference()));  //!< true if operator<  available; else false
      static const bool has_gt = JTEST(test(getReference() >  getReference()));  //!< true if operator>  available; else false
      static const bool has_le = JTEST(test(getReference() <= getReference()));  //!< true if operator<= available; else false
      static const bool has_ge = JTEST(test(getReference() >= getReference()));  //!< true if operator>= available; else false
    };


    /**
     * Test availability of comparison operators of data types which have a type definitions for first_type and second_type.
     * This applies to STL containers such as std::map.
     * Note that STL provides for the comparison of the container based on comparisons of its elements.
     */
    template<class T>
    class JComparisonAvailable<T, typename JVoid<typename T::second_type>::type>
    {
      typedef typename T::first_type     first_type;
      typedef typename T::second_type    second_type;

    public:
      static const bool has_eq = JComparisonAvailable<first_type>::has_eq && JComparisonAvailable<second_type>::has_eq;
      static const bool has_ne = JComparisonAvailable<first_type>::has_ne && JComparisonAvailable<second_type>::has_ne;
      static const bool has_lt = JComparisonAvailable<first_type>::has_lt && JComparisonAvailable<second_type>::has_lt;
      static const bool has_gt = JComparisonAvailable<first_type>::has_gt && JComparisonAvailable<second_type>::has_gt;
      static const bool has_le = JComparisonAvailable<first_type>::has_le && JComparisonAvailable<second_type>::has_le;
      static const bool has_ge = JComparisonAvailable<first_type>::has_ge && JComparisonAvailable<second_type>::has_ge;
    };


    /**
     * Test availability of comparison operators of data types which have a type definition for value_type.
     * This applies to STL containers such as std::vector and std::set.
     * Note that STL provides for the comparison of the container based on comparisons of its elements.
     */
    template<class T>
    class JComparisonAvailable<T, typename JVoid<typename T::value_type>::type> :
      public JComparisonAvailable<typename T::value_type>
    {};


    /**
     * Template base class for data structures without equality capability.
     * This class implements the operators <tt> == != </tt>.
     */
    template<class T>
    class JNoequals {
    private:
      /**
       * Equal operator.
       *
       * \param  first        first object
       * \param  second       second object
       * \return              true if two objects are equal; else false
       */
      friend JNullType operator==(const T& first, const T& second)
      { 
	return JNullType();
      }


      /**
       * Not equal operator.
       *
       * \param  first        first object
       * \param  second       second object
       * \return              true if two objects are not equal; else false
       */
      friend JNullType operator!=(const T& first, const T& second)
      {
	return JNullType();
      }
    };
  }

  using JLOCAL::JComparisonAvailable;
  using JLOCAL::JNoequals;
}

#endif
