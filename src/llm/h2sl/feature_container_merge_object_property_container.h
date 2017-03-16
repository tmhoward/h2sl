/**
 * @file feature_container_merge_object_property_container.h
 *
 * @brief
 *
 * Class used to describe a feature container matches child..
 */

#ifndef H2SL_FEATURE_CONTAINER_MERGE_OBJECT_PROPERTY_CONTAINER_H
#define H2SL_FEATURE_CONTAINER_MERGE_OBJECT_PROPERTY_CONTAINER_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Container_Merge_Object_Property_Container
     */
    class Feature_Container_Merge_Object_Property_Container: public Feature {
    public:
        Feature_Container_Merge_Object_Property_Container( const bool& invert = false );
        virtual ~Feature_Container_Merge_Object_Property_Container();
        Feature_Container_Merge_Object_Property_Container( const Feature_Container_Merge_Object_Property_Container& other );
        Feature_Container_Merge_Object_Property_Container& operator=( const Feature_Container_Merge_Object_Property_Container& other );
        
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
        
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        
    private:
        
    };
    
    /**
     * Feature_Container_Merge_Object_Property_Container class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Container_Merge_Object_Property_Container& other );
}

#endif /* H2SL_FEATURE_CONTAINER_MERGE_OBJECT_PROPERTY_CONTAINER_H */



