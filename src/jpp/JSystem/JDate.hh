#ifndef __JSYSTEM__JDATE__
#define __JSYSTEM__JDATE__

#include <time.h>
#include <string>
#include <istream>

#include "JLang/JException.hh"

/**
 * \file
 * Date and time functions.
 * \author mdejong, bjung
 */
namespace JSYSTEM {}
namespace JPP { using namespace JSYSTEM; }

namespace JSYSTEM {
  
  /**
   * Date and time formats.
   */
  enum JDateAndTimeFormat {
    HUMAN_READABLE = 0,        //!< Human readable format (Www Mmm dd hh:mm:ss yyyy)
    ISO8601        = 1         //!< ISO-8601 standard 
  };
  
  
  /**
   * Get ASCII formatted date.
   *
   * \param  option   formatting option
   * \return          date
   */
  inline const char* getDate(const JDateAndTimeFormat option = ISO8601)
  {
    static time_t    ts;
    static const int        MAX_SIZE = 256;
    static char      buffer[MAX_SIZE];

    time(&ts);

    switch (option) {
      
    case HUMAN_READABLE:
      strftime(buffer, MAX_SIZE, "%x", localtime(&ts));
      break;
      
    case ISO8601:
      strftime(buffer, MAX_SIZE, "%F", localtime(&ts));
      break;
      
    default:
      THROW(JLANG::JValueOutOfRange, "JDate::getDate(): Invalid formatting option.");
    }
      
    return buffer;
  }

  
  /**
   * Get ASCII formatted time.
   *
   * \param  option   formatting option
   * \return          time
   */
  inline const char* getTime(const JDateAndTimeFormat option = ISO8601)
  {
    static time_t    ts;
    static const int        MAX_SIZE = 256;
    static char      buffer[MAX_SIZE];

    time(&ts);

    switch (option) {
      
    case HUMAN_READABLE:
      strftime(buffer, MAX_SIZE, "%X %Z", localtime(&ts));
      break;

    case ISO8601:
      strftime(buffer, MAX_SIZE, "%T%z", localtime(&ts));
      break;

    default:
      THROW(JLANG::JValueOutOfRange, "JDate::getTime(): Invalid formatting option.");
    }
    
    return buffer;
  }


  /**
   * Auxililary class to get date and time.
   */
  struct JDateAndTime {
    /**
     * Default constructor.
     */
    JDateAndTime()
    {
      set();
    }


    /**
     * Constructor.
     *
     * \param  t1       time
     */
    JDateAndTime(const time_t t1)
    {
      set(t1);
    }


    /**
     * Smart pointer.
     *
     * \return          pointer to time structure
     */
    tm* operator->()
    {
      return tp;
    }
    

    /**
     * Smart pointer.
     *
     * \return          pointer to time structure
     */
    const tm* operator->() const
    {
      return tp;
    }

    int getSeconds() const { return tp->tm_sec; }           //!< seconds after the minute [0-59]
    int getMinutes() const { return tp->tm_min; }           //!< minutes after the hour   [0-59]
    int getHour()    const { return tp->tm_hour; }          //!< hours   after midnight   [0-23]
    int getDay()     const { return tp->tm_mday; }          //!< day   of the month       [1-31]
    int getMonth()   const { return tp->tm_mon  +    1; }   //!< month of the year        [1-12]
    int getYear()    const { return tp->tm_year + 1900; }   //!< year a.d.


    /**
     * Type conversion operator.
     *
     * \return          ASCII formatted date and time
     */
    operator std::string() const
    {
      return toString();
    }

    
    /**
     * Function to check ISO-8601 conformity of string-formatted date and time.
     *
     * \param  datestr  string-formatted date and time
     * \return          true if date and time are ISO-8601-conform; else false
     */
    inline static bool isISO8601(const std::string& datestr)
    {
      using namespace std;

      const size_t pos = ( datestr.find('Z') != string::npos ? datestr.find('Z') :
			  (datestr.find('+') != string::npos ? datestr.find('+') : datestr.find('-')) );

      if (pos != string::npos) {
	
	static tm t;

	const  string td = datestr.substr(0, pos);
	const  string tz = datestr.substr(pos+1);
	
	const  char*  p0 = strptime(td.c_str(), "%FT%T", &t);
	const  char*  p1 = ( tz.size() < 3 ? strptime(tz.c_str(), "%H",   &t) :
			    (tz.size() < 5 ? strptime(tz.c_str(), "%H%M", &t) : strptime(tz.c_str(), "%H:%M", &t)) );

	return ( (p0 != NULL && string(p0).empty()) && ((p1 != NULL && string(p1).empty()) || tz == "Z") );
      }

      return false;
    }
    

    /**
     * Get ASCII formatted date and time.
     *
     * \param  option   formatting option
     * \return          ASCII formatted date and time
     */
    inline std::string toString(const JDateAndTimeFormat option = ISO8601) const
    {
      using namespace std;

      mktime(tp);
      
      static const int        MAX_SIZE = 256;
      static char      buffer[MAX_SIZE];
      
      switch (option) {
	
      case ISO8601:
	strftime(buffer, MAX_SIZE, "%FT%T%z",           tp);
	break;
	
      case HUMAN_READABLE:
	strftime(buffer, MAX_SIZE, "%a %b %d %X %Z %Y", tp);
	break;

      default:
	THROW(JLANG::JValueOutOfRange, "JDateAndTime::toString(): Invalid formatting option.");
      }
      
      // remove the last character (carriage return) from the date string
      
      buffer[MAX_SIZE-1] = '\0';
      
      return string(buffer);
    }


    /**
     * Set date and time.
     *
     * \return          date and time
     */
    inline const JDateAndTime& operator()() const
    {
      set();

      return *this;
    }


    /**
     * Set to actual time.
     */
    void set() const
    {
      time(&ts);

      tp = localtime(&ts);
    }


    /**
     * Set to given time.
     *
     * \param  t1       time
     */
    void set(const time_t t1) const
    {
      ts = t1;
      tp = localtime(&t1);
    }


    /**
     * Get elapsed time since given date and time.
     *
     * \param  object   date and time
     * \return          time [s]
     */
    double getElapsedTime(const JDateAndTime& object) const
    {
      return difftime(this->ts, object.ts);
    }


    /**
     * Write date and time to output.
     *
     * \param  out            output stream
     * \param  object         date and time
     * \return                output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JDateAndTime& object)
    {
      return out << object.toString();
    }


  private:
    mutable time_t ts;
    mutable tm*    tp;
  };


  /**
   * Function object to get ASCII formatted date and time.
   */
  static JDateAndTime getDateAndTime;
}

#endif
