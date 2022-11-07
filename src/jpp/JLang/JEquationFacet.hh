#ifndef __JLANG__JEQUATIONFACET__
#define __JLANG__JEQUATIONFACET__

#include <istream>
#include <ostream>
#include <locale>
#include <string>
#include <iterator>
#include <cstdio>
#include <limits>

#include "JLang/JStringFacet.hh"
#include "JLang/JEquationParameters.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Facet class to specify parsing of equations in currect locale (see class JLANG::JEquation).
   * Tokens are defined as a piece of text delimited by various markers according the facet.
   * The list of markers is defined by the JLANG::JEquationParameters data structure.
   * This class extends the JLANG::JStringFacet and JLANG::JEquationParameters classes.
   */
  class JEquationFacet: 
    public JStringFacet,
    public JEquationParameters
  {
  public:

    using JStringFacet::get;
    using JStringFacet::put;


    /**
     * Default constructor.
     */
    JEquationFacet() :
      JStringFacet(),
      JEquationParameters()
    {}


    /**
     * Constructor.
     *
     * \param  parameters   equation parameters
     */
    JEquationFacet(const JEquationParameters& parameters) :
      JStringFacet(),
      JEquationParameters(parameters)
    {}


    /**
     * Clone this facet.
     *
     * \return           pointer to newly created facet
     */
    virtual JEquationFacet* clone() const override 
    {
      return new JEquationFacet(static_cast<const JEquationParameters&>(*this));
    }


    /**
     * Get character.
     *
     * \param  __begin   begin position of input stream
     * \param  __end     end   position of input stream
     * \param  format    format
     * \param  result    status after input operation
     * \param  buffer    output character
     * \return           position of input stream
     */
    istreambuf_iterator get(const istreambuf_iterator __begin,
			    const istreambuf_iterator __end,
			    const std::ios_base&      format,
			    std::ios_base::iostate&   result,
			    char&                     buffer) const
    {
      return do_get(__begin, __end, format, result, buffer);
    }


    /**
     * Put character.
     *
     * \param  out       begin position of output stream
     * \param  format    format
     * \param  c         fill character
     * \param  buffer    input character
     * \return           position of output stream buffer
     */
    ostreambuf_iterator put(ostreambuf_iterator  out,
			    const std::ios_base& format,
			    const char           c,
			    const char           buffer) const
    {
      return do_put(out, format, c, buffer);
    }


    /**
     * Get combined prefix for output.
     *
     * \param  prefix    prefix
     * \param  name      name
     * \return           prefix
     */
    const std::string getPrefix(const std::string& prefix, const std::string& name) const
    {
      if (prefix.empty())
	return name;
      else
	return prefix + getDefaultDivision() + name;
    }


    /**
     * Pop white spaces.
     *
     * \param  in        input stream
     * \return           input stream
     */
    std::istream& pop(std::istream& in) const
    {
      try {
	for (int c; (c = in.peek()) != EOF && isWhiteSpace((char) c); ) { in.get(); }
      }
      catch(const std::exception& error) {};

      return in;
    }

  protected:
    /**
     * Get string.
     *
     * \param  __begin   begin position of input stream
     * \param  __end     end   position of input stream
     * \param  format    format
     * \param  result    status after input operation
     * \param  buffer    output string
     * \return           position of input stream
     */
    virtual istreambuf_iterator do_get(const istreambuf_iterator __begin,
				       const istreambuf_iterator __end,
				       const std::ios_base&      format,
				       std::ios_base::iostate&   result,
				       std::string&              buffer) const override
    {
      using namespace std;

      result = (ios_base::iostate) 0;  // reset I/O status

      streamsize n = format.width();   // number of characters to read

      if (n == 0) {
	n = numeric_limits<streamsize>::max();
      }

      istreambuf_iterator i = __begin;

      while (i != __end && isWhiteSpace(*i)) {
	++i;
      }

      if (i == __end) {

	result |= ios_base::failbit;	
	result |= ios_base::eofbit;

      } else if (isSeparator(*i) ||
		 isEndOfLine(*i) ||
		 isDivision (*i)) {

	result |= ios_base::failbit;	

      } else { 

	buffer.clear();
	buffer.push_back(*i);

	for (++i, --n; i != __end && n != 0 && (!isWhiteSpace(*i) &&
						!isSeparator (*i) &&
						!isEndOfLine (*i) &&
						!isDivision  (*i)); ++i, --n)
	  buffer.push_back(*i);

	if (i == __end) {
	  result |= ios_base::eofbit;
	}
      }

      return i;
    }


    /**
     * Get character.
     *
     * \param  __begin   begin position of input stream
     * \param  __end     end   position of input stream
     * \param  format    format
     * \param  result    status after input operation
     * \param  buffer    output character
     * \return           position of input stream
     */
    virtual istreambuf_iterator do_get(const istreambuf_iterator __begin,
				       const istreambuf_iterator __end,
				       const std::ios_base&      format,
				       std::ios_base::iostate&   result,
				       char&                     buffer) const
    {
      using namespace std;

      result = (ios_base::iostate) 0;  // reset I/O status

      istreambuf_iterator i = __begin;

      while (i != __end && isWhiteSpace(*i)) {
	++i;
      }

      if (i == __end) {

	result |= ios_base::failbit;	
	result |= ios_base::eofbit;

      } else if (!isDivision(*i) && !isSeparator(*i)) {

	result |= ios_base::failbit;	

      } else {

	buffer = *i++;
      }

      return i;
    }


    /**
     * Put string.
     *
     * \param  out       begin position of output stream
     * \param  format    format
     * \param  c         fill character
     * \param  buffer    input string
     * \return           current position of output stream
     */
    virtual ostreambuf_iterator do_put(ostreambuf_iterator  out,
				       const std::ios_base& format,
				       const char           c,
				       const std::string&   buffer) const override
    {
      using namespace std;

      if (format.flags() & ios_base::right) {
	for (streamsize i = buffer.size(); i < format.width(); ++i, ++out) {
	  *out = c;
	}
      }

      for (string::const_iterator i = buffer.begin(); i != buffer.end(); ++i, ++out) {
	*out = *i;
      }
	
      if (format.flags() & ios_base::left) {
	for (streamsize i = buffer.size(); i < format.width(); ++i, ++out) {
	  *out = c;
	}
      }

      return out;
    }


    /**
     * Put character.
     *
     * \param  out       begin position of output stream
     * \param  format    format
     * \param  c         fill character
     * \param  buffer    input character
     * \return           current position of output stream
     */
    virtual ostreambuf_iterator do_put(ostreambuf_iterator  out,
				       const std::ios_base& format,
				       const char           c,
				       const char           buffer) const
    {
      using namespace std;

      if (format.flags() & ios_base::right) {
	for (streamsize i = 1; i < format.width(); ++i, ++out) {
	  *out = c;
	}
      }

      *out = buffer;
      ++out;
	
      if (format.flags() & ios_base::left) {
	for (streamsize i = 1; i < format.width(); ++i, ++out) {
	  *out = c;
	}
      }

      return out;
    }


    /**
     * \cond NEVER
     * Ignore characters until next end of line.
     *
     * \param  __begin   begin position of input stream
     * \param  __end     end   position of input stream
     * \return           position of input stream
     * \endcond
     */
    /*
    virtual istreambuf_iterator do_ignore(const istreambuf_iterator __begin,
					  const istreambuf_iterator __end) const
    {
      istreambuf_iterator i = __begin;

      while (i != __end && !isEndOfLine(*i)) {
	++i;
      }
	
      while (i != __end &&  isEndOfLine(*i)) {
	++i;          // skip end of line(s)
      }

      return i;
    }
    */

    /**
     * Read string.
     *
     * \param  __begin   begin position of input stream
     * \param  __end     end   position of input stream
     * \param  result    status after input operation
     * \param  buffer    output string
     * \return           position of input stream
     */
    virtual istreambuf_iterator do_getline(const istreambuf_iterator __begin,
					   const istreambuf_iterator __end,
					   std::ios_base::iostate&   result,
					   std::string&              buffer) const override
    {
      using namespace std;

      result = (ios_base::iostate) 0;  // reset I/O status

      istreambuf_iterator i = __begin;

      while (i != __end && isWhiteSpace(*i) && !isEndOfLine(*i)) {
	++i;
      }

      if (i == __end) {

	result |= ios_base::failbit;	
	result |= ios_base::eofbit;

      } else if (isEndOfLine(*i)) {

	buffer.clear();

      } else {

	buffer.clear();

	for (int count = 0; i != __end && (count != 0 || !isEndOfLine(*i)); ++i) {

	  buffer.push_back(*i);

	  if        (isLeftBracket (*i)) {
	    ++count;
	  } else if (isRightBracket(*i)) {
	    --count;
	  }
	}

	while (i != __end && isEndOfLine(*i)) {
	  ++i;          // skip end of line(s)
	}
      }

      return i;
    }

  private:

    JEquationFacet(const JEquationFacet&);   // not defined
    void operator=(const JEquationFacet&);   // not defined
  };


  /**
   * Auxiliary class for end of line.
   */
  struct JEndOfLine
  {
    /**
     * Default constructor.
     */
    JEndOfLine() :
      index(0)
    {}


    /**
     * Constructor.
     *
     * \param  i         index
     * \return           this JEndOfLine
     */
    const JEndOfLine& operator()(const unsigned int i) const
    {
      index = i;

      return *this;
    }


    /**
     * Print end of line.
     *
     * \param  out       output stream
     * \param  eol       end of line
     * \return           output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JEndOfLine& eol)
    {
      using namespace std;

      if (has_facet<JEquationFacet>(out.getloc()))
	out << use_facet<JEquationFacet>(out.getloc()).getPreferredEndOfLine(eol.index);
      else
	out << '\n';

      eol.index = 0;
      
      return out;
    }

  private:
    mutable unsigned int index;
  };


  /**
   * Type definition of stream manipulator for equational I/O.
   */
  typedef JLANG::JEquationFacet    setequation;


  /**
   * Parse facet.
   *
   * \param  in             input stream
   * \param  facet          facet
   * \return                input stream
   */
  inline std::istream& operator>>(std::istream& in, const JLANG::JEquationFacet& facet)
  {
    using namespace std;
  
    in.imbue(locale(in.getloc(), facet.clone()));
  
    return in;
  }


  /**
   * Parse facet.
   *
   * \param  out            output stream
   * \param  facet          facet
   * \return                output stream
   */
  inline std::ostream& operator<<(std::ostream& out, const JLANG::JEquationFacet& facet)
  {
    using namespace std;
  
    out.imbue(locale(out.getloc(), facet.clone()));
  
    return out;
  }


  /**
   * Print white space.
   *
   * \param  out       output stream
   * \return           output stream
   */
  inline std::ostream& white_space(std::ostream& out)
  {
    using namespace std;
    using namespace JLANG;
  
    if (has_facet<JEquationFacet>(out.getloc()))
      out << use_facet<JEquationFacet>(out.getloc()).getDefaultWhiteSpace();
    else
      out << ' ';
  
    return out;
  }


  /**
   * Print division.
   *
   * \param  out       output stream
   * \return           output stream
   */
  inline std::ostream& division(std::ostream& out)
  {
    using namespace std;
    using namespace JLANG;
  
    if (has_facet<JEquationFacet>(out.getloc()))
      out << use_facet<JEquationFacet>(out.getloc()).getDefaultDivision();
    else
      out << '.';
  
    return out;
  }


  /**
   * Print separator.
   *
   * \param  out       output stream
   * \return           output stream
   */
  inline std::ostream& separator(std::ostream& out)
  {
    using namespace std;
    using namespace JLANG;

    if (has_facet<JEquationFacet>(out.getloc()))
      out << use_facet<JEquationFacet>(out.getloc()).getDefaultSeparator();
    else
      out << '=';
  
    return out;
  }


  /**
   * Print end of line.
   */
  static const JLANG::JEndOfLine end_of_line;


  /**
   * Print left bracket.
   *
   * \param  out       output stream
   * \return           output stream
   */
  inline std::ostream& left_bracket(std::ostream& out)
  {
    using namespace std;
    using namespace JLANG;
  
    if (has_facet<JEquationFacet>(out.getloc()))
      out << use_facet<JEquationFacet>(out.getloc()).getLeftBracket();
    else
      out << '(';
  
    return out;
  }


  /**
   * Print right bracket.
   *
   * \param  out       output stream
   * \return           output stream
   */
  inline std::ostream& right_bracket(std::ostream& out)
  {
    using namespace std;
    using namespace JLANG;

    if (has_facet<JEquationFacet>(out.getloc()))
      out << use_facet<JEquationFacet>(out.getloc()).getRightBracket();
    else
      out << ')';
  
    return out;
  }
}

#endif
