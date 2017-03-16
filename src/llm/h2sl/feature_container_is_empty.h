/**
 * @file feature_container_is_empty.h
 *
 * @brief
 *
 * Class used to describe a feature container matches child..
 */

#ifndef H2SL_FEATURE_CONTAINER_IS_EMPTY_H
#define H2SL_FEATURE_CONTAINER_IS_EMPTY_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Container_Is_Empty
     */
    class Feature_Container_Is_Empty: public Feature {
    public:
        Feature_Container_Is_Empty( const bool& invert = false );
        Feature_Container_Is_Empty( xmlNodePtr root );
        virtual ~Feature_Container_Is_Empty();
        Feature_Container_Is_Empty( const Feature_Container_Is_Empty& other );
        Feature_Container_Is_Empty& operator=( const Feature_Container_Is_Empty& other );
        
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
        
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        
    private:
        
    };
    
    /**
     * Feature_Container_Is_Empty class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Container_Is_Empty& other );
}

#endif /* H2SL_FEATURE_CONTAINER_IS_EMPTY_H */



