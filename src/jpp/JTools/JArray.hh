#ifndef __JTOOLS__JARRAY__
#define __JTOOLS__JARRAY__

#include <istream>
#include <ostream>
#include <iterator>
#include <algorithm>

#include "JIO/JSerialisable.hh"
#include "JTools/JMultiKey.hh"
#include "JMath/JMath.hh"
#include "JLang/JAssert.hh"
#include "JLang/JClass.hh"
#include "JLang/JException.hh"
#include "JLang/JEquals.hh"
#include "JLang/JManip.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JIO::JReader;
  using JIO::JWriter;
  using JMATH::JMath;
  using JLANG::JEquals;
  using JLANG::JIndexOutOfRange;


  /**
   * One dimensional array of template objects with fixed length.\n
   * The internal data structure consists of a standard C-array.
   */
  template<unsigned int N, class T>
  class JArray :
    public JMath  < JArray<N,T> >,
    public JEquals< JArray<N,T> >
  {
  public:

    typedef  const T*                                      const_pointer;
    typedef  T*                                            pointer;
    typedef  const T*                                      const_iterator;
    typedef  T*                                            iterator;
    typedef  std::reverse_iterator<const_iterator>         const_reverse_iterator;
    typedef  std::reverse_iterator<iterator>               reverse_iterator;
    typedef  T&                                            reference;
    typedef  const T&                                      const_reference;
    typedef  unsigned int                                  size_type;
    typedef  typename JLANG::JClass<T>::argument_type      argument_type;


    /**
     * Default constructor.
     */
    JArray()
    {
      fill(T());
    }


    /**
     * Copy constructor.
     *
     * \param  array            array
     */
    template<unsigned int M>
    JArray(const JArray<M, T>& array)
    {
      STATIC_CHECK(M >= N);

      std::copy_n(array.data(), N, this->data());
    }


    /**
     * Copy constructor.
     *
     * \param  array            array
     */
    template<unsigned int M>
    JArray(const JArray<M, const T>& array)
    {
      STATIC_CHECK(M >= N);
 
      std::copy_n(array.data(), N, this->data());
    }


    /**
     * Copy constructor.
     *
     * \param  p                pointer to data
     */
    JArray(const T* p)
    {
      std::copy_n(p, N, this->data());
    }


    /**
     * Copy constructor.
     *
     * \param  key              multi-dimensional key
     */
    JArray(const JMultiKey<N, T>& key)
    {
      assign(key);
    }


    /**
     * Copy constructor.
     *
     * \param  key              multi-dimensional key
     */
    JArray(const JMultiKey<N, const T>& key)
    {
      assign(key);
    }


    /**
     * Append constructor.
     *
     * \param  array            array
     * \param  value            value
     */
    JArray(const JArray<N-1, T>& array, argument_type value)
    {
      std::copy_n(array.data(), N-1, this->data());

      this->buffer[N-1] = value;
    }


    /**
     * Append constructor.
     *
     * \param  array            array
     * \param  value            value
     */
    JArray(const JArray<N-1, const T>& array, argument_type value)
    {
      std::copy_n(array.data(), N-1, this->data());

      this->buffer[N-1] = value;
    }


    /**
     * Append constructor.
     *
     * \param  key              multi-dimensional key
     * \param  value            value
     */
    JArray(const JMultiKey<N-1, T>& key, argument_type value)
    {
      assign(key, value);
    }


    /**
     * Append constructor.
     *
     * \param  key              multi-dimensional key
     * \param  value            value
     */
    JArray(const JMultiKey<N-1, const T>& key, argument_type value)
    {
      assign(key, value);
    }


    /**
     * Initialise constructor.
     *
     * \param  value            first value
     * \param  args             remaining values
     */
    template<class ...Args>
    JArray(argument_type value, const Args& ...args)
    {
      set(value, args...);
    }


    /**
     * Set array.
     *
     * \param  args             values
     */
    template<class ...Args>
    JArray& set(const Args& ...args)
    {
      STATIC_CHECK(N == sizeof...(Args));

      __set__(0, args...);

      return *this;
    }


    const_iterator begin() const { return buffer; }                                    //!< get iterator to begin of data
    const_iterator end()   const { return buffer + N; }                                //!< get iterator to end   of data


    iterator begin() { return buffer; }                                                //!< get iterator to begin of data
    iterator end()   { return buffer + N; }                                            //!< get iterator to end   of data


    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }    //!< get reverse iterator to begin of data 
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }  //!< get reverse iterator to begin of data 


    reverse_iterator rbegin() { return reverse_iterator(end()); }                      //!< get reverse iterator to begin of data 
    reverse_iterator rend()   { return reverse_iterator(begin()); }                    //!< get reverse iterator to end   of data 


    /**
     * Get element at given index.
     *
     * \param  index            index
     * \return                  element
     */
    const_reference operator[](int index) const
    { 
      return buffer[index];
    }


    /**
     * Get element at given index.
     *
     * \param  index            index
     * \return                  element
     */
    reference operator[](int index)
    {
      return buffer[index];
    }
    

    /**
     * Get element at given index.
     *
     * \param  index            index
     * \return                  element at index
     */
    const_reference at(int index) const 
    { 
      if (index >= 0 && index < N)
	return buffer[index];
      else
	THROW(JIndexOutOfRange, "invalid index " << 0 << " <= " << index << " < " << N);
    }
    

    /**
     * Get element at given index.
     *
     * \param  index            index
     * \return                  element at index
     */
    reference at(int index)
    { 
      if (index >= 0 && index < N)
	return buffer[index];
      else
	THROW(JIndexOutOfRange, "invalid index " << 0 << " <= " << index << " < " << N);
    }


    /**
     * Get pointer to data.
     *
     * \return                  pointer to data
     */
    const_pointer data() const 
    { 
      return buffer;
    }


    /**
     * Get pointer to data.
     *
     * \return                  pointer to data
     */
    pointer data()
    { 
      return buffer;
    }


    /**
     * Get size of data.
     *
     * \return                  size of data
     */
    static size_type size()
    { 
      return N;
    }


    /**
     * Make a copy in which the first element is removed.
     *
     * \return                  array
     */
    JArray<N-1, T> pop_front() const
    {
      return JArray<N-1, T>(&buffer[1]);
    }


    /**
     * Make a copy in which the last element is removed.
     *
     * \return                  array
     */
    JArray<N-1, T> pop_back() const
    {
      return JArray<N-1, T>(buffer);
    }


    /**
     * Fill array.
     *
     * \param  value            value
     */
    void fill(argument_type value)
    {
      for (int i = 0; i != N; ++i) {
	buffer[i] = value;
      }
    }


    /**
     * Negate array.
     *
     * \return                  this array
     */
    JArray& negate()
    {
      for (int i = 0; i != N; ++i) {
	buffer[i] = -buffer[i];
      }

      return *this;
    }


    /**
     * Add array.
     *
     * \param  array            array
     * \return                  this array
     */
    JArray& add(const JArray& array)
    {
      for (int i = 0; i != N; ++i) {
	buffer[i] += array[i];
      }

      return *this;
    }


    /**
     * Subtract array.
     *
     * \param  array            array
     * \return                  this array
     */
    JArray& sub(const JArray& array)
    {
      for (int i = 0; i != N; ++i) {
	buffer[i] -= array[i];
      }

      return *this;
    }


    /**
     * Scale array.
     *
     * \param  factor           multiplication factor
     * \return                  this array
     */
    JArray& mul(const double factor)
    {
      for (int i = 0; i != N; ++i) {
	buffer[i] *= factor;
      }

      return *this;
    }


    /**
     * Scale array.
     *
     * \param  factor           division factor
     * \return                  this array
     */
    JArray& div(const double factor)
    {
      for (int i = 0; i != N; ++i) {
	buffer[i] /= factor;
      }

      return *this;
    }


    /** 
     * Check equality.
     *
     * \param  array            array
     * \return                  true if arrays are equal; else false
     */
    bool equals(const JArray<N, T>& array) const
    {
      for (int i = 0; i != N; ++i) {
	if (buffer[i] != array[i]) {
	  return false;
	}
      }

      return true;
    }


    /**
     * Read array from input stream.
     *
     * \param  in               input stream
     * \param  array            array
     * \return                  input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JArray& array)
    {
      for (iterator i = array.begin(); i != array.end(); ++i) {
	in >> *i;
      }

      return in;
    }


    /**
     * Write array to output stream.
     *
     * \param  out              output stream
     * \param  array            array
     * \return                  output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JArray& array)
    {
      for (const_iterator i = array.begin(); i != array.end(); ++i) {
	out << ' ' << SCIENTIFIC(12,5) << *i;
      }

      return out;
    }


    /**
     * Read array from input.
     *
     * \param  in               reader
     * \param  buffer           array
     * \return                  reader
     */
    friend inline JReader& operator>>(JReader& in, JArray& buffer)
    {
      for (iterator i = buffer.begin(); i != buffer.end(); ++i) {
        in >> *i;
      }

      return in;
    }


    /**
     * Write array to output.
     *
     * \param  out              writer
     * \param  buffer           array
     * \return                  writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JArray& buffer)
    {
      for (const_iterator i = buffer.begin(); i != buffer.end(); ++i) {
        out << *i;
      }

      return out;
    }

  protected:

    T buffer[N];


    /**
     * Recursive method for setting array.
     *
     * \param  i                index
     * \param  x                value at given index
     * \param  args             remaining values
     */
    template<class ...Args>
    void __set__(const int i, const argument_type x, const Args& ...args)
    {
      this->buffer[i] = x;

      __set__(i + 1, args...);
    }


    /**
     * Termination method for setting array.
     *
     * \param  i                index
     */
    void __set__(const int i) const
    {}


    /**
     * Recursive method for setting array.
     *
     * \param  key              multi-dimensional key
     */
    template<unsigned int M>
    void assign(const JMultiKey<M, T>& key)
    {
      buffer[N-M] = key.first;

      assign(key.second);
    }


    /**
     * Recursive method for setting array.
     *
     * \param  key              multi-dimensional key
     */
    template<unsigned int M>
    void assign(const JMultiKey<M, const T>& key)
    {
      buffer[N-M] = key.first;

      assign(key.second);
    }


    /**
     * Termination method for setting array.
     *
     * \param  key              one-dimensional key
     */
    void assign(const JMultiKey<1, T>& key)
    {
      buffer[N-1] = key.first;
    }


    /**
     * Termination method for setting array.
     *
     * \param  key              one-dimensional key
     */
    void assign(const JMultiKey<1, const T>& key)
    {
      buffer[N-1] = key.first;
    }


    /**
     * Recursive method for setting array.
     *
     * \param  key              multi-dimensional key
     * \param  value            value
     */
    template<unsigned int M>
    void assign(const JMultiKey<M, T>& key, argument_type value)
    {
      buffer[N-M-1] = key.first;

      assign(key.second, value);
    }


    /**
     * Recursive method for setting array.
     *
     * \param  key              multi-dimensional key
     * \param  value            value
     */
    template<unsigned int M>
    void assign(const JMultiKey<M, const T>& key, argument_type value)
    {
      buffer[N-M-1] = key.first;

      assign(key.second, value);
    }


    /**
     * Termination method for setting array.
     *
     * \param  key              one-dimensional key
     * \param  value            value
     */
    void assign(const JMultiKey<1, T>& key, argument_type value)
    {
      buffer[N-2] = key.first;
      buffer[N-1] = value;
    }


    /**
     * Termination method for setting array.
     *
     * \param  key              one-dimensional key
     * \param  value            value
     */
    void assign(const JMultiKey<1, const T>& key, argument_type value)
    {
      buffer[N-2] = key.first;
      buffer[N-1] = value;
    }
  };


  /**
   * One dimensional array of template objects with fixed length.
   * The internal data structure consists of a standard C-array.
   */
  template<class T>
  class JArray<1, T> :
    public JMath  < JArray<1,T> >,
    public JEquals< JArray<1,T> >
  {
  public:

    static const unsigned int N = 1;

    typedef  const T*                                      const_pointer;
    typedef  T*                                            pointer;
    typedef  const T*                                      const_iterator;
    typedef  T*                                            iterator;
    typedef  std::reverse_iterator<const_iterator>         const_reverse_iterator;
    typedef  std::reverse_iterator<iterator>               reverse_iterator;
    typedef  T&                                            reference;
    typedef  const T&                                      const_reference;
    typedef  unsigned int                                  size_type;
    typedef  typename JLANG::JClass<T>::argument_type      argument_type;


    /**
     * Default constructor.
     */
    JArray()
    {
      fill(T());
    }


    /**
     * Copy constructor.
     *
     * \param  array            array
     */
    template<unsigned int M>
    JArray(const JArray<M, T>& array)
    {
      buffer[0] = array[0];
    }


    /**
     * Copy constructor.
     *
     * \param  array            array
     */
    template<unsigned int M>
    JArray(const JArray<M, const T>& array)
    {
      buffer[0] = array[0];
    }


    /**
     * Copy constructor.
     *
     * \param  p                pointer to data
     */
    JArray(const T* p)
    {
      buffer[0] = p[0];
    }


    /**
     * Copy constructor.
     *
     * \param  key              multi-dimensional key
     */
    JArray(const JMultiKey<1, T>& key)
    {
      buffer[0] = key.first;
    }


    /**
     * Copy constructor.
     *
     * \param  key              multi-dimensional key
     */
    JArray(const JMultiKey<1, const T>& key)
    {
      buffer[0] = key.first;
    }


    /**
     * Initialise constructor.
     *
     * \param  value            value;
     */
    JArray(argument_type value)
    {
      buffer[0] = value;
    }


    const_iterator begin() const { return buffer; }                                    //!< get iterator to begin of data
    const_iterator end()   const { return buffer + N; }                                //!< get iterator to end   of data


    iterator begin() { return buffer; }                                                //!< get iterator to begin of data
    iterator end()   { return buffer + N; }                                            //!< get iterator to end   of data


    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }    //!< get reverse iterator to begin of data 
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }  //!< get reverse iterator to begin of data 


    reverse_iterator rbegin() { return reverse_iterator(end()); }                      //!< get reverse iterator to begin of data 
    reverse_iterator rend()   { return reverse_iterator(begin()); }                    //!< get reverse iterator to end   of data 


    /**
     * Get element at given index.
     *
     * \param  index            index
     * \return                  element
     */
    const_reference operator[](int index) const
    { 
      return buffer[index];
    }


    /**
     * Get element at given index.
     *
     * \param  index            index
     * \return                  element
     */
    reference operator[](int index)
    {
      return buffer[index];
    }
    

    /**
     * Get element at given index.
     *
     * \param  index            index
     * \return                  element at index
     */
    const_reference at(int index) const 
    { 
      if (index == 0)
	return buffer[index];
      else
	THROW(JIndexOutOfRange, "invalid index " << 0 << " <= " << index << " < " << N);
    }
    

    /**
     * Get element at given index.
     *
     * \param  index            index
     * \return                  element at index
     */
    reference at(int index)
    { 
      if (index == 0)
	return buffer[index];
      else
	THROW(JIndexOutOfRange, "invalid index " << 0 << " <= " << index << " < " << N);
    }


    /**
     * Get pointer to data.
     *
     * \return                  pointer to data
     */
    const_pointer data() const 
    { 
      return buffer;
    }


    /**
     * Get pointer to data.
     *
     * \return                  pointer to data
     */
    pointer data()
    { 
      return buffer;
    }


    /**
     * Get size of data.
     *
     * \return                  size of data
     */
    static size_type size()
    { 
      return N;
    }


    /**
     * Fill array.
     *
     * \param  value            value
     */
    void fill(argument_type value)
    {
      buffer[0] = value;
    }


    /**
     * Negate array.
     *
     * \return                  this array
     */
    JArray& negate()
    {
      buffer[0] = -buffer[0];

      return *this;
    }


    /**
     * Add array.
     *
     * \param  array            array
     * \return                  this array
     */
    JArray& add(const JArray& array)
    {
      buffer[0] += array[0];

      return *this;
    }


    /**
     * Subtract array.
     *
     * \param  array            array
     * \return                  this array
     */
    JArray& sub(const JArray& array)
    {
      buffer[0] -= array[0];

      return *this;
    }


    /**
     * Scale array.
     *
     * \param  factor           multiplication factor
     * \return                  this array
     */
    JArray& mul(const double factor)
    {
      buffer[0] *= factor;

      return *this;
    }


    /**
     * Scale array.
     *
     * \param  factor           division factor
     * \return                  this array
     */
    JArray& div(const double factor)
    {
      buffer[0] /= factor;

      return *this;
    }


    /** 
     * Check equality.
     *
     * \param  array            array
     * \return                  true if arrays are equal; else false
     */
    bool equals(const JArray<N, T>& array) const
    {
      return buffer[0] == array[0];
    }


    /**
     * Read array from input stream.
     *
     * \param  in               input stream
     * \param  array            array
     * \return                  input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JArray& array)
    {
      for (iterator i = array.begin(); i != array.end(); ++i) {
	in >> *i;
      }

      return in;
    }


    /**
     * Write array to output stream.
     *
     * \param  out              output stream
     * \param  array            array
     * \return                  output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JArray& array)
    {
      for (const_iterator i = array.begin(); i != array.end(); ++i) {
	out << ' ' << SCIENTIFIC(12,5) << *i;
      }

      return out;
    }


    /**
     * Read array from input.
     *
     * \param  in               reader
     * \param  buffer           array
     * \return                  reader
     */
    friend inline JReader& operator>>(JReader& in, JArray& buffer)
    {
      return in >> buffer[0];
    }


    /**
     * Write array to output.
     *
     * \param  out              writer
     * \param  buffer           array
     * \return                  writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JArray& buffer)
    {
      return out << buffer[0];
    }

  protected:
    T buffer[1];
  };


  /**
   * One dimensional read-only array of template objects with fixed length.
   * The internal data structure consists of a simple C-pointer to the actual data.
   * The user should ensure that the data are persistent.
   */ 
  template<unsigned int N, class T>
  class JArray<N, const T> {
  public:

    friend class JArray<N + 1, const T>;

    typedef  const T*                               const_pointer;
    typedef  const T*                               const_iterator;
    typedef  std::reverse_iterator<const_iterator>  const_reverse_iterator;
    typedef  const T&                               const_reference;
    typedef  unsigned int                           size_type;


    /**
     * Copy constructor.
     *
     * \param  array            array
     */
    template<unsigned int M>
    JArray(const JArray<M, T>& array) :
      p(array.data())
    {
      STATIC_CHECK(M >= N);
    }


    /**
     * Copy constructor.
     *
     * \param  array            array
     */
    template<unsigned int M>
    JArray(const JArray<M, const T>& array) :
      p(array.data())
    {
      STATIC_CHECK(M >= N);
    }


    const_iterator begin() const { return p; }                                         //!< get iterator to begin of data
    const_iterator end()   const { return p + N; }                                     //!< get iterator to end   of data


    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }    //!< get reverse iterator to begin of data 
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }  //!< get reverse iterator to end   of data 


    /**
     * Get element at given index.
     *
     * \param  index            index
     * \return                  element at index
     */
    const_reference operator[](int index) const
    {
      return p[index];
    }
    

    /**
     * Get element at given index.
     *
     * \param  index            index
     * \return                  element at index
     */
    const_reference at(int index) const 
    { 
      if (index >= 0 && index < N)
	return p[index];
      else
	THROW(JIndexOutOfRange, "invalid index " << 0 << " <= " << index << " < " << N);
    }


    /**
     * Get pointer to data.
     *
     * \return                  pointer to data
     */
    const_pointer data() const 
    { 
      return p;
    }
    

    /**
     * Get size of data.
     *
     * \return                  size of data
     */
    static size_type size()
    { 
      return N;
    }


    /**
     * Make a copy in which the first element is removed.
     *
     * \return                  array
     */
    JArray<N-1, const T> pop_front() const
    {
      return JArray<N-1, const T>(p + 1);
    }


    /**
     * Make a copy in which the last element is removed.
     *
     * \return                  array
     */
    JArray<N-1, const T> pop_back() const
    {
      return JArray<N-1, const T>(p);
    }


    /**
     * Write array to output.
     *
     * \param  out              writer
     * \param  buffer           array
     * \return                  writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JArray& buffer)
    {
      for (const_iterator i = buffer.begin(); i != buffer.end(); ++i) {
        out << *i;
      }

      return out;
    }

  protected:

    const_pointer p;

    /**
     * Constructor.
     *
     * \param  __p              pointer to data
     */
    JArray(const T* __p) :
      p(__p)
    {}
  };


  /**
   * One dimensional read-only array of template objects with fixed length.
   * The internal data structure consists of a simple C-pointer to the actual data.
   * The user should ensure that the data are persistent.
   */ 
  template<class T>
  class JArray<1, const T> {
  public:

    static const unsigned int N = 1;

    friend class JArray<N + 1, const T>;

    typedef  const T*                               const_pointer;
    typedef  const T*                               const_iterator;
    typedef  std::reverse_iterator<const_iterator>  const_reverse_iterator;
    typedef  const T&                               const_reference;
    typedef  unsigned int                           size_type;


    /**
     * Copy constructor.
     *
     * \param  array            array
     */
    template<unsigned int M>
    JArray(const JArray<M, T>& array) :
      p(array.data())
    {
      STATIC_CHECK(M >= N);
    }


    /**
     * Copy constructor.
     *
     * \param  array            array
     */
    template<unsigned int M>
    JArray(const JArray<M, const T>& array) :
      p(array.data())
    {
      STATIC_CHECK(M >= N);
    }


    const_iterator begin() const { return p; }                                         //!< get iterator to begin of data
    const_iterator end()   const { return p + N; }                                     //!< get iterator to end   of data


    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }    //!< get reverse iterator to begin of data 
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }  //!< get reverse iterator to end   of data 


    /**
     * Get element at given index.
     *
     * \param  index            index
     * \return                  element at index
     */
    const_reference operator[](int index) const
    {
      return p[index];
    }
    

    /**
     * Get element at given index.
     *
     * \param  index            index
     * \return                  element at index
     */
    const_reference at(int index) const 
    { 
      if (index >= 0 && index < N)
	return p[index];
      else
	THROW(JIndexOutOfRange, "invalid index " << 0 << " <= " << index << " < " << N);
    }


    /**
     * Get pointer to data.
     *
     * \return                  pointer to data
     */
    const_pointer data() const 
    { 
      return p;
    }
    

    /**
     * Get size of data.
     *
     * \return                  size of data
     */
    static size_type size()
    { 
      return N;
    }


    /**
     * Write array to output.
     *
     * \param  out              writer
     * \param  buffer           array
     * \return                  writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JArray& buffer)
    {
      return out << buffer[0];
    }

  protected:

    const_pointer p;

    /**
     * Constructor.
     *
     * \param  __p              pointer to data
     */
    JArray(const T* __p) :
      p(__p)
    {}
  };
}

#endif
