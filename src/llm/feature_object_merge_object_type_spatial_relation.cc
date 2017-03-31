/**
 * @file feature_object_merge_object_type_spatial_relation.cc
 *
 * @brief
 *
 * Class used to describe a feature spatial_relation matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_object_merge_object_type_spatial_relation.h"
//#include "h2sl/object_type.h"
#include "h2sl/object_type.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Merge_Object_Type_Spatial_Relation class constructor
 */
Feature_Object_Merge_Object_Type_Spatial_Relation::
Feature_Object_Merge_Object_Type_Spatial_Relation( const bool& invert,
                                                    const string& sortingKey,
                                                    const string& spatialRelationType ) : Feature( invert ) {
  insert_prop< std::string >( _string_properties, "sorting_key", sortingKey );
  insert_prop< std::string >( _string_properties, "spatial_relation_type", spatialRelationType );
}

Feature_Object_Merge_Object_Type_Spatial_Relation::
Feature_Object_Merge_Object_Type_Spatial_Relation( xmlNodePtr root ) : Feature() {
  insert_prop< std::string >( _string_properties, "sorting_key", "na" );
  insert_prop< std::string >( _string_properties, "spatial_relation_type", "na" );
  from_xml( root );
}

/**
 * Feature_Object_Merge_Object_Type_Spatial_Relation class destructor
 */
Feature_Object_Merge_Object_Type_Spatial_Relation::
~Feature_Object_Merge_Object_Type_Spatial_Relation() {
    
}

/**
 * Feature_Object_Merge_Object_Type_Spatial_Relation class copy constructor
 */
Feature_Object_Merge_Object_Type_Spatial_Relation::
Feature_Object_Merge_Object_Type_Spatial_Relation( const Feature_Object_Merge_Object_Type_Spatial_Relation& other ) : Feature( other ) {
    
}

/**
 * Feature_Object_Merge_Object_Type_Spatial_Relation class assignment operator
 */
Feature_Object_Merge_Object_Type_Spatial_Relation&
Feature_Object_Merge_Object_Type_Spatial_Relation::
operator=( const Feature_Object_Merge_Object_Type_Spatial_Relation& other ) {
    _invert = other._invert;
    _string_properties = other._string_properties;
    _int_properties = other._int_properties;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Object_Merge_Object_Type_Spatial_Relation::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Object_Merge_Object_Type_Spatial_Relation::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world,
      const Grounding* context){
  const Object * object = dynamic_cast< const Object* >( grounding );
  if( ( object != NULL ) && ( !children.empty() ) ){
    pair< const Phrase*, const Object_Type* > object_type_child( NULL, NULL );
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
          if( dynamic_cast< const Object_Type* >( children[ i ].second[ j ] ) != NULL ){
            object_type_child.first = children[ i ].first;
            object_type_child.second = static_cast< const Object_Type* >( children[ i ].second[ j ] );
          }
        }
      }
    }

    if( ( object_type_child.first != NULL ) && ( object_type_child.second != NULL ) && ( spatial_relation_child.first != NULL ) && ( spatial_relation_child.second != NULL ) ){
      if( ( object_type_child.first->min_word_order() < spatial_relation_child.first->min_word_order() ) && ( spatial_relation_child.second->spatial_relation_type() == spatial_relation_type() ) ){
        map< string, map< string, vector< Object* > > >::const_iterator it_sorted_objects_map = world->sorted_objects().find( sorting_key() );
        if( it_sorted_objects_map == world->sorted_objects().end() ){
          cout << "could not find sorting index \"" << sorting_key() << "\"" << endl;
          assert( false );
        }
        //assert( it_sorted_objects_map != world->sorted_objects().end() );
        map< string, vector< Object* > >::const_iterator it_sorted_objects = it_sorted_objects_map->second.find( object_type_child.second->type() );
        //assert( it_sorted_objects != it_sorted_objects_map->second.end() );
        if( it_sorted_objects != it_sorted_objects_map->second.end() ){
          if( !it_sorted_objects->second.empty() ){
            if( object->id() == it_sorted_objects->second.front()->id() ){
              return !_invert; 
            } else {
              return _invert;
            }
          }
        }
      }
    }
  }


  return false;
}

/**
 * exports the Feature_Object_Merge_Object_Type_Spatial_Relation class to an XML file
 */
void
Feature_Object_Merge_Object_Type_Spatial_Relation::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_merge_object_type_spatial_relation" ), NULL );
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( to_std_string( _invert ).c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "sorting_key" ), ( const xmlChar* )( sorting_key().c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "spatial_relation_type" ), ( const xmlChar* )( spatial_relation_type().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Object_Merge_Object_Type_Spatial_Relation class from an XML file
 */
void
Feature_Object_Merge_Object_Type_Spatial_Relation::
from_xml( xmlNodePtr root ){
  _invert = false;
  sorting_key() = "na";
  spatial_relation_type() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > feature_keys = { "invert", "sorting_key", "spatial_relation_type" };
    assert( check_keys( root, feature_keys ) );

    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ) {
      _invert = invert_prop.second;
    }

    pair< bool, std::string > sorting_key_prop = has_prop< std::string >( root, "sorting_key" );
    if( sorting_key_prop.first ) {
      sorting_key() = sorting_key_prop.second;
    }

    pair< bool, std::string > spatial_relation_type_prop = has_prop< std::string >( root, "spatial_relation_type" );
    if( spatial_relation_type_prop.first ) {
      spatial_relation_type() = spatial_relation_type_prop.second;
    }
  }
  return;
}

namespace h2sl {
    /**
     * Feature_Object_Merge_Object_Type_Spatial_Relation class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Object_Merge_Object_Type_Spatial_Relation& other ) {
        out << "Feature_Object_Merge_Object_Type_Spatial_Relation:(invert:(" << other.invert() << ") sorting_key:(" << other.sorting_key() << ") spatial_relation_type:(" << other.spatial_relation_type() << "))";
        return out;
    }
    
}
