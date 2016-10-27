/**
 * @file region.h
 *
 * @brief
 *
 * a class that describes a region in relation to an container
 */

#ifndef H2SL_REGION_ABSTRACT_CONTAINER_H
#define H2SL_REGION_ABSTRACT_CONTAINER_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/grounding.h"
#include "h2sl/abstract_container.h"


namespace h2sl {
  /**
   * Region_Abstract_Container class definition
   */
  class Region_Abstract_Container : public Grounding {
  public:
    enum Type {
        REGION_ABSTRACT_CONTAINER_TYPE_UNKNOWN,
        REGION_ABSTRACT_CONTAINER_TYPE_FRONT,
        REGION_ABSTRACT_CONTAINER_TYPE_BACK,
        REGION_ABSTRACT_CONTAINER_TYPE_LEFT,
        REGION_ABSTRACT_CONTAINER_TYPE_CENTER,
        REGION_ABSTRACT_CONTAINER_TYPE_RIGHT,
        REGION_ABSTRACT_CONTAINER_TYPE_SIDE,
        REGION_ABSTRACT_CONTAINER_TYPE_NEAR,
        REGION_ABSTRACT_CONTAINER_TYPE_FAR,
        REGION_ABSTRACT_CONTAINER_TYPE_NUM_TYPES
    };

    Region_Abstract_Container( const std::string& region_abstract_containerType = "na", const Abstract_Container& abstract_container = Abstract_Container() );
    virtual ~Region_Abstract_Container();
    Region_Abstract_Container( const Region_Abstract_Container& other );
    Region_Abstract_Container& operator=( const Region_Abstract_Container& other );
    bool operator==( const Region_Abstract_Container& other )const;
    bool operator!=( const Region_Abstract_Container& other )const;
    virtual Region_Abstract_Container* dup( void )const;
    
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    inline unsigned int& type( void ){ return _type; };
    inline const unsigned int& type( void )const{ return _type; };
    inline Abstract_Container& abstract_container( void ){ return _abstract_container; }; 
    inline const Abstract_Container& abstract_container( void )const{ return _abstract_container; }; 
   
  protected:
    unsigned int _type;
    std::vector< Abstract_Container* > _abstract_container;

  private: 

  };

  /**
   * Region_Abstract_Container class ostream operator
   */ 
  std::ostream& operator<<( std::ostream& out, const Region_Abstract_Container& other );
}

#endif /* H2SL_REGION_ABSTRACT_CONTAINER_H */
