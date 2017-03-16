/**
 * @file feature_phrase_has_pos_tag.h
 *
 * @brief
 *
 * Class used to describe a feature object matches child..
 */

#ifndef H2SL_FEATURE_PHRASE_HAS_POS_TAG_H
#define H2SL_FEATURE_PHRASE_HAS_POS_TAG_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Phrase_Has_POS_Tag
     */
    class Feature_Phrase_Has_POS_Tag: public Feature {
    public:
        Feature_Phrase_Has_POS_Tag( const bool& invert = false, const unsigned int& pos = h2sl::POS_UNKNOWN );
        virtual ~Feature_Phrase_Has_POS_Tag();
        Feature_Phrase_Has_POS_Tag( const Feature_Phrase_Has_POS_Tag& other );
        Feature_Phrase_Has_POS_Tag& operator=( const Feature_Phrase_Has_POS_Tag& other );
        
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
        
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        unsigned int _pos;
        
    private:
        
    };
    
    /**
     * Feature_Phrase_Has_POS_Tag class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Phrase_Has_POS_Tag& other );
}

#endif /* H2SL_FEATURE_PHRASE_HAS_POS_TAG_H */



