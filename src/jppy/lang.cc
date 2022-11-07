#include <iostream>
#include <sstream>
#include <string>

#include <pybind11/pybind11.h>

#include "JLang/JParameter.hh"

#include "utils.hh"

namespace py = pybind11;

namespace {

  template<class T>
  void declare_jparameter(py::module &m, const std::string& class_name) {

    using namespace JPP;
    using namespace std;

    py::class_<JParameter<T> >(m, class_name.c_str())
      .def(py::init<>())
      .def(py::init<const T&>(),
	   py::arg("value"))
      .def("get_value", static_cast<const T& (JParameter<T>::*)() const>(&JParameter<T>::getValue))
      .def("get_value", static_cast<T& (JParameter<T>::*)()>(&JParameter<T>::getValue))
      .def("set_value", &JParameter<T>::setValue,
	   py::arg("value"),
	   py::pos_only())
      .def("is_defined", &JParameter<T>::isDefined)
      .def("less", &JParameter<T>::less,
	   py::arg("parameter"))
      .def("__repr__", &UTILS::get_representation<JParameter<T> >);
  }
}


PYBIND11_MODULE(lang, m) {
  
  m.doc() = "Language auxiliary classes, interfaces and methods specific to Jpp";

  declare_jparameter<int>   (m, "JParameterI");  
  declare_jparameter<float> (m, "JParameterF");
  declare_jparameter<double>(m, "JParameterD");
}
