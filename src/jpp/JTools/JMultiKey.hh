#ifndef __JTOOLS__JMULTIKEY__
#define __JTOOLS__JMULTIKEY__

#include <istream>
#include <ostream>
#include <utility>
#include <cmath>

#include "JLang/JClass.hh"
#include "JLang/JException.hh"
#include "JLang/JComparable.hh"
#include "JLang/JManip.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JLANG::JComparable;
  using JLANG::JClass;

  
  /**
   * \cond NEVER
   * Forward declaration of template JMultiKey class.
   * \endcond
   */
  template<unsigned int N, class JKey_t>
  class JMultiKey;
    

  namespace {
    /**
     * Auxiliary class for copying between const and non-const key types.
     */
    template<unsigned int N, class JKey_t>
    struct JArgument {
      typedef const JMultiKey<N, const JKey_t>&   argument_type;
    };
    
    
    template<unsigned int N, class JKey_t>
    struct JArgument<N, const JKey_t> {
      typedef const JMultiKey<N, JKey_t>&         argument_type;
    };
  }


  /**
   * Multidimensional key.
   *
   * This class reproduces the key of a multidimensional map.
   * The individual data members can be accessed as, e.g:
   * <pre>
   *           JMultiKey<3, key_type>  key;
   *
   *           key[[.second].second].first;
   * </pre>
   */
  template<unsigned int N, class JKey_t>
  class JMultiKey :
    public std::pair<JKey_t, JMultiKey<N-1, JKey_t> >,
    public JComparable< JMultiKey<N, JKey_t> >
  {
  public:

    typedef JKey_t                                               key_type;
    typedef JMultiKey<N-1, JKey_t>                               mapped_type;
    typedef std::pair<key_type, mapped_type>                     pair_type;


    /**
     * Default constructor.
     */
    JMultiKey() :
      pair_type()
    {}


    /**
     * Constructor.
     * The secondary key is appended to the end of the primary keys.
     *
     * \param   __first      primary   keys
     * \param   __second     secondary key
     */
    JMultiKey(typename JClass<mapped_type>::argument_type __first,
	      typename JClass<key_type>   ::argument_type __second) :
      pair_type(__first.first, mapped_type(__first.second, __second))
    {}


    /**
     * Constructor.
     * The primary key is inserted at the start of the secondary keys.
     *
     * \param   __first      primary   key
     * \param   __second     secondary keys
     */
    JMultiKey(typename JClass<key_type>   ::argument_type __first,
	      typename JClass<mapped_type>::argument_type __second) :	      
      pair_type(__first, __second)
    {}


    /**
     * Copy constructor.
     *
     * \param   key          key
     */
    JMultiKey(typename JArgument<N, JKey_t>::argument_type key) :
      pair_type(key.first, key.second)
    {}


    /**
     * Less than method.
     *
     * \param   key          key
     * \return               true if this key less than given key; else false
     */
    bool less(const JMultiKey<N, JKey_t>& key) const
    {
      if (this->first == key.first)
	return this->second.less(key.second);
      else
	return this->first < key.first;
    }


    /**
     * Get length squared.
     *
     * \return               square of length
     */
    double getLengthSquared() const
    {
      return this->first*this->first + this->second.getLengthSquared();
    }


    /**
     * Get length.
     *
     * \return               length
     */
    double getLength() const
    {
      return sqrt(this->getLengthSquared());
    }

    
    /**
     * Get frontend key.
     *
     * \return               frontend key
     */
    JMultiKey<N-1, JKey_t> front() const
    {
      return JMultiKey<N-1, JKey_t>(this->first, this->second.front());
    }


    /**
     * Get backend key.
     *
     * \return               backend key
     */
    key_type back() const
    {
      return this->second.back();
    }


    /**
     * Read key from input.
     *
     * \param  in            input stream
     * \param  key           key
     * \return               input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JMultiKey<N, JKey_t>& key)
    {
      in >> key.first;
      in >> key.second;

      return in;
    }


    /**
     * Write key to output.
     *
     * \param  out           output stream
     * \param  key           key
     * \return               output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JMultiKey<N, JKey_t>& key)
    {
      const JFormat format(out, getFormat< JMultiKey<N, JKey_t> >(JFormat_t(9, 3, std::ios::fixed | std::ios::showpos)));

      out << format << key.first;
      out << ' ';
      out << key.second;

      return out;
    }
  };


  /**
   * Two-dimensional key.
   */
  template<class JKey_t>
  class JMultiKey<2, JKey_t> :
    public std::pair<JKey_t, JMultiKey<1, JKey_t> >,
    public JComparable< JMultiKey<2, JKey_t> >
  {
  public:

    typedef JKey_t                                               key_type;
    typedef JMultiKey<1, JKey_t>                                 mapped_type;
    typedef std::pair<key_type, mapped_type>                     pair_type;


    /**
     * Default constructor.
     */
    JMultiKey() :
      pair_type()
    {}


    /**
     * Constructor.
     * The secondary key is appended to the end of the primary key.
     *
     * \param   __first      primary   key
     * \param   __second     secondary key
     */
    JMultiKey(typename JClass<mapped_type>::argument_type __first,
	      typename JClass<key_type>   ::argument_type __second) :
      pair_type(__first.first, __second)
    {}


    /**
     * Constructor.
     * The primary key is inserted at the start of the secondary key.
     *
     * \param   __first      primary   key
     * \param   __second     secondary key
     */
    JMultiKey(typename JClass<key_type>   ::argument_type __first,
	      typename JClass<mapped_type>::argument_type __second) :
      pair_type(__first, __second.first)
    {}


    /**
     * Copy constructor.
     *
     * \param   key          key
     */
    JMultiKey(typename JArgument<2, JKey_t>::argument_type key) :
      pair_type(key.first, key.second)
    {}


    /**
     * Less than method.
     *
     * \param   key          key
     * \return               true if this key less than given key; else false
     */
    bool less(const JMultiKey<2, JKey_t>& key) const
    {
      if (this->first == key.first)
	return this->second.less(key.second);
      else
	return this->first < key.first;
    }


    /**
     * Get length squared.
     *
     * \return               square of length
     */
    double getLengthSquared() const
    {
      return this->first*this->first + this->second.getLengthSquared();
    }


    /**
     * Get length.
     *
     * \return               length
     */
    double getLength() const
    {
      return sqrt(this->getLengthSquared());
    }


    /**
     * Get frontend key.
     *
     * \return               frontend key
     */
    JMultiKey<1, JKey_t> front() const
    {
      return JMultiKey<1, JKey_t>(this->first);
    }


    /**
     * Get backend key.
     *
     * \return               backend key
     */
    key_type back() const
    {
      return this->second.back();
    }


    /**
     * Read key from input.
     *
     * \param  in            input stream
     * \param  key           key
     * \return               input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JMultiKey<2, JKey_t>& key)
    {
      in >> key.first;
      in >> key.second;

      return in;
    }


    /**
     * Write key to output.
     *
     * \param  out           output stream
     * \param  key           key
     * \return               output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JMultiKey<2, JKey_t>& key)
    {
      const JFormat format(out, getFormat< JMultiKey<2, JKey_t> >(JFormat_t(9, 3, std::ios::fixed | std::ios::showpos)));

      out << format << key.first;
      out << ' ';
      out << key.second;

      return out;
    }
  };


  /**
   * One-dimensional key.
   */  
  template<class JKey_t>
  class JMultiKey<1, JKey_t> :
    public JComparable< JMultiKey<1, JKey_t> >
  {
  public:

    typedef JKey_t                                               key_type;
    typedef JMultiKey<0, JKey_t>                                 mapped_type;


    /**
     * Default constructor.
     */
    JMultiKey() :
      first()
    {}


    /**
     * Constructor.
     * The secondary key is appended to the end of the primary key.
     *
     * \param   __first      primary   key
     * \param   __second     secondary key
     */
    JMultiKey(typename JClass<mapped_type>::argument_type __first,
	      typename JClass<key_type>   ::argument_type __second) :
      first(__second)
    {}


    /**
     * Constructor.
     * The primary key is inserted at the start of the secondary key.
     *
     * \param   __first      primary   key
     * \param   __second     secondary key
     */
    JMultiKey(typename JClass<key_type>   ::argument_type __first,
	      typename JClass<mapped_type>::argument_type __second) :
      first(__first)
    {}


    /**
     * Constructor.
     *
     * \param   __first      key
     */
    JMultiKey(typename JClass<key_type>::argument_type __first) :
      first(__first)
    {}


    /**
     * Copy constructor.
     *
     * \param   key          key
     */
    JMultiKey(typename JArgument<1, JKey_t>::argument_type key) :
      first(key.first)
    {}


    /**
     * Less than method.
     *
     * \param   key          key
     * \return               true if this key less than given key; else false
     */
    bool less(const JMultiKey<1, JKey_t>& key) const
    {
      return this->first < key.first;
    }
    

    /**
     * Get length squared.
     *
     * \return               square of length
     */
    double getLengthSquared() const
    {
      return this->first*this->first;
    }


    /**
     * Get length.
     *
     * \return               length
     */
    double getLength() const
    {
      return sqrt(this->getLengthSquared());
    }


    /**
     * Get backend key.
     *
     * \return               backend key
     */
    key_type back() const
    {
      return this->first;
    }


    /**
     * Read key from input.
     *
     * \param  in            input stream
     * \param  key           key
     * \return               input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JMultiKey<1, JKey_t>& key)
    {
      in >> key.first;

      return in;
    }


    /**
     * Write key to output.
     *
     * \param  out           output stream
     * \param  key           key
     * \return               output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JMultiKey<1, JKey_t>& key)
    {
      const JFormat format(out, getFormat< JMultiKey<1, JKey_t> >(JFormat_t(9, 3, std::ios::fixed | std::ios::showpos)));

      out << format << key.first;

      return out;
    }


    key_type first;
  };


  /**
   * Empty key.
   */
  template<class JKey_t>
  class JMultiKey<0, JKey_t>
  {
  public:

    typedef JKey_t                                               key_type;


    /**
     * Default constructor.
     */
    JMultiKey()
    {}
  };
}

#endif
