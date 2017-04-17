/**
 * @file feature_container_merge_abstract_container_container_type.h
 *
 * @brief
 *
 * Class used to describe a feature container matches child..
 */

#ifndef H2SL_FEATURE_CONTAINER_MERGE_ABSTRACT_CONTAINER_CONTAINER_TYPE_H
#define H2SL_FEATURE_CONTAINER_MERGE_ABSTRACT_CONTAINER_CONTAINER_TYPE_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Container_Merge_Abstract_Container_Container_Type
     */
    class Feature_Container_Merge_Abstract_Container_Container_Type: public Feature {
    public:
        Feature_Container_Merge_Abstract_Container_Container_Type( const bool& invert = false );
        Feature_Container_Merge_Abstract_Container_Container_Type( xmlNodePtr root );
        virtual ~Feature_Container_Merge_Abstract_Container_Container_Type();
        Feature_Container_Merge_Abstract_Container_Container_Type( const Feature_Container_Merge_Abstract_Container_Container_Type& other );
        Feature_Container_Merge_Abstract_Container_Container_Type& operator=( const Feature_Container_Merge_Abstract_Container_Container_Type& other );
        
        virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
       
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        
    private:
        
    };
    
    /**
     * Feature_Container_Merge_Abstract_Container_Container_Type class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Container_Merge_Abstract_Container_Container_Type& other );
}

#endif /* H2SL_FEATURE_CONTAINER_MERGE_ABSTRACT_CONTAINER_CONTAINER_TYPE_H */



