#ifndef __JLANG__JSHAREDCOUNTER__
#define __JLANG__JSHAREDCOUNTER__

#include <cstdlib>

/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Shared counter.
   */
  class JSharedCounter 
  {
  public:
    /**
     * Default constructor.
     */
    JSharedCounter() :
      counter(NULL)
    {}


    /**
     * Initialise counter.
     */
    void initialise()
    {
      detach();

      counter = new int(1);
    }


    /**
     * Attach this counter to given shared counter object.
     *
     * \param  object   shared counter
     */
    void attach(const JSharedCounter& object)
    {
      detach();

      counter = object.counter;

      if (counter != NULL) {
	++(*counter);
      }
    }


    /**
     * Detach.
     *
     * \return          true if counter at zero; else false
     */
    bool detach() 
    {
      if (counter != NULL) {

	if (--(*counter) == 0) {

	  delete counter;

	  counter = NULL;

	  return true;
	}

	counter = NULL;
      }

      return false;
    }


    /**
     * Get count
     * 
     * \return          count
     */
    const int getCount()
    {
      return (counter != NULL ? *counter : 0);
    }

  protected:
    int* counter;
  };
}

#endif
