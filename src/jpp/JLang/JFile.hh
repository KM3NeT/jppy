#ifndef __JLANG__JFILE__
#define __JLANG__JFILE__

#include <fcntl.h>

#include "JLang/JBinaryIO.hh"
#include "JLang/JAbstractFile.hh"
#include "JLang/JFileDescriptorMask.hh"
#include "JLang/JTimeval.hh"
#include <unistd.h>

/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * The JFile class extends the JAbstractFile class.
   * This class implements the JBinaryInput and JBinaryOutput interfaces.
   */
  class JFile :
    public JAbstractFile,
    public JBinaryInput,
    public JBinaryOutput
  {
  public:
    /**
     * Default constructor.
     */
    JFile() :
      JAbstractFile(),
      result(0)
    {}


    /**
     * Constructor.
     *
     * \param  file            file descriptor
     */
    JFile(const JAbstractFile& file) :
      JAbstractFile(file),
      result(1)
    {}


    /**
     * Close file.
     */
    void close()
    {
      if (fileDescriptor != FILE_CLOSED) {

        ::close(fileDescriptor);

        fileDescriptor = FILE_CLOSED;
	result         = 0;
      }
    }


    /**
     * Read data from file.
     *
     * \param  buffer       buffer
     * \param  length       number of bytes to read
     * \return              number of bytes read
     */
    virtual int read(char* buffer, const int length)
    {
      return (result = ::read(fileDescriptor, buffer, length));
    }
    

    /**
     * Write data to file.
     *
     * \param  buffer       buffer
     * \param  length       number of bytes to write
     * \return              number of bytes written
     */
    virtual int write(const char* buffer, const int length)
    {
      return (result = ::write(fileDescriptor, buffer, length));
    }


    /**
     * Check availability of input.
     * This method returns true if at least one byte can be read without blocking.
     *
     * \param  timeout      timeout
     * \return              true if ready to read; else false
     */
    bool in_avail(JTimeval timeout = JTimeval::min()) const
    {
      return JFileDescriptorMask(*this).in_avail(timeout);
    }


    /**
     * Check availability of output.
     * This method returns true if at least one byte can be written without blocking.
     *
     * \param  timeout      timeout
     * \return              true if ready to write; else false
     */
    bool out_avail(JTimeval timeout = JTimeval::min()) const
    {
      return JFileDescriptorMask(*this).out_avail(timeout);
    }


    /**
     * Check status.
     *
     * \return              true if last I/O operation successful; else false
     */
    virtual bool good() const
    {
      return is_open() && !eof() && !bad();
    }


    /**
     * Check status.
     *
     * \return              true if last I/O operation caused logical error; else false
     */
    virtual bool fail() const
    {
      return result == 0;
    }


    /**
     * Check status.
     *
     * \return              true if last I/O operation caused read/write error; else false
     */
    virtual bool bad() const
    {
      return fail();
    }


    /**
     * Check end of file.
     *
     * \return              true if end of file; else false
     */
    virtual bool eof() const
    {
      return result == EOF;
    }


  private:
    int result;
  };
}

#endif
