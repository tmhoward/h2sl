/**
 * @file feature_matches_ordered_child.ipp
 *
 * @brief
 *
 * a template used to describe a feature that checks the value of a type field
 */

/**
 * Feature_Matches_Ordered_Child class constructor
 */
template< class T >
Feature_Matches_Ordered_Child< T >::
Feature_Matches_Ordered_Child( const bool& invert,
                                const unsigned int& order ) : Feature( invert ),
                                                              _order( order ) {

}

/**
 * Feature_Matches_Ordered_Child class constructor
 */
template< class T >
Feature_Matches_Ordered_Child< T >::
Feature_Matches_Ordered_Child( xmlNodePtr root ) : Feature(),
                                                    _order( 0 ) {
  from_xml( root );
}

/**
 * Feature_Matches_Ordered_Child class copy constructor
 */
template< class T >
Feature_Matches_Ordered_Child< T >::
Feature_Matches_Ordered_Child( const Feature_Matches_Ordered_Child< T >& other ) : Feature( other ),
                                                                                    _order( other._order ) {

}

/**
 * Feature_Matches_Ordered_Child class destructor
 */
template< class T >
Feature_Matches_Ordered_Child< T >::
~Feature_Matches_Ordered_Child() {

}

/** 
 * Feature_Matches_Ordered_Child class assignment operator
 */
template< class T >
Feature_Matches_Ordered_Child< T >&
Feature_Matches_Ordered_Child< T >::
operator=( const Feature_Matches_Ordered_Child& other ){
  _invert = other._invert;
  _order = other._order;
  return (*this);
}

template< class T >
bool
Feature_Matches_Ordered_Child< T >::
value( const std::string& cv,
        const Grounding* grounding,
        const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

template< class T >
bool
Feature_Matches_Ordered_Child< T >::
value( const std::string& cv,
        const Grounding* grounding,
        const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const T * symbol = dynamic_cast< const T* >( grounding );
  if( symbol != NULL ){
    if( _order < children.size() ){
      for( unsigned int j = 0; j < children[ _order ].second.size(); j++ ){
        const T * child = dynamic_cast< const T* >( children[ _order ].second[ j ] );
        if( child != NULL ){
          if( *symbol == *child ){
            return !_invert;
          }
        }
      }
    }
    if( !children.empty() ){
      return _invert;
    }
  }
  return false;
}

/** 
 * imports the Feature_Matches_Ordered_Child class from an XML file
 */
template< class T >
void
Feature_Matches_Ordered_Child< T >::
from_xml( const std::string& filename ){
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( filename.c_str(), NULL, 0 );
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      xmlNodePtr l1 = NULL;
      for( l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( std::string( "feature_matches_ordered_child_" + T::class_name() ).c_str() ) ) == 0 ){
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
 * imports the Feature_Matches_Ordered_Child class from an XML node pointer
 */
template< class T >
void
Feature_Matches_Ordered_Child< T >::
from_xml( xmlNodePtr root ){
  _invert = false;
  _order = 0;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      std::string invert_string = ( char* )( tmp );
      _invert = ( bool )( std::strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
  
    std::pair< bool, int > order_prop = has_prop< int >( root, "order" );
    if( order_prop.first ) {
      _order = order_prop.second;
    }
  }
  return;
}

/**
 * exports the Feature_Matches_Ordered_Child class to an XML file
 */
template< class T >
void
Feature_Matches_Ordered_Child< T >::
to_xml( const std::string& filename )const{
  xmlDocPtr doc = xmlNewDoc( ( xmlChar* )( "1.0" ) );
  xmlNodePtr root = xmlNewDocNode( doc, NULL, ( xmlChar* )( "root" ), NULL );
  xmlDocSetRootElement( doc, root );
  to_xml( doc, root );
  xmlSaveFormatFileEnc( filename.c_str(), doc, "UTF-8", 1 );
  xmlFreeDoc( doc );
  return;
}

/**
 * exports the Feature_Matches_Ordered_Child class to an XML node pointer
 */
template< class T >
void
Feature_Matches_Ordered_Child< T >::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( std::string( "feature_matches_ordered_child_" + T::class_name() ).c_str() ), NULL );
  std::stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  xmlNewProp( node, ( const xmlChar* )( "order" ), ( const xmlChar* )( to_std_string( _order ).c_str() ) );
  xmlAddChild( root, node );
  return;
}

/** 
 * Feature_Matches_Ordered_Child class ostream operator
 */
template< class T >
std::ostream&
operator<<( std::ostream& out,
            const Feature_Matches_Ordered_Child< T >& other ){
  out << "Feature_Matches_Ordered_Child:( invert:\"" << other.invert() << "\")";
  return out;
}
