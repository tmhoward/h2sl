/**
 * @file feature_object_merge_object_region_container.cc
 *
 * @brief
 *
 * Class used to describe a feature container matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_object_merge_object_region_container.h"
#include "h2sl/region_container.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Merge_Object_Region_Container class constructor
 */
Feature_Object_Merge_Object_Region_Container::
Feature_Object_Merge_Object_Region_Container( const bool& invert ) : Feature( invert ) {
    
}


Feature_Object_Merge_Object_Region_Container::
Feature_Object_Merge_Object_Region_Container( xmlNodePtr root ) : Feature() {
  from_xml( root );
}

/**
 * Feature_Object_Merge_Object_Region_Container class destructor
 */
Feature_Object_Merge_Object_Region_Container::
~Feature_Object_Merge_Object_Region_Container() {
    
}

/**
 * Feature_Object_Merge_Object_Region_Container class copy constructor
 */
Feature_Object_Merge_Object_Region_Container::
Feature_Object_Merge_Object_Region_Container( const Feature_Object_Merge_Object_Region_Container& other ) : Feature( other ) {
    
}

/**
 * Feature_Object_Merge_Object_Region_Container class assignment operator
 */
Feature_Object_Merge_Object_Region_Container&
Feature_Object_Merge_Object_Region_Container::
operator=( const Feature_Object_Merge_Object_Region_Container& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
/**
 * returns the value of the feature
 */
bool
Feature_Object_Merge_Object_Region_Container::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world) {
    return value( cv, grounding, children, phrase, world, NULL );
    
}

bool
Feature_Object_Merge_Object_Region_Container::
value( const string& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context ){
    const Object * object = dynamic_cast< const Object* >( grounding );
    if( ( object != NULL ) && ( !children.empty() ) ){
      pair< const h2sl::Phrase*, const Object* > object_child( NULL, NULL );
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
            if( dynamic_cast< const Object* >( children[ i ].second[ j ] ) != NULL ){
              object_child.first = children[ i ].first;
              object_child.second = static_cast< const Object* >( children[ i ].second[ j ] );
            }
          }
        }
      }

      if( ( object_child.first != NULL ) && ( object_child.second != NULL ) && ( region_container_child.first != NULL ) && ( region_container_child.second != NULL ) ){ 
        if( object_child.first->min_word_order() < region_container_child.first->min_word_order() ){
          bool found_object_type_in_region_container_objects = false;
          for( unsigned int i = 0; i < region_container_child.second->container().container().size(); i++ ){
            Object * region_container_object = dynamic_cast< Object* >( region_container_child.second->container().container()[ i ] );
            if( region_container_object != NULL ){
              if( object->type() == region_container_object->type() ){
                found_object_type_in_region_container_objects = true;
              }
            }
          }
          if( !found_object_type_in_region_container_objects ){
            if( object->id() == object_child.second->id() ){
              return !_invert;
            } else {
              return _invert;
            }
          }
        }
      }
    }
    return false;
}

/**
 * exports the Feature_Object_Merge_Object_Region_Container class to an XML file
 */
void
Feature_Object_Merge_Object_Region_Container::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_merge_object_region_container" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Object_Merge_Object_Region_Container class from an XML file
 */
void
Feature_Object_Merge_Object_Region_Container::
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
     * Feature_Object_Merge_Object_Region_Container class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Object_Merge_Object_Region_Container& other ) {
      out << "class:\"Feature_Object_Merge_Object_Region_Container\" ";  
      return out;
    }
    
}
