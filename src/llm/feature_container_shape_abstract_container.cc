/**
 * @file feature_container_shape_abstract_container.cc
*
 * @brief
 *
 * Class used to describe a feature of the container number
 */
#include <assert.h>
#include <sstream>
#include <math.h>
#include <cmath>

#include "h2sl/feature_container_shape_abstract_container.h"
#include "h2sl/abstract_container.h"
#include "h2sl/object.h"
#include "h2sl/container.h"
#include "h2sl/container_type.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Shape_Abstract_Container class constructor
 */
Feature_Container_Shape_Abstract_Container::
Feature_Container_Shape_Abstract_Container( const bool& invert,
                          const std::string& containerType ) : Feature( invert ),
                                                                          _rho( 0.25 ),
                                                                          _theta( 1.20 ),
									  _container_type( containerType ),
                                                                          _max_object_count( 0 ),
                                                                          _direction( false ),
                                                                          _is_a_container( false ){
}

Feature_Container_Shape_Abstract_Container::
Feature_Container_Shape_Abstract_Container( xmlNodePtr root ) : Feature(),
                                                _rho( 0.25 ),
                                                _theta( 1.20 ),
                                                _container_type( "na" ),
                                                _max_object_count( 0 ),
                                                _direction( false ),
                                                _is_a_container( false ){
  from_xml( root );
}

/**
 * Feature_Container_Shape_Abstract_Container class copy constructor
 */
Feature_Container_Shape_Abstract_Container::
Feature_Container_Shape_Abstract_Container( const Feature_Container_Shape_Abstract_Container& other ) : Feature( other ),
                                                              _rho( other._rho ),
                                                              _theta( other._theta ),
							      _container_type( other._container_type ),
                                                              _max_object_count( other._max_object_count ),
                                                              _direction( other._direction ),
                                                              _is_a_container( other._is_a_container ){

}

/**
 * Feature_Container_Shape_Abstract_Container class destructor
 */
Feature_Container_Shape_Abstract_Container::
~Feature_Container_Shape_Abstract_Container() {

}

/** 
 * Feature_Container_Shape_Abstract_Container class assignment operator
 */
Feature_Container_Shape_Abstract_Container&
Feature_Container_Shape_Abstract_Container::
operator=( const Feature_Container_Shape_Abstract_Container& other ){
  _invert = other._invert;
  _container_type= other._container_type;
  return (*this);
}

/**
 * returns the value of the fature
 */
bool
Feature_Container_Shape_Abstract_Container::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Container_Shape_Abstract_Container::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Container* container_grounding = dynamic_cast< const Container* >( grounding );
  if( container_grounding != NULL ){
   
  Container_Type* container_type_child = NULL;
  for( unsigned int i = 0; i < children.size(); i++ ){
    for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
      if( dynamic_cast< Container_Type* >( children[ i ].second[ j ] ) != NULL ){
        container_type_child = static_cast< Container_Type* >( children[ i ].second[ j ] );
      }
    }
  }

  Abstract_Container* abstract_container_child = NULL;
  for( unsigned int i = 0; i < children.size(); i++ ){
    for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
      if( dynamic_cast< Abstract_Container* >( children[ i ].second[ j ] ) != NULL ){
        abstract_container_child = static_cast< Abstract_Container* >( children[ i ].second[ j ] );
      } 
    }
  } 
 
  if( ( container_type_child != NULL ) && ( abstract_container_child != NULL ) ){
    if( ( container_grounding != NULL ) && ( container_type_child->type() == _container_type ) ){
        vector< Object* > objects;
        for( unsigned int i = 0; i < container_grounding->container().size(); i++ ){
          Object * container_object = dynamic_cast< Object* >( container_grounding->container()[ i ] );
          if( container_object != NULL ){
            if( container_object->type() == abstract_container_child->type() ){
              objects.push_back( container_object );
            }
          }
        }

        if( _container_type == "row" ){
          if( in_a_row( objects, abstract_container_child->number(), 0 ) ){
            return !_invert;
          } else{
            return _invert;
          }
        } else if ( _container_type == "column" ) { 
          if( in_a_column( objects, abstract_container_child->number(), 0 ) ){
            return !_invert;
          } else{
            return _invert;
          }
        }
    }
  }
  }
  return false;
}

/**
 * converts (x,y) to (rho, theta)
 */
pair< double, double >
Feature_Container_Shape_Abstract_Container::
convert_to_rho_theta(double x, double y){
  pair < double, double > RT;
  RT.first = sqrt( x*x + y*y );
  RT.second = atan( y / x );
  return RT;
}

/**
 * returns true if in a row of size n
 */
bool
Feature_Container_Shape_Abstract_Container::
in_a_row( vector< Object* >& all_objects, unsigned int container_size, unsigned int counter  ){

  vector< Object* > selected_objects;

  // find objects along a x-plane
  for( unsigned int i = 0; i < all_objects.size(); i++ ){
    bool is_within_x = false;
    for( unsigned int j = 0; j < all_objects.size(); j++ ){
      if( i != j ){
        if( fabs( all_objects[ j ]->transform().position().x() - all_objects[ i ]->transform().position().x() ) < 0.1 ){
          is_within_x = true;
        }
      }
    }
    if( is_within_x ){
      selected_objects.push_back( all_objects[ i ] );
    }
  }
 
  // remove objects far enough away from other objects
  for( unsigned int i = 0; i < selected_objects.size(); i++ ){
    bool is_close = false;
    for( unsigned int j = 0; j < selected_objects.size(); j++ ){
      if( i != j ){
        if( Vector3::distance( selected_objects[ i ]->transform().position(), selected_objects[ j ]->transform().position() ) < 0.25 ){
          is_close = true;
        }
      }
    }
    if( !is_close ){
      selected_objects.erase( selected_objects.begin() + i );
    }
  }
 
  if( selected_objects.size() == container_size ){
    return true;
  } else {
    return false;
  }
}

/**
 * returns true if in a row of size n
 */
bool
Feature_Container_Shape_Abstract_Container::
in_a_column( vector< Object* >& all_objects, unsigned int container_size, unsigned int counter  ){
  vector< Object* > selected_objects;

  // find objects along a y-plane
  for( unsigned int i = 0; i < all_objects.size(); i++ ){
    bool is_within_x = false;
    for( unsigned int j = 0; j < all_objects.size(); j++ ){
      if( i != j ){
        if( fabs( all_objects[ j ]->transform().position().y() - all_objects[ i ]->transform().position().y() ) < 0.1 ){
          is_within_x = true;
        }
      }
    } 
    if( is_within_x ){
      selected_objects.push_back( all_objects[ i ] );
    }
  }

  // remove objects far enough away from other objects
  for( unsigned int i = 0; i < selected_objects.size(); i++ ){
    bool is_close = false;
    for( unsigned int j = 0; j < selected_objects.size(); j++ ){
      if( i != j ){
        if( Vector3::distance( selected_objects[ i ]->transform().position(), selected_objects[ j ]->transform().position() ) < 0.25 ){
          is_close = true;
        }
      }
    }
    if( !is_close ){
      selected_objects.erase( selected_objects.begin() + i );
    }
  }

  if( selected_objects.size() == container_size ){
    return true;
  } else {
    return false;
  }
}

/**
 * returns true if in a tower of size n
 */
bool
Feature_Container_Shape_Abstract_Container::
in_a_tower(){
  return false;
}

/** 
 * imports the Feature_Container_Shape_Abstract_Container class from an XML file
 */
void
Feature_Container_Shape_Abstract_Container::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_container_shape_abstract_container" ) ) == 0 ){
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
 * imports the Feature_Container_Shape_Abstract_Container class from an XML node pointer
 */
void
Feature_Container_Shape_Abstract_Container::
from_xml( xmlNodePtr root ){
  _invert = false;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool ) ( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( "container_type" ) );
    if( tmp != NULL){
      _container_type = ( const char* )( tmp );
      xmlFree( tmp );
    }
  }
}

/**
 * exports the Feature_Container_Shape_Abstract_Container class to an XML file
 */
void
Feature_Container_Shape_Abstract_Container::
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
 * exports the Feature_Container_Shape_Abstract_Container class to an XML node pointer
 */
void
Feature_Container_Shape_Abstract_Container::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_shape_abstract_container" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "container_type" ), ( const xmlChar* )( _container_type.c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_Container_Shape_Abstract_Container::
set_max_object_count( unsigned int x ){
  _max_object_count = x;
}

unsigned int&
Feature_Container_Shape_Abstract_Container::
max_object_count( void ){
  return _max_object_count;
}

const unsigned int&
Feature_Container_Shape_Abstract_Container::
max_object_count( void )const{
  return _max_object_count;
}

string&
Feature_Container_Shape_Abstract_Container::
container_type( void ){
  return _container_type;
}

const string&
Feature_Container_Shape_Abstract_Container::
container_type( void )const{
  return _container_type;
}

void
Feature_Container_Shape_Abstract_Container::
set_is_a_container( bool x ){
  _is_a_container = x;
}

bool&
Feature_Container_Shape_Abstract_Container::
is_a_container( void ){
  return _is_a_container;
}

const bool&
Feature_Container_Shape_Abstract_Container::
is_a_container( void )const{
  return _is_a_container;
}

void
Feature_Container_Shape_Abstract_Container::
set_direction( bool x ){
  _direction = x;
}

bool&
Feature_Container_Shape_Abstract_Container::
direction( void ){
  return _direction;
}

const bool&
Feature_Container_Shape_Abstract_Container::
direction( void )const{
  return _direction;
}

namespace h2sl {
  /** 
   * Feature_Container_Shape_Abstract_Container class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Container_Shape_Abstract_Container& other ){
    out << "class:\"Feature_Container_Shape_Abstract_Container\" ";
    out << "container_type:" << other.container_type() << "\" ";
    return out;
  }
}
