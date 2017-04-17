/**
 * @file feature_rule_container_type_merge_rule_object_types.cc
 *
 * @brief
 *
 * Class used to describe a feature rule_container_type matches child types
 */

#include <assert.h>
#include <sstream>

#include "h2sl/feature_rule_container_type_merge_rule_object_types.h"
#include "h2sl/rule_container_type.h"
#include "h2sl/rule_object_type.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Rule_Container_Type_Merge_Rule_Object_Types class constructor
 */
Feature_Rule_Container_Type_Merge_Rule_Object_Types::
Feature_Rule_Container_Type_Merge_Rule_Object_Types( const bool& invert,
                                                        const bool& Order ) : Feature( invert ),
                                                                                      _order( Order ) {
    
}

/**
 * Feature_Rule_Container_Type_Merge_Rule_Object_Types class constructor
 */
Feature_Rule_Container_Type_Merge_Rule_Object_Types::
Feature_Rule_Container_Type_Merge_Rule_Object_Types( xmlNodePtr root ) : Feature(),
                                                                            _order( false ) {
  from_xml( root );
}

/**
 * Feature_Rule_Container_Type_Merge_Rule_Object_Types class destructor
 */
Feature_Rule_Container_Type_Merge_Rule_Object_Types::
~Feature_Rule_Container_Type_Merge_Rule_Object_Types() {
    
}

/**
 * Feature_Rule_Container_Type_Merge_Rule_Object_Types class copy constructor
 */
Feature_Rule_Container_Type_Merge_Rule_Object_Types::
Feature_Rule_Container_Type_Merge_Rule_Object_Types( const Feature_Rule_Container_Type_Merge_Rule_Object_Types& other ) : Feature( other ),
                                                                                                                                _order( other._order ) {
    
}

/**
 * Feature_Rule_Container_Type_Merge_Rule_Object_Types class assignment operator
 */
Feature_Rule_Container_Type_Merge_Rule_Object_Types&
Feature_Rule_Container_Type_Merge_Rule_Object_Types::
operator=( const Feature_Rule_Container_Type_Merge_Rule_Object_Types& other ) {
    _invert = other._invert;
    _order = other._order;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Rule_Container_Type_Merge_Rule_Object_Types::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Rule_Container_Type_Merge_Rule_Object_Types::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    const Rule_Container_Type * rule_container_type = dynamic_cast< const Rule_Container_Type* >( grounding );
    if( ( rule_container_type != NULL ) && ( !children.empty() ) ){
      vector< pair< const h2sl::Phrase*, const Rule_Object_Type* > > rule_object_type_children;
      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if( dynamic_cast< const Rule_Object_Type* >( children[ i ].second[ j ] ) != NULL ){
            rule_object_type_children.push_back( pair< const h2sl::Phrase*, const Rule_Object_Type* >( NULL, NULL ) );
            rule_object_type_children.back().first = children[ i ].first;
            rule_object_type_children.back().second = static_cast< const Rule_Object_Type* >( children[ i ].second[ j ] );
          }
        }
      }

      if( rule_object_type_children.size() == 2 ){
        if( rule_object_type_children[ 0 ].first->min_word_order() < rule_object_type_children[ 1 ].first->min_word_order() ){
          if( *rule_object_type_children[ 1 ].second == *rule_object_type_children[ 0 ].second ){
            return _invert;
          } else {
            return !_invert;
          }
        }
      }
    }
    return false;
}

/**
 * exports the Feature_Rule_Container_Type_Merge_Rule_Object_Types class to an XML file
 */
void
Feature_Rule_Container_Type_Merge_Rule_Object_Types::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_rule_container_type_merge_rule_object_types" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    stringstream order_string;
    order_string << _order;
    xmlNewProp( node, ( const xmlChar* )( "order" ), ( const xmlChar* )( order_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Rule_Container_Type_Merge_Rule_Object_Types class from an XML file
 */
void
Feature_Rule_Container_Type_Merge_Rule_Object_Types::
from_xml( xmlNodePtr root ){
    _invert = false;
    _order = false;
    if( root->type == XML_ELEMENT_NODE ){
      xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
      if( tmp != NULL ){
        string invert_string = ( char* )( tmp );
        _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
        xmlFree( tmp );
      }
    
      tmp = xmlGetProp( root, ( const xmlChar* )( "order" ) );
      if( tmp != NULL ){
        string order_string = ( char* )( tmp );
        _order = ( bool )( strtol( order_string.c_str(), NULL, 10 ) );
        xmlFree( tmp );
      }
    }
    return;
}

namespace h2sl {
    /**
     * Feature_Rule_Container_Type_Merge_Rule_Object_Types class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Rule_Container_Type_Merge_Rule_Object_Types& other ) {
      out << "class:\"Feature_Rule_Container_Type_Merge_Rule_Object_Types\" invert:(" << other.invert() << ") order:(" << other.order() << ")";  
      return out;
    }
    
}
