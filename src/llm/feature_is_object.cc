/**
 * @file feature_is_object.cc
 *
 * @brief
 *
 * Class used to describe a feature object matches child type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/feature_is_object.h"
#include "h2sl/object.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Is_Object class constructor
 */
Feature_Is_Object::
Feature_Is_Object( const bool& invert ) : Feature( invert ) {
    
}

/**
 * Feature_Is_Object class destructor
 */
Feature_Is_Object::
~Feature_Is_Object() {
    
}

/**
 * Feature_Is_Object class copy constructor
 */
Feature_Is_Object::
Feature_Is_Object( const Feature_Is_Object& other ) : Feature( other ) {
    
}

/**
 * Feature_Is_Object class assignment operator
 */
Feature_Is_Object&
Feature_Is_Object::
operator=( const Feature_Is_Object& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Is_Object::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Is_Object::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    const Object * object = dynamic_cast< const Object* >( grounding );
    if( object != NULL ){
      return !_invert;
    } else {
      return _invert;
    }
}

/**
 * exports the Feature_Is_Object class to an XML file
 */
void
Feature_Is_Object::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_is_object" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Is_Object class from an XML file
 */
void
Feature_Is_Object::
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
     * Feature_Is_Object class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Is_Object& other ) {
	out << "class:\"Feature_Is_Object\" ";
        return out;
    }
    
}
