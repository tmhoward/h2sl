/**
 * @file feature_object_merge_object_property_region_container.cc
 *
 * @brief
 *
 * Class used to describe a feature container matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_object_merge_object_property_region_container.h"
#include "h2sl/object_property.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/region_container.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Merge_Object_Property_Region_Container class constructor
 */
Feature_Object_Merge_Object_Property_Region_Container::
Feature_Object_Merge_Object_Property_Region_Container( const bool& invert,
                                                const string& sortingKey ) : Feature( invert ) {
  insert_prop< std::string >( _string_properties, "sorting_key", sortingKey );    
}

Feature_Object_Merge_Object_Property_Region_Container::
Feature_Object_Merge_Object_Property_Region_Container( xmlNodePtr root ) : Feature() {
  insert_prop< std::string >( _string_properties, "sorting_key", "na" );
  from_xml( root );
}

/**
 * Feature_Object_Merge_Object_Property_Region_Container class destructor
 */
Feature_Object_Merge_Object_Property_Region_Container::
~Feature_Object_Merge_Object_Property_Region_Container() {
    
}

/**
 * Feature_Object_Merge_Object_Property_Region_Container class copy constructor
 */
Feature_Object_Merge_Object_Property_Region_Container::
Feature_Object_Merge_Object_Property_Region_Container( const Feature_Object_Merge_Object_Property_Region_Container& other ) : Feature( other ) {
    
}

/**
 * Feature_Object_Merge_Object_Property_Region_Container class assignment operator
 */
Feature_Object_Merge_Object_Property_Region_Container&
Feature_Object_Merge_Object_Property_Region_Container::
operator=( const Feature_Object_Merge_Object_Property_Region_Container& other ) {
    _invert = other._invert;
    _string_properties = other._string_properties;
    _int_properties = other._int_properties;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Object_Merge_Object_Property_Region_Container::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Object_Merge_Object_Property_Region_Container::
value( const string& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context ){
  
  const Object * object = dynamic_cast< const Object* >( grounding );
  if( ( object != NULL ) && ( !children.empty() ) ){
    pair< const Phrase*, const Object_Property* > object_property_child( NULL, NULL );
    pair< const Phrase*, const Region_Container* > region_container_child( NULL, NULL );
    // enforce that children come from different phrases
    for( unsigned int i = 0; i < children.size(); i++ ){
      for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
        if ( dynamic_cast< const Region_Container* >( children[ i ].second[ j ] ) != NULL ){
          region_container_child.first = children[ i ].first;
          region_container_child.second = static_cast< const Region_Container* >( children[ i ].second[ j ] );
        }
      }
    }
    if( region_container_child.second == NULL ){
      return false;
    }
    for( unsigned int i = 0; i < children.size(); i++ ){
      if( children[ i ].first != region_container_child.first ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if( dynamic_cast< const Object_Property* >( children[ i ].second[ j ] ) != NULL ){
            object_property_child.first = children[ i ].first;
            object_property_child.second = static_cast< const Object_Property* >( children[ i ].second[ j ] );
          }
        }
      }
    }
  
    if( ( object_property_child.first != NULL ) && ( object_property_child.second != NULL ) && ( region_container_child.first != NULL ) && ( region_container_child.second != NULL ) ){
      if( ( object_property_child.first->min_word_order() < region_container_child.first->min_word_order() ) && ( object_property_child.second->index() != 0 ) ){
        vector< Object* > sorted_objects;
        for( unsigned int i = 0; i < region_container_child.second->container().container().size(); i++ ){
          Object * region_container_child_object = dynamic_cast< Object* >( region_container_child.second->container().container()[ i ] );
          if( region_container_child_object != NULL ){
            if( region_container_child_object->type() == object_property_child.second->type() ){
              sorted_objects.push_back( region_container_child_object );
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

            if( object->id() == sorted_objects[ object_property_child.second->index() - 1 ]->id() ){
              return !_invert;
            } else {
              return _invert;
            } 
          }
/*
        if( spatial_relation_child.second->spatial_relation_type() == spatial_relation_type() ){
          map< string, map< string, vector< Object* > > >::const_iterator it_sorted_objects_map = world->sorted_objects().find( sorting_key() );
          if( it_sorted_objects_map == world->sorted_objects().end() ){
            cout << "could not find sorting index \"" << sorting_key() << "\"" << endl;
          }
          assert( it_sorted_objects_map != world->sorted_objects().end() );
          map< string, vector< Object* > >::const_iterator it_sorted_objects = it_sorted_objects_map->second.find( abstract_container_child.second->type() );
          assert( it_sorted_objects != it_sorted_objects_map->second.end() );
          for( unsigned int i = 0; i < abstract_container_child.second->number(); i++ ){
            if( object->id() == it_sorted_objects->second[ i ]->id() ){
              return !_invert;
            }
          }
        }
*/
        }
      }
    }
  }

  return false;  
}


/*bool
Feature_Object_Merge_Object_Property_Region_Container::
value( const string& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world ){
    const Object * object = dynamic_cast< const Object* >( grounding );
    if( ( object != NULL ) && ( !children.empty() ) ){
      pair< const h2sl::Phrase*, const Object_Property* > object_property_child( NULL, NULL );
      pair< const h2sl::Phrase*, const Container* > container_child( NULL, NULL );
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
        if( ( object_property_child.first->min_word_order() < container_child.first->min_word_order() ) && ( object_property_child.second->index() < container_child.second->container().size() ) && ( !container_child.second->container().empty() ) ){
          bool found_object_type_match = false;
          for( unsigned int i = 0; i < container_child.second->container().size(); i++ ){
            if( object_property_child.second->object_type() == container_child.second->container()[ i ]->type() ){
              found_object_type_match = true;
            }
          }
          if( found_object_type_match == _object_match ){
            vector< Object* > sorted_objects = container_child.second->container();
            if( object_property_child.second->index() < sorted_objects.size() ){
              if( object_property_child.second->relation_type() == Spatial_Relation::TYPE_CENTER ){
                World::min_center_distance_sort_objects( sorted_objects );
                if( *object == *sorted_objects[ object_property_child.second->index() ] ){
                  return !_invert;
                }
              } else if ( object_property_child.second->relation_type() == Spatial_Relation::TYPE_FAR ){
                World::max_distance_sort_objects( sorted_objects );
                if( *object == *sorted_objects[ object_property_child.second->index() ] ){
                  return !_invert;
                }
              } else if ( object_property_child.second->relation_type() == Spatial_Relation::TYPE_NEAR ){
                World::min_distance_sort_objects( sorted_objects );
                if( *object == *sorted_objects[ object_property_child.second->index() ] ){
                  return !_invert;
                }
              } else if ( object_property_child.second->relation_type() == Spatial_Relation::TYPE_LEFT ){
                World::max_y_sort_objects( sorted_objects );
                if( *object == *sorted_objects[ object_property_child.second->index() ] ){
                  return !_invert;
                }
              } else if ( object_property_child.second->relation_type() == Spatial_Relation::TYPE_RIGHT ){
                World::min_y_sort_objects( sorted_objects );
                if( *object == *sorted_objects[ object_property_child.second->index() ] ){
                  return !_invert;
                }
              } else if ( object_property_child.second->relation_type() == Spatial_Relation::TYPE_BACK ){
                World::max_x_sort_objects( sorted_objects );
                if( *object == *sorted_objects[ object_property_child.second->index() ] ){
                  return !_invert;
                }
              } else if ( object_property_child.second->relation_type() == Spatial_Relation::TYPE_FRONT ){
                World::min_x_sort_objects( sorted_objects );
                if( *object == *sorted_objects[ object_property_child.second->index() ] ){
                  return !_invert;
                }
              }
            }
            return _invert; 
          } else {
            const World * h2sl_nsf_nri_mvli_world = dynamic_cast< const World* >( world );
            if( object_property_child.second->relation_type() == Spatial_Relation::TYPE_CENTER ){
              if( object_property_child.second->index() < h2sl_nsf_nri_mvli_world->min_center_distance_sorted_objects()[ object_property_child.second->object_type() ].size() ){
                if( *object == *h2sl_nsf_nri_mvli_world->min_center_distance_sorted_objects()[ object_property_child.second->object_type() ][ object_property_child.second->index() ] ){
                  return !_invert;
                }
              }
            } else if ( object_property_child.second->relation_type() == Spatial_Relation::TYPE_FAR ){
              if( object_property_child.second->index() < h2sl_nsf_nri_mvli_world->max_distance_sorted_objects()[ object_property_child.second->object_type() ].size() ){
                if( *object == *h2sl_nsf_nri_mvli_world->max_distance_sorted_objects()[ object_property_child.second->object_type() ][ object_property_child.second->index() ] ){
                  return !_invert;
                }
              }
            } else if ( object_property_child.second->relation_type() == Spatial_Relation::TYPE_NEAR ){
              if( object_property_child.second->index() < h2sl_nsf_nri_mvli_world->min_distance_sorted_objects()[ object_property_child.second->object_type() ].size() ){
                if( *object == *h2sl_nsf_nri_mvli_world->min_distance_sorted_objects()[ object_property_child.second->object_type() ][ object_property_child.second->index() ] ){
                  return !_invert;
                }
              }
            } else if ( object_property_child.second->relation_type() == Spatial_Relation::TYPE_LEFT ){
              if( object_property_child.second->index() < h2sl_nsf_nri_mvli_world->max_y_sorted_objects()[ object_property_child.second->object_type() ].size() ){
                if( *object == *h2sl_nsf_nri_mvli_world->max_y_sorted_objects()[ object_property_child.second->object_type() ][ object_property_child.second->index() ] ){
                  return !_invert;
                }
              }
            } else if ( object_property_child.second->relation_type() == Spatial_Relation::TYPE_RIGHT ){
              if( object_property_child.second->index() < h2sl_nsf_nri_mvli_world->min_y_sorted_objects()[ object_property_child.second->object_type() ].size() ){
                if( *object == *h2sl_nsf_nri_mvli_world->min_y_sorted_objects()[ object_property_child.second->object_type() ][ object_property_child.second->index() ] ){
                  return !_invert;
                }
              }
            } else if ( object_property_child.second->relation_type() == Spatial_Relation::TYPE_BACK ){
              if( object_property_child.second->index() < h2sl_nsf_nri_mvli_world->max_x_sorted_objects()[ object_property_child.second->object_type() ].size() ){
                if( *object == *h2sl_nsf_nri_mvli_world->max_x_sorted_objects()[ object_property_child.second->object_type() ][ object_property_child.second->index() ] ){
                  return !_invert;
                }
              }
            } else if ( object_property_child.second->relation_type() == Spatial_Relation::TYPE_FRONT ){
              if( object_property_child.second->index() < h2sl_nsf_nri_mvli_world->min_x_sorted_objects()[ object_property_child.second->object_type() ].size() ){
                if( *object == *h2sl_nsf_nri_mvli_world->min_x_sorted_objects()[ object_property_child.second->object_type() ][ object_property_child.second->index() ] ){
                  return !_invert;
                }
              }
            }
            return _invert;
          } 
        }
      }
    }
    return false;
}*/

/**
 * exports the Feature_Object_Merge_Object_Property_Region_Container class to an XML file
 */
void
Feature_Object_Merge_Object_Property_Region_Container::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_merge_object_property_region_container" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( to_std_string( _invert ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "sorting_key" ), ( const xmlChar* )( sorting_key().c_str() ) ); 
  xmlAddChild( root, node );
  return;
}

/**
 * imports the Feature_Object_Merge_Object_Property_Region_Container class from an XML file
 */
void
Feature_Object_Merge_Object_Property_Region_Container::
from_xml( xmlNodePtr root ){
  _invert = false;
  sorting_key() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > feature_keys = { "invert", "sorting_key" };
    assert( check_keys( root, feature_keys ) );

    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ) {
      _invert = invert_prop.second;
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
     * Feature_Object_Merge_Object_Property_Region_Container class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Object_Merge_Object_Property_Region_Container& other ) {
      out << "class:\"Feature_Object_Merge_Object_Property_Region_Container\" ";  
      return out;
    }
    
}
