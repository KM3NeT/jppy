#ifndef __JTOOLS__JMULTIMAP__
#define __JTOOLS__JMULTIMAP__

#include "JLang/JEquals.hh"
#include "JLang/JForwardIterator.hh"

#include "JMath/JMath.hh"

#include "JTools/JDistance.hh"
#include "JTools/JPair.hh"
#include "JTools/JMultiPair.hh"
#include "JTools/JMapList.hh"
#include "JTools/JAbstractMultiMap.hh"
#include "JTools/JMultiKey.hh"


/**
 * \file
 *
 * General purpose multidimensional map based on a type list of maps.
 * \author mdejong
 */
namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JMATH::JMath;
  using JLANG::JEquals;
  using JLANG::JForwardIterator;


  /**
   * Multidimensional map.
   *
   * The first template parameter refers to the data type of the (multidimensional) key; 
   * the second to the data type of the overall mapped value;
   * the third to the list of maps used; and
   * the fourth to the distance operator.
   *
   * In addition to the standard STL iterators, 
   * there are super iteretors for linear access to the multidimensional map.\n
   * The access from a super iterator to the actual elements in the multidimensional map
   * is handled via the usual dereference and pointer operators.\n
   * The multidimensional key (see JTOOLS::JMultiKey) and value can directly be obtained
   * via member methods <tt>%getKey()</tt> and <tt>%getValue()</tt>, respectively.
   */
  template<class JAbscissa_t,
	   class JOrdinate_t,
	   class JMaplist_t,
	   class JDistance_t = JDistance<JAbscissa_t> >
  class JMultiMap;


  /**
   * Template specialisation of JMultiMap for map list with at least one map.
   */
  template<class JAbscissa_t, 
	   class JOrdinate_t, 
	   template<class, class, class> class JHead_t, 
	   class JTail_t, 
	   class JDistance_t>
  class JMultiMap<JAbscissa_t, JOrdinate_t, JMapList<JHead_t, JTail_t>, JDistance_t> :
    public JHead_t<JAbscissa_t, JMultiMap<JAbscissa_t, JOrdinate_t, JTail_t, JDistance_t>, JDistance_t>,
    public JMath< JMultiMap<JAbscissa_t, JOrdinate_t, JMapList<JHead_t, JTail_t>, JDistance_t> >
  {
  public:

    enum { NUMBER_OF_DIMENSIONS = JMapLength< JMapList<JHead_t, JTail_t> >::value };
    
    typedef JHead_t<JAbscissa_t,
		    JMultiMap<JAbscissa_t, JOrdinate_t, JTail_t, JDistance_t>,
		    JDistance_t>                                               map_type;

    typedef JAbscissa_t                                                        abscissa_type;
    typedef JOrdinate_t                                                        ordinate_type;

    typedef typename map_type::key_type                                        key_type;
    typedef typename map_type::mapped_type                                     mapped_type;
    typedef typename map_type::value_type                                      value_type;

    typedef typename map_type::const_iterator                                  const_iterator;
    typedef typename map_type::const_reverse_iterator                          const_reverse_iterator;
    typedef typename map_type::iterator                                        iterator;
    typedef typename map_type::reverse_iterator                                reverse_iterator;

    using map_type::insert; 
    using map_type::configure; 
    using map_type::get; 


    /**
     * Default constructor.
     */
    JMultiMap()
    {}


    /**
     * Add map.
     *
     * \param  map             multimap
     * \return                 this multimap
     */
    JMultiMap& add(const JMultiMap& map)
    {
      static_cast<map_type&>(*this).add(static_cast<const map_type&>(map));

      return *this;
    }
 

    /**
     * Subtract map.
     *
     * \param  map             multimap
     * \return                 this multimap
     */
    JMultiMap& sub(const JMultiMap& map)
    {
      static_cast<map_type&>(*this).sub(static_cast<const map_type&>(map));

      return *this;
    }


    /**
     * Scale contents.
     *
     * \param  value           multiplication factor
     * \return                 this multimap
     */
    JMultiMap& mul(const double value)
    {
      static_cast<map_type&>(*this).mul(value);

      return *this;
    }

    /**
     * Scale contents.
     *
     * \param  value           division factor
     * \return                 this multimap
     */
    JMultiMap& div(const double value)
    {
      static_cast<map_type&>(*this).div(value);

      return *this;
    }


    /**
     * Function application to each element of this multimap. 
     *
     * \param  function        function
     * \return                 function
     */
    template<class JFunction_t>
    void for_each(const JFunction_t& function)
    {
      function(*this);

      for (iterator i = this->begin(); i != this->end(); ++i) {
	i->getY().for_each(function);
      }
    }


    /**
     * Configure multidimensional map.
     *
     * \param  bounds          multidimensional bounds
     */
    void configure(const JAbstractMultiMap<NUMBER_OF_DIMENSIONS, abscissa_type>& bounds)
    {
      this->configure(JMultiKey<0, abscissa_type>(), bounds);
    }
    

    /**
     * Configure multidimensional map.
     *
     * \param  key             multidimensional key
     * \param  bounds          multidimensional bounds
     */
    template<unsigned int N, unsigned int M>
    void configure(const JMultiKey<M, abscissa_type>& key, const JAbstractMultiMap<N, abscissa_type>& bounds)
    {
      this->configure(bounds(key));

      for (iterator i = this->begin(); i != this->end(); ++i) {
        i->getY().configure(JMultiKey<M+1, abscissa_type>(key,i->getX()), bounds);
      }
    }

  private:    

    /**
     * Base class for multidimensional iterator.
     */
    template<class first_iterator, class second_iterator>
    struct iterator_base :
      public JEquals< iterator_base<first_iterator, second_iterator> >
    {
      typedef std::pair<first_iterator, first_iterator>                          range_type;
      typedef typename second_iterator::value_type                               value_type;
      typedef JMultiKey <NUMBER_OF_DIMENSIONS, const abscissa_type>              multikey_type;
      typedef JMultiPair<NUMBER_OF_DIMENSIONS, const abscissa_type, value_type>  multipair_type;


      /**
       * Auxiliary class for smart pointer.
       */
      struct pointer_type :
	private JPair<const key_type, second_iterator&>
      {
	/**
	 * Constructor.
	 *
	 * \param  key             key
	 * \param  value           value
	 */
	pointer_type(const key_type key, second_iterator& value) :
	  JPair<const key_type, second_iterator&>(key, value)
	{}


	/**
	 * Smart pointer operator.
	 *
	 * \return          pointer to object
	 */
	const JPair<const key_type, second_iterator&>* operator->() const
	{
	  return this;
	}
      };


      /**
       * Smart pointer operator.
       *
       * \return                 pointer to pair of iterators
       */
      pointer_type operator->()
      {
	return pointer_type(i->getX(), second);
      }


      /**
       * Dereference operator.
       *
       * \return                 multidimensional pair
       */
      multipair_type operator*()
      {
	return multipair_type(i->getX(), *second);
      }

      
      /**
       * Equality of super iterator.
       *
       * \param  cursor          super iterator
       * \return                 true if equal; else false
       */
      bool equals(const iterator_base& cursor) const
      {
	return i == cursor.i && (i == range.second || second.equals(cursor.second));
      }

      
      /**
       * Get multidimensional key.
       *
       * \return                 key
       */
      multikey_type getKey() const
      {
	return multikey_type(i->getX(), second.getKey());
      }

      
      /**
       * Get value.
       *
       * \return                 value
       */
      value_type& getValue() 
      {
	return this->second.getValue();
      }


    protected:
      range_type      range;
      first_iterator  i;
      second_iterator second;
    };


    /**
     * Helper class for multidimensional iterator.
     */
    template<class first_iterator, class second_iterator>
    struct iterator_helper :
      public iterator_base<first_iterator, second_iterator>,
      public JForwardIterator< iterator_helper<first_iterator, second_iterator> >
    {
      /**
       * Default constructor.
       */
      iterator_helper()
      {}


      /**
       * Constructor.
       *
       * \param  __begin         begin of data
       * \param  __end           end   of data
       */
      iterator_helper(first_iterator __begin,
		      first_iterator __end)
      {
	this->range = std::make_pair(__begin, __end);

	for (this->i = this->range.first; this->i != this->range.second; ++(this->i)) {

	  this->second = this->i->getY().super_begin();

	  if (this->second != this->i->getY().super_end()) {
	    break;
	  }
	}
      }


      /**
       * Increment super iterator.
       *
       * \return                 true if valid; else false
       */
      virtual bool increment() override
      {
	if (!this->second.increment()) {

	  while (++(this->i) != this->range.second) {

	    this->second = this->i->getY().super_begin();
	    
	    if (this->second != this->i->getY().super_end()) {
	      break;
	    }
	  }
	}

	return this->i != this->range.second;
      }
    };


    /**
     * Helper class for multidimensional reverse iterator.
     */
    template<class first_iterator, class second_iterator>
    struct reverse_iterator_helper :
      public iterator_base<first_iterator, second_iterator>,
      public JForwardIterator< reverse_iterator_helper<first_iterator, second_iterator> >
    {
      /**
       * Default constructor.
       */
      reverse_iterator_helper()
      {}
      

      /**
       * Constructor.
       *
       * \param  __begin         begin of data
       * \param  __end           end   of data
       */
      reverse_iterator_helper(first_iterator __begin,
			      first_iterator __end)
      {
	this->range = std::make_pair(__begin, __end);

	for (this->i = this->range.first; this->i != this->range.second; ++(this->i)) {

	  this->second = this->i->getY().super_rbegin();

	  if (this->second != this->i->getY().super_rend()) {
	    break;
	  }
	}
      }


      /**
       * Increment super iterator.
       *
       * \return                 true if valid; else false
       */
      virtual bool increment() override
      {
	if (!this->second.increment()) {

	  while (++(this->i) != this->range.second) {

	    this->second = this->i->getY().super_rbegin();
	    
	    if (this->second != this->i->getY().super_rend()) {
	      break;
	    }
	  }
	}

	return this->i != this->range.second;
      }
    };

  public:    

    class super_const_iterator;  // forward declaration


    /**
     * Multidimensional iterator. 
     */
    struct super_iterator :
      public iterator_helper<iterator, typename mapped_type::super_iterator>
    {

      friend class JMultiMap;
      friend class super_const_iterator;

      /**
       * Default constructor.
       */
      super_iterator()
      {}

    private:
      /**
       * Constructor.
       *
       * \param  __begin         begin of data
       * \param  __end           end   of data
       */
      super_iterator(iterator __begin,
		     iterator __end) :
	iterator_helper<iterator, typename mapped_type::super_iterator>(__begin, __end)
      {}
    };


    /**
     * Multidimensional const_iterator. 
     */
    struct super_const_iterator :
      public iterator_helper<const_iterator, typename mapped_type::super_const_iterator>,
      public JEquals<super_const_iterator, super_iterator>
    {

      friend class JMultiMap;

      /**
       * Default constructor.
       */
      super_const_iterator()
      {}


      /**
       * Copy constructor.
       *
       * \param  cursor          super iterator
       */
      super_const_iterator(const super_iterator& cursor)
      {
	this->range  = cursor.range;
	this->i      = cursor.i;
	this->second = cursor.second;
      }

      
      /**
       * Equality of super iterator.
       *
       * \param  cursor          super iterator
       * \return                 true if equal; else false
       */
      bool equals(const super_const_iterator& cursor) const
      {
	return static_cast<const iterator_base<const_iterator, typename mapped_type::super_const_iterator>&>(*this).equals(cursor);
      }

      
      /**
       * Equality of super iterator.
       *
       * \param  cursor          super iterator
       * \return                 true if equal; else false
       */
      bool equals(const super_iterator& cursor) const
      {
	return equals(super_const_iterator(cursor));
      }

    private:
      /**
       * Constructor.
       *
       * \param  __begin         begin of data
       * \param  __end           end   of data
       */
      super_const_iterator(const_iterator __begin,
			   const_iterator __end) :
	iterator_helper<const_iterator, typename mapped_type::super_const_iterator>(__begin, __end)
      {}
    };

    
    class super_const_reverse_iterator;  // forward declaration


    /**
     * Multidimensional reverse iterator. 
     */
    struct super_reverse_iterator :
      public reverse_iterator_helper<reverse_iterator, typename mapped_type::super_reverse_iterator>
    {

      friend class JMultiMap;
      friend class super_const_reverse_iterator;

      /**
       * Default constructor.
       */
      super_reverse_iterator()
      {}

    private:
      /**
       * Constructor.
       *
       * \param  __begin         reverse begin of data
       * \param  __end           reverse end   of data
       */
      super_reverse_iterator(reverse_iterator __begin,
			     reverse_iterator __end) :
	reverse_iterator_helper<reverse_iterator, typename mapped_type::super_reverse_iterator>(__begin, __end)
      {}
    };


    /**
     * Multidimensional const reverse iterator. 
     */
    struct super_const_reverse_iterator :
      public reverse_iterator_helper<const_reverse_iterator, typename mapped_type::super_const_reverse_iterator>,
      public JEquals<super_const_reverse_iterator, super_reverse_iterator>
    {

      friend class JMultiMap;

      /**
       * Default constructor.
       */
      super_const_reverse_iterator()
      {}


      /**
       * Copy constructor.
       *
       * \param  cursor          super reverse iterator
       */
      super_const_reverse_iterator(super_reverse_iterator cursor)
      {
	this->range  = cursor.range;
	this->i      = cursor.i;
	this->second = cursor.second;
      }

      
      /**
       * Equality of super reverse iterator.
       *
       * \param  cursor          super reverse iterator
       * \return                 true if equal; else false
       */
      bool equals(const super_const_reverse_iterator& cursor) const
      {
	return static_cast<const iterator_base<const_reverse_iterator, typename mapped_type::super_const_reverse_iterator>&>(*this).equals(cursor);
      }

      
      /**
       * Equality of super reverse iterator.
       *
       * \param  cursor          super reverse iterator
       * \return                 true if equal; else false
       */
      bool equals(const super_reverse_iterator& cursor) const
      {
	return equals(super_const_reverse_iterator(cursor));
      }

    private:
      /**
       * Constructor.
       *
       * \param  __begin         begin of data
       * \param  __end           end   of data
       */
      super_const_reverse_iterator(const_reverse_iterator __begin,
				   const_reverse_iterator __end) :
	reverse_iterator_helper<const_reverse_iterator, typename mapped_type::super_const_reverse_iterator>(__begin, __end)
      {}
    };


    /**
     * Get super iterator to begin of data.
     * 
     * \return                 super iterator
     */
    super_const_iterator super_begin() const
    {
      return super_const_iterator(this->begin(), this->end());
    }


    /**
     * Get super iterator to reverse begin of data
     *
     * \return                  super reverse iterator
     */
    super_const_reverse_iterator super_rbegin() const
    {
      return super_const_reverse_iterator(this->rbegin(), this->rend());
    }
 
   
    /**
     * Get super iterator to end of data.
     *
     * \return                 super iterator
     */
    super_const_iterator super_end() const
    {
      return super_const_iterator(this->end(), this->end());
    }


    /**
     * Get super iterator to reverse end of data.
     *
     * \return                 super reverse iterator
     */
    super_const_reverse_iterator super_rend() const
    {
      return super_const_reverse_iterator(this->rend(), this->rend());
    }


    /**
     * Get super iterator to begin of data.
     *
     * \return                 super iterator
     */
    super_iterator super_begin()
    {
      return super_iterator(this->begin(), this->end());
    }


    /**
     * Get super iterator to reverse begin of data.
     *
     * \return                 super iterator
     */
    super_reverse_iterator super_rbegin()
    {
      return super_reverse_iterator(this->rbegin(), this->rend());
    }    
 
   
    /**
     * Get super iterator to end of data.
     *
     * \return                 super iterator
     */
    super_iterator super_end()
    {
      return super_iterator(this->end(), this->end());
    }


    /**
     * Get super iterator to reverse end of data.
     *
     * \return                 super iterator
     */
    super_reverse_iterator super_rend()
    {
      return super_reverse_iterator(this->rend(), this->rend());
    }    


    /**
     * Get value.
     *
     * \param  key             multidimensional key
     * \return                 value
     */
    ordinate_type& get(const JMultiKey<NUMBER_OF_DIMENSIONS, const abscissa_type>& key)
    {
      return this->get(key.first).get(key.second);
    }


    /**
     * Insert element.
     *
     * \param  key             multidimensional key
     * \param  value           value
     */
    void insert(const JMultiKey<NUMBER_OF_DIMENSIONS, const abscissa_type>& key, const ordinate_type& value)
    {
      (*this)[key.first].insert(key.second, value);
    }


    /**
     * Insert element.
     *
     * \param  value           multidimensional pair
     */
    void insert(const JMultiPair<NUMBER_OF_DIMENSIONS, const abscissa_type, const ordinate_type&>& value)
    {
      (*this)[value.first].insert(value.second);
    }


    /**
     * Insert element.
     *
     * \param  value           multidimensional pair
     */
    void insert(const JMultiPair<NUMBER_OF_DIMENSIONS, const abscissa_type, ordinate_type&>& value)
    {
      (*this)[value.first].insert(value.second);
    }
  };


  /**
   * Terminator class of recursive JMultiMap class.
   */
  template<class JAbscissa_t,
	   class JOrdinate_t,
	   template<class, class, class> class JHead_t,
	   class JDistance_t>
  class JMultiMap<JAbscissa_t, JOrdinate_t, JMapList<JHead_t, JLANG::JNullType>, JDistance_t> :
    public JHead_t<JAbscissa_t, JOrdinate_t, JDistance_t>,
    public JMath< JMultiMap<JAbscissa_t, JOrdinate_t, JMapList<JHead_t, JLANG::JNullType>, JDistance_t> >
  {
  public:

    enum { NUMBER_OF_DIMENSIONS = 1 };

    typedef JHead_t<JAbscissa_t, JOrdinate_t, JDistance_t>                     map_type;
    
    typedef JAbscissa_t                                                        abscissa_type;
    typedef JOrdinate_t                                                        ordinate_type;

    typedef typename map_type::key_type                                        key_type;
    typedef typename map_type::mapped_type                                     mapped_type;
    typedef typename map_type::value_type                                      value_type;

    typedef typename map_type::const_iterator                                  const_iterator;
    typedef typename map_type::const_reverse_iterator                          const_reverse_iterator;
    typedef typename map_type::iterator                                        iterator;
    typedef typename map_type::reverse_iterator                                reverse_iterator;
     
    using map_type::insert; 
    using map_type::configure; 
    using map_type::get; 


    /**
     * Default constructor.
     */
    JMultiMap() 
    {}


    /**
     * Add map.
     *
     * \param  map             multimap
     * \return                 this multimap
     */
    JMultiMap& add(const JMultiMap& map)
    {
      static_cast<map_type&>(*this).add(static_cast<const map_type&>(map));

      return *this;
    }
 

    /**
     * Subtract map.
     *
     * \param  map             multimap
     * \return                 this multimap
     */
    JMultiMap& sub(const JMultiMap& map)
    {
      static_cast<map_type&>(*this).sub(static_cast<const map_type&>(map));

      return *this;
    }


    /**
     * Scale contents.
     *
     * \param  value           multiplication factor
     * \return                 this multimap
     */
    JMultiMap& mul(const double value)
    {
      static_cast<map_type&>(*this).mul(value);

      return *this;
    }

    /**
     * Scale contents.
     *
     * \param  value           division factor
     * \return                 this multimap
     */
    JMultiMap& div(const double value)
    {
      static_cast<map_type&>(*this).div(value);

      return *this;
    }


    /**
     * Termination of function application.
     *
     * \param  function        function
     * \return                 function
     */
    template<class JFunction_t>
    void for_each(const JFunction_t& function)
    {
      function(*this);
    }
    

    /**
     * Configure multidimensional map.
     *
     * \param  bounds          multidimensional bounds
     */
    void configure(const JAbstractMultiMap<NUMBER_OF_DIMENSIONS, abscissa_type>& bounds)
    {
      this->configure(JMultiKey<0, abscissa_type>(), bounds);
    }


    /**
     * Configure multidimensional map.
     *
     * \param  key             multidimensional key
     * \param  bounds          multidimensional bounds
     */
    template<unsigned int N, unsigned int M>
    void configure(const JMultiKey<M, abscissa_type>& key, const JAbstractMultiMap<N, abscissa_type>& bounds)
    {
      this->configure(bounds(key));
    }

  private:

    /**
     * Helper class for multidimensional iterator.
     */
    template<class iterator_type, class ordinate_type>
    struct iterator_helper :
      public JEquals         < iterator_helper<iterator_type, ordinate_type> >,
      public JForwardIterator< iterator_helper<iterator_type, ordinate_type> >
    {
      typedef std::pair<iterator_type, iterator_type>                            range_type;
      typedef ordinate_type                                                      value_type;
      typedef JMultiKey <NUMBER_OF_DIMENSIONS, const abscissa_type>              multikey_type;
      typedef JMultiPair<NUMBER_OF_DIMENSIONS, const abscissa_type, value_type>  multipair_type;


      /**
       * Auxiliary class for pair via smart pointer.
       */
      struct pointer_type :
	private JPair<const key_type, value_type>
      {
	/**
	 * Constructor.
	 *
	 * \param  key             key
	 * \param  value           value
	 */
	pointer_type(const key_type key, value_type value) :
	  JPair<const key_type, value_type>(key, value)
	{}


	/**
	 * Smart pointer operator.
	 *
	 * \return          pointer to object
	 */
	const JPair<const key_type, value_type>* operator->() const
	{
	  return this;
	}
      };
    

      /**
       * Default constructor.
       */
      iterator_helper()
      {}
      

      /**
       * Constructor.
       *
       * \param  __begin         begin of data
       * \param  __end           end   of data
       */
      iterator_helper(iterator_type __begin,
		      iterator_type __end) :
	range(__begin, __end),
	i(__begin)
      {}


      /**
       * Smart pointer operator.
       *
       * \return                 pointer to pair of iterators
       */
      pointer_type operator->()
      {
	return pointer_type(i->getX(), i->getY());
      }


      /**
       * Dereference operator.
       *
       * \return                 multidimensional pair
       */
      multipair_type operator*()
      { 
	return multipair_type(i->getX(), i->getY());
      }

      
      /**
       * Equality of super iterator.
       *
       * \param  cursor          super iterator
       * \return                 true if equal; else false
       */
      bool equals(const iterator_helper& cursor) const
      {
	return i == cursor.i;
      }


      /**
       * Increment super iterator.
       *
       * \return                 true if valid; else false
       */
      virtual bool increment() override
      {
	return ++i != range.second;
      }


      /**
       * Get multidimensional key.
       *
       * \return                 key
       */
      multikey_type getKey() const
      {
	return multikey_type(i->getX());
      }

      
      /**
       * Get value.
       *
       * \return                 value
       */
      value_type getValue() 
      {
	return i->getY();
      }

      
      range_type    range;
      iterator_type i;
    };

  public:

    class super_const_iterator;  // forward declaration


    /**
     * Terminator class of multidimensional iterator. 
     */
    struct super_iterator :
      public iterator_helper<iterator, ordinate_type&>
    {

      friend class JMultiMap;
      friend class super_const_iterator;

      /**
       * Default constructor.
       */
      super_iterator()
      {}

    private:
      /**
       * Constructor.
       *
       * \param  __begin         begin of data
       * \param  __end           end   of data
       */
      super_iterator(iterator __begin,
		     iterator __end) :
	iterator_helper<iterator, ordinate_type&>(__begin, __end)
      {}
    };


    /**
     * Terminator class of multidimensional const_iterator. 
     */
    struct super_const_iterator :
      public iterator_helper<const_iterator, const ordinate_type&>,
      public JEquals<super_const_iterator, super_iterator>
    {

      friend class JMultiMap;

      /**
       * Default constructor.
       */
      super_const_iterator()
      {}


      /**
       * Copy constructor.
       *
       * \param  cursor          super iterator
       */
      super_const_iterator(super_iterator cursor)
      {
	this->range = cursor.range;
	this->i     = cursor.i;
      }

      
      /**
       * Equality of super iterator.
       *
       * \param  cursor          super iterator
       * \return                 true if equal; else false
       */
      bool equals(const super_const_iterator& cursor) const
      {
	return this->i == cursor.i;
      }

      
      /**
       * Equality of super iterator.
       *
       * \param  cursor          super iterator
       * \return                 true if equal; else false
       */
      bool equals(const super_iterator& cursor) const
      {
	return this->i == cursor.i;
      }

    private:
      /**
       * Constructor.
       *
       * \param  __begin         begin of data
       * \param  __end           end   of data
       */
      super_const_iterator(const_iterator __begin,
			   const_iterator __end) :
	iterator_helper<const_iterator, const ordinate_type&>(__begin, __end)
      {}
    };


    class super_const_reverse_iterator;  // forward declaration


    /**
     * Terminator class of multidimensional reverse iterator. 
     */
    struct super_reverse_iterator :
      public iterator_helper<reverse_iterator, ordinate_type&>
    {

      friend class JMultiMap;
      friend class super_const_reverse_iterator;

      /**
       * Default constructor.
       */
      super_reverse_iterator()
      {}

    private:
      /**
       * Constructor.
       *
       * \param  __begin         begin of data
       * \param  __end           end   of data
       */
      super_reverse_iterator(reverse_iterator __begin,
			     reverse_iterator __end) :
	iterator_helper<reverse_iterator, ordinate_type&>(__begin, __end)
      {}
    };


    /**
     * Terminator class of multidimensional const_iterator. 
     */
    struct super_const_reverse_iterator :
      public iterator_helper<const_reverse_iterator, const ordinate_type&>,
      public JEquals<super_const_reverse_iterator, super_reverse_iterator>
    {

      friend class JMultiMap;

      /**
       * Default constructor.
       */
      super_const_reverse_iterator()
      {}


      /**
       * Copy constructor.
       *
       * \param  cursor          super reverse iterator
       */
      super_const_reverse_iterator(super_reverse_iterator cursor)
      {
	this->range = cursor.range;
	this->i     = cursor.i;
      }

      
      /**
       * Equality of super reverse iterator.
       *
       * \param  cursor          super reverse iterator
       * \return                 true if equal; else false
       */
      bool equals(const super_const_reverse_iterator& cursor) const
      {
	return this->i == cursor.i;
      }

      
      /**
       * Equality of super reverse iterator.
       *
       * \param  cursor          super reverse iterator
       * \return                 true if equal; else false
       */
      bool equals(const super_reverse_iterator& cursor) const
      {
	return this->i == cursor.i;
      }

    private:
      /**
       * Constructor.
       *
       * \param  __begin         begin of data
       * \param  __end           end   of data
       */
      super_const_reverse_iterator(const_reverse_iterator __begin,
				   const_reverse_iterator __end) :
	iterator_helper<const_reverse_iterator, const ordinate_type&>(__begin, __end)
      {}
    };


    /**
     * Get super iterator to begin of data.
     *
     * \return             super iterator
     */
    super_const_iterator super_begin() const
    {
      return super_const_iterator(this->begin(), this->end());
    }


    /**
     * Get super iterator to reverse begin of data.
     *
     * \return             super reverse iterator
     */
    super_const_reverse_iterator super_rbegin() const
    {
      return super_const_reverse_iterator(this->rbegin(), this->rend());
    }
 
   
    /**
     * Get super iterator to end of data.
     *
     * \return             super iterator
     */
    super_const_iterator super_end() const
    {
      return super_const_iterator(this->end(), this->end());
    }

    
    /**
     * Get super iterator to reverse end of data.
     *
     * \return             super reverse iterator
     */
    super_const_reverse_iterator super_rend() const
    {
      return super_const_reverse_iterator(this->rend(), this->rend());
    }


    /**
     * Get super iterator to begin of data.
     *
     * \return             super iterator
     */
    super_iterator super_begin()
    {
      return super_iterator(this->begin(), this->end());
    }


    /**
     * Get super iterator to reverse begin of data.
     *
     * \return             super reverse iterator
     */
    super_reverse_iterator super_rbegin()
    {
      return super_reverse_iterator(this->rbegin(), this->rend());
    }
 
   
    /**
     * Get super iterator to end of data.
     *
     * \return             super iterator
     */
    super_iterator super_end()
    {
      return super_iterator(this->end(), this->end());
    }
 
   
    /**
     * Get super iterator to reverse end of data.
     *
     * \return             super reverse iterator
     */
    super_reverse_iterator super_rend()
    {
      return super_reverse_iterator(this->rend(), this->rend());
    }


    /**
     * Get value.
     *
     * \param  key             multidimensional key
     * \return                 value
     */
    ordinate_type& get(const JMultiKey<NUMBER_OF_DIMENSIONS, abscissa_type>& key)
    {
      return get(key.first);
    }


    /**
     * Insert element.
     *
     * \param  key          multidimensional key
     * \param  value        value
     */
    void insert(const JMultiKey<NUMBER_OF_DIMENSIONS, const abscissa_type>& key, const ordinate_type& value)
    {
      insert(value_type(key.first, value));
    }


    /**
     * Insert element.
     *
     * \param  value        multidimensional pair
     */
    void insert(const JMultiPair<NUMBER_OF_DIMENSIONS, const abscissa_type, const ordinate_type&>& value)
    {
      insert(value_type(value.first, value.second));
    }


    /**
     * Insert element.
     *
     * \param  value        multidimensional pair
     */
    void insert(const JMultiPair<NUMBER_OF_DIMENSIONS, const abscissa_type, ordinate_type&>& value)
    {
      insert(value_type(value.first, value.second));
    }
  };
}

#endif
