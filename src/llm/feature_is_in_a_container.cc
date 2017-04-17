/**
 * @file feature_is_in_a_container.cc
*
 * @brief
 *
 * Class used to describe a feature of the container number
 */
#include <assert.h>
#include <sstream>
#include <math.h>
#include <cmath>

#include "h2sl/feature_is_in_a_container.h"
#include "h2sl/abstract_container.h"
#include "h2sl/object.h"
#include "h2sl/container.h"
#include "h2sl/container_type.h"

using namespace std;
using namespace h2sl;

bool 
min_x_sort_is_in_a_container( Object* a,
        Object* b ){
  return a->transform().position().x() < b->transform().position().x();
}

/**
 * Feature_Is_In_A_Container class constructor
 */
Feature_Is_In_A_Container::
Feature_Is_In_A_Container( const bool& invert,
                          const std::string& containerType ) : Feature( invert ),
                                                                          _rho( 0.25 ),
                                                                          _theta( 1.20 ),
									  _container_type( containerType ),
                                                                          _max_object_count( 0 ),
                                                                          _direction( false ),
                                                                          _is_a_container( false ){
}

Feature_Is_In_A_Container::
Feature_Is_In_A_Container( xmlNodePtr root ) : Feature(),
                                                _rho( 0.25 ),
                                                _theta( 1.20 ),
                                                _container_type( "na" ),
                                                _max_object_count( 0 ),
                                                _direction( false ),
                                                _is_a_container( false ){
  from_xml( root );
}

/**
 * Feature_Is_In_A_Container class copy constructor
 */
Feature_Is_In_A_Container::
Feature_Is_In_A_Container( const Feature_Is_In_A_Container& other ) : Feature( other ),
                                                              _rho( other._rho ),
                                                              _theta( other._theta ),
							      _container_type( other._container_type ),
                                                              _max_object_count( other._max_object_count ),
                                                              _direction( other._direction ),
                                                              _is_a_container( other._is_a_container ){

}

/**
 * Feature_Is_In_A_Container class destructor
 */
Feature_Is_In_A_Container::
~Feature_Is_In_A_Container() {

}

/** 
 * Feature_Is_In_A_Container class assignment operator
 */
Feature_Is_In_A_Container&
Feature_Is_In_A_Container::
operator=( const Feature_Is_In_A_Container& other ){
  _invert = other._invert;
  _container_type= other._container_type;
  return (*this);
}

/**
 * returns the value of the fature
 */
bool
Feature_Is_In_A_Container::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Is_In_A_Container::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Object* object_grounding = dynamic_cast< const Object* >( grounding );
  if( object_grounding != NULL ){
   
  Container_Type* container_type_child = NULL;
  for( unsigned int i = 0; i < children.size(); i++ ){
    for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
      if( dynamic_cast< Container_Type* >( children[ i ].second[ j ] ) != NULL ){
        container_type_child = static_cast< Container_Type* >( children[ i ].second[ j ] );
      }
    }
  }

  Container* container_child = NULL;
  for( unsigned int i = 0; i < children.size(); i++ ){
    for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
      if( dynamic_cast< Container* >( children[ i ].second[ j ] ) != NULL ){
        container_child = static_cast< Container* >( children[ i ].second[ j ] );
      }
    }
  }

/*  
  Abstract_Container* abstract_container_child = NULL;
  for( unsigned int i = 0; i < children.size(); i++ ){
    for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
      if( dynamic_cast< Abstract_Container* >( children[ i ].second[ j ] ) != NULL ){
        abstract_container_child = static_cast< Abstract_Container* >( children[ i ].second[ j ] );
      } 
    }
  } 
*/ 
//  if( abstract_container_child != NULL ){
  if( ( container_type_child != NULL ) && ( container_child != NULL ) ){
    if( ( object_grounding != NULL ) && ( container_type_child->type() == _container_type ) ){
        vector< Object* > objects;
        for( unsigned int i = 0; i < container_child->container().size(); i++ ){
          if( dynamic_cast< Object* >( container_child->container()[ i ] ) != NULL ){
            objects.push_back( static_cast< Object* >( container_child->container()[ i ] ) );
          }
        }

        if( _container_type == "row" ){
          if( in_a_row( object_grounding, objects, object_grounding, container_child->container().size(), 0 ) ){
            return !_invert;
          } else{
            return _invert;
          }
        } else if ( _container_type == "column" ) { 
          if( in_a_column( object_grounding, objects, object_grounding, container_child->container().size(), 0 ) ){
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
Feature_Is_In_A_Container::
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
Feature_Is_In_A_Container::
in_a_row( const Object* local_root_object, vector< Object* > all_objects, const Object* global_root_object, unsigned int container_size, unsigned int counter  ){
  if( ( counter + 1 ) == container_size ){
    return true;
  } else if( _max_object_count < counter + 1 ){
    _max_object_count = counter + 1;
  }
  
  //determine which objects are inside the admissible region and add them to node_objects; only include objects with y > root object's y
  pair< double, double > rho_theta;
  vector< Object* > node_objects;
  for( unsigned int i = 0; i < all_objects.size(); i++ ){
    if( all_objects[ i ]->id() != local_root_object->id() ){
      rho_theta = convert_to_rho_theta( ( all_objects[ i ]->transform().position().y() - local_root_object->transform().position().y() ), ( all_objects[ i ]->transform().position().x() - local_root_object->transform().position().x() ) );
      if( ( rho_theta.first <= _rho ) && ( abs( rho_theta.second ) <= _theta ) ){
        if( !direction() ){
          if( local_root_object->transform().position().y() > all_objects[ i ]->transform().position().y() ){
            //cout << "local root object: " << *local_root_object << endl;
            //cout << "checking object( " << *all_objects[ i ] << " ):" << endl;
            //cout << " rho: " << rho_theta.first << endl;
            //cout << " theta: " << rho_theta.second << endl;
            node_objects.push_back( all_objects[ i ] );
          }
        } else{
          if( local_root_object->transform().position().y() < all_objects[ i ]->transform().position().y() ){
            //cout << "local root object: " << *local_root_object << endl;
            //cout << "checking object( " << *all_objects[ i ] << " ):" << endl;
            //cout << " rho: " << rho_theta.first << endl;
            //cout << " theta: " << rho_theta.second << endl;
            node_objects.push_back( all_objects[ i ] );
          }
        }
      } 
    }
  }
  
  //check if node_objects is empty; if yes, check the current direction; if direction is false, change direction and call in_a_row, else false
  if( !node_objects.empty() ){
    for( unsigned int i = 0; i < node_objects.size(); i++ ){
      set_is_a_container( in_a_row( node_objects[ i ], all_objects, global_root_object, container_size, counter + 1 ) );
      if( is_a_container() ){
        //cout << node_objects[ i ]->id() << endl;
        return true;
      } else{ 
        return false; 
      }
    }
  } else if( !direction() ){
    set_direction( true );
    set_is_a_container( in_a_row( global_root_object, all_objects, global_root_object, container_size, counter ) );
    if( is_a_container() ){
      return true;
    } else{  
      return false;
    }
  }
  return false; // node_objects was empty and direction was true 
}

/**
 * returns true if in a row of size n
 */
bool
Feature_Is_In_A_Container::
in_a_column( const Object* local_root_object, vector< Object* > all_objects, const Object* global_root_object, unsigned int container_size, unsigned int counter  ){
  if( ( counter + 1 ) == container_size ){
    return true;
  } else if( _max_object_count < counter + 1 ){
    _max_object_count = counter + 1;
  }

  //determine which objects are inside the admissible region and add them to node_objects; only include objects with y > root object's y
  pair< double, double > rho_theta;
  vector< Object* > node_objects;
  for( unsigned int i = 0; i < all_objects.size(); i++ ){
    if( all_objects[ i ]->id() != local_root_object->id() ){
      rho_theta = convert_to_rho_theta( ( all_objects[ i ]->transform().position().x() - local_root_object->transform().position().x() ), ( all_objects[ i ]->transform().position().y() - local_root_object->transform().position().y() ) );
      if( ( rho_theta.first <= _rho ) && ( abs( rho_theta.second ) <= _theta ) ){
        if( !direction() ){
          if( local_root_object->transform().position().y() > all_objects[ i ]->transform().position().y() ){
            //cout << "local root object: " << *local_root_object << endl;
            //cout << "checking object( " << *all_objects[ i ] << " ):" << endl;
            //cout << " rho: " << rho_theta.first << endl;
            //cout << " theta: " << rho_theta.second << endl;
            node_objects.push_back( all_objects[ i ] );
          }
        } else{
          if( local_root_object->transform().position().y() < all_objects[ i ]->transform().position().y() ){
            //cout << "local root object: " << *local_root_object << endl;
            //cout << "checking object( " << *all_objects[ i ] << " ):" << endl;
            //cout << " rho: " << rho_theta.first << endl;
            //cout << " theta: " << rho_theta.second << endl;
            node_objects.push_back( all_objects[ i ] );
          }
        }
      }
    }
  }

  //check if node_objects is empty; if yes, check the current direction; if direction is false, change direction and call in_a_row, else false
  if( !node_objects.empty() ){
    for( unsigned int i = 0; i < node_objects.size(); i++ ){
      set_is_a_container( in_a_column( node_objects[ i ], all_objects, global_root_object, container_size, counter + 1 ) );
      if( is_a_container() ){
        //cout << node_objects[ i ]->id() << endl;
        return true;
      } else{
        return false;
      }
    }
  } else if( !direction() ){
    set_direction( true );
    set_is_a_container( in_a_column( global_root_object, all_objects, global_root_object, container_size, counter ) );
    if( is_a_container() ){
      return true;
    } else{
      return false;
    }
  }
  return false;
}

/**
 * returns true if in a tower of size n
 */
bool
Feature_Is_In_A_Container::
in_a_tower(){
  return false;
}

/** 
 * imports the Feature_Is_In_A_Container class from an XML file
 */
void
Feature_Is_In_A_Container::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_is_in_a_container" ) ) == 0 ){
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
 * imports the Feature_Is_In_A_Container class from an XML node pointer
 */
void
Feature_Is_In_A_Container::
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
 * exports the Feature_Is_In_A_Container class to an XML file
 */
void
Feature_Is_In_A_Container::
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
 * exports the Feature_Is_In_A_Container class to an XML node pointer
 */
void
Feature_Is_In_A_Container::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_is_in_a_container" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "container_type" ), ( const xmlChar* )( _container_type.c_str() ) );
  xmlAddChild( root, node );
  return;
}

void
Feature_Is_In_A_Container::
set_max_object_count( unsigned int x ){
  _max_object_count = x;
}

unsigned int&
Feature_Is_In_A_Container::
max_object_count( void ){
  return _max_object_count;
}

const unsigned int&
Feature_Is_In_A_Container::
max_object_count( void )const{
  return _max_object_count;
}

string&
Feature_Is_In_A_Container::
container_type( void ){
  return _container_type;
}

const string&
Feature_Is_In_A_Container::
container_type( void )const{
  return _container_type;
}

void
Feature_Is_In_A_Container::
set_is_a_container( bool x ){
  _is_a_container = x;
}

bool&
Feature_Is_In_A_Container::
is_a_container( void ){
  return _is_a_container;
}

const bool&
Feature_Is_In_A_Container::
is_a_container( void )const{
  return _is_a_container;
}

void
Feature_Is_In_A_Container::
set_direction( bool x ){
  _direction = x;
}

bool&
Feature_Is_In_A_Container::
direction( void ){
  return _direction;
}

const bool&
Feature_Is_In_A_Container::
direction( void )const{
  return _direction;
}

namespace h2sl {
  /** 
   * Feature_Is_In_A_Container class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Is_In_A_Container& other ){
    out << "class:\"Feature_Is_In_A_Container\" ";
    out << "container_type:" << other.container_type() << "\" ";
    return out;
  }
}
