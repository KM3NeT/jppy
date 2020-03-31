#include <pybind11/pybind11.h>

#include "JPhysics/JNPE_t.hh"

namespace py = pybind11;

PYBIND11_MODULE(npe, m) {
    m.doc() = "NPE utilities";
    py::class_<JMuonNPE_t>(m, "JMuonNPE")
        .def(py::init<const std::string &>(),
             py::arg("file_descriptor"))
        .def("calculate", &JMuonNPE_t::calculate,
             py::arg("E"),
             py::arg("R"),
             py::arg("theta"),
             py::arg("phi")
            ),
    py::class_<JShowerNPE_t>(m, "JShowerNPE")
        .def(py::init<const std::string &, int>(),
             py::arg("file_descriptor"),
             py::arg("number_of_points") = 0)
        .def("calculate", &JShowerNPE_t::calculate,
             py::arg("E"),
             py::arg("D"),
             py::arg("cd"),
             py::arg("theta"),
             py::arg("phi")
             );
}
