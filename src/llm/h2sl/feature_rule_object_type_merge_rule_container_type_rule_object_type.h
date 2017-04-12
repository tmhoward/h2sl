/**
 * @file feature_rule_object_type_merge_rule_container_type_rule_object_type.h
 *
 * @brief
 *
 * Class used to describe a feature container matches child..
 */

#ifndef H2SL_FEATURE_RULE_OBJECT_TYPE_MERGE_RULE_CONTAINER_TYPE_RULE_OBJECT_TYPE_H
#define H2SL_FEATURE_RULE_OBJECT_TYPE_MERGE_RULE_CONTAINER_TYPE_RULE_OBJECT_TYPE_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Rule_Object_Type_Merge_Rule_Container_Type_Rule_Object_Type
     */
    class Feature_Rule_Object_Type_Merge_Rule_Container_Type_Rule_Object_Type: public Feature {
    public:
        Feature_Rule_Object_Type_Merge_Rule_Container_Type_Rule_Object_Type( const bool& invert = false, const std::string& sortingKey = "na" );
        Feature_Rule_Object_Type_Merge_Rule_Container_Type_Rule_Object_Type( xmlNodePtr root );
        virtual ~Feature_Rule_Object_Type_Merge_Rule_Container_Type_Rule_Object_Type();
        Feature_Rule_Object_Type_Merge_Rule_Container_Type_Rule_Object_Type( const Feature_Rule_Object_Type_Merge_Rule_Container_Type_Rule_Object_Type& other );
        Feature_Rule_Object_Type_Merge_Rule_Container_Type_Rule_Object_Type& operator=( const Feature_Rule_Object_Type_Merge_Rule_Container_Type_Rule_Object_Type& other );
        
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
     * Feature_Rule_Object_Type_Merge_Rule_Container_Type_Rule_Object_Type class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Rule_Object_Type_Merge_Rule_Container_Type_Rule_Object_Type& other );
}

#endif /* H2SL_FEATURE_OBJECT_MERGE_ABSTRACT_CONTAINER_SPATIAL_RELATION_H */
