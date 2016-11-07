/**
 * @file feature_container_dominant_z_shape.cc
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

#include "h2sl/feature_container_dominant_z_shape.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/container.h"
#include "h2sl/abstract_container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Dominant_Z_Shape class constructor
 */
Feature_Container_Dominant_Z_Shape::
Feature_Container_Dominant_Z_Shape( const bool& invert, 
                           const double& model_deviation_tolerance,
                           const double& adjacent_distance_tolerance,
                           const unsigned int& min_num_elements) : Feature( invert ),
                                                    _model_deviation_tolerance( model_deviation_tolerance ),
                                                    _adjacent_distance_tolerance( adjacent_distance_tolerance ),
                                                    _min_num_elements( min_num_elements ) {

}

/** 
 * Feature_Container_Dominant_Z_Shape class copy constructor
 */ 
Feature_Container_Dominant_Z_Shape::
Feature_Container_Dominant_Z_Shape( const Feature_Container_Dominant_Z_Shape& other ) : Feature( other ),
                                                    _model_deviation_tolerance( other._model_deviation_tolerance ),
                                                    _adjacent_distance_tolerance( other._adjacent_distance_tolerance ),
                                                    _min_num_elements( other._min_num_elements ) {
}

/**
 * Feature_Container_Dominant_Z_Shape class destructor
 */
Feature_Container_Dominant_Z_Shape::
~Feature_Container_Dominant_Z_Shape() {

}

/** 
 * Feature_Container_Dominant_Z_Shape class assignment operator
 */
Feature_Container_Dominant_Z_Shape&
Feature_Container_Dominant_Z_Shape::
operator=( const Feature_Container_Dominant_Z_Shape& other ){
  _invert = other._invert;
  _model_deviation_tolerance = other._model_deviation_tolerance;
  _adjacent_distance_tolerance = other._adjacent_distance_tolerance;
  _min_num_elements = other._min_num_elements;
  return (*this);
}

/**
 * Helper function for min_z_sorting 
 */
bool
min_z_sort_container_spatial_concept( Grounding* a, Grounding* b ){

  if ((dynamic_cast< const Object* >(a) != NULL) && (dynamic_cast< const Object* >(b) != NULL)) {
  return ( ((dynamic_cast< const Object* >(a))->transform().position().x()) < 
          ((dynamic_cast< const Object* >(b))->transform().position().x()) );
  } else { 
  cout << "Cannot cast container entity into Object" << endl;
  return false;
  }
}

/**
 * Returns bool if the concept has dominant structure along the z axis. .
 * z-axis is positive going up.
 */
bool
Feature_Container_Dominant_Z_Shape::
container_expresses_dominant_z_structure( const std::vector < Grounding* >& container, 
                                          const double& model_deviation_tolerance, 
                                          const double& adjacent_distance_tolerance, 
                                          const unsigned int& min_num_elements ) {

  if ( dynamic_cast< const Object* >(container.front()) != NULL ) {

    // Check: if the container has at least the min. number of entities. 
    if ( container.size() < min_num_elements ){
      return false;
    }

    // Check: x-deviation is within bound.
    double mean_x = 0.0;
    for ( unsigned int i = 0; i < container.size(); i++ ){
      mean_x += dynamic_cast< const Object* >(container[ i ])->transform().position().x();
    }
    mean_x /= container.size();
    double rmse_x = 0.0;
    for ( unsigned int i = 0; i < container.size(); i++ ){
      rmse_x += pow( ( dynamic_cast< const Object* >(container[ i ])->transform().position().x() - mean_x ), 2.0 );
    }
    rmse_x /= container.size();
    rmse_x = sqrt( rmse_x );
    if ( rmse_x > model_deviation_tolerance ) {
      return false;
    }

    // Check: z-deviation is within bound. 
    double mean_y = 0.0;
    for ( unsigned int i = 0; i < container.size(); i++ ){
      mean_y += dynamic_cast< const Object* >(container[ i ])->transform().position().y();
    }
    mean_y /= container.size();
    double rmse_y = 0.0;
    for ( unsigned int i = 0; i < container.size(); i++ ){
      rmse_y += pow( ( dynamic_cast< const Object* >(container[ i ])->transform().position().y() - mean_y ), 2.0 );
    }
    rmse_y = sqrt( rmse_y );
    if ( rmse_y > model_deviation_tolerance ) {
      return false;
    }

   // Check inter-element adjacent distance tolerance.
    vector < Grounding* > sorted_container ( container );
    sort( sorted_container.begin(), sorted_container.end(), min_z_sort_container_spatial_concept );
    for( unsigned int i = 0; i < (sorted_container.size() - 1); i++ ){
      if( abs( dynamic_cast< const Object* >(sorted_container[ (i + 1) ])->transform().position().z() - 
               dynamic_cast< const Object* >(sorted_container[ i ])->transform().position().z() ) > adjacent_distance_tolerance ){
        return false;
      }
    }
  
  } else {
    return false;
  }

  return true;
}

/**
 * Returns the value evaluated by the feature.
 */
bool
Feature_Container_Dominant_Z_Shape::
value( const unsigned int& cv,
        const h2sl::Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const h2sl::Phrase* phrase,
        const World* world ) {
  return value( cv, grounding, children, phrase, world, NULL );

}

/**
 * Returns the value evaluated by the feature.
 */
bool
Feature_Container_Dominant_Z_Shape::
value( const unsigned int& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Container* container = dynamic_cast< const Container* >( grounding );
  if (container != NULL) {
    if (container_expresses_dominant_z_structure(container->container(), 
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
 * imports the Feature Container Dominant Z Shape class from an XML file
 */
void
Feature_Container_Dominant_Z_Shape::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_container_dominant_z_shape" ) ) == 0 ){
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
 * imports the Feature_Container_Dominant_Z_Shape class from an XML node pointer
 */
void
Feature_Container_Dominant_Z_Shape::
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
 * exports the Feature_Container_Dominant_Z_Shape class to an XML file
 */
void
Feature_Container_Dominant_Z_Shape::
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
 * exports the Feature_Container_Dominant_Z_Shape class to an XML node pointer
 */
void
Feature_Container_Dominant_Z_Shape::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_dominant_z_shape" ), NULL );
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
   * Feature_Container_Dominant_Z_Shape class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Container_Dominant_Z_Shape& other ){
     out << "class:\"Feature_Container_Dominant_Z_Shape\" ";
     out << "model_deviation_tolerance:" << other.model_deviation_tolerance() << "\" ";
     out << "adjacent_distance_tolerance:" << other.adjacent_distance_tolerance() << "\" ";
     out << "min_num_elements:" << other.min_num_elements() << "\" ";
     return out;
   }
}
