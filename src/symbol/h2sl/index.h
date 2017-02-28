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
    Index( xmlNodePtr root );
    virtual ~Index();
    Index& operator=( const Index& other );
    bool operator==( const Index& other )const;
    bool operator!=( const Index& other )const;
    virtual Index* dup( void )const;

    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "index" ); };
    static void fill_search_space( const Symbol_Dictionary& symbolDictionary, const World* world, std::map< std::string, std::pair< unsigned int, std::vector< Grounding* > > >& searchSpaces, const symbol_type_t& symbolType );
 
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

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
