#ifndef __JOSCPROB__JOSCPARAMETERSGRID__
#define __JOSCPROB__JOSCPARAMETERSGRID__

#include "JLang/JException.hh"

#include "JTools/JGrid.hh"

#include "JOscProb/JOscParametersInterface.hh"


/**
 * \author bjung, mdejong
 */

namespace JOSCPROB {}
namespace JPP { using namespace JOSCPROB; }

namespace JOSCPROB {

  using JTOOLS::JGrid;
  using JTOOLS::make_grid;
  

  /**
   * Data structure for oscillation parameter grids.
   */
  struct JOscParametersGrid :
    public JOscParametersInterface<JGrid<double> >
  {
    typedef JGrid<double>                                                      JGrid_t;
    typedef JOscParametersInterface<JGrid_t>                          JOscParameters_t;
    typedef typename JOscParameters_t::JParameter_t                       JParameter_t;
    typedef typename JOscParameters_t::argument_type                     argument_type;

    
    /**
     * Default constructor.
     */
    JOscParametersGrid() :
      JOscParameters_t()
    {}


    /**
     * Constructor.
     * 
     * \param  dM21sq          Squared mass difference between the first and second neutrino mass eigenstates               [eV2]
     * \param  dM31sq          Squared mass difference between the first and third neutrino mass eigenstates                [eV2]
     * \param  deltaCP         PMNS phase angle                                                                             [pi rad]
     * \param  sinsqTh12       Squared sine of the PMNS mixing angle between the first and second neutrino mass eigenstates [-]
     * \param  sinsqTh13       Squared sine of the PMNS mixing angle between the first and third neutrino mass eigenstates  [-]
     * \param  sinsqTh23       Squared sine of the PMNS mixing angle between the second and third neutrino mass eigenstates [-]
     */
    JOscParametersGrid(const JGrid_t& dM21sq,
		       const JGrid_t& dM31sq,
		       const JGrid_t& deltaCP,
		       const JGrid_t& sinsqTh12,
		       const JGrid_t& sinsqTh13,
		       const JGrid_t& sinsqTh23) :
      JOscParameters_t(dM21sq,
		       dM31sq,
		       deltaCP,
		       sinsqTh12,
		       sinsqTh13,
		       sinsqTh23)
    {
      if (!is_valid()) {
	THROW(JLANG::JValueOutOfRange, "JOscParametersGrid::JOscParametersGrid(...): Invalid parameters " << *this);
      }
    }


    /**
     * Constructor.
     * 
     * \param  name            parameter name
     * \param  value           parameter value
     * \param  args            remaining pairs of parameter names and values
     */
    template<class ...Args>
    JOscParametersGrid(const std::string& name,
		       const JGrid_t&     value,
		       const Args&     ...args) :
      JOscParameters_t(name, value, args...)      
    {
      if (!is_valid()) {
	THROW(JLANG::JValueOutOfRange, "JOscParametersGrid::JOscParametersGrid(...): Invalid parameters " << *this);
      }
    }


    /**
     * Constructor.
     * 
     * Values taken from the NuFIT 5.1 three-flavour global analysis best fit values in:\n
     * https://arxiv.org/abs/2111.03086?context=hep-ex\n
     * including the Super-Kamiokande atmospheric data.
     *
     * \param  useIO           toggle inverted ordering
     */
    JOscParametersGrid(const bool useIO) :
      JOscParameters_t( make_grid(         7.42e-5                       ),
			make_grid(useIO ? -2.490e-3 + 7.42e-5 : 2.510e-3 ),
			make_grid(useIO ?  1.544              : 1.278    ),
			make_grid(         0.304                         ),
			make_grid(useIO ?  0.02241            : 0.02246  ),
			make_grid(useIO ?  0.570              : 0.450)     )
    {}
   

    /**
     * Check validity of oscillation parameter grids.
     *
     * \return                 true if valid; else false
     */
    bool is_valid() const override
    {
      if ((this->sinsqTh12.isDefined() && this->sinsqTh12.getValue().getXmin() < 0.0) ||
	  (this->sinsqTh13.isDefined() && this->sinsqTh13.getValue().getXmin() < 0.0) ||
	  (this->sinsqTh23.isDefined() && this->sinsqTh23.getValue().getXmin() < 0.0)) {
	return false;
      }
      
      return true;
    }
  };
}

#endif
