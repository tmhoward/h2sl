/**
 * @file feature_container_max_distance 
 * 
 * @brief
 * 
 */

#ifndef H2SL_FEATURE_CONTAINER_MAX_DISTANCE_H
#define H2SL_FEATURE_CONTAINER_MAX_DISTANCE_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Container_Max_Distance class definition
   */
  class Feature_Container_Max_Distance : public Feature {
  public:
    Feature_Container_Max_Distance( const bool& invert = false );
    Feature_Container_Max_Distance( const Feature_Container_Max_Distance& other );
    virtual ~Feature_Container_Max_Distance();
    Feature_Container_Max_Distance& operator=( const Feature_Container_Max_Distance& other );
 
    virtual bool value( const std::string& cv, 
                        const Grounding* grounding, 
                        const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, 
                        const Phrase* phrase, 
                        const World* world );
 
   virtual bool value( const std::string& cv, 
                       const Grounding* grounding, 
                       const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, 
                       const Phrase* phrase, 
                       const World* world, 
                       const Grounding* context );
    
    virtual void from_xml( const std::string& file );
    virtual void from_xml( xmlNodePtr root );
    virtual void to_xml( const std::string& file )const;
    virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;

    virtual inline const feature_type_t type( void )const{ return FEATURE_TYPE_GROUNDING; };

  protected:

  private:

  };

  /** 
   * Feature_Container_Max_Distance class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Container_Max_Distance& other );
}

#endif /* H2SL_FEATURE_CONTAINER_MAX_DISTANCE_H */
