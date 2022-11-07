#ifndef __JLANG__JCOLORFACET__
#define __JLANG__JCOLORFACET__

#include <locale>
#include <ostream>
#include <string>
#include <vector>
#include <map>

#include "JLang/JType.hh"
#include "JLang/JTypeList.hh"
#include "JLang/JSinglePointer.hh"
#include "JLang/JException.hh"


/**
 * \author mdejong
 */

namespace JLANG {}
namespace JPP { using namespace JLANG; }


namespace JLANG {

  /**
   * Enumeration of text colors.
   */
  enum JColor_t {
    RED,              //!< red
    GREEN,            //!< green
    BLUE,             //!< blue
    WHITE,            //!< white
    CYAN,             //!< cyan
    PURPLE,           //!< purple
    YELLOW,           //!< yellow
    RESET,            //!< reset
    BOLD              //!< bold
  };


  /**
   * Facet interface to specify text color.
   * This class extends the std::locale::facet class.
   */
  struct JColorFacet : 
    public std::locale::facet
  {
    static std::locale::id  id;

  
    /**
     * Constructor.
     *
     * \param  refs      reference count
     */
    JColorFacet(std::size_t refs = 0) :
      std::locale::facet(refs)
    {}


    /**
     * Print color.
     *
     * \param  color     code
     * \return           text
     */
    virtual const char* c_str(const JColor_t color) const = 0;


    /**
     * Clone this facet.
     *
     * \return           pointer to newly created facet
     */
    virtual JColorFacet* clone() const = 0;


    /**
     * Check color.
     *
     * \param  color     code
     * \return           true if color; else false
     */
    static inline bool is_color(const int color)
    {
      return !is_bold(color) && !is_reset(color);
    }


    /**
     * Check bold.
     *
     * \param  color     code
     * \return           true if bold; else false
     */
    static inline bool is_bold(const int color)
    {
      return color == BOLD;
    }


    /**
     * Check reset.
     *
     * \param  color     code
     * \return           true if reset; else false
     */
    static inline bool is_reset(const int color)
    {
      return color == RESET;
    }
  private:

    JColorFacet(const JColorFacet&);          // not defined
    void operator=(const JColorFacet&);       // not defined
  };


  /**
   * Facet class to specify text color for ASCII.
   */
  struct JColorFacetASCII : 
    public JColorFacet
  {
    /**
     * Get name of facet.
     *
     * \return           name
     */
    static inline const char* getName() 
    {
      return "ASCII";
    }


    /**
     * Constructor.
     *
     * \param  refs      reference count
     */
    JColorFacetASCII(std::size_t refs = 0) :
      JColorFacet(refs)
    {}


    /**
     * Print color.
     *
     * \param  color     code
     * \return           text
     */
    virtual const char* c_str(const JColor_t color) const override 
    {
      switch (color) {
      case RED:     return "\033[91m"; 
      case GREEN:   return "\033[92m"; 
      case BLUE:    return "\033[94m";
      case WHITE:   return "\033[97m";
      case CYAN:    return "\033[96m";
      case PURPLE:  return "\033[95m";
      case YELLOW:  return "\033[93m";
      case BOLD:    return "\033[1m";
      case RESET:   return "\033[0m";
      default:      return "";
      }
    }


    /**
     * Clone this facet.
     *
     * \return           pointer to newly created facet
     */
    virtual JColorFacetASCII* clone() const override 
    {
      return new JColorFacetASCII();
    }
  };


  /**
   * Facet class to specify text color for ELcode.
   */
  struct JColorFacetELcode : 
    public JColorFacet
  {
    /**
     * Get name of facet.
     *
     * \return           name
     */
    static inline const char* getName() 
    {
      return "ELcode";
    }


    /**
     * Constructor.
     *
     * \param  refs      reference count
     */
    JColorFacetELcode(std::size_t refs = 0) :
      JColorFacet(refs)
    {}


    /**
     * Print color.
     *
     * \param  color     code
     * \return           text
     */
    virtual const char* c_str(const JColor_t color) const override 
    {
      static std::string buffer;

      history.push_back(color);

      switch (color) {

      case RED:    return "[color=red]";
      case GREEN:  return "[color=green]";
      case BLUE:   return "[color=blue]";
      case WHITE:  return "[color=white]";
      case CYAN:   return "[color=cyan]";
      case PURPLE: return "[color=purple]";
      case YELLOW: return "[color=yellow]";
      case BOLD:   return "[bold]";

      case RESET:

	buffer.clear();

	for (std::vector<int>::const_reverse_iterator i = history.rbegin(); i != history.rend(); ++i) {
	  if      (is_color(*i)) 
	    buffer += "[/color]";
	  else if (is_bold (*i)) 
	    buffer += "[/bold]";
	  else
	    ;
	}

	history.clear();

	return buffer.c_str();
	
      default:     return "";
      }
    }


    /**
     * Clone this facet.
     *
     * \return           pointer to newly created facet
     */
    virtual JColorFacetELcode* clone() const override 
    {
      return new JColorFacetELcode();
    }

  private:
    mutable std::vector<int> history;
  };


  /**
   * Typelist of color facets.
   */
  typedef JTYPELIST<JColorFacetASCII, JColorFacetELcode>::typelist    JColorFacetTypes_t;


  /**
   * Auxiliary map for color facets.
   */
  struct JColorFacetMap_t :
    public std::map<std::string, JSinglePointer<JColorFacet> >
  {
    typedef std::map<std::string, JSinglePointer<JColorFacet> >  map_type;
    typedef typename map_type::key_type                          key_type;
    typedef typename map_type::mapped_type                       mapped_type;
    typedef typename map_type::value_type                        value_type;


    /**
     * Default constructor.
     */
    JColorFacetMap_t()
    {
      for_each(*this, JType<JColorFacetTypes_t>());
    }


    /**
     * Get value for given key.
     *
     * Note that this method will throw an error if given key is absent.
     *
     * \param  key       key
     * \return           value
     */
    const mapped_type& operator[](const key_type& key) const
    {
      const_iterator p = find(key);

      if (p != this->end()) {
	return p->second;
      }

      THROW(JNullPointerException, "Invalid key " << key);
    }


    /**
     * Insert data type.
     *
     * \param  type      data type
     */
    template<class T>
    void operator()(const JType<T>& type) 
    {
      insert(value_type(T::getName(), new T()));
    }
  };


  /**
   * Color facets.
   */
  static const JColorFacetMap_t   color_facets;
}


/**
 * Print color.
 *
 * \param  out               output stream
 * \param  color             color
 * \return                   output stream
 */
inline std::ostream& operator<<(std::ostream& out, const JLANG::JColor_t color)
{
  using namespace std;
  using namespace JPP;

  const locale& loc = out.getloc();
          
  if      (has_facet<JColorFacetASCII>(loc)) 
    return out << use_facet<JColorFacetASCII> (loc).c_str(color);
  else if (has_facet<JColorFacetELcode>(loc)) 
    return out << use_facet<JColorFacetELcode>(loc).c_str(color);
  else
    return out << JColorFacetASCII().c_str(color);
}

#endif
