/**
 * @file feature_container_number_equals_world_objects.h
 * 
 * @brief
 * 
 * Class used to describe a feature of the container number_equals_world_objects
 */

#ifndef H2SL_FEATURE_CONTAINER_NUMBER_EQUALS_WORLD_OBJECTS_H
#define H2SL_FEATURE_CONTAINER_NUMBER_EQUALS_WORLD_OBJECTS_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Container_Number_Equals_World_Objects class definition
   */
  class Feature_Container_Number_Equals_World_Objects : public Feature {
  public:
    Feature_Container_Number_Equals_World_Objects( const bool& invert = false );
    Feature_Container_Number_Equals_World_Objects( const Feature_Container_Number_Equals_World_Objects& other );
    virtual ~Feature_Container_Number_Equals_World_Objects();
    Feature_Container_Number_Equals_World_Objects& operator=( const Feature_Container_Number_Equals_World_Objects& other );
 
    virtual bool value( const unsigned int& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world );
    
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };

  protected:

  private:

  };

  /** 
   * Feature_Container_Number_Equals_World_Objects class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Container_Number_Equals_World_Objects& other );
}

#endif /* H2SL_FEATURE_CONTAINER_NUMBER_EQUALS_WORLD_OBJECTS_H */
