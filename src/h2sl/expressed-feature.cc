/**
 * @file expressed-feature.cc
 * @author h2sl developers
 *
 * @version 2.0
 *
 * @section license
 *
 * this file is part of h2sl.
 *
 * this program is free software; you can redistribute it and/or modify
 * it under the terms of the gnu general public license as published by
 * the free software foundation; either version 2 of the license, or (at
 * your option) any later version.
 * 
 * this program is distributed in the hope that it will be useful, but
 * without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the gnu
 * general public license for more details.
 * 
 * you should have received a copy of the gnu general public license
 * along with this program; if not, see
 * <http://www.gnu.org/licenses/gpl-2.0.html> or write to the free
 * software foundation, inc., 51 franklin street, fifth floor, boston, ma
 * 02110-1301, usa.
 *
 * @section description
 */

#include <fstream>
#include <sstream>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <boost/algorithm/string.hpp>

#include "h2sl/common.h"
#include "h2sl/feature.h"
#include "h2sl/expressed-feature.h"

namespace h2sl {

///
/// ExpressedFeature default class constructor
///
ExpressedFeature::ExpressedFeature( const double& weightArg,
                                      const unsigned int& indexArg,
                                      const std::vector< std::shared_ptr< Feature > >& featuresArg ) : weight( weightArg ),
                                                                                                        index( indexArg ),
                                                                                                        features( featuresArg ) {

}

///
/// Method to load from a file
///
bool ExpressedFeature::from_file( const std::string& filename ) {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return from_xml( filename );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    return from_json( filename );
  } else {
    std::cerr << "[ExpressedFeature Class Error] Erroneous file suffix (\"" << filename << "\") in from_file";
    return false;
  }
}

///
/// Method to load from an XML file
///
bool ExpressedFeature::from_xml( const std::string& filename ) {
  tinyxml2::XMLDocument document;
  document.LoadFile(filename.c_str());
  const tinyxml2::XMLElement *root = document.FirstChildElement("root");
  if (root != nullptr) {
    const tinyxml2::XMLElement *child = root->FirstChildElement();
    from_xml(child);
  }
  return document.ErrorID();
}

///
/// Method to load from a tinyxml2 element
///
bool ExpressedFeature::from_xml( const tinyxml2::XMLElement* tmp ){
  return true;
}

///
/// Method to load from a json formatted file
///
bool ExpressedFeature::from_json( const std::string& filename ){
  std::ifstream json_file( filename );
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_file, root );
  return from_json( root );
}

///
/// Method to load from a json formatted string
///
bool ExpressedFeature::from_json_string( const std::string& value ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( value, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool ExpressedFeature::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a file
///
void ExpressedFeature::to_file( const std::string& filename )const {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return to_xml( filename );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    return to_json( filename );
  } else {
    try {
      std::stringstream error_msg;
      error_msg << "[ExpressedFeature Class Error] Erroneous file suffix (\"" << filename << "\") in to_file";
      throw std::runtime_error( error_msg.str() );
    } catch ( const char* msg ){
      std::cerr << msg << std::endl;
    }
  }
  return;
}

///
/// Method to write to an XML file
///
void ExpressedFeature::to_xml( const std::string& filename )const {
  tinyxml2::XMLDocument document;
  tinyxml2::XMLElement * root = document.NewElement( "root" );
  document.InsertEndChild( root );
  to_xml( document, root );
  document.SaveFile( filename.c_str() );
  return;
}

///
/// Method to write to a tinyxml2 document
///
void ExpressedFeature::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* tmp = document.NewElement( "expressed-feature" );
  tmp->SetAttribute( "weight", h2sl::to_std_string( weight ).c_str() );
  tmp->SetAttribute( "index", h2sl::to_std_string( index ).c_str() );
  for( auto & feature : features ){
    feature->to_xml( document, tmp );
  }
  root->InsertEndChild( tmp );
}

///
/// Method to write to a json file
///
void ExpressedFeature::to_json( const std::string& filename )const{
  std::ofstream outfile;
  outfile.open( filename );
  outfile << to_json();
  outfile.close();
  return;
}

///
/// Method to write to a json formatted string
///
std::string ExpressedFeature::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void ExpressedFeature::to_json( Json::Value& root )const{
  return;
}

///
/// ExpressedFeature class ostream operator
///
/*
std::ostream& operator<<(std::ostream& out, const ExpressedFeature& other) {
  return out;
}
*/
} // namespace h2sl