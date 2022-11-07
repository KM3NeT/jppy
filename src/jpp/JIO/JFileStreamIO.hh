#ifndef __JIO__JFILESTREAMIO__
#define __JIO__JFILESTREAMIO__

#include <fstream>

#include "JIO/JStreamIO.hh"
#include "JIO/JBufferedIO.hh"


/**
 * \author mdejong
 */

namespace JIO {}
namespace JPP { using namespace JIO; }

namespace JIO {

  /**
   * Binary buffered file input.
   */
  class JFileStreamReader :
    public std::ifstream,
    public JBufferedReader
  {
  public:

    using JAbstractObjectStatus::operator bool;
    using JAbstractObjectStatus::operator!;
    using JReader::operator>>;


    /**
     * Default constructor.
     */
    JFileStreamReader() :
      std::ifstream(),
      JBufferedReader(new JStreamReader(static_cast<std::ifstream&>(*this)))
    {}


    /**
     * Constructor.
     *
     * \param file_name       file name
     * \param size            size of internal buffer
     */
    JFileStreamReader(const char* file_name,
		      const int   size = 1048576) :
      std::ifstream  (),
      JBufferedReader(new JStreamReader(static_cast<std::ifstream&>(*this)), size)
    {
      open(file_name);
    }


    /**
     * Open file.
     *
     * \param file_name       file name
     */
    void open(const char* file_name)
    {
      static_cast<std::ifstream*>(this)->open(file_name, std::ios::binary);
    }


    /**
     * Clear status of reader.
     */
    virtual void clear() override
    {
      std::ifstream  ::clear();
      JBufferedReader::clear();
    }


    /**
     * Rewind.
     */
    void rewind()
    {
      seekg(0);   // rewind file stream

      reset();    // reset buffer
    }
  };


  /**
   * Binary buffered file output.
   */
  class JFileStreamWriter :
    public std::ofstream,
    public JBufferedWriter
  {
  public:

    using JAbstractObjectStatus::operator bool;
    using JAbstractObjectStatus::operator!;
    using JWriter::operator<<;


    /**
     * Default constructor.
     */
    JFileStreamWriter() :
      std::ofstream(),
      JBufferedWriter(new JStreamWriter(static_cast<std::ofstream&>(*this)))
    {}


    /**
     * Constructor.
     *
     * \param file_name       file name
     * \param size            size of internal buffer
     */
    JFileStreamWriter(const char* file_name,
		      const int   size = 1048576) :
      std::ofstream(),
      JBufferedWriter(new JStreamWriter(static_cast<std::ofstream&>(*this)), size)
    {
      open(file_name);
    }


    /**
     * Open file.
     *
     * \param file_name       file name
     */
    void open(const char* file_name)
    {
      static_cast<std::ofstream*>(this)->open(file_name, std::ios::binary);
    }


    /**
     * Close file.
     */
    void close()
    {
      static_cast<JBufferedWriter*>(this)->flush();
      static_cast<std::ofstream*>  (this)->close();
    }
  };
}

#endif
