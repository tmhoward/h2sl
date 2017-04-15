/**
 * @file feature_container_merge_container_type_container.h
 *
 * @brief
 *
 * Class used to describe a feature container matches child..
 */

#ifndef H2SL_FEATURE_CONTAINER_MERGE_CONTAINER_TYPE_CONTAINER_H
#define H2SL_FEATURE_CONTAINER_MERGE_CONTAINER_TYPE_CONTAINER_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Container_Merge_Container_Type_Container
     */
    class Feature_Container_Merge_Container_Type_Container: public Feature {
    public:
        Feature_Container_Merge_Container_Type_Container( const bool& invert = false, const bool& order = false );
        Feature_Container_Merge_Container_Type_Container( xmlNodePtr root );
        virtual ~Feature_Container_Merge_Container_Type_Container();
        Feature_Container_Merge_Container_Type_Container( const Feature_Container_Merge_Container_Type_Container& other );
        Feature_Container_Merge_Container_Type_Container& operator=( const Feature_Container_Merge_Container_Type_Container& other );
        
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
        
        inline bool& order( void ){ return _order; };
        inline const bool& order( void )const{ return _order; };

        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        bool _order;
        
    private:
        
    };
    
    /**
     * Feature_Container_Merge_Container_Type_Container class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Container_Merge_Container_Type_Container& other );
}

#endif /* H2SL_FEATURE_CONTAINER_MERGE_CONTAINER_TYPE_CONTAINER_H */



