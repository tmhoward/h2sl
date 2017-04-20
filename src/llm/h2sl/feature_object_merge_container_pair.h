/**
 * @file feature_object_merge_container_pair.h
 *
 * @brief
 *
 * Class used to describe a feature container matches child..
 */

#ifndef H2SL_FEATURE_OBJECT_MERGE_CONTAINER_PAIR_H
#define H2SL_FEATURE_OBJECT_MERGE_CONTAINER_PAIR_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Object_Merge_Container_Pair
     */
    class Feature_Object_Merge_Container_Pair: public Feature {
    public:
        Feature_Object_Merge_Container_Pair( const bool& invert = false, const std::string& sortingKey = "na" );
        Feature_Object_Merge_Container_Pair( xmlNodePtr root );
        virtual ~Feature_Object_Merge_Container_Pair();
        Feature_Object_Merge_Container_Pair( const Feature_Object_Merge_Container_Pair& other );
        Feature_Object_Merge_Container_Pair& operator=( const Feature_Object_Merge_Container_Pair& other );
        
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
       
        inline std::string& sorting_key( void ){ return get_prop< std::string >( _string_properties, "sorting_key" ); };
        inline const std::string& sorting_key( void )const{ return get_prop< std::string >( _string_properties, "sorting_key" ); };
 
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        
    private:
        
    };
    
    /**
     * Feature_Object_Merge_Container_Pair class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Object_Merge_Container_Pair& other );
}

#endif /* H2SL_FEATURE_OBJECT_MERGE_CONTAINER_PAIR_H */



