#ifndef __JLANG__JABSTRACTOBJECTSTATUS__
#define __JLANG__JABSTRACTOBJECTSTATUS__

/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * Interface for status of object.\n
   * This interface implements:
   *    - type conversion operator <tt>bool ()</tt>; and 
   *    - negate operator <tt>!</tt>.
   */
  struct JAbstractObjectStatus {
    /**
     * Get status of object.
     *
     * \return                status of this object
     */
    virtual bool getStatus() const = 0;


    /**
     * Type conversion operator.
     *
     * \return                status of this object
     */
    operator bool() const
    {
      return this->getStatus();
    }


    /**
     * Negated status of this object.
     *
     * \return                negated status of this object
     */
    bool operator!() const
    {
      return !(this->getStatus());
    }
  };
}

#endif
