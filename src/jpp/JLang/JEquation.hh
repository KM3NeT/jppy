#ifndef __JLANG__JEQUATION__
#define __JLANG__JEQUATION__

#include <string>
#include <locale>
#include <cstdio>
#include <sstream>

#include "JLang/JString.hh"
#include "JLang/JEquationFacet.hh"
#include "JLang/JValue.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * General purpose equation class.
   *
   * An equation could have the following formats:
   *
   * <pre>
   *       \<skip line\>  "comment"  \<end of line\>
   *       \<skip line\>  "comment"  \<end of line\>
   *       \<key\> \<separator\> \<value\> \<end of line\>
   *       \<key\> \<separator\> \<left bracket\> \<value\> \<value> \<right bracket\> \<end of line\>
   *       \<key\>\<division\>\<key\> \<separator\> \<value\> \<end of line\>
   * </pre>
   * In this:
   *        - lines starting with one of the skip line characters are ignored;
   *        - <tt>key</tt>   is the key of the equation; 
   *        - <tt>value</tt> is the value of the equation; and 
   *        - <tt>' '</tt> could be any of the specified white space characters.
   *
   * In case of a division of the key, the value of the equation will include
   * the following key up to the end of line.
   * The special characters are defined in the JEquationParameters class.
   * The use of this class for I/O is handled via the JEquationFacet class.
   */
  class JEquation {
  public:
    /**
     * Auxiliary data structure for equation.
     */
    template<class T>
    struct equation_type {
      /**
       * Constructor.
       *
       * \param  __key            key
       * \param  __value          value
       */
      equation_type(const std::string __key,
		    const T&          __value) :
	key  (__key),
	value(__value)
      {}


      /**
       * Write equation to output stream.
       *
       * \param  out              output stream
       * \param  equation         equation
       * \return                  output stream
       */
      friend inline std::ostream& operator<<(std::ostream& out, const equation_type& equation)
      {
	using namespace std;
	
	ostream::sentry sentry(out);
	
	if (sentry) {
	  
	  const locale& loc = out.getloc();
	  
	  if (has_facet<JEquationFacet>(loc)) {
	    
	    const JEquationFacet& facet = use_facet<JEquationFacet>(loc);
	    
	    out << equation.key;
	    out << facet.getDefaultSeparator();
	    out << equation.value;
	    out << facet.getDefaultEndOfLine();
	    
	  } else {
	    
	    out << equation.key;
	    out << '=';
	    out << equation.value;
	    out << endl;
	  }
	}

	return out;
      }

      std::string key;
      const T&    value;
    };


    /**
     * Auxiliary method to create equation type.
     *
     * \param  key              key
     * \param  value            value
     */
    template<class T>
    static inline equation_type<T> make_equation(const std::string& key, const T& value)
    {
      return equation_type<T>(key, value);
    }
    
    
    /**
     * Default constructor.
     */
    JEquation() :
      key  (),
      sep  ('\0'),
      value()
    {}


    /**
     * Constructor.
     *
     * \param  key              key
     * \param  value            value
     */
    JEquation(const std::string& key, const std::string& value) :
      key  (key),
      sep  ('\0'),
      value(value)
    {}


    /**
     * Constructor.
     *
     * \param  buffer           input
     * \param  facet            facet
     */
    JEquation(const std::string& buffer, const JEquationFacet& facet)
    {
      setEquation(buffer, facet);
    }


    /**
     * Get key.
     *
     * \return                  key
     */
    const std::string& getKey() const
    {
      return key;
    }


    /**
     * Get separator.
     *
     * \return                  separator
     */
    const char getSeparator() const
    {
      return sep;
    }


    /**
     * Get value.
     *
     * \return                  value
     */
    const std::string& getValue() const
    {
      return value;
    }


    /**
     * Read equation from input stream.
     *
     * \param  in               input stream
     * \param  equation         equation
     * \return                  input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JEquation& equation)
    {
      using namespace std;

      istream::sentry sentry(in, false);

      if (sentry) {

	equation = JEquation();

	const locale& loc = in.getloc();

	if (has_facet<JEquationFacet>(loc)) {

	  const JEquationFacet& facet = use_facet<JEquationFacet>(loc);

	  ios_base::iostate state = in.rdstate();


	  for (int c; (c = in.peek()) != EOF && facet.isSkipLine((char) c); ) {
	    facet.ignore(in);
	  }


	  if (state == ios_base::goodbit) facet.get(in, istreambuf_iterator<char>(), in, state, equation.key);
	  if (state == ios_base::goodbit) facet.get(in, istreambuf_iterator<char>(), in, state, equation.sep);
	  if (state == ios_base::goodbit) facet.getline(in, equation.value);


	  // remove white spaces and brackets before evaluation

	  if (facet.isSeparator(equation.sep)) {

	    JString::const_iterator         p = equation.value. begin();
	    JString::const_reverse_iterator q = equation.value.rbegin();

	    for ( ; ; ++p, ++q) {

	      for ( ; p != equation.value. end() && facet.isWhiteSpace(*p); ++p) {}
	      for ( ; q != equation.value.rend() && facet.isWhiteSpace(*q); ++q) {}
	    
	      if (p == equation.value. end() || *p != facet.getLeftBracket() ||
		  q == equation.value.rend() || *q != facet.getRightBracket()) {
		break;
	      }
	    }

	    if (p != equation.value.begin() || q != equation.value.rbegin()) {
	      equation.value = string(p, q.base());
	    }
	  }

          if (state != ios_base::goodbit && state != ios_base::eofbit) {
            in.setstate(state);
	  }

	  if (!(state & ios_base::eofbit)) {
	    if (!facet.isDivision (equation.sep) &&
		!facet.isSeparator(equation.sep)) {
	      in.setstate(ios_base::badbit);
	    }
	  }

	} else {

	  in.setstate(ios_base::failbit);
	}
      }

      return in;
    }


    /**
     * Write equation to output stream.
     *
     * \param  out              output stream
     * \param  equation         equation
     * \return                  output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JEquation& equation)
    {
      using namespace std;

      ostream::sentry sentry(out);

      if (sentry) {

	const locale& loc = out.getloc();

	if (has_facet<JEquationFacet>(loc)) {

	  const JEquationFacet& facet = use_facet<JEquationFacet>(loc);

	  out << equation.key;
	  out << facet.getDefaultSeparator();
	  out << equation.value;
	  out << facet.getDefaultEndOfLine();

	} else {

	  out << equation.key;
	  out << equation.sep;
	  out << equation.value;
	  out << endl;
	}
      }

      return out;
    }


    /**
     * Set equation.
     *
     * \param  buffer           input
     * \param  facet            facet
     * \return                  this equation
     */
    JEquation& setEquation(const std::string& buffer, const JEquationFacet& facet)
    {
      using namespace std;

      istringstream in(buffer);

      in.imbue(locale(in.getloc(), facet.clone()));

      in >> *this;

      return *this;
    }


    /**
     * Set this equation to its value.
     *
     * \param  facet            facet
     * \return                  this equation
     */
    JEquation& setEquation(const JEquationFacet& facet)
    {
      setEquation(getValue(), facet);

      return *this;
    }


    /**
     * Extract equation.
     *
     * \param  buffer           input
     * \param  facet            facet
     * \return                  equation
     */
    static JEquation valueOf(const std::string& buffer, const JEquationFacet& facet)
    {
      return JEquation(buffer, facet);
    }


    /**
     * Convert equation to string.
     *
     * \return                  string
     */
    std::string toString() const
    {
      std::string buffer;

      buffer += getKey();
      buffer += getSeparator();
      buffer += getValue();

      return buffer;
    }

  protected:
    JString   key;
    char      sep;
    JString   value;
  };
}

#endif
