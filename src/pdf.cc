#include <pybind11/pybind11.h>

#include "JLang/JException.hh"
#include "JTools/JCollection.hh"
#include "JTools/JMap.hh"
#include "JTools/JGridMap.hh"
#include "JTools/JMapList.hh"
#include "JTools/JSpline.hh"
#include "JTools/JPolint.hh"
#include "JTools/JElement.hh"
#include "JTools/JResult.hh"
#include "JPhysics/JPDFTable.hh"
#include "JPhysics/JPDFTypes.hh"
#include "JMath/JZero.hh"


/**
 * Auxiliary data structure for muon PDF.
 */
struct JMuonPDF_t {

  typedef JPP::JSplineFunction1D<JPP::JSplineElement2S<double, double>, 
				 JPP::JCollection, 
				 JPP::JResultPDF<double> >         JFunction1D_t;

  typedef JPP::JMAPLIST<JPP::JPolint1FunctionalMap,
			JPP::JPolint0FunctionalGridMap,
			JPP::JPolint0FunctionalGridMap>::maplist   JPDFMaplist_t;
  typedef JPP::JPDFTable<JFunction1D_t, JPDFMaplist_t>             JPDF_t;
  typedef JFunction1D_t::result_type                               result_type;


  /**
   * Constructor.
   *
   * The PDF file descriptor should contain the wild card character JPHYSICS::WILD_CARD.\n
   * The <tt>TTS</tt> corresponds to the additional time smearing applied to the PDFs.
   *
   * \param  fileDescriptor     PDF file descriptor
   * \param  TTS                TTS [ns]
   * \param  numberOfPoints     number of points for Gauss-Hermite integration of TTS
   * \param  epsilon            precision        for Gauss-Hermite integration of TTS
   */
  JMuonPDF_t(const std::string& fileDescriptor,
	     const double       TTS,
	     const int          numberOfPoints = 25,
	     const double       epsilon        = 1.0e-10)
  {
    using namespace std;
    using namespace JPP;

    const JPDFType_t pdf_t[] = { DIRECT_LIGHT_FROM_MUON,
				 SCATTERED_LIGHT_FROM_MUON,
				 DIRECT_LIGHT_FROM_EMSHOWERS,
				 SCATTERED_LIGHT_FROM_EMSHOWERS,
				 DIRECT_LIGHT_FROM_DELTARAYS,
				 SCATTERED_LIGHT_FROM_DELTARAYS };

    const  int N = sizeof(pdf_t) / sizeof(pdf_t[0]);
    
    JPDF_t pdf[N];

    const JPDF_t::JSupervisor supervisor(new JPDF_t::JDefaultResult(zero));

    for (int i = 0; i != N; ++i) {

      const string file_name = getFilename(fileDescriptor, pdf_t[i]);

      cout << "loading input from file " << file_name << "... " << flush;

      pdf[i].load(file_name.c_str());

      pdf[i].setExceptionHandler(supervisor);

      cout << "OK" << endl;
    }

    // Add PDFs

    cout << "adding PDFs... " << flush;

    pdfA = pdf[1];  pdfA.add(pdf[0]);
    pdfB = pdf[3];  pdfB.add(pdf[2]);
    pdfC = pdf[5];  pdfC.add(pdf[4]);

    cout << "OK" << endl;

    if        (TTS > 0.0) {

      cout << "bluring PDFs... " << flush;

      pdfA.blur(TTS, numberOfPoints, epsilon);
      pdfB.blur(TTS, numberOfPoints, epsilon);
      pdfC.blur(TTS, numberOfPoints, epsilon);

      cout << "OK" << endl;

    } else if (TTS < 0.0) {

      THROW(JValueOutOfRange, "Illegal value of TTS [ns]: " << TTS); 
    }
  }


  /**
   * Get PDF.
   *
   * The orientation of the PMT should be defined according this <a href="https://common.pages.km3net.de/jpp/JPDF.PDF">documentation</a>.\n
   * In this, the zenith and azimuth angles are limited to \f[\left[0, \pi\right]\f].
   *
   * \param  E                  muon energy at minimum distance of approach [GeV]
   * \param  R                  minimum distance of approach [m]
   * \param  theta              PMT zenith  angle [rad]
   * \param  phi                PMT azimuth angle [rad]
   * \param  t1                 arrival time relative to Cherenkov hypothesis [ns]
   * \return                    hypothesis value
   */
  result_type calculate(const double E,
			 const double R,
			 const double theta,
			 const double phi,
			 const double t1) const
  {
    using namespace JPP;

    result_type h1 = (pdfA(R, theta, phi, t1)                               +
		      pdfB(R, theta, phi, t1) * E                           +
		      pdfC(R, theta, phi, t1) * getDeltaRaysFromMuon(E));

    // safety measures

    if (h1.f <= 0.0) {
      h1.f  = 0.0;
      h1.fp = 0.0;
    }
          
    if (h1.v <= 0.0) {
      h1.v  = 0.0;
    }
          
    return h1;
  }

  JPDF_t pdfA;   //!< PDF for minimum ionisong particle
  JPDF_t pdfB;   //!< PDF for average energy losses
  JPDF_t pdfC;   //!< PDF for delta-rays
};


/**
   Python bindings.
 */

namespace py = pybind11;

PYBIND11_MODULE(pdf, m) {
    m.doc() = "PDF utilities";
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
