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

    Container( const std::vector< Object* >& container  = std::vector< Object* >(), const std::string& containerType = "group" );
    Container( const Container& other );
    virtual ~Container();
    Container& operator=( const Container& other );
    bool operator==( const Container& other )const;
    bool operator!=( const Container& other )const;
    Container* dup( void )const;    

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

    /*static std::string type_to_std_string( const Type type );
    static Type type_from_std_string( const std::string& arg );*/
    
    inline std::vector< Object* >& container( void ){ return _container; };
    inline const std::vector< Object* >& container( void )const{ return _container; };

    inline std::string& container_type( void ){ return get_prop< std::string >( _properties, "container_type" ); };
    inline const std::string& container_type( void )const{ return get_prop< std::string >( _properties, "container_type" ); };

  protected:
    std::vector< Object* > _container;
    /*unsigned int _type;*/
  private:

  };

  /**
   * Container class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Container& other );
}

#endif /* H2SL_CONTAINER_H */
