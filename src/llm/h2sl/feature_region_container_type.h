/**
 * @file feature_region_container_type.h
 * 
 * @brief
 * 
 * a class used to describe a feature of the region_container type
 */

#ifndef H2SL_FEATURE_REGION_CONTAINER_TYPE_H
#define H2SL_FEATURE_REGION_CONTAINER_TYPE_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Region_Container_Type class definition
   */
  class Feature_Region_Container_Type : public Feature {
  public:
    Feature_Region_Container_Type( const bool& invert = false, const std::string& region_container_type = "na" );
    Feature_Region_Container_Type( const Feature_Region_Container_Type& other );
    virtual ~Feature_Region_Container_Type();
    Feature_Region_Container_Type& operator=( const Feature_Region_Container_Type& other );

    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
    virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
 
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    std::string& region_container_type( void );
    const std::string& region_container_type( void )const;

    virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };

  protected:
    std::string _region_container_type;

  private:

  };

  /** 
   * Feature_Region_Container_Type class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Region_Container_Type& other );
}

#endif /* H2SL_FEATURE_REGION_CONTAINER_TYPE_H */
