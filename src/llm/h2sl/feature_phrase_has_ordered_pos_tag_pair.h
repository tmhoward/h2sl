/**
 * @file feature_phrase_has_ordered_pos_tag_pair.h
 *
 * @brief
 *
 * Class used to describe a feature object matches child..
 */

#ifndef H2SL_FEATURE_PHRASE_HAS_ORDERED_POS_TAG_PAIR_H
#define H2SL_FEATURE_PHRASE_HAS_ORDERED_POS_TAG_PAIR_H

#include <iostream>
#include <libxml/tree.h>

#include "h2sl/feature.h"

namespace h2sl {
    /**
     * Feature_Phrase_Has_Ordered_POS_Tag_Pair
     */
    class Feature_Phrase_Has_Ordered_POS_Tag_Pair: public Feature {
    public:
        Feature_Phrase_Has_Ordered_POS_Tag_Pair( const bool& invert = false, const unsigned int& pos_1 = h2sl::POS_UNKNOWN, const unsigned int& pos_2 = h2sl::POS_UNKNOWN );
        virtual ~Feature_Phrase_Has_Ordered_POS_Tag_Pair();
        Feature_Phrase_Has_Ordered_POS_Tag_Pair( const Feature_Phrase_Has_Ordered_POS_Tag_Pair& other );
        Feature_Phrase_Has_Ordered_POS_Tag_Pair& operator=( const Feature_Phrase_Has_Ordered_POS_Tag_Pair& other );
        
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world );
        virtual bool value( const std::string& cv, const Grounding* grounding, const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children, const Phrase* phrase, const World* world, const Grounding* context );
        
        virtual void to_xml( xmlDocPtr doc, xmlNodePtr root )const;
        
        virtual void from_xml( xmlNodePtr root );
        
        virtual inline const h2sl::feature_type_t type( void )const{ return h2sl::FEATURE_TYPE_GROUNDING; };
        
    protected:
        unsigned int _pos_1;
        unsigned int _pos_2;
        
    private:
        
    };
    
    /**
     * Feature_Phrase_Has_Ordered_POS_Tag_Pair class ostream operator
     */
    
    std::ostream& operator<<( std::ostream& out, const Feature_Phrase_Has_Ordered_POS_Tag_Pair& other );
}

#endif /* H2SL_FEATURE_PHRASE_HAS_ORDERED_POS_TAG_PAIR_H */



