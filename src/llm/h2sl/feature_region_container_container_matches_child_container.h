/**
 * @file feature_region_container_container_matches_child_container.h
 *
 * @brief
 *
 * Class used to describe a feature object matches child..
 */

#ifndef H2SL_FEATURE_REGION_CONTAINER_CONTAINER_MATCHES_CHILD_CONTAINER_H
#define H2SL_FEATURE_REGION_CONTAINER_CONTAINER_MATCHES_CHILD_CONTAINER_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Region_Container_Container_Matches_Child_Container
     */
    class Feature_Region_Container_Container_Matches_Child_Container: public Feature {
    public:
        Feature_Region_Container_Container_Matches_Child_Container( const bool& invert = false );
        virtual ~Feature_Region_Container_Container_Matches_Child_Container();
        Feature_Region_Container_Container_Matches_Child_Container( const Feature_Region_Container_Container_Matches_Child_Container& other );
        Feature_Region_Container_Container_Matches_Child_Container& operator=( const Feature_Region_Container_Container_Matches_Child_Container& other );
        
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
            
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
        
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        
    private:
        
    };
    
    /**
     * Feature_Region_Container_Container_Matches_Child_Container class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Region_Container_Container_Matches_Child_Container& other );
}

#endif /* H2SL_FEATURE_REGION_CONTAINER_CONTAINER_MATCHES_CHILD_CONTAINER_H */



