#ifndef __JPHYSICS__JPDFTRANSFORMER__
#define __JPHYSICS__JPDFTRANSFORMER__

#include <cmath>

#include "JLang/JCC.hh"
#include "JIO/JSerialisable.hh"
#include "JPhysics/JConstants.hh"
#include "JTools/JMultiMapTransformer.hh"
#include "JTools/JFunction1D_t.hh"
#include "JTools/JGrid.hh"
#include "JPhysics/JGeant_t.hh"


/**
 * \author mdejong
 */

namespace JPHYSICS {}
namespace JPP { using namespace JPHYSICS; }

namespace JPHYSICS {

  using JIO::JReader;
  using JIO::JWriter;
  using JTOOLS::JMultiMapTransformer;


  /**
   * Transformer for the 1D probability density function (PDF) of the time response of a PMT to a muon.
   *
   * PDFs are evaluated by interpolation for:
   *   -# distance of closest approach of the muon to the PMT [m]
   *   -# arrival time [ns]
   *
   * The evaluation of the weights is based on:
   *   -# effective attenuation length
   */
  template<class JArgument_t>
  class JPDFTransformer_t :
    public JMultiMapTransformer<1, JArgument_t>
  {
  public:

    typedef JMultiMapTransformer<1, JArgument_t>                     JMultiMapTransformer_t;

    typedef typename JMultiMapTransformer_t::clone_type              clone_type;
    typedef typename JMultiMapTransformer_t::argument_type           argument_type;
    typedef typename JMultiMapTransformer_t::const_array_type        const_array_type;

    using JMultiMapTransformer_t::getWeight;

    /**
     * Get shortest distance of approach.
     *
     * \return              distance [m]
     */
    static double getRmin()
    {
      return 0.01;
    }


    /**
     * Default constructor.
     */
    JPDFTransformer_t() :
      ln   (0.0),
      alpha(0),
      kmin (0.0),
      kmax (0.0)
    {}


    /**
     * Constructor.
     *  
     * \param  ln           Effective attenuation length [m]
     * \param  alpha        Distance dependence (power term)
     * \param  kmin         Minimal kappa
     * \param  kmax         Maximal kappa
     */
    JPDFTransformer_t(const double ln,
		      const int    alpha,
		      const double kmin,
		      const double kmax) :
      ln   (ln),
      alpha(alpha),
      kmin (kmin),
      kmax (kmax)
    {}


    /**
     * Clone object.
     * 
     * \return            pointer to newly created transformer
     */
    virtual clone_type clone() const override 
    {
      return new JPDFTransformer_t(*this);
    }


    /**
     * Evaluate arrival time.
     *
     * \param  buffer     {R_m}
     * \param  xn         old t_ns
     * \return            new t_ns
     */
    virtual argument_type putXn(const_array_type& buffer, const argument_type xn) const override 
    {
      using namespace JTOOLS;

      const double R = buffer[0];

      double x = xn;

      const double t0 =  R * getTanThetaC() * getInverseSpeedOfLight();
      const double t1 =  R * kmin * getInverseSpeedOfLight();

      x -= t1 - t0;
      
      if (kmax > kmin) {
	x /= R * (kmax - kmin) * getInverseSpeedOfLight();
      }

      return x;
    }


    /**
     * Evaluate arrival time.
     *
     * \param  buffer     {R_m}
     * \param  xn         old t_ns
     * \return            new t_ns
     */
    virtual argument_type getXn(const_array_type& buffer, const argument_type xn) const override 
    {
      using namespace JTOOLS;

      const double R = buffer[0];

      double x = xn;

      if (kmax > kmin) {
	x *= R * (kmax - kmin) * getInverseSpeedOfLight();
      }

      const double t0 =  R * getTanThetaC() * getInverseSpeedOfLight();
      const double t1 =  R * kmin * getInverseSpeedOfLight();
      
      x += t1 - t0;

      return x;
    }


    /**
     * Weight function.
     *
     * \param  buffer     {R_m}
     * \return            weight
     */
    virtual double getWeight(const_array_type& buffer) const override 
    {
      using namespace JTOOLS;

      const double R = buffer[0];

      const double n   = getIndexOfRefraction();
      const double ct0 = 1.0 / n;
      const double st0 = sqrt((1.0 + ct0)*(1.0 - ct0));
    
      const double d  = sqrt(getRmin()*getRmin() + R*R) / st0;
    
      return exp(-d/ln) / pow(d,alpha);
    }


    /**
     * Read PDF transformer from input.
     *
     * \param  in         reader
     * \return            reader
     */
    virtual JReader& read(JReader& in) override 
    {
      in >> ln;
      in >> alpha;
      in >> kmin;
      in >> kmax;

      return in;
    }


    /**
     * Write PDF transformer to output.
     *
     * \param  out        writer
     * \return            writer
     */
    virtual JWriter& write(JWriter& out) const override 
    {
      out << ln;
      out << alpha;
      out << kmin;
      out << kmax;

      return out;
    }


    /**
     * Print PDF transformer to output stream.
     *
     * \param  out        output stream
     * \return            output stream
     */
    std::ostream& print(std::ostream& out) const
    {
      using namespace std;

      out << "Effective attenuation length [m]   " << ln    << endl;
      out << "Distance dependence (power term)   " << alpha << endl;
      out << "Minimal kappa                      " << kmin  << endl;
      out << "Maximal kappa                      " << kmax  << endl;

      return out;
    }


    double ln;                 //!< Effective attenuation length [m]
    int    alpha;              //!< Distance dependence (power term)
    double kmin;               //!< minimal kappa
    double kmax;               //!< maximal kappa
  };


  /**
   * Transformer for the 1D probability density function (PDF) of the time response of a PMT due to a point source.
   *
   * PDFs are evaluated by interpolation for:
   *   -# distance between point source and PMT [m]
   *   -# arrival time [ns]
   *
   * The evaluation of the weights is based on:
   *   -# effective attenuation length
   */
  template<class JArgument_t>
  class JPD0Transformer_t : 
    public JMultiMapTransformer<1, JArgument_t>
  {
  public:

    typedef JMultiMapTransformer<1, JArgument_t>                     JMultiMapTransformer_t;

    typedef typename JMultiMapTransformer_t::clone_type              clone_type;
    typedef typename JMultiMapTransformer_t::argument_type           argument_type;
    typedef typename JMultiMapTransformer_t::const_array_type        const_array_type;

    using JMultiMapTransformer_t::getWeight;

    /**
     * Get shortest distance.
     *
     * \return              distance [m]
     */
    static double getDmin()
    {
      return 0.01;
    }


    /**
     * Default constructor.
     */
    JPD0Transformer_t() :
      ln   (0.0),
      alpha(0),
      kmin (0.0),
      kmax (0.0)
    {}


    /**
     * Constructor.
     *
     * \param  ln         Effective attenuation length [m]
     * \param  alpha      Distance dependence (power term)
     * \param  kmin       Minimal kappa
     * \param  kmax       Maximal kappa
     */
    JPD0Transformer_t(const double  ln,
		      const int     alpha,
		      const double  kmin,
		      const double  kmax) :
      ln   (ln),
      alpha(alpha),
      kmin (kmin),
      kmax (kmax)
    {}


    /**
     * Clone object.
     * 
     * \return            pointer to newly created transformer
     */
    virtual clone_type clone() const override 
    {
      return new JPD0Transformer_t(*this);
    }


    /**
     * Evaluate arrival time.
     *
     * \param  buffer     {D_m}
     * \param  xn         old t_ns
     * \return            new t_ns
     */
    virtual argument_type putXn(const_array_type& buffer, const argument_type xn) const override 
    {
      using namespace JTOOLS;

      const double D  = buffer[0];

      double x = xn;

      const double t0 =  D * getIndexOfRefraction() * getInverseSpeedOfLight();
      const double t1 =  D * kmin * getInverseSpeedOfLight();

      x -= t1 - t0;

      if (kmax > kmin) {
        x /= D * (kmax - kmin) * getInverseSpeedOfLight();
      }

      return x;
    }


    /**
     * Evaluate arrival time.
     *
     * \param  buffer     {D_m}
     * \param  xn         old t_ns
     * \return            new t_ns
     */
    virtual argument_type getXn(const_array_type& buffer, const argument_type xn) const override 
    {
      using namespace JTOOLS;

      const double D  = buffer[0];

      double x = xn;

      if (kmax > kmin) {
        x *= D * (kmax - kmin) * getInverseSpeedOfLight();
      }

      const double t0 =  D * getIndexOfRefraction() * getInverseSpeedOfLight();
      const double t1 =  D * kmin * getInverseSpeedOfLight();

      x += t1 - t0;

      return x;
    }


    /**
     * Weight function.
     *
     * \param  buffer     {D_m}
     * \return            weight
     */
    virtual double getWeight(const_array_type& buffer) const override 
    {
      using namespace JTOOLS;

      const double D  = buffer[0];

      const double d  = sqrt(getDmin()*getDmin() + D*D);

      return exp(-d/ln) / pow(d,alpha);
    }


    /**
     * Read PDF transformer from input.
     *
     * \param  in         reader
     * \return            reader
     */
    virtual JReader& read(JReader& in) override 
    {
      in >> ln;
      in >> alpha;
      in >> kmin;
      in >> kmax;

      return in;
    }


    /**
     * Write PDF transformer to output.
     *
     * \param  out        writer
     * \return            writer
     */
    virtual JWriter& write(JWriter& out) const override 
    {
      out << ln;
      out << alpha;
      out << kmin;
      out << kmax;

      return out;
    }


    /**
     * Print PDF transformer to output stream.
     *
     * \param  out        output stream
     * \return            output stream
     */
    std::ostream& print(std::ostream& out) const
    {
      using namespace std;

      out << "Effective attenuation length [m]   " << ln    << endl;
      out << "Distance dependence (power term)   " << alpha << endl;
      out << "Minimal kappa                      " << kmin  << endl;
      out << "Maximal kappa                      " << kmax  << endl;

      return out;
    }


    double ln;                 //!< Effective attenuation length [m]
    int    alpha;              //!< Distance dependence (power term)
    double kmin;               //!< minimal kappa
    double kmax;               //!< maximal kappa
  };


  /**
   * Transformer for the 2D probability density function (PDF) of the time response of a PMT due to an EM shower.
   *
   * PDFs are evaluated by interpolation for:
   *   -# distance between EM shower and PMT [m]
   *   -# cosine angle EM shower direction and EM shower - PMT position
   *   -# arrival time [ns]
   *
   * The evaluation of the weights is based on:
   *   -# effective attenuation length
   *   -# emission profile of the photons
   */
  template<class JArgument_t>
  class JPDGTransformer_t : 
    public JMultiMapTransformer<2, JArgument_t>
  {
  public:

    typedef JPD0Transformer_t      <JArgument_t>                     JFunction1DTransformer_t; 
    typedef JMultiMapTransformer<2, JArgument_t>                     JMultiMapTransformer_t;

    typedef typename JMultiMapTransformer_t::clone_type              clone_type;
    typedef typename JMultiMapTransformer_t::argument_type           argument_type;
    typedef typename JMultiMapTransformer_t::const_array_type        const_array_type;

    using JMultiMapTransformer_t::getWeight;


    /**
     * Default constructor.
     */
    JPDGTransformer_t() :
      transformer(),
      getShowerProbability()
    {}


    /**
     * Constructor.
     *
     * \param  ln         Effective attenuation length [m]
     * \param  alpha      Distance dependence (power term)
     * \param  kmin       Minimal kappa
     * \param  kmax       Maximal kappa
     * \param  geant      Function photon emission from EM-shower
     * \param  bmin       Baseline photon emission from EM-shower
     */
    JPDGTransformer_t(const double    ln,
		      const int       alpha,
		      const double    kmin,
		      const double    kmax,
		      const JGeant_t& geant,
		      const double    bmin) :
      transformer(ln, alpha, kmin, kmax),
      getShowerProbability(geant)
    {
      getShowerProbability.add(bmin);
      getShowerProbability.compile();
    }


    /**
     * Clone object.
     * 
     * \return            pointer to newly created transformer
     */
    virtual clone_type clone() const override 
    {
      return new JPDGTransformer_t(*this);
    }


    /**
     * Evaluate arrival time.
     *
     * \param  buffer     {D_m, cd}
     * \param  xn         old t_ns
     * \return            new t_ns
     */
    virtual argument_type putXn(const_array_type& buffer, const argument_type xn) const override 
    {
      return transformer.putXn(buffer, xn);
    }


    /**
     * Evaluate arrival time.
     *
     * \param  buffer     {D_m, cd}
     * \param  xn         old t_ns
     * \return            new t_ns
     */
    virtual argument_type getXn(const_array_type& buffer, const argument_type xn) const override 
    {
      return transformer.getXn(buffer, xn);
    }


    /**
     * Weight function.
     *
     * \param  buffer     {D_m, cd}
     * \return            weight
     */
    virtual double getWeight(const_array_type& buffer) const override 
    {
      using namespace JTOOLS;

      //const double D  = buffer[0];
      const double cd = buffer[1];

      return transformer.getWeight(buffer) * getShowerProbability(getIndexOfRefractionPhase(), cd);
    }


    /**
     * Read PDF transformer from input.
     *
     * \param  in         reader
     * \return            reader
     */
    virtual JReader& read(JReader& in) override 
    {
      in >> transformer;
      in >> getShowerProbability;

      return in;
    }


    /**
     * Write PDF transformer to output.
     *
     * \param  out        writer
     * \return            writer
     */
    virtual JWriter& write(JWriter& out) const override 
    {
      out << transformer;
      out << getShowerProbability;

      return out;
    }


    /**
     * Print PDF transformer to output stream.
     *
     * \param  out        output stream
     * \return            output stream
     */
    std::ostream& print(std::ostream& out) const
    {
      return transformer.print(out);
    }


    JFunction1DTransformer_t transformer;
    JGeant_t                 getShowerProbability;
  };


  /**
   * Template definition of transformer of the probability density function (PDF) of the time response of a PMT.\n
   * The actual implementation follows from the number of dimensions.
   */
  template<unsigned int N, class JArgument_t>
  class JPDFTransformer;


  /**
   * Template specialisation of transformer of the 2D probability density function (PDF) of the time response of a PMT due to a bright point.
   *
   * PDFs are evaluated by interpolation for:
   *   -# distance between bright point and PMT [m]
   *   -# cosine PMT angle
   *   -# arrival time [ns]
   *
   * The evaluation of the weights is based on:
   *   -# effective attenuation length
   */
  template<class JArgument_t>
  class JPDFTransformer<2, JArgument_t> : 
    public JMultiMapTransformer<2, JArgument_t>
  {
  public:

    typedef JPD0Transformer_t      <JArgument_t>                     JFunction1DTransformer_t;
    typedef JMultiMapTransformer<2, JArgument_t>                     JMultiMapTransformer_t;

    typedef typename JMultiMapTransformer_t::clone_type              clone_type;
    typedef typename JMultiMapTransformer_t::argument_type           argument_type;
    typedef typename JMultiMapTransformer_t::const_array_type        const_array_type;


    /**
     * Default constructor.
     */
    JPDFTransformer() :
      transformer()
    {}


    /**
     * Constructor.
     *
     * \param  ln         Effective attenuation length [m]
     * \param  alpha      Distance dependence (power term)
     * \param  kmin       Minimal kappa
     * \param  kmax       Maximal kappa
     */
    JPDFTransformer(const double ln,
		    const int    alpha,
		    const double kmin,
		    const double kmax) :
      transformer(ln, alpha, kmin, kmax)
    {}


    /**
     * Clone object.
     * 
     * \return            pointer to newly created transformer
     */
    virtual clone_type clone() const override 
    {
      return new JPDFTransformer(*this);
    }


    /**
     * Evaluate arrival time.
     *
     * \param  buffer     {D_m, ct}
     * \param  xn         old t_ns
     * \return            new t_ns
     */
    virtual argument_type putXn(const_array_type& buffer, const argument_type xn) const override 
    {
      return transformer.putXn(buffer, xn);
    }


    /**
     * Evaluate arrival time.
     *
     * \param  buffer     {D_m, ct}
     * \param  xn         old t_ns
     * \return            new t_ns
     */
    virtual argument_type getXn(const_array_type& buffer, const argument_type xn) const override 
    {
      return transformer.getXn(buffer, xn);
    }


    /**
     * Weight function.
     *
     * \param  buffer     {D_m, ct}
     * \return            weight
     */
    virtual double getWeight(const_array_type& buffer) const override 
    {
      return transformer.getWeight(buffer);
    }


    /**
     * Read PDF transformer from input.
     *
     * \param  in         reader
     * \return            reader
     */
    virtual JReader& read(JReader& in) override 
    {
      in >> transformer;

      return in;
    }


    /**
     * Write PDF transformer to output.
     *
     * \param  out        writer
     * \return            writer
     */
    virtual JWriter& write(JWriter& out) const override 
    {
      out << transformer;

      return out;
    }


    /**
     * Print PDF transfomer to output stream.
     *
     * \param  out        output stream
     * \return            output stream
     */
    std::ostream& print(std::ostream& out) const
    {
      return transformer.print(out);
    }


    JFunction1DTransformer_t transformer;
  };


  /**
   * Template specialisation of transformer of the 3D probability density function (PDF) of the time response of a PMT due to a muon.
   *
   * PDFs are evaluated by interpolation for:
   *   -# distance of closest approach of the muon to the PMT [m]
   *   -# zenith    angle of the PMT
   *   -# azimuthal angle of the PMT
   *   -# arrival time [ns]
   *
   * The evaluation of the weights is based on:
   *   -# effective attenuation length
   *   -# angular acceptance of PMT
   */
  template<class JArgument_t>
  class JPDFTransformer<3, JArgument_t> : 
    public JMultiMapTransformer<3, JArgument_t>
  {
  public:

    typedef JPDFTransformer_t      <JArgument_t>                     JFunction1DTransformer_t; 
    typedef JMultiMapTransformer<3, JArgument_t>                     JMultiMapTransformer_t;
 
    typedef typename JMultiMapTransformer_t::clone_type              clone_type;
    typedef typename JMultiMapTransformer_t::argument_type           argument_type;
    typedef typename JMultiMapTransformer_t::const_array_type        const_array_type;

    typedef JTOOLS::JGridPolint1Function1D_t                         JFunction1D_t;

    using JMultiMapTransformer_t::getWeight;


    /**
     * Default constructor.
     */
    JPDFTransformer() :
      transformer(),
      getAngularAcceptance()
    {}


    /**
     * Constructor.
     *  
     * \param  ln         Effective attenuation length [m]
     * \param  alpha      Distance dependence (power term)
     * \param  kmin       Minimal kappa
     * \param  kmax       Maximal kappa
     * \param  pmt        Function angular acceptance of PMT
     * \param  amin       Baseline angular acceptance of PMT
     */
    template<class T>
    JPDFTransformer(const double ln,
		    const int    alpha,
		    const double kmin,
		    const double kmax,
		    T            pmt,
		    const double amin) :
      transformer(ln, alpha, kmin, kmax),
      getAngularAcceptance()
    {
      getAngularAcceptance.configure(JTOOLS::make_grid(1000, -1.0, +1.0), pmt);
      getAngularAcceptance.add(amin);
      getAngularAcceptance.compile();
      getAngularAcceptance.setExceptionHandler(new JFunction1D_t::JDefaultResult(0.0));
    }


    /**
     * Clone object.
     * 
     * \return            pointer to newly created transformer
     */
    virtual clone_type clone() const override 
    {
      return new JPDFTransformer(*this);
    }


    /**
     * Evaluate arrival time.
     *
     * \param  buffer     {R_m, theta, phi}
     * \param  xn         old t_ns
     * \return            new t_ns
     */
    virtual argument_type putXn(const_array_type& buffer, const argument_type xn) const override 
    {
      return transformer.putXn(buffer, xn);
    }


    /**
     * Evaluate arrival time.
     *
     * \param  buffer     {R_m, theta, phi}
     * \param  xn         old t_ns
     * \return            new t_ns
     */
    virtual argument_type getXn(const_array_type& buffer, const argument_type xn) const override 
    {
      return transformer.getXn(buffer, xn);
    }


    /**
     * Weight function.
     *
     * \param  buffer     {R_m, theta, phi}
     * \return            weight
     */
    virtual double getWeight(const_array_type& buffer) const override 
    {
      using namespace JTOOLS;

      //const double R     = buffer[0];
      const double theta = buffer[1];
      const double phi   = buffer[2];

      const double n   = getIndexOfRefraction();
      const double ct0 = 1.0 / n;
      const double st0 = sqrt((1.0 + ct0)*(1.0 - ct0));
    
      const double px = sin(theta)*cos(phi);
      //const double py = sin(theta)*sin(phi);
      const double pz = cos(theta);
    
      const double ct = st0*px + ct0*pz;
    
      return transformer.getWeight(buffer) * getAngularAcceptance(ct);
    }


    /**
     * Read PDF transformer from input.
     *
     * \param  in         reader
     * \return            reader
     */
    virtual JReader& read(JReader& in) override 
    {
      in >> transformer;
      in >> getAngularAcceptance;

      getAngularAcceptance.compile();

      return in;
    }


    /**
     * Write PDF transformer to output.
     *
     * \param  out        writer
     * \return            writer
     */
    virtual JWriter& write(JWriter& out) const override 
    {
      out << transformer;
      out << getAngularAcceptance;

      return out;
    }


    /**
     * Print PDF transformer to output stream.
     *
     * \param  out        output stream
     * \return            output stream
     */
    std::ostream& print(std::ostream& out) const
    {
      return transformer.print(out);
    }


    JFunction1DTransformer_t transformer;
    JFunction1D_t            getAngularAcceptance;
  };


  /**
   * Template specialisation of transformer of the 4D probability density function (PDF) of the time response of a PMT due to an EM shower.
   *
   * PDFs are evaluated by interpolation for:
   *   -# distance between EM shower and PMT [m]
   *   -# cosine angle EM shower direction and EM shower - PMT position
   *   -# zenith    angle of the PMT
   *   -# azimuthal angle of the PMT
   *   -# arrival time [ns]
   *
   * The evaluation of the weights is based on:
   *   -# effective attenuation length
   *   -# emission profile of the photons
   *   -# angular acceptance of PMT
   */
  template<class JArgument_t>
  class JPDFTransformer<4, JArgument_t> : 
    public JMultiMapTransformer<4, JArgument_t>
  {
  public:

    typedef JPDGTransformer_t      <JArgument_t>                     JFunction2DTransformer_t;
    typedef JMultiMapTransformer<4, JArgument_t>                     JMultiMapTransformer_t;

    typedef typename JMultiMapTransformer_t::clone_type              clone_type;
    typedef typename JMultiMapTransformer_t::argument_type           argument_type;
    typedef typename JMultiMapTransformer_t::const_array_type        const_array_type;

    typedef JTOOLS::JGridPolint1Function1D_t                         JFunction1D_t;

    using JMultiMapTransformer_t::getWeight;


    /**
     * Default constructor.
     */
    JPDFTransformer() :
      transformer(),
      getAngularAcceptance()
    {}


    /**
     * Constructor.
     *
     * \param  ln         Effective attenuation length [m]
     * \param  alpha      Distance dependence (power term)
     * \param  kmin       Minimal kappa
     * \param  kmax       Maximal kappa
     * \param  geant      Function photon emission from EM-shower
     * \param  bmin       Baseline photon emission from EM-shower
     * \param  pmt        Function angular acceptance of PMT
     * \param  amin       Baseline angular acceptance of PMT
     */
    template<class T>
    JPDFTransformer(const double    ln,
		    const int       alpha,
		    const double    kmin,
		    const double    kmax,
		    const JGeant_t& geant,
		    const double    bmin,
		    T               pmt,
		    const double    amin) :
      transformer(ln, alpha, kmin, kmax, geant, bmin),
      getAngularAcceptance()
    {
      getAngularAcceptance.configure(JTOOLS::make_grid(1000, -1.0, +1.0), pmt);
      getAngularAcceptance.add(amin);
      getAngularAcceptance.compile();
      getAngularAcceptance.setExceptionHandler(new JFunction1D_t::JDefaultResult(0.0));
    }


    /**
     * Clone object.
     * 
     * \return            pointer to newly created transformer
     */
    virtual clone_type clone() const override 
    {
      return new JPDFTransformer(*this);
    }


    /**
     * Evaluate arrival time.
     *
     * \param  buffer     {D_m, cd, theta, phi}
     * \param  xn         old t_ns
     * \return            new t_ns
     */
    virtual argument_type putXn(const_array_type& buffer, const argument_type xn) const override 
    {
      return transformer.putXn(buffer, xn);
    }


    /**
     * Evaluate arrival time.
     *
     * \param  buffer     {D_m, cd, theta, phi}
     * \param  xn         old t_ns
     * \return            new t_ns
     */
    virtual argument_type getXn(const_array_type& buffer, const argument_type xn) const override 
    {
      return transformer.getXn(buffer, xn);
    }


    /**
     * Weight function.
     *
     * \param  buffer     {D_m, cd, theta, phi}
     * \return            weight
     */
    virtual double getWeight(const_array_type& buffer) const override 
    {
      const double cd    = buffer[1];
      const double theta = buffer[2];
      const double phi   = buffer[3];

      const double ct0 = (cd > -1.0 ? cd < +1.0 ? cd : +1.0 : -1.0);
      const double st0 = sqrt((1.0 + ct0)*(1.0 - ct0));
    
      const double px = sin(theta)*cos(phi);
      //const double py = sin(theta)*sin(phi);
      const double pz = cos(theta);
    
      const double ct = st0*px + ct0*pz;

      return transformer.getWeight(buffer) * getAngularAcceptance(ct);
    }


    /**
     * Read PDF transformer from input.
     *
     * \param  in         reader
     * \return            reader
     */
    virtual JReader& read(JReader& in) override 
    {
      in >> transformer;
      in >> getAngularAcceptance;

      getAngularAcceptance.compile();

      return in;
    }


    /**
     * Write PDF transformer to output.
     *
     * \param  out        writer
     * \return            writer
     */
    virtual JWriter& write(JWriter& out) const override 
    {
      out << transformer;
      out << getAngularAcceptance;

      return out;
    }


    /**
     * Print PDF transfomer to output stream.
     *
     * \param  out        output stream
     * \return            output stream
     */
    std::ostream& print(std::ostream& out) const
    {
      return transformer.print(out);
    }


    JFunction2DTransformer_t transformer;
    JFunction1D_t            getAngularAcceptance;
  };


  /**
   * Template specialisation of transformer of the 5D probability density function (PDF) of the time response of a PMT due to an EM shower.
   *
   * PDFs are evaluated by interpolation for:
   *   -# energy of the EM shower
   *   -# distance between EM shower and PMT [m]
   *   -# cosine angle EM shower direction and EM shower - PMT position
   *   -# zenith    angle of the PMT
   *   -# azimuthal angle of the PMT
   *   -# arrival time [ns]
   *
   * The evaluation of the weights is based on:
   *   -# energy of the EM shower
   *   -# effective attenuation length
   *   -# emission profile of the photons
   *   -# angular acceptance of PMT
   */
  template<class JArgument_t>
  class JPDFTransformer<5, JArgument_t> : 
    public JMultiMapTransformer<5, JArgument_t>
  {
  public:
    
    typedef JPDFTransformer     <4, JArgument_t>                     JFunction4DTransformer_t;
    typedef JMultiMapTransformer<5, JArgument_t>                     JMultiMapTransformer_t;
    
    typedef typename JMultiMapTransformer_t::clone_type              clone_type;
    typedef typename JMultiMapTransformer_t::argument_type           argument_type;
    typedef typename JMultiMapTransformer_t::const_array_type        const_array_type;
    
    
    /**
     * Default constructor.
     */
    JPDFTransformer() :
      transformer()
    {}    

    
    /**
     * Constructor.
     *
     * \param  transformer    transformer
     */
    JPDFTransformer(const JFunction4DTransformer_t& transformer) :
      transformer(transformer)
    {}


    /**
     * Constructor.
     *
     * \param  ln         Effective attenuation length [m]
     * \param  alpha      Distance dependence (power term)
     * \param  kmin       Minimal kappa
     * \param  kmax       Maximal kappa
     * \param  geant      Function photon emission from EM-shower
     * \param  bmin       Baseline photon emission from EM-shower
     * \param  pmt        Function angular acceptance of PMT
     * \param  amin       Baseline angular acceptance of PMT
     */
    
    template<class T>
    JPDFTransformer(const double    ln,
		    const int       alpha,
		    const double    kmin,
		    const double    kmax,
		    const JGeant_t& geant,
		    const double    bmin,
		    T               pmt,
		    const double    amin) :
      transformer(ln, alpha, kmin, kmax, geant, bmin, pmt, amin)
    {}


    /**
     * Clone object.
     * 
     * \return            pointer to newly created transformer
     */
    virtual clone_type clone() const override 
    {
      return new JPDFTransformer(*this);
    }
    
    
    /**
     * Evaluate arrival time.
     *
     * \param  buffer     {E_GeV, D_m, cd, theta, phi}
     * \param  xn         old t_ns
     * \return            new t_ns
     */
    virtual argument_type putXn(const_array_type& buffer, const argument_type xn) const override 
    {
      return transformer.putXn(buffer.pop_front(), xn);
    }
    
    
    /**
     * Evaluate arrival time.
     *
     * \param  buffer     {E_GeV, D_m, cd, theta, phi}
     * \param  xn         old t_ns
     * \return            new t_ns
     */
    virtual argument_type getXn(const_array_type& buffer, const argument_type xn) const override 
    {
      return transformer.getXn(buffer.pop_front(), xn);
    }
    
    
    /**
     * Weight function.
     *
     * \param  buffer     {E_GeV, D_m, cd, theta, phi}
     * \return            weight
     */
    virtual double getWeight(const_array_type& buffer) const override 
    { 
      const double E     = buffer[0];

      return transformer.getWeight(buffer.pop_front()) / E;
    }
    
    
    /**
     * Read PDF transformer from input.
     *
     * \param  in         reader
     * \return            reader
     */
    virtual JReader& read(JReader& in) override 
    {
      in >> transformer;

      return in;
    }
    
  
    /**
     * Write PDF transformer to output.
     *
     * \param  out        writer
     * \return            writer
     */
    virtual JWriter& write(JWriter& out) const override 
    {
      out << transformer;
      
      return out;
    }
    
    
    /**
     * Print PDF transfomer to output stream.
     *
     * \param  out        output stream
     * \return            output stream
     */
    std::ostream& print(std::ostream& out) const
    {
      return transformer.print(out);
    }
    
    JFunction4DTransformer_t transformer;
  };
}

#endif
