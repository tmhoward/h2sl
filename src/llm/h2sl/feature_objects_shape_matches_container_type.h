/**
 * @file feature_objects_shape_matches_container_type.h
 * 
 * @brief
 * 
 * Class used to describe a feature of the abstract_container number
 */

#ifndef H2SL_FEATURE_OBJECTS_SHAPE_MATCHES_CONTAINER_TYPE_H
#define H2SL_FEATURE_OBJECTS_SHAPE_MATCHES_CONTAINER_TYPE_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"
#include "h2sl/object.h"

namespace h2sl {
  /**
   * Feature_Objects_Shape_Matches_Container_Type class definition
   */
  class Feature_Objects_Shape_Matches_Container_Type : public Feature {
  public:
    Feature_Objects_Shape_Matches_Container_Type( const bool& invert = false );
    Feature_Objects_Shape_Matches_Container_Type( const Feature_Objects_Shape_Matches_Container_Type& other );
    virtual ~Feature_Objects_Shape_Matches_Container_Type();
    Feature_Objects_Shape_Matches_Container_Type& operator=( const Feature_Objects_Shape_Matches_Container_Type& other );

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

    std::pair< double, double > convert_to_rho_theta(double x, double y);
    bool in_a_row( std::vector< Object* > container_objects, unsigned int counter );
    bool in_a_column( std::vector< Object* > container_objects, unsigned int counter );
    bool in_a_tower();

    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };

  protected:
    const double _rho;
    const double _theta;
  private:

  };

  /** 
   * Feature_Objects_Shape_Matches_Container_Type class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Objects_Shape_Matches_Container_Type& other );
}

#endif /* H2SL_FEATURE_OBJECTS_SHAPE_MATCHES_CONTAINER_TYPE_H */


