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
Feature_Container_Merge_Container_Pair( const bool& invert,
                                      const string& sortingKey ) : Feature( invert ) {
  insert_prop< std::string >( _string_properties, "sorting_key", sortingKey ); 
}

Feature_Container_Merge_Container_Pair::
Feature_Container_Merge_Container_Pair( xmlNodePtr root ) : Feature() {
  insert_prop< std::string >( _string_properties, "sorting_key", "na" );
  from_xml( root );
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
    _string_properties = other._string_properties;
    _int_properties = other._int_properties;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Container_Merge_Container_Pair::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}


bool
Feature_Container_Merge_Container_Pair::
value( const string& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context ){
    const Container * container = dynamic_cast< const Container* >( grounding );
    if( ( container != NULL ) && ( !children.empty() ) ){

      vector< Object* > container_objects;
      for( unsigned int i = 0; i < container->container().size(); i++ ){
        Object * container_object = dynamic_cast< Object* >( container->container()[ i ] );
        if( container_object != NULL ){
          container_objects.push_back( container_object );
        }
      }

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
        if( second_container_child.second->type() != container->type() ){
          return false;
        }

        vector< Object* > first_sorted_objects;
        for( unsigned int i = 0; i < first_container_child.second->container().size(); i++ ){
          Object * first_container_child_object = dynamic_cast< Object* >( first_container_child.second->container()[ i ] );
          if( first_container_child_object != NULL ){
            first_sorted_objects.push_back( first_container_child_object );
          }
        }

        if( ( !first_sorted_objects.empty() ) && ( container_objects.size() == first_sorted_objects.size() ) ){ 
          // first check for a property of the first container objects
          if( sorting_key() == "min_x" ){
            sort( first_sorted_objects.begin(), first_sorted_objects.end(), World::min_x_sort );
          } else if ( sorting_key() == "max_x" ){
            sort( first_sorted_objects.begin(), first_sorted_objects.end(), World::max_x_sort );
          } else if ( sorting_key() == "min_y" ){
            sort( first_sorted_objects.begin(), first_sorted_objects.end(), World::min_y_sort );
          } else if ( sorting_key() == "max_y" ){
            sort( first_sorted_objects.begin(), first_sorted_objects.end(), World::max_y_sort );
          } else if ( sorting_key() == "min_z" ){
            sort( first_sorted_objects.begin(), first_sorted_objects.end(), World::min_z_sort );
          } else if ( sorting_key() == "max_z" ){
            sort( first_sorted_objects.begin(), first_sorted_objects.end(), World::max_z_sort );
          } else if ( sorting_key() == "min_abs_x" ){
            sort( first_sorted_objects.begin(), first_sorted_objects.end(), World::min_abs_x_sort );
          } else if ( sorting_key() == "max_abs_x" ){
            sort( first_sorted_objects.begin(), first_sorted_objects.end(), World::max_abs_x_sort );
          } else if ( sorting_key() == "min_abs_y" ){
            sort( first_sorted_objects.begin(), first_sorted_objects.end(), World::min_abs_y_sort );
          } else if ( sorting_key() == "max_abs_y" ){
            sort( first_sorted_objects.begin(), first_sorted_objects.end(), World::max_abs_y_sort );
          } else if ( sorting_key() == "min_distance" ){
            sort( first_sorted_objects.begin(), first_sorted_objects.end(), World::min_distance_sort );
          } else if ( sorting_key() == "max_distance" ){
            sort( first_sorted_objects.begin(), first_sorted_objects.end(), World::max_distance_sort );
          } else if ( sorting_key() == "min_center_distance" ){
            World::min_center_distance_sort_objects( first_sorted_objects );
          } else if ( sorting_key() == "max_center_distance" ){
            World::max_center_distance_sort_objects( first_sorted_objects );
          }

          map< string, map< string, vector< Object* > > >::const_iterator it_sorted_objects_map = world->sorted_objects().find( sorting_key() );
          if( it_sorted_objects_map == world->sorted_objects().end() ){
            cout << "could not find sorting index \"" << sorting_key() << "\"" << endl;
          }
          assert( it_sorted_objects_map != world->sorted_objects().end() );
          map< string, vector< Object* > >::const_iterator it_sorted_objects = it_sorted_objects_map->second.find( first_sorted_objects.back()->type() );
          assert( it_sorted_objects != it_sorted_objects_map->second.end() );
          bool all_first_match = true;
          for( unsigned int i = 0; i < first_sorted_objects.size(); i++ ){
            if( ( first_sorted_objects[ i ] != NULL ) && ( it_sorted_objects->second[ i ] != NULL ) ){
              if( first_sorted_objects[ i ]->id() != it_sorted_objects->second[ i ]->id() ){
                all_first_match = false;
              }
            }
          }

          if( all_first_match ){
            // now sort the second set and see if the object grounding matches
            vector< Object* > second_sorted_objects;
            for( unsigned int i = 0; i < second_container_child.second->container().size(); i++ ){
              Object * second_container_child_object = dynamic_cast< Object* >( second_container_child.second->container()[ i ] );
              if( second_container_child_object != NULL ){
                // check to make sure the object type is consistent with the first objects
                for( unsigned int j = 0; j < first_sorted_objects.size(); j++ ){
                  if( first_sorted_objects[ j ]->type() != second_container_child_object->type() ){
                    return false;
                  }
                }
                second_sorted_objects.push_back( second_container_child_object );
              }
            }

            if( ( !second_sorted_objects.empty() ) && ( first_sorted_objects.size() <= second_sorted_objects.size() ) ){
              // second check for a property of the second container objects
              if( sorting_key() == "min_x" ){
                sort( second_sorted_objects.begin(), second_sorted_objects.end(), World::min_x_sort );
              } else if ( sorting_key() == "max_x" ){
                sort( second_sorted_objects.begin(), second_sorted_objects.end(), World::max_x_sort );
              } else if ( sorting_key() == "min_y" ){
                sort( second_sorted_objects.begin(), second_sorted_objects.end(), World::min_y_sort );
              } else if ( sorting_key() == "max_y" ){
                sort( second_sorted_objects.begin(), second_sorted_objects.end(), World::max_y_sort );
              } else if ( sorting_key() == "min_z" ){
                sort( second_sorted_objects.begin(), second_sorted_objects.end(), World::min_z_sort );
              } else if ( sorting_key() == "max_z" ){
                sort( second_sorted_objects.begin(), second_sorted_objects.end(), World::max_z_sort );
              } else if ( sorting_key() == "min_abs_x" ){
                sort( second_sorted_objects.begin(), second_sorted_objects.end(), World::min_abs_x_sort );
              } else if ( sorting_key() == "max_abs_x" ){
                sort( second_sorted_objects.begin(), second_sorted_objects.end(), World::max_abs_x_sort );
              } else if ( sorting_key() == "min_abs_y" ){
                sort( second_sorted_objects.begin(), second_sorted_objects.end(), World::min_abs_y_sort );
              } else if ( sorting_key() == "max_abs_y" ){
                sort( second_sorted_objects.begin(), second_sorted_objects.end(), World::max_abs_y_sort );
              } else if ( sorting_key() == "min_distance" ){
                sort( second_sorted_objects.begin(), second_sorted_objects.end(), World::min_distance_sort );
              } else if ( sorting_key() == "max_distance" ){
                sort( second_sorted_objects.begin(), second_sorted_objects.end(), World::max_distance_sort );
              } else if ( sorting_key() == "min_center_distance" ){
                World::min_center_distance_sort_objects( second_sorted_objects );
              } else if ( sorting_key() == "max_center_distance" ){
                World::max_center_distance_sort_objects( second_sorted_objects );
              }
           
              bool all_match = true; 
              for( unsigned int i = 0; i < first_sorted_objects.size(); i++ ){
                bool found_object_match = false;
                for( unsigned int j = 0; j < container_objects.size(); j++ ){
                  if( ( second_sorted_objects[ i ] != NULL ) && ( container_objects[ j ] != NULL ) ){
                    if( second_sorted_objects[ i ]->id() == container_objects[ j ]->id() ){
                      found_object_match = true;
                    }
                  }
                }
                all_match = all_match && found_object_match;
              }    
              if( all_match ){
                return !_invert;
              } else {
                return _invert;
              }
            }
          }
   
 /*
          vector< Object* > sorted_objects = second_container_child.second->container();
 

          // check first container for nearest objects
          Container tmp( vector< Object* >(), first_container_child.second->type() );        
          if( first_container_child.second->container().size() < h2sl_nsf_nri_mvli_world->min_distance_sorted_objects()[ object->type() ].size() ){
            for( unsigned int i = 0; i < first_container_child.second->container().size(); i++ ){
              tmp.container().push_back( h2sl_nsf_nri_mvli_world->min_distance_sorted_objects()[ object->type() ][ i ] );
            }
          }
          if( *first_container_child.second == tmp ){
            if( first_container_child.second->container().size() < sorted_objects.size() ){   
              World::min_distance_sort_objects( sorted_objects );  
              for( unsigned int i = 0; i < first_container_child.second->container().size(); i++ ){
                if( *object == *sorted_objects[ i ] ){ 
                  return !_invert;
                }
              } 
            }
          }

          tmp.container().clear();
        
          // check first container for furthest objects
          if( first_container_child.second->container().size() < h2sl_nsf_nri_mvli_world->max_distance_sorted_objects()[ object->type() ].size() ){
            for( unsigned int i = 0; i < first_container_child.second->container().size(); i++ ){
              tmp.container().push_back( h2sl_nsf_nri_mvli_world->max_distance_sorted_objects()[ object->type() ][ i ] );
            }
          }
          if( *first_container_child.second == tmp ){
            if( first_container_child.second->container().size() < sorted_objects.size() ){
              World::max_distance_sort_objects( sorted_objects );
              for( unsigned int i = 0; i < first_container_child.second->container().size(); i++ ){
                if( *object == *sorted_objects[ i ] ){
                  return !_invert;
                }
              }
            }
          }     
          
          tmp.container().clear();
*/
        }
      }
    }
    return false;
}

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
    xmlNewProp( node, ( const xmlChar* )( "sorting_key" ), ( const xmlChar* )( sorting_key().c_str() ) );
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
    sorting_key() = "na";
    if( root->type == XML_ELEMENT_NODE ){
      vector< string > feature_keys = { "invert", "sorting_key" };
      assert( check_keys( root, feature_keys ) );

        xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
        if( tmp != NULL ){
            string invert_string = ( char* )( tmp );
            _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
            xmlFree( tmp );
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
     * Feature_Container_Merge_Container_Pair class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Container_Merge_Container_Pair& other ) {
      out << "class:\"Feature_Container_Merge_Container_Pair\" sorting_key:(" << other.sorting_key() << ")";  
      return out;
    }
    
}
