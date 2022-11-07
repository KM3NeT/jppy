#ifndef __JLANG__JSTREAMSTATE__
#define __JLANG__JSTREAMSTATE__

#include <istream>
#include <ostream>


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * This class can be used to temporarily exchange the states between streams.
   * The constructor transfers the state from the first stream to the second stream and
   * the destructor transfers back the state from the second stream to the first stream.
   */
  class JStreamState {
  public:
    /**
     * Constructor.
     * The stream state of <tt>from</tt> is transfered to stream state of <tt>to</tt>.
     *
     * \param  from            output stream
     * \param  to              output stream
     */
    JStreamState(std::ostream& from,
		 std::ostream& to) :
      __from(from),
      __to  (to)
    {
      __to.setstate(__from.rdstate());
    }


    /**
     * Constructor.
     * The stream state of <tt>from</tt> is transfered to stream state of <tt>to</tt>.
     *
     * \param  from            input stream
     * \param  to              input stream
     */
    JStreamState(std::istream& from,
		 std::istream& to) :
      __from(from),
      __to  (to)
    {
      __to.setstate(__from.rdstate());
    }

    
    /**
     * Destructor.
     * The stream state is transfered back.
     */
    ~JStreamState()
    {
      __from.setstate(__to.rdstate());
    }

  private:
    std::ios& __from;
    std::ios& __to;

    JStreamState(const JStreamState&);
    JStreamState(JStreamState&&);
    JStreamState& operator=(const JStreamState&);
    JStreamState& operator=(JStreamState&&);
  };
}

#endif
