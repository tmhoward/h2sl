/**
 * @file feature_min_x_region_abstract_container_spatial_relation.h
 * 
 * @brief
 * 
 * Class used to describe a feature of the region_abstract_container number
 */

#ifndef H2SL_FEATURE_MIN_X_REGION_ABSTRACT_CONTAINER_SPATIAL_RELATION_H
#define H2SL_FEATURE_MIN_X_REGION_ABSTRACT_CONTAINER_SPATIAL_RELATION_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Min_X_Region_Abstract_Container_Spatial_Relation class definition
   */
  class Feature_Min_X_Region_Abstract_Container_Spatial_Relation : public Feature {
  public:
    Feature_Min_X_Region_Abstract_Container_Spatial_Relation( const bool& invert = false, const std::string& relation_type = "na" );
    Feature_Min_X_Region_Abstract_Container_Spatial_Relation( const Feature_Min_X_Region_Abstract_Container_Spatial_Relation& other );
    virtual ~Feature_Min_X_Region_Abstract_Container_Spatial_Relation();
    Feature_Min_X_Region_Abstract_Container_Spatial_Relation& operator=( const Feature_Min_X_Region_Abstract_Container_Spatial_Relation& other );
 
    virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world );
    virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world,const Grounding* context );
    
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    std::string& relation_type( void );
    const std::string& relation_type( void )const;

    virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };

  protected:
    std::string _relation_type;

  private:

  };

  /** 
   * Feature_Min_X_Region_Abstract_Container_Spatial_Relation class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Min_X_Region_Abstract_Container_Spatial_Relation& other );
}

#endif /* H2SL_FEATURE_MIN_X_REGION_ABSTRACT_CONTAINER_SPATIAL_RELATION_H */
