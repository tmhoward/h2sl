/**
 * @file feature_container_merge_object_property_container.cc
 *
 * @brief
 *
 * Class used to describe a feature container matches child type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/feature_container_merge_object_property_container.h"
#include "h2sl/container.h"
#include "h2sl/object_property.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Merge_Object_Property_Container class constructor
 */
Feature_Container_Merge_Object_Property_Container::
Feature_Container_Merge_Object_Property_Container( const bool& invert ) : Feature( invert ) {
    
}

/**
 * Feature_Container_Merge_Object_Property_Container class destructor
 */
Feature_Container_Merge_Object_Property_Container::
~Feature_Container_Merge_Object_Property_Container() {
    
}

/**
 * Feature_Container_Merge_Object_Property_Container class copy constructor
 */
Feature_Container_Merge_Object_Property_Container::
Feature_Container_Merge_Object_Property_Container( const Feature_Container_Merge_Object_Property_Container& other ) : Feature( other ) {
    
}

/**
 * Feature_Container_Merge_Object_Property_Container class assignment operator
 */
Feature_Container_Merge_Object_Property_Container&
Feature_Container_Merge_Object_Property_Container::
operator=( const Feature_Container_Merge_Object_Property_Container& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Container_Merge_Object_Property_Container::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Container_Merge_Object_Property_Container::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    const Container * container = dynamic_cast< const Container* >( grounding );
    if( ( container != NULL ) && ( !children.empty() ) ){
      pair< const h2sl::Phrase*, const Object_Property* > object_property_child( NULL, NULL );
      pair< const h2sl::Phrase*, const Container* > container_child( NULL, NULL );
      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if( dynamic_cast< const Object_Property* >( children[ i ].second[ j ] ) != NULL ){
            object_property_child.first = children[ i ].first;
            object_property_child.second = static_cast< const Object_Property* >( children[ i ].second[ j ] );
          } else if ( dynamic_cast< const Container* >( children[ i ].second[ j ] ) != NULL ){
            container_child.first = children[ i ].first;
            container_child.second = static_cast< const Container* >( children[ i ].second[ j ] );
          }
        }
      }

      if( ( object_property_child.first != NULL ) && ( object_property_child.second != NULL ) && ( container_child.first != NULL ) && ( container_child.second != NULL ) ){ 
        if( object_property_child.first->min_word_order() < container_child.first->min_word_order() ){
          return !_invert;
        }
        return _invert;
      }
    }
    return false;
}

/**
 * exports the Feature_Container_Merge_Object_Property_Container class to an XML file
 */
void
Feature_Container_Merge_Object_Property_Container::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_merge_object_property_container" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Container_Merge_Object_Property_Container class from an XML file
 */
void
Feature_Container_Merge_Object_Property_Container::
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
     * Feature_Container_Merge_Object_Property_Container class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Container_Merge_Object_Property_Container& other ) {
      out << "class:\"Feature_Container_Merge_Object_Property_Container\" ";  
      return out;
    }
    
}
