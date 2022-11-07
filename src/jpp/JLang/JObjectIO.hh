#ifndef __JLANG__JOBJECTIO__
#define __JLANG__JOBJECTIO__

#include <string>
#include <fstream>

#include "JLang/JType.hh"
#include "JLang/JException.hh"


/**
 * \file
 *
 * General methods for loading and storing a single object from and to a file, respectively.
 * \author mdejong
 */
namespace JLANG {}
namespace JPP { using namespace JLANG; }


namespace JLANG {

  /**
   * Get error status of reader.
   *
   * \param  reader          reader
   * \return                 true if error; else false
   */
  template<class JReader_t>
  inline bool getError(const JReader_t& reader)
  {
    return !reader;
  }


  /**
   * Get error status of reader.
   *
   * \param  reader          reader
   * \return                 true if error; else false
   */
  inline bool getError(const std::ifstream& reader)
  {
    return reader.bad() || (reader.fail() && !reader.eof());
  }


  /**
   * Load object from input file.
   *
   * \param  file_name       file name
   * \param  object          object to be read
   */
  template<class JReader_t, class T>
  inline void load(const std::string& file_name, T& object)
  {
    load(file_name, object, JType<JReader_t>());
  }
  

  /**
   * Store object to output file.
   *
   * \param  file_name       file name
   * \param  object          object to be written
   */
  template<class JWriter_t, class T>
  inline void store(const std::string& file_name, const T& object)
  {
    store(file_name, object, JType<JWriter_t>());
  }
    
      
  /**
   * Load object from input file.
   *
   * This method makes use of the STD protocol for the given type reader, namely:
   * <pre>
   *   JReader_t in(file_name);
   *
   *   in >> object;
   *
   *   in.close();
   * </pre>
   * This method should be overloaded if the type reader requires a different protocol.
   *
   * \param  file_name       file name
   * \param  object          object to be read
   * \param  type            reader type
   */
  template<class JReader_t, class T>
  inline void load(const std::string& file_name, T& object, JType<JReader_t> type)
  {
    JReader_t in(file_name.c_str());
    
    if (!in) {
      THROW(JFileOpenException, "Error opening file: " << file_name);
    }    
    
    in >> object;
    
    if (getError(in)) {
      THROW(JFileReadException, "Error reading file: " << file_name);
    }
    
    in.close();
  }


  /**
   * Store object to output file.
   *
   * This method makes use of the STD protocol for the given type writer, namely:
   * <pre>
   *   JWriter_t out(file_name);
   *
   *   out << object;
   *
   *   out.close();
   * </pre>
   * This method should be overloaded if the type writer requires a different protocol.
   *
   * \param  file_name       file name
   * \param  object          object to be written
   * \param  type            writer type
   */
  template<class JWriter_t, class T>
  inline void store(const std::string& file_name, const T& object, JType<JWriter_t> type)
  {
    JWriter_t out(file_name.c_str());

    if (!out) {
      THROW(JFileOpenException, "Error opening file: " << file_name);
    }    

    out << object;

    out.close();
  }


  /**
   * Load object from input file.
   *
   * \param  file_name       file name
   * \param  object          object to be read
   */
  template<class T>
  inline void load(const std::string& file_name, T& object)
  {
    load(file_name, object, JType<std::ifstream>());
  }


  /**
   * Store object to output file.
   *
   * \param  file_name       file name
   * \param  object          object to be written
   */
  template<class T>
  inline void store(const std::string& file_name, const T& object)
  {
    store(file_name, object, JType<std::ofstream>());
  }
}

#endif
