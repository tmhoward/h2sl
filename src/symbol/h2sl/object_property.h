/**
 * @file object_property.h
 * 
 * @brief
 * 
 * a class used to describe a spatial relationship
 */

#ifndef H2SL_OBJECT_PROPERTY_H
#define H2SL_OBJECT_PROPERTY_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/grounding.h"

#include "h2sl/object.h"
#include "h2sl/spatial_relation.h"

/*#include "h2sl_nsf_nri_mvli/object_property_msg_t.hpp"*/

namespace h2sl {
  /**
   * Object_Property class definition
   */
  class Object_Property : public Grounding {
  public:
    Object_Property( const std::string& objectType = "na", const std::string& spatial_relationType = "na", const std::string& indexType = "na" );
    Object_Property( const Object_Property& other );
    Object_Property( xmlNodePtr root );
    virtual ~Object_Property();
    Object_Property& operator=( const Object_Property& other );
    bool operator==( const Object_Property& other )const;
    bool operator!=( const Object_Property& other )const;
    virtual Object_Property* dup( void )const;
 
    /*virtual bool from_msg( const object_property_msg_t * msg );
    virtual void to_msg( object_property_msg_t * msg )const;*/
 
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    /*static std::string index_to_std_string( const unsigned int& index );
    static unsigned int index_from_std_string( const std::string& arg );*/

    inline std::string& object_type( void ){ return get_prop< std::string >( _properties, "object_type" ); };
    inline const std::string& object_type( void )const{ return get_prop< std::string >( _properties, "object_type" ); };
    
    inline std::string& spatial_relation_type( void ){ return get_prop< std::string >( _properties, "spatial_relation_type" ); };
    inline const std::string& spatial_relation_type( void )const{ return get_prop< std::string >( _properties, "spatial_relation_type" ); };
    
    inline std::string& index_type( void ){ return get_prop< std::string >( _properties, "index_type" ); };
    inline const std::string& index_type( void )const{ return get_prop< std::string >( _properties, "index_type" ); };

  protected:
    /*unsigned int _object_type;
    unsigned int _relation_type;
    unsigned int _index;*/

  private:

  };

  /** 
   * Object_Property class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Object_Property& other );
}

#endif /* H2SL_OBJECT_PROPERTY_H */
