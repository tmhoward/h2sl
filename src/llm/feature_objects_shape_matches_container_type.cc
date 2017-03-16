/**
 * @file feature_objects_shape_matches_container_type.cc
 *
 * @brief
 *
 * Class used to describe a feature of the container number
 */
#include <assert.h>
#include <algorithm>
#include <sstream>
#include <math.h>
#include <cmath>

#include "h2sl/feature_objects_shape_matches_container_type.h"
#include "h2sl/abstract_container.h"
#include "h2sl/object.h"
#include "h2sl/container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Objects_Shape_Matches_Container_Type class constructor
 */
Feature_Objects_Shape_Matches_Container_Type::
Feature_Objects_Shape_Matches_Container_Type( const bool& invert ) : 
							Feature( invert ),
                                                        _rho( 0.25 ),
                                                        _theta( 1.20 ){
}

/**
 * Feature_Objects_Shape_Matches_Container_Type class copy constructor
 */
Feature_Objects_Shape_Matches_Container_Type::
Feature_Objects_Shape_Matches_Container_Type( 
                                const Feature_Objects_Shape_Matches_Container_Type& other ) : 
                                Feature( other ),
                                _rho( other._rho ),
                                _theta( other._theta ){

}

/**
 * Feature_Objects_Shape_Matches_Container_Type class destructor
 */
Feature_Objects_Shape_Matches_Container_Type::
~Feature_Objects_Shape_Matches_Container_Type() {

}

/** 
 * Feature_Objects_Shape_Matches_Container_Type class assignment operator
 */
Feature_Objects_Shape_Matches_Container_Type&
Feature_Objects_Shape_Matches_Container_Type::
operator=( const Feature_Objects_Shape_Matches_Container_Type& other ){
  _invert = other._invert;
  return (*this);
}

/**
 * returns the value of the feature
 */
bool
Feature_Objects_Shape_Matches_Container_Type::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world) {
        return value( cv, grounding, children, phrase, world, NULL );

}

/**
 * returns the value of the feature
 */
bool
Feature_Objects_Shape_Matches_Container_Type::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Container * container = dynamic_cast< const Container* >( grounding );
  // Extract the objects from the container.
  vector< Object* > objects; 
  if( container != NULL ){
    for( unsigned int i = 0; i < container->container().size(); i++ ){
      Object * object = dynamic_cast< Object* >( container->container()[ i ] );
      if( object != NULL){
        objects.push_back( object ); 
      } else {
        return false;
      }
    } 
  }
 
  
  //check if the grounding is a container, the container type is not unknown, and the container has objects
  if( ( container != NULL ) && ( container->type() != "group" ) && ( !objects.empty() ) ){
    if (container->type() == std::string("row")) {
      sort( objects.begin(), objects.end(), World::min_y_sort );
      if( in_a_row( objects, 0 ) ){
        return !_invert;
      } else{
        return _invert;
      }     
    } else if (container->type() == std::string("column")) {
        sort( objects.begin(), objects.end(), World::min_x_sort );
        if( in_a_column( objects, 0 ) ){
          return !_invert;
        } else{
          return _invert;
        }
    } else {
      // TODO. Include the case for tower.
      return false;
    } 
  }
   return false;
}


/**
 * converts (x,y) to (rho, theta)
 */
pair< double, double >
Feature_Objects_Shape_Matches_Container_Type::
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
Feature_Objects_Shape_Matches_Container_Type::
in_a_row( vector< Object* > container_objects, unsigned int counter  ){

  //check if the recursion has reached the final element in the container
  if( container_objects.size() <= counter + 1 ){
    return true;
  }
  //check if the next object in the list is in the admissible region
  pair< double, double > rho_theta;
  rho_theta = convert_to_rho_theta( ( container_objects[ counter + 1 ]->transform().position().y() - container_objects[ counter ]->transform().position().y() ), ( container_objects[ counter + 1 ]->transform().position().x() - container_objects[ counter ]->transform().position().x() ) );
  if( ( rho_theta.first <= _rho ) && ( abs( rho_theta.second ) <= _theta ) ){
    return in_a_row( container_objects, counter + 1 );
  } else{
    return false;
  }
}

/**
 * returns true if in a column of size n
 */
bool
Feature_Objects_Shape_Matches_Container_Type::
in_a_column( vector< Object* > container_objects, unsigned int counter  ){
  //check if the recursion has reached the final element in the container
  if( container_objects.size() <= counter + 1){
    return true;
  }

  //check if the next object in the list is in the admissible region
  pair< double, double > rho_theta;
  rho_theta = convert_to_rho_theta( ( container_objects[ counter + 1 ]->transform().position().x() - container_objects[ counter ]->transform().position().x() ), ( container_objects[ counter + 1 ]->transform().position().y() - container_objects[ counter ]->transform().position().y() ) );
  //cout << "local root object: " << *container_objects[ counter ] << endl;
  //cout << "checking object( " << *container_objects[ counter + 1 ] << " ):" << endl;
  //cout << " rho: " << rho_theta.first << endl;
  //cout << " theta: " << rho_theta.second << endl;
  if( ( rho_theta.first <= _rho ) && ( abs( rho_theta.second ) <= _theta ) ){
    return in_a_column( container_objects, counter + 1 );
  } else{
    return false;
  }
}


/**
 * returns true if in a tower of size n
 */
bool
Feature_Objects_Shape_Matches_Container_Type::
in_a_tower(){
  
  // TODO
  return false;
}

/** 
 * imports the Feature_Objects_Shape_Matches_Container_Type class from an XML file
 */
void
Feature_Objects_Shape_Matches_Container_Type::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* ) 
                  ( "feature_objects_shape_matches_container_type" ) ) == 0 ){
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
 * imports the Feature_Objects_Shape_Matches_Container_Type class from an XML node pointer
 */
void
Feature_Objects_Shape_Matches_Container_Type::
from_xml( xmlNodePtr root ){
  _invert = false;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool ) ( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
  }
}


/**
 * exports the Feature_Objects_Shape_Matches_Container_Type class to an XML file
 */
void
Feature_Objects_Shape_Matches_Container_Type::
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
 * exports the Feature_Objects_Shape_Matches_Container_Type class to an XML node pointer
 */
void
Feature_Objects_Shape_Matches_Container_Type::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )
                           ( "feature_objects_shape_matches_container_type" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Feature_Objects_Shape_Matches_Container_Type class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Objects_Shape_Matches_Container_Type& other ){
    out << "class:\"Feature_Objects_Shape_Matches_Container_Type\" ";
    return out;
  }
}
