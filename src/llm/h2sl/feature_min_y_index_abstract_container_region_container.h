/**
 * @file feature_min_y_index_abstract_container_region_container.h
 * 
 * @brief
 * 
 * Class used to describe a feature of the abstract_container number
 */

#ifndef H2SL_FEATURE_MIN_Y_INDEX_ABSTRACT_CONTAINER_REGION_CONTAINER_H
#define H2SL_FEATURE_MIN_Y_INDEX_ABSTRACT_CONTAINER_REGION_CONTAINER_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
  /**
   * Feature_Min_Y_Index_Abstract_Container_Region_Container class definition
   */
  class Feature_Min_Y_Index_Abstract_Container_Region_Container : public Feature {
  public:
    Feature_Min_Y_Index_Abstract_Container_Region_Container( const bool& invert = false, const std::string& relation_type = "na" );
    Feature_Min_Y_Index_Abstract_Container_Region_Container( const Feature_Min_Y_Index_Abstract_Container_Region_Container& other );
    virtual ~Feature_Min_Y_Index_Abstract_Container_Region_Container();
    Feature_Min_Y_Index_Abstract_Container_Region_Container& operator=( const Feature_Min_Y_Index_Abstract_Container_Region_Container& other );
 
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

    std::string& relation_type( void );
    const std::string& relation_type( void )const;

    virtual inline const feature_type_t type( void )const{ return FEATURE_TYPE_GROUNDING; };

  protected:
    std::string _relation_type;

  private:

  };

  /** 
   * Feature_Min_Y_Index_Abstract_Container_Region_Container class ostream operator
   */
  std::ostream& operator<<( std::ostream& out, const Feature_Min_Y_Index_Abstract_Container_Region_Container& other );
}

#endif /* H2SL_FEATURE_MIN_Y_INDEX_ABSTRACT_CONTAINER_REGION_CONTAINER_H */
