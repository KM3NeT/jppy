#ifndef __JLANG__JOBJECTID__
#define __JLANG__JOBJECTID__

#include <istream>
#include <ostream>

#include "JLang/JComparable.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {

  /**
   * Auxiliary class for object identification.
   */
  class JObjectID :
    public JComparable<JObjectID>,
    public JComparable<JObjectID, int>
  {
  public:
    /**
     * Default constructor.
     */
    JObjectID() :
      __id(-1)
    {}

  
    /**
     * Constructor.
     *
     * \param  id         identifier
     */
    JObjectID(const int id) :
      __id(id)
    {}


    /**
     * Get identifier.
     *
     * \return            identifier
     */
    int getID() const
    { 
      return __id;
    }


    /**
     * Get identifier.
     *
     * \return            identifier
     */
    int& getID()
    { 
      return __id;
    }


    /**
     * Set identifier.
     *
     * \param  id         identifier
     */
    void setID(const int id)
    { 
      this->__id = id;
    }


    /**
     * Less than method.
     *
     * \param  object     object identifier
     * \return            true if this identifier less than given identifier; else false
     */
    inline bool less(const JObjectID& object) const
    { 
      return this->getID() < object.getID();
    }


    /**
     * Less than method.
     *
     * \param  id         identifier
     * \return            true if this identifier less than given identifier; else false
     */
    inline bool less(const int id) const
    { 
      return this->getID() < id;
    }


    /**
     * More than method.
     *
     * \param  id         identifier
     * \return            true if this identifier greater than given identifier; else false
     */
    inline bool more(const int id) const
    { 
      return this->getID() > id;
    }

    
    /**
     * Read object identifier from input.
     *
     * \param  in         input stream
     * \param  object     object identifier
     * \return            input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JObjectID& object)
    {
      return in >> object.__id;
    }


    /**
     * Write object identifier to output.
     *
     * \param  out        output stream
     * \param  object     object identifier
     * \return            output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JObjectID& object)
    {
      return out << object.__id;
    }
           
  protected:
    int __id;
  };


  /**
   * Get undefined object identifier.
   *
   * \return           undefined object identifier
   */
  inline const JObjectID& getUndefinedObjectID()
  {
    static JObjectID id;

    return id;
  }
}

#endif
