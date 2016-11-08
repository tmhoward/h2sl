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
    Index( const std::string& indexType = "na" );
    Index( const Index& other );
    Index( xmlNodePtr root );
    virtual ~Index();
    Index& operator=( const Index& other );
    bool operator==( const Index& other )const;
    bool operator!=( const Index& other )const;
    virtual Index* dup( void )const;
 
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    inline std::string& index_type( void ){ return get_prop< std::string >( _properties, "index_type" ); };
    inline const std::string& index_type( void )const{ return get_prop< std::string >( _properties, "index_type" ); };

    static std::string class_name( void){ return "index"; };

  protected:

  private:

  };

  /** 
   * Index class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Index& other );
}

#endif /* H2SL_INDEX_H */
