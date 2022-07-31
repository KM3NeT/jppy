#ifndef __JLANG__JMEMORY__
#define __JLANG__JMEMORY__

#include <stdlib.h>


/**
 * \file
 *
 * Base class for memory management.
 * \author mdejong
 */
namespace JLANG {}
namespace JPP { using namespace JLANG; }

namespace JLANG {


  /**
   * Memory management class for create/release policy based on new/delete.
   */
  template<class JClass_t>
  class JNew {
  public:
    /**
     * Create object in memory.
     *
     * \return           pointer to data
     */
    static inline JClass_t* create()
    { 
      return new JClass_t(); 
    }
    

    /**
     * Release memory.
     *
     * \param  p         pointer to data
     */
    static inline void release(JClass_t* p) 
    {
      delete p;
    }
  };
  

  /**
   * Memory management class for create/release policy based on new []/delete [].
   */
  template<class JClass_t>
  class JNewCArray {
  public:
    /**
     * Create array of objects in memory.
     *
     * \param  size      number of elements
     * \return           pointer to data
     */
    static inline JClass_t* create(const unsigned int size)
    { 
      return new JClass_t[size];
    }


    /**
     * Release memory.
     *
     * \param  p         pointer to data
     */
    static inline void release(JClass_t* p)
    { 
      delete [] p;
    }
  };
  

  /**
   * Memory management class for create/release policy based on malloc()/free().
   */
  template<class JClass_t>
  class JMalloc {
  public:
    /**
     * Create object in memory.
     *
     * \return           pointer to data
     */
    static inline JClass_t* create()
    { 
      return (JClass_t*) malloc(sizeof(JClass_t));
    }


    /**
     * Create array of objects in memory.
     *
     * \param  size      number of elements
     * \return           pointer to data
     */
    static inline JClass_t* create(const unsigned int size)
    { 
      return (JClass_t*) malloc(size * sizeof(JClass_t));
    }


    /**
     * Release memory.
     *
     * \param  p         pointer to data
     */
    static inline void release(JClass_t* p)
    { 
      free((void*) p);
    }
  };
}


#endif
