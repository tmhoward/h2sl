/**
 * @file feature_container_is_empty.cc
 *
 * @brief
 *
 * Class used to describe a feature container matches child type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/feature_container_is_empty.h"
#include "h2sl/container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Is_Empty class constructor
 */
Feature_Container_Is_Empty::
Feature_Container_Is_Empty( const bool& invert ) : Feature( invert ) {
    
}

Feature_Container_Is_Empty::
Feature_Container_Is_Empty( xmlNodePtr root ) : Feature() {
  from_xml( root );
}

/**
 * Feature_Container_Is_Empty class destructor
 */
Feature_Container_Is_Empty::
~Feature_Container_Is_Empty() {
    
}

/**
 * Feature_Container_Is_Empty class copy constructor
 */
Feature_Container_Is_Empty::
Feature_Container_Is_Empty( const Feature_Container_Is_Empty& other ) : Feature( other ) {
    
}

/**
 * Feature_Container_Is_Empty class assignment operator
 */
Feature_Container_Is_Empty&
Feature_Container_Is_Empty::
operator=( const Feature_Container_Is_Empty& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Container_Is_Empty::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Container_Is_Empty::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    const Container * container = dynamic_cast< const Container* >( grounding );
    if( container != NULL ){
      if( container->container().empty() ){
        return !_invert;
      } else {
        return _invert;
      }
    }
    return false;
}

/**
 * exports the Feature_Container_Is_Empty class to an XML file
 */
void
Feature_Container_Is_Empty::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_is_empty" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Container_Is_Empty class from an XML file
 */
void
Feature_Container_Is_Empty::
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
     * Feature_Container_Is_Empty class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Container_Is_Empty& other ) {
      out << "class:\"Feature_Container_Is_Empty\" ";  
      return out;
    }
    
}
