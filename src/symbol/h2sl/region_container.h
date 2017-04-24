/**
 * @file region.h
 *
 * @brief
 *
 * a class that describes a region in relation to an container
 */

#ifndef H2SL_REGION_CONTAINER_H
#define H2SL_REGION_CONTAINER_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/grounding.h"
#include "h2sl/container.h"
#include "h2sl/spatial_relation.h"

namespace h2sl {
  /**
   * Region_Container class definition
   */
  class Region_Container : public Grounding {
  public:
    Region_Container( const std::string& spatialRelationType = "na",
                      const Container& container = Container() );
    virtual ~Region_Container();
    Region_Container( const Region_Container& other );
    Region_Container( xmlNodePtr root, World* world );
    Region_Container& operator=( const Region_Container& other );
    bool operator==( const Region_Container& other )const;
    bool operator!=( const Region_Container& other )const;
    virtual Region_Container* dup( void )const;
   
    virtual std::string evaluate_cv( const Grounding_Set* groundingSet )const;
    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "region_container" ); };
    virtual void scrape_grounding( const World * world, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;
    virtual void scrape_grounding( const World * world, std::vector< std::string >& classNames, std::map< std::string, std::vector< std::string > >& stringTypes, std::map< std::string, std::vector< int > >& intTypes )const;
    static void fill_search_space( const Symbol_Dictionary& symbolDictionary, const World* world, std::map< std::string, std::pair< std::string, std::vector< Grounding* > > >& searchSpaces, const std::string& symbolType ); 
    virtual void fill_rules( const World* world, Grounding_Set* groundingSet )const;
    virtual bool equals( const Grounding& other )const;

    virtual void from_xml( const std::string& file, World* world );
    virtual void from_xml( xmlNodePtr root, World* world );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual std::string to_latex( void )const;

    inline std::string& relation_type( void ){ return get_prop< std::string >( _string_properties, "spatial_relation_type" ); };
    inline const std::string& relation_type( void )const{ return get_prop< std::string >( _string_properties, "spatial_relation_type" ); };
    
    inline Container& container( void ){ return _container; };
    inline const Container& container( void )const{ return _container; };

    static std::string class_name( void ){ return "region_container"; };
   
  protected:
      Container _container;
  
  private:

  };

  /**
   * Region_Container class ostream operator
   */ 
  std::ostream& operator<<( std::ostream& out, const Region_Container& other );
}

#endif /* H2SL_REGION_CONTAINER_H */
