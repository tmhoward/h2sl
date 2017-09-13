/**
 * @file feature_object_matches_sorted_object.cc
 *
 * @brief
 *
 * Class used to describe feature_object_matches_sorted_object 
 */
#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/world.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/feature_object_matches_sorted_object.h"
#include "h2sl/object.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Matches_Sorted_Object class constructor
 */
Feature_Object_Matches_Sorted_Object::
Feature_Object_Matches_Sorted_Object( const bool& invert,
                                        const int& index,
                                        const string& sortingKey ) : Feature( invert ) {
  insert_prop< int >( _int_properties, "index", index );
  insert_prop< std::string >( _string_properties, "sorting_key", sortingKey );
}

Feature_Object_Matches_Sorted_Object::
Feature_Object_Matches_Sorted_Object( xmlNodePtr root ) : Feature() {
  insert_prop< int >( _int_properties, "index", 0 );
  insert_prop< std::string >( _string_properties, "sorting_key", "na" );
  from_xml( root );
}

/**
 * Feature_Object_Matches_Sorted_Object class copy constructor
 */
Feature_Object_Matches_Sorted_Object::
Feature_Object_Matches_Sorted_Object( const Feature_Object_Matches_Sorted_Object& other ) : Feature( other ) {

}

/**
 * Feature_Object_Matches_Sorted_Object class destructor
 */
Feature_Object_Matches_Sorted_Object::
~Feature_Object_Matches_Sorted_Object() {

}

/** 
 * Feature_Object_Matches_Sorted_Object class assignment operator
 */
Feature_Object_Matches_Sorted_Object&
Feature_Object_Matches_Sorted_Object::
operator=( const Feature_Object_Matches_Sorted_Object& other ){
  _invert = other._invert;
  _string_properties = other._string_properties;
  _int_properties = other._int_properties;
  return (*this);
}

/**
 * returns the value of the feature
 */
bool
Feature_Object_Matches_Sorted_Object::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}


/**
 * returns the value of the feature
 */
bool
Feature_Object_Matches_Sorted_Object::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Object* object = dynamic_cast< const Object* >( grounding );
  if( object != NULL ){
    map< string, map< string, vector< Object* > > >::const_iterator it_sorted_objects_map = world->sorted_objects().find( sorting_key() );
    if( it_sorted_objects_map == world->sorted_objects().end() ){
      cout << "could not find sorting index \"" << sorting_key() << "\"" << endl;
      assert( false );
    }
    //assert( it_sorted_objects_map != world->sorted_objects().end() );
    map< string, vector< Object* > >::const_iterator it_sorted_objects = it_sorted_objects_map->second.find( object->type() );
    //assert( it_sorted_objects != it_sorted_objects_map->second.end() );
    if( it_sorted_objects != it_sorted_objects_map->second.end() ){
      if( index() < ( int )( it_sorted_objects->second.size() ) ){
        if( object->id() == it_sorted_objects->second[ index() ]->id() ){
          return !_invert;
        }
        return _invert; 
      }
    }
  }
  return false;
}

/** 
 * imports the Feature_Object_Matches_Sorted_Object class from an XML file
 */
void
Feature_Object_Matches_Sorted_Object::
from_xml( const string& filename ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_object_matches_sorted_object" ) ) == 0 ){
            from_xml( l1 );
          }
        }
      }
      xmlFreeDoc( doc );
    }
  }
  return;
}


/** 
 * imports the Feature_Object_Matches_Sorted_Object class from an XML node pointer
 */
void
Feature_Object_Matches_Sorted_Object::
from_xml( xmlNodePtr root ){
  _invert = false;
  index() = 0;
  sorting_key() = "na";
  if( root->type == XML_ELEMENT_NODE ){
    vector< string > feature_keys = { "invert", "index", "sorting_key" };
    assert( check_keys( root, feature_keys ) );

    pair< bool, bool > invert_prop = has_prop< bool >( root, "invert" );
    if( invert_prop.first ) {
      _invert = invert_prop.second;
    }

    pair< bool, int > index_prop = has_prop< int >( root, "index" );
    if( index_prop.first ) {
      index() = index_prop.second;
    }

    pair< bool, std::string > sorting_key_prop = has_prop< std::string >( root, "sorting_key" );
    if( sorting_key_prop.first ) {
      sorting_key() = sorting_key_prop.second;
    }
  }
}

/**
 * exports the Feature_Object_Matches_Sorted_Object class to an XML file
 */
void
Feature_Object_Matches_Sorted_Object::
to_xml( const string& filename )const{
  xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( doc, root );
  to_xml( doc, root );
  xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
  xmlFreeDoc( doc );
  return;
}

/**
 * exports the Feature_Object_Matches_Sorted_Object class to an XML node pointer
 */
void
Feature_Object_Matches_Sorted_Object::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_matches_sorted_object" ), NULL );
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( to_std_string( _invert ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "index" ), ( const xmlChar* )( to_std_string( index() ).c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "sorting_key" ), ( const xmlChar* )( sorting_key().c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Feature_Object_Matches_Sorted_Object class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Object_Matches_Sorted_Object& other ){
    out << "Feature_Object_Matches_Sorted_Object:( invert:(" << other.invert() << ") index:(" << other.index() << ") sorting_key:(" << other.sorting_key() << "))";
    return out;
  }
}


