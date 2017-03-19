/**
 * @file feature_object_property_merge_index_object_property.h
 *
 * @brief
 *
 * Class used to describe a feature container matches child..
 */

#ifndef H2SL_FEATURE_OBJECT_PROPERTY_MERGE_INDEX_OBJECT_PROPERTY_H
#define H2SL_FEATURE_OBJECT_PROPERTY_MERGE_INDEX_OBJECT_PROPERTY_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Object_Property_Merge_Index_Object_Property
     */
    class Feature_Object_Property_Merge_Index_Object_Property: public Feature {
    public:
        Feature_Object_Property_Merge_Index_Object_Property( const bool& invert = false );
        virtual ~Feature_Object_Property_Merge_Index_Object_Property();
        Feature_Object_Property_Merge_Index_Object_Property( const Feature_Object_Property_Merge_Index_Object_Property& other );
        Feature_Object_Property_Merge_Index_Object_Property& operator=( const Feature_Object_Property_Merge_Index_Object_Property& other );
        
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
        
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        
    private:
        
    };
    
    /**
     * Feature_Object_Property_Merge_Index_Object_Property class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Object_Property_Merge_Index_Object_Property& other );
}

#endif /* H2SL_FEATURE_OBJECT_PROPERTY_MERGE_INDEX_OBJECT_PROPERTY_H */



