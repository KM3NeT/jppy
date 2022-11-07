#ifndef __JLANG__JVECTORIZE__
#define __JLANG__JVECTORIZE__

#include <ostream>
#include <vector>
#include <iterator>

#include "JLang/JSTDTypes.hh"
#include "JLang/JClass.hh"


/**
 * \file
 * Auxiliary methods to convert data members or return values of member methods of a set of objects to a single vector.
 * \author mdejong
 */
namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * Auxiliary data structure for return type of make methods.
   */
  template<class JElement_t, class JAllocator_t = std::allocator<JElement_t> >
  struct array_type :
    public std::vector<JElement_t, JAllocator_t>
  {
    /**
     * Write array to output stream.
     *
     * \param  out          output stream
     * \param  object       array
     * \return              output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const array_type& object)
    {
      for (typename array_type::const_iterator i = object.begin(); i != object.end(); ++i) {
	out << ' ' << *i;
      }

      return out;
    }
  };


  /**
   * Method to create array of values.
   *
   * \param  array        c-array of values
   * \return              data
   */
  template<class JValue_t, size_t N>
  inline const array_type<JValue_t>& make_array(const JValue_t (&array)[N])
  {
    static array_type<JValue_t> buffer;

    buffer.resize(N);

    for (size_t i = 0; i != N; ++i) {
      buffer[i] = array[i];
    }

    return buffer;
  }


  /**
   * Method to create array of values.
   *
   * \param  __begin      begin of data
   * \param  __end        end   of data
   * \return              data
   */
  template<class T>
  inline const array_type<typename std::iterator_traits<T>::value_type>& make_array(T __begin, T __end)
  {
    static array_type<typename std::iterator_traits<T>::value_type> buffer;

    buffer.assign(__begin, __end);

    return buffer;
  }


  /**
   * Method to create array of values of data member.
   *
   * \param  __begin      begin of data
   * \param  __end        end   of data
   * \param  value        pointer to data member
   * \return              data
   */
  template<class T, class JType_t, class JValue_t>
  inline const array_type<typename JClass<JValue_t>::value_type>& make_array(T __begin, T __end, JValue_t const JType_t::*value)
  {
    static array_type<typename JClass<JValue_t>::value_type> buffer;

    buffer.clear();

    for (T __p = __begin; __p != __end; ++__p) {
      buffer.push_back(*__p.*value);
    }

    return buffer;
  }


  /**
   * Method to create array of return values of member method.
   *
   * \param  __begin      begin of data
   * \param  __end        end   of data
   * \param  function     pointer to member method
   * \return              data
   */
  template<class T, class JType_t, class JValue_t>
  inline const array_type<typename JClass<JValue_t>::value_type>& make_array(T __begin, T __end, JValue_t (JType_t::*function)() const)
  {
    static array_type<typename JClass<JValue_t>::value_type> buffer;

    buffer.clear();

    for (T __p = __begin; __p != __end; ++__p) {
      buffer.push_back((*__p.*function)());
    }

    return buffer;
  }


  /**
   * Method to create array of keys of map.
   *
   * \param  data         data
   * \return              data
   */
  template<class JKey_t, class JValue_t, class JComparator_t, class JAllocator_t>
  inline const array_type<JKey_t>& get_keys(const std::map<JKey_t, JValue_t, JComparator_t, JAllocator_t>& data)
  {
    return make_array(data.begin(), data.end(), &std::map<JKey_t, JValue_t, JComparator_t, JAllocator_t>::value_type::first);
  }


  /**
   * Method to create array of values of map.
   *
   * \param  data         data
   * \return              data
   */
  template<class JKey_t, class JValue_t, class JComparator_t, class JAllocator_t>
  inline const array_type<JValue_t>& get_values(const std::map<JKey_t, JValue_t, JComparator_t, JAllocator_t>& data)
  {
    return make_array(data.begin(), data.end(), &std::map<JKey_t, JValue_t, JComparator_t, JAllocator_t>::value_type::second);
  }


  /**
   * Method to exclude outliers from already sorted data.\n
   * Note that the part after the returned iterator will be overwritten.
   *
   * \param  __begin      begin of data
   * \param  __end        end   of data
   * \param  value        pointer to data member,
   * \param  comparator   comparison method
   * \return              end   of sorted data
   */
  template<class T, class JResult_t, class JComparator_t>
  T make_set(T __begin,
	     T __end,
	     JResult_t std::iterator_traits<T>::value_type::*value,
	     const JComparator_t& comparator)

  {
    T p2 = __begin;
    T p0 = p2++;
    T p1 = p2++;

    if (p0 == __end) { return p0; }
    if (p1 == __end) { return p1; }

    if (p2 == __end) {
      if (comparator((*p0).*value, (*p1).*value))
	return p2;
      else
	return p0;
    }

    for ( ; p2 != __end; ++p2) {

      if (comparator((*p0).*value, (*p1).*value)) {
	if (comparator((*p1).*value, (*p2).*value)) {
	  *(++p0) = *p1;
	  *(++p1) = *p2;
	} else if (comparator((*p0).*value, (*p2).*value)) {
	  *p1 = *p2;
	}
      } else {
	if (comparator((*p2).*value, (*p0).*value)) {
	  *p0 = *p1;
	}
	*p1 = *p2;
      }
    }

    return ++p1;
  }
}

#endif
