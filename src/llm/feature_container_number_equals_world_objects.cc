/**
 * @file feature_container_number_equals_world_objects.cc
 *
 * @brief
 *
 * Class used to describe a feature of the container number_equals_world_objects
 */
#include <sstream>

#include "h2sl/feature_container_number_equals_world_objects.h"
#include "h2sl/container.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Number_Equals_World_Objects class constructor
 */
Feature_Container_Number_Equals_World_Objects::
Feature_Container_Number_Equals_World_Objects( const bool& invert ) : Feature( invert ) {

}

/**
 * Feature_Container_Number_Equals_World_Objects class copy constructor
 */
Feature_Container_Number_Equals_World_Objects::
Feature_Container_Number_Equals_World_Objects( const Feature_Container_Number_Equals_World_Objects& other ) : Feature( other ) {

}

/**
 * Feature_Container_Number_Equals_World_Objects class destructor
 */
Feature_Container_Number_Equals_World_Objects::
~Feature_Container_Number_Equals_World_Objects() {

}

/** 
 * Feature_Container_Number_Equals_World_Objects class assignment operator
 */
Feature_Container_Number_Equals_World_Objects&
Feature_Container_Number_Equals_World_Objects::
operator=( const Feature_Container_Number_Equals_World_Objects& other ){
  _invert = other._invert;
  return (*this);
}

/**
 * returns the value of the feature
 */
bool
Feature_Container_Number_Equals_World_Objects::
value( const unsigned int& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world ){
    return value( cv, grounding, children, phrase, world, NULL );
}

/**
 * returns the value of the fature
 */
bool
Feature_Container_Number_Equals_World_Objects::
value( const unsigned int& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Container_Number_Equals_World_Objects::
value( const unsigned int& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const Container* container = dynamic_cast< const Container* >( grounding );
  if( container != NULL ){
    if( container->container().size() > 0 ){
      //Can you check this part?
      vector< Object* > objects;
      for( unsigned int i = 0; i < static_cast< Object* >(world->_objects().size()); i++ ){
        if( static_cast< Object* >(world->_objects()[ i ]->type()) == container->container().front()->type() ){
          objects.push_back( static_cast< Object* >(world->_objects()[ i ]) );
        }
      }
      //-------------------------------------
      if( container->container().size() == objects.size() ){
        return !_invert;
      } else {
        return _invert;
      }
    }
  }
  return false;
}

//Original code
/*
 bool
 Feature_Container_Number_Equals_World_Objects::
 value( const unsigned int& cv,
        const h2sl::Grounding* grounding,
        const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
        const h2sl::Phrase* phrase,
        const World* world ){
   const Container* container = dynamic_cast< const Container* >( grounding );
   if( container != NULL ){
     if( container->container().size() > 0 ){
       vector< Object* > objects;
       for( unsigned int i = 0; i < world->nsf_nri_mvli_objects().size(); i++ ){
         if( world->nsf_nri_mvli_objects()[ i ]->type() == container->container().front()->type() ){
           objects.push_back( world->nsf_nri_mvli_objects()[ i ] );
         }
     }
     if( container->container().size() == objects.size() ){
       return !_invert;
     } else {
       return _invert;
     }
   }
  }
 return false;
 }
*/

/**
 * imports the Feature_Container_Number_Equals_World_Objects class from an XML file
 */
void
Feature_Container_Number_Equals_World_Objects::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_container_number_equals_world_objects" ) ) == 0 ){
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
 * imports the Feature_Container_Number_Equals_World_Objects class from an XML node pointer
 */
void
Feature_Container_Number_Equals_World_Objects::
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
 * exports the Feature_Container_Number_Equals_World_Objects class to an XML file
 */
void
Feature_Container_Number_Equals_World_Objects::
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
 * exports the Feature_Container_Number_Equals_World_Objects class to an XML node pointer
 */
void
Feature_Container_Number_Equals_World_Objects::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_number_equals_world_objects" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /** 
   * Feature_Container_Number_Equals_World_Objects class ostream operator
   */
  ostream&
  operator<<( ostream& out,
              const Feature_Container_Number_Equals_World_Objects& other ){
    out <<"class:\"Feature_Container_Number_Equals_World_Objects\" ";
    return out;
  }
}
