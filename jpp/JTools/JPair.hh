#ifndef __JTOOLS__JPAIR__
#define __JTOOLS__JPAIR__

#include <istream>
#include <ostream>

#include "JLang/JClass.hh"
#include "JIO/JSerialisable.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JIO::JReader;
  using JIO::JWriter;


  /**
   * Template specialisation for a pair of values.
   */
  template<class JKey_t, class JValue_t>
  class JPair
  {
  public:

    typedef JKey_t                                               key_type;
    typedef JValue_t                                             mapped_type;


    /**
     * Default constructor.
     */
    JPair() :
      first (),
      second()
    {}


    /**
     * Constructor.
     *
     * \param  key          JKey_t
     * \param  value        JValue_t
     */
    JPair(typename JLANG::JClass<key_type>   ::argument_type key,
	  typename JLANG::JClass<mapped_type>::argument_type value) :
      first (key),
      second(value)
    {}


    /**
     * Read pair from input.
     *
     * \param  in       input stream
     * \param  pair     pair
     * \return          input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JPair<JKey_t, JValue_t>& pair)
    {
      in >> pair.first;
      in >> pair.second;

      return in;
    }


    /**
     * Write pair to output.
     *
     * \param  out      output stream
     * \param  pair     pair
     * \return          output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JPair<JKey_t, JValue_t>& pair)
    {
      out << pair.first;
      out << ' ';
      out << pair.second;

      return out;
    }


    /**
     * Read pair from input.
     *
     * \param  in       reader
     * \param  pair     pair
     * \return          reader
     */
    friend inline JReader& operator>>(JReader& in, JPair<JKey_t, JValue_t>& pair)
    {
      in >> pair.first;
      in >> pair.second;

      return in;
    }


    /**
     * Write pair to output.
     *
     * \param  out      writer
     * \param  pair     pair
     * \return          writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JPair<JKey_t, JValue_t>& pair)
    {
      out << pair.first;
      out << pair.second;

      return out;
    }


    const key_type&    getKey()   const { return this->first; }
    const mapped_type& getValue() const { return this->second; }

    mapped_type& getValue() { return this->second; }

    JKey_t   first;
    JValue_t second;
  };


  /**
   * Template specialisation for a pair of references.
   */
  template<class JKey_t, class JValue_t>
  class JPair<JKey_t&, JValue_t&>
  {
  public:

    typedef JKey_t                                               key_type;
    typedef JValue_t                                             mapped_type;


    /**
     * Constructor.
     *
     * \param  key          JKey_t
     * \param  value        JValue_t
     */
    JPair(key_type&    key,
	  mapped_type& value) :
      first (key),
      second(value)
    {}

    const key_type&    getKey()   const { return this->first; }
    const mapped_type& getValue() const { return this->second; }

    key_type&    getKey()   { return this->first; }
    mapped_type& getValue() { return this->second; }

    JKey_t&   first;
    JValue_t& second;
  };


  /**
   * Template specialisation for a mixed pair of const and non-const references.
   */
  template<class JKey_t, class JValue_t>
  class JPair<const JKey_t&, JValue_t&>
  {
  public:

    typedef JKey_t                                               key_type;
    typedef JValue_t                                             mapped_type;


    /**
     * Constructor.
     *
     * \param  key          JKey_t
     * \param  value        JValue_t
     */
    JPair(const key_type& key,
	  mapped_type&    value) :
      first (key),
      second(value)
    {}

    const key_type&    getKey()   const { return this->first; }
    const mapped_type& getValue() const { return this->second; }

    mapped_type& getValue() { return this->second; }

    const JKey_t& first;
    JValue_t&     second;
  };


  /**
   * Template specialisation for a pair of const references.
   */
  template<class JKey_t, class JValue_t>
  class JPair<const JKey_t&, const JValue_t&>
  {
  public:

    typedef JKey_t                                               key_type;
    typedef JValue_t                                             mapped_type;


    /**
     * Constructor.
     *
     * \param  key          JKey_t
     * \param  value        JValue_t
     */
    JPair(const key_type&    key,
	  const mapped_type& value) :
      first (key),
      second(value)
    {}

    const key_type&    getKey()   const { return this->first; }
    const mapped_type& getValue() const { return this->second; }

    const JKey_t&   first;
    const JValue_t& second;
  };
}

#endif
