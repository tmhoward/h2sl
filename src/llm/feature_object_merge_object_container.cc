/**
 * @file feature_object_merge_object_container.cc
 *
 * @brief
 *
 * Class used to describe a feature container matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_object_merge_object_container.h"
#include "h2sl/container.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Merge_Object_Container class constructor
 */
Feature_Object_Merge_Object_Container::
Feature_Object_Merge_Object_Container( const bool& invert ) : Feature( invert ) {
    
}

/**
 * Feature_Object_Merge_Object_Container class destructor
 */
Feature_Object_Merge_Object_Container::
~Feature_Object_Merge_Object_Container() {
    
}

/**
 * Feature_Object_Merge_Object_Container class copy constructor
 */
Feature_Object_Merge_Object_Container::
Feature_Object_Merge_Object_Container( const Feature_Object_Merge_Object_Container& other ) : Feature( other ) {
    
}

/**
 * Feature_Object_Merge_Object_Container class assignment operator
 */
Feature_Object_Merge_Object_Container&
Feature_Object_Merge_Object_Container::
operator=( const Feature_Object_Merge_Object_Container& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Object_Merge_Object_Container::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}



bool
Feature_Object_Merge_Object_Container::
value( const string& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context ){
      const Object * object = dynamic_cast< const Object* >( grounding );

    if( ( object != NULL ) && ( !children.empty() ) ){
      pair< const Phrase*, const Object* > object_child( NULL, NULL );
      pair< const Phrase*, const Container* > container_child( NULL, NULL );
      
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
            if( dynamic_cast< const Object* >( children[ i ].second[ j ] ) != NULL ){
              object_child.first = children[ i ].first;
              object_child.second = static_cast< const Object* >( children[ i ].second[ j ] );
            }
          }
        }
      }

      if( ( object_child.first != NULL ) && ( object_child.second != NULL ) && ( container_child.first != NULL ) && ( container_child.second != NULL ) ){ 
        if( ( object_child.first->min_word_order() < container_child.first->min_word_order() ) && ( !container_child.second->container().empty() ) ){

          vector< Grounding* >sorted_objects_grounding = container_child.second->container();
          vector< Object* > sorted_objects; 
          for ( unsigned int i = 0; i < sorted_objects_grounding.size(); ++i ) {
            if ( dynamic_cast< Object* >( sorted_objects_grounding[ i ]) != NULL ) {
              sorted_objects.push_back( static_cast< Object* >( sorted_objects_grounding[ i ]) );
            } 
          }

          map< string, vector< Object* > >::const_iterator it;
 
          it = world->max_distance_sorted_objects().find( object_child.second->type() );
          if ( it != world->max_distance_sorted_objects().end() ) {
            if( *object_child.second == *it->second[ 0 ] ) {
              std::sort( sorted_objects.begin(), sorted_objects.end(), World::max_distance_sort );
              // look for the max distance object from within the container
              if( *object == *sorted_objects[ 0 ] ){
                return !_invert;
              }
            }

            if( *object_child.second == *it->second[ 1 ] ) {
              std::sort( sorted_objects.begin(), sorted_objects.end(), World::max_distance_sort );
              // look for the max distance object from within the container
              if( *object == *sorted_objects[ 1 ] ){
                return !_invert;
              }
            }

          }  

          it = world->min_distance_sorted_objects().find( object_child.second->type() );
          if ( it != world->min_distance_sorted_objects().end() ) {
            if( *object_child.second == *it->second[ 0 ] ) {
              std::sort( sorted_objects.begin(), sorted_objects.end(), World::min_distance_sort );
              // look for the min distance object from within the container
              if( *object == *sorted_objects[ 0 ] ){
                return !_invert;
              }
            }

            if( *object_child.second == *it->second[ 1 ] ) {
              std::sort( sorted_objects.begin(), sorted_objects.end(), World::min_distance_sort );
              // look for the min distance object from within the container
              if( *object == *sorted_objects[ 1 ] ){
                return !_invert;
              }
            }

          }   
        }
        return _invert;
      }
    }
    return false;    
}

/*bool
Feature_Object_Merge_Object_Container::
value( const string& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world ){
    const Object * object = dynamic_cast< const Object* >( grounding );
    if( ( object != NULL ) && ( !children.empty() ) ){
      pair< const h2sl::Phrase*, const Object* > object_child( NULL, NULL );
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
            if( dynamic_cast< const Object* >( children[ i ].second[ j ] ) != NULL ){
              object_child.first = children[ i ].first;
              object_child.second = static_cast< const Object* >( children[ i ].second[ j ] );
            }
          }
        }
      }

      if( ( object_child.first != NULL ) && ( object_child.second != NULL ) && ( container_child.first != NULL ) && ( container_child.second != NULL ) ){ 
        if( ( object_child.first->min_word_order() < container_child.first->min_word_order() ) && ( !container_child.second->container().empty() ) ){
          vector< Object* > sorted_objects = container_child.second->container();
          const World * world = dynamic_cast< const World* >( world );
          if( *object_child.second == *world->max_distance_sorted_objects()[ object_child.second->type() ][ 0 ] ){
            std::sort( sorted_objects.begin(), sorted_objects.end(), World::max_distance_sort );
            // look for the max distance object from within the container
            if( *object == *sorted_objects[ 0 ] ){
              return !_invert;
            }
          } else if( *object_child.second == *world->max_distance_sorted_objects()[ object_child.second->type() ][ 1 ] ){
            std::sort( sorted_objects.begin(), sorted_objects.end(), World::max_distance_sort );
            // look for the max distance object from within the container
            if( *object == *sorted_objects[ 1 ] ){
              return !_invert;
            }
          } else if( *object_child.second == *world->min_distance_sorted_objects()[ object_child.second->type() ][ 0 ] ){
            std::sort( sorted_objects.begin(), sorted_objects.end(), World::min_distance_sort );
            // look for the max distance object from within the container
            if( *object == *sorted_objects[ 0 ] ){
              return !_invert;
            }
          } else if( *object_child.second == *world->min_distance_sorted_objects()[ object_child.second->type() ][ 1 ] ){
            std::sort( sorted_objects.begin(), sorted_objects.end(), World::min_distance_sort );
            // look for the max distance object from within the container
            if( *object == *sorted_objects[ 1 ] ){
              return !_invert;
            }
          }
        }
        return _invert;
      }
    }
    return false;
}*/

/**
 * exports the Feature_Object_Merge_Object_Container class to an XML file
 */
void
Feature_Object_Merge_Object_Container::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_merge_object_container" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Object_Merge_Object_Container class from an XML file
 */
void
Feature_Object_Merge_Object_Container::
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
     * Feature_Object_Merge_Object_Container class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Object_Merge_Object_Container& other ) {
      out << "class:\"Feature_Object_Merge_Object_Container\" ";  
      return out;
    }
    
}
