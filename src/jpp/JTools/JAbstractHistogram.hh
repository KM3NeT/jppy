#ifndef __JTOOLS__JABSTRACTHISTOGRAM__
#define __JTOOLS__JABSTRACTHISTOGRAM__

#include <istream>
#include <ostream>

#include "JTools/JRange.hh"
#include "JTools/JGrid.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  /**
   * Simple data structure for histogram binning.
   */
  template<class JAbscissa_t>
  struct JAbstractHistogram :
    public JRange<JAbscissa_t>
  {

    typedef JAbscissa_t               abscissa_type;
    typedef JRange<abscissa_type>     range_type;


    /**
     * Default constructor.
     */
    JAbstractHistogram() :
      JRange<JAbscissa_t>(),
      number_of_bins(0)
    {}


    /**
     * Constructor.
     *
     * \param  nx              number of bins
     * \param  xmin            lower limit
     * \param  xmax            upper limit
     */
    JAbstractHistogram(const int           nx,
		       const abscissa_type xmin,
		       const abscissa_type xmax) :
      JRange<JAbscissa_t>(xmin, xmax),
      number_of_bins(nx)
    {}


    /**
     * Constructor.
     *
     * \param  xmin            lower limit
     * \param  xmax            upper limit
     */
    JAbstractHistogram(const abscissa_type xmin,
		       const abscissa_type xmax) :
      JRange<JAbscissa_t>(xmin, xmax),
      number_of_bins(0)
    {}


    /**
     * Get number of bins.
     *
     * \return                 number of bins
     */
    int getNumberOfBins() const
    { 
      return number_of_bins;
    }


    /**
     * Get bin width.
     *
     * \return                 bin width
     */
    double getBinWidth() const
    { 
      return this->getLength() / this->getNumberOfBins();
    }


    /**
     * Set bin width.
     *
     * If <tt>option < 0</tt>, adjust lower limit; if <tt>option > 0</tt>, adjust upper limit; else no adjustments.
     *
     * \param  dx              bin width
     * \param  option          option
     */
    void setBinWidth(const abscissa_type dx, int option = 0)
    { 
      number_of_bins = (int) (this->getLength() / dx);

      if (option < 0) { this->setLowerLimit(this->getUpperLimit() - number_of_bins + dx); }
      if (option > 0) { this->setUpperLimit(this->getLowerLimit() + number_of_bins + dx); }
    }


    /**
     * Check validity of histogram binning.
     *
     * \return                 true if both range and number of bins are valid; else false
     */
    bool is_valid() const
    {
      return static_cast<const range_type&>(*this).is_valid() && number_of_bins > 0;
    }


    /**
     * Type conversion operator.
     *
     * \return                 grid
     */
    operator JGrid<abscissa_type> () const
    {
      return make_grid(this->getNumberOfBins() + 1, this->getLowerLimit(), this->getUpperLimit());
    }

    
    /**
     * Read histogram from input.
     *
     * \param  in              input stream
     * \param  histogram       histogram
     * \return                 input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JAbstractHistogram<JAbscissa_t>& histogram)
    {
      return in >> histogram.number_of_bins >> static_cast<range_type&>(histogram);
    }


    /**
     * Write histogram to output.
     *
     * \param  out             output stream
     * \param  histogram       histogram
     * \return                 output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JAbstractHistogram<JAbscissa_t>& histogram)
    {
      return out << histogram.number_of_bins << ' ' << static_cast<const range_type&>(histogram);
    }

  protected:
    int number_of_bins;
  };


  /**
   * Helper method for JAbstractHistogram.
   *
   * \param  nx              number of bins
   * \param  xmin            lower limit
   * \param  xmax            upper limit
   * \return                 histogram
   */
  template<class JAbscissa_t>
  inline JAbstractHistogram<JAbscissa_t> make_histogram(const int         nx,
							const JAbscissa_t xmin,
							const JAbscissa_t xmax)
  {
    return JAbstractHistogram<JAbscissa_t>(nx, xmin, xmax);
  }
}

#endif
