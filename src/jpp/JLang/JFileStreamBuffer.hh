#ifndef __JLANG__JFILESTREAMBUFFER__
#define __JLANG__JFILESTREAMBUFFER__

#include <streambuf>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "JLang/JFile.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Input file stream buffer.
   */
  class JFileInputStreamBuffer : 
    protected virtual std::vector<char>,
    public    virtual std::streambuf
  {   
  public:


    typedef traits_type::int_type        int_type;
    

    /**
     * Constructor.
     *
     * \param  file            file
     * \param  size            size of internal buffer
     * \param  put_back        number of put back characters
     */
    JFileInputStreamBuffer(const JAbstractFile& file,
			   const std::size_t    size = 65536,
			   const std::size_t    put_back = 8) :
      in    (file),
      memory(put_back)
    {
      resize(size + put_back),

      setg(this->data(), this->data(), this->data());
    }

    
    /**
     * Check underflow.
     * This method reads as many bytes as possible.
     *
     * \return                 first character if OK; else EOF
     */
    virtual int_type underflow() override 
    {   
      if (gptr() >= egptr()) {

	char* __begin = this->data();

	if (eback() == __begin) { 
	  
	  // push put back characters

	  memmove(__begin, egptr() - memory, memory);
	
	  __begin += memory;
	}
	
	const size_t len = in.read(__begin, this->size() - (__begin - this->data()));
	
	if (len != 0)
	  setg(this->data(), __begin, __begin + len);
	else
	  return traits_type::eof();
      }
	
      return *gptr();
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
      return JFileDescriptorMask(in).in_avail(timeout);
    }

  protected:
    JFile       in;
    std::size_t memory;

  private:
    JFileInputStreamBuffer(const JFileInputStreamBuffer&);
    JFileInputStreamBuffer(JFileInputStreamBuffer&&);
    JFileInputStreamBuffer& operator=(const JFileInputStreamBuffer&);
    JFileInputStreamBuffer& operator=(JFileInputStreamBuffer&&);
  };  

  
  /**
   * Output file stream buffer.
   */
  class JFileOutputStreamBuffer : 
    protected virtual std::vector<char>,
    public    virtual std::streambuf
  {  
  public:


    typedef traits_type::int_type        int_type;
    typedef std::streamsize              streamsize;


    /**
     * Constructor.
     *
     * \param  file            file
     * \param  size            size of internal buffer
     */
    JFileOutputStreamBuffer(const JAbstractFile& file,
			    const std::size_t    size = 65536) :
      out    (file)
    {
      // reserve one byte for overflow character

      resize(size);

      setp(this->data(), this->data() + this->size() - 1);
    }


    /**
     * Destructor.
     */
    virtual ~JFileOutputStreamBuffer()
    {
      sync();
    }


    /**
     * Check overflow.
     * This method writes one byte if possible.
     *
     * \param  c               character
     * \return                 c if OK; else EOF
     */
    virtual int_type overflow(int_type c) override 
    {
      if (c != traits_type::eof()) {

	*pptr() = (char) c;

	pbump(1);

	if (flush() == traits_type::eof()) {
	  return traits_type::eof();
	}
      }

      return c;
    }


    /**
     * Synchronise buffer.
     */
    virtual int sync() override 
    {
      if (flush() == traits_type::eof())
	return -1;
      else
	return  0;
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
      return JFileDescriptorMask(out).out_avail(timeout);
    }

  protected:
    /**
     * Flush internal buffer.
     */
    int flush()
    {
      const int len = pptr() - pbase();
      
      if (len != 0) {

	if (out.write(this->data(), len) != len) {
	  return traits_type::eof();
	}

	pbump(-len);
      }

      return len;
    }


    JFile       out;

  private:
    JFileOutputStreamBuffer(const JFileOutputStreamBuffer&);
    JFileOutputStreamBuffer(JFileOutputStreamBuffer&&);
    JFileOutputStreamBuffer& operator=(const JFileOutputStreamBuffer&);
    JFileOutputStreamBuffer& operator=(JFileOutputStreamBuffer&&);
  };  



  /**
   * Input and output file stream buffer.
   */
  class JFileStreamBuffer : 
    public JFileInputStreamBuffer,
    public JFileOutputStreamBuffer
  {
  public:
    /**
     * Constructor.
     *
     * \param  in              input  file
     * \param  out             output file
     * \param  size            size of internal buffer
     * \param  put_back        number of put back characters
     */
    JFileStreamBuffer(const JAbstractFile& in,
		      const JAbstractFile& out,
		      const std::size_t    size = 65536,
		      const std::size_t    put_back = 8) :
      JFileInputStreamBuffer (in,  size, put_back),
      JFileOutputStreamBuffer(out, size)
    {}
  };
}

#endif
