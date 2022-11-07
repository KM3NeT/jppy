#ifndef __JEEP__JMESSAGE__
#define __JEEP__JMESSAGE__

#include <iostream>

#include "JLang/JFileStream.hh"
#include "Jeep/JColor.hh"


/**
 * \file
 * General purpose messaging.
 * \author mdejong
 */

extern int  debug;   //!< debug level
extern int  qaqc;    //!< QA/QC file descriptor

namespace JEEP {}
namespace JPP { using namespace JEEP; }

namespace JEEP {

  /**
   * Debug level.
   */
  enum JMessage_t {
    
    debug_t   = 3,    //!< debug
    status_t  = 2,    //!< status
    warning_t = 2,    //!< warning
    notice_t  = 1,    //!< notice
    error_t   = 0,    //!< error
    fatal_t   = 0     //!< fatal; exit
  };


  /**
   * Auxiliary class for handling debug parameter within a class.
   * Note that the derived class should include the statement <tt>using JMessage<..>::debug;</tt>
   * otherwise the linker will fail.
   */
  template<class T>
  struct JMessage {
    static int debug;    // debug level
  };

  
  /**
   * debug level (default is off).
   */
  template<class T>
  int JMessage<T>::debug = 0;
}


/**
 * Message macros.
 *
 * \param  A          std::ostream compatible output
 */
#define DEBUG(A)   do { if (debug >= JEEP::debug_t)   { std::cout << A << std::flush; }          } while (0)
#define STATUS(A)  do { if (debug >= JEEP::status_t)  { std::cout << A << std::flush; }          } while (0)
#define NOTICE(A)  do { if (debug >= JEEP::notice_t)  { std::cerr << A << std::flush; }          } while (0)
#define WARNING(A) do { if (debug >= JEEP::warning_t) { std::cerr << "WARNING: " << A << std::flush; }          } while (0)
#define ERROR(A)   do {                               { std::cerr << "ERROR: "   << A << std::flush; }          } while (0)
#define FATAL(A)   do {                               { std::cerr << "FATAL: "   << A << std::endl; exit(1);  } } while (0)


/**
 * Make std::ostream compatible output for variadic macro.
 *
 * When called,
 *  - first argument should correspond to a dummy value;
 *  - second to the ##__VA_ARGS__ macro; and
 *  - third to the fall back value (e.g.\ "");
 *
 * \param  A           dummy value
 * \param  B           ##__VA_ARGS__ macro
 * \return             output
 */
#define VARGS_STREAM(A, B, ...) B


/**
 * Assert macro.
 *
 * \param  A          test
 */
#define ASSERT(A, ...)  do {						\
    if (A) { NOTICE(JEEP::GREEN << "Test at " << __FILE__ << ":" << __LINE__ << " (" << #A << ") \"" << VARGS_STRING("", ##__VA_ARGS__, "") << "\" passed." << JEEP::RESET << std::endl); } \
    else   { FATAL (JEEP::RED   << "Test at " << __FILE__ << ":" << __LINE__ << " (" << #A << ") \"" << VARGS_STRING("", ##__VA_ARGS__, "") << "\" failed." << JEEP::RESET << std::endl); } } while (0)


/**
 * QA/QC output macro.
 *
 * \param  A          ostream compatible output
 */
#define QAQC(A) do { if (qaqc > 0) { JLANG::JFileOutputStream(qaqc) << A; } } while (0)

#endif
