#ifndef __JLANG__JSTRINGFACET__
#define __JLANG__JSTRINGFACET__

#include <istream>
#include <ostream>
#include <locale>
#include <string>
#include <iterator>
#include <limits>


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Facet class to specify parsing of a JLANG::JString object.
   * This class extends the std::locale::facet class.
   */
  class JStringFacet : 
    public std::locale::facet
  {
  public:

    static std::locale::id  id;


    typedef std::istreambuf_iterator<char, std::char_traits<char> >    istreambuf_iterator;
    typedef std::ostreambuf_iterator<char, std::char_traits<char> >    ostreambuf_iterator;


  
    /**
     * Constructor.
     *
     * \param  refs      reference count
     */
    JStringFacet(std::size_t refs = 0) :
      std::locale::facet(refs)
    {}


    /**
     * Clone this facet.
     *
     * \return           pointer to newly created facet
     */
    virtual JStringFacet* clone() const
    {
      return new JStringFacet();
    }


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
    istreambuf_iterator get(const istreambuf_iterator __begin,
			    const istreambuf_iterator __end,
			    const std::ios_base&      format,
			    std::ios_base::iostate&   result,
			    std::string&              buffer) const
    {
      return do_get(__begin, __end, format, result, buffer);
    }


    /**
     * Put string.
     *
     * \param  out       begin position of output stream
     * \param  format    format
     * \param  c         fill character
     * \param  buffer    input string
     * \return           position of output stream buffer
     */
    ostreambuf_iterator put(ostreambuf_iterator  out,
			    const std::ios_base& format,
			    const char           c,
			    const std::string&   buffer) const
    {
      return do_put(out, format, c, buffer);
    }


    /**
     * Ignore characters until next end of line.
     *
     * \param  in        input stream
     * \return           input stream
     */
    inline std::istream& ignore(std::istream& in) const
    {      
      if (do_ignore(in, istreambuf_iterator()) == istreambuf_iterator()) {
	in.setstate(std::ios_base::eofbit);
      }

      return in;
    }


    /**
     * Read characters until next end of line.
     *
     * \param  in        input stream
     * \param  buffer    output string
     * \return           input stream
     */
    inline std::istream& getline(std::istream& in, 
				 std::string&  buffer) const
    {
      using namespace std;

      ios_base::iostate state = in.rdstate();

      if (do_getline(in, istreambuf_iterator(), state, buffer) == istreambuf_iterator()) {
	in.setstate(std::ios_base::eofbit);
      }

      if (state != ios_base::goodbit && state != ios_base::eofbit) {
	in.setstate(state);
      }

      return in;
    }


    /**
     * Get index for stream associated facet data.
     *
     * \return           index
     */
    static int getIndex() 
    {
      static int i = std::ios_base::xalloc();

      return i;
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
				       std::string&              buffer) const
    {
      using namespace std;

      result = (ios_base::iostate) 0;  // reset I/O status

      streamsize n = format.width();   // number of characters to read

      if (n == 0) {
	n = numeric_limits<streamsize>::max();
      }

      istreambuf_iterator i = __begin;

      if (i == __end) {

	result |= ios_base::failbit;	
	result |= ios_base::eofbit;

      } else {

	buffer.clear();

	buffer.push_back(*i);
	  
	for (++i, --n; i != __end && n != 0; ++i, --n) {
	  buffer.push_back(*i);
	}

	if (i == __end) {
	  result |= ios_base::eofbit;
	}
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
				       const std::string&   buffer) const
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
     * Ignore characters until next end of line.
     *
     * \param  __begin   begin position of input stream
     * \param  __end     end   position of input stream
     * \return           position of input stream
     */
    virtual istreambuf_iterator do_ignore(const istreambuf_iterator __begin,
					  const istreambuf_iterator __end) const
    {
      istreambuf_iterator i = __begin;

      while (i != __end && *i != '\n') {
	++i;
      }

      while (i != __end && *i == '\n') {
	++i;          // skip end of line(s)
      }

      return i;
    }


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
					   std::string&              buffer) const
    {
      using namespace std;

      istreambuf_iterator i = __begin;

      if (i == __end) {

	result |= ios_base::failbit;	
	result |= ios_base::eofbit;

      } else {

	buffer.clear();

	for ( ; i != __end && *i != '\n'; ++i) {
	  buffer.push_back(*i);
	}

	if (i != __end) {
	  ++i;          // skip end of line
	}
      }

      return i;
    }


  private:

    JStringFacet(const JStringFacet&);        // not defined
    void operator=(const JStringFacet&);      // not defined
  };
}

#endif
