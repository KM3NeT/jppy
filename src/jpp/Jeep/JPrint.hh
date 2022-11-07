#ifndef __JEEP__JPRINT__
#define __JEEP__JPRINT__

#include <string>
#include <ostream>
#include <sstream>
#include <iomanip>

#include "JLang/JManip.hh"
#include "Jeep/JStreamToolkit.hh"


/**
 * \file
 * I/O formatting auxiliaries.
 * \author mdejong
 */
namespace JEEP {}
namespace JPP { using namespace JEEP; }


namespace JEEP {

  /**
   * Get output stream for conversion to std::string.
   *
   * Note that the stream is emptied before use.
   * 
   * \return           output stream
   */
  inline std::ostream& getOstream()
  {
    static std::ostringstream buffer;

    buffer.str("");

    return buffer;
  }


  /**
   * Get output C-string.
   *
   * Note that this method is needed to guarentee livetime of underlying std::string.
   *
   * \param  input     input
   * \return           C-string
   */
  inline const char* getCString(const std::string& input)
  {
    static std::string buffer;

    buffer = input;

    return buffer.c_str();
  }
}

/**
 * Auxiliary data structure for streaming of STL containers.
 *
 * This manipulator transfers the following stream operation to method JEEP::writeObject
 * so that all STL containers can directly be printed.
 */
struct JEEPZ
{
protected:
  /**
   * Auxiliary class for format STL containers.
   */
  struct JStream
  {
    /**
     * Constructor.
     *
     * \param  out         output stream
     */
    JStream(std::ostream& out) :
      out(out)
    {}


    /**
     * Write value to output stream.
     *
     * \param  value       value
     * \return             this JSTDStreamer
     */
    template<class T>
    std::ostream& operator<<(const T& value)
    {
      return JEEP::writeObject(out, value);
    }

  private:
    std::ostream& out;
  };

public:
  /**
   * Default constructor.
   */
  JEEPZ()
  {}


  /**
   * Format specifier.
   *
   * \param  out           output stream
   * \param  format        format
   * \return               output stream
   */
  friend inline JStream operator<<(std::ostream& out, const JEEPZ& format)
  {
    return JStream(out);
  }
};


/**
 * Make string.
 *
 * \param  A           std::ostream compatible construct
 * \return             std::string
 */
#define MAKE_STRING(A) (static_cast<std::ostringstream&>(JEEP::getOstream() << A << std::flush)).str()


/**
 * Make C-string.
 *
 * \param  A           std::ostream compatible construct
 * \return             C-string
 */
#define MAKE_CSTRING(A) JEEP::getCString(MAKE_STRING(A))

#endif
