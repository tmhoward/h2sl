/**
 * @file feature_container_merge_abstract_container_region.h
 *
 * @brief
 *
 * Class used to describe a feature container matches child..
 */

#ifndef H2SL_FEATURE_CONTAINER_MERGE_ABSRACT_CONTAINER_REGION_H
#define H2SL_FEATURE_CONTAINER_MERGE_ABSRACT_CONTAINER_REGION_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Container_Merge_Abstract_Container_Region
     */
    class Feature_Container_Merge_Abstract_Container_Region: public Feature {
    public:
        Feature_Container_Merge_Abstract_Container_Region( const bool& invert = false, const std::string& spatialRelationType = "na", const std::string& sortingKey = "na" );
        Feature_Container_Merge_Abstract_Container_Region( xmlNodePtr root );
        virtual ~Feature_Container_Merge_Abstract_Container_Region();
        Feature_Container_Merge_Abstract_Container_Region( const Feature_Container_Merge_Abstract_Container_Region& other );
        Feature_Container_Merge_Abstract_Container_Region& operator=( const Feature_Container_Merge_Abstract_Container_Region& other );
        
        virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
       
        inline std::string& spatial_relation_type( void ){ return get_prop< std::string >( _string_properties, "spatial_relation_type" ); };
        inline const std::string& spatial_relation_type( void )const{ return get_prop< std::string >( _string_properties, "spatial_relation_type" ); };
        inline std::string& sorting_key( void ){ return get_prop< std::string >( _string_properties, "sorting_key" ); };
        inline const std::string& sorting_key( void )const{ return get_prop< std::string >( _string_properties, "sorting_key" ); };
 
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        
    private:
        
    };
    
    /**
     * Feature_Container_Merge_Abstract_Container_Region class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Container_Merge_Abstract_Container_Region& other );
}

#endif /* H2SL_FEATURE_CONTAINER_MERGE_ABSRACT_CONTAINER_REGION_H */



