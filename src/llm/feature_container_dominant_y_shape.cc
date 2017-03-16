/**
 * @file feature_container_dominant_y_shape.cc
 *
 * @brief
 *
 * Class used to describe the feature container domimant y shape 
 */
#include <assert.h>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <cmath>

#include "h2sl/feature_container_dominant_y_shape.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/container.h"
#include "h2sl/abstract_container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Dominant_Y_Shape class constructor
 */
Feature_Container_Dominant_Y_Shape::
Feature_Container_Dominant_Y_Shape( const bool& invert, 
                           const double& model_deviation_tolerance,
                           const double& adjacent_distance_tolerance,
                           const unsigned int& min_num_elements) : Feature( invert ),
                                                    _model_deviation_tolerance( model_deviation_tolerance ),
                                                    _adjacent_distance_tolerance( adjacent_distance_tolerance ),
                                                    _min_num_elements( min_num_elements ) {

}

/** 
 * Feature_Container_Dominant_Y_Shape class copy constructor
 */ 
Feature_Container_Dominant_Y_Shape::
Feature_Container_Dominant_Y_Shape( const Feature_Container_Dominant_Y_Shape& other ) : Feature( other ),
                                                    _model_deviation_tolerance( other._model_deviation_tolerance ),
                                                    _adjacent_distance_tolerance( other._adjacent_distance_tolerance ),
                                                    _min_num_elements( other._min_num_elements ) {
}

/**
 * Feature_Container_Dominant_Y_Shape class destructor
 */
Feature_Container_Dominant_Y_Shape::
~Feature_Container_Dominant_Y_Shape() {

}

/** 
 * Feature_Container_Dominant_Y_Shape class assignment operator
 */
Feature_Container_Dominant_Y_Shape&
Feature_Container_Dominant_Y_Shape::
operator=( const Feature_Container_Dominant_Y_Shape& other ){
  _invert = other._invert;
  _model_deviation_tolerance = other._model_deviation_tolerance;
  _adjacent_distance_tolerance = other._adjacent_distance_tolerance;
  _min_num_elements = other._min_num_elements;
  return (*this);
}

/**
 * Helper function for min_y_sorting 
 */
//bool
//min_y_sort_container_spatial_concept( Object* a, Object* b ){
//  return a->transform().position().y() < b->transform().position().y();
//}
bool
min_y_sort_container_spatial_concept( Grounding* a, Grounding* b ){
  if ((dynamic_cast< const Object* >(a) != NULL) && (dynamic_cast< const Object* >(b) != NULL)) {
    return ( ( ( static_cast< const Object* >( a ) )->transform().position().y() ) < 
             ( ( static_cast< const Object* >( b ) )->transform().position().y() ) );
  } else { 
    cout << "Cannot cast container entity into Object" << endl;
    return false;
  }
}

/**
 * Returns bool if the concept is a row.
 * Note: row is along the y-axis (=ve y along the left side of the robot.)
 * x-axis is going forward from the robot.
 * z-axis is positive going up.
 */
bool
Feature_Container_Dominant_Y_Shape::
container_expresses_dominant_y_structure( const std::vector < Object* >& container, 
                                          const double& model_deviation_tolerance, 
                                          const double& adjacent_distance_tolerance, 
                                          const unsigned int& min_num_elements ) {
  // Check: if the container has at least the min. number of entities. 
  if ( container.size() < min_num_elements ){
    return false;
  }

  // Check: x-deviation is within bound.
  double mean_x = 0.0;
  for ( unsigned int i = 0; i < container.size(); i++ ){
    mean_x += container[ i ]->transform().position().x();
  }
  mean_x /= container.size();
  double rmse_x = 0.0;
  for ( unsigned int i = 0; i < container.size(); i++ ){
    rmse_x += pow( ( container[ i ]->transform().position().x() - mean_x ), 2.0 );
  }
  rmse_x /= container.size();
  rmse_x = sqrt( rmse_x );
  if ( rmse_x > model_deviation_tolerance ) {
    return false;
  }

  // Check: z-deviation is within bound. 
  double mean_z = 0.0;
  for ( unsigned int i = 0; i < container.size(); i++ ){
    mean_z += container[ i ]->transform().position().z();
  }
  mean_z /= container.size();
  double rmse_z = 0.0;
  for ( unsigned int i = 0; i < container.size(); i++ ){
    rmse_z += pow( ( container[ i ]->transform().position().z() - mean_z ), 2.0 );
  }
  rmse_z = sqrt( rmse_z );
  if ( rmse_z > model_deviation_tolerance ) {
    return false;
  }

 // Check inter-element adjacent distance tolerance.
  vector < Object* > sorted_container ( container );
  sort( sorted_container.begin(), sorted_container.end(), min_y_sort_container_spatial_concept );
  for( unsigned int i = 0; i < (sorted_container.size() - 1); i++ ){
    if( abs( sorted_container[ ( i + 1 ) ]->transform().position().y() - sorted_container[ i ]->transform().position().y() ) > adjacent_distance_tolerance ){
      return false;
    }
  }
  return true;
}

/**
 * Returns the value evaluated by the feature.
 */
bool
Feature_Container_Dominant_Y_Shape::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ) {
  return value( cv, grounding, children, phrase, world, NULL );

}

/**
 * Returns the value evaluated by the feature.
 */
bool
Feature_Container_Dominant_Y_Shape::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Container* container = dynamic_cast< const Container* >( grounding );
  if (container != NULL) {
    vector< Object* > object_container;
    for( unsigned int i = 0; i < container->container().size(); i++ ){
      if( dynamic_cast< Object* >( container->container()[ i ] ) != NULL ){
        object_container.push_back( static_cast< Object* >( container->container()[ i ] ) );
      } else{
        return false;
      }
    }
    if (container_expresses_dominant_y_structure( object_container, 
                                                 _model_deviation_tolerance, 
                                                 _adjacent_distance_tolerance, 
                                                 _min_num_elements)) {
      return !_invert; 
    } else {
      return _invert;
    }
  }
  return false;
}

/** 
 * imports the Feature Container Dominant Y Shape class from an XML file
 */
void
Feature_Container_Dominant_Y_Shape::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_container_dominant_y_shape" ) ) == 0 ){
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
 * imports the Feature_Container_Dominant_Y_Shape class from an XML node pointer
 */
void
Feature_Container_Dominant_Y_Shape::
from_xml( xmlNodePtr root ){
  _invert = false;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool ) ( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }

    tmp = xmlGetProp( root, ( const xmlChar* )( "model_deviation_tolerance" ) );
    if( tmp != NULL){
      string model_deviation_tolerance_string = ( const char* )( tmp );
      _model_deviation_tolerance = strtod( model_deviation_tolerance_string.c_str(), NULL );
      xmlFree( tmp );
    }

    tmp = xmlGetProp( root, ( const xmlChar* )( "adjacent_distance_tolerance" ) );
    if( tmp != NULL){
      string adjacent_distance_tolerance_string = ( const char* )( tmp );
      _adjacent_distance_tolerance = strtod( adjacent_distance_tolerance_string.c_str(), NULL );
      xmlFree( tmp );
    }

    tmp = xmlGetProp( root, ( const xmlChar* )( "min_num_elements" ) );
    if( tmp != NULL){
      string min_num_elements_string = ( const char* )( tmp );
      _min_num_elements = strtol( min_num_elements_string.c_str(), NULL, 10 );
      xmlFree( tmp );
    }


  }
}


/**
 * exports the Feature_Container_Dominant_Y_Shape class to an XML file
 */
void
Feature_Container_Dominant_Y_Shape::
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
 * exports the Feature_Container_Dominant_Y_Shape class to an XML node pointer
 */
void
Feature_Container_Dominant_Y_Shape::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_dominant_y_shape" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  stringstream model_deviation_tolerance_string;
  model_deviation_tolerance_string << _model_deviation_tolerance;
  xmlNewProp( node, ( const xmlChar* )( "model_deviation_tolerance" ), ( const xmlChar* )( model_deviation_tolerance_string.str().c_str() ) );
  stringstream adjacent_distance_tolerance_string;
  adjacent_distance_tolerance_string << _adjacent_distance_tolerance;
  xmlNewProp( node, ( const xmlChar* )( "adjacent_distance_tolerance" ), ( const xmlChar* )( adjacent_distance_tolerance_string.str().c_str() ) );
  stringstream min_num_elements_string;
  min_num_elements_string << _min_num_elements;
  xmlNewProp( node, ( const xmlChar* )( "min_num_elements" ), ( const xmlChar* )( min_num_elements_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Feature_Container_Dominant_Y_Shape class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Container_Dominant_Y_Shape& other ){
     out << "class:\"Feature_Container_Dominant_Y_Shape\" ";
     out << "model_deviation_tolerance:" << other.model_deviation_tolerance() << "\" ";
     out << "adjacent_distance_tolerance:" << other.adjacent_distance_tolerance() << "\" ";
     out << "min_num_elements:" << other.min_num_elements() << "\" ";
     return out;
   }
}

