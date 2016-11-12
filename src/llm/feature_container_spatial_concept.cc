/**
 * @file feature_container_spatial_concept.cc
 *
 * @brief
 *
 * Class used to describe a feature of the container_abstract_container number
 */
#include <assert.h>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <cmath>

#include "h2sl/feature_container_spatial_concept.h"
#include "h2sl/spatial_relation.h"
#include "h2sl/container.h"
#include "h2sl/abstract_container.h"

using namespace std;
using namespace h2sl;

/**
 * Helper function for min_x_sorting 
 */
/*bool
min_x_sort_container_spatial_concept( Object* a, Object* b ){
  return a->transform().position().x() < b->transform().position().x();
}*/

bool
min_x_sort_container_spatial_concept (Grounding* a, Grounding* b) {
    if ((dynamic_cast< const Object* >(a) != NULL) && (dynamic_cast< const Object* >(b) != NULL)) {
        return ( ((dynamic_cast< const Object* >(a))->transform().position().x()) <
                ((dynamic_cast< const Object* >(b))->transform().position().x()) );
    } else {
        cout << "Cannot cast container entity into Object" << endl;
        return false;
    }
}

/**
 * Helper function for min_y_sorting 
 */
/*bool
min_y_sort_container_spatial_concept( Object* a, Object* b ){
  return a->transform().position().y() < b->transform().position().y();
}*/

bool
min_y_sort_container_spatial_concept (Grounding* a, Grounding* b) {
    if ((dynamic_cast< const Object* >(a) != NULL) && (dynamic_cast< const Object* >(b) != NULL)) {
        return ( ((dynamic_cast< const Object* >(a))->transform().position().y()) <
                ((dynamic_cast< const Object* >(b))->transform().position().y()) );
    } else {
        cout << "Cannot cast container entity into Object" << endl;
        return false;
    }
}

/**
 * Helper function for min_z_sorting 
 */
/*bool
min_z_sort_container_spatial_concept( Object* a, Object* b ){
  return a->transform().position().z() < b->transform().position().z();
}*/

bool
min_z_sort_container_spatial_concept (Grounding* a, Grounding* b) {
    if ((dynamic_cast< const Object* >(a) != NULL) && (dynamic_cast< const Object* >(b) != NULL)) {
        return ( ((dynamic_cast< const Object* >(a))->transform().position().z()) <
                ((dynamic_cast< const Object* >(b))->transform().position().z()) );
    } else {
        cout << "Cannot cast container entity into Object" << endl;
        return false;
    }
}


/**
 * Feature_Container_Spatial_Concept class constructor
 */
Feature_Container_Spatial_Concept::
Feature_Container_Spatial_Concept( const bool& invert,
                           const double& model_deviation_tolerance,
                           const double& adjacent_distance_tolerance,
                           const unsigned int& min_num_elements) : Feature( invert ),
        					    _model_deviation_tolerance( model_deviation_tolerance ),
     						    _adjacent_distance_tolerance( adjacent_distance_tolerance ),
  						    _min_num_elements( min_num_elements ){

}

/**
 * Feature_Container_Spatial_Concept class copy constructor
 */
Feature_Container_Spatial_Concept::
Feature_Container_Spatial_Concept( const Feature_Container_Spatial_Concept& other ) : Feature( other ), 
        					    _model_deviation_tolerance( other._model_deviation_tolerance ),
     						    _adjacent_distance_tolerance( other._adjacent_distance_tolerance ),
  						    _min_num_elements( other._min_num_elements ){
}

/**
 * Feature_Container_Spatial_Concept class destructor
 */
Feature_Container_Spatial_Concept::
~Feature_Container_Spatial_Concept() {

}

/** 
 * Feature_Container_Spatial_Concept class assignment operator
 */
Feature_Container_Spatial_Concept&
Feature_Container_Spatial_Concept::
operator=( const Feature_Container_Spatial_Concept& other ){
  _invert = other._invert;
  _model_deviation_tolerance = other._model_deviation_tolerance;
  _adjacent_distance_tolerance = other._adjacent_distance_tolerance;
  _min_num_elements = other._min_num_elements;
  return (*this);
}


/**
 * Returns bool if the concept is a column.
 * Note: column is along the x-axis 
 * y-axis is along the left side of the robot.
 * x-axis is going forward from the robot.
 * z-axis is positive going up.
 */
bool
Feature_Container_Spatial_Concept::
container_expresses_column( const std::vector < Object* >& container_objects, const double& model_deviation_tolerance, const double& adjacent_distance_tolerance, const unsigned int& min_num_elements ){

  // Check: if the column has at least two objects.
  if ( container_objects.size() < min_num_elements ){
    return false;
  }

  // Check: y-deviation is within bound. 
  double mean_y = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    mean_y += container_objects[ i ]->transform().position().y();
  } 
  mean_y /= container_objects.size();
  double rmse_y = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    rmse_y += pow( ( container_objects[ i ]->transform().position().y() - mean_y ), 2.0 );
  }
  rmse_y /= container_objects.size();
  rmse_y = sqrt( rmse_y );
  if ( rmse_y > model_deviation_tolerance ) {
    return false;
  }
  
  // Check: z-deviation is within bound. 
  double mean_z = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    mean_z += container_objects[ i ]->transform().position().z();
  } 
  mean_z /= container_objects.size();
  double rmse_z = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    rmse_z += pow( ( container_objects[ i ]->transform().position().z() - mean_z ), 2.0 );
  }
  rmse_z = sqrt( rmse_z );
  if ( rmse_z > model_deviation_tolerance ) {
    return false;
  }
  
  // Check: adjacent elements are not far apart. 
  /*
  vector < Object* > sorted_container_objects;
  for( unsigned int i = 0; i < container_objects.size(); i++ ){
    sorted_container_objects.push_back( container_objects[ i ]); 
  }
  */
  vector < Object* > sorted_container_objects ( container_objects );
  sort( sorted_container_objects.begin(), sorted_container_objects.end(), min_x_sort_container_spatial_concept );
  for( unsigned int i = 0; i < (sorted_container_objects.size() - 1); i++ ){
    if( abs( sorted_container_objects[ (i + 1) ]->transform().position().x() - sorted_container_objects[ i ]->transform().position().x() ) > adjacent_distance_tolerance ){
      return false;
    }
  }
 
  // Return: if the container expresses a column.
  return true;
}


/**
 * Returns bool if the concept is a row.
 * Note: row is along the y-axis (=ve y along the left side of the robot.)
 * x-axis is going forward from the robot.
 * z-axis is positive going up.
 */
bool
Feature_Container_Spatial_Concept::
container_expresses_row( const std::vector < Object* >& container_objects, const double& model_deviation_tolerance, const double& adjacent_distance_tolerance, const unsigned int& min_num_elements ){

  // Check: if row has at least two objects.
  if ( container_objects.size() < min_num_elements ){
    return false;
  }

  // Check: x-deviation is within bound.
  double mean_x = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    mean_x += container_objects[ i ]->transform().position().x();
  }
  mean_x /= container_objects.size();
  double rmse_x = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    rmse_x += pow( ( container_objects[ i ]->transform().position().x() - mean_x ), 2.0 );
  }
  rmse_x /= container_objects.size();
  rmse_x = sqrt( rmse_x );
  if ( rmse_x > model_deviation_tolerance ) {
    return false;
  }

  // Check: z-deviation is within bound. 
  double mean_z = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    mean_z += container_objects[ i ]->transform().position().z();
  } 
  mean_z /= container_objects.size();
  double rmse_z = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    rmse_z += pow( ( container_objects[ i ]->transform().position().z() - mean_z ), 2.0 );
  }
  rmse_z = sqrt( rmse_z );
  if ( rmse_z > model_deviation_tolerance ) {
    return false;
  }
  
  // Check: adjacent elements are not far apart.
  /*vector < Object* > sorted_container_objects;
  for( unsigned int i = 0; i < container_objects.size(); i++ ){
    sorted_container_objects.push_back( container_objects[ i ]);
  }*/

  vector < Object* > sorted_container_objects ( container_objects );
  sort( sorted_container_objects.begin(), sorted_container_objects.end(), min_y_sort_container_spatial_concept );
  for( unsigned int i = 0; i < (sorted_container_objects.size() - 1); i++ ){
    if( abs( sorted_container_objects[ (i + 1) ]->transform().position().y() - sorted_container_objects[ i ]->transform().position().y() ) > adjacent_distance_tolerance ){
      return false;
    }
  }

  // Return: if the container expresses a row.
  return true;
}

bool
Feature_Container_Spatial_Concept::
container_expresses_row( std::vector < Object* >& container_objects, const double& model_deviation_tolerance, const double& adjacent_distance_tolerance, const unsigned int& min_num_elements ){

  // Check: if row has at least two objects.
  if ( container_objects.size() < min_num_elements ){
    return false;
  }

  // Check: x-deviation is within bound.
  double mean_x = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    mean_x += container_objects[ i ]->transform().position().x();
  }
  mean_x /= container_objects.size();
  double rmse_x = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    rmse_x += pow( ( container_objects[ i ]->transform().position().x() - mean_x ), 2.0 );
  }
  rmse_x /= container_objects.size();
  rmse_x = sqrt( rmse_x );
  if ( rmse_x > model_deviation_tolerance ) {
    return false;
  }

  // Check: z-deviation is within bound. 
  double mean_z = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    mean_z += container_objects[ i ]->transform().position().z();
  } 
  mean_z /= container_objects.size();
  double rmse_z = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    rmse_z += pow( ( container_objects[ i ]->transform().position().z() - mean_z ), 2.0 );
  }
  rmse_z = sqrt( rmse_z );
  if ( rmse_z > model_deviation_tolerance ) {
    return false;
  }
  
  // Check: adjacent elements are not far apart.
  /*vector < Object* > sorted_container_objects;
  for( unsigned int i = 0; i < container_objects.size(); i++ ){
    sorted_container_objects.push_back( container_objects[ i ]);
  }*/

  vector < Object* > sorted_container_objects ( container_objects );
  sort( sorted_container_objects.begin(), sorted_container_objects.end(), min_y_sort_container_spatial_concept );
  for( unsigned int i = 0; i < (sorted_container_objects.size() - 1); i++ ){
    if( abs( sorted_container_objects[ (i + 1) ]->transform().position().y() - sorted_container_objects[ i ]->transform().position().y() ) > adjacent_distance_tolerance ){
      return false;
    }
  }

  // Return: if the container expresses a row.
  return true;
}
bool
Feature_Container_Spatial_Concept::
container_expresses_tower( const std::vector < Object* >& container_objects, const double& model_deviation_tolerance, const double& adjacent_distance_tolerance, const unsigned int& min_num_elements ){

  // Check: if tower has at least two objects.
  if ( container_objects.size() < min_num_elements ){
    return false;
  }

  // Check: x-deviation is within bound.
  double mean_x = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    mean_x += container_objects[ i ]->transform().position().x();
  }
  mean_x /= container_objects.size();
  double rmse_x = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    rmse_x += pow( ( container_objects[ i ]->transform().position().x() - mean_x ), 2.0 );
  }
  rmse_x /= container_objects.size();
  rmse_x = sqrt( rmse_x );
  if ( rmse_x > model_deviation_tolerance ) {
    return false;
  }

  // Check: y-deviation is within bound. 
  double mean_y = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    mean_y += container_objects[ i ]->transform().position().y();
  } 
  mean_y /= container_objects.size();
  double rmse_y = 0.0;
  for ( unsigned int i = 0; i < container_objects.size(); i++ ){
    rmse_y += pow( ( container_objects[ i ]->transform().position().y() - mean_y ), 2.0 );
  }
  rmse_y = rmse_y /= container_objects.size();
  rmse_y = sqrt( rmse_y );
  if ( rmse_y > model_deviation_tolerance ) {
    return false;
  }
  
  // Check: adjacent elements along z are not far apart.
  /*vector < Object* > sorted_container_objects;
  for( unsigned int i = 0; i < container_objects.size(); i++ ){
    sorted_container_objects.push_back( container_objects[ i ]);
  }
  */
  vector < Object* > sorted_container_objects ( container_objects );
  sort( sorted_container_objects.begin(), sorted_container_objects.end(), min_z_sort_container_spatial_concept );
  for( unsigned int i = 0; i < (sorted_container_objects.size() - 1); i++ ){
    if( abs( sorted_container_objects[ (i + 1) ]->transform().position().z() - sorted_container_objects[ i ]->transform().position().z() ) > adjacent_distance_tolerance ){
      return false;
    }
  }

  // Return true: if the container expresses a tower.
  return true;
}

/**
 * Returns if the container is a group.
 * Note: here _adjacent_distance_tolerance is considered as kernel width.
 * Model deviation check applied on the cumulative score.
 * Min number of elements defined in the features file.
 */

bool
Feature_Container_Spatial_Concept::
container_expresses_group( const std::vector < Object* >& container_objects, const double& model_deviation_tolerance, const double& adjacent_distance_tolerance, const unsigned int& min_num_elements ){

  if( container_expresses_row( container_objects, model_deviation_tolerance, adjacent_distance_tolerance, min_num_elements ) ){
    return false;
  } else if( container_expresses_column( container_objects, model_deviation_tolerance, adjacent_distance_tolerance, min_num_elements ) ){
    return false;
  } else if( container_expresses_tower( container_objects, model_deviation_tolerance, adjacent_distance_tolerance, min_num_elements ) ) {
    return false;
  } else {
    
   // Check for group after eliminating the other structures.

    if ( container_objects.size() < min_num_elements ){
      return false;
    }

   // Check if there is an element within the distance bound. 
    bool found_element_within_distance_bound = false;
    double distance_pairwise = 0.0;
    for( unsigned int i = 0; i < container_objects.size(); i++ ){
      for ( unsigned int j = 0; j < container_objects.size(); j++ ){
        if ( i != j ){
          distance_pairwise = 0.0;
          distance_pairwise +=  pow( ( container_objects[ i ]->transform().position().x() - container_objects[ j ]->transform().position().x() ), 2.0 );
          distance_pairwise +=  pow( ( container_objects[ i ]->transform().position().y() - container_objects[ j ]->transform().position().y() ), 2.0 );
          distance_pairwise +=  pow( ( container_objects[ i ]->transform().position().z() - container_objects[ j ]->transform().position().z() ), 2.0 );
	  if( distance_pairwise < adjacent_distance_tolerance ){
            found_element_within_distance_bound = true;
          }
        }
      }
      if ( !found_element_within_distance_bound ) {
        return false;
      }
    }    

    // Check average distance of all pairwise within bound.
    unsigned int normalisation_constant = ( container_objects.size() * ( container_objects.size() + 1 ) / 2 ); // n(n+1)/2 values
    double distance_normalised = 0.0;
    double sigma = 0.2; // Parameter for the Gaussian kernel.
    for( unsigned int i = 0; i < container_objects.size(); i++ ){
      for( unsigned int j = (i + 1); j < container_objects.size(); j++ ){
        double norm_squared = 0.0;
        norm_squared += pow( ( container_objects[ i ]->transform().position().x() - container_objects[ j ]->transform().position().x() ), 2.0 );
        norm_squared += pow( ( container_objects[ i ]->transform().position().y() - container_objects[ j ]->transform().position().y() ), 2.0 );
        norm_squared += pow( ( container_objects[ i ]->transform().position().z() - container_objects[ j ]->transform().position().z() ), 2.0 );
        //distance_normalised += exp( - ( ( norm_squared ) / ( 2 * pow( sigma, 2.0 ) ) ) ); // gaussian kernel.
        distance_normalised = norm_squared; // linear kernel.
      }
    }
    distance_normalised /= normalisation_constant;
    if( distance_normalised > model_deviation_tolerance ){
      return false;
    }
    return true; 
  }
}


/**
 * returns the value of the feature
 */
bool
Feature_Container_Spatial_Concept::
value( const unsigned int& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world) {
    return value( cv, grounding, children, phrase, world, NULL );
    
}

bool
Feature_Container_Spatial_Concept::
value( const unsigned int& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context ){
    
}

/*bool
Feature_Container_Spatial_Concept::
value( const unsigned int& cv,
        const h2sl::Grounding* grounding,
        const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
        const h2sl::Phrase* phrase,
        const World* world ){
  const Container* container_grounding = dynamic_cast< const Container* >( grounding );

  if( container_grounding != NULL ){
    //cout << "in container grouding != NULL";
    //assert (false);
    switch ( Container::Type( container_grounding->type() )  ){
      case ( Container::TYPE_ROW ) :
        if( container_expresses_row( container_grounding->container(), _model_deviation_tolerance, _adjacent_distance_tolerance, _min_num_elements ) ){
          //cout << endl << "expresses row: " << endl;
          //cout << "!_invert: " << !_invert << endl;
          //assert (false);
          return !_invert;
        } else {
          //cout << "does not expresses row";
          //assert (false);
          return _invert;
        }
        break;
      case ( Container::TYPE_COLUMN ) :
        if( container_expresses_column( container_grounding->container(), _model_deviation_tolerance, _adjacent_distance_tolerance, _min_num_elements ) ){
          //cout << "expresses column";
          //assert (false);
          return !_invert;
          } else {
          //cout << "does not expresses column";
          //assert (false);
            return _invert;
          }
          break;
      case ( Container::TYPE_TOWER) :
        if( container_expresses_tower( container_grounding->container(), _model_deviation_tolerance, _adjacent_distance_tolerance, _min_num_elements ) ){
          return !_invert;
          } else {
            return _invert;
          }
          break;
      case ( Container::TYPE_GROUP ) :
        if( container_expresses_group( container_grounding->container() ) ){
          return !_invert;
          } else {
            return _invert;
          }
          break;
        default:
          //cout << "expresses default";
          //assert (false);
          return false;
      }
  }
  return false;
}*/

/** 
 * imports the Feature_Container_Spatial_Concept class from an XML file
 */
void
Feature_Container_Spatial_Concept::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_container_spatial_concept" ) ) == 0 ){
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
 * imports the Feature_Container_Spatial_Concept class from an XML node pointer
 */
void
Feature_Container_Spatial_Concept::
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
 * exports the Feature_Container_Spatial_Concept class to an XML file
 */
void
Feature_Container_Spatial_Concept::
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
 * exports the Feature_Container_Spatial_Concept class to an XML node pointer
 */
void
Feature_Container_Spatial_Concept::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_spatial_concept" ), NULL );
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

/**
 * Returns the model deviation tolerance. 
 */
double&
Feature_Container_Spatial_Concept::
model_deviation_tolerance( void ){
  return _model_deviation_tolerance;
}

/**
 * Returns the model deviation tolerance. Const version. 
 */
const double&
Feature_Container_Spatial_Concept::
model_deviation_tolerance( void )const{
  return _model_deviation_tolerance;
}

/**
 * Returns the distance threshold to nearest element. 
 */
double&
Feature_Container_Spatial_Concept::
adjacent_distance_tolerance( void ){
  return _adjacent_distance_tolerance;
}

/**
 * Returns the distance threshold to nearest element. Const version. 
 */
const double&
Feature_Container_Spatial_Concept::
adjacent_distance_tolerance( void )const{
  return _adjacent_distance_tolerance;
}

/**
 * Returns the min_num_elements 
 */
unsigned int&
Feature_Container_Spatial_Concept::
min_num_elements( void ){
  return _min_num_elements;
}

/**
 * Returns the min_num_elements. Const version. 
 */
const unsigned int&
Feature_Container_Spatial_Concept::
min_num_elements( void )const{
  return _min_num_elements;
}

namespace h2sl {
  /** 
   * Feature_Container_Spatial_Concept class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Container_Spatial_Concept& other ){
     out << "class:\"Feature_Container_Spatial_Concept\" "; 
     out << "model_deviation_tolerance:" << other.model_deviation_tolerance() << "\" ";
     out << "adjacent_distance_tolerance:" << other.adjacent_distance_tolerance() << "\" ";
     out << "min_num_elements:" << other.min_num_elements() << "\" ";
     return out;
   }
}
