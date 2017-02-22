/**
 * @file region.h
 *
 * @brief
 *
 * a class that describes a region in relation to an container
 */

#ifndef H2SL_REGION_ABSTRACT_CONTAINER_H
#define H2SL_REGION_ABSTRACT_CONTAINER_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/grounding.h"
#include "h2sl/abstract_container.h"


namespace h2sl {
  /**
   * Region_Abstract_Container class definition
   */
  class Region_Abstract_Container : public Grounding {
  public:
    Region_Abstract_Container( const std::string& region_abstract_containerType = "na",
                        const Abstract_Container& abstract_container = Abstract_Container() ); 

    virtual ~Region_Abstract_Container();
    Region_Abstract_Container( const Region_Abstract_Container& other );
    Region_Abstract_Container( xmlNodePtr root );
    Region_Abstract_Container& operator=( const Region_Abstract_Container& other );
    bool operator==( const Region_Abstract_Container& other )const;
    bool operator!=( const Region_Abstract_Container& other )const;
    virtual Region_Abstract_Container* dup( void )const;
   
    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "region_abstract_container" ); };
    static void fill_search_space( const Symbol_Dictionary& symbolDictionary, const World* world, std::vector< std::pair< unsigned int, Grounding* > >& searchSpaces, const symbol_type_t& symbolType ); 

    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    inline std::string& region_abstract_container_type( void ){ 
              return get_prop< std::string >( _string_properties, "region_abstract_container_type" ); };
    inline const std::string& region_abstract_container_type( void )const{ 
              return get_prop< std::string >( _string_properties, "region_abstract_container_type" ); };
  
    inline Abstract_Container& abstract_container( void ){ return _abstract_container; };
    inline const Abstract_Container& abstract_container( void )const{ return _abstract_container; };
      
    static std::string class_name( void ){ return "region_abstract_container"; };
   
  protected:
    Abstract_Container _abstract_container;

  private: 

  };

  /**
   * Region_Abstract_Container class ostream operator
   */ 
  std::ostream& operator<<( std::ostream& out, const Region_Abstract_Container& other );
}

#endif /* H2SL_REGION_ABSTRACT_CONTAINER_H */
