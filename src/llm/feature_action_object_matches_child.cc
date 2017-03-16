/**
 * @file feature_action_object_matches_child.cc
 *
 * @brief
 *
 * Class used to describe a feature action object matches child type
 */
#include <assert.h>
#include <sstream>

#include "h2sl/feature_action_object_matches_child.h"
#include "h2sl/action.h"
#include "h2sl/object.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Action_Object_Matches_Child class constructor
 */
Feature_Action_Object_Matches_Child::
Feature_Action_Object_Matches_Child( const bool& invert ) : Feature( invert ) {
    
}

/**
 * Feature_Action_Object_Matches_Child class destructor
 */
Feature_Action_Object_Matches_Child::
~Feature_Action_Object_Matches_Child() {
    
}

/**
 * Feature_Action_Object_Matches_Child class copy constructor
 */
Feature_Action_Object_Matches_Child::
Feature_Action_Object_Matches_Child( const Feature_Action_Object_Matches_Child& other ) : Feature( other ) {
    
}

/**
 * Feature_Action_Object_Matches_Child class assignment operator
 */
Feature_Action_Object_Matches_Child&
Feature_Action_Object_Matches_Child::
operator=( const Feature_Action_Object_Matches_Child& other ) {
    _invert = other._invert;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Action_Object_Matches_Child::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Action_Object_Matches_Child::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    const Action* action = dynamic_cast< const Action* >( grounding );
    if( ( action != NULL ) && ( !children.empty() ) ){
      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          const Object* object_child = dynamic_cast< const Object* >( children[ i ].second[ j ] );
          if( object_child != NULL ){
            for( std::map< string, string >::const_iterator it = action->params().begin(); it != action->params().end(); ++it ){
              if( ( it->first == "pick_object" ) || ( it->first =="place_object" ) ){
                if( it->second == object_child->name() ){
                  return !_invert;
                }
              }
            }
          }
        }
      }
      return _invert;
    }
    return false;
}

/**
 * exports the Feature_Action_Object_Matches_Child class to an XML file
 */
void
Feature_Action_Object_Matches_Child::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_action_object_matches_child" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Action_Object_Matches_Child class from an XML file
 */
void
Feature_Action_Object_Matches_Child::
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
     * Feature_Action_Object_Matches_Child class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Action_Object_Matches_Child& other ) {
        out << "class:\"Feature_Action_Object_Matches_Child\" ";
	return out;
    }
    
}
