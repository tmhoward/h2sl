/**
 * @file feature_object_matches_child_container_object.h
 *
 * @brief
 *
 * Class used to describe a feature object matches child_container_object..
 */

#ifndef H2SL_FEATURE_OBJECT_MATCHES_CHILD_CONTAINER_OBJECT_H
#define H2SL_FEATURE_OBJECT_MATCHES_CHILD_CONTAINER_OBJECT_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Object_Matches_Child_Container_Object
     */
    class Feature_Object_Matches_Child_Container_Object: public Feature {
    public:
        Feature_Object_Matches_Child_Container_Object( const bool& invert = false );
        Feature_Object_Matches_Child_Container_Object( xmlNodePtr root );
        virtual ~Feature_Object_Matches_Child_Container_Object();
        Feature_Object_Matches_Child_Container_Object( const Feature_Object_Matches_Child_Container_Object& other );
        Feature_Object_Matches_Child_Container_Object& operator=( const Feature_Object_Matches_Child_Container_Object& other );
        
        virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
        
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        
    private:
        
    };
    
    /**
     * Feature_Object_Matches_Child_Container_Object class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Object_Matches_Child_Container_Object& other );
}

#endif /* H2SL_FEATURE_OBJECT_MATCHES_CHILD_CONTAINER_OBJECT_H */



