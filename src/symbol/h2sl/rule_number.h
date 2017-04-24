/**
 * @file rule_number.h
 * 
 * @brief
 * 
 * a class used to describe a rule for a spatial relationship
 */

#ifndef H2SL_RULE_NUMBER_H
#define H2SL_RULE_NUMBER_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/symbol_dictionary.h"
#include "h2sl/world.h"
#include "h2sl/rule.h"

namespace h2sl {
  /**
   * Rule_Number class definition
   */
  class Rule_Number : public Rule {
  public:

    Rule_Number( const int& numberValue = 0 );
    Rule_Number( const Rule_Number& other );
    Rule_Number( xmlNodePtr root, World* world );
    virtual ~Rule_Number();
    Rule_Number& operator=( const Rule_Number& other );
    bool operator==( const Rule_Number& other )const;
    bool operator!=( const Rule_Number& other )const;
    virtual Rule_Number* dup( void )const;

    virtual std::string evaluate_cv( const Grounding_Set* groundingSet )const;
    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "rule_number" ); };
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

    inline int& number_value( void ){ return get_prop< int >( _int_properties, "number" ); };
    inline const int& number_value( void )const{ return get_prop< int >( _int_properties, "number" ); };

    static std::string class_name( void ){ return "rule_number"; };

  protected:

  private:

  };

  /** 
   * Rule_Number class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Rule_Number& other );
}

#endif /* H2SL_RULE_NUMBER_H */
