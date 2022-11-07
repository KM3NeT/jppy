#ifndef __JLANG__JEXCEPTION__
#define __JLANG__JEXCEPTION__

#include <exception>
#include <string>
#include <ostream>
#include <sstream>


/**
 * \file
 * Exceptions.
 * \author mdejong
 */
namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * General exception
   */
  class JException : public std::exception { 
  public:
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JException(const std::string& error) :
      std::exception(),
      buffer(error)
    {}


    /**
     * Destructor.
     */
    ~JException() throw()
    {}


    /**
     * Get error message.
     *
     * \return          error message
     */
    virtual const char* what() const throw() override 
    { 
      return buffer.c_str();
    }


    /**
     * Print error message of JException.
     *
     * \param  out        output stream
     * \param  exception  exception
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JException& exception)
    {
      return out << exception.what();
    }


    /**
     * Get output stream for conversion of exception.
     *
     * Note that the ostream is emptied before use.
     * 
     * \return           ostream
     */
    static inline std::ostream& getOstream()
    {
      static std::ostringstream buffer;

      buffer.str("");

      return buffer;
    }

  private:
    const std::string buffer;
  };


  /**
   * Exception for accessing an index in a collection that is outside of its range.
   */
  class JIndexOutOfRange : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JIndexOutOfRange(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for accessing an invalid pointer.
   */
  class JPointerException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JPointerException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for a functional operation.
   */
  class JFunctionalException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JFunctionalException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for an empty collection.
   */
  class JEmptyCollection : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JEmptyCollection(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for accessing a value in a collection that is outside of its range.
   */
  class JValueOutOfRange : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JValueOutOfRange(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for parsing value.
   */
  class JParseError : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JParseError(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for missing value.
   */
  class JNoValue : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JNoValue(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for null pointer operation.
   */
  class JNullPointerException :
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JNullPointerException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for cast operation.
   */
  class JCastException :
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JCastException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for numerical precision error.
   */
  class JNumericalPrecision : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JNumericalPrecision(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for division by zero.
   */
  class JDivisionByZero : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JDivisionByZero(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for failure of memory allocation.
   */
  class JMallocException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JMallocException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for failure of memory allocation.
   */
  class JNewException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JNewException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for I/O.
   */
  class JIOException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JIOException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for opening of file.
   */
  class JFileOpenException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JFileOpenException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for recovery of file.
   */
  class JFileRecoveryException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JFileRecoveryException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for reading of file.
   */
  class JFileReadException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JFileReadException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for end of file.
   */
  class JEndOfFile : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JEndOfFile(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for opening of pipe.
   */
  class JPipeOpenException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JPipeOpenException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for select call.
   */
  class JSelectException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JSelectException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for socket.
   */
  class JSocketException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JSocketException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for ControlHost.
   */
  class JControlHostException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JControlHostException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for socket channel.
   */
  class JSocketChannelException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JSocketChannelException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for creation of fork.
   */
  class JForkException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JForkException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for system call.
   */
  class JSystemException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JSystemException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception when parsing a value.
   */
  class JParserException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JParserException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception when parsing a value.
   */
  class JPropertiesException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JPropertiesException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for missing entry in dictionary.
   */
  class JDictionaryEntryNotFound : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JDictionaryEntryNotFound(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for duplicate entry in dictionary.
   */
  class JDictionaryDuplicateEntry : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JDictionaryDuplicateEntry(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Run time exception.
   */
  class JRunTimeException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JRunTimeException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Exception for absence of type information.
   */
  class JTypeInformationException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JTypeInformationException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Protocol exception.
   */
  class JProtocolException : 
    public JException 
  { 
  public: 
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JProtocolException(const std::string& error) :
      JException(error)
    {}
  };


  /**
   * Database exception.
   */
  class JDatabaseException :
    public JException
  {
  public:
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JDatabaseException(const std::string& error) :
      JException(error)
    {}
  };
}

/**
 * Make exception.
 *
 * \param  JException_t    exception
 * \param  A               message
 */
#define MAKE_EXCEPTION(JException_t, A) JException_t(static_cast<std::ostringstream&>(JLANG::JException::getOstream() << __FILE__ << ':' << __LINE__ << std::endl << A).str())

/**
 * Marco for throwing exception with std::ostream compatible message.
 *
 * \param  JException_t    exception
 * \param  A               message
 */
#ifndef THROW
#define THROW(JException_t, A) do { throw MAKE_EXCEPTION(JException_t, A); } while(0)
#endif

#endif
