/**
 * @file feature_container_merge_container_region.cc
 *
 * @brief
 *
 * Class used to describe a feature container matches child type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/feature_container_merge_container_region.h"
#include "h2sl/container.h"
#include "h2sl/region.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Merge_Container_Region class constructor
 */
Feature_Container_Merge_Container_Region::
Feature_Container_Merge_Container_Region( const bool& invert ) : Feature( invert ) {
    
}

Feature_Container_Merge_Container_Region::
Feature_Container_Merge_Container_Region( xmlNodePtr root ) : Feature() {
  from_xml( root );
}

/**
 * Feature_Container_Merge_Container_Region class destructor
 */
Feature_Container_Merge_Container_Region::
~Feature_Container_Merge_Container_Region() {
    
}

/**
 * Feature_Container_Merge_Container_Region class copy constructor
 */
Feature_Container_Merge_Container_Region::
Feature_Container_Merge_Container_Region( const Feature_Container_Merge_Container_Region& other ) : Feature( other ) {
    
}

/**
 * Feature_Container_Merge_Container_Region class assignment operator
 */
Feature_Container_Merge_Container_Region&
Feature_Container_Merge_Container_Region::
operator=( const Feature_Container_Merge_Container_Region& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Container_Merge_Container_Region::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Container_Merge_Container_Region::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    const Container * container = dynamic_cast< const Container* >( grounding );
    if( ( container != NULL ) && ( !children.empty() ) ){
      pair< const h2sl::Phrase*, const Container* > container_child( NULL, NULL );
      pair< const h2sl::Phrase*, const Region* > region_child( NULL, NULL );
      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if( dynamic_cast< const Container* >( children[ i ].second[ j ] ) != NULL ){
            container_child.first = children[ i ].first;
            container_child.second = static_cast< const Container* >( children[ i ].second[ j ] );
          } else if ( dynamic_cast< const Region* >( children[ i ].second[ j ] ) != NULL ){
            region_child.first = children[ i ].first;
            region_child.second = static_cast< const Region* >( children[ i ].second[ j ] );
          }
        }
      }

      if( ( container_child.first != NULL ) && ( container_child.second != NULL ) && ( region_child.first != NULL ) && ( region_child.second != NULL ) ){ 
        if( container_child.first->min_word_order() < region_child.first->min_word_order() ){
          if( *container == *container_child.second ){
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
 * exports the Feature_Container_Merge_Container_Region class to an XML file
 */
void
Feature_Container_Merge_Container_Region::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_merge_container_region" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Container_Merge_Container_Region class from an XML file
 */
void
Feature_Container_Merge_Container_Region::
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
     * Feature_Container_Merge_Container_Region class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Container_Merge_Container_Region& other ) {
      out << "class:\"Feature_Container_Merge_Container_Region\" ";  
      return out;
    }
    
}
