/**
 * @file feature_constraint_object_relationship.cc
 *
 * @brief
 *
 * Class used to describe a feature container matches child type
 */

#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/feature_constraint_object_relationship.h"
#include "h2sl/object_property.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/constraint.h"
#include "h2sl/world.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Constraint_Object_Relationship class constructor
 */
Feature_Constraint_Object_Relationship::
Feature_Constraint_Object_Relationship( const bool& invert,
                                                const string& sortingKey,
                                                const string& referenceType ) : Feature( invert ) {
  insert_prop< std::string >( _string_properties, "sorting_key", sortingKey );    
  insert_prop< std::string >( _string_properties, "reference_type", referenceType );    
}

Feature_Constraint_Object_Relationship::
Feature_Constraint_Object_Relationship( xmlNodePtr root ) : Feature() {
  insert_prop< std::string >( _string_properties, "sorting_key", "na" );
  insert_prop< std::string >( _string_properties, "reference_type", "na" );
  from_xml( root );
}

/**
 * Feature_Constraint_Object_Relationship class destructor
 */
Feature_Constraint_Object_Relationship::
~Feature_Constraint_Object_Relationship() {
    
}

/**
 * Feature_Constraint_Object_Relationship class copy constructor
 */
Feature_Constraint_Object_Relationship::
Feature_Constraint_Object_Relationship( const Feature_Constraint_Object_Relationship& other ) : Feature( other ) {
    
}

/**
 * Feature_Constraint_Object_Relationship class assignment operator
 */
Feature_Constraint_Object_Relationship&
Feature_Constraint_Object_Relationship::
operator=( const Feature_Constraint_Object_Relationship& other ) {
    _invert = other._invert;
    _string_properties = other._string_properties;
    _int_properties = other._int_properties;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Constraint_Object_Relationship::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Constraint_Object_Relationship::
value( const string& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context ){
  
  const Constraint * constraint = dynamic_cast< const Constraint* >( grounding );
  if( constraint != NULL ){
    map< string, Object* >::const_iterator it_payload = world->objects().find( constraint->payload() );
    if( it_payload != world->objects().end() ){
      if( it_payload->second != NULL ){
        vector< Object* > objects;

        for( map< string, Object* >::const_iterator it_world_object = world->objects().begin(); it_world_object != world->objects().end(); it_world_object++ ){
          if( it_world_object->second != NULL ){
            if( it_world_object->second->type() == reference_type() ){
              objects.push_back( it_world_object->second );
            }
          }
        }

        if( !objects.empty() ){
          if ( sorting_key() == "min_distance" ){
            Object* min_object = NULL;
            for( vector< Object* >::const_iterator it_object = objects.begin(); it_object != objects.end(); it_object++ ){
              if( min_object != NULL ){
                if( Vector3::distance( (*it_object)->transform().position(), it_payload->second->transform().position() ) < Vector3::distance( min_object->transform().position(), it_payload->second->transform().position() ) ){
                  min_object = *it_object;
                }  
              } else {
                min_object = *it_object;
              }
            } 
            if( constraint->reference() == min_object->id() ){
              return !_invert;
            } else {
              return _invert;
            }
          } else if ( sorting_key() == "max_distance" ){
            Object* max_object = NULL;
            for( vector< Object* >::const_iterator it_object = objects.begin(); it_object != objects.end(); it_object++ ){
              if( max_object != NULL ){
                if( Vector3::distance( (*it_object)->transform().position(), it_payload->second->transform().position() ) > Vector3::distance( max_object->transform().position(), it_payload->second->transform().position() ) ){
                  max_object = *it_object;
                }
              } else {
                max_object = *it_object;
              }
            }
            if( constraint->reference() == max_object->id() ){
              return !_invert;
            } else {
              return _invert;
            }
          } 
        }
      }
    }
  }

  return false;  
}

/**
 * exports the Feature_Constraint_Object_Relationship class to an XML file
 */
void
Feature_Constraint_Object_Relationship::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_constraint_object_relationship" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( to_std_string( _invert ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "sorting_key" ), ( const xmlChar* )( sorting_key().c_str() ) ); 
  xmlNewProp( node, ( const xmlChar* )( "reference_type" ), ( const xmlChar* )( reference_type().c_str() ) ); 
  xmlAddChild( root, node );
  return;
}

/**
 * imports the Feature_Constraint_Object_Relationship class from an XML file
 */
void
Feature_Constraint_Object_Relationship::
from_xml( xmlNodePtr root ){
  _invert = false;
  sorting_key() = "na";
  reference_type() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > feature_keys = { "invert", "sorting_key", "reference_type" };
    assert( check_keys( root, feature_keys ) );

    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ) {
      _invert = invert_prop.second;
    }
  
    pair< bool, std::string > sorting_key_prop = has_prop< std::string >( root, "sorting_key" );
    if( sorting_key_prop.first ) {
      sorting_key() = sorting_key_prop.second;
    }

    pair< bool, std::string > reference_type_prop = has_prop< std::string >( root, "reference_type" );
    if( reference_type_prop.first ) {
      reference_type() = reference_type_prop.second;
    }
  }
  return;
}

namespace h2sl {
    /**
     * Feature_Constraint_Object_Relationship class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Constraint_Object_Relationship& other ) {
      out << "Feature_Constraint_Object_Relationship:(invert:(" << other.invert() << ") sorting_key:(" << other.sorting_key() << ") reference_type:(" << other.reference_type() << "))"; 
      return out;
    }
    
}
