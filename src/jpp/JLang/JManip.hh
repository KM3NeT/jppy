#ifndef __JLANG_JMANIP__
#define __JLANG_JMANIP__

#include <string>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <functional>


/**
 * \file
 * I/O manipulators.
 * \author mdejong
 */
namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * Get index for user I/O manipulation.
   *
   * \return           index
   */
  inline int getIndex() 
  {
    static const int index = std::ios_base::xalloc();

    return index;
  }

  
  /**
   * Print options.
   */
  enum JPrintOption_t {
		       SHORT_PRINT  = 1,    //!< short  print
		       MEDIUM_PRINT = 2,    //!< medium print
		       LONG_PRINT   = 3     //!< long   print
  };
}


/**
 * Get print option.
 *
 * \param  out       output stream
 * \return           print option
 */
inline int getPrintOption(std::ostream& out)
{
  return out.iword(JLANG::getIndex());
}


/**
 * Set print option.
 *
 * \param  out       output stream
 * \param  option    print option
 */
inline void setPrintOption(std::ostream& out, const int option)
{
  out.iword(JLANG::getIndex()) = option;
}


/**
 * Get short print option.
 *
 * \param  out       output stream
 * \return           true if short print option is on; else false
 */
inline bool getShortprint(std::ostream& out)
{
  return getPrintOption(out) == JLANG::SHORT_PRINT;
}


/**
 * Set short print option.
 *
 * \param  out       output stream
 */
inline void setShortprint(std::ostream& out)
{
  return setPrintOption(out, JLANG::SHORT_PRINT);
}


/**
 * Get medium print option.
 *
 * \param  out       output stream
 * \return           true if medium print option is on; else false
 */
inline bool getMediumprint(std::ostream& out)
{
  return getPrintOption(out) == JLANG::MEDIUM_PRINT;
}


/**
 * Set medium print option.
 *
 * \param  out       output stream
 */
inline void setMediumprint(std::ostream& out)
{
  return setPrintOption(out, JLANG::MEDIUM_PRINT);
}


/**
 * Get long print option.
 *
 * \param  out       output stream
 * \return           true if long print option is on; else false
 */
inline bool getLongprint(std::ostream& out)
{
  return getPrintOption(out) == JLANG::LONG_PRINT;
}


/**
 * Set long print option.
 *
 * \param  out       output stream
 */
inline void setLongprint(std::ostream& out)
{
  return setPrintOption(out, JLANG::LONG_PRINT);
}


/**
 * Set short printing.
 *
 * \param  out       output stream
 * \return           output stream
 */
inline std::ostream& shortprint(std::ostream& out)
{
  setShortprint(out);

  return out;
}


/**
 * Set medium printing.
 *
 * \param  out       output stream
 * \return           output stream
 */
inline std::ostream& mediumprint(std::ostream& out)
{
  setMediumprint(out);

  return out;
}


/**
 * Set long printing.
 *
 * \param  out       output stream
 * \return           output stream
 */
inline std::ostream& longprint(std::ostream& out)
{
  setLongprint(out);

  return out;
}


/**
 * Print newline character.
 *
 * \param  out       output stream
 * \return           output stream
 */
inline std::ostream& newline(std::ostream& out)
{
  return out << '\n';
}


/**
 * Print white space character.
 *
 * \param  out       output stream
 * \return           output stream
 */
inline std::ostream& whitespace(std::ostream& out)
{
  return out << ' ';
}


/**
 * Print tab character.
 *
 * \param  out       output stream
 * \return           output stream
 */
inline std::ostream& tab(std::ostream& out)
{
  return out << '\t';
}


/**
 * Rewind character.
 *
 * \param  out       output stream
 * \return           output stream
 */
inline std::ostream& rewind(std::ostream& out)
{
  return (out << '\r').flush();
}


/**
 * Auxiliary data structure for alignment of data.
 */
struct WIDTH {
  /**
   * Constructor.
   *
   * \param  width         width
   */
  WIDTH(const int width)
  {
    this->width = width;
  }


  /**
   * Format specifier.
   *
   * \param  out           output stream
   * \param  format        format
   * \return               output stream
   */
  friend inline std::ostream& operator<<(std::ostream& out, const WIDTH& format)
  {
    using namespace std;
    
    return out << setw(format.width);
  }
  
  int  width;
};


/**
 * Auxiliary data structure for alignment of data.
 */
struct LEFT :
  public WIDTH
{
  /**
   * Constructor.
   *
   * \param  width         width
   */
  LEFT(const int width) :
    WIDTH(width)
  {}


  /**
   * Format specifier.
   *
   * \param  out           output stream
   * \param  format        format
   * \return               output stream
   */
  friend inline std::ostream& operator<<(std::ostream& out, const LEFT& format)
  {
    using namespace std;
    
    return out << setw(format.width) << left;
  }
};


/**
 * Auxiliary data structure for alignment of data.
 */
struct RIGHT :
  public WIDTH
{
  /**
   * Constructor.
   *
   * \param  width         width
   */
  RIGHT(const int width) :
    WIDTH(width)
  {}


  /**
   * Format specifier.
   *
   * \param  out           output stream
   * \param  format        format
   * \return               output stream
   */
  friend inline std::ostream& operator<<(std::ostream& out, const RIGHT& format)
  {
    using namespace std;
    
    return out << setw(format.width) << right;
  }
};


/**
 * Auxiliary data structure for sequence of same character.
 */
struct FILL :
  public WIDTH
{
  /**
   * Constructor.
   *
   * \param  width         width
   * \param  fill          fill character
   */
  FILL(const int  width = 0,
       const char fill  = ' ') :
    WIDTH(width)
  {
    this->fill = fill;
  }

  
  /**
   * Format specifier.
   *
   * \param  out           output stream
   * \param  format        format
   * \return               output stream
   */
  friend inline std::ostream& operator<<(std::ostream& out, const FILL& format)
  {
    using namespace std;

    return out << setfill(format.fill) << setw(format.width); 
  }

  char fill;
};


/**
 * Auxiliary data structure for alignment of data.
 */
struct CENTER :
  public WIDTH
{
protected:
  /**
   * Auxiliary class for format center.
   */
  struct JCenter :
    public WIDTH 
  {
    /**
     * Constructor.
     *
     * \param  out         output stream
     * \param  format      format center
     */
    JCenter(std::ostream& out, const WIDTH& format) :
      WIDTH(format),
      out   (out)
    {}


    /**
     * Write value to output stream.
     *
     * \param  value       value
     * \return             this JCenter
     */
    template<class T>
    std::ostream& operator<<(const T& value)
    {
      using namespace std;

      ostringstream os;

      os.copyfmt(out);

      os << value;

      const int  w = this->width - os.str().size();
      const char c = this->out.fill();

      if (w > 0)
	return this->out << FILL(w/2) << ' ' << os.str() << FILL((w+1)/2) << ' ' << setfill(c);
      else
	return this->out << os.str();
    }

  private:
    std::ostream& out; 
  };

public:
  /**
   * Constructor.
   *
   * \param  width         width
   */
  CENTER(const int width) :
    WIDTH(width)
  {}


  /**
   * Format specifier.
   *
   * \param  out           output stream
   * \param  format        format
   * \return               output stream
   */
  friend inline JCenter operator<<(std::ostream& out, const CENTER& format)
  {
    return JCenter(out, format);
  }
};


/**
 * Auxiliary data structure for floating point format specification.
 */
struct FIXED :
  public WIDTH
{
  /**
   * Constructor.
   *
   * \param  width         width
   * \param  precision     precision
   */
  FIXED(const int width,
	const int precision) :
    WIDTH(width)
  {
    this->precision = precision;
  }


  /**
   * Format specifier.
   *
   * \param  out           output stream
   * \param  format        format
   * \return               output stream
   */
  friend inline std::ostream& operator<<(std::ostream& out, const FIXED& format)
  {
    using namespace std;
    
    return out << fixed << right << setw(format.width) << setprecision(format.precision);
  }
  
  int precision;
};


/**
 * Auxiliary data structure for floating point format specification.
 */
struct SCIENTIFIC :
  public WIDTH
{
  /**
   * Constructor.
   *
   * \param  width         width
   * \param  precision     precision
   */
  SCIENTIFIC(const int width,
	     const int precision) :
    WIDTH(width)
  {
    this->precision = precision;
  }


  /**
   * Format specifier.
   *
   * \param  out           output stream
   * \param  format        format
   * \return               output stream
   */
  friend inline std::ostream& operator<<(std::ostream& out, const SCIENTIFIC& format)
  {
    using namespace std;
    
    return out << scientific << right << setw(format.width) << setprecision(format.precision);
  }
  
  int precision;
};


/**
 * Data structure for format specifications.
 */
struct JFormat_t {

  typedef std::ios_base::fmtflags  fmtflags;

  /**
   * Default constructor.
   */
  JFormat_t() :
    width    (0),
    precision(0),
    flags    (),
    fill     (' ') 
  {}


  /**
   * Constructor.
   *
   * \param  width        width
   * \param  precision    precision
   * \param  flags        flags
   * \param  fill         fill character
   */
  JFormat_t(const int      width,
	    const int      precision  = 0,
	    const fmtflags flags      = fmtflags(),
	    const char     fill       = ' ') :
    width    (width),
    precision(precision),
    flags    (flags),
    fill     (fill)
  {}
  

  /**
   * Constructor.
   *
   * \param  out          output stream
   */
  JFormat_t(std::ostream& out)
  {
    get(out);
  }


  /**
   * Check validity of this manipulator.
   *
   * \return              true if valid; else false
   */
  inline bool is_valid() const
  {
    return (width > 0);
  }

  
  /**
   * Get format specificaton from given output stream.
   *
   * \param  out          output stream
   */
  void get(std::ostream& out)
  {
    this->width     = out.width();
    this->precision = out.precision();
    this->flags     = out.flags();
    this->fill      = out.fill();
  }


  /**
   * Put format specificaton to given output stream.
   *
   * \param  out          output stream
   */
  void put(std::ostream& out) const
  {
    out.width    (this->width);
    out.precision(this->precision);
    out.flags    (this->flags);
    out.fill     (this->fill);
  }


  /**
   * Format specifier.
   *
   * \param  out           output stream
   * \param  format        format
   * \return               output stream
   */
  friend inline std::ostream& operator<<(std::ostream& out, const JFormat_t& format)
  {
    format.put(out);
    
    return out;
  }
  
  int      width;
  int      precision;
  fmtflags flags;
  char     fill;
};


/**
 * Auxiliary class to temporarily define format specifications.
 *
 * The format specification of the output stream in use will be restored when this object is destroyed.
 */
struct JFormat :
  public JFormat_t
{
  /**
   * Constructor.
   *
   * \param  out       output stream
   */
  JFormat(std::ostream& out) :
    JFormat_t(),
    out      (out),
    format   (out)
  {}


  /**
   * Constructor.
   *
   * \param  out       output stream
   * \param  format    format
   */
  JFormat(std::ostream& out, const JFormat_t& format) :
    JFormat_t(format),
    out      (out),
    format   (out)
  {}


  /**
   * Destructor.
   */
  ~JFormat()
  {
    format.put(out);
  }

private:
  std::ostream&   out;
  const JFormat_t format;
};


/**
 * Get format for given type.
 *
 * \return           format
 */
template<class T>
inline JFormat_t& getFormat()
{
  static JFormat_t manip;

  return manip;
}


/**
 * Get format for given type.
 *
 * \param  format    default format
 * \return           actual  format
 */
template<class T>
inline JFormat_t getFormat(const JFormat_t& format)
{
  const JFormat_t& buffer = getFormat<T>();

  if (buffer.is_valid())
    return buffer;
  else
    return format;
}


/**
 * Set format for given type.
 *
 * \param  format    format
 */
template<class T>
inline void setFormat(const JFormat_t& format)
{
  getFormat<T>() = format;
}


/**
 * Auxiliary data structure to convert (lambda) function to printable object.
 *
 * The (lambda) function should conform with the type definition LAMBDA::function_type.\n
 * This data structure acts as a simple "wrapper" and should be used if the lambda has capture functionality.
 */
struct LAMBDA {
  /**
   * Type definition of print function.
   */
  typedef std::function<void(std::ostream&)>  function_type;


  /**
   * Constructor.
   *
   * \param  f1         (lambda) function
   */
  LAMBDA(const function_type& f1) :
    f1(f1)
  {}


  /**
   * Write printable object to output stream.
   *
   * \param  out        output stream
   * \param  object     printable object
   * \return            output stream
   */
  friend inline std::ostream& operator<<(std::ostream& out, const LAMBDA& object)
  {
    object.f1(out);

    return out;
  }

private:
  const function_type& f1;
};

#endif
