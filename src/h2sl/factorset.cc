/**
 * @file factorset.cc
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

#include "h2sl/factorset.h"

namespace h2sl {

///
/// Function to sort the solutions by the probability of that solution
///
bool solutions_sort( const FactorSet::fs_search_solution_t& a, const FactorSet::fs_search_solution_t& b ){
  return a.prob > b.prob;
}

///
/// FactorSet default class constructor
///
FactorSet::FactorSet( const std::shared_ptr<FeaturePool>& featurePool,
                      const std::shared_ptr<SymbolSpace>& symbolSpace,
		                  const std::shared_ptr<LanguageVariable>& lvArg,
                      const std::shared_ptr<WorldDCG>& worldArg,
                      const std::shared_ptr< std::vector< std::shared_ptr< std::string > > >& cvsArg ) : factors(),
                                                                                                          language_variable( lvArg ),
                                                                                                          children(),
                                                                                                          solutions() {
  for( auto symbolvectors : symbolSpace->symbolsMap ){
    for( auto symbol : symbolvectors.second ){
      factors.push_back( std::make_shared<Factor>( featurePool, symbol, language_variable, worldArg, cvsArg ) );
    }
  }
}

///
/// Method to search through the factors to update the solutions vector
///
const FactorSet::vecFSSearchSolutions&
FactorSet::search( const std::shared_ptr<LLM>& llm, const unsigned int beamwidth, const bool debug ){
  // Clear any old solutions and initialize with a probability of 1
  solutions.clear();
  solutions.emplace_back( 1.0, std::make_shared<LanguageVariable>( language_variable->current() ) );  
  // Create the set of possible child solutions over which to search
  for( const auto& child_connection : children ){
    if( debug ){
      std::cout << "Adding child solutions for the factor with LV \"" << *(child_connection.factorset->language_variable) << "\"\n"
    		<< "Current solutions.size(): " << solutions.size() << std::endl;
    }
    vecFSSearchSolutions new_solutions;
    for( const auto& solution : solutions ){
      if( debug )
        std::cout << "Updating the solution with prob \"" << solution.prob << "\" and LV: " << *solution.language_variable << std::endl;
      for( const auto & child_solution : child_connection.factorset->solutions ){
        new_solutions.emplace_back( solution.prob, std::make_shared<h2sl::LanguageVariable>( *solution.language_variable ) );
	if( debug ){
          std::cout << "Adding the child solution with prob \"" << child_solution.prob << "\" and LV: " << *child_solution.language_variable << std::endl;
          std::cout << "Before: " << new_solutions.back().prob << "  " << *new_solutions.back().language_variable << std::endl;
	}
        new_solutions.back().prob *= child_solution.prob;
        new_solutions.back().language_variable->children.emplace_back( child_connection.label, child_solution.language_variable );
	if( debug )
          std::cout << "After: " << new_solutions.back().prob << "  " << *new_solutions.back().language_variable << std::endl;
      }
    }
    solutions = new_solutions;
  }

  if( debug )
    std::cout << "Solutions size:" << solutions.size() << std::endl;
  
  // iterate through each of the factors
  for( auto& p_factor : factors ){
    if( debug )
      std::cout << "\tEvaluating factor with symbol \"" << *(p_factor->symbol) << "\"" << std::endl;

    // keep a vector of new solutions to add after iterating through the factor
    vecFSSearchSolutions new_solutions;
    // initially use a placeholder for what will eventually be the llm pygx function
    for( auto& solution : solutions ){
      if( debug )
        std::cout << "\tEvaluating factor with solution \"" << solution.prob << ":" << *solution.language_variable << "\"" << std::endl;
      // evaluate the probabilities of each correspondence variable case together for efficiency
      // update the language variable to look at the current solution
      p_factor->language_variable = solution.language_variable;
      // generate the list of expressed features
      std::map< std::string, std::vector< ExpressedFeature > > expressed_features;
      p_factor->evaluate( expressed_features ); 

      // update the expressed feature weights
      for( auto& it_expressed_features : expressed_features ){
        for( auto & it_expressed_feature : it_expressed_features.second ){
          it_expressed_feature.weight = llm->weights[ it_expressed_feature.index ];
        }
      }

      // add the new solutions before modifying the probabilty of the unchanged solution
      for( const auto& cv : *p_factor->cvs ){
        if( *cv == "true" ){
          // Compute the pygx for the current factor; Include the sum_of_weights in the result; Store the result in the factor
          p_factor->pygx_result = llm->pygx( cv, p_factor->indices, true );
	  if( debug ){
            std::cout << "sum of weights:";
            if( p_factor->pygx_result->sum_of_weights ){
              for( const auto& [cv_key, sum_of_weights] : *(p_factor->pygx_result->sum_of_weights) ){
                std::cout << "\n\t" << cv_key << ":\t" << sum_of_weights; 
              }
            }
            std::cout << "\nnumerator: " << p_factor->pygx_result->numerator << "\ndenominator: "
                      << p_factor->pygx_result->denominator << std::endl;
          }
          // add a new solution as a copy of the current solution with the symbol added
          new_solutions.emplace_back( solution.prob, std::make_shared<h2sl::LanguageVariable>( *solution.language_variable ) );
          new_solutions.back().language_variable->symbols.push_back( std::make_shared<Symbol>( *p_factor->symbol ) );
          new_solutions.back().language_variable->symbols.back()->expressed_features = expressed_features;
          new_solutions.back().prob *= p_factor->pygx_result.value().prob;
	  if( debug )
            std::cout << "\t\tpygx:" << new_solutions.back().prob << " (added " << *p_factor->symbol << ")" << std::endl;
        }
      }

      // update the probability of the unchanged solution (cv false => factor's symbol is not added)
      for( const auto& cv : *p_factor->cvs ){
        if( *cv == "false" ){
          auto result = llm->pygx( cv, p_factor->indices );
          solution.prob *= result.prob;
	  if( debug )
            std::cout << *cv << "    pygx:" << solution.prob << std::endl;
        }
      }
    }

    // store this factor's new solutions once all have been considered
    for( const auto& new_solution : new_solutions ){
      solutions.push_back( new_solution );
    }

    // Sort the solutions to prepare the beam pruning
    sort( solutions.begin(), solutions.end(), solutions_sort );

    // print out current solutions before pruning the lowest probability solutions
    if( debug ){
      std::cout << "before pruning" << std::endl;
      std::cout << "  num solutions:" << solutions.size() << std::endl;
      for( const auto& solution : solutions ){
        std::cout << "    solution(" << solution.prob << "):" << *solution.language_variable << std::endl;
      }
      std::cout << std::endl;
    }
    // prune off any solutions beyond the beam width
    if( solutions.size() > beamwidth ){
      solutions.erase( solutions.begin() + beamwidth, solutions.end() );
    }

    // print out the solutions again after pruning the lowest probability solutions
    if( debug ){
      std::cout << "after pruning" << std::endl;
      std::cout << "\tnum solutions:" << solutions.size() << std::endl;
      for( const auto & solution : solutions ){
        std::cout << "    solution(" << solution.prob << "):" << *solution.language_variable << std::endl;
      }
      std::cout << std::endl;
    }
  } // end loop over factors 
  return solutions;
}

///
/// Method to load from a file
///
bool FactorSet::from_file( const std::string& filename ) {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return from_xml( filename );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    return from_json( filename );
  } else {
    std::cerr << "[FactorSet Class Error] Erroneous file suffix (\"" << filename << "\") in from_file";
    return false;
  }
}

///
/// Method to load from an XML file
///
bool FactorSet::from_xml( const std::string& filename ) {
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
bool FactorSet::from_xml( const tinyxml2::XMLElement* tmp ){
  return true;
}

///
/// Method to load from a json formatted file
///
bool FactorSet::from_json( const std::string& filename ){
  std::ifstream json_file( filename );
  Json::Reader reader;
  Json::Value root;
  reader.parse( json_file, root );
  return from_json( root );
}

///
/// Method to load from a json formatted string
///
bool FactorSet::from_json_string( const std::string& value ){
  Json::Reader reader;
  Json::Value root;
  reader.parse( value, root );
  return from_json( root );
}

///
/// Method to load from a json value
///
bool FactorSet::from_json( const Json::Value& root ){
  return true;
}

///
/// Method to write to a file
///
void FactorSet::to_file( const std::string& filename )const {
  if( boost::algorithm::ends_with( filename, "xml" ) ){
    return to_xml( filename );
  } else if ( boost::algorithm::ends_with( filename, "json" ) ){
    return to_json( filename );
  } else {
    std::cerr << "[FactorSet Class Error] Erroneous file suffix (\"" << filename << "\") in to_file";
    return;
  }
}

///
/// Method to write to an XML file
///
void FactorSet::to_xml( const std::string& filename )const {
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
void FactorSet::to_xml( tinyxml2::XMLDocument& document, tinyxml2::XMLElement* root )const{
  tinyxml2::XMLElement* tmp = document.NewElement( "factor-set" );
  root->InsertEndChild( tmp );
}

///
/// Method to write to a json file
///
void FactorSet::to_json( const std::string& filename )const{
  std::ofstream outfile;
  outfile.open( filename );
  outfile << to_json();
  outfile.close();
  return;
}

///
/// Method to write to a json formatted string
///
std::string FactorSet::to_json( void )const{
  Json::Value root;
  to_json( root );
  Json::FastWriter writer;
  return writer.write( root );
}

///
/// Method to write to a json value
///
void FactorSet::to_json( Json::Value& root )const{
  return;
}

///
///
///
unsigned int FactorSet::num_children_with_solutions( void )const{
  unsigned int tmp = 0;
  for( const auto& child_connection : children ){
    if( !child_connection.factorset->solutions.empty() ){
      tmp++;
    }
  }
  return tmp;
}

///
/// FactorSet class ostream operator
///
std::ostream& operator<<(std::ostream& out, const FactorSet& other) {

  return out;
}

} // namespace h2sl
