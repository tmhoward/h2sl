/**
 * @file feature_region_container_container_matches_child_container.cc
 *
 * @brief
 *
 * Class used to describe a feature region_container container matches child type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/feature_region_container_container_matches_child_container.h"
#include "h2sl/region_container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Region_Container_Container_Matches_Child_Container class constructor
 */
Feature_Region_Container_Container_Matches_Child_Container::
Feature_Region_Container_Container_Matches_Child_Container( const bool& invert ) : Feature( invert ) {
    
}

/**
 * Feature_Region_Container_Container_Matches_Child_Container class destructor
 */
Feature_Region_Container_Container_Matches_Child_Container::
~Feature_Region_Container_Container_Matches_Child_Container() {
    
}

/**
 * Feature_Region_Container_Container_Matches_Child_Container class copy constructor
 */
Feature_Region_Container_Container_Matches_Child_Container::
Feature_Region_Container_Container_Matches_Child_Container( const Feature_Region_Container_Container_Matches_Child_Container& other ) : Feature( other ) {
    
}

/**
 * Feature_Region_Container_Container_Matches_Child_Container class assignment operator
 */
Feature_Region_Container_Container_Matches_Child_Container&
Feature_Region_Container_Container_Matches_Child_Container::
operator=( const Feature_Region_Container_Container_Matches_Child_Container& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Region_Container_Container_Matches_Child_Container::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Region_Container_Container_Matches_Child_Container::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    const Region_Container * region_container = dynamic_cast< const Region_Container* >( grounding );
    if( ( region_container != NULL ) && ( !children.empty() ) ){
      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          const Container * child = dynamic_cast< const Container* >( children[ i ].second[ j ] );
          if( child != NULL ){
            if( *child == region_container->container() ){
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
 * exports the Feature_Region_Container_Container_Matches_Child_Container class to an XML file
 */
void
Feature_Region_Container_Container_Matches_Child_Container::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_region_container_container_matches_child_container" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Region_Container_Container_Matches_Child_Container class from an XML file
 */
void
Feature_Region_Container_Container_Matches_Child_Container::
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
     * Feature_Region_Container_Container_Matches_Child_Container class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Region_Container_Container_Matches_Child_Container& other ) {
	out << "class:\"Feature_Region_Container_Container_Matches_Child_Container\" ";
        return out;
    }
    
}
