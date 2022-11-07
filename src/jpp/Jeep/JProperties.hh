#ifndef __JEEP__JPROPERTIES__
#define __JEEP__JPROPERTIES__

#include <string>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

#include "JLang/JAbstractIO.hh"
#include "JLang/JException.hh"
#include "JLang/JSharedPointer.hh"
#include "JLang/JEquationParameters.hh"
#include "JLang/JEquationFacet.hh"
#include "JLang/JEquation.hh"
#include "JLang/JComparisonAvailable.hh"
#include "JLang/JCategory.hh"
#include "JLang/JClass.hh"
#include "Jeep/JStreamToolkit.hh"
#include "Jeep/JComparisonToolkit.hh"
#include "Jeep/JMessage.hh"


/**
 * \file
 * Utility class to parse parameter values.
 * \author mdejong
 */
namespace JEEP {}
namespace JPP { using namespace JEEP; }

namespace JEEP {
  
  using JLANG::JStreamInput;
  using JLANG::JStreamOutput;
  using JLANG::JStreamSuffixOutput;
  using JLANG::JFileOpenException;
  using JLANG::JFileReadException;
  using JLANG::JSharedPointer;
  using JLANG::JEquationParameters;
  using JLANG::JEquationFacet;
  using JLANG::JEquation;
  using JLANG::JComparisonAvailable;
  using JLANG::JPropertiesException;


  /**
   * Check for stream state.
   *
   * Note that end-of-file is not defined as an error so to normally process e.g.\ std::string and std::vector.
   *
   * \param  in            input stream
   * \return               true if failure; else false
   */
  inline bool fail(std::istream& in)
  {
    return in.bad() || (in.fail() && !in.eof());
  }
  

  /**
   * Interface for I/O of properties element.
   */
  class JPropertiesElementInterface :
    public JStreamInput,
    public JStreamOutput,
    public JStreamSuffixOutput
  {
  public:

    using JStreamOutput::write;
    using JStreamSuffixOutput::write;
    
    
    /**
     * Get properties type.
     *
     * \return               false
     */
    virtual bool is_properties() const 
    { 
      return false;
    }


    /**
     * Equality between property element interfaces.
     *
     * \param  element       properties element interface
     * \return               false
     */
    virtual bool equals(const JPropertiesElementInterface& element) const 
    {
      return false;
    }
  };


  class JPropertiesElement;     // Forward declaration.


  /**
   * Template class for I/O of properties element. 
   *
   * This class implements the JPropertiesElementInterface interface.
   */
  template<class T>
  class JPropertiesTemplateElement :
    public JPropertiesElementInterface 
  {

    friend class JPropertiesElement;

  public:
    /**
     * Constructor.
     *
     * \param  value         reference of template object
     */
    JPropertiesTemplateElement(T& value) : 
      JPropertiesElementInterface(),
      object(value)
    {}

 
    /**
     * Stream input.
     *
     * \param  in            input stream
     * \return               input stream
     */
    virtual std::istream& read(std::istream& in) override 
    {
      using namespace std;

      readObject(in, object);

      string buffer;

      in >> buffer;

      if (!buffer.empty()) {
	THROW(JPropertiesException, "JProperties: pending data <" << buffer << (in.peek() != EOF ? "..." : "") << ">");
      }

      return in;
    }


    /**
     * Stream output.
     *
     * \param  out           output stream
     * \param  prefix        prefix
     * \param  postfix       postfix
     * \return               output stream
     */
    virtual std::ostream& write(std::ostream& out,
				const char*   prefix, 
				const char    postfix) const override
    { 
      return writeObject(out, prefix, object, postfix);
    }


    /**
     * Stream output.
     *
     * \param  out           output stream
     * \return               output stream
     */
    virtual std::ostream& write(std::ostream& out) const override 
    { 
      return writeObject(out, object);
    }


    /**
     * Equality between property element interfaces.
     *
     * \param  element       properties element interface
     * \return               true if equal; else false
     */
    virtual bool equals(const JPropertiesElementInterface& element) const override 
    { 
      const JPropertiesTemplateElement<T>* p = dynamic_cast<const JPropertiesTemplateElement<T>*>(&element);

      if (p != NULL)
	return compareObjects(object, p->object);
      else
	return false;
    }


  protected:
    T& object;
  };


  /**
   * Template specialisation of JPropertiesTemplateElement for const data type.
   *
   * This class implements the JPropertiesElementInterface interface.
   */
  template<class T>
  class JPropertiesTemplateElement<const T> : 
    public JPropertiesElementInterface 
  {

    friend class JPropertiesElement;

  public:
    /**
     * Constructor.
     *
     * \param  value         reference of template object
     */
    JPropertiesTemplateElement(const T& value) : 
      JPropertiesElementInterface(),
      object(value)
    {}


    /**
     * Stream input.
     *
     * \param  in            input stream
     * \return               input stream
     */
    virtual std::istream& read(std::istream& in) override 
    {
      THROW(JPropertiesException, "JPropertiesTemplateElement<>::read() reading of const data type.");
    }


    /**
     * Stream output.
     *
     * \param  out           output stream
     * \return               output stream
     */
    virtual std::ostream& write(std::ostream& out) const override 
    { 
      return writeObject(out, object);
    }


    /**
     * Stream output.
     *
     * \param  out           output stream
     * \param  prefix        prefix
     * \param  postfix       postfix
     * \return               output stream
     */
    virtual std::ostream& write(std::ostream& out,
				const char*   prefix, 
				const char    postfix) const
    { 
      return writeObject(out, prefix, object, postfix);
    }


    /**
     * Equality between property element interfaces.
     *
     * \param  element       properties element interface
     * \return               true if equal; else false
     */
    virtual bool equals(const JPropertiesElementInterface& element) const override 
    { 
      const JPropertiesTemplateElement<const T>* p = dynamic_cast<const JPropertiesTemplateElement<const T>*>(&element);

      if (p != NULL)
	return compareObjects(object, p->object);
      else
	return false;
    }


  protected:
    const T& object;
  };


  /**
   * The property value class.
   * This class consists of a pointer to the JPropertiesElementInterface.
   * This class implements the assignment and type conversion operators.
   */
  class JPropertiesElement :
    public JSharedPointer<JPropertiesElementInterface>
  {
  public:
    /**
     * Default constructor.
     */
    JPropertiesElement() :
      end_marker(false)
    {}


    /**
     * Constructor.
     *
     * \param  value         reference to template object
     */
    template<class T>
    JPropertiesElement(T& value) :
      end_marker(false)
    {
      reset(new JPropertiesTemplateElement<T>(value));
    }


    /**
     * Equality between property elements.
     *
     * \param  element       properties element
     * \return               true if equal; else false
     */
    bool equals(const JPropertiesElement& element) const 
    { 
      return get()->equals(*element.get());
    }

    
    /**
     * Assignment operator.
     *
     * \param  value         reference to template object
     * \return               this JPropertiesElement
     */
    template<class T>
    JPropertiesElement& operator=(T& value)
    {
      reset(new JPropertiesTemplateElement<T>(value));

      return *this;
    }

    
    /**
     * Get value.
     *
     * \return               value of this JPropertiesElement
     */
    template<class T>
    const T& getValue() const
    {
      const JPropertiesTemplateElement<T>* p = dynamic_cast<const JPropertiesTemplateElement<T>*>(this->get());

      if (p != NULL)
	return p->object;
      else
	THROW(JPropertiesException, "Inconsistent data type.");
    }

    
    /**
     * Get value.
     *
     * \return               value of this JPropertiesElement
     */
    template<class T>
    T& getValue()
    {
      JPropertiesTemplateElement<T>* p = dynamic_cast<JPropertiesTemplateElement<T>*>(this->get());

      if (p != NULL)
	return p->object;
      else
	THROW(JPropertiesException, "Inconsistent data type.");
    }


    
    /**
     * Set value of this JPropertiesElement.
     *
     * \param  value         value
     */
    template<class T>
    void setValue(const T& value)
    {
      JPropertiesTemplateElement<T>* p = dynamic_cast<JPropertiesTemplateElement<T>*>(this->get());

      if (p != NULL)
	p->object = value;
      else
	THROW(JPropertiesException, "Inconsistent data type.");
    }


    /**
     * Convert to string.
     *
     * \return               value of this JPropertiesElement
     */
    std::string toString() const
    {
      std::ostringstream os;
      
      get()->write(os, "", '\0');

      return os.str();
    }

    
    /**
     * Convert to template type.
     *
     * \return               value of this JPropertiesElement
     */
    template<class T>
    T toValue() const
    {
      T value;

      std::istringstream in(this->toString());

      in >> value;

      return value;
    }

    
    /**
     * Type conversion operator.
     *
     * \return               value of this JPropertiesElement
     */
    template<class T>
    operator const T&() const
    {
      return getValue<T>();
    }


    /**
     * Get end marker.
     *
     * \return               end marker
     */
    bool getEndMarker() const
    {
      return end_marker;
    }


    /**
     * Set end marker.
     *
     * \param  marker        if true stop reading after this properties element, else continue
     */
    void setEndMarker(const bool marker)
    {
      end_marker = marker;
    }


  private:
    bool end_marker;
  };


  /**
   * Utility class to parse parameter values.
   *
   * The mapping between a parameter (of any type) and a value
   * has to be defined in the user's program, e.g.
   *
   *\code{.cpp}
   
   #include "Jeep/JProperties.hh"

   ifstream in(filename.c_str());
    
   JProperties zap;
   
   int    integer_value;
   double double_value;
   string string_value;
   
   zap.insert(make_property(integer_value));   // key == parameter name
   zap.insert(make_property(double_value));    // key == parameter name
   
   zap["mies"] = string_value;
   
   zap.read(in);
   zap.write(cout);
   \endcode
   */
  class JProperties : 
    public std::map<std::string, JPropertiesElement>,
    public JEquationParameters,
    public JMessage<JProperties>
  {
  public:

    typedef std::map<std::string, JPropertiesElement>                  JMap_t;

    using JMessage<JProperties>::debug;

    
    /**
     * Utility method to strip off all leading characters from a string until specified character(s).
     *
     * \param  buffer        input string
     * \param  sep           last character(s) to strip
     * \return               modified string
     */
    static inline std::string getKey(const std::string& buffer, const std::string& sep)
    {
      using namespace std;

      const size_type pos = buffer.find_last_of(sep);

      if (pos != string::npos)
	return buffer.substr(pos + 1);
      else
	return buffer;
    }


    /**
     * Auxiliary class to handle input from file.
     *
     * The assignment of a value will cause the file with the corresponding name to be opened and read. 
     * The contents of the file are processed in the same way as any input to the associated JProperties object.
     */
    class JFileReader {
    public:
      JFileReader(JProperties& __properties) :
	properties(__properties)
      {}
  

      /**
       * Assignment operator.
       *
       * \param  file_name       file name
       * \return                 this JFileReader
       */
      JFileReader& operator=(const std::string& file_name)
      {
	read(file_name);
      
	return *this;
      }
    

      /**
       * Stream input.
       *
       * \param  in              input stream
       * \param  fileReader      file reader object
       * \return                 input stream
       */
      friend inline std::istream& operator>>(std::istream& in, JProperties::JFileReader& fileReader)
      {
	std::string file_name;
      
	in >> file_name;
      
	fileReader.read(file_name);

	return in;
      }


      /**
       * Stream output.
       *
       * \param  out             output stream
       * \param  fileReader      file reader
       * \return                 output stream
       */
      friend inline std::ostream& operator<<(std::ostream& out, const JProperties::JFileReader& fileReader)
      {
	return out;
      }


    private:
      /**
       * Read properties from file.
       *
       * \param  file_name       file name
       */
      void read(const std::string& file_name)
      {
	std::ifstream in(file_name.c_str());
      
	if (!in) {
	  THROW(JFileOpenException, "JFileReader: error opening file " << file_name);
	}

	properties.read(in);
      
	if (in.bad()) {
	  THROW(JFileReadException, "JFileReader: error reading file " << file_name);
	}

	in.close();
      }


      JProperties& properties;
    };


    /**
     * Constructor.
     *
     * \param  debug             debug level
     */
    JProperties(const int debug = 0) :
      JMap_t(),
      JEquationParameters()
    {
      this->debug = debug;
    }


    /**
     * Constructor.
     *
     * \param  parameters        equation parameters
     * \param  debug             debug level
     */
    JProperties(const JEquationParameters& parameters,
		const int debug = 0) :
      JMap_t(),
      JEquationParameters(parameters)
    {
      this->debug = debug;
    }


    /**
     * Put object at given key.
     *
     * \param  key               key
     * \param  object            object
     */
    template<class T>
    void put(const std::string& key, T& object)
    {
      this->insert(value_type(key, JPropertiesElement(object)));
    }
    

    /** 
     * Join properties objects.
     *
     * \param  properties        properties
     */
    JProperties& join(const JProperties& properties)
    { 
      JEquationParameters::join(properties);

      insert(properties.begin(), properties.end());

      return *this;
    }

    
    /**
     * Read equation.
     *
     * \param  equation          equation
     * \return                   status
     */
    bool read(const JEquation& equation)
    {
      using namespace std;

      iterator p = find(equation.getKey());
	  
      DEBUG("Processing key: " << equation.getKey() << ' ' << (p != end()) << endl);
      
      if (p != end()) {
	  
	istringstream is(equation.getValue());

	if        (isDivision(equation.getSeparator())) {
	    
	  if (p->second->is_properties()) {

	    p->second->read(is);
	      
	  } else {
	    
	    ERROR("JProperties::read(): no properties object after division <" << equation.getKey() << ">" << endl);
	  }
	  
	} else if (isSeparator(equation.getSeparator())) {
	  
	  try {
	    p->second->read(is);     
	  }
	  catch(const exception& error) {
	    ERROR("JProperties::read(): read error at key <" << equation.getKey() << "> " << error.what() << endl);
	  }
	  
	} else {
	  
	  ERROR("JProperties::read(): illegal character following key <" << equation.getKey() << "> " << equation.getSeparator() << endl);
	}
	
	if (p->second.getEndMarker()) {
	  return false;
	}
	
	if (fail(is)) {
	  
	  ERROR("JProperties::read(): error reading data for key <" << equation.getKey() << "> " << equation.getValue() << endl);
	}
	
      } else {
	
	WARNING("JProperties::read(): unknown key <" << equation.getKey() << ">" << endl);
      }

      return true;
    }


    /**
     * Read from input string.
     *
     * \param  buffer            input string
     * \return                   read status
     */
    bool read(const std::string& buffer) 
    { 
      std::istringstream in(buffer); 

      return !fail(read(in));
    }


    /**
     * Read from input stream.
     *
     * The input format is:
     * <pre>
     *	  [\<key\>\<sub\>]\<key\>\<sep\>\<value\>\<eol\>
     *	  [\<key\>\<sub\>]\<key\>\<sep\>\<value\>\<eol\>
     * </pre>
     * In this, white spaces are ignored.
     * The reading of key and value pairs is controlled by the JLANG::JEquationFacet class.
     *
     * \param  in                input stream
     * \return                   input stream
     */
    std::istream& read(std::istream& in)
    {
      using namespace std;

      in.imbue(locale(in.getloc(), new JEquationFacet(*this)));

      for (JEquation equation; in >> equation && read(equation); ) {}

      return in;
    }


    /**
     * Read from input stream according given format.
     *
     * For each key in the format specification,
     * a corresponding value will be read from the input stream.
     *
     * \param  in                input stream
     * \param  format            format
     * \return                   input stream
     */
    std::istream& read(std::istream& in, const std::string& format) 
    {
      using namespace std;
      
      istringstream is(format);

      vector<string> buffer;

      for (string key; is >> key; ) {
        buffer.push_back(key);
      }

      return read(in, buffer.begin(), buffer.end());
    }


    /**
     * Read from input stream according given format.
     *
     * For each key in the format specification,
     * a corresponding value will be read from the input stream.
     *
     * \param  in                input stream
     * \param  __begin           begin of format
     * \param  __end             end   of format
     * \return                   input stream
     */
    template<class T>
    std::istream& read(std::istream& in, T __begin, T __end)
    {
      using namespace std;
      
      for (T i = __begin; i != __end; ++i) {

	iterator p = find(*i);
	  
	if (p != end()) {

	  p->second->read(in);
	  
	} else {
	
	  WARNING("JProperties::read(): unknown key <" << *i << ">" << endl);
	}
      }

      return in;
    }

    
    /** 
     * Write the current parameter values.
     *
     * The output format is
     *
     *	  [\<key\>\<sub\>]\<key\>\<sep\>\<value\>\<eol\>
     *	  [\<key\>\<sub\>]\<key\>\<sep\>\<value\>\<eol\>
     *
     * in this, white spaces are omitted.
     *
     * \param  out               output stream
     * \return                   output stream
     */
    std::ostream& write(std::ostream& out) const
    {
      using namespace std;

      for (const_iterator i = begin(); i != end(); ++i) {

	char c = ' ';

	if (i->second->is_properties()) {
	  c = getDefaultDivision ();
	} else {
	  c = getDefaultSeparator();
	}

	i->second->write(out, (i->first + c).c_str(), getDefaultEndOfLine());
      }

      out << flush;

      return out;
    }


    /**
     * Write to output stream according given format.
     *
     * For each key in the format specification,
     * a corresponding value will be written to the output stream.
     *
     * \param  out               output stream
     * \param  format            format
     * \return                   output stream
     */
    std::ostream& write(std::ostream& out, const std::string& format) 
    {
      using namespace std;
      
      istringstream is(format);

      vector<string> buffer;

      for (string key; is >> key; ) {
        buffer.push_back(key);
      }

      return write(out, buffer.begin(), buffer.end());
    }


    /**
     * Write to output stream according given format.
     *
     * For each key in the format specification,
     * a corresponding value will be written to the output stream.
     *
     * \param  out               output stream
     * \param  __begin           begin of format
     * \param  __end             end   of format
     * \return                   output stream
     */
    template<class T>
    std::ostream& write(std::ostream& out, T __begin, T __end)
    {
      using namespace std;
      
      for (T i = __begin; i != __end; ++i) {

	iterator p = find(*i);
	  
	if (p != end()) {

	  out << getDefaultWhiteSpace();

	  p->second->write(out);
	  
	} else {
	
	  WARNING("JProperties::write(): unknown key <" << *i << ">" << endl);
	}
      }

      return out;
    }


    /**
     * Stream editing of input format.
     *
     * For each key in the format specification,
     * a corresponding value will be written to the output stream.
     *
     * \param  format            format
     * \param  prefix            prefix  key word
     * \param  postfix           postfix key word
     * \return                   output stream
     */
    std::string sed(const std::string& format,
		    const std::string& prefix  = "", 
		    const std::string& postfix = "")
    {
      using namespace std;
      
      string buffer = format;
      
      for (iterator i = begin(); i != end(); ++i) {

	string::size_type ipos = 0;
	
	while ((ipos = buffer.find(prefix + i->first + postfix, ipos)) != string::npos) {

	  ostringstream out;

	  i->second->write(out);

	  buffer.replace(ipos, prefix.length() + i->first.length() + postfix.length(), out.str());
	}
      }
      
      return buffer;
    }

    
    /**
     * Get value.
     *
     * \param  key               key
     * \return                   value of this JPropertiesElement
     */
    template<class T>
    const T& getValue(const std::string& key) const
    {
      const_iterator i = find(key);

      if (i != end())
	return i->second.getValue<T>();
      else
	THROW(JPropertiesException, "Key <" << key << "> not found at JPropertiesElement::getValue()");
    }

    
    /**
     * Get value.
     *
     * \param  key               key
     * \return                   value of this JPropertiesElement
     */
    template<class T>
    T& getValue(const std::string& key) 
    {
      iterator i = find(key);

      if (i != end())
	return i->second.getValue<T>();
      else
	THROW(JPropertiesException, "Key <" << key << "> not found at JPropertiesElement::getValue()");
    }

    
    /**
     * Set value.
     *
     * \param  key               key
     * \param  value             value
     */
    template<class T>
    void setValue(const std::string& key, const T& value)
    {
      iterator i = find(key);

      if (i != end())
	return i->second.setValue<T>(value);
      else
	THROW(JPropertiesException, "Key <" << key << "> not found at JPropertiesElement::setValue()");
    }


    /**
     * Get string value.
     *
     * \param  key               key
     * \return                   value
     */
    std::string getString(const std::string& key) const
    {
      const_iterator i = find(key);

      if (i != end())
	return i->second.toString();
      else
	THROW(JPropertiesException, "Key <" << key << "> not found at JPropertiesElement::getString()");
    }

 
    /** 
     * Print the current parameter values.
     *
     * \param  out               output stream
     * \return                   output stream
     */
    std::ostream& print(std::ostream& out) const
    {
      write(out);

      return out;
    }
 

    /**
     * Stream input.
     *
     * \param  in                input stream
     * \param  properties        properties
     * \return                   input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JProperties& properties)
    {
      return properties.read(in); 
    }
  
  
    /**
     * Stream output.
     *
     * \param  out               output stream
     * \param  properties        properties
     * \return                   output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JProperties& properties)
    { 
      return properties.write(out); 
    }
  };


  /**
   * Template specialisation for JFileReader.
   * This class will not produce ASCII output.
   */
  template<>
  class JPropertiesTemplateElement<JProperties::JFileReader> : 
    public JPropertiesElementInterface 
  {
  public:
    /**
     * Constructor.
     *
     * \param  value             reference of template bject
     */
    JPropertiesTemplateElement(JProperties::JFileReader& value) : 
      JPropertiesElementInterface(),
      object(value)
    {}

 
    /**
     * Stream input.
     *
     * \param  in                input stream
     * \return                   input stream
     */
    virtual std::istream& read(std::istream& in) override 
    {
      return in >> object;
    }


    /**
     * Stream output.
     *
     * \param  out               output stream
     * \param  prefix            prefix
     * \param  postfix           postfix
     * \return                   output stream
     */
    virtual std::ostream& write(std::ostream& out, 
				const char*   prefix, 
				const char    postfix) const override
    { 
      return out;
    }


    /**
     * Stream output.
     *
     * \param  out               output stream
     * \return                   output stream
     */
    virtual std::ostream& write(std::ostream& out) const override 
    {
      return out;
    }


    /**
     * Equality between property element interfaces.
     *
     * \param  element           properties element interface
     * \return                   true if equal; else false
     */
    virtual bool equals(const JPropertiesElementInterface& element) const override 
    { 
      return false;
    }

  private:
    JProperties::JFileReader& object;
  };


  /**
   * Template specialisation for JProperties.
   */
  template<>
  class JPropertiesTemplateElement<JProperties> : 
    public JPropertiesElementInterface 
  {
  public:
    /**
     * Constructor.
     *
     * \param  value             reference of template bject
     */
    JPropertiesTemplateElement(const JProperties& value) : 
      JPropertiesElementInterface(),
      object(value)
    {}

 
    /**
     * Stream input.
     *
     * \param  in                input stream
     * \return                   input stream
     */
    virtual std::istream& read(std::istream& in) override 
    {
      object.read(in);

      return in;
    }


    /**
     * Stream output.
     *
     * \param  out               output stream
     * \param  prefix            prefix
     * \param  postfix           postfix
     * \return                   output stream
     */
    virtual std::ostream& write(std::ostream& out, 
				const char*   prefix, 
				const char    postfix) const override
    {
      using namespace std;
      
      for (JProperties::const_iterator i = object.begin(); i != object.end(); ++i) {
	
	char c = ' ';

	if (i->second->is_properties()) {
	  c = object.getDefaultDivision ();
	} else {
	  c = object.getDefaultSeparator();
	}

	i->second->write(out, (prefix + i->first + c).c_str(), postfix);
      }
      
      out << flush;
      
      return out;
    }


    /**
     * Stream output.
     *
     * \param  out               output stream
     * \return                   output stream
     */
    virtual std::ostream& write(std::ostream& out) const override 
    {
      return writeObject(out, object);
    }


    /**
     * Get properties type.
     *
     * \return                   true
     */
    virtual bool is_properties() const override 
    {
      return true;
    }


  private:
    JProperties object;
  };


  /**
   * Get properties of a given object.
   *
   * This method transfers the making of the property object to the corresponding class 
   * whilst preserving the constness of the argument.\n
   * The corresponding class should implement the method:
   * <pre>
   *   template<bool is_constant>
   *   static JProperties getProperties(typename JCategory<T, is_constant>::reference_type object,
   *				        const JEquationParameters& equation,
   *				        const int debug)
   * </pre>
   *
   * \param  object            object
   * \param  parameters        equation parameters
   * \param  debug             debug level
   */
  template<class T>
  inline JProperties& getProperties(T& object,
				    const JEquationParameters& parameters = JEquationParameters(),
				    const int debug = 1)
  {
    using namespace JPP;

    static JProperties properties;

    properties = T::template getProperties<JClass<T>::is_constant>(object, parameters, debug);
    
    return properties; 
  }
}


using JLANG::JEquationParameters;
using JLANG::JCategory;
using JEEP::JProperties;
using JEEP::getProperties;


/**
 * macro to convert (template) parameter to JPropertiesElement object
 */
#define gmake_property(A) JProperties::value_type(JProperties::getKey(#A,".>/:"), JEEP::JPropertiesElement(A))


#endif
