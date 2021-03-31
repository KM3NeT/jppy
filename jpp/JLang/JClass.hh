#ifndef __JLANG__JCLASS__
#define __JLANG__JCLASS__


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Data structure for primitive types.
   */
  template<class T>
  struct JPrimitive {
    
    typedef const T      argument_type;
    
    enum { is_primitive  = true  };
  };
  
  
  /**
   * Data structure for method argument types.
   */
  template<class T>
  struct JArgument {
    
    typedef const T&     argument_type;
    
    enum { is_primitive  = false };
  };
  

  /**
   * Specialisations of JArgument for primitive types.
   */
  template<> struct JArgument<bool>                   : public JPrimitive<bool> {};
  template<> struct JArgument<char>                   : public JPrimitive<char> {};
  template<> struct JArgument<unsigned char>          : public JPrimitive<unsigned char> {};
  template<> struct JArgument<short>                  : public JPrimitive<short> {};
  template<> struct JArgument<unsigned short>         : public JPrimitive<unsigned short> {};
  template<> struct JArgument<int>                    : public JPrimitive<int> {};
  template<> struct JArgument<unsigned int>           : public JPrimitive<unsigned int> {};
  template<> struct JArgument<long int>               : public JPrimitive<long int> {};
  template<> struct JArgument<unsigned long int>      : public JPrimitive<unsigned long int> {};
  template<> struct JArgument<long long int>          : public JPrimitive<long long int> {};
  template<> struct JArgument<unsigned long long int> : public JPrimitive<unsigned long long int> {};
  template<> struct JArgument<float>                  : public JPrimitive<float> {};
  template<> struct JArgument<double>                 : public JPrimitive<double> {};
  template<> struct JArgument<long double>            : public JPrimitive<long double> {};


  /**
   * Data structure to check whether given data type is an iterator.
   */
  template<class T>
  struct is_iterator {

    static T make();

    typedef void*  buffer[2];

    static buffer&                                          test(...);  //!< any type
    template<class R> static typename R::iterator_category* test(R);    //!< iterator
    template<class R> static void*                          test(R *);  //!< pointer

    static const bool value = sizeof(test(make())) == sizeof(void *); 
  };  
  

  /**
   * Template for generic class types.
   */
  template<class T>
  struct JClass {

    typedef typename JArgument<T>::argument_type     argument_type;
    typedef T                                        value_type;
    typedef value_type&                              reference_type;
    typedef value_type*                              pointer_type;

    enum { is_primitive  = JArgument<T>::is_primitive };
    enum { is_reference  = false };
    enum { is_pointer    = false };
    enum { is_constant   = false };
  };

  
  /**
   * Specialisation of JClass for const class types.
   */
  template<class T>
  struct JClass<const T> {

    typedef typename JArgument<T>::argument_type     argument_type;
    typedef T                                        value_type;
    typedef const value_type&                        reference_type;
    typedef const value_type*                        pointer_type;

    enum { is_primitive  = JArgument<T>::is_primitive };
    enum { is_reference  = false };
    enum { is_pointer    = false };
    enum { is_constant   = true  };
  };

  
  /**
   * Specialisation of JClass for reference class types.
   */
  template<class T>
  struct JClass<T&> {

    typedef T&                                       argument_type;
    typedef T                                        value_type;
    typedef value_type&                              reference_type;
    typedef value_type*                              pointer_type;

    enum { is_primitive  = JArgument<T>::is_primitive };
    enum { is_reference  = true  };
    enum { is_pointer    = false };
    enum { is_constant   = false };
  };


  /**
   * Specialisation of JClass for const reference class types.
   */
  template<class T>
  struct JClass<const T&> {

    typedef const T&                                 argument_type;
    typedef T                                        value_type;
    typedef const value_type&                        reference_type;
    typedef const value_type*                        pointer_type;

    enum { is_primitive  = JArgument<T>::is_primitive };
    enum { is_reference  = true  };
    enum { is_pointer    = false };
    enum { is_constant   = true  };
  };

  
  /**
   * Specialisation of JClass for pointer class types.
   */
  template<class T>
  struct JClass<T*> {

    typedef T*                                       argument_type;
    typedef T                                        value_type;
    typedef value_type*&                             reference_type;
    typedef value_type*                              pointer_type;

    enum { is_primitive  = JArgument<T>::is_primitive };
    enum { is_reference  = false };
    enum { is_pointer    = true  };
    enum { is_constant   = false };
  };


  /**
   * Specialisation of JClass for const pointer class types.
   */
  template<class T>
  struct JClass<const T*> {

    typedef const T*                                 argument_type;
    typedef T                                        value_type;
    typedef const value_type*&                       reference_type;
    typedef const value_type*                        pointer_type;

    enum { is_primitive  = JArgument<T>::is_primitive };
    enum { is_reference  = false };
    enum { is_pointer    = true  };
    enum { is_constant   = true  };
  };

  
  /**
   * Specialisation of JClass for pointer class types.
   */
  template<class T>
  struct JClass<T*&> {

    typedef T*&                                      argument_type;
    typedef T                                        value_type;
    typedef value_type*&                             reference_type;
    typedef value_type*                              pointer_type;

    enum { is_primitive  = JArgument<T>::is_primitive };
    enum { is_reference  = true };
    enum { is_pointer    = true  };
    enum { is_constant   = false };
  };


  /**
   * Specialisation of JClass for const pointer class types.
   */
  template<class T>
  struct JClass<const T*&> {

    typedef const T*&                                argument_type;
    typedef T                                        value_type;
    typedef const value_type*&                       reference_type;
    typedef const value_type*                        pointer_type;

    enum { is_primitive  = JArgument<T>::is_primitive };
    enum { is_reference  = true };
    enum { is_pointer    = true  };
    enum { is_constant   = true  };
  };
}

#endif
