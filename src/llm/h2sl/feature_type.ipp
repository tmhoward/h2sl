/**
 * @file feature_type.ipp
 *
 * @brief
 *
 * a template used to describe a feature that checks the value of a type field
 */

/**
 * Feature_Type class constructor
 */
template< class T >
Feature_Type< T >::
Feature_Type( const bool& invert,
              const unsigned int& symbolType ) : Feature( invert ),
                                                  _symbol_type( symbolType ){

}

/**
 * Feature_Type class constructor
 */
template< class T >
Feature_Type< T >::
Feature_Type( xmlNodePtr root  ): Feature(),
                                  _symbol_type( 0 ){
  from_xml( root );
}

/**
 * Feature_Type class copy constructor
 */
template< class T >
Feature_Type< T >::
Feature_Type( const Feature_Type< T >& other ) : Feature( other ),
                                                  _symbol_type( other._symbol_type ){

}

/**
 * Feature_Type class destructor
 */
template< class T >
Feature_Type< T >::
~Feature_Type() {

}

/** 
 * Feature_Type class assignment operator
 */
template< class T >
Feature_Type< T >&
Feature_Type< T >::
operator=( const Feature_Type& other ){
  _invert = other._invert;
  _symbol_type = other._symbol_type;
  return (*this);
}

template< class T >
bool
Feature_Type< T >::
value( const std::string& cv,
        const Grounding* grounding,
        const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world ){
  return value( cv, grounding, children, phrase, world, NULL );
}

template< class T >
bool
Feature_Type< T >::
value( const std::string& cv,
        const Grounding* grounding,
        const std::vector< std::pair< const Phrase*, std::vector< Grounding* > > >& children,
        const Phrase* phrase,
        const World* world,
        const Grounding* context ){
  const T * symbol = dynamic_cast< const T* >( grounding );
  if( symbol != NULL ){
    if( symbol->type() == _symbol_type ){
      return !_invert;
    }
  }
  return false;
}

/** 
 * imports the Feature_Type class from an XML file
 */
template< class T >
void
Feature_Type< T >::
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
          if( xmlStrcmp( l1->name, ( const xmlChar* )( std::string( "feature_type_" + T::class_name() ).c_str() ) ) == 0 ){
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
 * imports the Feature_Type class from an XML node pointer
 */
template< class T >
void
Feature_Type< T >::
from_xml( xmlNodePtr root ){
  _invert = false;
  _symbol_type = 0;
  if( root->type == XML_ELEMENT_NODE ){
    xmlChar * tmp = xmlGetProp( root, ( const xmlChar* )( "invert" ) );
    if( tmp != NULL ){
      std::string invert_string = ( char* )( tmp );
      _invert = ( bool )( std::strtol( invert_string.c_str(), NULL, 10 ) );
      xmlFree( tmp );
    }
    tmp = xmlGetProp( root, ( const xmlChar* )( std::string( T::class_name() + "_type" ).c_str() ) );
    if( tmp != NULL ){
      std::string symbol_type_string = ( char* )( tmp );
      _symbol_type = T::type_from_std_string( symbol_type_string );
      xmlFree( tmp );
    }
  }
  return;
}

/**
 * exports the Feature_Type class to an XML file
 */
template< class T >
void
Feature_Type< T >::
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
 * exports the Feature_Type class to an XML node pointer
 */
template< class T >
void
Feature_Type< T >::
to_xml( xmlDocPtr doc,
        xmlNodePtr root )const{
  xmlNodePtr node = xmlNewDocNode( doc, NULL, ( xmlChar* )( std::string( "feature_type_" + T::class_name() ).c_str() ), NULL );
  std::stringstream invert_string;
  invert_string << _invert;
  xmlNewProp( node, ( const xmlChar* )( "invert" ), ( const xmlChar* )( invert_string.str().c_str() ) );
  std::stringstream symbol_type_string;
  symbol_type_string << T::type_to_std_string( _symbol_type );
  xmlNewProp( node, ( const xmlChar* )( std::string( T::class_name() + "_type" ).c_str() ), ( const xmlChar* )( symbol_type_string.str().c_str() ) );
  xmlAddChild( root, node );
  return;
}

/** 
 * Feature_Type class ostream operator
 */
template< class T >
std::ostream&
operator<<( std::ostream& out,
            const Feature_Type< T >& other ){
  out << "Feature_Type:( class_name:\"" << T::class_name() << "\" invert:\"" << other.invert() << "\" symbol_type:\"" << T::type_to_std_string( other.symbol_type() ) << "\" )";
  return out;
}
