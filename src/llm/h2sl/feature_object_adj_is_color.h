/**
 * @file feature_object_adj_is_color.h
 *
 * @brief
 *
 * a class used to describe a feature of the object type
 */

#ifndef H2SL_FEATURE_OBJECT_ADJ_IS_COLOR_H
#define H2SL_FEATURE_OBJECT_ADJ_IS_COLOR_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Adj_Is_Color class definition
     */
    class Feature_Object_Adj_Is_Color : public Feature {
    public:
        Feature_Object_Adj_Is_Color( const bool& invert = false );
        Feature_Object_Adj_Is_Color( const Feature_Object_Adj_Is_Color& other );
        virtual ~Feature_Object_Adj_Is_Color();
        Feature_Object_Adj_Is_Color& operator=( const Feature_Object_Adj_Is_Color& other );
        
        virtual bool value( const unsigned int& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world );
        virtual bool value( const unsigned int& cv, const h2sl::Grounding* grounding, const std::vector< std::pair< const h2sl::Phrase*, std::vector< h2sl::Grounding* > > >& children, const h2sl::Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void from_xml( const std::string& file );
        virtual void from_xml( xmlNodePtr root );
        virtual void to_xml( const std::string& file )const;
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        unsigned int& object_color( void );
        const unsigned int& object_color( void )const;
        
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        unsigned int _object_color;
        
    private:
        
    };
    
    /**
     * Feature_Adj_Is_Color class ostream operator
     */
    std::ostream& operator<<( std::ostream& out, const Feature_Object_Adj_Is_Color& other );
}

#endif /* H2SL_FEATURE_OBJECT_ADJ_IS_COLOR_H */





