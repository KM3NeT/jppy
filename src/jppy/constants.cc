#include <pybind11/pybind11.h>

#include "JPhysics/JConstants.hh"

namespace py = pybind11;

PYBIND11_MODULE(constants, m) {
  m.doc() = "Jpp constants";
  m.def("get_speed_of_light", &JPHYSICS::getSpeedOfLight);
  m.def("get_inverse_speed_of_light", &JPHYSICS::getInverseSpeedOfLight);
  m.def("get_index_of_refraction", &JPHYSICS::getIndexOfRefraction);
  m.def("get_index_of_refraction_phase", &JPHYSICS::getIndexOfRefractionPhase);
  m.def("get_tan_theta_c", &JPHYSICS::getTanThetaC);
  m.def("get_cos_theta_c", &JPHYSICS::getCosThetaC);
  m.def("get_sin_theta_c", &JPHYSICS::getSinThetaC);
  m.def("get_kappa_c", &JPHYSICS::getKappaC);
}

