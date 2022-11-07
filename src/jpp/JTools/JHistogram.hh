#ifndef __JTOOLS__JHISTOGRAM__
#define __JTOOLS__JHISTOGRAM__

#include "JLang/JClass.hh"
#include "JMath/JZero.hh"
#include "JIO/JSerialisable.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JIO::JReader;
  using JIO::JWriter;


  /**
   * Template definition of histogram object interface.
   * This class also comprises auxiliary data.
   */
  template<class JAbscissa_t, class JContents_t>
  class JHistogram
  {
  protected:
    /**
     * Default constructor.
     */
    JHistogram() :
      underflow(JMATH::zero),
      overflow (JMATH::zero),
      integral (JMATH::zero)
    {}


  public:

    typedef JAbscissa_t                                   abscissa_type;
    typedef JContents_t                                   contents_type;


    /**
     * Virtual destructor.
     */
    virtual ~JHistogram()
    {}


    /**
     * Reset.
     */
    void reset()
    {
      this->underflow = JMATH::zero;
      this->overflow  = JMATH::zero;
      this->integral  = JMATH::zero;
    }


    /**
     * Histogram filling.
     *
     * \param  pX              pointer to abscissa values
     * \param  w               weight
     */
    virtual void evaluate(const abscissa_type* pX,
			  typename JLANG::JClass<contents_type>::argument_type w) = 0;


    /**
     * Add histogram.
     *
     * \param  histogram       histogram
     * \return                 this histogram
     */
    JHistogram& add(const JHistogram& histogram)
    {
      this->underflow += histogram.underflow;
      this->overflow  += histogram.overflow;
      this->integral  += histogram.integral;

      return *this;
    }


    /**
     * Subtract histogram.
     *
     * \param  histogram       histogram
     * \return                 this histogram
     */
    JHistogram& sub(const JHistogram& histogram)
    {
      this->underflow -= histogram.underflow;
      this->overflow  -= histogram.overflow;
      this->integral  -= histogram.integral;

      return *this;
    }


    /**
     * Scale histogram.
     *
     * \param  value           multiplication factor
     * \return                 this histogram
     */
    JHistogram& mul(const double value)
    {
      this->underflow *= value;
      this->overflow  *= value;
      this->integral  *= value;

      return *this;
    }


    /**
     * Scale histogram.
     *
     * \param  value           division factor
     * \return                 this histogram
     */
    JHistogram& div(double value)
    {
      this->underflow /= value;
      this->overflow  /= value;
      this->integral  /= value;

      return *this;
    }


    /**
     * Get contents below lower limit.
     *
     * \return                 contents
     */
    const contents_type& getUnderflow() const
    {
      return underflow;
    }

    
    /**
     * Get contents above upper limit.
     *
     * \return                 contents
     */
    const contents_type& getOverflow() const
    {
      return overflow;
    }

    
    /**
     * Get contents above upper limit.
     *
     * \return                 contents
     */
    const contents_type& getIntegral() const
    {
      return integral;
    }


    /**
     * Read histogram from input.
     *
     * \param  in              reader
     * \param  object          histogram
     * \return                 reader
     */
    friend inline JReader& operator>>(JReader& in, JHistogram& object)
    {
      in >> object.underflow;
      in >> object.overflow;
      in >> object.integral;

      return in;
    }


    /**
     * Write histogram to output.
     *
     * \param  out             writer
     * \param  object          histogram
     * \return                 writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JHistogram& object)
    {
      out << object.underflow;
      out << object.overflow;
      out << object.integral;

      return out;
    }


  protected:
    contents_type underflow;
    contents_type overflow;
    contents_type integral;
  };


  /**
   * Functional histogram cumulator.
   */
  struct JCumulator {
    /**
     * Default constructor.
     */
    JCumulator()
    {}


    /**    
     * Compile function.
     *
     * \param  function          function
     * \return                   this cumulator
     */
    template<class JHistogram_t>
    const JCumulator& operator()(JHistogram_t& function) const
    {
      function.cumlative();

      return *this;
    }
  };

  
  /**
   * Function object for functional object compilation.
   */
  static const JCumulator cumulator;
}

#endif
