/**
 * @file factor.cc
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

#include "h2sl/factor.h"

namespace h2sl {

///
/// Factor default class constructor
///
Factor::Factor( const std::shared_ptr<Symbol>& symbolArg,
                const std::shared_ptr<LanguageVariable>& lvArg,
                const std::shared_ptr<WorldDCG>& worldArg,
                const std::shared_ptr< std::vector< std::shared_ptr< std::string > > >& cvsArg,
                const FeatureSet& featureSet ) : symbol( symbolArg ),
                                                  language_variable( lvArg ),
                                                  world( worldArg ),
                                                  cvs( cvsArg ),
                                                  feature_set( featureSet ) {}

///
/// Factor class constructor that constructs a feature set from a feature-pool
///
Factor::Factor( const std::shared_ptr<FeaturePool>& featurePool,
                const std::shared_ptr<Symbol>& symbolArg,
                const std::shared_ptr<LanguageVariable>& lvArg,
                const std::shared_ptr<WorldDCG>& worldArg,
                const std::shared_ptr< std::vector< std::shared_ptr< std::string > > >& cvsArg,
	        const bool debug ) : symbol( symbolArg ),
				     language_variable( lvArg ),
				     world( worldArg ),
				     cvs( cvsArg ),
				     feature_set()
{
  // Get a reference to the featurePool's map of language features for the current language variable; Add one if it does not exist
  if( debug )
    std::cout << "language_variable->key():" << language_variable->key() << std::endl;
  const auto& [it_lv_features, lv_result] = featurePool->language_variable_features.emplace(
                                              language_variable->key(), std::map<std::string, std::shared_ptr<Feature>>() );
  auto& rmap_language_variable_features = it_lv_features->second;

  // Get a reference to the featurePool's map of symbol features for the current symbol; Add one if it does not exist
  if( debug )
    std::cout << "symbol->key():" << symbol->key() << std::endl;
  const auto& [it_symbol_features, symbol_result] =
                  featurePool->symbol_features.emplace( symbol->key(), std::map<std::string, std::shared_ptr<Feature>>() );
  auto& rmap_symbol_features = it_symbol_features->second;

  // Each constituent_feature_set holds the (shared_ptrs to) features for a single category in the cartesian product
  // I.e. The CV features, the Language features, and the Static/Dynamic Symbol features
  for( const auto & constituent_feature_set : featurePool->constituent_feature_sets ){
    // Add a new vector of shared_ptr features to be filled either with duplicates or copies
    feature_set.constituent_feature_sets.push_back( std::vector< std::shared_ptr<h2sl::Feature> >() );
    // Whether a feature is a dupe (new ownership) or a copy (shared ownership) depends on whether its evaluation is static across factors
    for( auto & p_feature : constituent_feature_set ){
      // Copy the correspodence variable features since they will be evaluated every time
      if( p_feature->type == FeatureType::CorrespondenceVariable  ){
        feature_set.constituent_feature_sets.back().push_back( p_feature );
      } else if( p_feature->type == FeatureType::Language ){
        // Since all Factors in a FactorSet (column) share the same language variable, this feature can be shared across those Factors
        // Check the map of language_variable_features to see if it has already been added from the feature-pool
        // If it has, use a copy of it; If it has not, add a duplicate of it from the feature-pool to the map of language_variable_features
        auto const cit_language_variable_feature = rmap_language_variable_features.find( p_feature->key() );
        if( cit_language_variable_feature != rmap_language_variable_features.cend() ){
          // The feature is in the map, so add a copy of it to the feature_set
	  if( debug )
            std::cout << "\tLoading existing feature " << cit_language_variable_feature->second->print_string() << " (" << cit_language_variable_feature->second << ")" << std::endl;
          feature_set.constituent_feature_sets.back().push_back( cit_language_variable_feature->second );
        } else {
          // The feature is not in the map, so add a duplicate to the map and then a copy of the duplicate to the feature_set
          const auto& [ it_language_variable_feature_dup, result ] = rmap_language_variable_features.emplace( p_feature->key(), p_feature->dup() );
          feature_set.constituent_feature_sets.back().push_back( it_language_variable_feature_dup->second );
	  if( debug )
            std::cout << "\tAdding new feature " << it_language_variable_feature_dup->second->print_string() << " (" << it_language_variable_feature_dup->second << ")" << std::endl;
        }
      } else if( p_feature->type == FeatureType::DynamicSymbol ){
        // Since all Factors in a FactorSet (column) have different symbols, DynamicSymbol features cannot be shared across those Factors
        auto const cit_symbol_feature = rmap_symbol_features.find( p_feature->key() );
        if( cit_symbol_feature != rmap_symbol_features.cend() ){
	  if( debug )
            std::cout << "\tDuplicating existing feature " << cit_symbol_feature->second->print_string() << " (" << cit_symbol_feature->second << ")" << std::endl;
          feature_set.constituent_feature_sets.back().push_back( cit_symbol_feature->second->dup() );
        } else{
          const auto& [ it_symbol_feature_dup, result ] = rmap_symbol_features.emplace( p_feature->key(), p_feature->dup() );
          feature_set.constituent_feature_sets.back().push_back( it_symbol_feature_dup->second );
	  if( debug )
            std::cout << "\tAdding new feature " << it_symbol_feature_dup->second->print_string() << " (" << it_symbol_feature_dup->second << ")" << std::endl;
        }
      } else if( p_feature->type == FeatureType::StaticSymbol ) {
        // determine if a new instance of this feature is stored for the particular symbol to streamline similar features across a symbol
        auto const cit_symbol_feature = rmap_symbol_features.find( p_feature->key() );
        if( cit_symbol_feature != rmap_symbol_features.cend() ){
	  if( debug )
            std::cout << "\tLoading existing feature " << cit_symbol_feature->second->print_string() << " (" << cit_symbol_feature->second << ")" << std::endl;
          // found the feature already allocated in the symbol_features maps, load directly into the feature set
          feature_set.constituent_feature_sets.back().push_back( cit_symbol_feature->second );
        } else {
          // insert the feature in the symbol_features maps, then load into the feature set
          const auto& [ it_symbol_feature_dup, result ] =  rmap_symbol_features.emplace( p_feature->key(), p_feature->dup() );
          feature_set.constituent_feature_sets.back().push_back( it_symbol_feature_dup->second );
	  if( debug )
            std::cout << "  adding new feature " << it_symbol_feature_dup->second->print_string() << " (" << it_symbol_feature_dup->second << ")" << std::endl;
        }
      }
    }
  }
  if( debug )
    std::cout << "num_constituent_features:" << feature_set.num_constituent_features() << std::endl;
}

///
/// Method to evaluate the Factor's FeatureSet using the members of the Factor
///
void Factor::evaluate(std::map< std::string, std::vector< ExpressedFeature > >& expressedFeatures, const bool& debug ){
  indices = feature_set.evaluate( cvs, symbol, language_variable, world, expressedFeatures, debug );
  return;
}

///
/// Method to load from a file
///
bool Factor::from_file( const std::string& filename ) {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return from_xml( filename );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    return from_json( filename );
  } else {
    std::cerr << "[Factor Class Error] Erroneous file suffix (\"" << filename << "\") in from_file";
    return false;
  }
}

///
/// Method to load from an XML file
///
bool Factor::from_xml( const std::string& filename ) {
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
bool Factor::from_xml( const tinyxml2::XMLElement* tmp ){
  return true;
}

///
/// Method to load from a json formatted file
///
bool Factor::from_json( const std::string& filename ){
  std::ifstream json_file( filename );
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_file, root );
  return from_json( root );
}

///
/// Method to load from a json formatted string
///
bool Factor::from_json_string( const std::string& value ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( value, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool Factor::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a file
///
void Factor::to_file( const std::string& filename )const {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return to_xml( filename );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    return to_json( filename );
  } else {
    try {
      std::stringstream error_msg;
      error_msg << "[Factor Class Error] Erroneous file suffix (\"" << filename << "\") in to_file";
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
void Factor::to_xml( const std::string& filename )const {
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
void Factor::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* tmp = document.NewElement( "factor" );
  root->InsertEndChild( tmp );
}

///
/// Method to write to a json file
///
void Factor::to_json( const std::string& filename )const{
  std::ofstream outfile;
  outfile.open( filename );
  outfile << to_json();
  outfile.close();
  return;
}

///
/// Method to write to a json formatted string
///
std::string Factor::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void Factor::to_json( Json::Value& root )const{
  return;
}

///
/// Factor class ostream operator
///
std::ostream& operator<<(std::ostream& out, const Factor& other) {
  out << "symbol:{" << *other.symbol << "},language_variable:{" << *other.language_variable << "},feature_set:{" << other.feature_set << "}";
  return out;
}

} // namespace h2sl
