#ifndef __JTOOLS__JFUNCTIONAL__
#define __JTOOLS__JFUNCTIONAL__

#include "JLang/JSharedPointer.hh"
#include "JLang/JNullType.hh"
#include "JLang/JException.hh"
#include "JLang/JVoid.hh"
#include "JLang/JClass.hh"
#include "JMath/JZero.hh"


/**
 * \author mdejong
 */

namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JLANG::JClass;
  using JLANG::JException;
  using JLANG::JNullType;
  using JLANG::JVoid;
  using JLANG::JSharedPointer;


  /**
   * Template definition of function object interface.
   */
  template<class JArgument_t = JNullType, class JResult_t = JNullType>
  class JFunctional;


  /**
   * Template specialisation of compilable function object.
   */
  template<>
  class JFunctional<JNullType, JNullType> 
  {
  protected:
    /**
     * Function compilation.
     */
    virtual void do_compile() = 0;


  public:
    /**
     * Virtual destructor.
     */
    virtual ~JFunctional() 
    {}


    /**
     * Function compilation.
     */
    void compile() 
    {
      do_compile();
    }
  };


  /**
   * Template definition of recursive function value evaluation.
   */
  template<class JArgument_t, class JResult_t>
  class JFunctional : 
    public virtual JFunctional<JNullType, JNullType>
  {
  protected:
    /**
     * Default constructor.
     */
    JFunctional() :
      JFunctional<JNullType, JNullType>(),
      supervisor(JSupervisor::getInstance())
    {}

  public:

    class JSupervisor;

    typedef JArgument_t                                   argument_type;
    typedef JResult_t                                     result_type;
    typedef JFunctional<argument_type, result_type>       functional_type;
    typedef JSupervisor                                   supervisor_type;


    /**
     * Recursive function value evaluation.
     *
     * \param  pX              pointer to abscissa values
     * \return                 function value
     */
    virtual result_type evaluate(const argument_type* pX) const = 0;

    
    /**
     * Recursive function value evaluation.
     *
     * \param  function        function
     * \param  pX              pointer to abscissa values
     */
    static result_type getValue(const JFunctional&   function,
				const argument_type* pX)
    {
      return function.evaluate(pX);
    }


    /**
     * Termination of recursive function value evaluation.
     *
     * \param  value           result
     * \param  pX              pointer to abscissa values
     */
    static   typename JClass<result_type>::argument_type 
    getValue(typename JClass<result_type>::argument_type value,
	     const argument_type* pX)
    {
      return value;
    }


    /**
     * Exception handler for functional object.
     */
    struct JExceptionHandler
    {
      /**
       * Default constructor.
       */
      JExceptionHandler()
      {}


      /**
       * Virtual destructor.
       */
      virtual ~JExceptionHandler()
      {}
      
      
      /**
       * Implementation of exception handler.
       * This implementation throws the exception.
       *
       * \param  error           error
       */
      virtual result_type action(const JException& error) const
      {
	throw error;
      }
    };


    /**
     * Exception handler for functional object using default result.
     */
    struct JDefaultResult :
      public JExceptionHandler
    {
      /**
       * Constructor.
       * 
       * \param  value             default result in case of exception
       */
      JDefaultResult(const result_type value) :
	JExceptionHandler(),
	defaultResult(value)
      {}


      /**
       * Constructor.
       * 
       * \param  value             default result in case of exception
       */
      JDefaultResult(const JMATH::JZero& value) :
	JExceptionHandler(),
	defaultResult()
      {}


      /**
       * Implementation of exception handler.
       * This implementation returns the default value.
       *
       * \param  error             error
       * \return                   default value
       */
      virtual result_type action(const JException& error) const override 
      {
	return defaultResult;
      }
      
    private:
      result_type defaultResult;    
    };


    /**
     * Place holder for exception handler.
     */
    class JSupervisor :
      public JSharedPointer<JExceptionHandler> 
    {

      typedef JSharedPointer<JExceptionHandler>  supervisor_type;

    public:
      /**
       * Default constructor.
       */
      JSupervisor() :
	supervisor_type(new JExceptionHandler())
      {}

      
      /**
       * Constructor
       *
       * \param  exception_handler  pointer to exception handler
       */
      JSupervisor(JExceptionHandler* exception_handler) :
	supervisor_type(exception_handler)
      {}


      /**
       * Get reference to unique instance of this class object.
       *
       * \return                   supervisor
       */
      static const JSupervisor& getInstance()
      {
	static const JSupervisor supervisor;
	
	return supervisor;
      }


      /**
       * Set exception handler of given functional object.
       *
       * \param  function          function
       * \return                   this supervisor
       */
      const JSupervisor& operator()(functional_type& function) const
      {
	function.setExceptionHandler(*this);

	return *this;
      }
    };


    /**
     * Get supervisor.
     *
     * \return                   supervisor
     */
    JSupervisor getSupervisor() const
    {
      return supervisor;
    }


    /**
     * Get exception handler.
     *
     * \return                   exception handler
     */
    const JExceptionHandler& getExceptionHandler() const
    {
      return *supervisor;
    }


    /**
     * Set the supervisor for handling of exceptions.
     *
     * \param  supervisor        supervisor
     */
    void setExceptionHandler(const JSupervisor& supervisor)
    {
      this->supervisor = supervisor;
    }


  protected:
    JSupervisor supervisor;
  };


  /**
   * Template definition of function object interface in multidimensions.
   */
  template<class JArgument_t, class JResult_t>
  struct JFunction : 
    public virtual JFunctional<JArgument_t, JResult_t>
  {
    typedef JFunctional<JArgument_t, JResult_t>                         functional_type;
    typedef typename functional_type::argument_type                     argument_type;
    typedef typename functional_type::result_type                       result_type;
  };


  /**
   * Template definition of function object interface in one dimension.
   * This class provides for the standard function operator <tt>()</tt>.
   */
  template<class JArgument_t, class JResult_t>
  struct JFunction1D : 
    public JFunction<JArgument_t, JResult_t>
  {
    enum { NUMBER_OF_DIMENSIONS = 1 };

    typedef JFunctional<JArgument_t, JResult_t>                         functional_type;
    typedef typename functional_type::argument_type                     argument_type;
    typedef typename functional_type::result_type                       result_type;


    /**
     * Function value evaluation.
     *
     * \param  x               argument value
     * \return                 function value
     */
    result_type operator()(const argument_type x) const 
    {
      return this->evaluate(&x);
    }
  };


  /**
   * Functional object compiler.
   */
  struct JCompiler {
    /**
     * Default constructor.
     */
    JCompiler()
    {}


    /**    
     * Compile function.
     *
     * \param  function          function
     * \return                   this compiler
     */
    const JCompiler& operator()(JFunctional<>& function) const
    {
      function.compile();

      return *this;
    }
  };

  
  /**
   * Function object for functional object compilation.
   */
  static const JCompiler compiler;


  /**
   * Auxiliary class to evaluate result type.
   * The result type is the actual data type.
   */
  template<class JClass_t, class JResultType_t = void>
  struct JResultType {

    typedef JClass_t                           result_type;
  };


  /**
   * Auxiliary class to evaluate result type.
   * The result type is the result type of the function object.
   */
  template<class JClass_t>
  struct JResultType<JClass_t, typename JVoid<typename JClass_t::result_type>::type> {

    typedef typename JClass_t::result_type     result_type;
  };
}

#endif
