#ifndef __JTOOLS__JCOLLECTION__
#define __JTOOLS__JCOLLECTION__

#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

#include "JLang/JClass.hh"
#include "JLang/JException.hh"
#include "JLang/JLangToolkit.hh"
#include "JMath/JZero.hh"
#include "JMath/JMath.hh"
#include "JIO/JSerialisable.hh"
#include "JTools/JDistance.hh"
#include "JTools/JTransformer.hh"
#include "JTools/JMappableCollection.hh"
#include "JTools/JAbstractCollection.hh"

/**
 * \file
 *
 * General purpose class for a collection of sorted elements.
 * \author mdejong
 */
namespace JTOOLS {}
namespace JPP { using namespace JTOOLS; }

namespace JTOOLS {

  using JMATH::JMath;
  using JIO::JReader;
  using JIO::JWriter;
  using JLANG::JValueOutOfRange;
  using JLANG::JClass;
  using JLANG::JException;
  

  /**
   * General purpose class for collection of elements, see:
   \htmlonly
   <a href="JTools.PDF";>Collection of elements.</a>
   \endhtmlonly
   *
   * This class implements the JMappableCollection and JAbstractCollection interfaces.
   *
   * The data type of the elements of the collection should have the following policy
   * type definition and member methods.
   * <pre>
   *    typedef \<abscissa type\>   abscissa_type;
   *    typedef \<ordinate type\>   ordinate_type;
   *
   *    (constructor)(abscissa_type, ordinate_type);
   *
   *    abscissa_type  %getX() const;
   *    ordinate_type  %getY() const;
   *    ordinate_type& %getY();
   * </pre>
   *
   * The elements in a collection are sorted according to their abscissa values and 
   * the given distance operator.
   * The distance operator constitues a binary method returning the distance between
   * two abscissa values; The default distance operator is JDistance.
   *
   * For the binary I/O of a collection of elements, the data structure of the elements
   * should provide for an implementation of the following operators:
   * <pre>
   *       JReader& operator>>(JReader& in);
   *       JWriter& operator<<(JWriter& out);
   * </pre>
   */
  template<class JElement_t, class JDistance_t = JDistance<typename JElement_t::abscissa_type> >
  class JCollection :
    public std::vector<JElement_t>,
    public JMappableCollection<typename JElement_t::abscissa_type, 
			       typename JElement_t::ordinate_type>,
    public JAbstractCollection<typename JElement_t::abscissa_type>,
    public JMath< JCollection<JElement_t, JDistance_t> >
  {
  public:

    typedef typename JElement_t::abscissa_type                   abscissa_type;
    typedef typename JElement_t::ordinate_type                   ordinate_type;
    typedef JElement_t                                           value_type;
    typedef JDistance_t                                          distance_type;

    typedef JCollection<JElement_t, JDistance_t>                 collection_type;
    
    typedef std::vector<value_type>                              container_type;

    typedef typename container_type::const_iterator              const_iterator;
    typedef typename container_type::const_reverse_iterator      const_reverse_iterator;
    typedef typename container_type::iterator                    iterator;
    typedef typename container_type::reverse_iterator            reverse_iterator;

    typedef JCollectionElementTransformer<value_type>            transformer_type;
    typedef std::pair<const_iterator, bool>                      pair_type;

    using JMappableCollection<abscissa_type, ordinate_type>::operator[];


    /**
     * Auxiliary class for ordering of objects in the collection by their abscissa values.
     */
    struct JComparator {
      /**
       * Comparison of elements.
       *
       * \param  first           first  element
       * \param  second          second element
       * \return                 true if first element less than second element; else false
       */
      inline bool operator()(const JElement_t& first, 
			     const JElement_t& second) const
      {
	return this->getDistance(first.getX(), second.getX()) > 0.0;
      }
      
      
      /**
       * Comparison of element and abscissa value.
       *
       * \param  element         element
       * \param  x               abscissa value
       * \return                 true if element less than abscissa value; else false
       */
      inline bool operator()(const JElement_t& element, typename JClass<abscissa_type>::argument_type x) const
      {
	return this->getDistance(element.getX(), x) > 0.0;
      }


      /**
       * Function object for distance evaluation.
       */
      JDistance_t getDistance;
    };


    /**
     * Default constructor.
     */
    JCollection()
    {}


    /**
     * Clear.
     */
    virtual void clear() override 
    {
      container_type::clear();
    }


    /**
     * Get ordinate value.
     *
     * \param  x               abscissa value
     * \return                 ordinate value
     */
    virtual const ordinate_type& get(typename JClass<abscissa_type>::argument_type x) const override 
    {
      const_iterator i = this->lower_bound(x);

      if (i == this->end() || this->getDistance(x, i->getX()) > distance_type::precision) {
	THROW(JValueOutOfRange, "Invalid abscissa value " << x);
      }

      return i->getY();
    }


    /**
     * Get ordinate value.
     *
     * \param  x               abscissa value
     * \return                 ordinate value
     */
    virtual ordinate_type& get(typename JClass<abscissa_type>::argument_type x) override 
    {
      iterator i = this->lower_bound(x);

      if (i == this->end() || this->getDistance(x, i->getX()) > distance_type::precision) {
        i = container_type::insert(i, value_type(x, ordinate_type()));
      }

      return i->getY();
    }


    /**
     * Get number of elements.
     *
     * \return                 number of elements
     */
    virtual int getSize() const override 
    {
      return (int) this->size();
    }


    /**
     * Get abscissa value.
     *
     * \param  index           index
     * \return                 abscissa value
     */
    virtual abscissa_type getX(int index) const override 
    {
      return this->at(index).getX();
    }


    /**
     * Get minimal abscissa value.
     *
     * \return                 abscissa value
     */
    virtual abscissa_type getXmin() const override 
    {
      return this->begin()->getX();
    }


    /**
     * Get maximal abscissa value.
     *
     * \return                 abscissa value
     */
    virtual abscissa_type getXmax() const override 
    {
      return this->rbegin()->getX();
    }



    /**
     * Get ordinate value.
     *
     * \param  index           index
     * \return                 ordinate value
     */
    const ordinate_type& getY(int index) const
    {
      return this->at(index).getY();
    }


    /**
     * Get ordinate value.
     *
     * \param  index           index
     * \return                 ordinate value
     */
    ordinate_type& getY(int index) 
    {
      return this->at(index).getY();
    }


    /**
     * Transform collection.
     *
     * \param  transformer     element transformer
     */
    void transform(const transformer_type& transformer)
    {
      for (iterator i = this->begin(); i != this->end(); ++i) {
        *i = transformer(*i);
      }
      
      sort();
    }


    /**
     * Sort elements.
     */
    void sort()
    {
      std::sort(this->begin(), this->end(), compare);
    }


    /**
     * Get first position of element <tt>i</tt>, where <tt>x >= i->getX()</tt>.
     *
     * \param  x               abscissa value
     * \return                 position of corresponding element
     */
    const_iterator lower_bound(typename JClass<abscissa_type>::argument_type x) const
    {
      return std::lower_bound(this->begin(), this->end(), x, compare);
    }


    /**
     * Get first position of element <tt>i</tt>, where <tt>x >= i->getX()</tt>.
     *
     * \param  x               abscissa value
     * \return                 position of corresponding element
     */
    iterator lower_bound(typename JClass<abscissa_type>::argument_type x) 
    {
      return std::lower_bound(this->begin(), this->end(), x, compare);
    }


    /**
     * Insert element.
     *
     * \param  element         element
     * \return                 (iterator, status), where status is true if inserted; else false
     */
    pair_type insert(const value_type& element)
    {
      iterator i = this->lower_bound(element.getX());

      if (i == this->end() || this->getDistance(element.getX(), i->getX()) > 0.0)
        return pair_type(container_type::insert(i, element), true);
      else
        return pair_type(this->end(), false);
    }

    
    /**
     * Configure collection.
     *
     * \param  bounds          abscissa values
     */
    void configure(const JAbstractCollection<abscissa_type>& bounds)
    {
      configure(bounds, JMATH::getZero<ordinate_type>());
    }


    /**
     * Configure collection.
     *
     * \param  bounds          abscissa values
     * \param  value           ordinate value
     */
    void configure(const JAbstractCollection<abscissa_type>&     bounds,
		   typename JClass<ordinate_type>::argument_type value)
    {
      this->resize(bounds.getSize());

      for (iterator i = this->begin(); i != this->end(); ++i) {

	const abscissa_type x = bounds.getX(std::distance(this->begin(),i));

	*i = value_type(x,value);
      }
    }


    /**
     * Configure collection.
     *
     * \param  bounds          abscissa values
     * \param  function        function
     */
    template<class JFunction1D_t>
    void configure(const JAbstractCollection<abscissa_type>& bounds, 
		   const JFunction1D_t&                      function)
    {
      using namespace JLANG;

      collection_type* out = (is_identical(*this, function) ? new collection_type() : this);

      for (int i = 0; i != bounds.getSize(); ++i) {

	const abscissa_type x = bounds.getX(i);

	out->put(x, function(x));
      }

      if (is_identical(*this, function)) {
	
	this->swap(*out);

	delete out;
      }
    }


    /**
     * Test whether collections are compatible.
     *
     * \param  collection      collection
     * \return                 true if collections are compatible; else false
     */
    bool is_compatible(const JCollection& collection) const
    {
      if (this->empty() || collection.empty()) {

	return true;

      } else {

	const double precision = JDistance<abscissa_type>::precision;
	
	const_iterator p = this->begin();
	const_iterator q = collection.begin();
	
	if        (getDistance(p->getX(), q->getX()) > precision) { 

	  do { 
	    ++p; 
	  } while (p != this->end()      && getDistance(p->getX(), q->getX()) > precision); 

	} else if (getDistance(q->getX(), p->getX()) > precision) { 

	  do { 
	    ++q; 
	  } while (q != collection.end() && getDistance(q->getX(), p->getX()) > precision); 
	}

	for ( ; p != this->end() && q != collection.end(); ++p, ++q) {
	  if (fabs(getDistance(p->getX(), q->getX())) > precision) {
	    return false;
	  }
	}

	return true;
      }
    }


    /**
     * Negate collection.
     *
     * \return                 this collection
     */
    JCollection& negate()
    {
      for (iterator i = this->begin(); i != this->end(); ++i) {
	i->getY() = -i->getY();
      }

      return *this;
    }


    /**
     * Add collection.
     *
     * \param  collection      collection
     * \return                 this collection
     */
    JCollection& add(const JCollection& collection)
    {
      if (!collection.empty()) {

	if (this->empty()) {

	  for (const_iterator i = collection.begin(); i != collection.end(); ++i) {
	    this->put(i->getX(), +i->getY());
	  }

	} else if (this->is_compatible(collection)) {

	  const double precision = JDistance<abscissa_type>::precision;

	  iterator       p = this->begin();
	  const_iterator q = collection.begin();

	  if        (getDistance(p->getX(), q->getX()) > precision) { 

	    do {
	      ++p; 
	    } while (p != this->end()      && getDistance(p->getX(), q->getX()) > precision); 

	  } else if (getDistance(q->getX(), p->getX()) > precision) { 

	    do { 
	      ++q; 
	    } while (q != collection.end() && getDistance(q->getX(), p->getX()) > precision); 
	  }

	  const_iterator i = q;

	  for ( ; p != this->end() && i != collection.end(); ++p, ++i) {
	    p->getY() += i->getY();
	  }

	  for ( ; i != collection.end(); ++i) {
	    this->put(i->getX(), +i->getY());
	  }

	  for (i = collection.begin(); i != q; ++i) {
	    this->put(i->getX(), +i->getY());
	  }
	}
      }
      
      return *this;
    }


    /**
     * Subtract collection.
     *
     * \param  collection      collection
     * \return                 this collection
     */
    JCollection& sub(const JCollection& collection)
    {
      if (!collection.empty()) {

	if (this->empty()) {

	  for (const_iterator i = collection.begin(); i != collection.end(); ++i) {
	    this->put(i->getX(), -i->getY());
	  }

	} else if (this->is_compatible(collection)) {

	  const double precision = JDistance<abscissa_type>::precision;

	  iterator       p = this->begin();
	  const_iterator q = collection.begin();

	  if        (getDistance(p->getX(), q->getX()) > precision) { 

	    do {
	      ++p; 
	    } while (p != this->end()      && getDistance(p->getX(), q->getX()) > precision); 

	  } else if (getDistance(q->getX(), p->getX()) > precision) { 

	    do { 
	      ++q; 
	    } while (q != collection.end() && getDistance(q->getX(), p->getX()) > precision); 
	  }

	  const_iterator i = q;

	  for ( ; p != this->end() && i != collection.end(); ++p, ++i) {
	    p->getY() -= i->getY();
	  }

	  for ( ; i != collection.end(); ++i) {
	    this->put(i->getX(), -i->getY());
	  }

	  for (i = collection.begin(); i != q; ++i) {
	    this->put(i->getX(), -i->getY());
	  }

	} else {

	  throw JException("JCollection::add() collections incompatible.");
	}
      }
      
      return *this;
    }


    /**
     * Scale contents.
     *
     * \param  value           multiplication factor
     * \return                 this collection
     */
    JCollection& mul(const double value)
    {
      for (iterator i = this->begin(); i != this->end(); ++i) {
	i->getY() *= value;
      }

      return *this;
    }


    /**
     * Scale contents.
     *
     * \param  value           division factor
     * \return                 this collection
     */
    JCollection& div(const double value)
    {
      for (iterator i = this->begin(); i != this->end(); ++i) {
	i->getY() /= value;
      }

      return *this;
    }


    /**
     * Add offset.
     *
     * \param  value           offset
     * \return                 this collection
     */
    JCollection& add(typename JClass<ordinate_type>::argument_type value)
    {
      for (iterator i = this->begin(); i != this->end(); ++i) {
	i->getY() += value;
      }

      return *this;
    }


    /**
     * Subtract offset.
     *
     * \param  value           offset
     * \return                 this collection
     */
    JCollection& sub(typename JClass<ordinate_type>::argument_type value)
    {
      for (iterator i = this->begin(); i != this->end(); ++i) {
	i->getY() -= value;
      }

      return *this;
    }


    /**
     * Add function.
     *
     * \param  function        function
     * \return                 this collection
     */
    template<class JFunction1D_t>
    JCollection& add(const JFunction1D_t& function)
    {
      for (iterator i = this->begin(); i != this->end(); ++i) {
	i->getY() += function(i->getX());
      }

      return *this;
    }


    /**
     * Subtract function.
     *
     * \param  function        function
     * \return                 this collection
     */
    template<class JFunction1D_t>
    JCollection& sub(const JFunction1D_t& function)
    {
      for (iterator i = this->begin(); i != this->end(); ++i) {
	i->getY() -= function(i->getX());
      }

      return *this;
    }


    /**
     * Add offset to collaction.
     *
     * \param  collection      collection
     * \param  value           offset
     * \return                 collection
     */
    friend JCollection& operator+=(JCollection& collection, typename JClass<ordinate_type>::argument_type value)
    {
      return collection.add(value);
    }


    /**
     * Subtract offset from collaction.
     *
     * \param  collection      collection
     * \param  value           offset
     * \return                 collection
     */
    friend JCollection& operator-=(JCollection& collection, typename JClass<ordinate_type>::argument_type value)
    {
      return collection.sub(value);
    }
 

    /**
     * Add function.
     *
     * \param  collection      collection
     * \param  function        function
     * \return                 this collection
     */
    template<class JFunction1D_t>
    friend JCollection& operator+=(JCollection& collection, const JFunction1D_t& function)
    {
      return collection.add(function);
    }
 

    /**
     * Subtract function.
     *
     * \param  collection      collection
     * \param  function        function
     * \return                 this collection
     */
    template<class JFunction1D_t>
    friend JCollection& operator-=(JCollection& collection, const JFunction1D_t& function)
    {
      return collection.sub(function);
    }


    /**
     * Read collection from input.
     *
     * \param  in              reader
     * \param  collection      collection
     * \return                 reader
     */
    friend inline JReader& operator>>(JReader& in, JCollection& collection)
    {
      int n;

      if (in >> n) {

	collection.resize(n);

	for (typename JCollection::iterator i = collection.begin(); i != collection.end(); ++i) {
	  in >> *i;
	}
      }

      return in;
    }


    /**
     * Write collection to output.
     *
     * \param  out             writer
     * \param  collection      collection
     * \return                 writer
     */
    friend inline JWriter& operator<<(JWriter& out, const JCollection& collection)
    {
      const int n = collection.size();

      out << n;

      for (typename JCollection::const_iterator i = collection.begin(); i != collection.end(); ++i) {
        out << *i;
      }

      return out;
    }


    /**
     * Get comparator.
     *
     * \return                 comparator
     */
    const JComparator& getComparator() const
    {
      return compare;
    }


    /**
     * Function object for distance evaluation.
     */
    JDistance_t getDistance;


  protected:
    /**
     * Function object for comparison.
     */
    JComparator compare;


    /**
     * Resize collection
     *
     * \param  size             size
     */
    void resize(typename container_type::size_type size)
    {
      container_type::resize(size);
    }

  private:
    void erase();
    void push_back();
    void pop_back();
  };


  /**
   * Conversion of data points to integral values.
   *
   * The integration is based on the trapezoidal rule applied to the input data points.
   *
   * \param  input             collection
   * \param  output            mappable collection
   * \return                   integral
   */
  template<class JElement_t,
           class JDistance_t>
  inline typename JElement_t::ordinate_type 
  integrate(const JCollection<JElement_t, JDistance_t>& input, typename JMappable<JElement_t>::map_type& output)
  {
    typedef typename JElement_t::ordinate_type                             ordinate_type;
    typedef typename JCollection<JElement_t, JDistance_t>::const_iterator  const_iterator;

    ordinate_type V(JMATH::zero);

    if (input.getSize() > 1) {

      output.put(input.begin()->getX(), V);

      for (const_iterator j = input.begin(), i = j++; j != input.end(); ++i, ++j) {

        V += 0.5 * input.getDistance(i->getX(), j->getX()) * (i->getY() + j->getY());

        output.put(j->getX(), V);
      }
    }

    return V;
  }
}

#endif
