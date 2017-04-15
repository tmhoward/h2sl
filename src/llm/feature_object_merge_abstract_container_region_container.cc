/**
 * @file feature_object_merge_abstract_container_region_container.cc
 *
 * @brief
 *
 * Class used to describe a feature container matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_object_merge_abstract_container_region_container.h"
#include "h2sl/abstract_container.h"
#include "h2sl/region_container.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Merge_Abstract_Container_Region_Container class constructor
 */
Feature_Object_Merge_Abstract_Container_Region_Container::
Feature_Object_Merge_Abstract_Container_Region_Container( const bool& invert,
                                                          const string& spatialRelationType,
                                                          const string& sortingKey ) : Feature( invert ) {
   insert_prop< std::string >( _string_properties, "spatial_relation_type", spatialRelationType );
   insert_prop< std::string >( _string_properties, "sorting_key", sortingKey );
 
}

/**
 * Feature_Object_Merge_Abstract_Container_Region_Container class constructor
 */
Feature_Object_Merge_Abstract_Container_Region_Container::
Feature_Object_Merge_Abstract_Container_Region_Container( xmlNodePtr root ) : Feature() {
   insert_prop< std::string >( _string_properties, "spatial_relation_type", "na" );
   insert_prop< std::string >( _string_properties, "sorting_key", "na" );
  from_xml( root );
} 

/**
 * Feature_Object_Merge_Abstract_Container_Region_Container class destructor
 */
Feature_Object_Merge_Abstract_Container_Region_Container::
~Feature_Object_Merge_Abstract_Container_Region_Container() {
    
}

/**
 * Feature_Object_Merge_Abstract_Container_Region_Container class copy constructor
 */
Feature_Object_Merge_Abstract_Container_Region_Container::
Feature_Object_Merge_Abstract_Container_Region_Container( const Feature_Object_Merge_Abstract_Container_Region_Container& other ) : Feature( other ) {
    
}

/**
 * Feature_Object_Merge_Abstract_Container_Region_Container class assignment operator
 */
Feature_Object_Merge_Abstract_Container_Region_Container&
Feature_Object_Merge_Abstract_Container_Region_Container::
operator=( const Feature_Object_Merge_Abstract_Container_Region_Container& other ) {
    _invert = other._invert;
    _string_properties = other._string_properties;
    _int_properties = other._int_properties;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Object_Merge_Abstract_Container_Region_Container::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Object_Merge_Abstract_Container_Region_Container::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world,
      const Grounding* context){
    const Object * object = dynamic_cast< const Object* >( grounding );
    if( ( object != NULL ) && ( !children.empty() ) ){
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
          if( dynamic_cast< const Object* >( region_container_child.second->container().container()[ i ] ) != NULL ){
            if( abstract_container_child.second->type() == static_cast< const Object* >( region_container_child.second->container().container()[ i ] )->type() ){
              num_matching_objects++;
            }
          }
        }

        if( ( abstract_container_child.first->min_word_order() < region_container_child.first->min_word_order() ) && ( num_matching_objects == 0 ) && ( region_container_child.second->relation_type() == spatial_relation_type() ) ){
          map< string, map< string, vector< Object* > > >::const_iterator it_sorted_objects_map = world->sorted_objects().find( sorting_key() );
          if( it_sorted_objects_map == world->sorted_objects().end() ){
            cout << "could not find sorting index \"" << sorting_key() << "\"" << endl;
          }
          assert( it_sorted_objects_map != world->sorted_objects().end() );
          map< string, vector< Object* > >::const_iterator it_sorted_objects = it_sorted_objects_map->second.find( abstract_container_child.second->type() );
          if( it_sorted_objects != it_sorted_objects_map->second.end() ){
            for( int i = 0; i < abstract_container_child.second->number(); i++ ){
              if( object->id() == it_sorted_objects->second[ i ]->id() ){
                return !_invert;
              }
            }
          }
          return _invert;
        }
      }
    }
    return false;
}

/**
 * exports the Feature_Object_Merge_Abstract_Container_Region_Container class to an XML file
 */
void
Feature_Object_Merge_Abstract_Container_Region_Container::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_merge_abstract_container_region_container" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "spatial_relation_type" ), ( const xmlChar* )( spatial_relation_type().c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "sorting_key" ), ( const xmlChar* )( sorting_key().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Object_Merge_Abstract_Container_Region_Container class from an XML file
 */
void
Feature_Object_Merge_Abstract_Container_Region_Container::
from_xml( xmlNodePtr root ){
    _invert = false;
    spatial_relation_type() = "na";
    sorting_key() = "na";
    if( root->type == XML_ELEMENT_NODE ){
      vector< string > feature_keys = { "invert", "spatial_relation_type", "sorting_key" };
      assert( check_keys( root, feature_keys ) );

        xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
        if( tmp != NULL ){
            string invert_string = ( char* )( tmp );
            _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
            xmlFree( tmp );
        }
    
        pair< bool, std::string > spatial_relation_type_prop = has_prop< std::string >( root, "spatial_relation_type" );
        if( spatial_relation_type_prop.first ) {
          spatial_relation_type() = spatial_relation_type_prop.second;
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
     * Feature_Object_Merge_Abstract_Container_Region_Container class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Object_Merge_Abstract_Container_Region_Container& other ) {
      out << "class:\"Feature_Object_Merge_Abstract_Container_Region_Container\" ";  
      return out;
    }
    
}
