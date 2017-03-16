/**
 * @file feature_min_distance_container.cc
 *
 * @brief
 *
 * Class used to describe a feature of the container number
 */
#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/spatial_relation.h"
#include "h2sl/feature_min_distance_container.h"
#include "h2sl/container.h"

using namespace std;
using namespace h2sl;

bool 
min_distance_sort_container( Grounding* a, Grounding* b ){
 if ((dynamic_cast< const Object* >(a) != NULL) && (dynamic_cast< const Object* >(b) != NULL)) {
   return ( ((dynamic_cast< const Object* >(a))->transform().position().norm()) <
          ((dynamic_cast< const Object* >(b))->transform().position().norm()) );
  } else {
    cout << "Cannot cast container entity into Object" << endl;
    return false;
  }
}

/**
 * Feature_Min_Distance_Container class constructor
 */
Feature_Min_Distance_Container::
Feature_Min_Distance_Container( const bool& invert,
                          const std::string& relation_type ) : Feature( invert ),
                                                                          _relation_type( relation_type ) {

}

/**
 * Feature_Min_Distance_Container class copy constructor
 */
Feature_Min_Distance_Container::
Feature_Min_Distance_Container( const Feature_Min_Distance_Container& other ) : Feature( other ),
                                                              _relation_type( other._relation_type ) {

}

/**
 * Feature_Min_Distance_Container class destructor
 */
Feature_Min_Distance_Container::
~Feature_Min_Distance_Container() {

}

/** 
 * Feature_Min_Distance_Container class assignment operator
 */
Feature_Min_Distance_Container&
Feature_Min_Distance_Container::
operator=( const Feature_Min_Distance_Container& other ){
  _invert = other._invert;
  _relation_type = other._relation_type;
  return (*this);
}

/**
 * returns the value of the feature
 */
bool
Feature_Min_Distance_Container::
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
Feature_Min_Distance_Container::
value( const string& cv,
        const h2sl::Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ) {
  const Object* object_grounding = dynamic_cast< const Object* >( grounding );
  if( object_grounding != NULL ){
    Container* container_child = NULL;
    Spatial_Relation* spatial_relation_child = NULL;

    for( unsigned int i = 0; i < children.size(); i++ ){
      for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
        if( dynamic_cast< Container* >( children[ i ].second[ j ] ) != NULL ){
          container_child = static_cast< Container* >( children[ i ].second[ j ] );
        } else if ( dynamic_cast< Spatial_Relation* >( children[ i ].second[ j ] ) != NULL ){
          spatial_relation_child = static_cast< Spatial_Relation* >( children[ i ].second[ j ] );
        } 
      }
    } 
 
    if (container_child != NULL) {
      if (spatial_relation_child != NULL ) {
        if (spatial_relation_child->relation_type() == _relation_type ) {
          if( !container_child->container().empty() ){
            if (dynamic_cast< const Object* >(container_child->container().front()) != NULL) {
              if (!container_child->container().empty()) {
                vector< Grounding* > container_child_objects = container_child->container();
                sort( container_child_objects.begin(), container_child_objects.end(), min_distance_sort_container );
                if( *object_grounding == *dynamic_cast< const Object* >(container_child_objects.front()) ){
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
    
    
 /*   if( container_child != NULL ){
        if( spatial_relation_child != NULL ){
            if( spatial_relation_child->type() == _relation_type ){
                if( object_grounding != NULL ){
                    if( !container_child->container().empty() ){
                        vector< Object* > objects = container_child->container();
                        sort( objects.begin(), objects.end(), min_distance_sort_container );
                        if( *object_grounding == *objects[ 0 ] ){
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
  */
}

/** 
 * imports the Feature_Min_Distance_Container class from an XML file
 */
void
Feature_Min_Distance_Container::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_min_distance_container" ) ) == 0 ){
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
 * imports the Feature_Min_Distance_Container class from an XML node pointer
 */
void
Feature_Min_Distance_Container::
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
 * exports the Feature_Min_Distance_Container class to an XML file
 */
void
Feature_Min_Distance_Container::
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
 * exports the Feature_Min_Distance_Container class to an XML node pointer
 */
void
Feature_Min_Distance_Container::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_min_distance_container" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "relation_type" ), ( const xmlChar* )( _relation_type.c_str() ) );
  xmlAddChild( root, node );
  return;
}

std::string&
Feature_Min_Distance_Container::
relation_type( void ){
  return _relation_type;
}

const std::string&
Feature_Min_Distance_Container::
relation_type( void )const{
  return _relation_type;
}

namespace h2sl {
  /** 
   * Feature_Min_Distance_Container class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Min_Distance_Container& other ){
    out << "class:\"Feature_Min_Distance__Container\" ";
    out << "relation_type:\"" << other.relation_type() << "\" ";
    return out;
  }
}
