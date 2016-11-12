/**
 * @file feature_container_spatial_concept.h
 * 
 * @brief
 * 
 * Class used to describe a feature of the container_spatial_concept
 */

#ifndef H2SL_FEATURE_CONTAINER_SPATIAL_CONCEPT_H
#define H2SL_FEATURE_CONTAINER_SPATIAL_CONCEPT_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"
#include "h2sl/object.h"

namespace h2sl {
  /**
   * Feature_Container_Spatial_Concept class definition
   */
  class Feature_Container_Spatial_Concept : public Feature {
  public:
    Feature_Container_Spatial_Concept( const bool& invert = false, const double& model_deviation_tolerance = 0.1, const double& adjacent_distance_tolerance = 0.1, const unsigned int& min_num_elements = 2 );

    Feature_Container_Spatial_Concept( const Feature_Container_Spatial_Concept& other );
    virtual ~Feature_Container_Spatial_Concept();
    Feature_Container_Spatial_Concept& operator=( const Feature_Container_Spatial_Concept& other );
 
    virtual bool value( const unsigned int& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world );
    virtual bool value( const unsigned int& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world, const Grounding* context );
    
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    double& model_deviation_tolerance( void );
    const double& model_deviation_tolerance( void ) const;

    double& adjacent_distance_tolerance( void );
    const double& adjacent_distance_tolerance( void ) const;

    unsigned int& min_num_elements( void );
    const unsigned int& min_num_elements( void )const;

    virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };

    static bool container_expresses_row( const std::vector < Object* >& container_objects, const double& model_deviation_tolerance = 0.1, const double& adjacent_distance_tolerance = 0.1, const unsigned int& min_num_elements = 2 );
    static bool container_expresses_column( const std::vector < Object* >& container_objects, const double& model_deviation_tolerance = 0.1, const double& adjacent_distance_tolerance = 0.1, const unsigned int& min_num_elements = 2 );
    static bool container_expresses_tower( const std::vector < Object* >& container_objects, const double& model_deviation_tolerance = 0.1, const double& adjacent_distance_tolerance = 0.1, const unsigned int& min_num_elements = 2 );
    static bool container_expresses_group( const std::vector < Object* >& container_objects, const double& model_deviation_tolerance = 0.1, const double& adjacent_distance_tolerance = 0.1, const unsigned int& min_num_elements = 2 );

    static bool container_expresses_row( std::vector < Object* >& container_objects, const double& model_deviation_tolerance = 0.1, const double& adjacent_distance_tolerance = 0.1, const unsigned int& min_num_elements = 2 );

  protected:
    
    //bool container_expresses_row( const std::vector < Object* >& container_objects );
    //bool container_expresses_column( const std::vector < Object* >& container_objects );
    //bool container_expresses_tower( const std::vector < Object* >& container_objects );
    //bool container_expresses_group( const std::vector < Object* >& container_objects );
    
    double _model_deviation_tolerance;
    double _adjacent_distance_tolerance;
    unsigned int _min_num_elements;

  private:

  };

  /** 
   * Feature_Container_Spatial_Concept class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Container_Spatial_Concept& other );
}

#endif /* H2SL_FEATURE_CONTAINER_SPATIAL_CONCEPT_H */
