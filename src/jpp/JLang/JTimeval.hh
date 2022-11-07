#ifndef __JLANG__JTIMEVAL__
#define __JLANG__JTIMEVAL__

#include <sys/time.h>
#include <istream>
#include <ostream>
#include <limits>

#include "JLang/JComparable.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Auxiliary class for time values.
   * This class encapsulates the <tt>timeval</tt> data structure.
   */
  class JTimeval :
    public timeval,
    public JComparable<JTimeval>
  {
  public:
    /**
     * Default constructor.
     */
    JTimeval()
    {
      this->tv_sec  = 0;
      this->tv_usec = 0;
    }


    /**
     * Constructor.
     *
     * \param  tv_us                time [us]
     */
    JTimeval(const int tv_us)
    {
      this->tv_sec  = 0;
      this->tv_usec = tv_us;
    }


    /**
     * Constructor.
     *
     * \param  tv_s                 time [s]
     * \param  tv_us                time [us]
     */
    JTimeval(const int tv_s, const int tv_us)
    {
      this->tv_sec  = tv_s;
      this->tv_usec = tv_us;
    }


    /**
     * Get time value.
     *
     * \return                     time value
     */
    const JTimeval& getTimeval() const
    {
      return static_cast<const JTimeval&>(*this);
    }


    /**
     * Get time value.
     *
     * \return                     time value
     */
    JTimeval& getTimeval()
    {
      return static_cast<JTimeval&>(*this);
    }


    /**
     * Set time value.
     *
     * \param  timeval             time value
     */
    void setTimeval(const JTimeval& timeval)
    {
      static_cast<JTimeval&>(*this) = timeval;
    }


    /**
     * Less than method.
     *
     * \param  value                time value
     * \result                      true if this time value less than given time value; else false
     */
    inline bool less(const JTimeval& value) const
    {
      if (this->tv_sec == value.tv_sec)
	return this->tv_usec < value.tv_usec;
      else
	return this->tv_sec  < value.tv_sec;
    }


    /**
     * Get minimal time value.
     *
     * \return                      time value
     */
    static JTimeval min()
    {
      return JTimeval(0, 0);
    }



    /**
     * Get maximal time value.
     *
     * \return                      time value
     */
    static JTimeval max()
    {
      return JTimeval(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    }



    /**
     * Get pointer to time value.
     *
     * \return                      pointer to time value
     */
    const timeval* get() const
    {
      return static_cast<const timeval*>(this);
    }


    /**
     * Get pointer to time value.
     *
     * \return                      pointer to time value
     */
    timeval* get()
    {
      return static_cast<timeval*>(this);
    }


    /**
     * Address of operator.
     *
     * \return                      pointer to time value
     */
    const timeval* operator &() const
    {
      return get();
    }


    /**
     * Address of operator.
     *
     * \return                      pointer to time value
     */
    timeval* operator &()
    {
      return get();
    }


    /**
     * Read time value from input.
     *
     * \param  in         input stream
     * \param  time       time value
     * \return            input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JTimeval& time)
    {
      return in >> time.tv_sec >> time.tv_usec;
    }


    /**
     * Write time value to output.
     *
     * \param  out        output stream
     * \param  time       time value
     * \return            output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JTimeval& time)
    {
      return out << time.tv_sec << ' ' << time.tv_usec;
    }
  };
}

#endif
