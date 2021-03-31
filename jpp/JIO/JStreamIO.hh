#ifndef __JIO__JSTREAMIO__
#define __JIO__JSTREAMIO__

#include <istream>
#include <ostream>

#include "JIO/JSerialisable.hh"


/**
 * \author mdejong
 */

namespace JIO {}
namespace JPP { using namespace JIO; }

namespace JIO {


  /**
   * Binary input based on std::istream.
   * This class implements the JReader interface.
   */
  class JStreamReader : 
    public JReader 
  {
  public:
    /**
     * Constructor.
     *
     * \param  __in    input stream
     */
    JStreamReader(std::istream& __in) :
      in(__in)
    {}


    /**
     * Status of reader.
     *
     * \return         status of this reader
     */
    virtual bool getStatus() const override 
    { 
      return (bool) in; 
    }


    /**
     * Clear status of reader.
     */
    virtual void clear() override 
    {
      in.clear();
    }


    /**
     * Read byte array.
     *
     * \param  buffer  pointer to byte array
     * \param  length  number of bytes
     * \return         number of bytes read
     */
    virtual int read(char* buffer, const int length) override 
    { 
      in.read(buffer, length); 

      return in.gcount();
    }

  protected:
    std::istream& in;
  };


  /**
   * Binary output based on std::ostream.
   * This class implements the JWriter interface.
   */
  class JStreamWriter : 
    public JWriter
  {
  public:
    /**
     * Constructor.
     *
     * \param  __out   output stream
     */
    JStreamWriter(std::ostream& __out) :
      out(__out)
    {}


    /**
     * Status of writer.
     *
     * \return         status of this writer
     */
    virtual bool getStatus() const override 
    { 
      return (bool) out; 
    }


    /**
     * Write byte array.
     *
     * \param  buffer  pointer to byte array
     * \param  length  number of bytes
     * \return         number of bytes written
     */
    virtual int write(const char* buffer, const int length) override 
    { 
      out.write(buffer, length); 

      return length;
    }

  protected:
    std::ostream& out;
  };
}

#endif
