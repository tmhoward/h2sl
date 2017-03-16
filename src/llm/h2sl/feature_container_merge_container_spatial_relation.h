/**
 * @file feature_container_merge_container_spatial_relation.h
 *
 * @brief
 *
 * Class used to describe a feature container matches child..
 */

#ifndef H2SL_FEATURE_CONTAINER_MERGE_CONTAINER_SPATIAL_RELATION_H
#define H2SL_FEATURE_CONTAINER_MERGE_CONTAINER_SPATIAL_RELATION_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Container_Merge_Container_Spatial_Relation
     */
    class Feature_Container_Merge_Container_Spatial_Relation: public Feature {
    public:
        Feature_Container_Merge_Container_Spatial_Relation( const bool& invert = false );
        virtual ~Feature_Container_Merge_Container_Spatial_Relation();
        Feature_Container_Merge_Container_Spatial_Relation( const Feature_Container_Merge_Container_Spatial_Relation& other );
        Feature_Container_Merge_Container_Spatial_Relation& operator=( const Feature_Container_Merge_Container_Spatial_Relation& other );
        
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
        
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        
    private:
        
    };
    
    /**
     * Feature_Container_Merge_Container_Spatial_Relation class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Container_Merge_Container_Spatial_Relation& other );
}

#endif /* H2SL_FEATURE_CONTAINER_MERGE_CONTAINER_SPATIAL_RELATION_H */



