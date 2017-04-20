/**
 * @file feature_container_merge_container_type_container.cc
 *
 * @brief
 *
 * Class used to describe a feature container matches child type
 */

#include <assert.h>
#include <sstream>

#include "h2sl/feature_container_merge_container_type_container.h"
#include "h2sl/container.h"
#include "h2sl/container_type.h"

using namespace std;
using namespace h2sl;

/**
 * Feature_Container_Merge_Container_Type_Container class constructor
 */
Feature_Container_Merge_Container_Type_Container::
Feature_Container_Merge_Container_Type_Container( const bool& invert,
                                                  const bool& order ) : Feature( invert ),
                                                                        _order( order ) {
    
}

/**
 * Feature_Container_Merge_Container_Type_Container class constructor
 */
Feature_Container_Merge_Container_Type_Container::
Feature_Container_Merge_Container_Type_Container( xmlNodePtr root ) : Feature(),
                                                                      _order( false ) {
  from_xml( root );
}

/**
 * Feature_Container_Merge_Container_Type_Container class destructor
 */
Feature_Container_Merge_Container_Type_Container::
~Feature_Container_Merge_Container_Type_Container() {
    
}

/**
 * Feature_Container_Merge_Container_Type_Container class copy constructor
 */
Feature_Container_Merge_Container_Type_Container::
Feature_Container_Merge_Container_Type_Container( const Feature_Container_Merge_Container_Type_Container& other ) : Feature( other ),
                                                                                                                    _order( other._order ) {
    
}

/**
 * Feature_Container_Merge_Container_Type_Container class assignment operator
 */
Feature_Container_Merge_Container_Type_Container&
Feature_Container_Merge_Container_Type_Container::
operator=( const Feature_Container_Merge_Container_Type_Container& other ) {
    _invert = other._invert;
    _order = other._order;
    return (*this);
}

/**
 * returns the value of the feature.
 */
bool
Feature_Container_Merge_Container_Type_Container::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

bool
Feature_Container_Merge_Container_Type_Container::
value( const string& cv,
        const Grounding* grounding,
        const vector< pair< const Phrase*, vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
    const Container * container = dynamic_cast< const Container* >( grounding );
    if( ( container != NULL ) && ( !children.empty() ) ){
      pair< const h2sl::Phrase*, const Container* > container_child( NULL, NULL );
      pair< const h2sl::Phrase*, const Container_Type* > container_type_child( NULL, NULL );
      for( unsigned int i = 0; i < children.size(); i++ ){
        for( unsigned int j = 0; j < children[ i ].second.size(); j++ ){
          if( dynamic_cast< const Container* >( children[ i ].second[ j ] ) != NULL ){
            container_child.first = children[ i ].first;
            container_child.second = static_cast< const Container* >( children[ i ].second[ j ] );
          } else if ( dynamic_cast< const Container_Type* >( children[ i ].second[ j ] ) != NULL ){
            container_type_child.first = children[ i ].first;
            container_type_child.second = static_cast< const Container_Type* >( children[ i ].second[ j ] );
          }
        }
      }

      if( ( container_child.first != NULL ) && ( container_child.second != NULL ) && ( container_type_child.first != NULL ) && ( container_type_child.second != NULL ) ){ 
        if( _order ){
          if( container_type_child.first->min_word_order() < container_child.first->min_word_order() ){
            if( ( container->type() == container_type_child.second->type() ) && ( container->container().size() == container_child.second->container().size() ) ){
              for( unsigned int i = 0; i < container->container().size(); i++ ){
                bool found_match = false;
                for( unsigned int j = 0; j < container_child.second->container().size(); j++ ){
                  if( container->container()[ i ] != NULL ){
                    if( container->container()[ i ]->matches_class_name( "object" ) && container_child.second->container()[ j ]->matches_class_name( "object" ) ){
                      if( *static_cast< const Object* >( container->container()[ i ] ) == *static_cast< const Object* >( container_child.second->container()[ j ] ) ){
                        found_match = true;
                      }
                    }
                  }
                }   
                if( !found_match ){
                  return _invert;
                }
              }
              return !_invert;            
            } else {
              return _invert;
            }
          }
        } else {
          if( container_type_child.first->min_word_order() > container_child.first->min_word_order() ){
            if( ( container->type() == container_type_child.second->type() ) && ( container->container().size() == container_child.second->container().size() ) ){
              for( unsigned int i = 0; i < container->container().size(); i++ ){
                bool found_match = false;
                for( unsigned int j = 0; j < container_child.second->container().size(); j++ ){
                  if( container->container()[ i ] != NULL ){
                    if( container->container()[ i ]->matches_class_name( "object" ) && container_child.second->container()[ j ]->matches_class_name( "object" ) ){
                      if( *static_cast< const Object* >( container->container()[ i ] ) == *static_cast< const Object* >( container_child.second->container()[ j ] ) ){
                        found_match = true;
                      }
                    }
                  }
                }
                if( !found_match ){
                  return _invert;
                }
              }
              return !_invert;
            } else {
              return _invert;
            }
          }
        }
      }
    }
    return false;
}

/**
 * exports the Feature_Container_Merge_Container_Type_Container class to an XML file
 */
void
Feature_Container_Merge_Container_Type_Container::
to_xml( xmlDocPtr doc, xmlNodePtr root )const{
    xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( "feature_container_merge_container_type_container" ), NULL );
    stringstream invert_string;
    invert_string << _invert;
    xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
    std::stringstream order_string;
  order_string << _order;
  xmlNewProp( node, ( const xmlChar* )( "order" ), ( const xmlChar* )( order_string.str().c_str() ) );
  xmlAddChild( root, node );
    xmlAddChild( root, node );
    return;
}

/**
 * imports the Feature_Container_Merge_Container_Type_Container class from an XML file
 */
void
Feature_Container_Merge_Container_Type_Container::
from_xml( xmlNodePtr root ){
    _invert = false;
    _order = false;
    if( root->type == XML_ELEMENT_NODE ){
        xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
        if( tmp != NULL ){
            string invert_string = ( char* )( tmp );
            _invert = ( bool )( strtol( invert_string.c_str(), NULL, 10 ) );
            xmlFree( tmp );
        }

        std::pair< bool, bool > order_prop = has_prop< bool >( root, "order" );
        if( order_prop.first ) {
          _order = order_prop.second;
        }
    }
    return;
}

namespace h2sl {
    /**
     * Feature_Container_Merge_Container_Type_Container class ostream operator
     */
    ostream&
    operator<<( ostream& out,
               const Feature_Container_Merge_Container_Type_Container& other ) {
      out << "class:\"Feature_Container_Merge_Container_Type_Container\" invert:(" << other.invert() << ") order:\"" << other.order() << "\")";  
      return out;
    }
    
}
