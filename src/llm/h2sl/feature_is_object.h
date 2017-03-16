/**
 * @file feature_is_object.h
 *
 * @brief
 *
 * Class used to describe a feature object matches child..
 */

#ifndef H2SL_FEATURE_IS_OBJECT_H
#define H2SL_FEATURE_IS_OBJECT_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Is_Object
     */
    class Feature_Is_Object: public Feature {
    public:
        Feature_Is_Object( const bool& invert = false );
        virtual ~Feature_Is_Object();
        Feature_Is_Object( const Feature_Is_Object& other );
        Feature_Is_Object& operator=( const Feature_Is_Object& other );
        
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
        
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        
    private:
        
    };
    
    /**
     * Feature_Is_Object class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Is_Object& other );
}

#endif /* H2SL_FEATURE_IS_OBJECT_H */



