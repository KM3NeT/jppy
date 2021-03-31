#ifndef __JTOOLS__JMULTIMAP__
#define __JTOOLS__JMULTIMAP__

#include "JLang/JSinglePointer.hh"
#include "JLang/JEquals.hh"
#include "JLang/JForwardIterator.hh"

#include "JTools/JDistance.hh"
#include "JTools/JPair.hh"
#include "JTools/JMultiPair.hh"
#include "JTools/JMapList.hh"
#include "JTools/JAbstractMultiMap.hh"
#include "JTools/JMultiKey.hh"
#include "JMath/JMath.hh"


/**
 * \file
 *
 * JMultiMap is a general purpose multidimensional map based on a type list of maps.
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
   * In addition to the standard STL iterators, there is a 
   * super_[const_[reverse_]]iterator for linear access to the multidimensional map.
   * The access from the super_iterator to the actual elements in the multidimensional map
   * is handeld via a the standard dereference and pointer operators yielding 
   * a multidimensional key (see JTOOLS::JMultiKey) or pair (see JTOOLS::JMultiPair), respectively.
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
		    JDistance_t>                             map_type;
    
    typedef JAbscissa_t                                      abscissa_type;
    typedef JOrdinate_t                                      ordinate_type;

    typedef typename map_type::key_type                      key_type;
    typedef typename map_type::mapped_type                   mapped_type;
    typedef typename map_type::value_type                    value_type;

    typedef typename map_type::const_iterator                const_iterator;
    typedef typename map_type::const_reverse_iterator        const_reverse_iterator;
    typedef typename map_type::iterator                      iterator;
    typedef typename map_type::reverse_iterator              reverse_iterator;

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

    
    class super_const_iterator;  // forward declaration


    /**
     * Multidimensional iterator. 
     */
    class super_iterator :
      public JEquals         <super_iterator>,
      public JForwardIterator<super_iterator>
    {

      friend class JMultiMap;
      friend class super_const_iterator;

    public:

      typedef int                                                                difference_type;
      typedef JPair<const key_type&, typename mapped_type::super_iterator&>      value_type;
      typedef JLANG::JSinglePointer<value_type>                                  pointer;
      typedef JMultiPair<NUMBER_OF_DIMENSIONS, const JAbscissa_t, JOrdinate_t&>  reference;
      typedef std::forward_iterator_tag                                          iterator_category;
      typedef JMultiKey <NUMBER_OF_DIMENSIONS, const JAbscissa_t>                multikey_type;

      /**
       * Default constructor.
       */
      super_iterator()
      {}


      /**
       * Smart pointer operator.
       *
       * \return                 pointer to pair of iterators
       */
      pointer operator->()
      { 
	return pointer(new value_type(i->getX(), second));
      }


      /**
       * Dereference operator.
       *
       * \return                 multidimensional pair
       */
      reference operator*()
      {
	return reference(i->getX(), *second);
      }


      /**
       * Equality of super iterator.
       *
       * \param  cursor          super iterator
       * \return                 true if equal; else false
       */
      virtual bool equals(const super_iterator& cursor) const
      {
	return i == cursor.i && (i == range.second || second.equals(cursor.second));
      }


      /**
       * Increment super_iterator.
       *
       * \return                 true if valid; else false
       */
      virtual bool increment() override 
      {
	if (!second.increment()) {

	  while (++i != range.second) {

	    second = i->getY().super_begin();
	    
	    if (second != i->getY().super_end()) {
	      break;
	    }
	  }
	}

	return i != range.second;
      }

      
      /**
       * Get multi-dimensional key.
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
      JOrdinate_t& getValue() 
      {
	return second.getValue();
      }


    private:
      /**
       * Constructor.
       *
       * \param  __begin         begin of data
       * \param  __end           end   of data
       */
      super_iterator(iterator __begin,
		     iterator __end) :
	range(__begin, __end)
      {
	for (i = range.first; i != range.second; ++i) {

	  second = i->getY().super_begin();

	  if (second != i->getY().super_end()) {
	    break;
	  }
	}
      }


      std::pair<iterator, iterator> range;
      iterator i;
      typename mapped_type::super_iterator second;
    };


    /**
     * Multidimensional const_iterator. 
     */
    class super_const_iterator :
      public JEquals         <super_const_iterator>,
      public JForwardIterator<super_const_iterator>
    {

      friend class JMultiMap;

    public:

      typedef int                                                                      difference_type;
      typedef JPair<const key_type&, typename mapped_type::super_const_iterator&>      value_type;
      typedef JLANG::JSinglePointer<value_type>                                        pointer;
      typedef JMultiPair<NUMBER_OF_DIMENSIONS, const JAbscissa_t, const JOrdinate_t&>  reference;
      typedef std::forward_iterator_tag                                                iterator_category;      
      typedef JMultiKey <NUMBER_OF_DIMENSIONS, const JAbscissa_t>                      multikey_type;
      

      /**
       * Default constructor.
       */
      super_const_iterator()
      {}


      /**
       * Copy constructor.
       *
       * \param  cursor          super_iterator
       */
      super_const_iterator(super_iterator cursor) :
	range (cursor.range),
	i     (cursor.i),
	second(cursor.second)
      {}


      /**
       * Smart pointer operator.
       *
       * \return                 pointer to pair of iterators
       */
      pointer operator->()
      { 
	return pointer(new value_type(i->getX(), second));
      }


      /**
       * Dereference operator.
       *
       * \return                 multidimensional pair
       */
      reference operator*()
      { 
	return reference(i->getX(), *second);
      }


      /**
       * Equality of super iterator.
       *
       * \param  cursor          super iterator
       * \return                 true if equal; else false
       */
      virtual bool equals(const super_const_iterator& cursor) const
      {
	return i == cursor.i && (i == range.second || second.equals(cursor.second));
      }


      /**
       * Increment super_iterator.
       *
       * \return                 true if valid; else false
       */
      virtual bool increment() override 
      {
	if (!second.increment()) {

	  while (++i != range.second) {

	    second = i->getY().super_begin();
	    
	    if (second != i->getY().super_end()) {
	      break;
	    }
	  }
	}

	return i != range.second;
      }


      /**
       * Get multi-dimensional key.
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
      const JOrdinate_t& getValue() 
      {
	return second.getValue();
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
	range(__begin, __end)
      {
	for (i = range.first; i != range.second; ++i) {

	  second = i->getY().super_begin();

	  if (second != i->getY().super_end()) {
	    break;
	  }
	}
      }


      std::pair<const_iterator, const_iterator> range;
      const_iterator i;
      typename mapped_type::super_const_iterator second;
    };

    
    class super_const_reverse_iterator;  // forward declaration


    /**
     * Multidimensional reverse iterator. 
     */
    class super_reverse_iterator :
      public JEquals         <super_reverse_iterator>,
      public JForwardIterator<super_reverse_iterator>
    {

      friend class JMultiMap;
      friend class super_const_reverse_iterator;

    public:

      typedef int                                                                        difference_type;
      typedef JPair<const key_type&, typename mapped_type::super_reverse_iterator&>      value_type;
      typedef JLANG::JSinglePointer<value_type>                                          pointer;
      typedef JMultiPair<NUMBER_OF_DIMENSIONS, const JAbscissa_t, JOrdinate_t&>          reference;
      typedef std::forward_iterator_tag                                                  iterator_category;
      typedef JMultiKey <NUMBER_OF_DIMENSIONS, const JAbscissa_t>                        multikey_type;

      
      /**
       * Default constructor.
       */
      super_reverse_iterator()
      {}


      /**
       * Smart pointer operator.
       *
       * \return                 pointer to pair of iterators
       */
      pointer operator->()
      { 
	return pointer(new value_type(i->getX(), second));
      }


      /**
       * Dereference operator.
       *
       * \return                 multidimensional pair
       */
      reference operator*()
      {
	return reference(i->getX(), *second);
      }


      /**
       * Equality of super reverse iterator.
       *
       * \param  cursor          super reverse iterator
       * \return                 true if equal; else false
       */
      virtual bool equals(const super_reverse_iterator& cursor) const
      {
	return i == cursor.i && (i == range.second || second.equals(cursor.second));
      }


      /**
       * Increment super_iterator.
       *
       * \return                 true if valid; else false
       */
      virtual bool increment() override 
      {
	if (!second.increment()) {

	  while (++i != range.second) {

	    second = i->getY().super_rbegin();
	    
	    if (!second.equals(i->getY().super_rend())) {
	      break;
	    }
	  }
	}

	return i != range.second;
      }

      
      /**
       * Get multi-dimensional key.
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
      JOrdinate_t& getValue() 
      {
	return second.getValue();
      }


    private:
      /**
       * Constructor.
       *
       * \param  __begin         reverse begin of data
       * \param  __end           reverse end   of data
       */
      super_reverse_iterator(reverse_iterator __begin,
			     reverse_iterator __end) :
	range(__begin, __end)
      {
	for (i = range.first; i != range.second; ++i) {

	  second = i->getY().super_rbegin();

	  if (!second.equals(i->getY().super_rend())) {
	    break;
	  }
	}
      }


      std::pair<reverse_iterator, reverse_iterator> range;
      reverse_iterator i;
      typename mapped_type::super_reverse_iterator second;
    };


    /**
     * Multidimensional const_reverse_iterator. 
     */
    class super_const_reverse_iterator :
      public JEquals         <super_const_reverse_iterator>,
      public JForwardIterator<super_const_reverse_iterator>
    {

      friend class JMultiMap;

    public:

      typedef int                                                                              difference_type;
      typedef JPair<const key_type&, typename mapped_type::super_const_reverse_iterator&>      value_type;
      typedef JLANG::JSinglePointer<value_type>                                                pointer;
      typedef JMultiPair<NUMBER_OF_DIMENSIONS, const JAbscissa_t, const JOrdinate_t&>          reference;
      typedef std::forward_iterator_tag                                                        iterator_category;      
      typedef JMultiKey <NUMBER_OF_DIMENSIONS, const JAbscissa_t>                              multikey_type;
      

      /**
       * Default constructor.
       */
      super_const_reverse_iterator()
      {}


      /**
       * Copy constructor.
       *
       * \param  cursor          super_iterator
       */
      super_const_reverse_iterator(super_reverse_iterator cursor) :
	range (cursor.range),
	i     (cursor.i),
	second(cursor.second)
      {}


      /**
       * Smart pointer operator.
       *
       * \return                 pointer to pair of iterators
       */
      pointer operator->()
      { 
	return pointer(new value_type(i->getX(), second));
      }


      /**
       * Dereference operator.
       *
       * \return                 multidimensional pair
       */
      reference operator*()
      { 
	return reference(i->getX(), *second);
      }


      /**
       * Equality of super reverse iterator.
       *
       * \param  cursor          super reverse iterator
       * \return                 true if equal; else false
       */
      virtual bool equals(const super_const_reverse_iterator& cursor) const
      {
	return i == cursor.i && (i == range.second || second.equals(cursor.second));
      }


      /**
       * Increment super_iterator.
       *
       * \return                 true if valid; else false
       */
      virtual bool increment() override 
      {
	if (!second.increment()) {

	  while (++i != range.second) {

	    second = i->getY().super_rbegin();
	    
	    if (second != i->getY().super_rend()) {
	      break;
	    }
	  }
	}

	return i != range.second;
      }


      /**
       * Get multi-dimensional key.
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
      const JOrdinate_t& getValue() 
      {
	return second.getValue();
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
	range(__begin, __end)
      {
	for (i = range.first; i != range.second; ++i) {

	  second = i->getY().super_rbegin();

	  if (second != i->getY().super_rend()) {
	    break;
	  }
	}
      }


      std::pair<const_reverse_iterator, const_reverse_iterator> range;
      const_reverse_iterator i;
      typename mapped_type::super_const_reverse_iterator second;
    };


    /**
     * Get super_iterator to begin of data.
     * 
     * \return                 super iterator
     */
    super_const_iterator super_begin() const
    {
      return super_const_iterator(this->begin(), this->end());
    }


    /**
     * Get super_reverse_iterator to reverse begin of data
     *
     * \return                  super reverse iterator
     */
    super_const_reverse_iterator super_rbegin() const
    {
      return super_const_reverse_iterator(this->rbegin(), this->rend());
    }
 
   
    /**
     * Get super_iterator to end of data.
     *
     * \return                 super iterator
     */
    super_const_iterator super_end() const
    {
      return super_const_iterator(this->end(), this->end());
    }


    /**
     * Get super_reverse_iterator to end of data.
     *
     * \return                 super reverse iterator
     */
    super_const_reverse_iterator super_rend() const
    {
      return super_const_reverse_iterator(this->rend(), this->rend());
    }


    /**
     * Get super_iterator to begin of data.
     *
     * \return                 super iterator
     */
    super_iterator super_begin()
    {
      return super_iterator(this->begin(), this->end());
    }


    /**
     * Get super_reverse_iterator to begin of data.
     *
     * \return                 super iterator
     */
    super_reverse_iterator super_rbegin()
    {
      return super_reverse_iterator(this->rbegin(), this->rend());
    }    
 
   
    /**
     * Get super_iterator to end of data.
     *
     * \return                 super iterator
     */
    super_iterator super_end()
    {
      return super_iterator(this->end(), this->end());
    }


    /**
     * Get super_reverse_iterator to end of data.
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
    ordinate_type& get(const JMultiKey<NUMBER_OF_DIMENSIONS, abscissa_type>& key)
    {
      return this->get(key.first).get(key.second);
    }


    /**
     * Insert element.
     *
     * \param  key             multidimensional key
     * \param  value           value
     */
    void insert(const JMultiKey<NUMBER_OF_DIMENSIONS, abscissa_type>& key, const ordinate_type& value)
    {
      (*this)[key.first].insert(key.second, value);
    }


    /**
     * Insert element.
     *
     * \param  value           multidimensional pair
     */
    void insert(const JMultiPair<NUMBER_OF_DIMENSIONS, abscissa_type, ordinate_type>& value)
    {
      (*this)[value.first].insert(value.second);
    }


    /**
     * Insert element.
     *
     * \param  value           multidimensional iterator value
     */
    void insert(const typename super_iterator::reference& value)
    {
      (*this)[value.first].insert(value.second);
    }


    /**
     * Insert element.
     *
     * \param  value           multidimensional iterator value
     */
    void insert(const typename super_const_iterator::reference& value)
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

    typedef JHead_t<JAbscissa_t, JOrdinate_t, JDistance_t>   map_type;
    
    typedef JAbscissa_t                                      abscissa_type;
    typedef JOrdinate_t                                      ordinate_type;

    typedef typename map_type::key_type                      key_type;
    typedef typename map_type::mapped_type                   mapped_type;
    typedef typename map_type::value_type                    value_type;

    typedef typename map_type::const_iterator                const_iterator;
    typedef typename map_type::const_reverse_iterator        const_reverse_iterator;

    typedef typename map_type::iterator                      iterator;
    typedef typename map_type::reverse_iterator              reverse_iterator;

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


    class super_const_iterator;  // forward declaration


    /**
     * Terminator class of multidimensional iterator. 
     */
    class super_iterator :
      public JEquals         <super_iterator>,
      public JForwardIterator<super_iterator>
    {

      friend class JMultiMap;
      friend class super_const_iterator;

    public:

      typedef size_t                                          difference_type;      
      typedef JPair<const key_type&, mapped_type&>            value_type;
      typedef JLANG::JSinglePointer<value_type>               pointer;
      typedef JMultiPair<1, const JAbscissa_t, JOrdinate_t&>  reference;
      typedef std::forward_iterator_tag                       iterator_category;      
      typedef JMultiKey <1, const JAbscissa_t>                multikey_type;


      /**
       * Default constructor.
       */
      super_iterator()
      {}


      /**
       * Smart pointer operator.
       *
       * \return                 pointer to pair of iterators
       */
      pointer operator->()
      { 
	return pointer(new value_type(i->getX(), i->getY()));
      }


      /**
       * Dereference operator.
       *
       * \return                 multidimensional pair
       */
      reference operator*()
      { 
	return reference(i->getX(), i->getY());
      }


      /**
       * Equality of super iterator.
       *
       * \param  cursor          super iterator
       * \return                 true if equal; else false
       */
      virtual bool equals(const super_iterator& cursor) const
      {
	return i == cursor.i;
      }


      /**
       * Increment super_iterator.
       *
       * \return                 true if valid; else false
       */
      virtual bool increment() override 
      {
	return ++i != range.second;
      }

      
      /**
       * Get multi-dimensional key.
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
      JOrdinate_t& getValue() 
      {
	return i->getY();
      }

    private:
      /**
       * Constructor.
       *
       * \param  __begin         begin of data
       * \param  __end           end   of data
       */
      super_iterator(iterator __begin,
		     iterator __end) :
	range(__begin, __end),
	i    (__begin)
      {}


      std::pair<iterator, iterator> range;
      iterator i;
    };


    /**
     * Terminator class of multidimensional const_iterator. 
     */
    class super_const_iterator :
      public JEquals         <super_const_iterator>,
      public JForwardIterator<super_const_iterator>
    {

      friend class JMultiMap;

    public:

      typedef size_t                                                difference_type;      
      typedef JPair<const key_type&, const mapped_type&>            value_type;
      typedef JLANG::JSinglePointer<value_type>                     pointer;
      typedef JMultiPair<1, const JAbscissa_t, const JOrdinate_t&>  reference;
      typedef std::forward_iterator_tag                             iterator_category;      
      typedef JMultiKey <1, const JAbscissa_t>                      multikey_type;
      

      /**
       * Default constructor.
       */
      super_const_iterator()
      {}


      /**
       * Copy constructor.
       *
       * \param  cursor          super_iterator
       */
      super_const_iterator(super_iterator cursor) :
	range(cursor.range),
	i    (cursor.i)
      {}


      /**
       * Smart pointer operator.
       *
       * \return                 pointer to pair of iterators
       */
      pointer operator->()
      {
	return pointer(new value_type(i->getX(), i->getY()));
      }


      /**
       * Dereference operator.
       *
       * \return                 multidimensional pair
       */
      reference operator*()
      { 
	return reference(i->getX(), i->getY());
      }


      /**
       * Equality of super iterator.
       *
       * \param  cursor          super iterator
       * \return                 true if equal; else false
       */
      virtual bool equals(const super_const_iterator& cursor) const
      {
	return i == cursor.i;
      }


      /**
       * Increment super_iterator.
       *
       * \return                 true if valid; else false
       */
      virtual bool increment() override 
      {
	return ++i != range.second;
      }

      
      /**
       * Get multi-dimensional key.
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
      const JOrdinate_t& getValue() 
      {
	return i->getY();
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
	range(__begin, __end),
	i    (__begin)
      {}


      std::pair<const_iterator, const_iterator> range;
      const_iterator i;
    };


    class super_const_reverse_iterator;  // forward declaration


    /**
     * Terminator class of multidimensional reverse iterator. 
     */
    class super_reverse_iterator :
      public JEquals         <super_iterator>,
      public JForwardIterator<super_iterator>
    {

      friend class JMultiMap;
      friend class super_const_reverse_iterator;

    public:

      typedef size_t                                          difference_type;      
      typedef JPair<const key_type&, mapped_type&>            value_type;
      typedef JLANG::JSinglePointer<value_type>               pointer;
      typedef JMultiPair<1, const JAbscissa_t, JOrdinate_t&>  reference;
      typedef std::forward_iterator_tag                       iterator_category;      
      typedef JMultiKey <1, const JAbscissa_t>                multikey_type;


      /**
       * Default constructor.
       */
      super_reverse_iterator()
      {}


      /**
       * Smart pointer operator.
       *
       * \return                 pointer to pair of reverse iterators
       */
      pointer operator->()
      { 
	return pointer(new value_type(i->getX(), i->getY()));
      }


      /**
       * Dereference operator.
       *
       * \return                 multidimensional pair
       */
      reference operator*()
      { 
	return reference(i->getX(), i->getY());
      }


      /**
       * Equality of super reverse iterator.
       *
       * \param  cursor          super reverse iterator
       * \return                 true if equal; else false
       */
      virtual bool equals(const super_reverse_iterator& cursor) const
      {
	return i == cursor.i;
      }


      /**
       * Increment super_reverse_iterator.
       *
       * \return                 true if valid; else false
       */
      virtual bool increment() override 
      {
	return ++i != range.second;
      }

      
      /**
       * Get multi-dimensional key.
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
      JOrdinate_t& getValue() 
      {
	return i->getY();
      }

    private:
      /**
       * Constructor.
       *
       * \param  __begin         begin of data
       * \param  __end           end   of data
       */
      super_reverse_iterator(reverse_iterator __begin,
			     reverse_iterator __end) :
	range(__begin, __end),
	i    (__begin)
      {}


      std::pair<reverse_iterator, reverse_iterator> range;
      reverse_iterator i;
    };


    /**
     * Terminator class of multidimensional const_iterator. 
     */
    class super_const_reverse_iterator :
      public JEquals         <super_const_reverse_iterator>,
      public JForwardIterator<super_const_reverse_iterator>
    {

      friend class JMultiMap;

    public:

      typedef size_t                                                difference_type;      
      typedef JPair<const key_type&, const mapped_type&>            value_type;
      typedef JLANG::JSinglePointer<value_type>                     pointer;
      typedef JMultiPair<1, const JAbscissa_t, const JOrdinate_t&>  reference;
      typedef std::forward_iterator_tag                             iterator_category;      
      typedef JMultiKey <1, const JAbscissa_t>                      multikey_type;
      

      /**
       * Default constructor.
       */
      super_const_reverse_iterator()
      {}


      /**
       * Copy constructor.
       *
       * \param  cursor          super_iterator
       */
      super_const_reverse_iterator(super_reverse_iterator cursor) :
	range(cursor.range),
	i    (cursor.i)
      {}


      /**
       * Smart pointer operator.
       *
       * \return                 pointer to pair of iterators
       */
      pointer operator->()
      {
	return pointer(new value_type(i->getX(), i->getY()));
      }


      /**
       * Dereference operator.
       *
       * \return                 multidimensional pair
       */
      reference operator*()
      { 
	return reference(i->getX(), i->getY());
      }


      /**
       * Equality of super reverse iterator.
       *
       * \param  cursor          super reverse iterator
       * \return                 true if equal; else false
       */
      virtual bool equals(const super_const_reverse_iterator& cursor) const
      {
	return i == cursor.i;
      }


      /**
       * Increment super_iterator.
       *
       * \return                 true if valid; else false
       */
      virtual bool increment() override 
      {
	return ++i != range.second;
      }

      
      /**
       * Get multi-dimensional key.
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
      const JOrdinate_t& getValue() 
      {
	return i->getY();
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
	range(__begin, __end),
	i    (__begin)
      {}


      std::pair<const_reverse_iterator, const_reverse_iterator> range;
      const_reverse_iterator i;
    };


     /**
     * Get super_iterator to begin of data.
     *
     * \return             super_iterator
     */
    super_const_iterator super_begin() const
    {
      return super_const_iterator(this->begin(), this->end());
    }


    /**
     * Get super_reverse_iterator to reverse begin of data.
     *
     * \return             super_reverse_iterator
     */
    super_const_reverse_iterator super_rbegin() const
    {
      return super_const_reverse_iterator(this->rbegin(), this->rend());
    }
 
   
    /**
     * Get super_iterator to end of data.
     *
     * \return             super_iterator
     */
    super_const_iterator super_end() const
    {
      return super_const_iterator(this->end(), this->end());
    }

    
    /**
     * Get super_reverse_iterator to reverse end of data.
     *
     * \return             super_reverse_iterator
     */
    super_const_reverse_iterator super_rend() const
    {
      return super_const_reverse_iterator(this->rend(), this->rend());
    }


    /**
     * Get super_iterator to begin of data.
     *
     * \return             super_iterator
     */
    super_iterator super_begin()
    {
      return super_iterator(this->begin(), this->end());
    }


    /**
     * Get super_reverse_iterator to reverse begin of data.
     *
     * \return             super_reverse_iterator
     */
    super_reverse_iterator super_rbegin()
    {
      return super_reverse_iterator(this->rbegin(), this->rend());
    }
 
   
    /**
     * Get super_iterator to end of data.
     *
     * \return             super_iterator
     */
    super_iterator super_end()
    {
      return super_iterator(this->end(), this->end());
    }
 
   
    /**
     * Get super_reverse_iterator to end of data.
     *
     * \return             super_reverse_iterator
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
    ordinate_type& get(const JMultiKey<1, abscissa_type>& key)
    {
      return get(key.first);
    }


    /**
     * Insert element.
     *
     * \param  key          multidimensional key
     * \param  value        value
     */
    void insert(const JMultiKey<1, JAbscissa_t>& key, const JOrdinate_t& value)
    {
      insert(value_type(key.first, value));
    }


    /**
     * Insert element.
     *
     * \param  value        multidimensional pair
     */
    void insert(const JMultiPair<1, JAbscissa_t, JOrdinate_t>& value)
    {
      insert(value_type(value.first, value.second));
    }


    /**
     * Insert element.
     *
     * \param  value        multidimensional iterator value
     */
    void insert(const typename super_iterator::reference& value)
    {
      insert(value_type(value.first, value.second));
    }


    /**
     * Insert element.
     *
     * \param  value        multidimensional iterator value
     */
    void insert(const typename super_const_iterator::reference& value)
    {
      insert(value_type(value.first, value.second));
    }
  };
}

#endif
