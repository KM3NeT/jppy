#ifndef __JLANG__JBINARYIO__
#define __JLANG__JBINARYIO__


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Interface for binary input.
   */
  class JBinaryInput {
  public:
    /**
     * Virtual destructor.
     */
    virtual ~JBinaryInput()
    {}


    /**
     * Read byte array.
     *
     * \param  buffer  pointer to byte array
     * \param  length  number of bytes
     * \return         number of bytes read
     */
    virtual int read(char* buffer, const int length) = 0;
  };


  /**
   * Interface for binary output.
   */
  class JBinaryOutput {
  public:
    /**
     * Virtual destructor.
     */
    virtual ~JBinaryOutput()
    {}


    /**
     * Write byte array.
     *
     * \param  buffer  pointer to byte array
     * \param  length  number of bytes
     * \return         number of bytes written
     */
    virtual int write(const char* buffer, const int length) = 0;
  };
}

#endif
