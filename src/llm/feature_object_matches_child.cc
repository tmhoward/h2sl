/**
 * @file feature_object_matches_child.cc
 *
 * @brief
 *
 * Class used to describe a feature object matches child type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/feature_object_matches_child.h"
#include "h2sl/object.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Matches_Child class constructor
 */
Feature_Object_Matches_Child::
Feature_Object_Matches_Child( const bool& invert ) : Feature( invert ) {
    
}

/**
 * Feature_Object_Matches_Child class destructor
 */
Feature_Object_Matches_Child::
~Feature_Object_Matches_Child() {
    
}

/**
 * Feature_Object_Matches_Child class copy constructor
 */
Feature_Object_Matches_Child::
Feature_Object_Matches_Child( const Feature_Object_Matches_Child& other ) : Feature( other ) {
    
}

/**
 * Feature_Object_Matches_Child class assignment operator
 */
Feature_Object_Matches_Child&
Feature_Object_Matches_Child::
operator=( const Feature_Object_Matches_Child& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature
 */
bool
Feature_Object_Matches_Child::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}


/**
 * returns the value of the feature.
 */
bool
Feature_Object_Matches_Child::
value( const string& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context ){
    const Object * object = dynamic_cast< const Object* >( grounding );
    if( ( object != NULL ) && ( !children.empty() ) ){
      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          const Object * child = dynamic_cast< const Object* >( children[ i ].second[ j ] );
          if( child != NULL ){
            if( *child == *object ){
              return !_invert;
            }
          }
        }
      }
      return _invert;
    }
    return false;
}

/**
 * exports the Feature_Object_Matches_Child class to an XML file
 */
void
Feature_Object_Matches_Child::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_matches_child" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Object_Matches_Child class from an XML file
 */
void
Feature_Object_Matches_Child::
from_xml( xmlNodePtr root ){
    _invert = false;
    if( root->type == XML_ELEMENT_NODE ){
        xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
        if( tmp != NULL ){
            string invert_string = ( char* )( tmp );
            _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
            xmlFree( tmp );
        }
    }
    return;
}

namespace h2sl {
    /**
     * Feature_Object_Matches_Child class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Object_Matches_Child& other ) {
	out << "class:\"Feature_Object_Matches_Child\" ";
        return out;
    }
    
}
