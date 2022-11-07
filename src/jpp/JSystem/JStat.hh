#ifndef __JSYSTEM__JSTAT__
#define __JSYSTEM__JSTAT__

#include <sys/stat.h>
#include <string>

#include "JSystem/JDate.hh"
#include "JLang/JAbstractObjectStatus.hh"


/**
 * \file
 * File status.
 * \author mdejong
 */
namespace JSYSTEM {}
namespace JPP { using namespace JSYSTEM; }

namespace JSYSTEM {

  using JLANG::JAbstractObjectStatus;


  /**
   * Auxiliary class for file status.
   * This class encapsulates the <tt>stat</tt> data structure.
   */
  struct JStat :
    public stat,
    public JAbstractObjectStatus
  {
    /**
     * Default constructor.
     */
    JStat()
    {}


    /**
     * Constructor.
     *
     * \param  file_name      file name
     */
    JStat(const char* file_name)
    {
      get(file_name);
    }


    /**
     * Get status of object.
     *
     * \return                status of this object
     */
    virtual bool getStatus() const override 
    {
      return this->error == 0;
    }
     

    /**
     * Get status of file.
     *
     * \param  file_name      file name
     * \return                file status
     */
    const JStat& get(const char* file_name) 
    {
      if (::stat(file_name, static_cast<stat*>(this)) != 0)
	this->error = errno;
      else
	this->error = 0;

      return *this;
    }

     
    /**
     * Get status of file.
     *
     * \param  file_name      file name
     * \return                file status
     */
    const JStat& operator()(const char* file_name) 
    {
      return get(file_name);
    }


    /**
     * Get UID of file.
     *
     * \return                UID
     */
    uid_t getUID() const
    {
      return this->st_uid;
    }
     

    /**
     * Get GID of file.
     *
     * \return                GID
     */
    uid_t getGID() const
    {
      return this->st_gid;
    }
     

    /**
     * Get size of file.
     *
     * \return                size [B]
     */
    off_t getSize() const
    {
      return this->st_size;
    }
     

    /**
     * Get time of last access.
     *
     * \return                date and time
     */
    JDateAndTime getTimeOfLastAccess() const
    {
      return JDateAndTime(this->st_atime);
    }


    /**
     * Get time of last modification.
     *
     * \return                date and time
     */
    JDateAndTime getTimeOfLastModification() const
    {
      return JDateAndTime(this->st_mtime);
    }


    /**
     * Get time of last change.
     *
     * \return                date and time
     */
    JDateAndTime getTimeOfLastChange() const
    {
      return JDateAndTime(this->st_ctime);
    }


    /**
     * Get error of last call.
     *
     * \return                error
     */
    int getError() const
    {
      return this->error;
    }

    int error;  //!< error code from last call
  };


  /**
   * Function object for file status.
   */
  static JStat getFileStatus;
}

#endif

