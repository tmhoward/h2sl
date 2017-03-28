/**
 * @file feature_object_merge_object_property_container.cc
 *
 * @brief
 *
 * Class used to describe a feature container matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_object_merge_object_property_container.h"
#include "h2sl/object_property.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/container.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Merge_Object_Property_Container class constructor
 */
Feature_Object_Merge_Object_Property_Container::
Feature_Object_Merge_Object_Property_Container( const bool& invert,
                                                const string& sortingKey,
                                                const string& spatialRelationType ) : Feature( invert ) {
  insert_prop< std::string >( _string_properties, "sorting_key", sortingKey );    
  insert_prop< std::string >( _string_properties, "spatial_relation_type", spatialRelationType );    
}

Feature_Object_Merge_Object_Property_Container::
Feature_Object_Merge_Object_Property_Container( xmlNodePtr root ) : Feature() {
  insert_prop< std::string >( _string_properties, "sorting_key", "na" );
  insert_prop< std::string >( _string_properties, "spatial_relation_type", "na" );
  from_xml( root );
}

/**
 * Feature_Object_Merge_Object_Property_Container class destructor
 */
Feature_Object_Merge_Object_Property_Container::
~Feature_Object_Merge_Object_Property_Container() {
    
}

/**
 * Feature_Object_Merge_Object_Property_Container class copy constructor
 */
Feature_Object_Merge_Object_Property_Container::
Feature_Object_Merge_Object_Property_Container( const Feature_Object_Merge_Object_Property_Container& other ) : Feature( other ) {
    
}

/**
 * Feature_Object_Merge_Object_Property_Container class assignment operator
 */
Feature_Object_Merge_Object_Property_Container&
Feature_Object_Merge_Object_Property_Container::
operator=( const Feature_Object_Merge_Object_Property_Container& other ) {
    _invert = other._invert;
    _string_properties = other._string_properties;
    _int_properties = other._int_properties;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Object_Merge_Object_Property_Container::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Object_Merge_Object_Property_Container::
value( const string& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context ){
  
  const Object * object = dynamic_cast< const Object* >( grounding );
  if( ( object != NULL ) && ( !children.empty() ) ){
    pair< const Phrase*, const Object_Property* > object_property_child( NULL, NULL );
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
    for( unsigned int i = 0; i < children.size(); i++ ){
      if( children[ i ].first != container_child.first ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if( dynamic_cast< const Object_Property* >( children[ i ].second[ j ] ) != NULL ){
            object_property_child.first = children[ i ].first;
            object_property_child.second = static_cast< const Object_Property* >( children[ i ].second[ j ] );
          }
        }
      }
    }
 
    if( ( object_property_child.first != NULL ) && ( object_property_child.second != NULL ) && ( container_child.first != NULL ) && ( container_child.second != NULL ) ){
      if( ( object_property_child.first->min_word_order() < container_child.first->min_word_order() ) && ( object_property_child.second->index() != 0 ) && ( object_property_child.second->relation_type() == spatial_relation_type() ) && (object->type() == object_property_child.second->type() ) ){
        vector< Object* > sorted_objects;
        for( unsigned int i = 0; i < container_child.second->container().size(); i++ ){
          Object * container_child_object = dynamic_cast< Object* >( container_child.second->container()[ i ] );
          if( container_child_object != NULL ){
            if( container_child_object->type() == object_property_child.second->type() ){
              sorted_objects.push_back( container_child_object );
            }
          }
        }

        if( !sorted_objects.empty() ){
          if( object_property_child.second->index() <= ( int )( sorted_objects.size() ) ){
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
/*
            cout << "object:" << *object << endl;
            cout << "spatial_relation_type():" << spatial_relation_type() << endl;
            cout << "sorting_key():" << sorting_key() << endl;
            cout << "sorted_objects" << sorted_objects << endl;
            cout << "index:" << object_property_child.second->index() << endl;
*/    
            if( object->id() == sorted_objects[ object_property_child.second->index() - 1 ]->id() ){
//              cout << "  FOUND MATCH" << endl;
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
 * exports the Feature_Object_Merge_Object_Property_Container class to an XML file
 */
void
Feature_Object_Merge_Object_Property_Container::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_merge_object_property_container" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( to_std_string( _invert ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "sorting_key" ), ( const xmlChar* )( sorting_key().c_str() ) ); 
  xmlNewProp( node, ( const xmlChar* )( "spatial_relation_type" ), ( const xmlChar* )( spatial_relation_type().c_str() ) ); 
  xmlAddChild( root, node );
  return;
}

/**
 * imports the Feature_Object_Merge_Object_Property_Container class from an XML file
 */
void
Feature_Object_Merge_Object_Property_Container::
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
     * Feature_Object_Merge_Object_Property_Container class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Object_Merge_Object_Property_Container& other ) {
      out << "Feature_Object_Merge_Object_Property_Container:(invert:(" << other.invert() << ") sorting_key:(" << other.sorting_key() << ") spatial_relation_type:(" << other.spatial_relation_type() << "))"; 
      return out;
    }
    
}
