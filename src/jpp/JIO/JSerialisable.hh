#ifndef __JIO__JSERIALISABLE__
#define __JIO__JSERIALISABLE__

#include "JLang/JBinaryIO.hh"
#include "JLang/JObjectID.hh"
#include "JLang/JAbstractObjectStatus.hh"


/**
 * \author mdejong
 */

/**
 * Auxiliary classes and methods for binary I/O.
 */
namespace JIO {}
namespace JPP { using namespace JIO; }

namespace JIO {

  using JLANG::JBinaryInput;
  using JLANG::JBinaryOutput;
  using JLANG::JAbstractObjectStatus;

  class JReader;      //!< Forward declaration of binary input.
  class JWriter;      //!< Forward declaration of binary output.


  /**
   * Interface class for a data structure with binary I/O.
   */
  class JSerialisable {
  public:
    /**
     * Virtual destructor.
     */
    virtual ~JSerialisable()
    {}


    /**
     * Read from input.
     *
     * \param  in    JReader
     * \return       JReader
     */
    virtual JReader& read(JReader& in) = 0;


    /**
     * Write to output.
     *
     * \param  out   JWriter
     * \return       JWriter
     */
    virtual JWriter& write(JWriter& out) const = 0;
  };


  /**
   * Interface for binary input.
   */
  class JReader :
    public JLANG::JBinaryInput,
    public JLANG::JAbstractObjectStatus
  {
  public:
    /**
     * Clear status of reader.
     */
    virtual void clear()
    {}


    /**
     * Read serialisable data object.
     *
     * \param  object  serialisable data object
     * \return         JReader
     */
    JReader& operator>>(JSerialisable& object) 
    { 
      return object.read(*this); 
    }


    JReader& operator>>(bool&                   value) { read((char*) &value, sizeof(bool));                   return *this; }
    JReader& operator>>(char&                   value) { read((char*) &value, sizeof(char));                   return *this; }
    JReader& operator>>(unsigned char&          value) { read((char*) &value, sizeof(unsigned char));          return *this; }
    JReader& operator>>(short&                  value) { read((char*) &value, sizeof(short));                  return *this; }
    JReader& operator>>(unsigned short&         value) { read((char*) &value, sizeof(unsigned short));         return *this; }
    JReader& operator>>(int&                    value) { read((char*) &value, sizeof(int));                    return *this; }
    JReader& operator>>(unsigned int&           value) { read((char*) &value, sizeof(unsigned int));           return *this; }
    JReader& operator>>(long int&               value) { read((char*) &value, sizeof(long int));               return *this; }
    JReader& operator>>(unsigned long int&      value) { read((char*) &value, sizeof(unsigned long int));      return *this; }
    JReader& operator>>(long long int&          value) { read((char*) &value, sizeof(long long int));          return *this; }
    JReader& operator>>(unsigned long long int& value) { read((char*) &value, sizeof(unsigned long long int)); return *this; }
    JReader& operator>>(float&                  value) { read((char*) &value, sizeof(float));                  return *this; }
    JReader& operator>>(double&                 value) { read((char*) &value, sizeof(double));                 return *this; }
    JReader& operator>>(long double&            value) { read((char*) &value, sizeof(long double));            return *this; }
    JReader& operator>>(JLANG::JObjectID&       value) { return (*this) >> value.getID(); }


    /**
     * Read object.
     *
     * \param  object     object
     * \return            this reader
     */
    inline JReader& load(JSerialisable& object)
    {
      return object.read(*this);
    }


    /**
     * Read object.
     *
     * \param  object     object
     * \return            this reader
     */
    template<class T>
    inline JReader& load(T& object)
    {
      return *this >> object;
    }
  };  


  /**
   * Interface for binary output.
   */
  class JWriter :
    public JLANG::JBinaryOutput,
    public JLANG::JAbstractObjectStatus
  {
  public:
    /**
     * Write serialisable data object.
     *
     * \param  object  serialisable data object
     * \return         JWriter
     */
    JWriter& operator<<(const JSerialisable& object) 
    { 
      return object.write(*this); 
    }


    JWriter& operator<<(const bool                   value) { write((const char*) &value, sizeof(bool));                   return *this; }
    JWriter& operator<<(const char                   value) { write((const char*) &value, sizeof(char));                   return *this; } 
    JWriter& operator<<(const unsigned char          value) { write((const char*) &value, sizeof(unsigned char));          return *this; }
    JWriter& operator<<(const short                  value) { write((const char*) &value, sizeof(short));                  return *this; }
    JWriter& operator<<(const unsigned short         value) { write((const char*) &value, sizeof(unsigned short));         return *this; }
    JWriter& operator<<(const int                    value) { write((const char*) &value, sizeof(int));                    return *this; }
    JWriter& operator<<(const unsigned int           value) { write((const char*) &value, sizeof(unsigned int));           return *this; }
    JWriter& operator<<(const long int               value) { write((const char*) &value, sizeof(long int));               return *this; }
    JWriter& operator<<(const unsigned long int      value) { write((const char*) &value, sizeof(unsigned long int));      return *this; }
    JWriter& operator<<(const long long int          value) { write((const char*) &value, sizeof(long long int));          return *this; }
    JWriter& operator<<(const unsigned long long int value) { write((const char*) &value, sizeof(unsigned long long int)); return *this; }
    JWriter& operator<<(const float                  value) { write((const char*) &value, sizeof(float));                  return *this; }
    JWriter& operator<<(const double                 value) { write((const char*) &value, sizeof(double));                 return *this; }
    JWriter& operator<<(const long double            value) { write((const char*) &value, sizeof(long double));            return *this; }
    JWriter& operator<<(const JLANG::JObjectID&      value) { return (*this) << value.getID(); }


    /**
     * Write object.
     *
     * \param  object     object
     * \return            this writer
     */
    inline JWriter& store(const JSerialisable& object)
    {
      return object.write(*this);
    }


    /**
     * Write object.
     *
     * \param  object     object
     * \return            this writer
     */
    template<class T>
    inline JWriter& store(const T& object)
    {
      return *this << object;
    }
  };
}

#endif
