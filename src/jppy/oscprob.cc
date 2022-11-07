#include <iostream>
#include <sstream>
#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>

#include "JOscProb/JOscChannel.hh"
#include "JOscProb/JOscParametersInterface.hh"
#include "JOscProb/JOscParameters.hh"
#include "JOscProb/JOscProbToolkit.hh"
#include "JOscProb/JBaselineCalculator.hh"
#include "JOscProb/JOscProbInterpolator.hh"

#include "JppyOscProbInterpolator.hh"

#include "utils.hh"

namespace py = pybind11;

namespace {

  template<class T>
  class JppyOscParametersInterface :
    public JOSCPROB::JOscParametersInterface<T>
  {
    using JOSCPROB::JOscParametersInterface<T>::JOscParametersInterface; // Inherit constructors

    bool is_valid() const override { // Trampoline (needed for virtual functions)
      PYBIND11_OVERRIDE_PURE(
			     bool,
			     JOSCPROB::JOscParametersInterface<T>,
			     is_valid
			     );
    }
  };
}


PYBIND11_MODULE(oscprob, m) {
  
  m.doc() = "Oscillation probability interpolation utilities";

  py::class_<JOSCPROB::JOscParametersInterface<double>, JppyOscParametersInterface<double> >(m, "JOscParametersInterfaceD")
    .def(py::init<>())
    .def(py::init<const double, const double, const double, const double, const double, const double>(),
	 py::arg("dM21sq"),
	 py::arg("dM31sq"),
	 py::arg("deltaCP"),
	 py::arg("sinsqTh12"),
	 py::arg("sinsqTh13"),
	 py::arg("sinsqTh23"))
    .def(py::init<
	 const std::string&, const double&>(),
	 py::arg("name"),
	 py::arg("value"),
	 py::pos_only())	 
    .def(py::init<
	 const std::string&, const double&,
	 const std::string&, const double&>(),
	 py::arg("name1"),
	 py::arg("value1"),
	 py::arg("name2"),
	 py::arg("value2"),
	 py::pos_only())	 
    .def(py::init<
	 const std::string&, const double&,
	 const std::string&, const double&,	 
	 const std::string&, const double&>(),
	 py::arg("name1"),
	 py::arg("value1"),
	 py::arg("name2"),
	 py::arg("value2"),
	 py::arg("name3"),
	 py::arg("value3"),
	 py::pos_only())	 
    .def(py::init<
	 const std::string&, const double&,
	 const std::string&, const double&,
	 const std::string&, const double&,	 	 
	 const std::string&, const double&>(),
	 py::arg("name1"),
	 py::arg("value1"),
	 py::arg("name2"),
	 py::arg("value2"),
	 py::arg("name3"),
	 py::arg("value3"),
	 py::arg("name4"),
	 py::arg("value4"),
	 py::pos_only())	 
    .def(py::init<
	 const std::string&, const double&,
	 const std::string&, const double&,
	 const std::string&, const double&,	 
	 const std::string&, const double&,	 	 
	 const std::string&, const double&>(),
	 py::arg("name1"),
	 py::arg("value1"),
	 py::arg("name2"),
	 py::arg("value2"),
	 py::arg("name3"),
	 py::arg("value3"),
	 py::arg("name4"),
	 py::arg("value4"),
	 py::arg("name5"),
	 py::arg("value5"),
	 py::pos_only())	 
    .def(py::init<
	 const std::string&, const double&,
	 const std::string&, const double&,
	 const std::string&, const double&,
	 const std::string&, const double&,
	 const std::string&, const double&,
	 const std::string&, const double&>(),
	 py::arg("name1"),
	 py::arg("value1"),
	 py::arg("name2"),
	 py::arg("value2"),
	 py::arg("name3"),
	 py::arg("value3"),
	 py::arg("name4"),
	 py::arg("value4"),
	 py::arg("name5"),
	 py::arg("value5"),
	 py::arg("name6"),
	 py::arg("value6"),
	 py::pos_only())	 
    .def("set", static_cast<void (JOSCPROB::JOscParametersInterface<double>::*)(const std::string&, const double&)>(&JOSCPROB::JOscParametersInterface<double>::set),
	 py::arg("name"),
	 py::arg("value"),
	 py::pos_only())	 
    .def("set", static_cast<void (JOSCPROB::JOscParametersInterface<double>::*)(const std::string&, const double&,
							       const std::string&, const double&)>(&JOSCPROB::JOscParametersInterface<double>::set),
	 py::arg("name1"),
	 py::arg("value1"),
	 py::arg("name2"),
	 py::arg("value2"),
	 py::pos_only())	 
    .def("set", static_cast<void (JOSCPROB::JOscParametersInterface<double>::*)(const std::string&, const double&,
							       const std::string&, const double&,
							       const std::string&, const double&)>(&JOSCPROB::JOscParametersInterface<double>::set),
	 py::arg("name1"),
	 py::arg("value1"),
	 py::arg("name2"),
	 py::arg("value3"),
	 py::arg("name3"),
	 py::arg("value3"),
	 py::pos_only())	 
    .def("set", static_cast<void (JOSCPROB::JOscParametersInterface<double>::*)(const std::string&, const double&,
							       const std::string&, const double&,
							       const std::string&, const double&,							       
							       const std::string&, const double&)>(&JOSCPROB::JOscParametersInterface<double>::set),
	 py::arg("name1"),
	 py::arg("value1"),
	 py::arg("name2"),
	 py::arg("value3"),
	 py::arg("name3"),
	 py::arg("value3"),
	 py::arg("name4"),
	 py::arg("value4"),
	 py::pos_only())	 
    .def("set", static_cast<void (JOSCPROB::JOscParametersInterface<double>::*)(const std::string&, const double&,
							       const std::string&, const double&,
							       const std::string&, const double&,							       
							       const std::string&, const double&,							       
							       const std::string&, const double&)>(&JOSCPROB::JOscParametersInterface<double>::set),
	 py::arg("name1"),
	 py::arg("value1"),
	 py::arg("name2"),
	 py::arg("value3"),
	 py::arg("name3"),
	 py::arg("value3"),
	 py::arg("name4"),
	 py::arg("value4"),
	 py::arg("name5"),
	 py::arg("value5"),
	 py::pos_only())
    .def("set", static_cast<void (JOSCPROB::JOscParametersInterface<double>::*)(const std::string&, const double&,
							       const std::string&, const double&,
							       const std::string&, const double&,
							       const std::string&, const double&,					   
							       const std::string&, const double&,							       
							       const std::string&, const double&)>(&JOSCPROB::JOscParametersInterface<double>::set),
	 py::arg("name1"),
	 py::arg("value1"),
	 py::arg("name2"),
	 py::arg("value3"),
	 py::arg("name3"),
	 py::arg("value3"),
	 py::arg("name4"),
	 py::arg("value4"),
	 py::arg("name5"),
	 py::arg("value5"),
	 py::arg("name6"),
	 py::arg("value6"),
	 py::pos_only())
    .def("join", &JOSCPROB::JOscParametersInterface<double>::join)
    .def("is_valid",&JOSCPROB::JOscParametersInterface<double>::is_valid)
    .def("size", &JOSCPROB::JOscParametersInterface<double>::size)
    .def("contains", &JOSCPROB::JOscParametersInterface<double>::contains,
	 py::arg("parameters"))
    .def("equals", &JOSCPROB::JOscParametersInterface<double>::equals,
	 py::arg("parameters"))
    .def("__eq__", [](const JOSCPROB::JOscParametersInterface<double>& first,
		      const JOSCPROB::JOscParametersInterface<double>& second) { return first == second; } )
    .def("__ne__", [](const JOSCPROB::JOscParametersInterface<double>& first,
		      const JOSCPROB::JOscParametersInterface<double>& second) { return first != second; } )
    .def("__repr__", &UTILS::get_representation<JOSCPROB::JOscParametersInterface<double>>)
    .def_readwrite("dM21sq", &JOSCPROB::JOscParametersInterface<double>::dM21sq)
    .def_readwrite("dM31sq", &JOSCPROB::JOscParametersInterface<double>::dM31sq)
    .def_readwrite("deltaCP", &JOSCPROB::JOscParametersInterface<double>::deltaCP)
    .def_readwrite("sinsqTh12", &JOSCPROB::JOscParametersInterface<double>::sinsqTh12)
    .def_readwrite("sinsqTh13", &JOSCPROB::JOscParametersInterface<double>::sinsqTh13)
    .def_readwrite("sinsqTh23", &JOSCPROB::JOscParametersInterface<double>::sinsqTh23);

  py::class_<JOSCPROB::JOscParameters, JOSCPROB::JOscParametersInterface<double> >(m, "JOscParameters")
    .def(py::init<const double, const double, const double, const double, const double, const double>(),
	 py::arg("dM21sq"),
	 py::arg("dM31sq"),
	 py::arg("deltaCP"),
	 py::arg("sinsqTh12"),
	 py::arg("sinsqTh13"),
	 py::arg("sinsqTh23"))
    .def(py::init<const bool>(),
	 py::arg("useIO"))
    .def(py::init<
	 const std::string&, const double&>(),
	 py::arg("name"),
	 py::arg("value"),
	 py::pos_only())	 
    .def(py::init<
	 const std::string&, const double&,
	 const std::string&, const double&>(),
	 py::arg("name1"),
	 py::arg("value1"),
	 py::arg("name2"),
	 py::arg("value2"),
	 py::pos_only())	 
    .def(py::init<
	 const std::string&, const double&,
	 const std::string&, const double&,	 
	 const std::string&, const double&>(),
	 py::arg("name1"),
	 py::arg("value1"),
	 py::arg("name2"),
	 py::arg("value2"),
	 py::arg("name3"),
	 py::arg("value3"),
	 py::pos_only())	 
    .def(py::init<
	 const std::string&, const double&,
	 const std::string&, const double&,
	 const std::string&, const double&,	 	 
	 const std::string&, const double&>(),
	 py::arg("name1"),
	 py::arg("value1"),
	 py::arg("name2"),
	 py::arg("value2"),
	 py::arg("name3"),
	 py::arg("value3"),
	 py::arg("name4"),
	 py::arg("value4"),
	 py::pos_only())	 
    .def(py::init<
	 const std::string&, const double&,
	 const std::string&, const double&,
	 const std::string&, const double&,	 
	 const std::string&, const double&,	 	 
	 const std::string&, const double&>(),
	 py::arg("name1"),
	 py::arg("value1"),
	 py::arg("name2"),
	 py::arg("value2"),
	 py::arg("name3"),
	 py::arg("value3"),
	 py::arg("name4"),
	 py::arg("value4"),
	 py::arg("name5"),
	 py::arg("value5"),
	 py::pos_only())	 
    .def(py::init<
	 const std::string&, const double&,
	 const std::string&, const double&,
	 const std::string&, const double&,
	 const std::string&, const double&,
	 const std::string&, const double&,
	 const std::string&, const double&>(),
	 py::arg("name1"),
	 py::arg("value1"),
	 py::arg("name2"),
	 py::arg("value2"),
	 py::arg("name3"),
	 py::arg("value3"),
	 py::arg("name4"),
	 py::arg("value4"),
	 py::arg("name5"),
	 py::arg("value5"),
	 py::arg("name6"),
	 py::arg("value6"),
	 py::pos_only())	 
    .def("is_valid",&JOSCPROB::JOscParameters::is_valid);
    
  py::enum_<JOSCPROB::JFlavour_t>(m, "JFlavour_t")
    .value("ELECTRON",JOSCPROB::JFlavour_t::ELECTRON)
    .value("MUON",JOSCPROB::JFlavour_t::MUON)
    .value("TAU",JOSCPROB::JFlavour_t::TAU)
    .value("FLAVOUR_UNDEFINED",JOSCPROB::JFlavour_t::FLAVOUR_UNDEFINED);
    
  py::enum_<JOSCPROB::JChargeParity_t>(m, "JChargeParity_t")
    .value("ANTIPARTICLE",JOSCPROB::JChargeParity_t::ANTIPARTICLE)
    .value("PARTICLE",JOSCPROB::JChargeParity_t::PARTICLE)
    .value("CPARITY_UNDEFINED",JOSCPROB::JChargeParity_t::CPARITY_UNDEFINED);

  py::enum_<JOSCPROB::OscProbFlavour_t>(m, "OscProbFlavour_t")
    .value("ELECTRON",JOSCPROB::OscProbFlavour_t::ELECTRON)
    .value("MUON",JOSCPROB::OscProbFlavour_t::MUON)
    .value("TAU",JOSCPROB::OscProbFlavour_t::TAU);

  m.def("get_flavour", static_cast<JOSCPROB::JFlavour_t (*)(const int)>(&JOSCPROB::getFlavour));
  m.def("get_charge_parity", static_cast<JOSCPROB::JChargeParity_t (*)(const int)>(&JOSCPROB::getChargeParity));
  m.def("get_oscprob_flavour", static_cast<JOSCPROB::OscProbFlavour_t (*)(const int)>(&JOSCPROB::getOscProbFlavour));
    
  py::class_<JOSCPROB::JOscChannel>(m, "JOscChannel")
    .def(py::init<const JOSCPROB::JFlavour_t,
                  const JOSCPROB::JFlavour_t,
                  const JOSCPROB::JChargeParity_t>(),
	 py::arg("in"),
	 py::arg("out"),
	 py::arg("Cparity"))
    .def(py::init<const int, const int, const int>(),
	 py::arg("in"),
	 py::arg("out"),
	 py::arg("Cparity"))
    .def(py::self <  py::self)
    .def(py::self >  py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def("__repr__", &UTILS::get_representation<JOSCPROB::JOscChannel>)
    .def_readwrite("in", &JOSCPROB::JOscChannel::in)
    .def_readwrite("out", &JOSCPROB::JOscChannel::out)
    .def_readwrite("Cparity", &JOSCPROB::JOscChannel::Cparity);

  py::class_<JOSCPROB::JBaselineCalculator>(m, "JBaselineCalculator")
    .def(py::init<const double, const double>(),
	 py::arg("Lmin"),
	 py::arg("Lmax"))
    .def("__call__", &JOSCPROB::JBaselineCalculator::operator(),
	 py::arg("costh"))
    .def("__repr__", &UTILS::get_representation<JOSCPROB::JBaselineCalculator>);
    
  py::class_<JOSCPROB::JOscProbInterpolator<>>(m, "JOscProbInterpolator")
    .def(py::init<>())
    .def(py::init<const char*>(),
	 py::arg("file_name"))
    .def(py::init<const char*, const JOSCPROB::JOscParameters&>(),
	 py::arg("file_name"),
	 py::arg("parameters"))
    .def("load", &JOSCPROB::JOscProbInterpolator<>::load,
	 py::arg("file_name"))
    .def("get_table_parameters", &JOSCPROB::JOscProbInterpolator<>::getTableParameters)
    .def("get_baseline_calculator", &JOSCPROB::JOscProbInterpolator<>::getBaselineCalculator)
    .def("__call__", static_cast<double (JOSCPROB::JOscProbInterpolator<>::*)(const JOSCPROB::JOscChannel&,
    									      const double,
    									      const double) const>(&JOSCPROB::JOscProbInterpolator<>::operator()),
    	 py::arg("channel"),
    	 py::arg("E"),
    	 py::arg("costh"))
    .def("__call__", static_cast<double (JOSCPROB::JOscProbInterpolator<>::*)(const JOSCPROB::JOscParameters&,
									      const JOSCPROB::JOscChannel&,
									      const double,
									      const double)>(&JOSCPROB::JOscProbInterpolator<>::operator()),
	 py::arg("parameters"),	 
	 py::arg("channel"),
	 py::arg("E"),
	 py::arg("costh")
	 );
    
    py::class_<JOSCPROB::JppyOscProbInterpolator<>, JOSCPROB::JOscProbInterpolator<> >(m, "JppyOscProbInterpolator")
    .def(py::init<>())
    .def(py::init<const char*>(),
	 py::arg("file_name")) 
    .def(py::init<const char*, const JOSCPROB::JOscParameters&>(),
	 py::arg("file_name"),
	 py::arg("parameters"))
    .def("__call__", static_cast<py::array_t<double> (JOSCPROB::JppyOscProbInterpolator<>::*)(const JOSCPROB::JOscChannel&,
    									                      const py::array_t<double>&,
    									                      const py::array_t<double>&) const>(&JOSCPROB::JppyOscProbInterpolator<>::operator()),
    	 py::arg("channel"),
    	 py::arg("E"),
    	 py::arg("costh"))
    .def("__call__", static_cast<py::array_t<double> (JOSCPROB::JppyOscProbInterpolator<>::*)(const JOSCPROB::JOscParameters&,
									                      const JOSCPROB::JOscChannel&,
                                                                                              const py::array_t<double>&,
									                      const py::array_t<double>&)>(&JOSCPROB::JppyOscProbInterpolator<>::operator()),
	 py::arg("parameters"),	 
	 py::arg("channel"),
	 py::arg("E"),
	 py::arg("costh")
	 );
}
