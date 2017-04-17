/**
 * @file feature_rule_container_type_equals_world_objects.h
 * 
 * @brief
 * 
 * Class used to describe a feature of the container number_equals_world_objects
 */

#ifndef H2SL_FEATURE_RULE_CONTAINER_NUMBER_EQUALS_WORLD_OBJECTS_H
#define H2SL_FEATURE_RULE_CONTAINER_NUMBER_EQUALS_WORLD_OBJECTS_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Rule_Container_Type_Equals_World_Objects class definition
   */
  class Feature_Rule_Container_Type_Equals_World_Objects : public Feature {
  public:
    Feature_Rule_Container_Type_Equals_World_Objects( const bool& invert = false, const std::string& objectType = "na", const int& number = 0 );
    Feature_Rule_Container_Type_Equals_World_Objects( xmlNodePtr root );
    Feature_Rule_Container_Type_Equals_World_Objects( const Feature_Rule_Container_Type_Equals_World_Objects& other );
    virtual ~Feature_Rule_Container_Type_Equals_World_Objects();
    Feature_Rule_Container_Type_Equals_World_Objects& operator=( const Feature_Rule_Container_Type_Equals_World_Objects& other );
 
    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
    
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    inline std::string& object_type( void ){ return get_prop< std::string >( _string_properties, "object_type" ); };
    inline const std::string& object_type( void )const{ return get_prop< std::string >( _string_properties, "object_type" ); };

    inline int& number( void ){ return get_prop< int >( _int_properties, "number" ); };
    inline const int& number( void )const{ return get_prop< int >( _int_properties, "number" ); };

    virtual inline const feature_type_t type( void )const{ return FEATURE_TYPE_GROUNDING; };

  protected:

  private:

  };

  /** 
   * Feature_Rule_Container_Type_Equals_World_Objects class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Rule_Container_Type_Equals_World_Objects& other );
}

#endif /* H2SL_FEATURE_RULE_CONTAINER_NUMBER_EQUALS_WORLD_OBJECTS_H */
