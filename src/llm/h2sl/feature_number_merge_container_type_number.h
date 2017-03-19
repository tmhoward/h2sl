/**
 * @file feature_number_merge_container_type_number.h
 *
 * @brief
 *
 * Class used to describe a feature container matches child..
 */

#ifndef H2SL_FEATURE_NUMBER_MERGE_CONTAINER_TYPE_NUMBER_H
#define H2SL_FEATURE_NUMBER_MERGE_CONTAINER_TYPE_NUMBER_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Number_Merge_Container_Type_Number
     */
    class Feature_Number_Merge_Container_Type_Number: public Feature {
    public:
        Feature_Number_Merge_Container_Type_Number( const bool& invert = false );
        virtual ~Feature_Number_Merge_Container_Type_Number();
        Feature_Number_Merge_Container_Type_Number( const Feature_Number_Merge_Container_Type_Number& other );
        Feature_Number_Merge_Container_Type_Number& operator=( const Feature_Number_Merge_Container_Type_Number& other );
        
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
        
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        
    private:
        
    };
    
    /**
     * Feature_Number_Merge_Container_Type_Number class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Number_Merge_Container_Type_Number& other );
}

#endif /* H2SL_FEATURE_NUMBER_MERGE_CONTAINER_TYPE_NUMBER_H */



