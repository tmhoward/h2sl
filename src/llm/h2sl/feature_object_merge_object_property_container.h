/**
 * @file feature_object_merge_object_property_container.h
 *
 * @brief
 *
 * Class used to describe a feature container matches child..
 */

#ifndef H2SL_FEATURE_OBJECT_MERGE_OBJECT_PROPERTY_CONTAINER_H
#define H2SL_FEATURE_OBJECT_MERGE_OBJECT_PROPERTY_CONTAINER_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Object_Merge_Object_Property_Container
     */
    class Feature_Object_Merge_Object_Property_Container: public Feature {
    public:
        Feature_Object_Merge_Object_Property_Container( const bool& invert = false, const std::string& sortingKey = "na", const std::string& spatialRelationType = "na" );
        Feature_Object_Merge_Object_Property_Container( xmlNodePtr root );
        virtual ~Feature_Object_Merge_Object_Property_Container();
        Feature_Object_Merge_Object_Property_Container( const Feature_Object_Merge_Object_Property_Container& other );
        Feature_Object_Merge_Object_Property_Container& operator=( const Feature_Object_Merge_Object_Property_Container& other );
        
        virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
       
        inline std::string& sorting_key( void ){ return get_prop< std::string >( _string_properties, "sorting_key" ); };
        inline const std::string& sorting_key( void )const{ return get_prop< std::string >( _string_properties, "sorting_key" ); };
        inline std::string& spatial_relation_type( void ){ return get_prop< std::string >( _string_properties, "spatial_relation_type" ); };
        inline const std::string& spatial_relation_type( void )const{ return get_prop< std::string >( _string_properties, "spatial_relation_type" ); };
 
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        bool _object_match;
        
    private:
        
    };
    
    /**
     * Feature_Object_Merge_Object_Property_Container class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Object_Merge_Object_Property_Container& other );
}

#endif /* H2SL_FEATURE_OBJECT_MERGE_OBJECT_PROPERTY_CONTAINER_H */



