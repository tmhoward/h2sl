/**
 * @file feature_object_property_merge_object_property_spatial_relation.cc
 *
 * @brief
 *
 * Class used to describe a feature spatial_relation matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_object_property_merge_object_property_spatial_relation.h"
#include "h2sl/object_property.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Property_Merge_Object_Property_Spatial_Relation class constructor
 */
Feature_Object_Property_Merge_Object_Property_Spatial_Relation::
Feature_Object_Property_Merge_Object_Property_Spatial_Relation( const bool& invert ) : Feature( invert ) {
    
}

/**
 * Feature_Object_Property_Merge_Object_Property_Spatial_Relation class destructor
 */
Feature_Object_Property_Merge_Object_Property_Spatial_Relation::
~Feature_Object_Property_Merge_Object_Property_Spatial_Relation() {
    
}

/**
 * Feature_Object_Property_Merge_Object_Property_Spatial_Relation class copy constructor
 */
Feature_Object_Property_Merge_Object_Property_Spatial_Relation::
Feature_Object_Property_Merge_Object_Property_Spatial_Relation( const Feature_Object_Property_Merge_Object_Property_Spatial_Relation& other ) : Feature( other ) {
    
}

/**
 * Feature_Object_Property_Merge_Object_Property_Spatial_Relation class assignment operator
 */
Feature_Object_Property_Merge_Object_Property_Spatial_Relation&
Feature_Object_Property_Merge_Object_Property_Spatial_Relation::
operator=( const Feature_Object_Property_Merge_Object_Property_Spatial_Relation& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Object_Property_Merge_Object_Property_Spatial_Relation::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Object_Property_Merge_Object_Property_Spatial_Relation::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    const Object_Property * object_property = dynamic_cast< const Object_Property* >( grounding );
    if( ( object_property != NULL ) && ( !children.empty() ) ){
      pair< const h2sl::Phrase*, const Object_Property* > object_property_child( NULL, NULL );
      pair< const h2sl::Phrase*, const Spatial_Relation* > spatial_relation_child( NULL, NULL );
      // enforce that children come from different phrases
      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if ( dynamic_cast< const Spatial_Relation* >( children[ i ].second[ j ] ) != NULL ){
            spatial_relation_child.first = children[ i ].first;
            spatial_relation_child.second = static_cast< const Spatial_Relation* >( children[ i ].second[ j ] );
          }
        }
      }
      for( unsigned int i = 0; i < children.size(); i++ ){
        if( children[ i ].first != spatial_relation_child.first ){
          for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
            if( dynamic_cast< const Object_Property* >( children[ i ].second[ j ] ) != NULL ){
              object_property_child.first = children[ i ].first;
              object_property_child.second = static_cast< const Object_Property* >( children[ i ].second[ j ] );
            }
          }
        }
      }


      if( ( object_property_child.first != NULL ) && ( object_property_child.second != NULL ) && ( spatial_relation_child.first != NULL ) && ( spatial_relation_child.second != NULL ) ){
        if( ( object_property_child.first->min_word_order() < spatial_relation_child.first->min_word_order() ) ){
          if( object_property->type() == object_property_child.second->type() ){
            if( object_property->index() == object_property_child.second->index() ){
              if( object_property->relation_type() == spatial_relation_child.second->spatial_relation_type() ){
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
 * exports the Feature_Object_Property_Merge_Object_Property_Spatial_Relation class to an XML file
 */
void
Feature_Object_Property_Merge_Object_Property_Spatial_Relation::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_property_merge_object_property_spatial_relation" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Object_Property_Merge_Object_Property_Spatial_Relation class from an XML file
 */
void
Feature_Object_Property_Merge_Object_Property_Spatial_Relation::
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
     * Feature_Object_Property_Merge_Object_Property_Spatial_Relation class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Object_Property_Merge_Object_Property_Spatial_Relation& other ) {
      out << "class:\"Feature_Object_Property_Merge_Object_Property_Spatial_Relation\" ";  
      return out;
    }
    
}
