/**
 * @file rule_index.h
 * 
 * @brief
 * 
 * a class used to describe a rule for a spatial relationship
 */

#ifndef H2SL_RULE_INDEX_H
#define H2SL_RULE_INDEX_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/symbol_dictionary.h"
#include "h2sl/world.h"
#include "h2sl/rule.h"

namespace h2sl {
  /**
   * Rule_Index class definition
   */
  class Rule_Index : public Rule {
  public:

    Rule_Index( const int& indexValue = 0 );
    Rule_Index( const Rule_Index& other );
    Rule_Index( xmlNodePtr root, World* world );
    virtual ~Rule_Index();
    Rule_Index& operator=( const Rule_Index& other );
    bool operator==( const Rule_Index& other )const;
    bool operator!=( const Rule_Index& other )const;
    virtual Rule_Index* dup( void )const;

    virtual std::string evaluate_cv( const Grounding_Set* groundingSet )const;
    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "rule_index" ); };
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

    inline int& index_value( void ){ return get_prop< int >( _int_properties, "index" ); };
    inline const int& index_value( void )const{ return get_prop< int >( _int_properties, "index" ); };

    static std::string class_name( void ){ return "rule_index"; };

  protected:

  private:

  };

  /** 
   * Rule_Index class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Rule_Index& other );
}

#endif /* H2SL_RULE_INDEX_H */
