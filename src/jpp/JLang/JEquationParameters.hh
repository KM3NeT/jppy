#ifndef __JLANG__JEQUATIONPARAMETERS__
#define __JLANG__JEQUATIONPARAMETERS__

#include <string>


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {
  

  /**
   * Simple data structure to support I/O of equations (see class JLANG::JEquation).
   */
  class JEquationParameters {
  public:
    /**
     * Default constructor.
     */
    JEquationParameters()
    {
      this->sep   = "=";
      this->eol   = "\n\r;";
      this->div   = "./";
      this->skip  = "#";
      this->left  = '(';
      this->right = ')';
      this->ws    = " \t\n\v\f\r";
    }


    /**
     * Constructor.
     *
     * \param sep          separator   characters
     * \param eol          end of line characters
     * \param div          division    characters
     * \param skip         skip line   characters
     * \param left         left  bracket
     * \param right        right bracket
     * \param ws           white space characters
     */
    JEquationParameters(const std::string& sep,
			const std::string& eol,
			const std::string& div,
			const std::string& skip,
			const char         left  = '(',
			const char         right = ')',
			const std::string& ws    = " \t\n\v\f\r")
    {
      this->sep   = sep;
      this->eol   = eol;
      this->div   = div;
      this->skip  = skip;
      this->left  = left;
      this->right = right;
      this->ws    = ws;
    }


    /**
     * Get equation parameters.
     *
     * \return             equation parameters
     */
    const JEquationParameters& getEquationParameters() const
    {
      return *this;
    }


    /**
     * Set equation parameters.
     *
     * \param  buffer      equation parameters
     */
    void setEquationParameters(const JEquationParameters& buffer)
    {
      static_cast<JEquationParameters&>(*this) = buffer;
    }


    /**
     * Get default separator character.
     *
     * \return             separator between parameter and its value
     */
    const char getDefaultSeparator() const
    { 
      if (sep.empty())
	return '=';
      else
	return sep[0];
    }


    /**
     * Get separator characters.
     *
     * \return             separator between parameter and its value
     */
    const std::string& getSeparator() const
    { 
      return sep;
    }


    /**
     * Set separator character(s).
     *
     * \param sep          separator between parameter and its value
     */
    void setSeparator(const std::string& sep) 
    { 
      this->sep = sep; 
    }


    /**
     * Get default end of line character.
     *
     * \return             end of line character
     */
    const char getDefaultEndOfLine() const
    { 
      if (eol.empty())
	return '\n';
      else
	return eol[0];
    }


    /**
     * Get preferred end of line character.
     *
     * \param  index       index
     * \return             end of line character
     */
    const char getPreferredEndOfLine(const unsigned int index) const
    { 
      if      (eol.empty())
	return '\n';
      else if (index < eol.size())
	return eol[index];
      else
	return eol[0];
    }


    /**
     * Get end of line characters.
     *
     * \return             end of line characters
     */
    const std::string& getEndOfLine() const
    { 
      return eol;
    }


    /**
     * Set end of line characters.
     *
     * \param eol          end of line character
     */
    void setEndOfLine(const std::string& eol) 
    { 
      this->eol = eol;
    }


    /**
     * Get default division character.
     *
     * \return             division character
     */
    const char getDefaultDivision() const
    { 
      if (div.empty())
	return '.';
      else
	return div[0];
    }


    /**
     * Get division characters.
     *
     * \return             division characters
     */
    const std::string& getDivision() const
    { 
      return div;
    }


    /**
     * Set division characters.
     *
     * \param div          division characters
     */
    void setDivision(const std::string& div) 
    { 
      this->div = div;
    }


    /**
     * Get default skip line character.
     *
     * \return             skip line character
     */
    const char getDefaultSkipLine() const
    { 
      if (skip.empty())
	return '#';
      else
	return skip[0];
    }


    /**
     * Get skip line characters.
     *
     * \return             skip line characters
     */
    const std::string& getSkipLine() const
    { 
      return skip;
    }


    /**
     * Set skip line characters.
     *
     * \param skip         skip line characters
     */
    void setSkipLine(const std::string& skip) 
    { 
      this->skip = skip;
    }


    /**
     * Set brackets.
     *
     * \param  left        left  bracket
     * \param  right       right bracket
     */
    void setBrackets(const char left, const char right)
    {
      this->left  = left;
      this->right = right;
    }


    /**
     * Get left bracket.
     *
     * \return             left bracket
     */
    char getLeftBracket() const
    {
      return left;
    }


    /**
     * Get right bracket.
     *
     * \return             right bracket
     */
    char getRightBracket() const
    {
      return right;
    }


    /**
     * Get default white space character.
     *
     * \return             white space character
     */
    const char getDefaultWhiteSpace() const
    { 
      if (ws.empty())
	return ' ';
      else
	return ws[0];
    }


    /**
     * Get white space characters.
     *
     * \return             white space characters
     */
    const std::string& getWhiteSpace() const
    { 
      return ws;
    }


    /**
     * Set white space characters.
     *
     * \param ws           white space characters
     */
    void setWhiteSpace(const std::string& ws) 
    { 
      this->ws = ws;
    }


    /** 
     * Join equation parameters.
     *
     * \param value        equation parameters
     */
    JEquationParameters& join(const JEquationParameters& value)
    { 
      using namespace std;

      for (string::const_iterator i = value.sep.begin(); i != value.sep.end(); ++i) {
	if (!isSeparator(*i)) {
	  sep += *i;
	}
      }

      for (string::const_iterator i = value.eol.begin(); i != value.eol.end(); ++i) {
	if (!isEndOfLine(*i)) {
	  eol += *i;
	}
      }

      for (string::const_iterator i = value.div.begin(); i != value.div.end(); ++i) {
	if (!isDivision(*i)) {
	  div += *i;
	}
      }

      for (string::const_iterator i = value.skip.begin(); i != value.skip.end(); ++i) {
	if (!isSkipLine(*i)) {
	  skip += *i;
	}
      }

      for (string::const_iterator i = value.ws.begin(); i != value.ws.end(); ++i) {
	if (!isWhiteSpace(*i)) {
	  ws += *i;
	}
      }

      return *this;
    }


    /**
     * Test for separator character.
     * 
     * \param c            character
     * \result             true if separator; else false
     */
    inline bool isSeparator(const char c) const
    { 
      return sep .find(c) != std::string::npos;
    }


    /**
     * Test for end of line character.
     * 
     * \param c            character
     * \result             true if end of line; else false
     */
    inline bool isEndOfLine   (const char c) const { return eol .find(c) != std::string::npos; }


    /**
     * Test for division character.
     * 
     * \param c            character
     * \result             true if division; else false
     */
    inline bool isDivision(const char c) const
    { 
      return div .find(c) != std::string::npos;
    }


    /**
     * Test for skip line character.
     * 
     * \param c            character
     * \result             true if skip line; else false
     */
    inline bool isSkipLine(const char c) const
    { 
      return skip.find(c) != std::string::npos;
    }


    /**
     * Test for left bracket character.
     * 
     * \param c            character
     * \result             true if left bracket; else false
     */
    inline bool isLeftBracket(const char c) const
    { 
      return c == left;
    }


    /**
     * Test for right bracket character.
     * 
     * \param c            character
     * \result             true if right bracket; else false
     */
    inline bool isRightBracket(const char c) const
    { 
      return c == right;
    }


    /**
     * Test for white space character.
     * 
     * \param c            character
     * \result             true if white space; else false
     */
    inline bool isWhiteSpace(const char c) const
    { 
      return ws  .find(c) != std::string::npos;
    }

  protected:
    std::string sep;
    std::string eol;
    std::string div;
    std::string skip;
    char        left;
    char        right;
    std::string ws;
  };
}

#endif
