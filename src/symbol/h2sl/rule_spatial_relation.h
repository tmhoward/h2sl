/**
 * @file rule_spatial_relation.h
 * 
 * @brief
 * 
 * a class used to describe a rule for a spatial relationship
 */

#ifndef H2SL_RULE_SPATIAL_RELATION_H
#define H2SL_RULE_SPATIAL_RELATION_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/symbol_dictionary.h"
#include "h2sl/world.h"
#include "h2sl/rule.h"

namespace h2sl {
  /**
   * Rule_Spatial_Relation class definition
   */
  class Rule_Spatial_Relation : public Rule {
  public:

    Rule_Spatial_Relation( const std::string& spatialRelationType = "na" );
    Rule_Spatial_Relation( const Rule_Spatial_Relation& other );
    Rule_Spatial_Relation( xmlNodePtr root, World* world );
    virtual ~Rule_Spatial_Relation();
    Rule_Spatial_Relation& operator=( const Rule_Spatial_Relation& other );
    bool operator==( const Rule_Spatial_Relation& other )const;
    bool operator!=( const Rule_Spatial_Relation& other )const;
    virtual Rule_Spatial_Relation* dup( void )const;

    virtual std::string evaluate_cv( const Grounding_Set* groundingSet )const;
    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "rule_spatial_relation" ); };
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

    static std::string class_name( void ){ return "rule_spatial_relation"; };

  protected:

  private:

  };

  /** 
   * Rule_Spatial_Relation class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Rule_Spatial_Relation& other );
}

#endif /* H2SL_RULE_SPATIAL_RELATION_H */
