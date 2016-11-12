/**
 * @file feature_container_extend_spatial_concept.cc
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

#include "h2sl/feature_container_extend_spatial_concept.h"
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
min_x_sort_container_extend_spatial_concept( Object* a, Object* b ){
    return a->transform().position().x() < b->transform().position().x();
}*/

bool
min_x_sort_container_extend_spatial_concept (Grounding* a, Grounding* b) {
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
min_y_sort_container_extend_spatial_concept( Object* a, Object* b ){
    return a->transform().position().y() < b->transform().position().y();
}*/

bool
min_y_sort_container_extend_spatial_concept (Grounding* a, Grounding* b) {
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
min_z_sort_container_extend_spatial_concept( Object* a, Object* b ){
    return a->transform().position().z() < b->transform().position().z();
}*/
bool
min_z_sort_container_extend_spatial_concept (Grounding* a, Grounding* b) {
    if ((dynamic_cast< const Object* >(a) != NULL) && (dynamic_cast< const Object* >(b) != NULL)) {
        return ( ((dynamic_cast< const Object* >(a))->transform().position().z()) <
                ((dynamic_cast< const Object* >(b))->transform().position().z()) );
    } else {
        cout << "Cannot cast container entity into Object" << endl;
        return false;
    }
}

/**
 * Feature_Container_Extend_Spatial_Concept class constructor
 */
Feature_Container_Extend_Spatial_Concept::
Feature_Container_Extend_Spatial_Concept( const bool& invert,
                           const double& model_deviation_tolerance,
                           const double& adjacent_distance_tolerance,
                           const unsigned int& min_num_elements) : Feature( invert ),
        					    _model_deviation_tolerance( model_deviation_tolerance ),
     						    _adjacent_distance_tolerance( adjacent_distance_tolerance ),
  						    _min_num_elements( min_num_elements ){

}

/**
 * Feature_Container_Extend_Spatial_Concept class copy constructor
 */
Feature_Container_Extend_Spatial_Concept::
Feature_Container_Extend_Spatial_Concept( const Feature_Container_Extend_Spatial_Concept& other ) : Feature( other ), 
        					    _model_deviation_tolerance( other._model_deviation_tolerance ),
     						    _adjacent_distance_tolerance( other._adjacent_distance_tolerance ),
  						    _min_num_elements( other._min_num_elements ){
}

/**
 * Feature_Container_Extend_Spatial_Concept class destructor
 */
Feature_Container_Extend_Spatial_Concept::
~Feature_Container_Extend_Spatial_Concept() {

}

/** 
 * Feature_Container_Extend_Spatial_Concept class assignment operator
 */
Feature_Container_Extend_Spatial_Concept&
Feature_Container_Extend_Spatial_Concept::
operator=( const Feature_Container_Extend_Spatial_Concept& other ){
  _invert = other._invert;
  _model_deviation_tolerance = other._model_deviation_tolerance;
  _adjacent_distance_tolerance = other._adjacent_distance_tolerance;
  _min_num_elements = other._min_num_elements;
  return (*this);
}

/**
 * Returns bool if the concept is a row and an extensible object is found from the world model.
 */
bool
Feature_Container_Extend_Spatial_Concept::
container_expresses_row_and_extendable( const std::vector < Object* >& container_objects, const std::vector < Object* >& world_objects ){
  if ( !Feature_Container_Spatial_Concept::container_expresses_row( container_objects, 
						      _model_deviation_tolerance, 
						      _adjacent_distance_tolerance, 
						      _min_num_elements ) ){
    return false;
  }

  vector < Object* > container_concat_world_objects ( container_objects );
    for(unsigned int i = 0; i < world_objects.size(); i++ ){
      container_concat_world_objects.push_back( world_objects[ i ] );
      if( Feature_Container_Spatial_Concept::container_expresses_row( container_concat_world_objects, 
                      				      _model_deviation_tolerance, 
                      				      _adjacent_distance_tolerance, 
                      				      _min_num_elements ) ){
        return true;
      } 
      container_concat_world_objects.pop_back();
    }

  return false;
}

/**
 * Returns bool if the concept is a column and an extensible object is found from the world model.
 */
bool
Feature_Container_Extend_Spatial_Concept::
container_expresses_column_and_extendable( const std::vector < Object* >& container_objects, const std::vector < Object* >& world_objects ){
  if ( !Feature_Container_Spatial_Concept::container_expresses_column( container_objects, 
						      _model_deviation_tolerance, 
						      _adjacent_distance_tolerance, 
						      _min_num_elements ) ){
    return false;
  } 

  vector < Object* > container_concat_world_objects ( container_objects );
    for(unsigned int i = 0; i < world_objects.size(); i++ ){
      container_concat_world_objects.push_back( world_objects[ i ] );
      if( Feature_Container_Spatial_Concept::container_expresses_column( container_concat_world_objects, 
                      				      _model_deviation_tolerance, 
                      				      _adjacent_distance_tolerance, 
                      				      _min_num_elements ) ){
        return true;
      } 
      container_concat_world_objects.pop_back();
    }
  return false;
}

/**
 * Returns bool if the concept is a tower and an extensible object is found from the world model.
 */
bool
Feature_Container_Extend_Spatial_Concept::
container_expresses_tower_and_extendable( const std::vector < Object* >& container_objects, const std::vector < Object* >& world_objects ){
  if ( !Feature_Container_Spatial_Concept::container_expresses_tower( container_objects, 
						      _model_deviation_tolerance, 
						      _adjacent_distance_tolerance, 
						      _min_num_elements ) ){
    return false;
  } 

  vector < Object* > container_concat_world_objects ( container_objects );
    for(unsigned int i = 0; i < world_objects.size(); i++ ){
      container_concat_world_objects.push_back( world_objects[ i ] );
      if( Feature_Container_Spatial_Concept::container_expresses_tower( container_concat_world_objects, 
                      				      _model_deviation_tolerance, 
                      				      _adjacent_distance_tolerance, 
                      				      _min_num_elements ) ){
        return true;
      } 
      container_concat_world_objects.pop_back();
    }
  return false;
}

/**
 * Returns bool if the concept is a group and an extensible object is found from the world model.
 */
bool
Feature_Container_Extend_Spatial_Concept::
container_expresses_group_and_extendable( const std::vector < Object* >& container_objects, const std::vector < Object* >& world_objects ){
  if ( !Feature_Container_Spatial_Concept::container_expresses_group( container_objects, 
						      _model_deviation_tolerance, 
						      _adjacent_distance_tolerance, 
						      _min_num_elements ) ){
    return false;
  } 

  vector < Object* > container_concat_world_objects ( container_objects );
    for(unsigned int i = 0; i < world_objects.size(); i++ ){
      container_concat_world_objects.push_back( world_objects[ i ] );
      if( Feature_Container_Spatial_Concept::container_expresses_group( container_concat_world_objects, 
                      				      _model_deviation_tolerance, 
                      				      _adjacent_distance_tolerance, 
                      				      _min_num_elements ) ){
        return true;
      } 
      container_concat_world_objects.pop_back();
    }
  return false;
}

/**
 * returns the value of the feature
 */
bool
Feature_Container_Extend_Spatial_Concept::
value( const unsigned int& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world) {
    return value( cv, grounding, children, phrase, world, NULL );
    
}

bool
Feature_Container_Extend_Spatial_Concept::
value( const unsigned int& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context ){
    
}

/*bool
Feature_Container_Extend_Spatial_Concept::
value( const unsigned int& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world ){
  const Container* container_grounding = dynamic_cast< const Container* >( grounding );

  if( container_grounding != NULL ){
      const vector < Object* > container_objects = container_grounding->container();
      if ( container_objects.size() != 0 ) {
        // World objects matching container object types and not in container.
        vector< Object* > world_objects;
        for( unsigned int i = 0; i < world->nsf_nri_mvli_objects().size(); i++ ){
          if( world->nsf_nri_mvli_objects()[ i ]->type() == container_objects[ 0 ]->type() ){
	    Object* world_object = world->nsf_nri_mvli_objects()[ i ];
	    bool not_found = true;
            for( unsigned int j = 0; j < container_objects.size(); j++ ){
	      const Object* container_object = container_objects[ j ]; 
	      if( *world_object == *container_object ){
                not_found = false;   
              }		
            }
	    if( not_found ) {
	      world_objects.push_back( world_object );
            }
          }
        }

	//cout << world_objects.size() << endl;
	//cout << world->nsf_nri_mvli_objects().size() << endl;
        //assert( !world_objects.empty() );

       if ( world_objects.size() != 0 ){
         switch( Container::Type( container_grounding->type() ) ){
           case( Container::TYPE_ROW ):
             if( container_expresses_row_and_extendable( container_objects, world_objects ) ){
               return !_invert;
             } else {
               return _invert;
             }
             break;
           case( Container::TYPE_COLUMN ):
             if( container_expresses_column_and_extendable( container_objects, world_objects ) ){
               return !_invert;
             } else {
               return _invert;
             }
             break;
           case( Container::TYPE_TOWER ):
             if( container_expresses_tower_and_extendable( container_objects, world_objects ) ){
               return !_invert;
             } else {
               return _invert;
             }
             break;
           case( Container::TYPE_GROUP ):
             if( container_expresses_group_and_extendable( container_objects, world_objects ) ){
               return !_invert;
             } else {
               return _invert;
             }
             break;
           default: 
             return false;
          } 
        }

     }
  }
  return false;
}*/

/** 
 * imports the Feature_Container_Extend_Spatial_Concept class from an XML file
 */
void
Feature_Container_Extend_Spatial_Concept::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_container_extend_spatial_concept" ) ) == 0 ){
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
 * imports the Feature_Container_Extend_Spatial_Concept class from an XML node pointer
 */
void
Feature_Container_Extend_Spatial_Concept::
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
 * exports the Feature_Container_Extend_Spatial_Concept class to an XML file
 */
void
Feature_Container_Extend_Spatial_Concept::
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
 * exports the Feature_Container_Extend_Spatial_Concept class to an XML node pointer
 */
void
Feature_Container_Extend_Spatial_Concept::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_extend_spatial_concept" ), NULL );
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
Feature_Container_Extend_Spatial_Concept::
model_deviation_tolerance( void ){
  return _model_deviation_tolerance;
}

/**
 * Returns the model deviation tolerance. Const version. 
 */
const double&
Feature_Container_Extend_Spatial_Concept::
model_deviation_tolerance( void )const{
  return _model_deviation_tolerance;
}

/**
 * Returns the distance threshold to nearest element. 
 */
double&
Feature_Container_Extend_Spatial_Concept::
adjacent_distance_tolerance( void ){
  return _adjacent_distance_tolerance;
}

/**
 * Returns the distance threshold to nearest element. Const version. 
 */
const double&
Feature_Container_Extend_Spatial_Concept::
adjacent_distance_tolerance( void )const{
  return _adjacent_distance_tolerance;
}

/**
 * Returns the min_num_elements 
 */
unsigned int&
Feature_Container_Extend_Spatial_Concept::
min_num_elements( void ){
  return _min_num_elements;
}

/**
 * Returns the min_num_elements. Const version. 
 */
const unsigned int&
Feature_Container_Extend_Spatial_Concept::
min_num_elements( void )const{
  return _min_num_elements;
}

namespace h2sl {
  /** 
   * Feature_Container_Extend_Spatial_Concept class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Container_Extend_Spatial_Concept& other ){
     out << "class:\"Feature_Container_Extend_Spatial_Concept\" "; 
     out << "model_deviation_tolerance:" << other.model_deviation_tolerance() << "\" ";
     out << "adjacent_distance_tolerance:" << other.adjacent_distance_tolerance() << "\" ";
     out << "min_num_elements:" << other.min_num_elements() << "\" ";
     return out;
   }
}
