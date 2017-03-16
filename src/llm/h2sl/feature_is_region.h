/**
 * @file feature_is_region.h
 *
 * @brief
 *
 * Class used to describe a feature object matches child..
 */

#ifndef H2SL_FEATURE_IS_REGION_H
#define H2SL_FEATURE_IS_REGION_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Is_Region
     */
    class Feature_Is_Region: public Feature {
    public:
        Feature_Is_Region( const bool& invert = false );
        virtual ~Feature_Is_Region();
        Feature_Is_Region( const Feature_Is_Region& other );
        Feature_Is_Region& operator=( const Feature_Is_Region& other );
        
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
        
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        
    private:
        
    };
    
    /**
     * Feature_Is_Region class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Is_Region& other );
}

#endif /* H2SL_FEATURE_IS_REGION_H */



