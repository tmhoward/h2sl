/**
 * @file feature_min_abs_y_abstract_container.cc
 *
 * @brief
 *
 * Class used to describe a feature of the abstract_container number
 */
#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/world.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/feature_min_abs_y_abstract_container.h"
#include "h2sl/abstract_container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Min_Abs_Y_Abstract_Container class constructor
 */
Feature_Min_Abs_Y_Abstract_Container::
Feature_Min_Abs_Y_Abstract_Container( const bool& invert,
                          const std::string& relation_type ) : Feature( invert ),
                                                                          _relation_type( relation_type ) {

}

/**
 * Feature_Min_Abs_Y_Abstract_Container class copy constructor
 */
Feature_Min_Abs_Y_Abstract_Container::
Feature_Min_Abs_Y_Abstract_Container( const Feature_Min_Abs_Y_Abstract_Container& other ) : Feature( other ),
                                                              _relation_type( other._relation_type ) {

}

/**
 * Feature_Min_Abs_Y_Abstract_Container class destructor
 */
Feature_Min_Abs_Y_Abstract_Container::
~Feature_Min_Abs_Y_Abstract_Container() {

}

/** 
 * Feature_Min_Abs_Y_Abstract_Container class assignment operator
 */
Feature_Min_Abs_Y_Abstract_Container&
Feature_Min_Abs_Y_Abstract_Container::
operator=( const Feature_Min_Abs_Y_Abstract_Container& other ){
  _invert = other._invert;
  _relation_type = other._relation_type;
  return (*this);
}

/**
 * returns the value of the feature
 */
bool
Feature_Min_Abs_Y_Abstract_Container::
value( const string& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

/**
 * returns the value of the fature
 */
bool
Feature_Min_Abs_Y_Abstract_Container::
value( const string& cv,
        const h2sl::Grounding* grounding,
        const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
        const h2sl::Phrase* phrase,
        const World* world,
        const Grounding* context ) {
  const Object* object_grounding = dynamic_cast< const Object* >( grounding );
  if( object_grounding != NULL ){
    Abstract_Container* abstract_container_child = NULL;
    Spatial_Relation* spatial_relation_child = NULL;
    for( unsigned int i = 0; i < children.size(); i++ ){
      for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
        if( dynamic_cast< Abstract_Container* >( children[ i ].second[ j ] ) != NULL ){
          abstract_container_child = static_cast< Abstract_Container* >( children[ i ].second[ j ] );
        } else if ( dynamic_cast< Spatial_Relation* >( children[ i ].second[ j ] ) != NULL ){
          spatial_relation_child = static_cast< Spatial_Relation* >( children[ i ].second[ j ] );
        } 
      }
    }  
 
    if( abstract_container_child != NULL ){
      if( spatial_relation_child != NULL ){
        if( spatial_relation_child->relation_type() == _relation_type ){
          map< string, vector< Object* > >::const_iterator it;
          it  = world->min_abs_y_sorted_objects().find( abstract_container_child->type() );
          if (it != world->min_abs_y_sorted_objects().end() ) {
            for( unsigned int i = 0; i < it->second.size(); i++ ){
              if( *object_grounding == *(it->second[ i ]) ){
//                map< string, unsigned int >::const_iterator itnum = world->numeric_map().find( abstract_container_child->number() );
//                assert( itnum != world->numeric_map().end() );
//                if( i < itnum->second ){
                if( i < abstract_container_child->number() ){
                  return !_invert;
                } else {
                  return _invert;
                }
              }
            }
          }
        }
      }
    }
  }
  return false;
}

/** 
 * imports the Feature_Min_Abs_Y_Abstract_Container class from an XML file
 */
void
Feature_Min_Abs_Y_Abstract_Container::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_min_abs_y_abstract_container" ) ) == 0 ){
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
 * imports the Feature_Min_Abs_Y_Abstract_Container class from an XML node pointer
 */
void
Feature_Min_Abs_Y_Abstract_Container::
from_xml( xmlNodePtr root ){
  _invert = false;
  _relation_type = "na";
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool ) ( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "relation_type" ) );
    if( tmp != NULL){
      string relation_type_string = ( const char* )( tmp );
       _relation_type = relation_type_string;
      xmlFree( tmp );
    }
  }
}

/**
 * exports the Feature_Min_Abs_Y_Abstract_Container class to an XML file
 */
void
Feature_Min_Abs_Y_Abstract_Container::
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
 * exports the Feature_Min_Abs_Y_Abstract_Container class to an XML node pointer
 */
void
Feature_Min_Abs_Y_Abstract_Container::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_min_abs_y_abstract_container" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "relation_type" ), ( const xmlChar* )( _relation_type.c_str() ) );
  xmlAddChild( root, node );
  return;
}

std::string&
Feature_Min_Abs_Y_Abstract_Container::
relation_type( void ){
  return _relation_type;
}

const std::string&
Feature_Min_Abs_Y_Abstract_Container::
relation_type( void )const{
  return _relation_type;
}

namespace h2sl {
  /** 
   * Feature_Min_Abs_Y_Abstract_Container class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Min_Abs_Y_Abstract_Container& other ){
    out << "class:\"Feature_Min_Abs_Y_Abstract_Container\" ";
    out << "relation_type:\"" << other.relation_type() << "\" ";
    return out;
  }
}
