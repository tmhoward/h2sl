/**
 * @file feature_constraint_object_relationship.h
 *
 * @brief
 *
 * Class used to describe a feature container matches child..
 */

#ifndef H2SL_FEATURE_CONSTRAINT_OBJECT_RELATIONSHIP_H
#define H2SL_FEATURE_CONSTRAINT_OBJECT_RELATIONSHIP_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Constraint_Object_Relationship
     */
    class Feature_Constraint_Object_Relationship: public Feature {
    public:
        Feature_Constraint_Object_Relationship( const bool& invert = false, const std::string& sortingKey = "na", const std::string& referenceType = "na" );
        Feature_Constraint_Object_Relationship( xmlNodePtr root );
        virtual ~Feature_Constraint_Object_Relationship();
        Feature_Constraint_Object_Relationship( const Feature_Constraint_Object_Relationship& other );
        Feature_Constraint_Object_Relationship& operator=( const Feature_Constraint_Object_Relationship& other );
        
        virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );

        inline std::string& sorting_key( void ){ return get_prop< std::string >( _string_properties, "sorting_key" ); };
        inline const std::string& sorting_key( void )const{ return get_prop< std::string >( _string_properties, "sorting_key" ); };
        inline std::string& reference_type( void ){ return get_prop< std::string >( _string_properties, "reference_type" ); };
        inline const std::string& reference_type( void )const{ return get_prop< std::string >( _string_properties, "reference_type" ); };
        
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        
    private:
        
    };
    
    /**
     * Feature_Constraint_Object_Relationship class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Constraint_Object_Relationship& other );
}

#endif /* H2SL_FEATURE_CONSTRAINT_OBJECT_RELATIONSHIP_H */



