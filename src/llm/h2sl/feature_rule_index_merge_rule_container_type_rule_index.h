/**
 * @file feature_rule_index_merge_rule_container_type_rule_index.h
 *
 * @brief
 *
 * Class used to describe a feature container matches child..
 */

#ifndef H2SL_FEATURE_RULE_INDEX_MERGE_RULE_CONTAINER_TYPE_RULE_INDEX_H
#define H2SL_FEATURE_RULE_INDEX_MERGE_RULE_CONTAINER_TYPE_RULE_INDEX_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index
     */
    class Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index: public Feature {
    public:
        Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index( const bool& invert = false, const std::string& sortingKey = "na" );
        Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index( xmlNodePtr root );
        virtual ~Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index();
        Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index( const Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index& other );
        Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index& operator=( const Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index& other );
        
        virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );

        inline std::string& sorting_key( void ){ return get_prop< std::string >( _string_properties, "sorting_key" ); };
        inline const std::string& sorting_key( void )const{ return get_prop< std::string >( _string_properties, "sorting_key" ); };
        
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        
    private:
        
    };
    
    /**
     * Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index& other );
}

#endif /* H2SL_FEATURE_OBJECT_MERGE_ABSTRACT_CONTAINER_SPATIAL_RELATION_H */
