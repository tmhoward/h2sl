/**
 * @file feature_rule_spatial_relation_merge_rule_spatial_relations.cc
 *
 * @brief
 *
 * Class used to describe a feature rule_spatial_relation matches child types
 */

#include <assert.h>
#include <sstream>

#include "h2sl/feature_rule_spatial_relation_merge_rule_spatial_relations.h"
#include "h2sl/rule_spatial_relation.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations class constructor
 */
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations::
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations( const bool& invert,
                                                        const bool& matchesFirst ) : Feature( invert ),
                                                                                      _matches_first( matchesFirst ) {
    
}

/**
 * Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations class constructor
 */
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations::
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations( xmlNodePtr root ) : Feature(),
                                                                            _matches_first( false ) {
  from_xml( root );
}

/**
 * Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations class destructor
 */
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations::
~Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations() {
    
}

/**
 * Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations class copy constructor
 */
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations::
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations( const Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations& other ) : Feature( other ),
                                                                                                                                _matches_first( other._matches_first ) {
    
}

/**
 * Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations class assignment operator
 */
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations&
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations::
operator=( const Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations& other ) {
    _invert = other._invert;
    _matches_first = other._matches_first;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    const Rule_Spatial_Relation * rule_spatial_relation = dynamic_cast< const Rule_Spatial_Relation* >( grounding );
    if( ( rule_spatial_relation != NULL ) && ( !children.empty() ) ){
      vector< pair< const h2sl::Phrase*, const Rule_Spatial_Relation* > > rule_spatial_relation_children;
      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if( dynamic_cast< const Rule_Spatial_Relation* >( children[ i ].second[ j ] ) != NULL ){
            rule_spatial_relation_children.push_back( pair< const h2sl::Phrase*, const Rule_Spatial_Relation* >( NULL, NULL ) );
            rule_spatial_relation_children.back().first = children[ i ].first;
            rule_spatial_relation_children.back().second = static_cast< const Rule_Spatial_Relation* >( children[ i ].second[ j ] );
          }
        }
      }

      if( rule_spatial_relation_children.size() == 2 ){
        if( rule_spatial_relation_children[ 0 ].first->min_word_order() < rule_spatial_relation_children[ 1 ].first->min_word_order() ){
          if( _matches_first ){
            if( *rule_spatial_relation == *rule_spatial_relation_children[ 0 ].second ){
              return !_invert;
            } else {
              return _invert;
            }
          } else {
            if( *rule_spatial_relation == *rule_spatial_relation_children[ 1 ].second ){
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
 * exports the Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations class to an XML file
 */
void
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_rule_spatial_relation_merge_rule_spatial_relations" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    stringstream matches_first_string;
    matches_first_string << _matches_first;
    xmlNewProp( node, ( const xmlChar* )( "matches_first" ), ( const xmlChar* )( matches_first_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations class from an XML file
 */
void
Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations::
from_xml( xmlNodePtr root ){
    _invert = false;
    _matches_first = false;
    if( root->type == XML_ELEMENT_NODE ){
      xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
      if( tmp != NULL ){
        string invert_string = ( char* )( tmp );
        _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
        xmlFree( tmp );
      }
    
      tmp = xmlGetProp( root, ( const xmlChar* )( "matches_first" ) );
      if( tmp != NULL ){
        string matches_first_string = ( char* )( tmp );
        _matches_first = ( bool )( strtol( matches_first_string.c_str(), NULL, 10 ) );
        xmlFree( tmp );
      }
    }
    return;
}

namespace h2sl {
    /**
     * Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations& other ) {
      out << "class:\"Feature_Rule_Spatial_Relation_Merge_Rule_Spatial_Relations\" invert:(" << other.invert() << ") matches_first:(" << other.matches_first() << ")";  
      return out;
    }
    
}
