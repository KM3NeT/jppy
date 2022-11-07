#ifndef __JLANG__JSTRINGSTREAM__
#define __JLANG__JSTRINGSTREAM__

#include <sstream>
#include <fstream>

#include "JLang/JStreamState.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * Wrapper class around STL stringstream class to facilitate optional loading of data from file.
   */
  class JStringStream :
    public std::stringstream,
    public JStreamState
  {
  public:
    /**
     * Constructor.
     *
     * This constructor loads all data from given input stream.
     *
     * \param  in       input stream
     */
    JStringStream(std::istream& in) :
      std::stringstream(),
      JStreamState(in, static_cast<std::istream&>(*this))
    {
      using namespace std;

      istream::sentry sentry(in);   // skips white spaces

      if (sentry) {
	(*this) << in.rdbuf();
      }
    }

    
    /**
     * Load data from file with name corresponding to current contents.
     *
     * Note that if the file exists but is empty,
     * the state of the stream is set to fail by C++ standard.
     */
    void load()
    {
      using namespace std;
      
      ifstream in(this->str().c_str());

      if (in) {

	this->str("");                     // reset

	(*this) << in.rdbuf();             // copy

	in.close();
      }
    }
  };
}

#endif
