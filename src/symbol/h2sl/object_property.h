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
    Object_Property( const std::string& objectType = "na", const std::string& spatial_relationType = "na", const int& index = 0 );
    Object_Property( const Object_Property& other );
    Object_Property( xmlNodePtr root, World* world );
    virtual ~Object_Property();
    Object_Property& operator=( const Object_Property& other );
    bool operator==( const Object_Property& other )const;
    bool operator!=( const Object_Property& other )const;
    virtual Object_Property* dup( void )const;

    virtual std::string evaluate_cv( const Grounding_Set* groundingSet )const;
    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "object_property" ); };
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

    inline std::string& type( void ){ return get_prop< std::string >( _string_properties, "object_type" ); };
    inline const std::string& type( void )const{ return get_prop< std::string >( _string_properties, "object_type" ); };
    
    inline std::string& relation_type( void ){ return get_prop< std::string >( _string_properties, "spatial_relation_type" ); };
    inline const std::string& relation_type( void )const{ return get_prop< std::string >( _string_properties, "spatial_relation_type" ); };
    
    inline int& index( void ){ return get_prop< int >( _int_properties, "index" ); };
    inline const int& index( void )const{ return get_prop< int >( _int_properties, "index" ); };

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
