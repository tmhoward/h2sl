/**
 * @file feature_container_merge_container_pair.cc
 *
 * @brief
 *
 * Class used to describe a feature container matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_container_merge_container_pair.h"
#include "h2sl/abstract_container.h"
#include "h2sl/region_container.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Merge_Container_Pair class constructor
 */
Feature_Container_Merge_Container_Pair::
Feature_Container_Merge_Container_Pair( const bool& invert ) : Feature( invert ) {
    
}

/**
 * Feature_Container_Merge_Container_Pair class destructor
 */
Feature_Container_Merge_Container_Pair::
~Feature_Container_Merge_Container_Pair() {
    
}

/**
 * Feature_Container_Merge_Container_Pair class copy constructor
 */
Feature_Container_Merge_Container_Pair::
Feature_Container_Merge_Container_Pair( const Feature_Container_Merge_Container_Pair& other ) : Feature( other ) {
    
}

/**
 * Feature_Container_Merge_Container_Pair class assignment operator
 */
Feature_Container_Merge_Container_Pair&
Feature_Container_Merge_Container_Pair::
operator=( const Feature_Container_Merge_Container_Pair& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Container_Merge_Container_Pair::
value( const unsigned int& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Container_Merge_Container_Pair::
value( const unsigned int& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context ){
    
}

/*bool
Feature_Container_Merge_Container_Pair::
value( const unsigned int& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world ){
    const Container * container = dynamic_cast< const Container* >( grounding );
    if( ( container != NULL ) && ( !children.empty() ) ){
      pair< const h2sl::Phrase*, const Container* > first_container_child( NULL, NULL );
      pair< const h2sl::Phrase*, const Container* > second_container_child( NULL, NULL );

      unsigned int num_containers = 0;


      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if( dynamic_cast< const Container* >( children[ i ].second[ j ] ) != NULL ){
            second_container_child.first = children[ i ].first;
            second_container_child.second = static_cast< const Container* >( children[ i ].second[ j ] );
            num_containers++;
          }
        }
      }

      if( num_containers != 2 ){
        return false;
      }

      for( unsigned int i = 0; i < children.size(); i++ ){
        if( children[ i ].first != second_container_child.first ){
          for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
            if ( dynamic_cast< const Container* >( children[ i ].second[ j ] ) != NULL ){
              first_container_child.first = children[ i ].first;
              first_container_child.second = static_cast< const Container* >( children[ i ].second[ j ] );
            }
          }
        }
      }
      
      if( ( first_container_child.first != NULL ) && ( first_container_child.second != NULL ) && ( second_container_child.first != NULL ) && ( second_container_child.second != NULL ) ){
        if( ( container->container().size() == first_container_child.second->container().size() ) && ( !container->container().empty() ) ){ 
          unsigned int object_type = Object_Type::TYPE_UNKNOWN;
          for( unsigned int i = 0; i < container->container().size(); i++ ){
            if( i == 0 ){
              object_type = container->container()[ i ]->type();
            } else if ( object_type != container->container()[ i ]->type() ){
              return false;
            }  
          }
 
          vector< Object* > sorted_objects = second_container_child.second->container();
 
          const World * h2sl_nsf_nri_mvli_world = dynamic_cast< const World* >( world );

          // check first container for nearest objects
          Container tmp( vector< Object* >(), first_container_child.second->type() );        
          if( first_container_child.second->container().size() < h2sl_nsf_nri_mvli_world->min_distance_sorted_objects()[ object_type ].size() ){
            for( unsigned int i = 0; i < first_container_child.second->container().size(); i++ ){
              tmp.container().push_back( h2sl_nsf_nri_mvli_world->min_distance_sorted_objects()[ object_type ][ i ] );
            }
          }
          if( *first_container_child.second == tmp ){
            if( first_container_child.second->container().size() < sorted_objects.size() ){   
              Container tmp2( vector< Object* >(), container->type() );
              World::min_distance_sort_objects( sorted_objects );  
              for( unsigned int i = 0; i < first_container_child.second->container().size(); i++ ){
                tmp2.container().push_back( sorted_objects[ i ] );
              }     

              if( *container == tmp2 ){
                return !_invert;
              } 
            }
          }

          tmp.container().clear();
        
          // check first container for furthest objects
          if( first_container_child.second->container().size() < h2sl_nsf_nri_mvli_world->max_distance_sorted_objects()[ object_type ].size() ){
            for( unsigned int i = 0; i < first_container_child.second->container().size(); i++ ){
              tmp.container().push_back( h2sl_nsf_nri_mvli_world->max_distance_sorted_objects()[ object_type ][ i ] );
            }
          }
          if( *first_container_child.second == tmp ){
            if( first_container_child.second->container().size() < sorted_objects.size() ){
              Container tmp2( vector< Object* >(), container->type() );
              World::max_distance_sort_objects( sorted_objects );
              for( unsigned int i = 0; i < first_container_child.second->container().size(); i++ ){
                tmp2.container().push_back( sorted_objects[ i ] );
              }

              if( *container == tmp2 ){
                return !_invert;
              }
            }
          }     
          
          tmp.container().clear();

          return _invert;
        }
      }
    }
    return false;
}*/

/**
 * exports the Feature_Container_Merge_Container_Pair class to an XML file
 */
void
Feature_Container_Merge_Container_Pair::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_merge_container_pair" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Container_Merge_Container_Pair class from an XML file
 */
void
Feature_Container_Merge_Container_Pair::
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
     * Feature_Container_Merge_Container_Pair class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Container_Merge_Container_Pair& other ) {
      out << "class:\"Feature_Container_Merge_Container_Pair\" ";  
      return out;
    }
    
}
