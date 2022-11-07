#ifndef __JPHYSICS__JPDFTYPES__
#define __JPHYSICS__JPDFTYPES__

#include <string>
#include <sstream>

#include "Jeep/JeepToolkit.hh"

/**
 * \file
 *
 * Numbering scheme for PDF types.
 * \author mdejong
 */
namespace JPHYSICS {}
namespace JPP { using namespace JPHYSICS; }

namespace JPHYSICS {


  /**
   * PDF types
   */
  enum JPDFType_t {

    DIRECT_LIGHT_FROM_MUON             =    1,      //!< direct    light from muon
    SCATTERED_LIGHT_FROM_MUON          =    2,      //!< scattered light from muon

    DIRECT_LIGHT_FROM_EMSHOWERS        =    3,      //!< direct    light from EM showers
    SCATTERED_LIGHT_FROM_EMSHOWERS     =    4,      //!< scattered light from EM showers

    DIRECT_LIGHT_FROM_DELTARAYS        =    5,      //!< direct    light from delta-rays
    SCATTERED_LIGHT_FROM_DELTARAYS     =    6,      //!< scattered light from delta-rays

    SCATTERED_LIGHT_FROM_MUON_5D       =   12,      //!< scattered light from muon

    DIRECT_LIGHT_FROM_EMSHOWER         =   13,      //!< direct    light from EM shower
    SCATTERED_LIGHT_FROM_EMSHOWER      =   14,      //!< scattered light from EM shower

    //DIRECT_LIGHT_FROM_SHOWER           =   15,      //!< direct + scattered light from HADRONIC shower

    DIRECT_LIGHT_FROM_BRIGHT_POINT     =   23,      //!< direct    light from bright point
    SCATTERED_LIGHT_FROM_BRIGHT_POINT  =   24,      //!< scattered light from bright point

    LIGHT_FROM_ELONGATED_EMSHOWER      =  113,      //!< light from elongated EM shower

    LIGHT_FROM_MUON                    = 1001,      //!< direct and scattered light from muon
    LIGHT_FROM_EMSHOWERS               = 1003,      //!< direct and scattered light from EM showers
    LIGHT_FROM_DELTARAYS               = 1005,      //!< direct and scattered light from delta-rays
    LIGHT_FROM_EMSHOWER                = 1013,      //!< direct and scattered light from EM shower
    LIGHT_FROM_BRIGTH_POINT            = 1023       //!< direct and scattered light from brigth point
  };


  /**
   * Get PDF label.
   *
   * \param  pdf                PDF type
   * \return                    PDF label
   */
  inline std::string getLabel(const JPDFType_t pdf)
  {
    std::ostringstream os;

    os << pdf;

    return os.str();
  }


  /**
   * Get PDF type.
   *
   * \param  file_name          file name
   * \return                    PDF type (-1 in case of error)
   */
  inline int getPDFType(const std::string& file_name)
  {
    using namespace std;

    static const char* digits = "0123456789";
    
    int type = -1;
    
    string buffer = JEEP::getFilename(file_name);
    
    string::size_type pos = buffer.find_first_of(digits);

    if (pos != string::npos) {

      string::size_type len = buffer.substr(pos).find_first_not_of(digits);

      istringstream(buffer.substr(pos, len)) >> type;
    }

    return type;
  }


  /**
   * Get PDF file name.
   *
   * The input file name should contain the wild card character JEEP::FILENAME_WILD_CARD 
   * which will be replaced by the label corresponding to the given PDF type.
   * 
   * \param  file_name          input  file name
   * \param  pdf                PDF type
   * \return                    output file name
   */
  inline std::string getFilename(const std::string& file_name, 
				 const JPDFType_t   pdf)
  {
    return JEEP::setWildCard(file_name, getLabel(pdf));
  }


  /**
   * Test if given PDF type corresponds to Cherenkov light from muon.
   *
   * \param  pdf                PDF type
   * \return                    true if PDF corresponds to muon; else false
   */
  inline bool is_muon(const int pdf)
  {
    return (pdf == DIRECT_LIGHT_FROM_MUON           ||
	    pdf == SCATTERED_LIGHT_FROM_MUON        ||
	    pdf == LIGHT_FROM_MUON);
  }


  /**
   * Test if given PDF type corresponds to Cherenkov light from Bremsstrahlung.
   *
   * \param  pdf                PDF type
   * \return                    true if PDF corresponds to Bremsstrahlung; else false
   */
  inline bool is_bremsstrahlung(const int pdf)
  {
    return (pdf == DIRECT_LIGHT_FROM_EMSHOWERS      ||
	    pdf == SCATTERED_LIGHT_FROM_EMSHOWERS   ||
	    pdf == LIGHT_FROM_EMSHOWERS);
  }


  /**
   * Test if given PDF type corresponds to Cherenkov light from delta-rays.
   *
   * \param  pdf                PDF type
   * \return                    true if PDF corresponds to delta-rays; else false
   */
  inline bool is_deltarays(const int pdf)
  {
    return (pdf == DIRECT_LIGHT_FROM_DELTARAYS      ||
	    pdf == SCATTERED_LIGHT_FROM_DELTARAYS   ||
	    pdf == LIGHT_FROM_DELTARAYS);
  }


  /**
   * Test if given PDF type corresponds to scattered light.
   *
   * \param  pdf                PDF type
   * \return                    true if PDF corresponds to scattered light; else false
   */
  inline bool is_scattered(const int pdf)
  {
    return (pdf == SCATTERED_LIGHT_FROM_MUON        ||
	    pdf == SCATTERED_LIGHT_FROM_EMSHOWERS   ||
	    pdf == SCATTERED_LIGHT_FROM_DELTARAYS   ||
	    pdf == SCATTERED_LIGHT_FROM_MUON_5D     ||
	    pdf == SCATTERED_LIGHT_FROM_EMSHOWER);
  }
}

#endif
