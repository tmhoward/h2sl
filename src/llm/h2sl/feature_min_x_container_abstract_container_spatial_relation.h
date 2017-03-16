/**
 * @file feature_min_x_container_abstract_container_spatial_relation.h
 * 
 * @brief
 * 
 * Class used to describe a feature of the container_abstract_container number
 */

#ifndef H2SLFEATURE_MIN_X_CONTAINER_ABSTRACT_CONTAINER_SPATIAL_RELATION_H
#define H2SLFEATURE_MIN_X_CONTAINER_ABSTRACT_CONTAINER_SPATIAL_RELATION_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Min_X_Container_Abstract_Container_Spatial_Relation class definition
   */
  class Feature_Min_X_Container_Abstract_Container_Spatial_Relation : public Feature {
  public:
    Feature_Min_X_Container_Abstract_Container_Spatial_Relation( const bool& invert = false, 
						const std::string& relationType = "na" );

    Feature_Min_X_Container_Abstract_Container_Spatial_Relation( const Feature_Min_X_Container_Abstract_Container_Spatial_Relation& other );
    virtual ~Feature_Min_X_Container_Abstract_Container_Spatial_Relation();
    Feature_Min_X_Container_Abstract_Container_Spatial_Relation& operator=( const Feature_Min_X_Container_Abstract_Container_Spatial_Relation& other );

    virtual bool value( const std::string& cv, 
                        const Grounding* grounding, 
                        const std::vector<std::pair<const Phrase*, std::vector<Grounding*>>>& children, 
                        const Phrase* phrase, 
                        const World* world );

    virtual bool value( const std::string& cv, 
                        const Grounding* grounding, 
                        const std::vector<std::pair<const Phrase*, std::vector<Grounding*>>>& children, 
                        const Phrase* phrase, 
                        const World* world,
                        const Grounding* context );

    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    inline std::string& relation_type( void ) {return _relation_type;};
    inline const std::string& relation_type( void )const{return _relation_type;};

    virtual inline const feature_type_t type( void )const{ return FEATURE_TYPE_GROUNDING; };

  protected:
    std::string _relation_type;

  private:

  };

  /** 
   * Feature_Min_X_Container_Abstract_Container_Spatial_Relation class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Min_X_Container_Abstract_Container_Spatial_Relation& other );
}

#endif /* H2SL_FEATURE_MIN_X_CONTAINER_ABSTRACT_CONTAINER_SPATIAL_RELATION_H */


