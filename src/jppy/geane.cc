#include <pybind11/pybind11.h>

#include "JPhysics/JGeane.hh"

namespace py = pybind11;

PYBIND11_MODULE(geane, m) {
  m.doc() = "Utilities for muon energy losses";
  m.def("geanc", &JPHYSICS::geanc);
  py::class_<JPHYSICS::JGeane>(m, "JGeane");
  py::class_<JPHYSICS::JGeane_t, JPHYSICS::JGeane>(m, "JGeane_t")
    .def(py::init<const double, const double>(),
	 py::arg("a"),
	 py::arg("b"))
    .def("get_a", &JPHYSICS::JGeane_t::getA)
    .def("get_b", &JPHYSICS::JGeane_t::getB)
    .def("get_E", &JPHYSICS::JGeane_t::getE,
	 py::arg("E"),
	 py::arg("dx"))
    .def("get_X", &JPHYSICS::JGeane_t::getX,
	 py::arg("E0"),
	 py::arg("E1")
	 );
  py::class_<JPHYSICS::JGeaneWater, JPHYSICS::JGeane>(m, "JGeaneWater")
    .def(py::init<>())
    .def("get_a", &JPHYSICS::JGeaneWater::getA)
    .def("get_b", &JPHYSICS::JGeaneWater::getB)
    .def("get_E", &JPHYSICS::JGeaneWater::getE,
	 py::arg("E"),
	 py::arg("dx"))
    .def("get_X", &JPHYSICS::JGeaneWater::getX,
	 py::arg("E0"),
	 py::arg("E1")
	 );
}
