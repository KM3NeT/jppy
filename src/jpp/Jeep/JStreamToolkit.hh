#ifndef __JEEP__JSTREAMTOOLKIT__
#define __JEEP__JSTREAMTOOLKIT__

#include <istream>
#include <ostream>
#include <iterator>

#include "JLang/JSTDTypes.hh"


/**
 * \author mdejong
 */

namespace JEEP {}
namespace JPP { using namespace JEEP; }

namespace JEEP {

  /**
   * Stream input of object.
   *
   * \param  in            input stream
   * \param  object        object
   * \return               input stream
   */
  template<class T>
  inline std::istream& readObject(std::istream& in, T& object) 
  {
    return in >> object;
  }
  

  /**
   * Stream output of object.
   *
   * \param  out           output stream
   * \param  object        object
   * \return               output stream
   */
  template<class T>
  inline std::ostream& writeObject(std::ostream& out, const T& object)
  { 
    return out << object;
  }


  /**
   * Stream output of object.
   *
   * \param  out           output stream
   * \param  prefix        prefix
   * \param  object        object
   * \param  postfix       postfix
   * \return               output stream
   */
  template<class T>
  inline std::ostream& writeObject(std::ostream& out,
				   const char*   prefix, 
				   const T&      object,
				   const char    postfix)
  { 
    return out << prefix << object << postfix;
  }


  /**
   * Template specialisation of method readObject() for std::pair.
   *
   * \param  in            input stream
   * \param  object        object
   * \return               input stream
   */
  template<class JFirst_t, class JSecond_t>
  inline std::istream& readObject(std::istream& in, std::pair<JFirst_t, JSecond_t>& object)
  {
    readObject(in, object.first);
    readObject(in, object.second);

    return in;
  }


  /**
   * Template specialisation of method writeObject() for std::pair.
   *
   * \param  out           output stream
   * \param  object        object
   * \return               output stream
   */
  template<class JFirst_t, class JSecond_t>
  inline std::ostream& writeObject(std::ostream& out, const std::pair<JFirst_t, JSecond_t>& object)
  { 
    writeObject(out, object.first);
    writeObject(out, ' ');
    writeObject(out, object.second);

    return out;
  }


  /**
   * Template specialisation of method writeObject() for std::pair.
   *
   * \param  out           output stream
   * \param  prefix        prefix
   * \param  object        object
   * \param  postfix       postfix
   * \return               output stream
   */
  template<class JFirst_t, class JSecond_t>
  inline std::ostream& writeObject(std::ostream& out,
				   const char*   prefix, 
				   const std::pair<JFirst_t, JSecond_t>& object,
				   const char    postfix)
  { 
    writeObject(out, prefix);
    writeObject(out, object.first);
    writeObject(out, ' ');
    writeObject(out, object.second);
    writeObject(out, postfix);
      
    return out;
  }


  /**
   * Template specialisation of method readObject() for std::vector.
   *
   * \param  in            input stream
   * \param  object        object
   * \return               input stream
   */
  template<class JElement_t, class JAllocator_t>
  inline std::istream& readObject(std::istream& in, std::vector<JElement_t, JAllocator_t>& object)
  {
    for (JElement_t element; readObject(in, element); ) {
      object.push_back(element);
    }
    
    return in;
  }


  /**
   * Template specialisation of method writeObject() for std::vector.
   *
   * \param  out           output stream
   * \param  object        object
   * \return               output stream
   */
  template<class JElement_t, class JAllocator_t>
  inline std::ostream& writeObject(std::ostream& out, const std::vector<JElement_t, JAllocator_t>& object)
  { 
    for (typename std::vector<JElement_t, JAllocator_t>::const_iterator i = object.begin(); i != object.end(); ++i) {
      writeObject(out, ' ');
      writeObject(out, *i);
    }
    
    return out;
  }


  /**
   * Template specialisation of method writeObject() for std::vector.
   *
   * \param  out           output stream
   * \param  prefix        prefix
   * \param  object        object
   * \param  postfix       postfix
   * \return               output stream
   */
  template<class JElement_t, class JAllocator_t>
  inline std::ostream& writeObject(std::ostream& out,
				   const char*   prefix, 
				   const std::vector<JElement_t, JAllocator_t>& object,
				   const char    postfix)
  { 
    for (typename std::vector<JElement_t, JAllocator_t>::const_iterator i = object.begin(); i != object.end(); ++i) {
      writeObject(out, prefix, *i, postfix);
    }
    
    return out;
  }


  /**
   * Template specialisation of method readObject() for std::list.
   *
   * \param  in            input stream
   * \param  object        object
   * \return               input stream
   */
  template<class JElement_t, class JAllocator_t>
  inline std::istream& readObject(std::istream& in, std::list<JElement_t, JAllocator_t>& object)
  {
    for (JElement_t element; readObject(in, element); ) {
      object.push_back(element);
    }
    
    return in;
  }


  /**
   * Template specialisation of method writeObject() for std::list.
   *
   * \param  out           output stream
   * \param  object        object
   * \return               output stream
   */
  template<class JElement_t, class JAllocator_t>
  inline std::ostream& writeObject(std::ostream& out, const std::list<JElement_t, JAllocator_t>& object)
  { 
    for (typename std::list<JElement_t, JAllocator_t>::const_iterator i = object.begin(); i != object.end(); ++i) {
      writeObject(out, ' ');
      writeObject(out, *i);
    }
    
    return out;
  }


  /**
   * Template specialisation of method writeObject() for std::list.
   *
   * \param  out           output stream
   * \param  prefix        prefix
   * \param  object        object
   * \param  postfix       postfix
   * \return               output stream
   */
  template<class JElement_t, class JAllocator_t>
  inline std::ostream& writeObject(std::ostream& out,
				   const char*   prefix, 
				   const std::list<JElement_t, JAllocator_t>& object,
				   const char    postfix)
  { 
    for (typename std::list<JElement_t, JAllocator_t>::const_iterator i = object.begin(); i != object.end(); ++i) {
      writeObject(out, prefix, *i, postfix);
    }
    
    return out;
  }


  /**
   * Template specialisation of method readObject() for std::set.
   *
   * \param  in            input stream
   * \param  object        object
   * \return               input stream
   */
  template<class JElement_t, class JComparator_t, class JAllocator_t>
  inline std::istream& readObject(std::istream& in, std::set<JElement_t, JComparator_t, JAllocator_t>& object)
  {
    for (JElement_t element; readObject(in, element); ) {

      const std::pair<typename std::set<JElement_t, JComparator_t, JAllocator_t>::iterator, bool> result = object.insert(element);

      if (!result.second) {
	object.erase (result.first);
	object.insert(element);
      }
    }
    
    return in;
  }


  /**
   * Template specialisation of method writeObject() for std::set.
   *
   * \param  out           output stream
   * \param  object        object
   * \return               output stream
   */
  template<class JElement_t, class JComparator_t, class JAllocator_t>
  inline std::ostream& writeObject(std::ostream& out, const std::set<JElement_t, JComparator_t, JAllocator_t>& object)
  { 
    for (typename std::set<JElement_t, JComparator_t, JAllocator_t>::const_iterator i = object.begin(); i != object.end(); ++i) {
      writeObject(out, ' ');
      writeObject(out, *i);
    }
    
    return out;
  }


  /**
   * Template specialisation of method writeObject() for std::set.
   *
   * \param  out           output stream
   * \param  prefix        prefix
   * \param  object        object
   * \param  postfix       postfix
   * \return               output stream
   */
  template<class JElement_t, class JComparator_t, class JAllocator_t>
  inline std::ostream& writeObject(std::ostream& out,
				   const char*   prefix, 
				   const std::set<JElement_t, JComparator_t, JAllocator_t>& object,
				   const char    postfix)
  { 
    for (typename std::set<JElement_t, JComparator_t, JAllocator_t>::const_iterator i = object.begin(); i != object.end(); ++i) {
      writeObject(out, prefix, *i, postfix);
    }
    
    return out;
  }


  /**
   * Template specialisation of method readObject() for std::multiset.
   *
   * \param  in            input stream
   * \param  object        object
   * \return               input stream
   */
  template<class JElement_t, class JComparator_t, class JAllocator_t>
  inline std::istream& readObject(std::istream& in, std::multiset<JElement_t, JComparator_t, JAllocator_t>& object)
  {
    for (JElement_t element; readObject(in, element); ) {
      object.insert(element);
    }
    
    return in;
  }


  /**
   * Template specialisation of method writeObject() for std::multiset.
   *
   * \param  out           output stream
   * \param  object        object
   * \return               output stream
   */
  template<class JElement_t, class JComparator_t, class JAllocator_t>
  inline std::ostream& writeObject(std::ostream& out, const std::multiset<JElement_t, JComparator_t, JAllocator_t>& object)
  { 
    for (typename std::multiset<JElement_t, JComparator_t, JAllocator_t>::const_iterator i = object.begin(); i != object.end(); ++i) {
      writeObject(out, ' ');
      writeObject(out, *i);
    }
    
    return out;
  }


  /**
   * Template specialisation of method writeObject() for std::multiset.
   *
   * \param  out           output stream
   * \param  prefix        prefix
   * \param  object        object
   * \param  postfix       postfix
   * \return               output stream
   */
  template<class JElement_t, class JComparator_t, class JAllocator_t>
  inline std::ostream& writeObject(std::ostream& out,
				   const char*   prefix, 
				   const std::multiset<JElement_t, JComparator_t, JAllocator_t>& object,
				   const char    postfix)
  { 
    for (typename std::multiset<JElement_t, JComparator_t, JAllocator_t>::const_iterator i = object.begin(); i != object.end(); ++i) {
      writeObject(out, prefix, *i, postfix);
    }
    
    return out;
  }


  /**
   * Template specialisation of method readObject() for std::map.
   *
   * \param  in            input stream
   * \param  object        object
   * \return               input stream
   */
  template<class JKey_t, class JValue_t, class JComparator_t, class JAllocator_t>
  inline std::istream& readObject(std::istream& in, std::map<JKey_t, JValue_t, JComparator_t, JAllocator_t>& object)
  {
    for (std::pair<JKey_t, JValue_t> element; readObject(in, element); ) {

      const std::pair<typename std::map<JKey_t, JValue_t, JComparator_t, JAllocator_t>::iterator, bool> result = object.insert(element);

      if (!result.second) {
	result.first->second = element.second;
      }
    }

    return in;
  }


  /**
   * Template specialisation of method writeObject() for std::map.
   *
   * \param  out           output stream
   * \param  object        object
   * \return               output stream
   */
  template<class JKey_t, class JValue_t, class JComparator_t, class JAllocator_t>
  inline std::ostream& writeObject(std::ostream& out, const std::map<JKey_t, JValue_t, JComparator_t, JAllocator_t>& object)
  { 
    for (typename std::map<JKey_t, JValue_t, JComparator_t, JAllocator_t>::const_iterator i = object.begin(); i != object.end(); ++i) {
      writeObject(out, ' ');
      writeObject(out, *i);
    }

    return out;
  }
  

  /**
   * Template specialisation of method writeObject() for std::map.
   *
   * \param  out           output stream
   * \param  prefix        prefix
   * \param  object        object
   * \param  postfix       postfix
   * \return               output stream
   */
  template<class JKey_t, class JValue_t, class JComparator_t, class JAllocator_t>
  inline std::ostream& writeObject(std::ostream& out,
				   const char*   prefix, 
				   const std::map<JKey_t, JValue_t, JComparator_t, JAllocator_t>& object,
				   const char    postfix)
  { 
    for (typename std::map<JKey_t, JValue_t, JComparator_t, JAllocator_t>::const_iterator i = object.begin(); i != object.end(); ++i) {
      writeObject(out, prefix, *i, postfix);
    }

    return out;
  }


  /**
   * Template specialisation of method readObject() for std::multimap.
   *
   * \param  in            input stream
   * \param  object        object
   * \return               input stream
   */
  template<class JKey_t, class JValue_t, class JComparator_t, class JAllocator_t>
  inline std::istream& readObject(std::istream& in, std::multimap<JKey_t, JValue_t, JComparator_t, JAllocator_t>& object)
  {
    for (std::pair<JKey_t, JValue_t> element; readObject(in, element); ) {
      object.insert(element);
    }

    return in;
  }


  /**
   * Template specialisation of method writeObject() for std::multimap.
   *
   * \param  out           output stream
   * \param  object        object
   * \return               output stream
   */
  template<class JKey_t, class JValue_t, class JComparator_t, class JAllocator_t>
  inline std::ostream& writeObject(std::ostream& out, const std::multimap<JKey_t, JValue_t, JComparator_t, JAllocator_t>& object)
  { 
    for (typename std::multimap<JKey_t, JValue_t, JComparator_t, JAllocator_t>::const_iterator i = object.begin(); i != object.end(); ++i) {
      writeObject(out, ' ');
      writeObject(out, *i);
    }

    return out;
  }
  

  /**
   * Template specialisation of method writeObject() for std::multimap.
   *
   * \param  out           output stream
   * \param  prefix        prefix
   * \param  object        object
   * \param  postfix       postfix
   * \return               output stream
   */
  template<class JKey_t, class JValue_t, class JComparator_t, class JAllocator_t>
  inline std::ostream& writeObject(std::ostream& out,
				   const char*   prefix, 
				   const std::multimap<JKey_t, JValue_t, JComparator_t, JAllocator_t>& object,
				   const char    postfix)
  { 
    for (typename std::multimap<JKey_t, JValue_t, JComparator_t, JAllocator_t>::const_iterator i = object.begin(); i != object.end(); ++i) {
      writeObject(out, prefix, *i, postfix);
    }

    return out;
  }
  

  /**
   * Write array of objects. 
   *
   * \param  out           output stream
   * \param  left          left  bracket
   * \param  right         right bracket
   * \param  sep           separator
   * \param  __begin       begin of data
   * \param  __end         end   of data
   * \return               output stream
   */
  template<class T>
  inline std::ostream& writeArray(std::ostream& out,
				  const char*   left,
				  const char*   right,
				  const char*   sep,
				  T             __begin,
				  T             __end)
  { 
    if (std::distance(__begin, __end) != 0) {

      out << left;

      T i = __begin;
      
      writeObject(out, *i); 
      
      while (++i != __end) {
	
	out << sep;
	
	JEEP::writeObject(out, *i); 
      }
      
      out << right;
    }

    return out;
  }
}
#endif
