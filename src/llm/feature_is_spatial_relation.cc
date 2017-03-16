/**
 * @file feature_is_spatial_relation.cc
 *
 * @brief
 *
 * Class used to describe a feature object matches child type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/feature_is_spatial_relation.h"
#include "h2sl/spatial_relation.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Is_Spatial_Relation class constructor
 */
Feature_Is_Spatial_Relation::
Feature_Is_Spatial_Relation( const bool& invert ) : Feature( invert ) {
    
}

/**
 * Feature_Is_Spatial_Relation class destructor
 */
Feature_Is_Spatial_Relation::
~Feature_Is_Spatial_Relation() {
    
}

/**
 * Feature_Is_Spatial_Relation class copy constructor
 */
Feature_Is_Spatial_Relation::
Feature_Is_Spatial_Relation( const Feature_Is_Spatial_Relation& other ) : Feature( other ) {
    
}

/**
 * Feature_Is_Spatial_Relation class assignment operator
 */
Feature_Is_Spatial_Relation&
Feature_Is_Spatial_Relation::
operator=( const Feature_Is_Spatial_Relation& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Is_Spatial_Relation::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Is_Spatial_Relation::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    const Spatial_Relation * spatial_relation = dynamic_cast< const Spatial_Relation* >( grounding );
    if( spatial_relation != NULL ){
      return !_invert;
    } else {
      return _invert;
    }
}

/**
 * exports the Feature_Is_Spatial_Relation class to an XML file
 */
void
Feature_Is_Spatial_Relation::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_is_spatial_relation" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Is_Spatial_Relation class from an XML file
 */
void
Feature_Is_Spatial_Relation::
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
     * Feature_Is_Spatial_Relation class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Is_Spatial_Relation& other ) {
	out << "class:\"Feature_Is_Spatial_Relation\" ";
        return out;
    }
    
}
