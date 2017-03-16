/**
 * @file feature_is_region.cc
 *
 * @brief
 *
 * Class used to describe a feature object matches child type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/feature_is_region.h"
#include "h2sl/region.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Is_Region class constructor
 */
Feature_Is_Region::
Feature_Is_Region( const bool& invert ) : Feature( invert ) {
    
}

/**
 * Feature_Is_Region class destructor
 */
Feature_Is_Region::
~Feature_Is_Region() {
    
}

/**
 * Feature_Is_Region class copy constructor
 */
Feature_Is_Region::
Feature_Is_Region( const Feature_Is_Region& other ) : Feature( other ) {
    
}

/**
 * Feature_Is_Region class assignment operator
 */
Feature_Is_Region&
Feature_Is_Region::
operator=( const Feature_Is_Region& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Is_Region::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Is_Region::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    const Region * region = dynamic_cast< const Region* >( grounding );
    if( region != NULL ){
      return !_invert;
    } else {
      return _invert;
    }
}

/**
 * exports the Feature_Is_Region class to an XML file
 */
void
Feature_Is_Region::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_is_region" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Is_Region class from an XML file
 */
void
Feature_Is_Region::
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
     * Feature_Is_Region class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Is_Region& other ) {
	out << "class:\"Feature_Is_Region\" ";
        return out;
    }
    
}
