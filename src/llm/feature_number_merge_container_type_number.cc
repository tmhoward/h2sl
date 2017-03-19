/**
 * @file feature_number_merge_container_type_number.cc
 *
 * @brief
 *
 * Class used to describe a feature container matches child type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/feature_number_merge_container_type_number.h"
#include "h2sl/container_type.h"
#include "h2sl/number.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Number_Merge_Container_Type_Number class constructor
 */
Feature_Number_Merge_Container_Type_Number::
Feature_Number_Merge_Container_Type_Number( const bool& invert ) : Feature( invert ) {
    
}

/**
 * Feature_Number_Merge_Container_Type_Number class destructor
 */
Feature_Number_Merge_Container_Type_Number::
~Feature_Number_Merge_Container_Type_Number() {
    
}

/**
 * Feature_Number_Merge_Container_Type_Number class copy constructor
 */
Feature_Number_Merge_Container_Type_Number::
Feature_Number_Merge_Container_Type_Number( const Feature_Number_Merge_Container_Type_Number& other ) : Feature( other ) {
    
}

/**
 * Feature_Number_Merge_Container_Type_Number class assignment operator
 */
Feature_Number_Merge_Container_Type_Number&
Feature_Number_Merge_Container_Type_Number::
operator=( const Feature_Number_Merge_Container_Type_Number& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Number_Merge_Container_Type_Number::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Number_Merge_Container_Type_Number::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    const Number * number = dynamic_cast< const Number* >( grounding );
    if( ( number != NULL ) && ( !children.empty() ) ){
      pair< const h2sl::Phrase*, const Container_Type* > container_type_child( NULL, NULL );
      pair< const h2sl::Phrase*, const Number* > number_child( NULL, NULL );
      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if( dynamic_cast< const Container_Type* >( children[ i ].second[ j ] ) != NULL ){
            container_type_child.first = children[ i ].first;
            container_type_child.second = static_cast< const Container_Type* >( children[ i ].second[ j ] );
          } else if ( dynamic_cast< const Number* >( children[ i ].second[ j ] ) != NULL ){
            number_child.first = children[ i ].first;
            number_child.second = static_cast< const Number* >( children[ i ].second[ j ] );
          }
        }
      }

      if( ( container_type_child.first != NULL ) && ( container_type_child.second != NULL ) && ( number_child.first != NULL ) && ( number_child.second != NULL ) ){ 
        if( container_type_child.first->min_word_order() < number_child.first->min_word_order() ){
          if( *number == *number_child.second ){
            return !_invert;
          }
        }
        return _invert;
      }
    }
    return false;
}

/**
 * exports the Feature_Number_Merge_Container_Type_Number class to an XML file
 */
void
Feature_Number_Merge_Container_Type_Number::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_number_merge_container_type_number" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Number_Merge_Container_Type_Number class from an XML file
 */
void
Feature_Number_Merge_Container_Type_Number::
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
     * Feature_Number_Merge_Container_Type_Number class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Number_Merge_Container_Type_Number& other ) {
      out << "class:\"Feature_Number_Merge_Container_Type_Number\" ";  
      return out;
    }
    
}
