/**
 * @file rule_object_color.h
 * 
 * @brief
 * 
 * a class used to describe a rule for a spatial relationship
 */

#ifndef H2SL_RULE_COLOR_TYPE_H
#define H2SL_RULE_COLOR_TYPE_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/symbol_dictionary.h"
#include "h2sl/world.h"
#include "h2sl/rule.h"

namespace h2sl {
  /**
   * Rule_Object_Color class definition
   */
  class Rule_Object_Color : public Rule {
  public:

    Rule_Object_Color( const std::string& objectColor = "na" );
    Rule_Object_Color( const Rule_Object_Color& other );
    Rule_Object_Color( xmlNodePtr root, World* world );
    virtual ~Rule_Object_Color();
    Rule_Object_Color& operator=( const Rule_Object_Color& other );
    bool operator==( const Rule_Object_Color& other )const;
    bool operator!=( const Rule_Object_Color& other )const;
    virtual Rule_Object_Color* dup( void )const;

    virtual std::string evaluate_cv( const Grounding_Set* groundingSet )const;
    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "rule_object_color" ); };
    virtual void scrape_grounding( const World * world, std::map< std::string, std::vector< std::string > >& stringColors, std::map< std::string, std::vector< int > >& intColors )const;
    virtual void scrape_grounding( const World * world, std::vector< std::string >& classNames, std::map< std::string, std::vector< std::string > >& stringColors, std::map< std::string, std::vector< int > >& intColors )const;
    static void fill_search_space( const Symbol_Dictionary& symbolDictionary, const World* world, std::map< std::string, std::pair< std::string, std::vector< Grounding* > > >& searchSpaces, const std::string& symbolType );
    virtual void fill_rules( const World* world, Grounding_Set* groundingSet )const;
    virtual bool equals( const Grounding& other )const;
 
    virtual void from_xml( const std::string& file, World* world );
    virtual void from_xml( xmlNodePtr root, World* world );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual std::string to_latex( void )const;

    inline std::string& object_color( void ){ return get_prop< std::string >( _string_properties, "object_color" ); };
    inline const std::string& object_color( void )const{ return get_prop< std::string >( _string_properties, "object_color" ); };

    static std::string class_name( void ){ return "rule_object_color"; };

  protected:

  private:

  };

  /** 
   * Rule_Object_Color class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Rule_Object_Color& other );
}

#endif /* H2SL_RULE_COLOR_TYPE_H */
