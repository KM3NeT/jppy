#ifndef __JLANG__JTYPELIST__
#define __JLANG__JTYPELIST__

#include "JLang/JNullType.hh"
#include "JLang/JType.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Type list.
   */
  template<class JHead_t = JNullType, class JTail_t = JNullType>
  struct JTypeList 
  {
    typedef JHead_t                  head_type;
    typedef JTail_t                  tail_type;
  };

  
  /**
   * List of identical types.
   */
  template<unsigned int N, class T>
  struct JMultipleType 
  {
    typedef JTypeList<T, typename JMultipleType<N-1, T>::typelist>       typelist;
  };


  /**
   * Terminator class of list of identical types.
   */
  template<class T>
  struct JMultipleType<1, T>
  {
    typedef JTypeList<T, JNullType>                                      typelist;
  };


  /**
   * Append to type list.
   *
   * Source code is taken from reference:
   * A. Alexandrescu, Modern C++ Design, Addison Wesley.
   */
  

  /**
   * Template specialisation of append to type list.
   */
  template<class JHead_t, class JTail_t>
  struct JAppend
  {
    typedef JTypeList<JHead_t, JTail_t>                                  typelist;
  };
  

  /**
   * Template specialisation of append to type list.
   */
  template<>
  struct JAppend<JNullType, JNullType>
  {
    typedef JTypeList<>                                                  typelist;
  };
  

  /**
   * Template specialisation of append to type list.
   */
  template<class T>
  struct JAppend<JNullType, T>
  {
    typedef JTypeList<T>                                                 typelist;
  };
  

  /**
   * Template specialisation of append to type list.
   */
  template<class JHead_t, class JTail_t>
  struct JAppend<JNullType, JTypeList<JHead_t, JTail_t> >
  {
    typedef JTypeList<JHead_t, JTail_t>                                  typelist;
  };
  

  /**
   * Template specialisation of append to type list.
   */
  template<class JHead_t, class JTail_t, class T>
  struct JAppend<JTypeList<JHead_t, JTail_t>, T>
  {
    typedef JTypeList<JHead_t, typename JAppend<JTail_t, T>::typelist>   typelist;
  };


  /**
   * Removal of data type from type list.
   *
   * Source code is taken from reference:
   * A. Alexandrescu, Modern C++ Design, Addison Wesley.
   */
  template<class JTypelist_t, class T>
  struct JRemove; 
  

  /**
   * Template specialisation of removal of data type from type list.
   */
  template<class T>
  struct JRemove<JNullType, T>
  {
    typedef JTypeList<>                                                  typelist;
  };
 

  /**
   * Template specialisation of removal of data type from type list.
   */
  template<class T, class JTail_t>
  struct JRemove<JTypeList<T, JTail_t>, T>
  {
    typedef JTail_t                                                      typelist;
  };
  

  /**
   * Template specialisation of removal of data type from type list.
   */
  template<class JHead_t, class JTail_t, class T>
  struct JRemove<JTypeList<JHead_t, JTail_t>, T>
  {
    typedef JTypeList<JHead_t, typename JRemove<JTail_t, T>::typelist>   typelist;
  };
  

  /**
   * Template specialisation of removal of type list from type list.
   */
  template<class JHead_t1, class JTail_t1, class JHead_t2, class JTail_t2>
  struct JRemove<JTypeList<JHead_t1, JTail_t1>, 
		 JTypeList<JHead_t2, JTail_t2> >
  {
    typedef typename JRemove<typename JRemove<JTypeList<JHead_t1, JTail_t1>, JHead_t2>::typelist,
			     JTail_t2>::typelist                                 typelist;
  };
  

  /**
   * Template specialisation of removal of type list from type list.
   */
  template<class JHead_t1, class JTail_t1, class JHead_t2>
  struct JRemove<JTypeList<JHead_t1, JTail_t1>, 
		 JTypeList<JHead_t2, JNullType> >
  {
    typedef typename JRemove<JTypeList<JHead_t1, JTail_t1>, JHead_t2>::typelist  typelist;
  };


  /**
   * Length of type list.
   *
   * Source code is taken from reference:
   * A. Alexandrescu, Modern C++ Design, Addison Wesley.
   */
  template<class JTypeList_t> struct JLength {};


  /**
   * Recursive length of type list.
   */
  template<class JHead_t, class JTail_t>
  struct JLength< JTypeList<JHead_t, JTail_t> >
  {
    enum { value = 1 + JLength<JTail_t>::value };
  };


  /**
   * Terminator class of length of type list.
   */
  template<>
  struct JLength<JNullType> 
  {
    enum { value = 0 };
  };


  /**
   * Test presence of data type in type list.
   */
  template<class JTypeList_t, class T>
  struct JHasType;

  
  /**
   * Recursive test of presence data type in type list.
   */
  template<class JHead_t, class JTail_t, class T>
  struct JHasType<JTypeList<JHead_t, JTail_t>, T>
  {
    enum { value = JHasType<JTail_t, T>::value };
  };
  

  /**
   * Identify presence data type in type list.
   */
  template<class JTail_t, class T>
  struct JHasType<JTypeList<T, JTail_t>, T>
  {
    enum { value = true };
  };


  /**
   * Termination of recursive test of presence data type in type list.
   */
  template<class T>
  struct JHasType<JNullType, T>
  {
    enum { value = false };
  };
  

  /**
   * Specialisation of JHasType for single class type.
   */
  template<class T>
  struct JHasType<T, T>
  {
    enum { value = true };
  };


  /**
   * Resolve template class to JTypeList.
   */
  template<class T>
  struct JResolveTypeList
  {
    typedef JTypeList<T>                 typelist;
  };
  

  /**
   * Resolve JTypeList to JTypeList.
   */
  template<class JHead_t, class JTail_t>
  struct JResolveTypeList< JTypeList<JHead_t, JTail_t> >
  {
    typedef JTypeList<JHead_t, JTail_t>  typelist;
  };


  /**
   * Extraction of data type from type list.
   *
   * Source code is taken from reference:
   * A. Alexandrescu, Modern C++ Design, Addison Wesley.
   */
  template<class JTypelist_t, unsigned int index, bool range_check = true>
  struct JTypeAt;


  /**
   * Recursive extraction of data type from type list.
   */
  template<class JHead_t, class JTail_t, unsigned int index, bool range_check>
  struct JTypeAt<JTypeList<JHead_t, JTail_t>, index, range_check>
  {
    typedef typename JTypeAt<JTail_t, index - 1, range_check>::value_type        value_type;
  };


  /**
   * Termination of recursive extraction of data type from type list.
   */
  template<class JHead_t, class JTail_t, bool range_check>
  struct JTypeAt<JTypeList<JHead_t, JTail_t>, 0, range_check>
  {
    typedef JHead_t                                                              value_type;
  };


  /**
   * Termination of recursive extraction of data type from type list.
   */
  template<unsigned int index>
  struct JTypeAt<JNullType, index, false>
  {
    typedef JNullType                                                            value_type;
  };


  /**
   * Indexing of data type in type list.
   */
  template<class JTypeList_t, class T>
  struct JIndexOf;

  
  /**
   * Recursive indexing of data type in type list.
   */
  template<class JHead_t, class JTail_t, class T>
  struct JIndexOf<JTypeList<JHead_t, JTail_t>, T>
  {
  private:
    enum { tmp   = JIndexOf<JTail_t, T>::value };

  public:
    enum { value = (tmp == -1 ? -1 : tmp + 1) };
  };
  

  /**
   * Identify indexi of data type in type list.
   */
  template<class JTail_t, class T>
  struct JIndexOf<JTypeList<T, JTail_t>, T>
  {
    enum { value = 0 };
  };


  /**
   * Termination of recursive indexing of data type in type list.
   */
  template<class T>
  struct JIndexOf<JNullType, T>
  {
    enum { value = -1 };
  };  


  /**
   * Auxiliary class for recursive type list generation.
   */
  template<class T, class ...Args>
  struct JTYPELIST {
    typedef JTypeList<T, typename JTYPELIST<Args...>::typelist>  typelist;
  };


  /**
   * Template specialisation for expanding type list.
   */
  template<class JHead_t, class JTail_t, class T, class ...Args>
  struct JTYPELIST<JTypeList<JHead_t, JTail_t>, T, Args...> {
    typedef JTypeList<JHead_t, typename JTYPELIST<JTail_t, T, Args...>::typelist>  typelist;
  };


  /**
   * Template specialisation for expanding type list.
   */
  template<class JHead_t, class T, class ...Args>
  struct JTYPELIST<JTypeList<JHead_t, JNullType>, T, Args...> {
    typedef JTypeList<JHead_t, typename JTYPELIST<T, Args...>::typelist>  typelist;
  };


  /**
   * Template specialisation for expanding type list.
   */
  template<class JHead_t, class JTail_t>
  struct JTYPELIST< JTypeList<JHead_t, JTail_t> > {
    typedef JTypeList<JHead_t, JTail_t>  typelist;
  };


  /**
   * Termination class for type list generation.
   */
  template<class T>
  struct JTYPELIST<T> {
    typedef JTypeList<T>  typelist;
  };


  /**
   * Termination class for type list generation.
   */
  template<>
  struct JTYPELIST<JNullType> {
    typedef JTypeList<>   typelist;
  };


  /**
   * For each data type method.
   *
   * The given object should provide for the function object operator
   * <pre>
   *    template<class T>
   *    void object()(JType<T> type);
   * </pre>
   *
   * \param  object           object
   * \param  typelist         type list
   * \return                  object
   */
  template<class JObject_t, class JHead_t, class JTail_t>
  JObject_t& for_each(JObject_t& object, JType< JTypeList<JHead_t, JTail_t> > typelist)
  {
    for_each(object, JType<JHead_t>());
    for_each(object, JType<JTail_t>());

    return object;
  }


  /**
   * For each data type method.
   *
   * The given object should provide for the function object operator
   * <pre>
   *    template<class T>
   *    void object()(JType<T> type);
   * </pre>
   *
   * \param  object           object
   * \param  type             type
   * \return                  object
   */
  template<class JObject_t, class T>
  JObject_t& for_each(JObject_t& object, JType<T> type)
  {
    object(type);

    return object;
  }


  /**
   * Termination method of for each data type method.
   *
   * \param  object           object
   * \param  type             null type
   * \return                  object
   */
  template<class JObject_t>
  JObject_t& for_each(JObject_t& object, JType<JNullType> type)
  {
    return object;
  }
}

#endif
