/**
 * @file llm.cc
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
#include <cmath>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <boost/algorithm/string.hpp>

#include "h2sl/common.h"
#include "h2sl/llm.h"

namespace h2sl {

///
/// LLM parameter class constructor from a provided vector of weights
///
LLM::LLM( const std::vector< double >& weightsArg ) : feature_pool(), weights( weightsArg ) {

}

///
/// LLM parameter class constructor from a provided FeaturePool
///
LLM::LLM( const std::shared_ptr< FeaturePool >& featurePoolArg ) : feature_pool( featurePoolArg ), weights() {
  weights.resize( feature_pool->num_constituent_features() );
}

///
/// Method to compute the conditional probability function (Probability of Y Given X) approximated by the LLM
///
const LLM::pygx_result_t
LLM::pygx( const std::shared_ptr< std::string >& cv,
            const std::map< std::string, FeaturePool::indices_t >& indices,
            const bool store_sums )const{
  // Initialize the result container; This will hold the numerator/denominator
  auto result = pygx_result_t();
  if( store_sums )
    result.sum_of_weights = std::map< std::string, double >();

  // Iterate through each index
  for( const auto& [cv_key, cv_indices] : indices ){
    // Compute the sum of the weights
    double sum_of_weights = sum_of_feature_weights( cv_indices.feature_indices );
    if( store_sums ){
      // Store the sum of weights for the CV (cv_key)
      result.sum_of_weights->emplace( cv_key, sum_of_weights );
    }
    // Exponentiate the sum
    double exponentiated_sum = exp( sum_of_weights );
    // If the cv_key matches the Y cv, update the numerator
    if( *cv == cv_key )
      result.numerator = exponentiated_sum;
    // Always update the denominator
    result.denominator += exponentiated_sum;
  }
  // Compute the actual probability
  result.prob = result.numerator / result.denominator;
  if( std::isinf( result.denominator ) ){
    throw std::runtime_error("LLM::pygx() produced an infinity in the denominator.");
  }

  // Prevent 1 or 0
  if( result.prob == 0.0 ) result.prob = std::numeric_limits<double>::min();
  if( result.prob == 1.0 ) result.prob = (1.0 - std::numeric_limits<double>::min());

  return result;
}

/*
///
/// Method to compute the conditional probability function (Probability of Y Given X) approximate by the LLM
///
double LLM::pygx( std::shared_ptr<FeaturePool>& featureSpace,
                  std::shared_ptr< std::string >& cv,
                  const std::map< std::string, std::shared_ptr< std::string > >& cvs,
                  const std::shared_ptr<Symbol>& symbol,
                  const std::shared_ptr<LanguageVariable>& lv,
                  const std::shared_ptr<World>& world,
                  const bool& debug )const
{
  // iterate over all of the cv's and keep track of the sum to normalize the result later
  double numerator = 0.0;
  double denominator = 0.0;
  for( std::map< std::string, std::shared_ptr< std::string> >::const_iterator it_cvs = cvs.begin(); it_cvs != cvs.end(); it_cvs++ ){
    double tmp = exp( featureSpace->evaluate( it_cvs->second, lv, world, symbol, debug ) ); // TODO replace this with evaluation of the weighted features
    if( *cv == *(it_cvs->second) ){
      numerator = tmp;
    }
    denominator += tmp;
  }
  return ( numerator / denominator );
}

///
/// LLM probability of y (correspondence variable) given x (symbol, world, dialogue)
///
void LLM::pygxs( std::shared_ptr<FeaturePool>& featureSpace, std::vector< std::pair< std::shared_ptr<std::string>, double > >& cvs, const std::shared_ptr<Symbol>& symbol, const std::shared_ptr<LanguageVariable>& lv, const std::shared_ptr<World>& world )const{
  // iterate over all of the cv's and keep track of the sum to normalize the result later
  double sum = 0.0;
  for( std::vector< std::pair< std::shared_ptr<std::string>, double > >::iterator it_cvs = cvs.begin(); it_cvs != cvs.end(); it_cvs++ ){
    double tmp = exp( featureSpace->evaluate( it_cvs->first, lv, world, symbol ) ); // TODO replace this with evaluation of the weighted features
    it_cvs->second = tmp;
    sum += tmp;
  }

  // normalize the result by the sum of each of the weighted feature sums
  for( std::vector< std::pair< std::shared_ptr<std::string>, double > >::iterator it_cvs = cvs.begin(); it_cvs != cvs.end(); it_cvs++ ){
    it_cvs->second /= sum;
  }
  return;
}
*/

///
/// Method to compute the sum of feature weights for a provided set of vector index values
///
double LLM::sum_of_feature_weights( const std::vector< size_t >& indices )const{
  double sum = 0.0;
  for( const auto& feature_index : indices ){
    // Enforce a vector bounds check
    if( feature_index >= weights.size() )
      throw std::out_of_range("The feature weight index was out of bounds");
    // Add the weight
    sum += weights[feature_index];
  }
  return sum;
}

///
/// Method to compute the sum of feature weights for a provided vector of feature_indices_t
///
double LLM::sum_of_feature_weights( const FeaturePool::vecFeatureIndices& feature_indices )const{
  double sum = 0.0;
  for( const auto& feature_index : feature_indices ){
    // Enforce a vector bounds check
    if( feature_index.weight >= weights.size() )
      throw std::out_of_range("The feature weight index was out of bounds");
    // Add the weight
    sum += weights[ feature_index.weight ];
  }
  return sum;
}

///
/// Method to load from a file
///
bool LLM::from_file( const std::string& filename ) {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return from_xml( filename );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    return from_json( filename );
  } else {
      std::cout << "[LLM Class Error] Erroneous file suffix (\"" << filename << "\") in from_file" << std::endl;
    return false;
  }
}

///
/// Method to load from an XML file
///
bool LLM::from_xml( const std::string& filename ) {

  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename.c_str() );

  // Check for loading errors
  if( doc.Error() )
    return false;

  // Extract the root element from the document object
  const tinyxml2::XMLElement* root = doc.RootElement();
  if( root == nullptr )
    return false;

  // Check for any llm child element of the root
  const tinyxml2::XMLElement* llm_elem = root->FirstChildElement("llm");
  if( llm_elem == nullptr )
    return false;

  return from_xml( llm_elem );
}

///
/// Method to load from a tinyxml2 element
///
bool LLM::from_xml( const tinyxml2::XMLElement* llm_element ){
  feature_pool = nullptr;
  weights.clear();

  // Import the feature-pool
  const tinyxml2::XMLElement* feature_pool_elem = llm_element->FirstChildElement("feature-pool");
  if( feature_pool_elem == nullptr ){
    std::cout << "failed to find feature-pool" << std::endl;
    return false;
  }
  feature_pool = std::make_shared< FeaturePool >( feature_pool_elem );

  // import the weights
  const tinyxml2::XMLAttribute* weights_attr = llm_element->FindAttribute("weights");
  if( weights_attr == nullptr ){
    std::cout << "failed to find weights" << std::endl;
    return false;
  }

  weights = h2sl::double_vector_from_std_string( weights_attr->Value() );

  return true;
}

///
/// Method to load from a json formatted file
///
bool LLM::from_json( const std::string& filename ){
  std::ifstream json_file( filename );
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_file, root );
  return from_json( root );
}

///
/// Method to load from a json formatted string
///
bool LLM::from_json_string( const std::string& value ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( value, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool LLM::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a file
///
void LLM::to_file( const std::string& filename )const {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return to_xml( filename );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    return to_json( filename );
  } else {
    try {
      std::stringstream error_msg;
      error_msg << "[LLM Class Error] Erroneous file suffix (\"" << filename << "\") in to_file";
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
void LLM::to_xml( const std::string& filename )const {
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
void LLM::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* tmp = document.NewElement( "llm" );
  feature_pool->to_xml( document, tmp );
  std::stringstream weights_string;
  for( unsigned int i = 0; i < weights.size(); i++ ){
    weights_string << weights[i];
    if( i != ( weights.size() - 1 ) ){
      weights_string << ",";
    }
  }
  tmp->SetAttribute( "weights", weights_string.str().c_str() );
  root->InsertEndChild( tmp );
}

///
/// Method to write to a json file
///
void LLM::to_json( const std::string& filename )const{
  std::ofstream outfile;
  outfile.open( filename );
  outfile << to_json();
  outfile.close();
  return;
}

///
/// Method to write to a json formatted string
///
std::string LLM::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void LLM::to_json( Json::Value& root )const{
  return;
}

///
/// LLM class ostream operator
///
std::ostream& operator<<(std::ostream& out, const LLM& other) {

  out << "llm size: " << other.weights.size() << std::endl;
  for( auto weight : other.weights ){
    out << weight << ", ";
  }
  return out;
}

} // namespace h2sl
