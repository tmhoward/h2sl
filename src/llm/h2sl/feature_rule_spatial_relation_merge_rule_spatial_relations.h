/**
 * @file feature_rule_spatial_relation_merge_rule_spatial_relations.h
 *
 * @brief
 *
 * Class used to describe a feature spatial_relation matches child..
 */

#ifndef H2SL_FEATURE_RULE_SPATIAL_RELATION_MERGE_RULE_SPATIAL_RELATIONS_H
#define H2SL_FEATURE_RULE_SPATIAL_RELATION_MERGE_RULE_SPATIAL_RELATIONS_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations
     */
    class Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations: public Feature {
    public:
        Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations( const bool& invert = false, const bool& matchesFirst = false );
        Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations( xmlNodePtr root );
        virtual ~Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations();
        Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations( const Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations& other );
        Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations& operator=( const Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations& other );
        
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );

        inline bool& matches_first( void ){ return _matches_first; };
        inline const bool& matches_first( void )const{ return _matches_first; };

        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        bool _matches_first;        

    private:
        
    };
    
    /**
     * Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations& other );
}

#endif /* H2SL_FEATURE_RULE_SPATIAL_RELATION_MERGE_RULE_SPATIAL_RELATIONS_H */



