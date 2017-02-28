/**
 * @file container.h
 *
 * @brief
 *
 * a class that describes a container object
 */

#ifndef H2SL_CONTAINER_H
#define H2SL_CONTAINER_H

#include <iostream>
#include <vector>
#include <libxml/tree.h>

#include "h2sl/grounding.h"
#include "h2sl/object.h"

namespace h2sl {
  /**
   * Container class definition
   */
  class Container : public Grounding {
  public:

    Container( const std::vector< Grounding* >& groundings  = std::vector< Grounding* >(), 
               const std::string& containerType = "group" );

    Container( const Container& other );
    Container( xmlNodePtr root );
    virtual ~Container();
    Container& operator=( const Container& other );
    bool operator==( const Container& other )const;
    bool operator!=( const Container& other )const;
    Container* dup( void )const;    

    virtual bool matches_class_name( const std::string& arg )const{ return ( arg == "container" ); };
    static void fill_search_space( const Symbol_Dictionary& symbolDictionary, const World* world, std::map< std::string, std::pair< unsigned int, std::vector< Grounding* > > >& searchSpaces, const symbol_type_t& symbolType );

    bool has_object( const Object* object )const;
    double min_distance_to_object( const Object& object )const;
    h2sl::Vector3 mean_position( void )const;
    h2sl::Vector3 variance_position( void )const;
    double deviation( const Object& object )const;
    double max_deviation( void )const;
    std::pair< Object*, Object* > bounds( void )const;
    h2sl::Vector3 principal_axis( void )const;

    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
    
    inline std::vector< Grounding* >& container( void ){ return _groundings; };
    inline const std::vector< Grounding* >& container( void )const{ return _groundings; };

    inline std::string& type( void ){ return get_prop< std::string >( _string_properties, "container_type" ); };
    inline const std::string& type( void )const{ return get_prop< std::string >( _string_properties, "container_type" ); };

    static std::string class_name( void ){ return "container"; };

  protected:
    std::vector< Grounding* > _groundings;
      
  private:

  };

  /**
   * Container class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Container& other );
}

#endif /* H2SL_CONTAINER_H */
