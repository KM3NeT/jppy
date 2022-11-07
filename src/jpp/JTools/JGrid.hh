#ifndef __JTOOLS__JGRID__
#define __JTOOLS__JGRID__

#include <istream>
#include <ostream>

#include "JLang/JClass.hh"

#include "JIO/JSerialisable.hh"

#include "JTools/JAbstractCollection.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JIO::JReader;
  using JIO::JWriter;
  
  using JLANG::JClass;

  template<class JElement_t, class JDistance_t>
  class JCollection;

  
  /**
   * Simple data structure for an abstract collection of equidistant abscissa values.
   *
   * This class implements the JAbstractCollection interface.
   */
  template<class JAbscissa_t>
  struct JGrid : 
    public JAbstractCollection<JAbscissa_t>
  {
    typedef typename JAbstractCollection<JAbscissa_t>::abscissa_type      abscissa_type;


    /**
     * Default constructor.
     */
    JGrid() :
      size(0),
      xmin(),
      xmax()
    {}


    /**
     * Constructor.
     *
     * \param  nx              number of elements
     * \param  Xmin            lower limit
     * \param  Xmax            upper limit
     */
    JGrid(const int           nx,
	  const abscissa_type Xmin,
	  const abscissa_type Xmax) :
      size(nx),
      xmin(Xmin),
      xmax(Xmax)
    {}


    /**
     * Get number of elements.
     *
     * \return                 number of elements
     */
    virtual int getSize() const override 
    { 
      return size;
    }
    

    /**
     * Get abscissa value.
     *
     * \param  index           index
     * \return                 abscissa value
     */
    virtual abscissa_type getX(int index) const override 
    {
      return (index == 0 ? xmin : xmin + index * ((xmax - xmin) / (size - 1)));
    }

    
    /**
     * Get minimal abscissa value.
     *
     * \return                 abscissa value
     */
    virtual abscissa_type getXmin() const override 
    {
      return xmin;
    }


    /**
     * Get maximal abscissa value.
     *
     * \return                 abscissa value
     */
    virtual abscissa_type getXmax() const override 
    {
      return xmax;
    }    

         
    /**
     * Get index of given abscissa value.
     *
     * \param  x               abscissa value
     * \return                 index
     */
    int getIndex(typename JClass<abscissa_type>::argument_type x) const
    {
      return (int) ((size - 1) * (x - xmin) / (xmax - xmin));
    }


    /**    
     * Configure collection.
     *
     * \param  collection        collection
     * \return                   this grid
     */
    template<class JElement_t, class JDistance_t>
    const JGrid& operator()(JCollection<JElement_t, JDistance_t>& collection) const
    {
      collection.configure(*this);

      return *this;
    }


    /**
     * Binary stream input.
     *
     * \param  in              input stream
     * \param  grid            grid
     * \return                 input stream
     */
    friend inline JReader& operator>>(JReader& in, JGrid<JAbscissa_t>& grid)
    {
      return in >> grid.size >> grid.xmin >> grid.xmax;
    }


    /**
     * Binary stream output.
     *
     * \param  out             output stream
     * \param  grid            grid
     * \return                 output stream
     */
    friend inline JWriter& operator<<(JWriter& out, const JGrid<JAbscissa_t>& grid)
    {
      return out << grid.size << grid.xmin << grid.xmax;
    }    
    
    
    /**
     * Read grid from input.
     *
     * \param  in       input stream
     * \param  grid     grid
     * \return          input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JGrid<JAbscissa_t>& grid)
    {
      return in >> grid.size >> grid.xmin >> grid.xmax;
    }


    /**
     * Write grid to output.
     *
     * \param  out      output stream
     * \param  grid     grid
     * \return          output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JGrid<JAbscissa_t>& grid)
    {
      return out << grid.size << ' ' << grid.xmin << ' ' << grid.xmax;
    }

  protected:
    int           size;
    abscissa_type xmin;
    abscissa_type xmax;
  };


  /**
   * Helper method for JGrid.
   *
   * \param  nx              number of elements
   * \param  Xmin            lower limit
   * \param  Xmax            upper limit
   * \return                 grid
   */
  template<class JAbscissa_t>
  inline JGrid<JAbscissa_t> make_grid(const int         nx,
				      const JAbscissa_t Xmin,
				      const JAbscissa_t Xmax)
  {
    return JGrid<JAbscissa_t>(nx, Xmin, Xmax);
  }


  /**
   * Helper method to create a grid with a single value.
   *
   * \param  value           value
   * \return                 single-valued grid
   */
  template<class JAbscissa_t>
  inline JGrid<JAbscissa_t> make_grid(const JAbscissa_t value)
  {
    return JGrid<JAbscissa_t>(1, value, value);
  }  
}

#endif
