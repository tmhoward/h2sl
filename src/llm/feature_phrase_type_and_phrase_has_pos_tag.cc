/**
 * @file feature_phrase_type_and_phrase_has_pos_tag.cc
 *
 * @brief
 *
 * Class used to describe a feature object matches child type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/feature_phrase_type_and_phrase_has_pos_tag.h"
#include "h2sl/pos.h"
#include "h2sl/object.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Phrase_Type_And_Phrase_Has_POS_Tag class constructor
 */
Feature_Phrase_Type_And_Phrase_Has_POS_Tag::
Feature_Phrase_Type_And_Phrase_Has_POS_Tag( const bool& invert,
                            const unsigned int& pos,
                            const phrase_type_t& phraseType ) : Feature( invert ),
                                                        _pos( pos ),
                                                        _phrase_type( phraseType ) {
    
}

/**
 * Feature_Phrase_Type_And_Phrase_Has_POS_Tag class destructor
 */
Feature_Phrase_Type_And_Phrase_Has_POS_Tag::
~Feature_Phrase_Type_And_Phrase_Has_POS_Tag() {
    
}

/**
 * Feature_Phrase_Type_And_Phrase_Has_POS_Tag class copy constructor
 */
Feature_Phrase_Type_And_Phrase_Has_POS_Tag::
Feature_Phrase_Type_And_Phrase_Has_POS_Tag( const Feature_Phrase_Type_And_Phrase_Has_POS_Tag& other ) : Feature( other ),
                                                                        _pos( other._pos ),
                                                                        _phrase_type( other._phrase_type ) {
    
}

/**
 * Feature_Phrase_Type_And_Phrase_Has_POS_Tag class assignment operator
 */
Feature_Phrase_Type_And_Phrase_Has_POS_Tag&
Feature_Phrase_Type_And_Phrase_Has_POS_Tag::
operator=( const Feature_Phrase_Type_And_Phrase_Has_POS_Tag& other ) {
    _invert = other._invert;
    _pos = other._pos;
    _phrase_type = other._phrase_type;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Phrase_Type_And_Phrase_Has_POS_Tag::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Phrase_Type_And_Phrase_Has_POS_Tag::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    if( phrase != NULL ){
      if( phrase->type() == _phrase_type ){
        for( unsigned int i = 0; i < phrase->words().size(); i++ ){
          if( phrase->words()[ i ].pos() == _pos ){
            return !_invert;
          } 
        }
        return _invert;
      }
      return false;
    } else {
      return false;
    }
}

/**
 * exports the Feature_Phrase_Type_And_Phrase_Has_POS_Tag class to an XML file
 */
void
Feature_Phrase_Type_And_Phrase_Has_POS_Tag::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_phrase_type_and_phrase_has_pos_tag" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "pos" ), ( const xmlChar* )( h2sl::pos_t_to_std_string( ( h2sl::pos_t )( _pos ) ).c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "phrase_type" ), ( const xmlChar* )( Phrase::phrase_type_t_to_std_string( _phrase_type ).c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Phrase_Type_And_Phrase_Has_POS_Tag class from an XML file
 */
void
Feature_Phrase_Type_And_Phrase_Has_POS_Tag::
from_xml( xmlNodePtr root ){
    _invert = false;
    if( root->type == XML_ELEMENT_NODE ){
        xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
        if( tmp != NULL ){
            string invert_string = ( char* )( tmp );
            _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
            xmlFree( tmp );
        }
        tmp = xmlGetProp( root, ( const xmlChar* )( "pos" ) );
        if( tmp != NULL ){
            string pos_string = ( char* )( tmp );
            _pos = h2sl::pos_t_from_std_string( pos_string );
            xmlFree( tmp );
        }
        tmp = xmlGetProp( root, ( const xmlChar* )( "phrase_type" ) );
        if( tmp != NULL ){
            string phrase_type_string = ( char* )( tmp );
            _phrase_type = Phrase::phrase_type_t_from_std_string( phrase_type_string );
            xmlFree( tmp );
        }
    }
    return;
}

namespace h2sl {
    /**
     * Feature_Phrase_Type_And_Phrase_Has_POS_Tag class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Phrase_Type_And_Phrase_Has_POS_Tag& other ) {
	out << "class:\"Feature_Phrase_Type_And_Phrase_Has_POS_Tag(invert:(" << other.invert() << ") phrase_type:" << Phrase::phrase_type_t_to_std_string( other.phrase_type() ) << ") pos:" << pos_t_to_std_string( ( pos_t )( other.pos() ) ) << ")";
        return out;
    }
    
}
