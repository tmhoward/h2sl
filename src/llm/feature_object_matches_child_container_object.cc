/**
 * @file feature_object_matches_child_container_object.cc
 *
 * @brief
 *
 * Class used to describe a feature object matches child_container_object type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/feature_object_matches_child_container_object.h"
#include "h2sl/object.h"
#include "h2sl/container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Matches_Child_Container_Object class constructor
 */
Feature_Object_Matches_Child_Container_Object::
Feature_Object_Matches_Child_Container_Object( const bool& invert ) : Feature( invert ) {
    
}

Feature_Object_Matches_Child_Container_Object::
Feature_Object_Matches_Child_Container_Object( xmlNodePtr root ) : Feature() {
  from_xml( root );
}

/**
 * Feature_Object_Matches_Child_Container_Object class destructor
 */
Feature_Object_Matches_Child_Container_Object::
~Feature_Object_Matches_Child_Container_Object() {
    
}

/**
 * Feature_Object_Matches_Child_Container_Object class copy constructor
 */
Feature_Object_Matches_Child_Container_Object::
Feature_Object_Matches_Child_Container_Object( const Feature_Object_Matches_Child_Container_Object& other ) : Feature( other ) {
    
}

/**
 * Feature_Object_Matches_Child_Container_Object class assignment operator
 */
Feature_Object_Matches_Child_Container_Object&
Feature_Object_Matches_Child_Container_Object::
operator=( const Feature_Object_Matches_Child_Container_Object& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature
 */
bool
Feature_Object_Matches_Child_Container_Object::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world) {
    return value( cv, grounding, children, phrase, world, NULL );
    
}

/**
 * returns the value of the feature.
 */
bool
Feature_Object_Matches_Child_Container_Object::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world,
      const Grounding* context ){
    const Object * object = dynamic_cast< const Object* >( grounding );
    
    if( object != NULL ){
      bool found_container = false;
      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          const Container* container_child = dynamic_cast< Container* >( children[ i ].second[ j ] );
          if( container_child != NULL ){
            found_container = true;
            for( unsigned int k = 0; k < container_child->container().size(); k++ ){
              if( dynamic_cast< Object* >( container_child->container()[ k ] ) != NULL ) {
                if( *object == *static_cast< Object* >( container_child->container()[ k ] ) ) { 
                  return !_invert;
                }
              }
            }
          }
        }
      }
      if( found_container ){
        return _invert;
      }
    }
    return false;
}

/**
 * exports the Feature_Object_Matches_Child_Container_Object class to an XML file
 */
void
Feature_Object_Matches_Child_Container_Object::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_matches_child_container_object" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Object_Matches_Child_Container_Object class from an XML file
 */
void
Feature_Object_Matches_Child_Container_Object::
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
     * Feature_Object_Matches_Child_Container_Object class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Object_Matches_Child_Container_Object& other ) {
	out << "class:\"Feature_Object_Matches_Child_Container_Object\" ";
        return out;
    }
}
