#ifndef __JLANG__JFILESTREAM__
#define __JLANG__JFILESTREAM__

#include <istream>
#include <ostream>

#include "JLang/JFile.hh"
#include "JLang/JFileStreamBuffer.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Streaming of input.
   */
  class JFileInputStream : 
    protected JFileInputStreamBuffer,
    public    std::istream
  {   
  public:
    /**
     * Constructor.
     *
     * \param  file            file
     * \param  size            size of internal buffer
     */
    JFileInputStream(const JAbstractFile& file,
		     const std::size_t    size = 65536) :
      JFileInputStreamBuffer(file, size),
      std::istream(this)
    {}
  };  

  
  /**
   * Streaming of output.
   */
  class JFileOutputStream : 
    protected JFileOutputStreamBuffer, 
    public    std::ostream
  {  
  public:
    /**
     * Constructor.
     *
     * \param  file            file
     * \param  size            size of internal buffer
     */
    JFileOutputStream(const JAbstractFile& file,
		      const std::size_t    size = 65536) :
      JFileOutputStreamBuffer(file, size),
      std::ostream(this)
    {}
  };  


  /**
   * Streaming of input and output.
   */
  class JFileStream : 
    protected JFileStreamBuffer,
    public    std::iostream
  {   
  public:
    /**
     * Constructor.
     *
     * \param  in              input  file
     * \param  out             output file
     * \param  size            size of internal buffer
     */
    JFileStream(const JAbstractFile& in,
		const JAbstractFile& out,
		const std::size_t    size = 65536) :
      JFileStreamBuffer (in, out, size),
      std::iostream(this)
    {}
  };  
}

#endif
