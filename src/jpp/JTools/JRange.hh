#ifndef __JTOOLS__JRANGE__
#define __JTOOLS__JRANGE__

#include <cmath>
#include <utility>
#include <functional>

#include "JLang/JClass.hh"
#include "JLang/JBool.hh"
#include "JLang/JEquals.hh"
#include "JLang/JVectorize.hh"
#include "JTools/JPair.hh"
#include "JMath/JMath.hh"
#include "JMath/JLimits.hh"


/**
 * \file
 *
 * Auxiliary class to define a range between two values.
 * \author mdejong
 */
namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JLANG::JEquals;
  using JLANG::array_type;
  using JLANG::make_array;
  using JMATH::JMath;


  /**
   * Range of values.
   */
  template<class T, class JComparator_t = std::less<T> >
  class JRange :
    public JPair<T,T>,
    public JEquals< JRange<T> >,
    public JMath  < JRange<T> >
  {
  public:

    typedef std::pair<T,T>                             pair_type;
    typedef JRange<T, JComparator_t>                   range_type;
    typedef typename JLANG::JClass<T>::argument_type   argument_type;


    /**
     * Default constructor.\n
     * This range corresponds to the maximal possible range.
     */
    JRange() :
      JPair<T,T>(getMinimum(), getMaximum())
    {}


    /**
     * Constructor.
     *
     * \param  pair             pair
     */
    JRange(const pair_type& pair) :
      JPair<T,T>(pair.first, pair.second)
    {}


    /**
     * Constructor.
     *
     * \param  x                lower limit
     * \param  y                upper limit
     */
    JRange(argument_type x,
	   argument_type y) :
      JPair<T,T>(x, y)
    {}


    /**
     * Constructor.
     *
     * \param  x                lower and upper limit
     */
    JRange(argument_type x) :
      JPair<T,T>(x, x)
    {}


    /**
     * Constructor.\n
     * The arguments could be values or iterators.
     *
     * \param  first            first
     * \param  second           second
     */
    template<class R>
    JRange(R first, R second) :
      JPair<T,T>()
    {
      setRange(first, second);
    }


    /**
     * Constructor.
     *
     * \param  buffer           input data
     */
    template<class JElement_t, class JAllocator_t>
    JRange(const array_type<JElement_t, JAllocator_t>& buffer) :
      JPair<T,T>()
    {
      setRange(buffer);
    }


    /**
     * Type conversion operator.
     *
     * \return                  piar
     */
    operator pair_type() const
    {
      return pair_type(getLowerLimit(), getUpperLimit());
    }


    /**
     * Get range.
     *
     * \return                  range
     */
    const range_type& getRange() const
    {
      return static_cast<const range_type&>(*this);
    }


    /**
     * Set range.
     *
     * \param  range            range
     */
    void setRange(const range_type& range)
    {
      static_cast<range_type&>(*this) = range;
    }


    /**
     * Set lower and upper limit.
     *
     * \param  x                lower limit
     * \param  y                upper limit
     */
    void setRange(argument_type x, argument_type y)
    {
      this->first  = x;
      this->second = y;
    }


    /**
     * Set range.\n
     * The arguments could be values or iterators.
     *
     * \param  first            first
     * \param  second           second
     */
    template<class R>
    void setRange(R first, R second)
    {
      using namespace JLANG;
      
      setRange(first, second, JBool<is_iterator<R>::value>());
    }


    /**
     * Set lower and upper limit according to input data.
     *
     * \param  buffer           input data
     */
    template<class JElement_t, class JAllocator_t>
    void setRange(const array_type<JElement_t, JAllocator_t>& buffer)
    {
      setRange(getMaximum(), getMinimum());

      for (typename array_type<JElement_t, JAllocator_t>::const_iterator i = buffer.begin(); i != buffer.end(); ++i) {
	include(*i);
      }
    }


    /**
     * Get lower limit.
     *
     * \return                  lower limit
     */
    T getLowerLimit() const
    {
      return this->first;
    }


    /**
     * Get upper limit.
     *
     * \return                  upper limit
     */
    T getUpperLimit() const
    {
      return this->second;
    }


    /**
     * Set lower limit.
     *
     * \param  x                lower limit
     */
    void setLowerLimit(argument_type x)
    {
      this->first  = x;
    }


    /**
     * Set upper limit.
     *
     * \param  y                upper limit
     */
    void setUpperLimit(argument_type y)
    {
      this->second = y;
    }


    /**
     * Fix lower limit.
     *
     * The range is shifted to the given lower limit.
     *
     * \param  x                lower limit
     */
    void fixLowerLimit(argument_type x)
    {
      this->second += x - this->first;
      this->first   = x;
    }


    /**
     * Fix upper limit.
     *
     * The range is shifted to the given upper limit.
     *
     * \param  y                upper limit
     */
    void fixUpperLimit(argument_type y)
    {
      this->first  += y - this->second;
      this->second  = y;
    }

    
    /**
     * Equal method.
     *
     * \param  range            range
     * \result                  true if this module range equal to given module range; else false
     */
    inline bool equals(const range_type& range) const
    {
      return (!this->compare(this->getLowerLimit(), range.getLowerLimit()) &&
	      !this->compare(range.getLowerLimit(), this->getLowerLimit()) &&
	      !this->compare(this->getUpperLimit(), range.getUpperLimit()) &&
	      !this->compare(range.getUpperLimit(), this->getUpperLimit()));
    }


    /**
     * Get length (difference between upper and lower limit).
     *
     * \return                  length
     */
    T getLength() const
    {
      return getUpperLimit() - getLowerLimit();
    }


    /**
     * Set length (difference between upper and lower limit).
     *
     * \param  length           length
     */
    void setLength(argument_type length)
    {
      setUpperLimit(getLowerLimit() + length);
    }


    /**
     * Check validity of range.
     *
     * \return                  true if lower limit less than or equal to upper limit; else false
     */
    bool is_valid() const
    {
      return !compare(getUpperLimit(), getLowerLimit());
    }


    /**
     * Test whether value is inside range.
     *
     * \param  x                value
     * \return                  true if lower limit <= value <= upper limit; else false
     */
    bool in_range(argument_type x) const
    {
      return (!compare(x, getLowerLimit()) && 
	      !compare(getUpperLimit(), x));
    }


    /**
     * Test whether value is inside range.
     *
     * \param  x                value
     * \return                  true if lower limit <= value <= upper limit; else false
     */
    bool operator()(argument_type x) const
    {
      return in_range(x);
    }


    /**
     * Constrain value to range.\n
     * This method returns the original value if it is in this range, else
     * lower limit if value < lower limit or upper limit if value > upper limit.
     *
     * \param  x                value
     * \return                  lower limit <= x <= upper limit
     */
    T constrain(argument_type x) const
    {
      if (compare(x, getLowerLimit())) { return getLowerLimit(); }
      if (compare(getUpperLimit(), x)) { return getUpperLimit(); }

      return x;
    }


    /**
     * Modulo value with respect to range.\n
     *
     * \param  x                value
     * \return                  lower limit <= x <= upper limit
     */
    T mod(argument_type x) const
    {
      if      (compare(x, getLowerLimit()))
	return x + getLength() * floor((getUpperLimit() - x) / getLength());
      else if (compare(getUpperLimit(), x))
	return x - getLength() * floor((x - getLowerLimit()) / getLength());
      else
	return x;
    }


    /**
     * Test overlap with given range.\n
     *
     * \param  range            range
     * \return                  true if there is a non-zero overlap; else false
     */
    bool overlap(const range_type& range) const
    {  
      return (!compare(range.getUpperLimit(), getLowerLimit()) &&
	      !compare(getUpperLimit(), range.getLowerLimit()));
    }
    

    /**
     * Include given value to range.\n
     * The new lower limit is the minimim of the original lower limit and given value and\n
     * the new upper limit is the maximum of the original upper limit and given value;
     *
     * \param  x                value
     * \return                  range
     */
    range_type& include(argument_type x)
    {
      if (compare(x, getLowerLimit())) { setLowerLimit(x); }
      if (compare(getUpperLimit(), x)) { setUpperLimit(x); }

      return *this;
    }
    
    
    /**
     * Join ranges.\n
     * The new lower limit is the maximim of the two lower limits and\n
     * the new upper limit is the minimum of the two upper limits.\n
     * This operation results in an equal or smaller range and
     * may result in an unphysical range (i.e.\ lower limit > upper limit).
     *
     * \param  range            range
     */
    range_type& join(const range_type& range)
    {
      if (compare(getLowerLimit(), range.getLowerLimit())) { setLowerLimit(range.getLowerLimit()); }
      if (compare(range.getUpperLimit(), getUpperLimit())) { setUpperLimit(range.getUpperLimit()); }

      return *this;      
    }


    /**
     * Combine ranges.\n
     * The new lower limit is the minimim of the two lower limits and\n
     * the new upper limit is the maximum of the two upper limits.\n
     * This operation results in an equal or larger range.
     *
     * \param  range            range
     */
    range_type& combine(const range_type& range)
    {  
      if (compare(range.getLowerLimit(), getLowerLimit())) { setLowerLimit(range.getLowerLimit()); }
      if (compare(getUpperLimit(), range.getUpperLimit())) { setUpperLimit(range.getUpperLimit()); }

      return *this;
    }
    
    
    /**
     * Add offset.
     *
     * \param  x                offset
     */
    range_type& add(argument_type x)
    { 
      this->first  += x;
      this->second += x;

      return *this;
    }


    /**
     * Subtract offset.
     *
     * \param  x                offset
     */
    range_type& sub(argument_type x)
    { 
      this->first  -= x;
      this->second -= x;

      return *this;
    }


    /**
     * Add offsets.\n
     * The new lower limit is the sum of the two lower limits and\n
     * the new upper limit is the sum of the two upper limits.
     *
     * \param  range            offset
     */
    range_type& add(const range_type& range)
    { 
      this->first  += range.getLowerLimit();
      this->second += range.getUpperLimit();

      return *this;
    }


    /**
     * Subtract offsets.\n
     * The new lower limit is the difference of the two lower limits and\n
     * the new upper limit is the difference of the two upper limits.
     *
     * \param  range            offset
     */
    range_type& sub(const range_type& range)
    { 
      this->first  -= range.getLowerLimit();
      this->second -= range.getUpperLimit();

      return *this;
    }

    
    /**
     * Multiply range.
     *
     * \param  factor           factor
     */
    range_type& mul(const double factor)
    { 
      this->first  *= factor;
      this->second *= factor;

      return *this;
    }

    
    /**
     * Divide range.
     *
     * \param  factor           factor
     */
    range_type& div(const double factor)
    { 
      this->first  /= factor;
      this->second /= factor;

      return *this;
    }


    /**
     * Get minimum possible value.
     *
     * \return                  minimum possible value
     */
    static T getMinimum() 
    { 
      return JMATH::JLimits<T>::min();
    }


    /**
     * Get maximum possible value.
     *
     * \return                  maximum possible value
     */
    static T getMaximum() 
    { 
      return JMATH::JLimits<T>::max();
    }

    
    /**
     * Default range.
     * This range corresponds to an unphysical range.
     */
    static const JRange<T, JComparator_t> DEFAULT_RANGE;


    /**
     * Function object.
     *
     * \param  first            first  argument
     * \param  second           second argument
     * \return                  true if first < second; else false
     */
    JComparator_t compare;

  protected:
    /**
     * Set range.
     *
     * \param  first            first
     * \param  second           second
     * \param  option           false
     */
    template<class R>
    void setRange(R first, R second, const JLANG::JBool<false>& option)
    {
      setRange((argument_type) first, (argument_type) second);
    }


    /**
     * Set range.
     *
     * \param  first            first
     * \param  second           second
     * \param  option           true
     */
    template<class R>
    void setRange(R first, R second, const JLANG::JBool<true>& option)
    {
      setRange(getMaximum(), getMinimum());

      for (R i = first; i != second; ++i) {
	include(*i);
      }
    }
  };


  /**
   * Default range.
   * This range corresponds to an unphysical range.
   */
  template<class T, class JComparator_t>
  const JRange<T, JComparator_t> JRange<T, JComparator_t>::DEFAULT_RANGE(JRange<T, JComparator_t>::getMaximum(), 
									 JRange<T, JComparator_t>::getMinimum()); 


  /**
   * Add ranges.\n
   * The new lower limit is the sum of the two lower limits and\n
   * the new upper limit is the sum of the two upper limits.
   *
   * \param  first               first  range
   * \param  second              second range
   * \result                     range
   */
  template<class T, class JComparator_t>
  inline JRange<T, JComparator_t> operator+(const JRange<T, JComparator_t>& first, const JRange<T, JComparator_t>& second)
  {
    return JRange<T, JComparator_t>(first).add(second);
  }


  /**
   * Subtract ranges.\n
   * The new lower limit is the difference of the two lower limits and
   * the new upper limit is the difference of the two upper limits.
   *
   * \param  first               first  range
   * \param  second              second range
   * \result                     range
   */
  template<class T, class JComparator_t>
  inline JRange<T, JComparator_t> operator-(const JRange<T, JComparator_t>& first, const JRange<T, JComparator_t>& second)
  {
    return JRange<T, JComparator_t>(first).sub(second);
  }


  /**
   * Test overlap between ranges.
   *
   * \param  first               first  range
   * \param  second              second range
   * \return                     true if there is a non-zero overlap; else false
   */
  template<class T, class JComparator_t>
  inline bool overlap(const JRange<T, JComparator_t>& first, const JRange<T, JComparator_t>& second)
  {
    return first.overlap(second);
  }


  /**
   * Join ranges.\n
   * The new lower limit is the maximim of the two lower limits and\n
   * the new upper limit is the minimum of the two upper limits.\n
   * This operation results in an equal or smaller range and
   * may result in an unphysical range (i.e.\ lower limit > upper limit).
   *
   * \param  first               first  range
   * \param  second              second range
   * \result                     range
   */
  template<class T, class JComparator_t>
  inline JRange<T, JComparator_t> join(const JRange<T, JComparator_t>& first, const JRange<T, JComparator_t>& second)
  {  
    return JRange<T, JComparator_t>(first).join(second);
  }


  /**
   * Combine ranges.\n
   * The new lower limit is the minimim of the two lower limits and\n
   * the new upper limit is the maximum of the two upper limits.\n
   * This operation results in an equal or larger range.
   *
   * \param  first               first  range
   * \param  second              second range
   * \result                     range
   */
  template<class T, class JComparator_t>
  inline JRange<T, JComparator_t> combine(const JRange<T, JComparator_t>& first, const JRange<T, JComparator_t>& second)
  {  
    return JRange<T, JComparator_t>(first).combine(second);
  }

  
  /**
   * Auxiliary method to create range of values.
   *
   * \param  x                lower limit
   * \param  y                upper limit
   * \return                  range
   */
  template<class T>
  inline JRange<T> make_range(T x, T y)
  {
    return JRange<T>(x,y);
  } 


  /**
   * Get expected number of occurrences due to given rate within specified interval.
   *
   * \param  range            interval
   * \param  R                rate
   * \return                  expectation value
   */
  template<class T>
  inline double getN(const JRange<T>& range, const double R)
  {
    return R * (range.getUpperLimit() - range.getLowerLimit());
  }
}

#endif
