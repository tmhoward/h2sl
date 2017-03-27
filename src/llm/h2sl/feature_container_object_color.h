/**
 * @file feature_container_object_color.h
 * 
 * @brief
 * 
 * Class used to describe a feature of the container object_color
 */

#ifndef H2SL_FEATURE_CONTAINER_OBJECT_COLOR_H
#define H2SL_FEATURE_CONTAINER_OBJECT_COLOR_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Container_Object_Color class definition
   */
  class Feature_Container_Object_Color : public Feature {
  public:
    Feature_Container_Object_Color( const bool& invert = false, const std::string& objectColor = "na" );
    Feature_Container_Object_Color( xmlNodePtr root );
    Feature_Container_Object_Color( const Feature_Container_Object_Color& other );
    virtual ~Feature_Container_Object_Color();
    Feature_Container_Object_Color& operator=( const Feature_Container_Object_Color& other );
 
    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
    
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    inline std::string& object_color( void ){ return get_prop< std::string >( _string_properties, "object_color" ); };
    inline const std::string& object_color( void )const{ return get_prop< std::string >( _string_properties, "object_color" ); };

    virtual inline const feature_type_t type( void )const{ return FEATURE_TYPE_GROUNDING; };

  protected:

  private:

  };

  /** 
   * Feature_Container_Object_Color class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Container_Object_Color& other );
}

#endif /* H2SL_FEATURE_CONTAINER_OBJECT_COLOR_H */
