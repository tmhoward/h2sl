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
/*#include "h2sl_nsf_nri_mvli/spatial_relation.h"

#include "h2sl_nsf_nri_mvli/index_msg_t.hpp"*/

namespace h2sl {
  /**
   * Index class definition
   */
  class Index : public Grounding {
  public:
    enum Type {
      TYPE_FIRST,
      TYPE_SECOND,
      TYPE_THIRD,
      TYPE_FOURTH,
      TYPE_FIFTH,
      TYPE_UNKNOWN,
      TYPE_NUM_TYPES
    };

    Index( const Type& type = Index::TYPE_UNKNOWN );
    Index( const Index& other );
    virtual ~Index();
    Index& operator=( const Index& other );
    bool operator==( const Index& other )const;
    bool operator!=( const Index& other )const;
    virtual Index* dup( void )const;
 
    /*virtual bool from_msg( const index_msg_t * msg );
    virtual void to_msg( index_msg_t * msg )const;*/
 
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    static std::string type_to_std_string( const Type& type );
    static Index::Type type_from_std_string( const std::string& arg );

    inline Type& type( void ){ return _type; };
    inline const Type& type( void )const{ return _type; };

  protected:
    Type _type;

  private:

  };

  /** 
   * Index class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Index& other );
}

#endif /* H2SL_INDEX_H */
