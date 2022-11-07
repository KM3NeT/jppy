#ifndef __JLANG__JSTRING__
#define __JLANG__JSTRING__

#include <string>
#include <sstream>
#include <limits>
#include <ctype.h>
#include <stdio.h>

#include "JLang/JStringFacet.hh"
#include "JLang/JException.hh"
#include "JLang/JLangToolkit.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * Wrapper class around STL string class.
   */
  class JString :
    public std::string
  {
  public:

    using std::string::replace;
    using std::string::assign;


    /**
     * Default constructor.
     */
    JString() :
      std::string()
    {}
   
    
    /**
     * Constructor.
     *
     * \param  buffer         string
     */
    explicit JString(const std::string& buffer) :
      std::string(buffer)
    {}
     
    
    /**
     * Constructor.
     *
     * \param  c              char
     */
    explicit JString(const char c):
      std::string(1,c)
    {}

    
    /**
     * Constructor.
     *
     * \param  buffer         string
     * \param  length         length
     */
    explicit JString(const char* buffer, const std::string::size_type length) :
      std::string(buffer, length)
    {}
    
    
    /**
     * Constructor.
     *
     * \param  value          value
     */
    template<class T>
    JString(const T& value) :
      std::string(valueOf(value))
    {}


    /**
     * Constructor.\n
     * Each '%' in the format string will be replaced by the corresponding argument.
     *
     * \param  format           format
     * \param  args             values
     */
    template<class ...Args>
    JString(const char* format, const Args& ...args) :
      std::string(format)
    {
      __set__(args...);
    }


    /**
     * Constructor.\n
     * This constructor compiles (see below) the input string.
     *
     * \param  buffer         input string
     * \param  facet          facet
     */
    JString(const JString& buffer, const JStringFacet& facet) :
      std::string(buffer)
    {
      compile(facet);
    }
 

    /**
     * Compile token with given facet.\n
     * This method uses the given facet to parse the input string.
     * The result is then compatible with the definition of a token and may be empty.
     *
     * \param  facet          facet
     * \result                this string
     */
    JString& compile(const JStringFacet& facet)
    {
      using namespace std;

      istringstream is(*this);

      ios_base::iostate state;

      facet.get(is, istreambuf_iterator<char>(), is, state, *this);

      return *this;
    }



    /**
     * Test if this string starts with the specified prefix.
     *
     * \param  prefix         prefix
     * \return                true if this string starts with prefix; else false
     */
    bool startsWith(const std::string& prefix) const
    {						
      return this->size() >= prefix.size() && this->substr(0, prefix.size()) == prefix;
    }


    /**
     * Test if this string ends with the specified suffix.
     *
     * \param  suffix         suffix
     * \return                true if this string ends with suffix; else false
     */
    bool endsWith(const std::string& suffix) const
    {						
      return this->size() >= suffix.size() && this->substr(this->size() - suffix.size()) == suffix;
    }
    

    /**
     * Replace characters.
     *
     * \param  target         target character
     * \param  replacement    replacement character
     * \param  max            maximum number of replacements
     * \return                this string
     */
    JString& replace(const char         target,
		     const char         replacement,
		     const std::size_t  max = std::numeric_limits<std::size_t>::max())
    {
      for (std::size_t i = this->find(target), n = max; i != std::string::npos && n != 0; i = find(target,i), --n) {
	(*this)[i] = replacement;
      }
      
      return *this;
    }

    
    /**
     * Replace character sequences.
     *
     * \param  target         target string
     * \param  replacement    replacement string
     * \param  max            maximum number of replacements
     * \return                this string
     */
    JString& replace(const std::string& target,
		     const std::string& replacement,
		     const std::size_t  max = std::numeric_limits<std::size_t>::max())
    {
      for (std::size_t i = this->find(target), n = max; i != std::string::npos && n != 0; i = this->find(target,i), --n) {
	replace(this->begin() + i, this->begin() + i + target.length(), replacement);
      }
      
      return *this;
    }

    
    /**
     * Replace character sequence.
     *
     * \param  target         target string
     * \param  value          value
     * \param  max            maximum number of replacements
     * \return                this string
     */
    template<class T>
    JString& replace(const std::string& target,
		     const T&           value,
		     const std::size_t  max = std::numeric_limits<std::size_t>::max())
    {
      for (std::size_t i = this->find(target), n = max; i != std::string::npos && n != 0; i = this->find(target,i), --n) {
	replace(this->begin() + i, this->begin() + i + target.length(), JString(value));
      }
      
      return *this;
    }

    
    /**
     * Trim string.\n
     * Returns the modified string, with leading and trailing white spaces omitted.
     *
     * \return                this string
     */
    JString& trim()
    {
      *this = JLANG::trim(*this);
 
      return *this;
    }

    
    /**
     * Trim string.\n
     * Returns the modified string, with leading and trailing target characters omitted.
     *
     * \param  c              strip character
     * \return                this string
     */
    JString& trim(const char c)
    {
      *this = JLANG::trim(*this, c);

      return *this;
    }

    
    /**
     * Trim string.\n
     * Returns the modified string, with leading and trailing target characters omitted.
     *
     * \param  target         character(s) to strip
     * \return                this string
     */
    JString trim(const std::string& target)
    {
      *this = JLANG::trim(*this, target);

      return *this;
    }

    
    /**
     * Convert all character to upper case.
     *
     * \return                this string
     */
    JString& toUpper()
    {
      for (iterator i = begin(); i != end(); ++i) {
	*i = toupper(*i);
      }

      return *this;
    }

    
    /**
     * Convert all character to lower case.
     *
     * \return                this string
     */
    JString& toLower()
    {
      for (iterator i = begin(); i != end(); ++i) {
	*i = tolower(*i);
      }

      return *this;
    }

    
    /**
     * Convert enumeration type to string.
     *
     * \param  input          value
     * \return                string
     */
    static JString valueOf(const int input)
    {
      std::ostringstream os;

      if (os << input)
	return JString(os.str());
      else
	throw JException("JString::valueOf()");
    }

    
    /**
     * Convert value to string.
     *
     * \param  input          value
     * \return                string
     */
    template<class T>
    static JString valueOf(const T& input)
    {
      std::ostringstream os;

      if (os << input)
	return JString(os.str());
      else
	throw JException("JString::valueOf()");
    }


    /**
     * Convert string to value.
     *
     * \param  input          string
     * \return                value
     */
    template<class T>
    static const T& toValue(const JString& input)
    {
      static T value;

      std::istringstream is(input);

      if (is >> value)
	return value;
      else
	throw JException("JString::toValue<T>()");
    }

    
    /**
     * Assign (part of) string to value.
     *
     * \param  output         value
     * \return                remaining string
     */
    template<class T>
    JString& assign(T& output)
    {
      using namespace std;

      istringstream is(*this);

      is >> output;

      if (!is) {
	throw JException("JString::assign()");
      }

      this->assign(istreambuf_iterator<char>(is),
		   istreambuf_iterator<char>());

      return *this;
    }
 

    /**
     * Read string from input stream.
     *
     * \param  in             input stream
     * \param  object         string
     * \return                input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JString& object)
    {
      using namespace std;

      istream::sentry sentry(in);   // skips white spaces

      if (sentry) {

        const locale& loc = in.getloc();

        if (has_facet<JStringFacet>(loc)) {

          ios_base::iostate state;

          use_facet<JStringFacet>(loc).get(in, istreambuf_iterator<char>(), in, state, object);

          if (state != ios_base::goodbit && state != ios_base::eofbit) {
	    in.setstate(state);
	  }

        } else {

          in >> static_cast<string&>(object);
	}
      }

      return in;
    }


    /**
     * Write string to output stream.
     *
     * \param  out            output stream
     * \param  object         string
     * \return                output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JString& object)
    {
      using namespace std;

      const locale& loc = out.getloc();

      if (has_facet<JStringFacet>(loc)) {

        use_facet<JStringFacet>(loc).put(out, out, out.fill(), object);

      } else {

        out << static_cast<const string&>(object);
      }

      return out;
    }

  private:
    /**
     * Recursive method for formatting string.
     *
     * \param  value            next value
     * \param  args             remaining values
     */
    template<class T, class ...Args>
    void __set__(const T& value, const Args& ...args)
    {
      using namespace std;

      const size_t pos = this->find('%');

      if (pos != string::npos) {

	replace(pos, 1, JString::valueOf(value));

	__set__(args...);
      }
    }


    /**
     * Termination method for formatting string.
     */
    void __set__() const
    {}
  };


  /**
   * Read string from input stream until end of line.
   *
   * \param  in             input stream
   * \param  object         string
   * \return                input stream
   */
  inline std::istream& getline(std::istream& in, JString& object)
  {
    using namespace std;
    
    istream::sentry sentry(in, true);   // do not skip white spaces
    
    if (sentry) {
      
      const locale& loc = in.getloc();
      
      if (has_facet<JStringFacet>(loc))	{

	use_facet<JStringFacet>(loc).getline(in, object);

      } else {

	getline(in, static_cast<string&>(object));
      }
    }

    return in;
  }
}

#endif
