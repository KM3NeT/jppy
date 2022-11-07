#ifndef __JEEP__JEEPTOOLKIT__
#define __JEEP__JEEPTOOLKIT__

#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>

#include "JLang/gzstream.h"
#include "JLang/JException.hh"


/**
 * \file
 *
 * Auxiliary methods for handling file names, type names and environment.
 * \author mdejong
 */

/**
 * General puprpose classes and methods.
 */
namespace JEEP {}
namespace JPP { using namespace JEEP; }

namespace JEEP {

  using JLANG::JNoValue;

  /**
   * Nick names of environment variables.
   */
  static const char* const LD_LIBRARY_PATH      = "LD_LIBRARY_PATH";  //!< library file paths
  static const char* const PATH                 = "PATH";             //!< binary  file paths
  static const char* const SHELL                = "SHELL";            //!< SHELL
  static const char* const JPP_PAGES            = "JPP_PAGES";        //!< Jpp document pages

  static const char        PATHNAME_SEPARATOR   = '/';                //!< path name separator
  static const char        PATHLIST_SEPARATOR   = ':';                //!< path list separator
  static const char        FILENAME_SEPARATOR   = '.';                //!< file name separator
  static const char* const TYPENAME_SEPARATOR   = "::";               //!< type name separator
  static const char        PROTOCOL_SEPARATOR   = ':';                //!< protocol  separator
  static const char        FILENAME_WILD_CARD   = '%';                //!< wild card character for file name substitution


  /**
   * Check presence of wild card.
   *
   * \param  file_name         file name
   * \return                   true if wild card present; else false
   */
  inline bool hasWildCard(const std::string& file_name)
  {
    return (file_name.find(FILENAME_WILD_CARD) != std::string::npos);
  }


  /**
   * Get file name by setting wild card to given value.
   *
   * \param  file_name          input  file name
   * \param  value              value
   * \return                    output file name
   */
  inline std::string setWildCard(const std::string& file_name, const std::string& value)
  {
    using namespace std;
    using namespace JPP;

    string buffer = file_name;

    string::size_type pos = buffer.find(FILENAME_WILD_CARD);

    if (pos == string::npos) {
      THROW(JNoValue, "Method getFilename(): Missing wild card character \'" << FILENAME_WILD_CARD << "\'.");
    }

    return buffer.replace(pos, 1, value);
  }


  /**
   * Strip leading and trailing white spaces from file name.
   *
   * \param  file_name         file name
   * \return                   file name
   */
  inline std::string strip(const std::string& file_name)
  {
    using namespace std;

    string::const_iterator         p = file_name. begin();
    string::const_reverse_iterator q = file_name.rbegin();
    
    for ( ; p != file_name.end() && q != file_name.rend() && isspace(*p); ++p) {}
    for ( ; p != file_name.end() && q != file_name.rend() && isspace(*q); ++q) {}
    
    return string(p,q.base());
  }


  /**
   * Get file name extension, i.e.\ part after last JEEP::FILENAME_SEPARATOR if any.
   *
   * \param  file_name         file name
   * \return                   extension (excluding separator)
   */
  inline std::string getFilenameExtension(const std::string& file_name)
  {
    using namespace std;

    const size_t pos = file_name.rfind(FILENAME_SEPARATOR);

    if (pos != string::npos)
      return file_name.substr(pos + 1);
    else
      return "";
  }


  /**
   * Get file name part, i.e.\ part after last JEEP::PATHNAME_SEPARATOR if any.
   *
   * \param  file_name          file name
   * \return                    file name part (excluding separator)
   */
  inline std::string getFilename(const std::string& file_name)
  {
    using namespace std;

    const string buffer = strip(file_name);
    const size_t pos    = buffer.rfind(PATHNAME_SEPARATOR);

    if (pos != string::npos)
      return buffer.substr(pos + 1);
    else
      return buffer;
  }


  /**
   * Get path, i.e.\ part before last JEEP::PATHNAME_SEPARATOR if any.
   *
   * \param  file_name          file name
   * \return                    path (including separator)
   */
  inline std::string getPath(const std::string& file_name)
  {
    using namespace std;

    const string buffer = strip(file_name);
    const size_t pos    = buffer.rfind(PATHNAME_SEPARATOR);

    if (pos != string::npos)
      return buffer.substr(0, pos + 1);
    else
      return "";
  }


  /**
   * Get full path, i.e.\ add JEEP::PATHNAME_SEPARATOR if necessary.
   *
   * \param  path               path
   * \return                    path (including separator)
   */
  inline std::string getFullPath(const std::string& path)
  {
    using namespace std;

    const string buffer = strip(path);

    if (buffer.empty() || *buffer.rbegin() == PATHNAME_SEPARATOR) {

      return buffer;

    } else {

      return buffer + PATHNAME_SEPARATOR;
    }
  }

  
  /**
   * Compose full file name and introduce JEEP::PATHNAME_SEPARATOR if needed.
   *
   * \param  path               path
   * \param  file_name          file name
   * \return                    file name
   */
  inline std::string getFilename(const std::string& path, const std::string& file_name)
  {
    using namespace std;

    const string buffer = getFullPath(path);

    if (buffer.empty())
      return strip(file_name);
    else
      return buffer + strip(file_name);
  }


  /**
   * Get selected path from environment variable for given file name.
   *
   * The environment variable is parsed according character JEEP::PATHLIST_SEPARATOR.
   * The first path in which a file exists with the given file name is returned.
   * If no existing file is found, an empty path is returned.
   *
   * \param  variable           environment variable
   * \param  file_name          file name
   * \return                    path
   */
  inline std::string getPath(const std::string& variable, const std::string& file_name)
  {
    using namespace std;

    string path = "";

    if (!file_name.empty() && file_name[0] != PATHNAME_SEPARATOR) {

      const string buffer(getenv(variable.c_str()));
	
      if (!buffer.empty()) {
	  
	size_t pos = 0, len;
	  
	do { 
	  
	  len  = buffer.substr(pos).find(PATHLIST_SEPARATOR);
	  path = buffer.substr(pos,len);

	} while (!ifstream(getFilename(path, file_name).c_str()).good() && len != string::npos && (pos += len + 1) != buffer.length());
      }
    }

    if (ifstream(getFilename(path, file_name).c_str()).good())
      return path;
    else
      return "";
  }  


  /**
   * Get full file name (see JEEP::getPath).
   *
   * \param  variable           environment variable
   * \param  file_name          file name
   * \return                    file name
   */
  inline std::string getFullFilename(const std::string& variable, const std::string& file_name)
  {
    return getFilename(getPath(variable, file_name), file_name);
  }


  /**
   * Get name space, i.e.\ part before JEEP::TYPENAME_SEPARATOR.
   *
   * \param  type_name         type name
   * \return                   name space (possibly empty)
   */
  inline std::string getNamespace(const std::string& type_name)
  {
    using namespace std;

    const size_t pos = type_name.rfind(TYPENAME_SEPARATOR);

    if (pos != string::npos)
      return type_name.substr(0, pos);
    else
      return "";
  }


  /**
   * Get type name, i.e.\ part after JEEP::TYPENAME_SEPARATOR.
   *
   * \param  type_name         type name
   * \return                   class name
   */
  inline std::string getClassname(const std::string& type_name)
  {
    using namespace std;

    const size_t pos = type_name.rfind(TYPENAME_SEPARATOR);

    if (pos != string::npos)
      return type_name.substr(pos + 2);
    else
      return type_name;
  }


  /**
   * Get protocol, i.e.\ part before first JEEP::PROTOCOL_SEPARATOR if any.
   *
   * \param  file_name         file name
   * \return                   protocol (excluding separator)
   */
  inline std::string getProtocol(const std::string& file_name)
  {
    using namespace std;

    const size_t pos = file_name.find(PROTOCOL_SEPARATOR);

    if (pos != string::npos)
      return file_name.substr(0, pos);
    else
      return "";
  }


  /**
   * Get URL of document pages.
   *
   * \return                   URL
   */
  inline std::string getURL()
  {
    const char* const url = getenv(JPP_PAGES);

    return std::string(url != NULL ? url : "");
  }


  /**
   * Open file.
   *
   * \param  file_name           file name
   * \return                     pointer to input stream
   */
  template<class T>
  inline T* open(const std::string& file_name);


  /**
   * Open file.
   *
   * \param  file_name           file name
   * \return                     pointer to input stream
   */
  template<>
  inline std::istream* open(const std::string& file_name)
  {
    using namespace std;
    using namespace JPP;

    if      (getFilenameExtension(file_name) == "gz") 
      return new igzstream(file_name.c_str());
    else if (getFilenameExtension(file_name) == "txt") 
      return new ifstream (file_name.c_str());
    else
      return NULL;
  }


  /**
   * Open file.
   *
   * \param  file_name           file name
   * \return                     pointer to output stream
   */
  template<>
  inline std::ostream* open(const std::string& file_name)
  {
    using namespace std;
    using namespace JPP;

    if      (getFilenameExtension(file_name) == "gz") 
      return new ogzstream(file_name.c_str());
    else if (getFilenameExtension(file_name) == "txt") 
      return new ofstream (file_name.c_str());
    else
      return NULL;
  }


  /**
   * Close file.
   *
   * \param  pf                  pointer to file stream
   */
  inline void close(std::istream* pf)
  {
    using namespace std;
    using namespace JPP;

    if (dynamic_cast<ifstream*> (pf) != NULL) { dynamic_cast<ifstream*> (pf)->close(); return; }
    if (dynamic_cast<igzstream*>(pf) != NULL) { dynamic_cast<igzstream*>(pf)->close(); return; }
  }


  /**
   * Close file.
   *
   * \param  pf                  pointer to file stream
   */
  inline void close(std::ostream* pf)
  {
    using namespace std;
    using namespace JPP;

    if (dynamic_cast<ofstream*> (pf) != NULL) { dynamic_cast<ofstream*> (pf)->close(); return; }
    if (dynamic_cast<ogzstream*>(pf) != NULL) { dynamic_cast<ogzstream*>(pf)->close(); return; }
  }
}

#endif
