/**
 * @file feature_object_adj_is_color.cc
 *
 * @brief
 *
 * a class used to describe a feature of the object type
 */

#include <sstream>

#include "h2sl/object.h"
#include "h2sl/feature_object_adj_is_color.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Object_Adj_Is_Color class constructor
 */
Feature_Object_Adj_Is_Color::
Feature_Object_Adj_Is_Color( const bool& invert ) : Feature( invert ){
    
}

/**
 * Feature_Object_Adj_Is_Color class copy constructor
 */
Feature_Object_Adj_Is_Color::
Feature_Object_Adj_Is_Color( const Feature_Object_Adj_Is_Color& other ) : Feature( other ){
    
}

/**
 * Feature_Object_Adj_Is_Color class destructor
 */
Feature_Object_Adj_Is_Color::
~Feature_Object_Adj_Is_Color() {
    
}

/**
 * Feature_Object_Adj_Is_Color class assignment operator
 */
Feature_Object_Adj_Is_Color&
Feature_Object_Adj_Is_Color::
operator=( const Feature_Object_Adj_Is_Color& other ){
  _invert = other._invert;
  return (*this);
}

/**
 * returns the value of the feature
 */
bool
Feature_Object_Adj_Is_Color::
value( const unsigned int& cv,
      const Grounding* grounding,
      const vector< pair< const Phrase*, vector< Grounding* > > >& children,
      const Phrase* phrase,
      const World* world) {
    return value( cv, grounding, children, phrase, world, NULL );
    
}

bool
Feature_Object_Adj_Is_Color::
value( const unsigned int& cv,
      const h2sl::Grounding* grounding,
      const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
      const h2sl::Phrase* phrase,
      const World* world,
      const Grounding* context ){
    
}

/*bool
Feature_Object_Adj_Is_Color::
value( const unsigned int& cv,
    const h2sl::Grounding* grounding,
    const vector< pair< const h2sl::Phrase*, vector< h2sl::Grounding* > > >& children,
    const h2sl::Phrase* phrase,
    const World* world ){
  const Object* object = dynamic_cast< const Object* >( grounding );
  
  if( object != NULL ){
    if( phrase->type() == h2sl::PHRASE_NP ){
      for( unsigned int i = 0; i < Object_Color::TYPE_NUM_TYPES; i++ ){
        const h2sl::Word * color_word = new h2sl::Word( h2sl::POS_JJ, Object_Color::type_to_std_string( ( Object_Color::Type )( i ) ).c_str(), 0 ); 
        if( phrase->has_word( *color_word ) ){
          return !_invert;
        }
      }
      return _invert;
    }
  }
  return false;
}*/

/**
 * imports the Feature_Object_Adj_Is_Color class from an XML file
 */
void
Feature_Object_Adj_Is_Color::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "feature_object_adj_is_color" ) ) == 0 ){
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
 * imports the Feature_Object_Adj_Is_Color class from an XML node pointer
 */
void
Feature_Object_Adj_Is_Color::
from_xml( xmlNodePtr root ){
  _invert = false;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      string invert_string = ( const char* )( tmp );
      _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
  }
}

/**
 * exports the Feature_Object_Adj_Is_Color class to an XML file
 */
void
Feature_Object_Adj_Is_Color::
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
 * exports the Feature_Object_Adj_Is_Color class to an XML node pointer
 */
void
Feature_Object_Adj_Is_Color::
to_xml( xmlDocPtr doc,
       xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_object_adj_is_color" ), NULL );
  stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

namespace h2sl {
  /**
   * Feature_Object_Adj_Is_Color class ostream operator
   */
  ostream&
  operator<<( ostream& out,
             const Feature_Object_Adj_Is_Color& other ){
	out << "class:\"Feature_Object_Adj_Is_Color\" ";
        return out;
    }
}
