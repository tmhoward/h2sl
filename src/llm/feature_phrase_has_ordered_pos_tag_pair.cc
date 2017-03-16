/**
 * @file feature_phrase_has_ordered_pos_tag_pair.cc
 *
 * @brief
 *
 * Class used to describe a feature object matches child type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/feature_phrase_has_ordered_pos_tag_pair.h"
#include "h2sl/object.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Phrase_Has_Ordered_POS_Tag_Pair class constructor
 */
Feature_Phrase_Has_Ordered_POS_Tag_Pair::
Feature_Phrase_Has_Ordered_POS_Tag_Pair( const bool& invert,
                            const unsigned int& pos_1,
                            const unsigned int& pos_2 ) : Feature( invert ),
                                                        _pos_1( pos_1 ), 
                                                        _pos_2( pos_2 ){
    
}

/**
 * Feature_Phrase_Has_Ordered_POS_Tag_Pair class destructor
 */
Feature_Phrase_Has_Ordered_POS_Tag_Pair::
~Feature_Phrase_Has_Ordered_POS_Tag_Pair() {
    
}

/**
 * Feature_Phrase_Has_Ordered_POS_Tag_Pair class copy constructor
 */
Feature_Phrase_Has_Ordered_POS_Tag_Pair::
Feature_Phrase_Has_Ordered_POS_Tag_Pair( const Feature_Phrase_Has_Ordered_POS_Tag_Pair& other ) : Feature( other ),
                                                                        _pos_1( other._pos_1 ),
                                                                        _pos_2( other._pos_2 ) {
    
}

/**
 * Feature_Phrase_Has_Ordered_POS_Tag_Pair class assignment operator
 */
Feature_Phrase_Has_Ordered_POS_Tag_Pair&
Feature_Phrase_Has_Ordered_POS_Tag_Pair::
operator=( const Feature_Phrase_Has_Ordered_POS_Tag_Pair& other ) {
    _invert = other._invert;
    _pos_1 = other._pos_2;
    _pos_2 = other._pos_2;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Phrase_Has_Ordered_POS_Tag_Pair::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Phrase_Has_Ordered_POS_Tag_Pair::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    if( phrase != NULL ){
      for( unsigned int i = 0; i < phrase->words().size(); i++ ){
        if( phrase->words()[ i ].pos() == _pos_1 ){
          for( unsigned int j = i; j < phrase->words().size(); j++ ){
            if( phrase->words()[ j ].pos() == _pos_2 ){
              return !_invert;
            }
          }
        } 
      }
      return _invert;
    } else {
      return false;
    }
}

/**
 * exports the Feature_Phrase_Has_Ordered_POS_Tag_Pair class to an XML file
 */
void
Feature_Phrase_Has_Ordered_POS_Tag_Pair::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_phrase_has_ordered_pos_tag_pair" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "pos_1" ), ( const xmlChar* )( h2sl::pos_t_to_std_string( ( h2sl::pos_t )( _pos_1 ) ).c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "pos_2" ), ( const xmlChar* )( h2sl::pos_t_to_std_string( ( h2sl::pos_t )( _pos_2 ) ).c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Phrase_Has_Ordered_POS_Tag_Pair class from an XML file
 */
void
Feature_Phrase_Has_Ordered_POS_Tag_Pair::
from_xml( xmlNodePtr root ){
    _invert = false;
    if( root->type == XML_ELEMENT_NODE ){
        xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
        if( tmp != NULL ){
            string invert_string = ( char* )( tmp );
            _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
            xmlFree( tmp );
        }
        tmp = xmlGetProp( root, ( const xmlChar* )( "pos_1" ) );
        if( tmp != NULL ){
            string pos_string = ( char* )( tmp );
            _pos_1 = h2sl::pos_t_from_std_string( pos_string );
            xmlFree( tmp );
        }
        tmp = xmlGetProp( root, ( const xmlChar* )( "pos_2" ) );
        if( tmp != NULL ){
            string pos_string = ( char* )( tmp );
            _pos_2 = h2sl::pos_t_from_std_string( pos_string );
            xmlFree( tmp );
        }
    }
    return;
}

namespace h2sl {
    /**
     * Feature_Phrase_Has_Ordered_POS_Tag_Pair class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Phrase_Has_Ordered_POS_Tag_Pair& other ) {
	out << "class:\"Feature_Phrase_Has_Ordered_POS_Tag_Pair\" ostream operator does not show pos_1 or pos_2 ";        
        return out;
    }
    
}
