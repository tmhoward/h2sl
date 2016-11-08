/**
 * @file feature_region_abstract_container_object_type.h
 * 
 * @brief
 * 
 * Class used to describe a feature of the region_abstract_container type
 */

#ifndef H2SL_FEATURE_REGION_ABSTRACT_CONTAINER_OBJECT_TYPE_H
#define H2SL_FEATURE_REGION_ABSTRACT_CONTAINER_OBJECT_TYPE_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Region_Abstract_Container_Object_Type class definition
   */
  class Feature_Region_Abstract_Container_Object_Type : public Feature {
  public:
      Feature_Region_Abstract_Container_Object_Type( const bool& invert = false, const std::string& object_type = "na" );
    Feature_Region_Abstract_Container_Object_Type( const Feature_Region_Abstract_Container_Object_Type& other );
    virtual ~Feature_Region_Abstract_Container_Object_Type();
    Feature_Region_Abstract_Container_Object_Type& operator=( const Feature_Region_Abstract_Container_Object_Type& other );
 
    virtual bool value( const unsigned int& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
    virtual bool value( const unsigned int& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
    
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    //unsigned int& object_type( void );
    //const unsigned int& object_type( void )const;

    std::string& object_type( void );
    const std::string& object_type( void )const;
    
    virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };

  protected:
    //unsigned int _object_type;
    std::string _object_type;
      
  private:

  };

  /** 
   * Feature_Region_Abstract_Container_Object_Type class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Region_Abstract_Container_Object_Type& other );
}

#endif /* H2SL_FEATURE_REGION_ABSTRACT_CONTAINER_OBJECT_TYPE_H */
