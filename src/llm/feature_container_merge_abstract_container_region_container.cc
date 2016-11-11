/**
 * @file feature_container_merge_abstract_container_region_container.cc
 *
 * @brief
 *
 * Class used to describe a feature container matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_container_merge_abstract_container_region_container.h"
#include "h2sl/abstract_container.h"
#include "h2sl/region_container.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Merge_Abstract_Container_Region_Container class constructor
 */
Feature_Container_Merge_Abstract_Container_Region_Container::
Feature_Container_Merge_Abstract_Container_Region_Container( const bool& invert ) : Feature( invert ) {
    
}

/**
 * Feature_Container_Merge_Abstract_Container_Region_Container class destructor
 */
Feature_Container_Merge_Abstract_Container_Region_Container::
~Feature_Container_Merge_Abstract_Container_Region_Container() {
    
}

/**
 * Feature_Container_Merge_Abstract_Container_Region_Container class copy constructor
 */
Feature_Container_Merge_Abstract_Container_Region_Container::
Feature_Container_Merge_Abstract_Container_Region_Container( const Feature_Container_Merge_Abstract_Container_Region_Container& other ) : Feature( other ) {
    
}

/**
 * Feature_Container_Merge_Abstract_Container_Region_Container class assignment operator
 */
Feature_Container_Merge_Abstract_Container_Region_Container&
Feature_Container_Merge_Abstract_Container_Region_Container::
operator=( const Feature_Container_Merge_Abstract_Container_Region_Container& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Container_Merge_Abstract_Container_Region_Container::
value( const unsigned int& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Container_Merge_Abstract_Container_Region_Container::
value( const unsigned int& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context){
    
}

/*bool
Feature_Container_Merge_Abstract_Container_Region_Container::
value( const unsigned int& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context){
    const Container * container = dynamic_cast< const Container* >( grounding );
    if( ( container != NULL ) && ( !children.empty() ) ){
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
        if( children[ i ].first != abstract_container_child.first ){
          for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
            if ( dynamic_cast< const Region_Container* >( children[ i ].second[ j ] ) != NULL ){
              region_container_child.first = children[ i ].first;
              region_container_child.second = static_cast< const Region_Container* >( children[ i ].second[ j ] );
            }
          }
        }
      }
      

      if( ( abstract_container_child.first != NULL ) && ( abstract_container_child.second != NULL ) && ( region_container_child.first != NULL ) && ( region_container_child.second != NULL ) ){ 
        unsigned int num_matching_objects = 0;
        for( unsigned int i = 0; i < region_container_child.second->container().container().size(); i++ ){
          if( abstract_container_child.second->type() == region_container_child.second->container().container()[ i ]->type() ){
            num_matching_objects++;
          }
        }


        if( ( abstract_container_child.first->min_word_order() < region_container_child.first->min_word_order() ) && ( num_matching_objects == 0 ) ){
          Container tmp( vector< Object* >(), container->type() );
          const World * h2sl_nsf_nri_mvli_world = dynamic_cast< const World* >( world );
          if( region_container_child.second->type() == Spatial_Relation::TYPE_NEAR ){
            if( abstract_container_child.second->num() < h2sl_nsf_nri_mvli_world->min_distance_sorted_objects()[ abstract_container_child.second->type() ].size() ){
              for( unsigned int i = 0; i < abstract_container_child.second->num(); i++ ){
                tmp.container().push_back( h2sl_nsf_nri_mvli_world->min_distance_sorted_objects()[ abstract_container_child.second->type() ][ i ] );
              }
              if( tmp == *container ){
                return !_invert;
              }
            }
          } else if( region_container_child.second->type() == Spatial_Relation::TYPE_FAR ){
            if( abstract_container_child.second->num() < h2sl_nsf_nri_mvli_world->max_distance_sorted_objects()[ abstract_container_child.second->type() ].size() ){
              for( unsigned int i = 0; i < abstract_container_child.second->num(); i++ ){
                tmp.container().push_back( h2sl_nsf_nri_mvli_world->max_distance_sorted_objects()[ abstract_container_child.second->type() ][ i ] );
              }
              if( tmp == *container ){
                return !_invert;
              }
            }
          } else if( region_container_child.second->type() == Spatial_Relation::TYPE_LEFT ){
            if( abstract_container_child.second->num() < h2sl_nsf_nri_mvli_world->max_y_sorted_objects()[ abstract_container_child.second->type() ].size() ){
              for( unsigned int i = 0; i < abstract_container_child.second->num(); i++ ){
                tmp.container().push_back( h2sl_nsf_nri_mvli_world->max_y_sorted_objects()[ abstract_container_child.second->type() ][ i ] );
              }
              if( tmp == *container ){
                return !_invert;
              }
            }
          } else if( region_container_child.second->type() == Spatial_Relation::TYPE_RIGHT ){
            if( abstract_container_child.second->num() < h2sl_nsf_nri_mvli_world->min_y_sorted_objects()[ abstract_container_child.second->type() ].size() ){
              for( unsigned int i = 0; i < abstract_container_child.second->num(); i++ ){
                tmp.container().push_back( h2sl_nsf_nri_mvli_world->min_y_sorted_objects()[ abstract_container_child.second->type() ][ i ] );
              }
              if( tmp == *container ){
                return !_invert;
              }
            }
          } else if ( region_container_child.second->type() == Spatial_Relation::TYPE_FRONT ){
            if( abstract_container_child.second->num() < h2sl_nsf_nri_mvli_world->min_x_sorted_objects()[ abstract_container_child.second->type() ].size() ){
              for( unsigned int i = 0; i < abstract_container_child.second->num(); i++ ){
                tmp.container().push_back( h2sl_nsf_nri_mvli_world->min_x_sorted_objects()[ abstract_container_child.second->type() ][ i ] );
              }
              if( tmp == *container ){
                return !_invert;
              }
            }
          } else if ( region_container_child.second->type() == Spatial_Relation::TYPE_BACK ){
            if( abstract_container_child.second->num() < h2sl_nsf_nri_mvli_world->max_x_sorted_objects()[ abstract_container_child.second->type() ].size() ){
              for( unsigned int i = 0; i < abstract_container_child.second->num(); i++ ){
                tmp.container().push_back( h2sl_nsf_nri_mvli_world->max_x_sorted_objects()[ abstract_container_child.second->type() ][ i ] );
              }
              if( tmp == *container ){
                return !_invert;
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
 * exports the Feature_Container_Merge_Abstract_Container_Region_Container class to an XML file
 */
void
Feature_Container_Merge_Abstract_Container_Region_Container::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_merge_abstract_container_region_container" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Container_Merge_Abstract_Container_Region_Container class from an XML file
 */
void
Feature_Container_Merge_Abstract_Container_Region_Container::
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
     * Feature_Container_Merge_Abstract_Container_Region_Container class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Container_Merge_Abstract_Container_Region_Container& other ) {
      out << "class:\"Feature_Container_Merge_Abstract_Container_Region_Container\" ";  
      return out;
    }
    
}