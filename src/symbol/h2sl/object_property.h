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
 
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    inline std::string& type( void ){ return get_prop< std::string >( _properties, "object_type" ); };
    inline const std::string& type( void )const{ return get_prop< std::string >( _properties, "object_type" ); };
    
    inline std::string& relation_type( void ){ return get_prop< std::string >( _properties, "spatial_relation_type" ); };
    inline const std::string& relation_type( void )const{ return get_prop< std::string >( _properties, "spatial_relation_type" ); };
    
    inline std::string& index( void ){ return get_prop< std::string >( _properties, "index_type" ); };
    inline const std::string& index( void )const{ return get_prop< std::string >( _properties, "index_type" ); };

    static std::string class_name( void ){ return "object_property"; };


  protected:

  private:

  };

  /** 
   * Object_Property class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Object_Property& other );
}

#endif /* H2SL_OBJECT_PROPERTY_H */
