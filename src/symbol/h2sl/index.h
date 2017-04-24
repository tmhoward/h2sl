/**
 * @file index.h
 * 
 * @brief
 * 
 * a class used to describe a spatial relationship
 */

#ifndef H2SL_INDEX_H
#define H2SL_INDEX_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/grounding.h"
#include "h2sl/object.h"


namespace h2sl {
  /**
   * Index class definition
   */
  class Index : public Grounding {
  public:
    Index( const int& value = 0 );
    Index( const Index& other );
    Index( xmlNodePtr root, World* world );
    virtual ~Index();
    Index& operator=( const Index& other );
    bool operator==( const Index& other )const;
    bool operator!=( const Index& other )const;
    virtual Index* dup( void )const;

    virtual std::string evaluate_cv( const Grounding_Set* groundingSet )const;
    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "index" ); };
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

    inline int& value( void ){ return get_prop< int >( _int_properties, "value" ); };
    inline const int& value( void )const{ return get_prop< int >( _int_properties, "value" ); };

    static std::string class_name( void ){ return "index"; };

  protected:

  private:

  };

  /** 
   * Index class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Index& other );
}

#endif /* H2SL_INDEX_H */
