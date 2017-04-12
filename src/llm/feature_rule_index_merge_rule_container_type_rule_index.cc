/**
 * @file feature_rule_index_merge_rule_container_type_rule_index.cc
 *
 * @brief
 *
 * Class used to describe a feature index matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_rule_index_merge_rule_container_type_rule_index.h"
#include "h2sl/rule_index.h"
#include "h2sl/rule_container_type.h"
#include "h2sl/rule_index.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index class constructor
 */
Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index::
Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index( const bool& invert,
                                                        const string& sortingKey ) : Feature( invert ) {
  insert_prop< std::string >( _string_properties, "sorting_key", sortingKey );
}

Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index::
Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index( xmlNodePtr root ) : Feature() {
  insert_prop< std::string >( _string_properties, "sorting_key", "na" );
  from_xml( root );
}

/**
 * Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index class destructor
 */
Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index::
~Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index() {
    
}

/**
 * Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index class copy constructor
 */
Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index::
Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index( const Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index& other ) : Feature( other ) {
    
}

/**
 * Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index class assignment operator
 */
Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index&
Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index::
operator=( const Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index& other ) {
    _invert = other._invert;
    _string_properties = other._string_properties;
    _int_properties = other._int_properties;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world,
      const Grounding* context){
    const Rule_Index * rule_index = dynamic_cast< const Rule_Index* >( grounding );
    if( ( rule_index != NULL ) && ( !children.empty() ) ){
    pair< const Phrase*, const Rule_Container_Type* > rule_container_type_child( NULL, NULL );
    pair< const Phrase*, const Rule_Index* > rule_index_child( NULL, NULL );
    // enforce that children come from different phrases
    for( unsigned int i = 0; i < children.size(); i++ ){
      for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
        if ( dynamic_cast< const Rule_Container_Type* >( children[ i ].second[ j ] ) != NULL ){
          rule_container_type_child.first = children[ i ].first;
          rule_container_type_child.second = static_cast< const Rule_Container_Type* >( children[ i ].second[ j ] );
        }
      }
    }
    if( rule_container_type_child.second == NULL ){
      return false;
    }
    for( unsigned int i = 0; i < children.size(); i++ ){
      if( children[ i ].first != rule_container_type_child.first ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if( dynamic_cast< const Rule_Index* >( children[ i ].second[ j ] ) != NULL ){
            rule_index_child.first = children[ i ].first;
            rule_index_child.second = static_cast< const Rule_Index* >( children[ i ].second[ j ] );
          }
        }
      }
    }

    if( ( rule_container_type_child.first != NULL ) && ( rule_container_type_child.second != NULL ) && ( rule_index_child.first != NULL ) && ( rule_index_child.second != NULL ) ){
      if( rule_container_type_child.first->min_word_order() > rule_index_child.first->min_word_order() ){
        if( *rule_index == *rule_index_child.second ){
          return !_invert;
        } else {
          return _invert;
        }
      }
    }
  }


  return false;
}

/**
 * exports the Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index class to an XML file
 */
void
Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_rule_index_merge_rule_container_type_rule_index" ), NULL );
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( to_std_string( _invert ).c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "sorting_key" ), ( const xmlChar* )( sorting_key().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index class from an XML file
 */
void
Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index::
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
     * Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index& other ) {
        out << "Feature_Rule_Index_Merge_Rule_Container_Type_Rule_Index:(invert:(" << other.invert() << ") sorting_key:(" << other.sorting_key() << ")";
        return out;
    }
    
}
