#ifndef __JLANG__JLANGTOOLKIT__
#define __JLANG__JLANGTOOLKIT__

#include <string>
#include <sstream>
#include <iterator>
#include <ctype.h>


/**
 * \author mdejong
 */

/**
 * Auxiliary classes and methods for language specific functionality.
 */
namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  using std::size_t;
  
  
  /**
   * Get size of c-array.
   *
   * \param  array          array
   * \return                size
   */
  template<class T, size_t N>
  inline size_t getSize(T (&array)[N])
  {
    return N;
  }


  /**
   * Check if two objects are indentical.
   *
   * \param  first          first  object
   * \param  second         second object
   * \return                true if addresses are equal; else false
   */
  template<class JFirst_t, class JSecond_t>
  inline bool is_identical(JFirst_t& first, JSecond_t& second)
  {
    return (void*) &first == (void*) &second;
  }

  
  /**
   * Check if string is an integer.\n
   *
   * \param  buffer         input string
   * \return                true if integer; else false
   */
  inline bool is_integer(const std::string& buffer)
  {
    using namespace std;
    
    for (string::const_iterator i = buffer. begin(); i != buffer.end(); ++i) {
      if (!isdigit(*i)) {
	return false;
      }
    }
    
    return !buffer.empty();
  }

  
  /**
   * Trim string.\n
   * Returns a copy of the string, with leading and trailing white spaces omitted.
   *
   * \param  buffer         input string
   * \return                modified string
   */
  inline std::string trim(const std::string& buffer)
  {
    using namespace std;
    
    string::const_iterator         p = buffer. begin();
    string::const_reverse_iterator q = buffer.rbegin();
    
    while (p != q.base() && isspace(*p)) { ++p; }
    while (p != q.base() && isspace(*q)) { ++q; }
    
    return string(p,q.base());
  }


  /**
   * Trim string.\n
   * Returns a copy of the string, with leading and trailing target characters omitted.
   *
   * \param  buffer         input string
   * \param  c              strip character
   * \return                modified string
   */
  inline std::string trim(const std::string& buffer, const char c)
  {
    using namespace std;

    string::const_iterator         p = buffer. begin();
    string::const_reverse_iterator q = buffer.rbegin();
    
    while (p != q.base() && *p == c) { ++p; }
    while (p != q.base() && *q == c) { ++q; }
    
    return string(p,q.base());
  }


  /**
   * Replace tokens in string.\n
   * Returns a copy of the string, with all occurrences of <tt>target</tt> replaced by <tt>replacement</tt>.
   *
   * \param  input          input       string
   * \param  target         target      string
   * \param  replacement    replacement string
   * \return                modified    string
   */
  inline std::string replace(const std::string& input, const std::string& target, const std::string& replacement)
  {
    using namespace std;

    string buffer = input;

    for (size_t i = buffer.find(target); i != string::npos; i = buffer.find(target,i)) {
      buffer.replace(buffer.begin() + i, buffer.begin() + i + target.length(), replacement);
    }

    return buffer;
  }


  /**
   * Replace characters in string.\n
   * Returns a copy of the string, with all occurrences of <tt>target</tt> replaced by <tt>replacement</tt>.
   *
   * \param  input          input       string
   * \param  target         target      character
   * \param  replacement    replacement string
   * \return                modified    string
   */
  inline std::string replace(const std::string& input, const char target, const std::string& replacement)
  {
    return replace(input, std::string(1, target), replacement);
  }


  /**
   * Replace characters in string.\n
   * Returns a copy of the string, with all occurrences of <tt>target</tt> replaced by <tt>replacement</tt>.
   *
   * \param  input          input       string
   * \param  target         target      character
   * \param  replacement    replacement character
   * \return                modified    string
   */
  inline std::string replace(const std::string& input, const char target, const char replacement)
  {
    using namespace std;

    string buffer = input;

    for (string::iterator i = buffer.begin(); i != buffer.end(); ++i) {
      if (*i == target) {
	*i = replacement;
      }
    }

    return buffer;
  }


  /**
   * Trim string.\n
   * Returns a copy of the string, with leading and trailing target characters omitted.
   *
   * \param  buffer         input string
   * \param  target         strip character(s)
   * \return                modified string
   */
  inline std::string trim(const std::string& buffer, const std::string& target)
  {
    using namespace std;
    
    string::const_iterator         p = buffer. begin();
    string::const_reverse_iterator q = buffer.rbegin();
      
    while (p != q.base() && target.find(*p) != string::npos) { ++p; }
    while (p != q.base() && target.find(*q) != string::npos) { ++q; }
    
    return string(p,q.base());
  }

  
  /**
   * Convert value to string.
   *
   * \param  value          value
   * \return                string
   */
  template<class T>
  inline std::string to_string(const T& value)
  {
    using namespace std;

    ostringstream os;

    os << value;

    return os.str();
  }


  /**
   * Convert string to value.
   *
   * \param  input          string
   * \return                value
   */
  template<class T>
  inline T to_value(const std::string& input)
  {
    using namespace std;

    T value;

    istringstream(input) >> value;

    return value;
  }


  /**
   * Convert all character to upper case.
   *
   * \param  value          value
   * \return                string
   */
  inline std::string to_upper(const std::string& value)
  {
    using namespace std;
    
    string buffer(value);

    for (string::iterator i = buffer.begin(); i != buffer.end(); ++i) {
      *i = toupper(*i);
    }

    return buffer;
  }


  /**
   * Convert all character to lower case.
   *
   * \param  value          value
   * \return                string
   */
  inline std::string to_lower(const std::string& value)
  {
    using namespace std;
    
    string buffer(value);

    for (string::iterator i = buffer.begin(); i != buffer.end(); ++i) {
      *i = tolower(*i);
    }

    return buffer;
  }


  /**
   * Count number of white space separated tokens.
   *
   * \param  buffer         input string
   * \return                number of tokens
   */
  inline size_t get_number_of_tokens(const std::string& buffer)
  {
    using namespace std;

    istringstream is(buffer);

    return distance(istream_iterator<string>(is), istream_iterator<string>());
  }


  /**
   * Check quotation.
   *
   * \param  value          value
   * \return                true if quoted; else false
   */
  inline bool is_single_quoted(const std::string& value)
  {
    return (value.size() > 1  &&  *value.begin() == '\''  &&  *value.rbegin() == '\'');
  }


  /**
   * Check quotation.
   *
   * \param  value          value
   * \return                true if quoted; else false
   */
  inline bool is_double_quoted(const std::string& value)
  {
    return (value.size() > 1  &&  *value.begin() == '\"'  &&  *value.rbegin() == '\"');
  }


  /**
   * Quote string.
   *
   * \param  value          value
   * \return                string
   */
  inline std::string single_quote(const std::string& value)
  {
    if (!is_single_quoted(value))
      return "\'" + value + "\'";
    else
      return value;
  }


  /**
   * Quote string.
   *
   * \param  value          value
   * \return                string
   */
  inline std::string double_quote(const std::string& value)
  {
    if (!is_double_quoted(value))
      return "\"" + value + "\"";
    else
      return value;
  }
}

#endif
