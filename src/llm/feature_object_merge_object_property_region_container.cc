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
                                                        const bool& objectMatch ) : Feature( invert ),
                                                                                    _object_match( objectMatch ) {
    
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
Feature_Object_Merge_Object_Property_Region_Container( const Feature_Object_Merge_Object_Property_Region_Container& other ) : Feature( other ),
                                                                                                                              _object_match( other._object_match ) {
    
}

/**
 * Feature_Object_Merge_Object_Property_Region_Container class assignment operator
 */
Feature_Object_Merge_Object_Property_Region_Container&
Feature_Object_Merge_Object_Property_Region_Container::
operator=( const Feature_Object_Merge_Object_Property_Region_Container& other ) {
    _invert = other._invert;
    _object_match = other._object_match;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Object_Merge_Object_Property_Region_Container::
value( const unsigned int& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Object_Merge_Object_Property_Region_Container::
value( const unsigned int& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context ){
    
}

/*bool
Feature_Object_Merge_Object_Property_Region_Container::
value( const unsigned int& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world ){
    const Object * object = dynamic_cast< const Object* >( grounding );
    if( ( object != NULL ) && ( !children.empty() ) ){
      pair< const h2sl::Phrase*, const Object_Property* > object_property_child( NULL, NULL );
      pair< const h2sl::Phrase*, const Region_Container* > region_container_child( NULL, NULL );
      // enforce that children come from different phrases
      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if ( dynamic_cast< const Region_Container* >( children[ i ].second[ j ] ) != NULL ){
            region_container_child.first = children[ i ].first;
            region_container_child.second = static_cast< const Region_Container* >( children[ i ].second[ j ] );
          }
        }
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
        if( ( object_property_child.first->min_word_order() < region_container_child.first->min_word_order() ) && ( object_property_child.second->index() < region_container_child.second->container().container().size() ) && ( !region_container_child.second->container().container().empty() ) ){
          bool found_object_type_match = false;
          for( unsigned int i = 0; i < region_container_child.second->container().container().size(); i++ ){
            if( object_property_child.second->object_type() == region_container_child.second->container().container()[ i ]->type() ){
              found_object_type_match = true;
            }
          }
          if( found_object_type_match == _object_match ){
            vector< Object* > sorted_objects = region_container_child.second->container().container();
            if( object_property_child.second->index() < sorted_objects.size() ){
              if( region_container_child.second->type() == Spatial_Relation::TYPE_CENTER ){
                World::min_center_distance_sort_objects( sorted_objects );
                if( *object == *sorted_objects[ object_property_child.second->index() ] ){
                  return !_invert;
                }
              } else if ( region_container_child.second->type() == Spatial_Relation::TYPE_FAR ){
                World::max_distance_sort_objects( sorted_objects );
                if( *object == *sorted_objects[ object_property_child.second->index() ] ){
                  return !_invert;
                }
              } else if ( region_container_child.second->type() == Spatial_Relation::TYPE_NEAR ){
                World::min_distance_sort_objects( sorted_objects );
                if( *object == *sorted_objects[ object_property_child.second->index() ] ){
                  return !_invert;
                }
              } else if ( region_container_child.second->type() == Spatial_Relation::TYPE_LEFT ){
                World::max_y_sort_objects( sorted_objects );
                if( *object == *sorted_objects[ object_property_child.second->index() ] ){
                  return !_invert;
                }
              } else if ( region_container_child.second->type() == Spatial_Relation::TYPE_RIGHT ){
                World::min_y_sort_objects( sorted_objects );
                if( *object == *sorted_objects[ object_property_child.second->index() ] ){
                  return !_invert;
                }
              } else if ( region_container_child.second->type() == Spatial_Relation::TYPE_BACK ){
                World::max_x_sort_objects( sorted_objects );
                if( *object == *sorted_objects[ object_property_child.second->index() ] ){
                  return !_invert;
                }
              } else if ( region_container_child.second->type() == Spatial_Relation::TYPE_FRONT ){
                World::min_x_sort_objects( sorted_objects );
                if( *object == *sorted_objects[ object_property_child.second->index() ] ){
                  return !_invert;
                }
              } 
              return _invert;
            } 
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
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    stringstream object_match_string;
    object_match_string << _object_match;
    xmlNewProp( node, ( const xmlChar* )( "object_match" ), ( const xmlChar* )( object_match_string.str().c_str() ) );
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
    if( root->type == XML_ELEMENT_NODE ){
        xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
        if( tmp != NULL ){
            string invert_string = ( char* )( tmp );
            _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
            xmlFree( tmp );
        }
        tmp = xmlGetProp( root, ( const xmlChar* )( "object_match" ) );
        if( tmp != NULL ){
            string object_match_string = ( char* )( tmp );
            _object_match = ( bool )( strtol( object_match_string.c_str(), NULL, 10 ) );
            xmlFree( tmp );
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
