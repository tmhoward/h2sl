/**
 * @file feature_rule_spatial_relation_merge_rule_spatial_relation_rule_container_type.cc
 *
 * @brief
 *
 * Class used to describe a feature spatial_relation matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_rule_spatial_relation_merge_rule_spatial_relation_rule_container_type.h"
#include "h2sl/rule_spatial_relation.h"
#include "h2sl/rule_container_type.h"
#include "h2sl/rule_spatial_relation.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type class constructor
 */
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type::
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type( const bool& invert,
                                                        const string& sortingKey,
                                                        const string& spatialRelationType ) : Feature( invert ) {
  insert_prop< std::string >( _string_properties, "sorting_key", sortingKey );
  insert_prop< std::string >( _string_properties, "spatial_relation_type", spatialRelationType );
}

Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type::
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type( xmlNodePtr root ) : Feature() {
  insert_prop< std::string >( _string_properties, "sorting_key", "na" );
  insert_prop< std::string >( _string_properties, "spatial_relation_type", "na" );
  from_xml( root );
}

/**
 * Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type class destructor
 */
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type::
~Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type() {
    
}

/**
 * Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type class copy constructor
 */
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type::
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type( const Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type& other ) : Feature( other ) {
    
}

/**
 * Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type class assignment operator
 */
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type&
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type::
operator=( const Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type& other ) {
    _invert = other._invert;
    _string_properties = other._string_properties;
    _int_properties = other._int_properties;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world,
      const Grounding* context){
    const Rule_Spatial_Relation * rule_spatial_relation = dynamic_cast< const Rule_Spatial_Relation* >( grounding );
    if( ( rule_spatial_relation != NULL ) && ( !children.empty() ) ){
    pair< const Phrase*, const Rule_Container_Type* > rule_container_type_child( NULL, NULL );
    pair< const Phrase*, const Rule_Spatial_Relation* > rule_spatial_relation_child( NULL, NULL );
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
          if( dynamic_cast< const Rule_Spatial_Relation* >( children[ i ].second[ j ] ) != NULL ){
            rule_spatial_relation_child.first = children[ i ].first;
            rule_spatial_relation_child.second = static_cast< const Rule_Spatial_Relation* >( children[ i ].second[ j ] );
          }
        }
      }
    }

    if( ( rule_container_type_child.first != NULL ) && ( rule_container_type_child.second != NULL ) && ( rule_spatial_relation_child.first != NULL ) && ( rule_spatial_relation_child.second != NULL ) ){
      if( rule_container_type_child.first->min_word_order() > rule_spatial_relation_child.first->min_word_order() ){
        if( *rule_spatial_relation == *rule_spatial_relation_child.second ){
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
 * exports the Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type class to an XML file
 */
void
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_rule_spatial_relation_merge_rule_spatial_relation_rule_container_type" ), NULL );
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( to_std_string( _invert ).c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "sorting_key" ), ( const xmlChar* )( sorting_key().c_str() ) );
    xmlNewProp( node, ( const xmlChar* )( "spatial_relation_type" ), ( const xmlChar* )( spatial_relation_type().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type class from an XML file
 */
void
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type::
from_xml( xmlNodePtr root ){
  _invert = false;
  sorting_key() = "na";
  spatial_relation_type() = "na";

  if( root->type == XML_ELEMENT_NODE ){
    vector< string > feature_keys = { "invert", "sorting_key", "spatial_relation_type" };
    assert( check_keys( root, feature_keys ) );

    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ) {
      _invert = invert_prop.second;
    }

    pair< bool, std::string > sorting_key_prop = has_prop< std::string >( root, "sorting_key" );
    if( sorting_key_prop.first ) {
      sorting_key() = sorting_key_prop.second;
    }

    pair< bool, std::string > spatial_relation_type_prop = has_prop< std::string >( root, "spatial_relation_type" );
    if( spatial_relation_type_prop.first ) {
      spatial_relation_type() = spatial_relation_type_prop.second;
    }

  }
  return;
}

namespace h2sl {
    /**
     * Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type& other ) {
        out << "Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relation_Rule_Container_Type:(invert:(" << other.invert() << ") sorting_key:(" << other.sorting_key() << ") spatial_relation_type:(" << other.spatial_relation_type() << "))";
        return out;
    }
    
}
