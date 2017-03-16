/**
 * @file feature_object_merge_abstract_container_index_region_container.cc
 *
 * @brief
 *
 * Class used to describe a feature container matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_object_merge_abstract_container_index_region_container.h"
#include "h2sl/abstract_container.h"
#include "h2sl/region_container.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Merge_Abstract_Container_Index_Region_Container class constructor
 */
Feature_Object_Merge_Abstract_Container_Index_Region_Container::
Feature_Object_Merge_Abstract_Container_Index_Region_Container( const bool& invert ) : Feature( invert ) {
    
}

/**
 * Feature_Object_Merge_Abstract_Container_Index_Region_Container class destructor
 */
Feature_Object_Merge_Abstract_Container_Index_Region_Container::
~Feature_Object_Merge_Abstract_Container_Index_Region_Container() {
    
}

/**
 * Feature_Object_Merge_Abstract_Container_Index_Region_Container class copy constructor
 */
Feature_Object_Merge_Abstract_Container_Index_Region_Container::
Feature_Object_Merge_Abstract_Container_Index_Region_Container( const Feature_Object_Merge_Abstract_Container_Index_Region_Container& other ) : Feature( other ) {
    
}

/**
 * Feature_Object_Merge_Abstract_Container_Index_Region_Container class assignment operator
 */
Feature_Object_Merge_Abstract_Container_Index_Region_Container&
Feature_Object_Merge_Abstract_Container_Index_Region_Container::
operator=( const Feature_Object_Merge_Abstract_Container_Index_Region_Container& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */


bool
Feature_Object_Merge_Abstract_Container_Index_Region_Container::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Object_Merge_Abstract_Container_Index_Region_Container::
value( const string& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context ){
    
}


/*bool
Feature_Object_Merge_Abstract_Container_Index_Region_Container::
value( const string& cv,
       const h2sl::Grounding* grounding,
       const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
       const h2sl::Phrase* phrase,
       const World* world,
       const Grounding* context ){
    
    if ( children.empty() ) {
      return false; 
    }

    const Object * object = dynamic_cast< const Object* >( grounding );
    if( object == NULL) {
      return false;
    }

    pair< const h2sl::Phrase*, const Abstract_Container* > abstract_container_child( NULL, NULL );
    pair< const h2sl::Phrase*, const Region_Container* > region_container_child( NULL, NULL );

    for( unsigned int i = 0; i < children.size(); i++ ){
      for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
        if( dynamic_cast< const Abstract_Container* >( children[ i ].second[ j ] ) != NULL ){
          abstract_container_child.first = children[ i ].first;
          abstract_container_child.second = static_cast< const Abstract_Container* >( children[ i ].second[ j ] );
        }
      }
    }
    for( unsigned int i = 0; i < children.size(); i++ ){
      for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
        if ( dynamic_cast< const Region_Container* >( children[ i ].second[ j ] ) != NULL ){
          region_container_child.first = children[ i ].first;
          region_container_child.second = static_cast< const Region_Container* >( children[ i ].second[ j ] );
        }
      }
    }

    if( ( abstract_container_child.first == NULL ) || ( abstract_container_child.second == NULL ) 
          || ( region_container_child.first == NULL ) || ( region_container_child.second == NULL ) ){ 
      return false;
    } 
      
    unsigned int num_matching_objects = 0;
    for( unsigned int i = 0; i < region_container_child.second->container().container().size(); i++ ){
      const Object * region_container_object = dynamic_cast< const Object* >( region_container_child.second->container().container()[ i ] );
      if( region_container_object != NULL ) {
        if( abstract_container_child.second->type() == region_container_object->type() ){
          num_matching_objects++;
        }
      }
    }  

    if( ( abstract_container_child.first->min_word_order() < region_container_child.first->min_word_order() ) && 
          ( num_matching_objects >= abstract_container_child.second->num() ) ){

      if( ( abstract_container_child.second->number() == 1 ) && 
          ( abstract_container_child.second->index() < abstract_container_child.second->number() ) && 
          ( abstract_container_child.second->index() < region_container_child.second->container().container().size() ) ){
        
        vector< Object* > objects;
        for( unsigned int i = 0; i < region_container_child.second->container().container().size(); i++ ){
          const object * region_container_object = dynamic_cast< const Object* >( region_container_child.second->container().container()[ i ] );
          if( region_container_object->type() == abstract_container_child.second->type() ){
            objects.push_back( region_container_object );
          }
        }

        if( region_container_child.second->type() == string( "left" ) ){
          std::sort( objects.begin(), objects.end(), World::max_y_sort );
          if( ( abstract_container_child.second->index() ) < objects.size() ){
            if( *object == *objects[ world->index_map()[ abstract_container_child.second->index() ] ] ){
              return !_invert;
            }
          }
        } else if( region_container_child.second->type() == string( "right" ) ){
          std::sort( objects.begin(), objects.end(), World::min_y_sort );
          if( ( abstract_container_child.second->index() ) < objects.size() ){
            if( *object == *objects[ world->index_map()[ abstract_container_child.second->index() ] ] ){
              return !_invert;
            }
          }
        } else if ( region_container_child.second->type() == string( "front" ) ){
          std::sort( objects.begin(), objects.end(), World::min_x_sort );
          if( ( abstract_container_child.second->index() ) < objects.size() ){
            if( *object == *objects[ world->index_map()[ abstract_container_child.second->index() ] ] ){
              return !_invert;
            }
          }
        } else if ( region_container_child.second->type() == string( "back" ) ){
          std::sort( objects.begin(), objects.end(), World::max_x_sort );
          if( ( abstract_container_child.second->index() ) < objects.size() ){
            if( *object == *objects[ world->index_map()[ abstract_container_child.second->index() ] ] ){
              return !_invert;
            }
          }
        }
      }
    }
    return _invert;
}*/

/*

      if( ( abstract_container_child.first != NULL ) && ( abstract_container_child.second != NULL ) && ( region_container_child.first != NULL ) && ( region_container_child.second != NULL ) ){ 
        unsigned int num_matching_objects = 0;

        if( ( abstract_container_child.first->min_word_order() < region_container_child.first->min_word_order() ) && ( num_matching_objects >= abstract_container_child.second->num() ) ){
          if( ( abstract_container_child.second->num() == 1 ) && ( abstract_container_child.second->index() < abstract_container_child.second->num() ) && ( abstract_container_child.second->index() < region_container_child.second->container().container().size() ) ){
            vector< Object* > objects;
            for( unsigned int i = 0; i < region_container_child.second->container().container().size(); i++ ){
              if( region_container_child.second->container().container()[ i ]->type() == abstract_container_child.second->type() ){
                objects.push_back( region_container_child.second->container().container()[ i ] );
              }
            }
            if( region_container_child.second->type() == Spatial_Relation::TYPE_LEFT ){
              std::sort( objects.begin(), objects.end(), World::max_y_sort );
              if( ( abstract_container_child.second->index() ) < objects.size() ){
                if( *object == *objects[ abstract_container_child.second->index() ] ){
                  return !_invert;
                }
              }
            } else if( region_container_child.second->type() == Spatial_Relation::TYPE_RIGHT ){
              std::sort( objects.begin(), objects.end(), World::min_y_sort );
              if( ( abstract_container_child.second->index() ) < objects.size() ){
                if( *object == *objects[ abstract_container_child.second->index() ] ){
                  return !_invert;
                }
              }
            } else if ( region_container_child.second->type() == Spatial_Relation::TYPE_FRONT ){
              std::sort( objects.begin(), objects.end(), World::min_x_sort );
              if( ( abstract_container_child.second->index() ) < objects.size() ){
                if( *object == *objects[ abstract_container_child.second->index() ] ){
                  return !_invert;
                }
              }
            } else if ( region_container_child.second->type() == Spatial_Relation::TYPE_BACK ){
              std::sort( objects.begin(), objects.end(), World::max_x_sort );
              if( ( abstract_container_child.second->index() ) < objects.size() ){
                if( *object == *objects[ abstract_container_child.second->index() ] ){
                  return !_invert;
                }
              }
            }
          }
        }
        return _invert;
      }
    }
    return false;
}*/

/**
 * exports the Feature_Object_Merge_Abstract_Container_Index_Region_Container class to an XML file
 */
void
Feature_Object_Merge_Abstract_Container_Index_Region_Container::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_merge_abstract_container_index_region_container" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Object_Merge_Abstract_Container_Index_Region_Container class from an XML file
 */
void
Feature_Object_Merge_Abstract_Container_Index_Region_Container::
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
     * Feature_Object_Merge_Abstract_Container_Index_Region_Container class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Object_Merge_Abstract_Container_Index_Region_Container& other ) {
      out << "class:\"Feature_Object_Merge_Abstract_Container_Index_Region_Container\" ";  
      return out;
    }
    
}
