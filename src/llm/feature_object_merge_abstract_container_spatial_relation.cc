/**
 * @file feature_object_merge_abstract_container_spatial_relation.cc
 *
 * @brief
 *
 * Class used to describe a feature spatial_relation matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_object_merge_abstract_container_spatial_relation.h"
#include "h2sl/abstract_container.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Merge_Abstract_Container_Spatial_Relation class constructor
 */
Feature_Object_Merge_Abstract_Container_Spatial_Relation::
Feature_Object_Merge_Abstract_Container_Spatial_Relation( const bool& invert,
                                                          const string& spatialRelationType,
                                                          const string& sortingKey ) : Feature( invert ) {
  insert_prop< std::string >( _string_properties, "spatial_relation_type", spatialRelationType );
  insert_prop< std::string >( _string_properties, "sorting_key", sortingKey );    
}

Feature_Object_Merge_Abstract_Container_Spatial_Relation::
Feature_Object_Merge_Abstract_Container_Spatial_Relation( xmlNodePtr root ) : Feature() {
  insert_prop< std::string >( _string_properties, "spatial_relation_type", "na" );
  insert_prop< std::string >( _string_properties, "sorting_key", "na" );
  from_xml( root );
}

/**
 * Feature_Object_Merge_Abstract_Container_Spatial_Relation class destructor
 */
Feature_Object_Merge_Abstract_Container_Spatial_Relation::
~Feature_Object_Merge_Abstract_Container_Spatial_Relation() {
    
}

/**
 * Feature_Object_Merge_Abstract_Container_Spatial_Relation class copy constructor
 */
Feature_Object_Merge_Abstract_Container_Spatial_Relation::
Feature_Object_Merge_Abstract_Container_Spatial_Relation( const Feature_Object_Merge_Abstract_Container_Spatial_Relation& other ) : Feature( other ) {
    
}

/**
 * Feature_Object_Merge_Abstract_Container_Spatial_Relation class assignment operator
 */
Feature_Object_Merge_Abstract_Container_Spatial_Relation&
Feature_Object_Merge_Abstract_Container_Spatial_Relation::
operator=( const Feature_Object_Merge_Abstract_Container_Spatial_Relation& other ) {
    _invert = other._invert;
    _string_properties = other._string_properties;
    _int_properties = other._int_properties;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Object_Merge_Abstract_Container_Spatial_Relation::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Object_Merge_Abstract_Container_Spatial_Relation::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world,
      const Grounding* context ){
    const Object * object = dynamic_cast< const Object* >( grounding );
    if( ( object != NULL ) && ( !children.empty() ) ){
      pair< const Phrase*, const Abstract_Container* > abstract_container_child( NULL, NULL );
      pair< const Phrase*, const Spatial_Relation* > spatial_relation_child( NULL, NULL );
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
            if( dynamic_cast< const Abstract_Container* >( children[ i ].second[ j ] ) != NULL ){
              abstract_container_child.first = children[ i ].first;
              abstract_container_child.second = static_cast< const Abstract_Container* >( children[ i ].second[ j ] );
            }
          }
        }
      }

      if( ( abstract_container_child.first != NULL ) && ( abstract_container_child.second != NULL ) && ( spatial_relation_child.first != NULL ) && ( spatial_relation_child.second != NULL ) ){
        if( ( abstract_container_child.first->min_word_order() < spatial_relation_child.first->min_word_order() ) ){
          if( spatial_relation_child.second->spatial_relation_type() == spatial_relation_type() ){
            map< string, map< string, vector< Object* > > >::const_iterator it_sorted_objects_map = world->sorted_objects().find( sorting_key() );
            if( it_sorted_objects_map == world->sorted_objects().end() ){
              cout << "could not find sorting index \"" << sorting_key() << "\"" << endl;
              assert( false );
            }
            //assert( it_sorted_objects_map != world->sorted_objects().end() );
            map< string, vector< Object* > >::const_iterator it_sorted_objects = it_sorted_objects_map->second.find( abstract_container_child.second->type() );
            //assert( it_sorted_objects != it_sorted_objects_map->second.end() );
            if( it_sorted_objects != it_sorted_objects_map->second.end() ){
              for( unsigned int i = 0; i < abstract_container_child.second->number(); i++ ){
                if( object->id() == it_sorted_objects->second[ i ]->id() ){
                  return !_invert;
                }
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
 * exports the Feature_Object_Merge_Abstract_Container_Spatial_Relation class to an XML file
 */
void
Feature_Object_Merge_Abstract_Container_Spatial_Relation::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_merge_abstract_container_spatial_relation" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "spatial_relation_type" ), ( const xmlChar* )( spatial_relation_type().c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "sorting_key" ), ( const xmlChar* )( sorting_key().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Object_Merge_Abstract_Container_Spatial_Relation class from an XML file
 */
void
Feature_Object_Merge_Abstract_Container_Spatial_Relation::
from_xml( xmlNodePtr root ){
  _invert = false;
  spatial_relation_type() = "na";
  sorting_key() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > feature_keys = { "invert", "spatial_relation_type", "sorting_key" };
    assert( check_keys( root, feature_keys ) );

    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ) {
      _invert = invert_prop.second;
    }

    pair< bool, std::string > spatial_relation_type_prop = has_prop< std::string >( root, "spatial_relation_type" );
    if( spatial_relation_type_prop.first ) {
      spatial_relation_type() = spatial_relation_type_prop.second;
    }

    pair< bool, std::string > sorting_key_prop = has_prop< std::string >( root, "sorting_key" );
    if( sorting_key_prop.first ) {
      sorting_key() = sorting_key_prop.second;
    }
  }
  return;
}

namespace h2sl {
    /**
     * Feature_Object_Merge_Abstract_Container_Spatial_Relation class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Object_Merge_Abstract_Container_Spatial_Relation& other ) {
      out << "Feature_Object_Merge_Abstract_Container_Spatial_Relation:( invert:(" << other.invert() << ") spatial_relation_type:(" << other.spatial_relation_type() << ") sorting_key:(" << other.sorting_key() << "))";  
      return out;
    }
    
}
