#ifndef __JIO__JBUFFEREDIO__
#define __JIO__JBUFFEREDIO__

#include <algorithm>
#include <cstring>

#include "JLang/JSinglePointer.hh"
#include "JIO/JSerialisable.hh"


/**
 * \author mdejong
 */

namespace JIO {}
namespace JPP { using namespace JIO; }

namespace JIO {


  /**
   * Buffered binary input.
   * This class implements the JReader interface.
   */
  class JBufferedReader : 
    public JReader 
  {
  public:
    /**
     * Constructor.
     * Note that this object owns the reader pointed to.
     *
     * \param  __in    pointer to reader
     * \param  __size  size of internal buffer
     */
    JBufferedReader(JReader* __in, const int __size = 1048576) :
      in(__in)
    {
      size   = std::max(__size, 1024);
      buffer = new char[size];

      reset();
    }


    /**
     * Destructor.
     */
    ~JBufferedReader()
    {
      delete [] buffer;
    }


    /**
     * Status of reader.
     *
     * \return         status of this reader
     */
    virtual bool getStatus() const override 
    { 
      return in->getStatus() || !eof;
    }


    /**
     * Clear status of reader.
     */
    virtual void clear() override 
    {
      in->clear();
    }


    /**
     * Read byte array.
     *
     * \param  zbuf    pointer to byte array
     * \param  n       number of bytes
     * \return         number of bytes
     */
    virtual int read(char* zbuf, int n) override 
    { 
      for (int i = 0; i != n; ) {

	int m = n - i;

	if (m > ls - pos) {
	  
	  memmove(buffer, buffer + pos, ls - pos);
	  
	  ls  -= pos;
	  pos  = 0;
	  ls  += in->read(buffer + ls, size - ls); 

	  if (m > ls) {

	    if (ls == 0) {

	      eof = true;

	      return n - i;
	    }

	    m = ls;
	  }
	}

	memcpy(zbuf + i, buffer + pos, m);
	
	i   += m;
	pos += m;
      }

      eof = false;

      return n;
    }

  protected:
    /**
     * Reset.
     */
    void reset()
    {
      pos = 0;
      ls  = 0;
      eof = true;
    }

    JLANG::JSinglePointer<JReader> in;

    char* buffer;  //!< internal buffer
    int   size;    //!< size of internal buffer
    int   pos;     //!< pointer to begin of available data
    int   ls;      //!< pointer to end   of available data
    bool  eof;     //!< end of file

  private:
    JBufferedReader(const JBufferedReader&);
    JBufferedReader(JBufferedReader&&);
    JBufferedReader& operator=(const JBufferedReader&);
    JBufferedReader& operator=(JBufferedReader&&);
  };


  /**
   * Buffered binary output.
   * This class implements the JWriter interface.
   */
  class JBufferedWriter : 
    public JWriter 
  {
  public:
    /**
     * Constructor.
     * Note that this object owns the writer pointed to.
     *
     * \param  __out   pointer to writer
     * \param  __size  size of internal buffer
     */
    JBufferedWriter(JWriter* __out, const int __size = 1048576) :
      out(__out)
    {
      size   = std::max(__size, 1024);
      buffer = new char[size];

      pos = 0;
    }


    /**
     * Destructor.
     */
    ~JBufferedWriter()
    {
      flush();

      delete [] buffer;
    }


    /**
     * Status of writer.
     *
     * \return         status of this writer
     */
    virtual bool getStatus() const override 
    { 
      return (bool) *out;
    }


    /**
     * Write byte array.
     *
     * \param  zbuf    pointer to byte array
     * \param  n       number of bytes
     * \return         number of bytes
     */
    virtual int write(const char* zbuf, int n) override 
    { 
      for (int i = 0; i != n; ) {

	int m = n - i;

	if (m > size - pos) {

	  flush();
	
	  if (m > size - pos) {
	    m = size - pos;
	  }
	}

	memcpy(buffer + pos, zbuf + i, m);
	
	i   += m;
	pos += m;
      }

      return n;
    }

    
    /**
     * Write internal data to output.
     */
    void flush()
    {
      pos -= out->write(buffer, pos); 
    }


  protected:
    JLANG::JSinglePointer<JWriter> out;

    char* buffer;  //!< internal buffer
    int   size;    //!< size of internal buffer
    int   pos;     //!< pointer to end of buffered data

  private:
    JBufferedWriter(const JBufferedWriter&);
    JBufferedWriter(JBufferedWriter&&);
    JBufferedWriter& operator=(const JBufferedWriter&);
    JBufferedWriter& operator=(JBufferedWriter&&);
  };
}

#endif
