/**
 * @file feature_object_property_merge_index_object_property.cc
 *
 * @brief
 *
 * Class used to describe a feature spatial_relation matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_object_property_merge_index_object_property.h"
#include "h2sl/object_property.h"
#include "h2sl/index.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Property_Merge_Index_Object_Property class constructor
 */
Feature_Object_Property_Merge_Index_Object_Property::
Feature_Object_Property_Merge_Index_Object_Property( const bool& invert ) : Feature( invert ) {
    
}

/**
 * Feature_Object_Property_Merge_Index_Object_Property class destructor
 */
Feature_Object_Property_Merge_Index_Object_Property::
~Feature_Object_Property_Merge_Index_Object_Property() {
    
}

/**
 * Feature_Object_Property_Merge_Index_Object_Property class copy constructor
 */
Feature_Object_Property_Merge_Index_Object_Property::
Feature_Object_Property_Merge_Index_Object_Property( const Feature_Object_Property_Merge_Index_Object_Property& other ) : Feature( other ) {
    
}

/**
 * Feature_Object_Property_Merge_Index_Object_Property class assignment operator
 */
Feature_Object_Property_Merge_Index_Object_Property&
Feature_Object_Property_Merge_Index_Object_Property::
operator=( const Feature_Object_Property_Merge_Index_Object_Property& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Object_Property_Merge_Index_Object_Property::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Object_Property_Merge_Index_Object_Property::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    const Object_Property * object_property = dynamic_cast< const Object_Property* >( grounding );
    if( ( object_property != NULL ) && ( !children.empty() ) ){
      pair< const h2sl::Phrase*, const Index* > index_child( NULL, NULL );
      pair< const h2sl::Phrase*, const Object_Property* > object_property_child( NULL, NULL );
      // enforce that children come from different phrases
      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if ( dynamic_cast< const Index* >( children[ i ].second[ j ] ) != NULL ){
            index_child.first = children[ i ].first;
            index_child.second = static_cast< const Index* >( children[ i ].second[ j ] );
          }
        }
      }
      for( unsigned int i = 0; i < children.size(); i++ ){
        if( children[ i ].first != index_child.first ){
          for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
            if( dynamic_cast< const Object_Property* >( children[ i ].second[ j ] ) != NULL ){
              object_property_child.first = children[ i ].first;
              object_property_child.second = static_cast< const Object_Property* >( children[ i ].second[ j ] );
            }
          }
        }
      }


      if( ( object_property_child.first != NULL ) && ( object_property_child.second != NULL ) && ( index_child.first != NULL ) && ( index_child.second != NULL ) ){
        if( ( index_child.first->min_word_order() < object_property_child.first->min_word_order() ) ){
          if( object_property->type() == object_property_child.second->type() ){
            if( object_property->index() == index_child.second->value() ){
              if( object_property->relation_type() == object_property_child.second->relation_type() ){
                return !_invert;
              }
            }
          }
          return _invert;
        }
      }
    }
    return false;
}

/**
 * exports the Feature_Object_Property_Merge_Index_Object_Property class to an XML file
 */
void
Feature_Object_Property_Merge_Index_Object_Property::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_property_merge_index_object_property" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Object_Property_Merge_Index_Object_Property class from an XML file
 */
void
Feature_Object_Property_Merge_Index_Object_Property::
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
     * Feature_Object_Property_Merge_Index_Object_Property class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Object_Property_Merge_Index_Object_Property& other ) {
      out << "class:\"Feature_Object_Property_Merge_Index_Object_Property\" ";  
      return out;
    }
    
}
