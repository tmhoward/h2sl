/**
 * @file feature_min_x_container_region_abstract_container.cc
 *
 * @brief
 *
 * Class used to describe a feature of the container_region_abstract_container number
 */
#include <assert.h>
#include <sstream>
#include <algorithm>

#include "h2sl/spatial_relation.h"
#include "h2sl/container.h"
#include "h2sl/feature_min_x_container_region_abstract_container.h"
#include "h2sl/region_abstract_container.h"

using namespace std;
using namespace h2sl;

/*bool
min_x_container_region_abstract_container_sort( Object* a,
        Object* b ){
  return a->transform().position().x() < b->transform().position().x();
}*/

bool
min_x_container_region_abstract_container_sort (Grounding* a, Grounding* b) {
    if ((dynamic_cast< const Object* >(a) != NULL) && (dynamic_cast< const Object* >(b) != NULL)) {
        return ( ((dynamic_cast< const Object* >(a))->transform().position().x()) <
                ((dynamic_cast< const Object* >(b))->transform().position().x()) );
    } else {
        cout << "Cannot cast container entity into Object" << endl;
        return false;
    }
}

/**
 * Feature_Min_X_Container_Region_Abstract_Container class constructor
 */
Feature_Min_X_Container_Region_Abstract_Container::
Feature_Min_X_Container_Region_Abstract_Container( const bool& invert,
                                                  const std::string& relation_type ) : Feature( invert ),
                                                                          _relation_type( relation_type ) {

}

/**
 * Feature_Min_X_Container_Region_Abstract_Container class copy constructor
 */
Feature_Min_X_Container_Region_Abstract_Container::
Feature_Min_X_Container_Region_Abstract_Container( const Feature_Min_X_Container_Region_Abstract_Container& other ) : Feature( other ),
                                                              _relation_type( other._relation_type ) {

}

/**
 * Feature_Min_X_Container_Region_Abstract_Container class destructor
 */
Feature_Min_X_Container_Region_Abstract_Container::
~Feature_Min_X_Container_Region_Abstract_Container() {

}

/** 
 * Feature_Min_X_Container_Region_Abstract_Container class assignment operator
 */
Feature_Min_X_Container_Region_Abstract_Container&
Feature_Min_X_Container_Region_Abstract_Container::
operator=( const Feature_Min_X_Container_Region_Abstract_Container& other ){
  _invert = other._invert;
  _relation_type = other._relation_type;
  return (*this);
}

/**
 * returns the value of the feature
 */
bool
Feature_Min_X_Container_Region_Abstract_Container::
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
Feature_Min_X_Container_Region_Abstract_Container::
value( const string& cv,
        const h2sl::Grounding* grounding,
        const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
        const h2sl::Phrase* phrase,
        const World* world,
        const Grounding* context ){
  Region_Abstract_Container* region_abstract_container_child = NULL;
  Spatial_Relation* spatial_relation_child = NULL;
  const Container* container = NULL;
  bool is_equal = true;
  
  if( dynamic_cast< const Container* >( grounding ) != NULL ){
    container = static_cast< const Container* >( grounding );
  } else{
    return false;
  }
  
  for( unsigned int i = 0; i < children.size(); i++ ){
    for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
      if( dynamic_cast< Region_Abstract_Container* >( children[ i ].second[ j ] ) != NULL ){
        region_abstract_container_child = static_cast< Region_Abstract_Container* >( children[ i ].second[ j ] );
      } else if ( dynamic_cast< Spatial_Relation* >( children[ i ].second[ j ] ) != NULL ){
        spatial_relation_child = static_cast< Spatial_Relation* >( children[ i ].second[ j ] );
      }
    } 
  } 
 
  
  // Check all the conditions where the feature does not apply.a
  // Return false.
  if ((region_abstract_container_child == NULL) || (spatial_relation_child == NULL)
      || (container == NULL) || ( dynamic_cast< const Object* >(container->container().front()) == NULL )
      || (spatial_relation_child->relation_type() != _relation_type)) {
      return false;
  }
    
  // Create vector.
  // Fill in objects from the world that match AC object type.
  vector< Object* > type_matched_obj;
  for( unsigned int i = 0; i < world->objects().size(); i++ ) {
    if( world->objects()[ i ]->type() == region_abstract_container_child->spatial_relation_type() ) {
        type_matched_obj.push_back( world->objects()[ i ] );
    }
  }
    
  // Sort according to the x-coordinate.
  // Different types of sort functions imply different predictors.
  sort( type_matched_obj.begin(), type_matched_obj.end(),min_x_container_region_abstract_container_sort );
    
  // Check upto the number of elements implied by the AC in the object match array.
  // if they match with any of the objects in the container grounding.
  if (container->container().size() == region_abstract_container_child->number()) {
    bool allObjectsFound = true;
    bool isEqual = false;
    for (unsigned int i = 0; i < region_abstract_container_child->number(); ++i) {
        isEqual = false;
        for (unsigned int j = 0; j < container->container().size(); j++) {
            if (*type_matched_obj[ i ] == *dynamic_cast< const Object* >(container->container()[ j ])) {
                isEqual = true;
            }
        }
        // If an object is not found in the container
        // Then set the flag to false.
        if (!isEqual) {
            allObjectsFound = false;
        }
    }
    // The feature fires if all the objects are found.
    if (allObjectsFound) {
        return !_invert;
    } else {
        return _invert;
    }
  }
  return false;
}
    
    /*if( region_abstract_container_child != NULL ){
    if( spatial_relation_child != NULL ){
      if( spatial_relation_child->type() == _relation_type ){
        if( container_grounding != NULL ){
          vector< Object* > objects;
          for( unsigned int i = 0; i < world->nsf_nri_mvli_objects().size(); i++ ){
            if( world->nsf_nri_mvli_objects()[ i ]->type() == region_abstract_container_child->spatial_relation_type() ){
              objects.push_back( world->nsf_nri_mvli_objects()[ i ] );
            }
          }
          
          sort( objects.begin(), objects.end(), min_x_container_region_abstract_container_sort );
          
            
            
          objects.resize( region_abstract_container_child->num() );
          vector< Object* > container_objects = container_grounding->container();
          for( unsigned int i = 0; i < container_grounding->container().size(); i++ ){
            container_objects.push_back( container_grounding->container()[ i ] );
          }

          if( container_objects.size() == objects.size() ){
            cout << endl << endl << "THIS NEVER RUNS!" << endl << endl;
            sort( container_objects.begin(), container_objects.end(), min_x_container_region_abstract_container_sort );
            for( unsigned int i = 0; i < objects.size(); i++ ){
              if( container_objects[ i ] != objects[ i ] ){
                  is_equal = false;
              }
            }
            if( is_equal ){
              return !_invert;
            } else{
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
 * imports the Feature_Min_X_Container_Region_Abstract_Container class from an XML file
 */
void
Feature_Min_X_Container_Region_Abstract_Container::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_min_x_container_region_abstract_container" ) ) == 0 ){
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
 * imports the Feature_Min_X_Container_Region_Abstract_Container class from an XML node pointer
 */
void
Feature_Min_X_Container_Region_Abstract_Container::
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
 * exports the Feature_Min_X_Container_Region_Abstract_Container class to an XML file
 */
void
Feature_Min_X_Container_Region_Abstract_Container::
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
 * exports the Feature_Min_X_Container_Region_Abstract_Container class to an XML node pointer
 */
void
Feature_Min_X_Container_Region_Abstract_Container::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_min_x_container_region_abstract_container" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "relation_type" ), ( const xmlChar* )( _relation_type.c_str() ) );
  xmlAddChild( root, node );
  return;
}

std::string&
Feature_Min_X_Container_Region_Abstract_Container::
relation_type( void ){
  return _relation_type;
}

const std::string&
Feature_Min_X_Container_Region_Abstract_Container::
relation_type( void )const{
  return _relation_type;
}

namespace h2sl {
  /** 
   * Feature_Min_X_Container_Region_Abstract_Container class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Min_X_Container_Region_Abstract_Container& other ){
    out << "class:\"Feature_Min_X_Container_Region_Abstract_Container\" ";
    out << "relation_type:\"" << other.relation_type() << "\" ";
    return out;
  }
}
