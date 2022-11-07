#ifndef __JLANG__JVALUE__
#define __JLANG__JVALUE__

#include <istream>
#include <ostream>
#include <sstream>
#include <string>

#include "JLang/JAbstractIO.hh"
#include "JLang/JEquationFacet.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Forward declaration for friend declaration of JValueOutput inside JValueInput.
   */
  template<class T>
  class JValueOutput;


  /**
   * Wrapper class around template object.
   * This class implements the JStreamInput interface.
   * Note that this class can be used in conjuction with the JEquationFacet.
   */
  template<class T>
  class JValueInput :
    public JStreamInput
  {
  public:


    friend class JValueOutput<T>;


    /**
     * Constructor.
     *
     * \param  object  input object
     */
    JValueInput(T& object) :
      p(&object)
    {}


    /**
     * Constructor.
     *
     * \param  ps      pointer to valid object
     */
    JValueInput(void* ps) :
      p((T*) ps)
    {}


    operator const T&() const { return *p; }   //!< type conversion operator
    operator       T&()       { return *p; }   //!< type conversion operator

    
    /**
     * Stream input.
     *
     * \param  in      input stream
     * \return         input stream
     */
    virtual std::istream& read(std::istream& in) override 
    {
      return in >> *p;
    }


  protected:
    T* p;
  };


  /**
   * Wrapper class around template object.
   * This class implements the JStreamOutput interface.
   * Note that this class can be used in conjuction with the JEquationFacet.
   */
  template<class T>
  class JValueOutput :
    public JStreamOutput
  {
  public:
    /**
     * Constructor.
     *
     * \param  object  input object
     */
    JValueOutput(const T& object) :
      p(&object)
    {}


    /**
     * Constructor.
     *
     * \param  object  input object
     */
    JValueOutput(const JValueInput<T>& object) :
      p(object.p)
    {}


    /**
     * Constructor.
     *
     * \param  ps      pointer to valid object
     */
    JValueOutput(const void* ps) :
      p((const T*) ps)
    {}


    operator const T&() const { return *p; }   //!< type conversion operator


    /**
     * Stream output.
     *
     * \param  out     output stream
     * \return         output stream
     */
    virtual std::ostream& write(std::ostream& out) const override 
    {
      return out << *p;
    }


  protected:
    const T* p;
  };


  /**
   * Wrapper class around template object.
   * This class implements the JStreamInput and JStreamOutput interfaces.
   */
  template<class T>
  class JValue :
    public JValueInput <T>,
    public JValueOutput<T>
  {
  public:
    /**
     * Constructor.
     *
     * \param  object  input object
     */
    JValue(T& object) :
      JValueInput <T>(&object),
      JValueOutput<T>(&object)
    {}


    /**
     * Constructor.
     *
     * \param  ps      pointer to valid object
     */
    JValue(void* ps) :
      JValueInput <T>(ps) ,
      JValueOutput<T>(ps)
    {}
  };


  /**
   * Read JStreamInput from input stream.
   *
   * \param  in        input stream
   * \param  object    JStreamInput
   * \return           input stream
   */
  template<class T>
  inline std::istream& operator>>(std::istream& in, JValueInput<T>& object)
  {
    using namespace std;

    istream::sentry sentry(in);   // skips white spaces

    if (sentry) {

      const locale& loc = in.getloc();

      if (has_facet<JEquationFacet>(loc)) {

	string buffer;

	const JEquationFacet& facet = use_facet<JEquationFacet>(loc);

	facet.getline(in, buffer);

	istringstream is(buffer);

	is.imbue(locale(in.getloc(), facet.clone()));

	object.read(is);

	if (is.fail()) {
	  in.setstate(ios_base::badbit);
	}

      } else {

	object.read(in);
      }
    }

    return in;
  }


  /**
   * Write JStreamOutput to output stream.
   *
   * \param  out       output stream
   * \param  object    JStreamOutput
   * \return           output stream
   */
  template<class T>
  inline std::ostream& operator<<(std::ostream& out, const JValueOutput<T>& object)
  {
    return object.write(out);
  }
}

#endif
