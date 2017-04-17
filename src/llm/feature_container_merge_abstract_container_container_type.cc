/**
 * @file feature_container_merge_abstract_container_container_type.cc
 *
 * @brief
 *
 * Class used to describe a feature container_type matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_container_merge_abstract_container_container_type.h"
#include "h2sl/abstract_container.h"
#include "h2sl/container.h"
#include "h2sl/container_type.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Merge_Abstract_Container_Container_Type class constructor
 */
Feature_Container_Merge_Abstract_Container_Container_Type::
Feature_Container_Merge_Abstract_Container_Container_Type( const bool& invert ) : Feature( invert ) {
}

Feature_Container_Merge_Abstract_Container_Container_Type::
Feature_Container_Merge_Abstract_Container_Container_Type( xmlNodePtr root ) : Feature() {
  from_xml( root );
}

/**
 * Feature_Container_Merge_Abstract_Container_Container_Type class destructor
 */
Feature_Container_Merge_Abstract_Container_Container_Type::
~Feature_Container_Merge_Abstract_Container_Container_Type() {
    
}

/**
 * Feature_Container_Merge_Abstract_Container_Container_Type class copy constructor
 */
Feature_Container_Merge_Abstract_Container_Container_Type::
Feature_Container_Merge_Abstract_Container_Container_Type( const Feature_Container_Merge_Abstract_Container_Container_Type& other ) : Feature( other ) {
    
}

/**
 * Feature_Container_Merge_Abstract_Container_Container_Type class assignment operator
 */
Feature_Container_Merge_Abstract_Container_Container_Type&
Feature_Container_Merge_Abstract_Container_Container_Type::
operator=( const Feature_Container_Merge_Abstract_Container_Container_Type& other ) {
    _invert = other._invert;
    _string_properties = other._string_properties;
    _int_properties = other._int_properties;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Container_Merge_Abstract_Container_Container_Type::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Container_Merge_Abstract_Container_Container_Type::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world,
      const Grounding* context ){
    const Container * container = dynamic_cast< const Container* >( grounding );
    if( ( container != NULL ) && ( !children.empty() ) ){
      pair< const Phrase*, const Abstract_Container* > abstract_container_child( NULL, NULL );
      pair< const Phrase*, const Container_Type* > container_type_child( NULL, NULL );
      // enforce that children come from different phrases
      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if ( dynamic_cast< const Container_Type* >( children[ i ].second[ j ] ) != NULL ){
            container_type_child.first = children[ i ].first;
            container_type_child.second = static_cast< const Container_Type* >( children[ i ].second[ j ] );
          }
        }
      }
      for( unsigned int i = 0; i < children.size(); i++ ){
        if( children[ i ].first != container_type_child.first ){
          for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
            if( dynamic_cast< const Abstract_Container* >( children[ i ].second[ j ] ) != NULL ){
              abstract_container_child.first = children[ i ].first;
              abstract_container_child.second = static_cast< const Abstract_Container* >( children[ i ].second[ j ] );
            }
          }
        }
      }

      if( ( abstract_container_child.first != NULL ) && ( abstract_container_child.second != NULL ) && ( container_type_child.first != NULL ) && ( container_type_child.second != NULL ) ){
        if( ( abstract_container_child.first->min_word_order() > container_type_child.first->min_word_order() ) ){
          if( container_type_child.second->type() == container->type() ){
            vector< Object* > sorted_objects;
            for( unsigned int i = 0; i < container->container().size(); i++ ){
              Object * container_object = dynamic_cast< Object* >( container->container()[ i ] );
              if( container_object != NULL ){
                if( ( container_object->type() == abstract_container_child.second->type() ) && ( container_object->color() == abstract_container_child.second->color() ) ){
                  sorted_objects.push_back( container_object );
                }
              }
            }
      
            if( ( int )( sorted_objects.size() ) == abstract_container_child.second->number() ){
              return !_invert;
            } else {
              return _invert;
            }
          } else {
            return _invert;
          }
        }
      }
    }
    return false;
}

/**
 * exports the Feature_Container_Merge_Abstract_Container_Container_Type class to an XML file
 */
void
Feature_Container_Merge_Abstract_Container_Container_Type::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_merge_abstract_container_container_type" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Container_Merge_Abstract_Container_Container_Type class from an XML file
 */
void
Feature_Container_Merge_Abstract_Container_Container_Type::
from_xml( xmlNodePtr root ){
  _invert = false;
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > feature_keys = { "invert" };
    assert( check_keys( root, feature_keys ) );

    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ) {
      _invert = invert_prop.second;
    }
  }
  return;
}

namespace h2sl {
    /**
     * Feature_Container_Merge_Abstract_Container_Container_Type class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Container_Merge_Abstract_Container_Container_Type& other ) {
      out << "Feature_Container_Merge_Abstract_Container_Container_Type:( invert:(" << other.invert() << "))";  
      return out;
    }
    
}
