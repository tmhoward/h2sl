/**
 * @file spatial_relation.h
 * 
 * @brief
 * 
 * a class used to describe a spatial relationship
 */

#ifndef H2SL_SPATIAL_RELATION_H
#define H2SL_SPATIAL_RELATION_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/grounding.h"
#include "h2sl/object.h"

namespace h2sl {
  /**
   * Spatial_Relation class definition
   */
  class Spatial_Relation : public Grounding {
  public:

    Spatial_Relation( const std::string& spatialRelationType = "na" );
    Spatial_Relation( const Spatial_Relation& other );
    Spatial_Relation( xmlNodePtr root, World* world );
    virtual ~Spatial_Relation();
    Spatial_Relation& operator=( const Spatial_Relation& other );
    bool operator==( const Spatial_Relation& other )const;
    bool operator!=( const Spatial_Relation& other )const;
    virtual Spatial_Relation* dup( void )const;

    virtual std::string evaluate_cv( const Grounding_Set* groundingSet )const;
    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "spatial_relation" ); };
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

    inline std::string& spatial_relation_type( void ){ return get_prop< std::string >( _string_properties, "spatial_relation_type" ); };
    inline const std::string& spatial_relation_type( void )const{ return get_prop< std::string >( _string_properties, "spatial_relation_type" ); };

    static std::string class_name( void ){ return "spatial_relation"; };

  protected:

  private:

  };

  /** 
   * Spatial_Relation class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Spatial_Relation& other );
}

#endif /* H2SL_SPATIAL_RELATION_H */
