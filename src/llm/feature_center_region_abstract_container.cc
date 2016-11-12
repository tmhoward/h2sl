/**
 * @file feature_center_region_abstract_container.cc
 *
 * @brief
 *
 * Class used to describe a feature of the center_region_abstract_container number
 */
#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/spatial_relation.h"
#include "h2sl/feature_center_region_abstract_container.h"
#include "h2sl/region_abstract_container.h"
#include "h2sl/region_container.h"

#include "h2sl/vector3.h"

using namespace std;
using namespace h2sl;

/**
 * Function for sorting (with a reference position as an argument) 
 */
bool 
center_sort_region_abstract_container( Object* a, Object* b, h2sl::Vector3& centroid_position ){
  return h2sl::Vector3::distance( a->transform().position(), centroid_position ) < h2sl::Vector3::distance( b->transform().position(), centroid_position);
}

class sorter {
	h2sl::Vector3& _pos;
public:
	sorter ( h2sl::Vector3& pos) : _pos( pos ) {}
	bool operator()( Object* a, Object* b ) const {
		return center_sort_region_abstract_container( a, b, _pos );
	}
};


/**
 * Feature_Center_Region_Abstract_Container class constructor
 */
Feature_Center_Region_Abstract_Container::
Feature_Center_Region_Abstract_Container( const bool& invert,
                                         const std::string& relation_type ) : Feature( invert ),
                                                                          _relation_type( relation_type ) {

}

/**
 * Feature_Center_Region_Abstract_Container class copy constructor
 */
Feature_Center_Region_Abstract_Container::
Feature_Center_Region_Abstract_Container( const Feature_Center_Region_Abstract_Container& other ) : Feature( other ),
                                                              _relation_type( other._relation_type ) {

}

/**
 * Feature_Center_Region_Abstract_Container class destructor
 */
Feature_Center_Region_Abstract_Container::
~Feature_Center_Region_Abstract_Container() {

}

/** 
 * Feature_Center_Region_Abstract_Container class assignment operator
 */
Feature_Center_Region_Abstract_Container&
Feature_Center_Region_Abstract_Container::
operator=( const Feature_Center_Region_Abstract_Container& other ){
  _invert = other._invert;
  _relation_type = other._relation_type;
  return (*this);
}

/**
 * returns the value of the feature
 */

bool
Feature_Center_Region_Abstract_Container::
value( const unsigned int& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world) {
    return value( cv, grounding, children, phrase, world, NULL );
    
}

bool
Feature_Center_Region_Abstract_Container::
value( const unsigned int& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context ){
    
}

/*bool
Feature_Center_Region_Abstract_Container::
value( const unsigned int& cv,
        const h2sl::Grounding* grounding,
        const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
        const h2sl::Phrase* phrase,
        const World* world ){
  Region_Abstract_Container* region_abstract_container_child = NULL;
  Container* container_child = NULL;
  const Object* object_grounding = dynamic_cast< const Object* >( grounding );

  for( unsigned int i = 0; i < children.size(); i++ ){
    for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
      if( dynamic_cast< Region_Abstract_Container* >( children[ i ].second[ j ] ) != NULL ){
        region_abstract_container_child = static_cast< Region_Abstract_Container* >( children[ i ].second[ j ] );
      } else if ( dynamic_cast< Container* >( children[ i ].second[ j ] ) != NULL ){
        container_child = static_cast< Container* >( children[ i ].second[ j ] );
      }
    }
  }
 
  
  if( region_abstract_container_child != NULL ){
    if( region_abstract_container_child->type() == _relation_type ){
      if( container_child != NULL) {    
        if( object_grounding != NULL ){
   	  vector< Object* > objects_container = container_child->container();
	  vector< Object* > objects_match_type;
	  for ( unsigned int i = 0; i < objects_container.size(); i++ ) {
            if ( region_abstract_container_child->abstract_container().type() == objects_container[ i ]->type() ){
              objects_match_type.push_back( objects_container[ i ] );
            } 
          }
	   	
	  double centroid_x = 0, centroid_y = 0, centroid_z = 0;
	  for ( unsigned int i = 0; i < objects_match_type.size(); i++ ) {
	    centroid_x += objects_match_type[ i ]->transform().position().x();
	    centroid_y += objects_match_type[ i ]->transform().position().y();
	    centroid_z += objects_match_type[ i ]->transform().position().z();
          }
	  centroid_x = centroid_x / objects_match_type.size();	
	  centroid_y = centroid_y / objects_match_type.size();	
	  centroid_z = centroid_z / objects_match_type.size();	
	  h2sl::Vector3 centroid_pos( centroid_x, centroid_y, centroid_z );
  	  
	  sort( objects_match_type.begin(), objects_match_type.end(), sorter( centroid_pos ) );
	
          for( unsigned int i = 0; i < objects_match_type.size(); i++ ){	
      	    if ( *object_grounding == *objects_match_type[ i ] ){
              if( i < region_abstract_container_child->abstract_container().num() ){
                     return !_invert;
                } else {
                  return _invert;
                } 
              } else {
               return _invert;
              }
          }
        }
      }
    }
  }
  return false;
}*/

/** 
 * imports the Feature_Center_Region_Abstract_Container class from an XML file
 */
void
Feature_Center_Region_Abstract_Container::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_center_region_abstract_container" ) ) == 0 ){
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
 * imports the Feature_Center_Region_Abstract_Container class from an XML node pointer
 */
void
Feature_Center_Region_Abstract_Container::
from_xml( xmlNodePtr root ){
  _invert = false;
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
 * exports the Feature_Center_Region_Abstract_Container class to an XML file
 */
void
Feature_Center_Region_Abstract_Container::
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
 * exports the Feature_Center_Region_Abstract_Container class to an XML node pointer
 */
void
Feature_Center_Region_Abstract_Container::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_center_region_abstract_container" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "relation_type" ), ( const xmlChar* )( _relation_type.c_str() ) );
  xmlAddChild( root, node );
  return;
}

std::string&
Feature_Center_Region_Abstract_Container::
relation_type( void ){
  return _relation_type;
}

const std::string&
Feature_Center_Region_Abstract_Container::
relation_type( void )const{
  return _relation_type;
}

namespace h2sl {
  /** 
   * Feature_Center_Region_Abstract_Container class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Center_Region_Abstract_Container& other ){
    out << "class:\"Abstract_Container\" "; 
    out << "relation_type:\"" << other.relation_type() << "\" ";
    return out;
  }
}
