#ifndef __JPHYSICS__JCONSTANTS__
#define __JPHYSICS__JCONSTANTS__

#include <math.h>

#include "JMath/JConstants.hh"


/**
 * \file
 * Physics constants. 
 * \author mdejong
 */
namespace JPHYSICS {}
namespace JPP { using namespace JPHYSICS; }

namespace JPHYSICS {

  using JMATH::PI;
  using JMATH::EULER;

  /**
   * Physics constants.
   */
  static const double C                         = 0.299792458;          //!< Speed of light in vacuum [m/ns]
  static const double C_INVERSE                 = 1.0/C;                //!< Inverse speed of light in vacuum [ns/m]
  static const double AVOGADRO                  = 6.0221415e23;         //!< Avogadro's number [gr^-1]
  static const double NUCLEON_MOLAR_MASS        = 1.0;                  //!< nucleon molar mass [g/mol]
  static const double H                         = 4.13566733e-15;       //!< Planck constant [eV s]
  static const double HBAR                      = H/(2*PI);             //!< Planck constant [eV s]
  static const double HBARC                     = HBAR*C*1.0e9;         //!< Planck constant [eV m]
  static const double ALPHA_ELECTRO_MAGNETIC    = 1.0/137.036;          //!< Electro-Magnetic coupling constant
  static const double THETA_MCS                 = 13.6e-3;              //!< Multiple Coulomb scattering constant [GeV]

  /**
   * Fixed environment values.
   */
  static const double DENSITY_SEA_WATER         = 1.038;                //!< Density  of sea water [g/cm^3]
  static const double DENSITY_ROCK              = 2.65;                 //!< Density  of rock      [g/cm^3]
  static const double SALINITY_SEA_WATER        = 0.035;                //!< Salinity of sea water
  static const double INDEX_OF_REFRACTION_WATER = 1.3800851282;         //!< Average index of refraction of water corresponding to the group velocity 
  static const double X0_WATER_M                = 0.36;                 //!< Radiation length pure water [m]

  /**
   * Derived quantities of optical medium.
   */
  static const double TAN_THETA_C_WATER         = sqrt((INDEX_OF_REFRACTION_WATER - 1.0) * (INDEX_OF_REFRACTION_WATER + 1.0)); //!< Average tangent corresponding to the group velocity 
  static const double COS_THETA_C_WATER         = 1.0 / INDEX_OF_REFRACTION_WATER;                                             //!< Average cosine  corresponding to the group velocity 
  static const double SIN_THETA_C_WATER         = TAN_THETA_C_WATER * COS_THETA_C_WATER;                                       //!< Average sine    corresponding to the group velocity 
  static const double KAPPA_WATER               = 0.96;                                                                        //!< Average R-dependence of arrival time of Cherenkov light

  /**
   * Particle masses.
   * Note that the neutrino masses are set to zero.
   */
  static const double MASS_PHOTON               = 0.0;                 //!< photon     mass [GeV]

  static const double MASS_ELECTRON_NEUTRINO    = 0.0;                 //!< electron neutrino mass [GeV]
  static const double MASS_MUON_NEUTRINO        = 0.0;                 //!< muon     neutrino mass [GeV]
  static const double MASS_TAU_NEUTRINO         = 0.0;                 //!< tau      neutrino mass [GeV]
  
  static const double MASS_ELECTRON             = 0.510998946e-3;      //!< electron       mass [GeV]
  static const double MASS_MUON                 = 0.1056583745;        //!< muon           mass [GeV]
  static const double MASS_TAU                  = 1.77682;             //!< tau            mass [GeV]

  static const double MASS_NEUTRAL_PION         = 0.1349766;           //!< pi_0           mass [GeV]
  static const double MASS_CHARGED_PION         = 0.13957018;          //!< pi^+/-         mass [GeV]
  static const double MASS_NEUTRAL_KAON         = 0.497614;            //!< K_0            mass [GeV]
  static const double MASS_CHARGED_KAON         = 0.493677;            //!< K^+/-          mass [GeV]
  static const double MASS_NEUTRAL_RHO          = 0.77526;             //!< rho_0          mass [GeV]
  static const double MASS_CHARGED_RHO          = 0.77511;             //!< rho^+/-        mass [GeV]
  static const double MASS_NEUTRAL_D            = 1.86483;             //!< D_0            mass [GeV]
  static const double MASS_CHARGED_D            = 1.86965;             //!< D^+/-          mass [GeV]
  static const double MASS_CHARGED_D_S          = 1.96834;             //!< D_s^+/-        mass [GeV]

  static const double MASS_PROTON               = 0.9382720813;        //!< proton         mass [GeV]
  static const double MASS_NEUTRON              = 0.9395654133;        //!< neutron        mass [GeV]
  static const double MASS_DELTA_1232           = 1.232;               //!< Delta (1232)   mass [GeV]
  static const double MASS_LAMBDA               = 1.115683;            //!< Lambda         mass [GeV]
  static const double MASS_NEUTRAL_SIGMA        = 1.192642;            //!< Sigma_0        mass [GeV]
  static const double MASS_CHARGED_SIGMA        = 1.18937;             //!< Sigma^+/-      mass [GeV]
  static const double MASS_NEUTRAL_XI           = 1.31486;             //!< Xi_0           mass [GeV]
  static const double MASS_CHARGED_XI           = 1.32171;             //!< Xi^+/-         mass [GeV]
  static const double MASS_CHARGED_OMEGA        = 1.67245;             //!< Omega^+/-      mass [GeV]
  static const double MASS_CHARGED_LAMBDA_C     = 2.28646;             //!< Lambda_c^+/-   mass [GeV]
  static const double MASS_DOUBLYCHARGED_SIGMA_C = 2.45397;            //!< Sigma_c^++/--  mass [GeV]  
  static const double MASS_CHARGED_SIGMA_C      = 2.4529;              //!< Sigma_c^+/-    mass [GeV]
  static const double MASS_NEUTRAL_SIGMA_C      = 2.45375;             //!< Sigma_c_0      mass [GeV]
  static const double MASS_CHARGED_XI_C         = 2.46793;             //!< Xi_c^+/-       mass [GeV]
  static const double MASS_NEUTRAL_XI_C         = 2.47091;             //!< Xi_c_0         mass [GeV]
  static const double MASS_NEUTRAL_OMEGA_C      = 2.6952;              //!< Omega_c_0      mass [GeV]
  static const double MASS_NEUTRAL_B            = 5.27958;             //!< B_0            mass [GeV]
  static const double MASS_CHARGED_B            = 5.27926;             //!< B^+/-          mass [GeV]
  static const double MASS_NEUTRAL_B_S          = 5.36677;             //!< B_s^0          mass [GeV]
  static const double MASS_NEUTRAL_LAMBDA_B     = 5.6194;              //!< Lambda_b^0     mass [GeV]
  static const double MASS_NEUTRAL_XI_B         = 5.7878;              //!< Xi_b^0         mass [GeV]
  static const double MASS_CHARGED_XI_B         = 5.7911;              //!< Xi_b^+/-       mass [GeV]
  static const double MASS_CHARGED_OMEGA_B      = 6.071;               //!< Omega_b^+/-    mass [GeV]
  static const double MASS_CHARGED_B_C          = 6.2756;              //!< B_c^+/-        mass [GeV]
  
  /** 
   * Get speed of light.
   * 
   * return           speed of light [m/ns]
   */
  inline const double getSpeedOfLight()
  {
    return C;
  }


  /**
   * Get inverse speed of light.
   *
   * return           inverse speed of light [ns/m]
   */
  inline const double getInverseSpeedOfLight()
  { 
    return C_INVERSE; 
  }


  /**
   * Get average index of refraction of water corresponding to group velocity.
   *
   * \return          index of refraction
   */
  inline double getIndexOfRefraction() 
  {
    return INDEX_OF_REFRACTION_WATER;
  }


  /**
   * Get average index of refraction of water corresponding to phase velocity.
   *
   * \return          index of refraction
   */
  inline double getIndexOfRefractionPhase() 
  {
    return 1.35;
  }


  /**
   * Get average tangent of Cherenkov angle of water corresponding to group velocity.
   *
   * \return          tan(theta_C)
   */
  inline double getTanThetaC() 
  {
    return TAN_THETA_C_WATER;
  }


  /**
   * Get average cosine of Cherenkov angle of water corresponding to group velocity.
   *
   * \return          cos(theta_C)
   */
  inline double getCosThetaC() 
  {
    return COS_THETA_C_WATER;
  }


  /**
   * Get average sine of Cherenkov angle of water corresponding to group velocity.
   *
   * \return          sin(theta_C)
   */
  inline double getSinThetaC() 
  {
    return SIN_THETA_C_WATER;
  }


  /**
   * Get average R-dependence of arrival time of Cherenkov light (a.k.a. kappa).
   *
   * \return          kappa
   */
  inline double getKappaC() 
  {
    return KAPPA_WATER;
  }
}

#endif
