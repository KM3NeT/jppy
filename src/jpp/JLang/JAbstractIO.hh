#ifndef __JLANG__JABSTRACTIO__
#define __JLANG__JABSTRACTIO__

#include <istream>
#include <ostream>


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Interface for ASCII input using standard streams.
   */
  class JStreamInput {
  public:
    /**
     * Virtual destructor.
     */
    virtual ~JStreamInput()
    {}


    /**
     * Stream input.
     *
     * \param  in      input stream
     * \return         input stream
     */
    virtual std::istream& read(std::istream& in) = 0;


    /**
     * Read object from input.
     *
     * \param  in       input stream
     * \param  object   object
     * \return          input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JStreamInput& object)
    {
      return object.read(in);
    }
  };


  /**
   * Interface for ASCII output using standard streams.
   */
  class JStreamOutput {
  public:
    /**
     * Virtual destructor.
     */
    virtual ~JStreamOutput()
    {}


    /**
     * Stream output.
     *
     * \param  out     output stream
     * \return         output stream
     */
    virtual std::ostream& write(std::ostream& out) const = 0;


    /**
     * Write object to output.
     *
     * \param  out      output stream
     * \param  object   object
     * \return          output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JStreamOutput& object)
    {
      return object.write(out);
    }
  };


  /**
   * Interface for ASCII output with prefix and postfix using standard streams.
   */
  class JStreamSuffixOutput {
  public:
    /**
     * Virtual destructor.
     */
    virtual ~JStreamSuffixOutput()
    {}


    /**
     * Stream output.
     *
     * \param  out           output stream
     * \param  prefix        prefix
     * \param  postfix       postfix
     * \return               output stream
     */
    virtual std::ostream& write(std::ostream& out,
				const char*   prefix, 
				const char    postfix) const = 0;
  };
}

#endif
