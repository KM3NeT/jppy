#ifndef __JLANG__JABSTRACTFILE__
#define __JLANG__JABSTRACTFILE__

#include <stdio.h>

#include "JLang/JComparable.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * The JAbstractFile class encapsulates the c-style file descriptor.
   */
  class JAbstractFile :
    public JComparable<JAbstractFile>
  {
  public:

    static const int FILE_CLOSED = -1;


    /**
     * Default constructor.
     */
    JAbstractFile() :
      fileDescriptor(FILE_CLOSED)
    {}


    /**
     * Constructor.
     *
     * \param  file            file descriptor
     */
    JAbstractFile(const int file) :
      fileDescriptor(file)
    {}


    /**
     * Constructor.
     *
     * \param  stream          file stream
     */
    JAbstractFile(FILE* stream) :
      fileDescriptor(fileno(stream))
    {}


    /**
     * Less than operation.
     *
     * \param  file      JAbstractFile to be compared
     * \return           true if this file descriptor is less; else false
     */
    bool less(const JAbstractFile& file) const
    {
      return getFileDescriptor() < file.getFileDescriptor();
    }


    /**
     * Get file descriptor.
     *
     * \return                file descriptor
     */
    int getFileDescriptor() const
    {
      return fileDescriptor;
    }


    /**
     * Get open status.
     */
    bool is_open() const
    {
      return fileDescriptor != FILE_CLOSED;
    }


  protected:
    int fileDescriptor;
  };
}

#endif
