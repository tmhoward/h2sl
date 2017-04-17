/**
 * @file feature_object_merge_region_abstract_container_container.cc
 *
 * @brief
 *
 * Class used to describe a feature container matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_object_merge_region_abstract_container_container.h"
#include "h2sl/region_abstract_container.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/container.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Merge_Region_Abstract_Container_Container class constructor
 */
Feature_Object_Merge_Region_Abstract_Container_Container::
Feature_Object_Merge_Region_Abstract_Container_Container( const bool& invert,
                                                const bool& objectMatch,
                                                const string& sortingKey,
                                                const string& spatialRelationType ) : Feature( invert ),
                                                                                      _object_match( objectMatch ) {
  insert_prop< std::string >( _string_properties, "sorting_key", sortingKey );    
  insert_prop< std::string >( _string_properties, "spatial_relation_type", spatialRelationType );    
}

Feature_Object_Merge_Region_Abstract_Container_Container::
Feature_Object_Merge_Region_Abstract_Container_Container( xmlNodePtr root ) : Feature(),
                                                                              _object_match( false ) {
  insert_prop< std::string >( _string_properties, "sorting_key", "na" );
  insert_prop< std::string >( _string_properties, "spatial_relation_type", "na" );
  from_xml( root );
}

/**
 * Feature_Object_Merge_Region_Abstract_Container_Container class destructor
 */
Feature_Object_Merge_Region_Abstract_Container_Container::
~Feature_Object_Merge_Region_Abstract_Container_Container() {
    
}

/**
 * Feature_Object_Merge_Region_Abstract_Container_Container class copy constructor
 */
Feature_Object_Merge_Region_Abstract_Container_Container::
Feature_Object_Merge_Region_Abstract_Container_Container( const Feature_Object_Merge_Region_Abstract_Container_Container& other ) : Feature( other ),
                              _object_match( false ) {
    
}

/**
 * Feature_Object_Merge_Region_Abstract_Container_Container class assignment operator
 */
Feature_Object_Merge_Region_Abstract_Container_Container&
Feature_Object_Merge_Region_Abstract_Container_Container::
operator=( const Feature_Object_Merge_Region_Abstract_Container_Container& other ) {
    _invert = other._invert;
    _object_match = other._object_match;
    _string_properties = other._string_properties;
    _int_properties = other._int_properties;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Object_Merge_Region_Abstract_Container_Container::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Object_Merge_Region_Abstract_Container_Container::
value( const string& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context ){
  
  const Object * object = dynamic_cast< const Object* >( grounding );
  if( ( object != NULL ) && ( !children.empty() ) ){
    pair< const Phrase*, const Region_Abstract_Container* > region_abstract_container_child( NULL, NULL );
    pair< const Phrase*, const Container* > container_child( NULL, NULL );
    // enforce that children come from different phrases
    for( unsigned int i = 0; i < children.size(); i++ ){
      for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
        if ( dynamic_cast< const Container* >( children[ i ].second[ j ] ) != NULL ){
          container_child.first = children[ i ].first;
          container_child.second = static_cast< const Container* >( children[ i ].second[ j ] );
        }
      }
    }
    if( container_child.second == NULL ){
      return false;
    }
    for( unsigned int i = 0; i < children.size(); i++ ){
      if( children[ i ].first != container_child.first ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if( dynamic_cast< const Region_Abstract_Container* >( children[ i ].second[ j ] ) != NULL ){
            region_abstract_container_child.first = children[ i ].first;
            region_abstract_container_child.second = static_cast< const Region_Abstract_Container* >( children[ i ].second[ j ] );
          }
        }
      }
    }
 
    if( ( region_abstract_container_child.first != NULL ) && ( region_abstract_container_child.second != NULL ) && ( container_child.first != NULL ) && ( container_child.second != NULL ) ){
      if( ( region_abstract_container_child.first->min_word_order() < container_child.first->min_word_order() ) && ( region_abstract_container_child.second->index() != 0 ) && ( region_abstract_container_child.second->spatial_relation_type() == spatial_relation_type() ) && (object->type() == region_abstract_container_child.second->type() ) ){
        vector< Object* > sorted_objects;
        for( unsigned int i = 0; i < container_child.second->container().size(); i++ ){
          Object * container_child_object = dynamic_cast< Object* >( container_child.second->container()[ i ] );
          if( container_child_object != NULL ){
            if( container_child_object->type() == region_abstract_container_child.second->type() ){
              sorted_objects.push_back( container_child_object );
            }
          }
        }

        // check container objects
        if( _object_match ){
          if( !sorted_objects.empty() ){
            if( region_abstract_container_child.second->index() <= ( int )( sorted_objects.size() ) ){
              if( sorting_key() == "min_x" ){
                sort( sorted_objects.begin(), sorted_objects.end(), World::min_x_sort );
              } else if ( sorting_key() == "max_x" ){
                sort( sorted_objects.begin(), sorted_objects.end(), World::max_x_sort );
              } else if ( sorting_key() == "min_y" ){
                sort( sorted_objects.begin(), sorted_objects.end(), World::min_y_sort );
              } else if ( sorting_key() == "max_y" ){
                sort( sorted_objects.begin(), sorted_objects.end(), World::max_y_sort );
              } else if ( sorting_key() == "min_z" ){
                sort( sorted_objects.begin(), sorted_objects.end(), World::min_z_sort );
              } else if ( sorting_key() == "max_z" ){
                sort( sorted_objects.begin(), sorted_objects.end(), World::max_z_sort );
              } else if ( sorting_key() == "min_abs_x" ){
                sort( sorted_objects.begin(), sorted_objects.end(), World::min_abs_x_sort );
              } else if ( sorting_key() == "max_abs_x" ){
                sort( sorted_objects.begin(), sorted_objects.end(), World::max_abs_x_sort );
              } else if ( sorting_key() == "min_abs_y" ){
                sort( sorted_objects.begin(), sorted_objects.end(), World::min_abs_y_sort );
              } else if ( sorting_key() == "max_abs_y" ){
                sort( sorted_objects.begin(), sorted_objects.end(), World::max_abs_y_sort );
              } else if ( sorting_key() == "min_distance" ){
                sort( sorted_objects.begin(), sorted_objects.end(), World::min_distance_sort );
              } else if ( sorting_key() == "max_distance" ){
                sort( sorted_objects.begin(), sorted_objects.end(), World::max_distance_sort );
              } else if ( sorting_key() == "min_center_distance" ){
                World::min_center_distance_sort_objects( sorted_objects );
              } else if ( sorting_key() == "max_center_distance" ){
                World::max_center_distance_sort_objects( sorted_objects );
              } 

              for( int i = 0; i < region_abstract_container_child.second->number(); i++ ){
                if( object->id() == sorted_objects[ i ]->id() ){
                  return !_invert;
                }
              }
              return _invert;
            }
          }
        } else {
          // check world objects
          map< string, map< string, vector< Object* > > >::const_iterator it_sorted_objects_map = world->sorted_objects().find( sorting_key() );
          if( it_sorted_objects_map == world->sorted_objects().end() ){
            cout << "could not find sorting index \"" << sorting_key() << "\"" << endl;
          }
          assert( it_sorted_objects_map != world->sorted_objects().end() );
          map< string, vector< Object* > >::const_iterator it_sorted_objects = it_sorted_objects_map->second.find( object->type() );
          assert( it_sorted_objects != it_sorted_objects_map->second.end() );
  
          if( (  region_abstract_container_child.second->number() < ( int )( it_sorted_objects->second.size() ) ) && ( it_sorted_objects->second.size() > 1 ) ){
            for( int i = 0; i < region_abstract_container_child.second->number(); i++ ){
              if( object->id() == it_sorted_objects->second[ i ]->id() ){
                return !_invert;
              }
            }
            return _invert;
          }
        }
      }
    }
  }

  return false;  
}

/**
 * exports the Feature_Object_Merge_Region_Abstract_Container_Container class to an XML file
 */
void
Feature_Object_Merge_Region_Abstract_Container_Container::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_merge_region_abstract_container_container" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( to_std_string( _invert ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "object_match" ), ( const xmlChar* )( to_std_string( _object_match ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "sorting_key" ), ( const xmlChar* )( sorting_key().c_str() ) ); 
  xmlNewProp( node, ( const xmlChar* )( "spatial_relation_type" ), ( const xmlChar* )( spatial_relation_type().c_str() ) ); 
  xmlAddChild( root, node );
  return;
}

/**
 * imports the Feature_Object_Merge_Region_Abstract_Container_Container class from an XML file
 */
void
Feature_Object_Merge_Region_Abstract_Container_Container::
from_xml( xmlNodePtr root ){
  _invert = false;
  _object_match = false;
  sorting_key() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > feature_keys = { "invert", "object_match", "sorting_key", "spatial_relation_type" };
    assert( check_keys( root, feature_keys ) );

    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ) {
      _invert = invert_prop.second;
    }

    pair< bool, bool > object_match_prop = has_prop< bool >( root, "object_match" );
    if( object_match_prop.first ) {
      _object_match = object_match_prop.second;
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
     * Feature_Object_Merge_Region_Abstract_Container_Container class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Object_Merge_Region_Abstract_Container_Container& other ) {
      out << "Feature_Object_Merge_Region_Abstract_Container_Container:(invert:(" << other.invert() << ") object_match:(" << other.object_match() << ") sorting_key:(" << other.sorting_key() << ") spatial_relation_type:(" << other.spatial_relation_type() << "))"; 
      return out;
    }
    
}
