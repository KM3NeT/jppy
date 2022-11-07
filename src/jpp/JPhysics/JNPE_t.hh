
#include "JLang/JException.hh"
#include "JTools/JCollection.hh"
#include "JTools/JMap.hh"
#include "JTools/JGridMap.hh"
#include "JTools/JMapList.hh"
#include "JTools/JSpline.hh"
#include "JTools/JPolint.hh"
#include "JTools/JElement.hh"
#include "JPhysics/JNPETable.hh"
#include "JPhysics/JPDFTable.hh"
#include "JPhysics/JPDFToolkit.hh"
#include "JPhysics/JPDFTypes.hh"
#include "JPhysics/JGeanz.hh"
#include "JMath/JZero.hh"


/**
 * \file
 *
 * Auxiliary data structure for muon PDF.
 * \author mdejong
 */
struct JMuonNPE_t {

  typedef JPP::JMAPLIST<JPP::JPolint1FunctionalMap,
			JPP::JPolint1FunctionalGridMap,
			JPP::JPolint1FunctionalGridMap>::maplist   JNPEMaplist_t;
  typedef JPP::JNPETable<double, double, JNPEMaplist_t>            JNPE_t;


  /**
   * Constructor.
   *
   * The PDF file descriptor should contain the wild card character JPHYSICS::WILD_CARD.\n
   *
   * \param  fileDescriptor     PDF file descriptor
   */
  JMuonNPE_t(const std::string& fileDescriptor)
  {
    using namespace std;
    using namespace JPP;

    const JPDFType_t pdf_t[] = { DIRECT_LIGHT_FROM_MUON,
				 SCATTERED_LIGHT_FROM_MUON,
				 DIRECT_LIGHT_FROM_DELTARAYS,
				 SCATTERED_LIGHT_FROM_DELTARAYS,
 				 DIRECT_LIGHT_FROM_EMSHOWERS,
				 SCATTERED_LIGHT_FROM_EMSHOWERS };

    const  int N = sizeof(pdf_t) / sizeof(pdf_t[0]);
    
    typedef JPP::JSplineFunction1D<JSplineElement2D<double, double>,
                                   JCollection,
                                   double>                          JFunction1D_t;
    typedef JPDFTable<JFunction1D_t, JNPEMaplist_t>                 JPDF_t;


    const JNPE_t::JSupervisor supervisor(new JNPE_t::JDefaultResult(zero));

    for (int i = 0; i != N; ++i) {

      JPDF_t pdf;

      const JPDFType_t type      = pdf_t[i];
      const string     file_name = getFilename(fileDescriptor, type);

      cout << "loading PDF from file " << file_name << "... " << flush;

      pdf.load(file_name.c_str());

      cout << "OK" << endl;

      pdf.setExceptionHandler(supervisor);

      if      (is_bremsstrahlung(type))
	YB.push_back(JNPE_t(pdf));
      else if (is_deltarays(type))
	YA.push_back(JNPE_t(pdf));
      else
	Y1.push_back(JNPE_t(pdf));
    }

    // Add PDFs

    cout << "adding PDFs... " << flush;

    Y1[1].add(Y1[0]); Y1.erase(Y1.begin());
    YA[1].add(YA[0]); YA.erase(YA.begin());
    YB[1].add(YB[0]); YB.erase(YB.begin());
  
    cout << "OK" << endl;
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
   * \return                    number of photo-electrons
   */
  double calculate(const double E,
		   const double R,
		   const double theta,
		   const double phi) const
  {
    using namespace JPP;

    const double y1 = getNPE(Y1, R, theta, phi);
    const double yA = getNPE(YA, R, theta, phi);
    const double yB = getNPE(YB, R, theta, phi);

    if (E >= MASS_MUON * INDEX_OF_REFRACTION_WATER)
      return y1  +  getDeltaRaysFromMuon(E) * yA  +  E * yB;
    else
      return 0.0;
  }

private:
  std::vector<JNPE_t> Y1;     //!< light from muon
  std::vector<JNPE_t> YA;     //!< light from delta-rays
  std::vector<JNPE_t> YB;     //!< light from EM showers

  /**
   * Get number of photo-electrons.
   *
   * \param  NPE                NPE tables
   * \param  R                  distance between muon and PMT [m]
   * \param  theta              zenith  angle orientation PMT [rad]
   * \param  phi                azimuth angle orientation PMT [rad]
   * \return                    number of photo-electrons
   */
  static inline double getNPE(const std::vector<JNPE_t>& NPE,
			      const double R,
			      const double theta,
			      const double phi)
  {
    using namespace std;
    using namespace JPP;

    double npe = 0.0;

    for (vector<JNPE_t>::const_iterator i = NPE.begin(); i != NPE.end(); ++i) {

      if (R <= i->getXmax()) {

	try {

	  const double y = get_value((*i)(std::max(R, i->getXmin()), theta, phi));

	  if (y > 0.0) {
	    npe += y;
	  }
	}
	catch(const exception& error) {
	  cerr << error.what() << endl;
	}
      }
    }

    return npe;
  }
};


/**
 * Auxiliary data structure for shower PDF.
 */
struct JShowerNPE_t {

  typedef JPP::JMAPLIST<JPP::JPolint1FunctionalMap,
			JPP::JPolint1FunctionalMap,
			JPP::JPolint1FunctionalGridMap,
			JPP::JPolint1FunctionalGridMap>::maplist   JNPEMaplist_t;
  typedef JPP::JNPETable<double, double, JNPEMaplist_t>            JNPE_t;


  /**
   * Constructor.
   *
   * The PDF file descriptor should contain the wild card character JPHYSICS::WILD_CARD.\n
   *
   * \param  fileDescriptor     PDF file descriptor
   * \param  numberOfPoints     number of points for shower elongation
   */
  JShowerNPE_t(const std::string& fileDescriptor,
	       const int          numberOfPoints = 0) :
    numberOfPoints(numberOfPoints)
  {
    using namespace std;
    using namespace JPP;
    
    const JPDFType_t pdf_t[] = { SCATTERED_LIGHT_FROM_EMSHOWER,
				 DIRECT_LIGHT_FROM_EMSHOWER };

    const  int N = sizeof(pdf_t) / sizeof(pdf_t[0]);

    typedef JPP::JSplineFunction1D<JSplineElement2D<double, double>,
                                   JCollection,
                                   double>                          JFunction1D_t;
    typedef JPDFTable<JFunction1D_t, JNPEMaplist_t>                 JPDF_t;


    const JNPE_t::JSupervisor supervisor(new JNPE_t::JDefaultResult(zero));

    for (int i = 0; i != N; ++i) {

      const string file_name = getFilename(fileDescriptor, pdf_t[i]);

      cout << "loading input from file " << file_name << "... " << flush;

      JPDF_t pdf;

      pdf.load(file_name.c_str());

      pdf.setExceptionHandler(supervisor);

      if (npe.empty())
	npe = JNPE_t(pdf);
      else
	npe.add(JNPE_t(pdf));

      F[i] = JNPE_t(pdf);

      cout << "OK" << endl;
    }
  }


  /**
   * Get PDF.
   *
   * The orientation of the PMT should be defined according this <a href="https://common.pages.km3net.de/jpp/JPDF.PDF">documentation</a>.\n
   * In this, the zenith and azimuth angles are limited to \f[\left[0, \pi\right]\f].
   *
   * \param  E                  shower energy at minimum distance of approach [GeV]
   * \param  D                  distance [m]
   * \param  cd                 cosine emission angle
   * \param  theta              PMT zenith  angle [rad]
   * \param  phi                PMT azimuth angle [rad]
   * \return                    hypothesis value
   */
  double calculate(const double E,
		   const double D,
		   const double cd,
		   const double theta,
		   const double phi) const
  {
    using namespace std;
    using namespace JPP;

    double Y = 0.0;
  
    if (numberOfPoints > 0) {

      const double W = 1.0 / (double) numberOfPoints;

      for (int i = 0; i != numberOfPoints; ++i) {

	const double z = geanz.getLength(E, (i + 0.5) / (double) numberOfPoints);

	const double __D  = sqrt(D*D - 2.0*(D*cd)*z + z*z);
	const double __cd = (D * cd - z) / __D;

	try {
	  Y += W * npe (__D, __cd, theta, phi);
	}
	catch(const exception& error) {
	  //cerr << error.what() << endl;
	}
      }

    } else {

      try {
	Y = npe(D, cd, theta, phi);
      }
      catch(const exception& error) {
	//cerr << error.what() << endl;
      }
    }

    return E * Y;
  }

private:
  int            numberOfPoints;
  JNPE_t npe;    //!< PDF for shower
  JNPE_t F[2];   //!< PDF for shower
};
