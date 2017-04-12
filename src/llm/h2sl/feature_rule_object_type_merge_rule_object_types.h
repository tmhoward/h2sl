/**
 * @file feature_rule_object_type_merge_rule_object_types.h
 *
 * @brief
 *
 * Class used to describe a feature object matches child..
 */

#ifndef H2SL_FEATURE_RULE_OBJECT_TYPE_MERGE_RULE_OBJECT_TYPES_H
#define H2SL_FEATURE_RULE_OBJECT_TYPE_MERGE_RULE_OBJECT_TYPES_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Rule_Object_Type_Merge_Rule_Object_Types
     */
    class Feature_Rule_Object_Type_Merge_Rule_Object_Types: public Feature {
    public:
        Feature_Rule_Object_Type_Merge_Rule_Object_Types( const bool& invert = false, const bool& matchesFirst = false );
        Feature_Rule_Object_Type_Merge_Rule_Object_Types( xmlNodePtr root );
        virtual ~Feature_Rule_Object_Type_Merge_Rule_Object_Types();
        Feature_Rule_Object_Type_Merge_Rule_Object_Types( const Feature_Rule_Object_Type_Merge_Rule_Object_Types& other );
        Feature_Rule_Object_Type_Merge_Rule_Object_Types& operator=( const Feature_Rule_Object_Type_Merge_Rule_Object_Types& other );
        
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
     * Feature_Rule_Object_Type_Merge_Rule_Object_Types class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Rule_Object_Type_Merge_Rule_Object_Types& other );
}

#endif /* H2SL_FEATURE_RULE_OBJECT_TYPE_MERGE_RULE_OBJECT_TYPES_H */



