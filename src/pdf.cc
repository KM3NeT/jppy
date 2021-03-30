#include <pybind11/pybind11.h>

#include "JPhysics/JPDF_t.hh"

namespace py = pybind11;

PYBIND11_MODULE(pdf, m) {
    m.doc() = "PDF utilities";
    py::class_<JPDF>(m, "JSinglePDF")
        .def(py::init<const std::string &, double, int, double>(),
             py::arg("file_descriptor"),
             py::arg("TTS"),
             py::arg("number_of_points") = 25,
             py::arg("epsilon") = 1e-10)
        .def("calculate", &JPDF::calculate,
             py::arg("E"),
             py::arg("R"),
             py::arg("theta"),
             py::arg("phi"),
             py::arg("t1")
            ),
    py::class_<JMuonPDF_t>(m, "JMuonPDF")
        .def(py::init<const std::string &, double, int, double>(),
             py::arg("file_descriptor"),
             py::arg("TTS"),
             py::arg("number_of_points") = 25,
             py::arg("epsilon") = 1e-10)
        .def("calculate", &JMuonPDF_t::calculate,
             py::arg("E"),
             py::arg("R"),
             py::arg("theta"),
             py::arg("phi"),
             py::arg("t1")
            ),
    py::class_<JShowerPDF_t>(m, "JShowerPDF")
        .def(py::init<const std::string &, double, int, double>(),
             py::arg("file_descriptor"),
             py::arg("TTS"),
             py::arg("number_of_points") = 25,
             py::arg("epsilon") = 1e-10)
        .def("calculate", &JShowerPDF_t::calculate,
             py::arg("E"),
             py::arg("D"),
             py::arg("cd"),
             py::arg("theta"),
             py::arg("phi"),
             py::arg("t1")
            ),
    py::class_<JTOOLS::JResultPDF<double>>(m, "JResultPDF")
        .def(py::init<double, double, double, double>(),
             py::arg("f"),
             py::arg("fp"),
             py::arg("v"),
             py::arg("V"))
        .def_readonly("f", &JTOOLS::JResultPDF<double>::f)
        .def_readonly("fp", &JTOOLS::JResultPDF<double>::fp)
        .def_readonly("v", &JTOOLS::JResultPDF<double>::v)
        .def_readonly("V", &JTOOLS::JResultPDF<double>::V);
}
