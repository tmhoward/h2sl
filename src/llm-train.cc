/**
 * @file llm-train.cc
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

#include <chrono>

#include <boost/program_options.hpp>

#include <lbfgs.h>

#include "h2sl/common.h"
#include "h2sl/world.h"
#include "h2sl/dcg.h"
#include "h2sl/example.h"
#include "h2sl/feature-pool.h"
#include "h2sl/symbolspace.h"

/**
  A structure to contain relevant function data to be used by the NLopt optimizer object

  p_llm: pointer to the log-linear model for computing the objective and the gradient
  examples: the vector of examples over which the objective is computed
  lambda: the hyper-parameter value for tuning the regularization term of the objective function & gradient
  debug: optional flag for verbose debug printouts
**/
struct func_data_t{
  std::shared_ptr<h2sl::LLM> p_llm;
  std::vector< std::shared_ptr<h2sl::Example> > examples;
  double lambda;
  bool debug;
};

/**
  Method to print a map of std::vector< unsigned int > values
**/
inline std::ostream& operator<<( std::ostream& out, const std::map< std::string, std::vector< unsigned int > >& other ){
  out << "[" << other.size() << "]:{";
  for( auto it_other = other.cbegin(); it_other != other.cend(); ++it_other ){
    out << it_other->first << "[" << it_other->second.size() << "]:{";
    for( auto it_other_elem = it_other->second.cbegin(); it_other_elem != it_other->second.cend(); ++it_other_elem ){
      out << *it_other_elem;
      if( std::next( it_other_elem ) != it_other->second.cend() ){
        out << ",";
      }
    }
    out << "}";
    if( std::next( it_other ) != other.cend() ){
      out << ",";
    }
  }
  out << "}";
  return out;
}

/**
  Method to load an instruction field from an XML file
**/
std::optional< std::string > instruction_from_xml( const std::string& filename ){

  // Load the file into a document object
  tinyxml2::XMLDocument doc;
  doc.LoadFile( filename.c_str() );

  // Check for loading errors
  if( doc.Error() ){
    std::cout << "xml document had loading errors in instruction_from_xml( " << filename << ")" << std::endl;
    return std::nullopt;
  }

  // Extract the root element from the document object
  const tinyxml2::XMLElement* root = doc.RootElement();
  if( root == nullptr ){
    std::cout << "No root element found in instruction_from_xml( " << filename << ")" << std::endl;
    return std::nullopt;
  }

  // Check for any instruction child element of the root
  const tinyxml2::XMLElement* instruction_elem = root->FirstChildElement("instruction");
  if( instruction_elem == nullptr ){
    std::cout << "No instruction element found in instruction_from_xml( " << filename << ")" << std::endl;
    return std::nullopt;
  }

  // Read the instruction attribute and return it
  const tinyxml2::XMLAttribute* text_attr = instruction_elem->FindAttribute("text");
  if( text_attr == nullptr ){
    std::cout << "Failed to find instruction text" << std::endl;
    return std::nullopt;
  }
  return std::make_optional< std::string >( text_attr->Value() );
}



/**
  Method to recursively extract the individual Factor-level examples from a provided h2sl::LanguageVariable and its children. The LV
  has associated labeled symbols that indicate a TRUE correspondence. For all other symbols, it is treated as a FALSE correspondence
  example.

  @brief Extract the Factor-level examples from the provided LanguageVariable.
  @param[in]    filename        The filename of the example file - this is useful for debug printouts
  @param[in]    instruction     The instruction associated with the example
  @param[in]    cvs             The correspondence variable string values
  @param[in]    p_lv            The LanguageVariable being scraped for examples
  @param[in]    p_world         The world associated with the provided language variable
  @param[in]    p_symbolspace   The symbolspace associated with the LV given the world and symbol dictionary
  @param[in]    examples        The vector to store extracted examples
  @param[in]    debug           Optional flag for verbose debug printouts
  @returns                      none
  @throws                       no expected throws
**/
void scrape_examples( const std::string& filename,
                      const std::string& instruction,
                      const std::shared_ptr<std::vector<std::shared_ptr<std::string>>>& cvs,
                      const std::shared_ptr<h2sl::LanguageVariable>& p_lv,
                      const std::shared_ptr<h2sl::World>& p_world,
                      const std::shared_ptr<h2sl::SymbolSpace>& p_symbolspace,
                      std::vector< std::shared_ptr<h2sl::Example> >& examples,
                      const bool debug = false )
{
  // Store the true/false CV string ptrs for easy use
  std::shared_ptr<std::string> p_cv_true = nullptr;
  std::shared_ptr<std::string> p_cv_false = nullptr;
  for( auto cv : *cvs ){
    if( (*cv) == "true" ){
      p_cv_true = cv;
    } else if ( (*cv) == "false" ){
      p_cv_false = cv;
    }
  }

  // Create an example for each symbol in the symbolspace; TRUE if part of the LV, False otherwise
  if( debug ) std::cout << "Checking the language variable: " << *p_lv << std::endl;
  for( const auto& [symbol_type, v_symbols] : p_symbolspace->symbolsMap ){
    for( const auto& symbol : v_symbols ){
      if( debug ) std::cout << "\tChecking the symbol " << *symbol << std::endl;
      bool found_match = false;
      for( const auto& lv_symbol : p_lv->symbols() ){
        if( debug ) std::cout << "\t\tComparing to the LV's symbol " << *lv_symbol << std::endl;
        if( *lv_symbol == *symbol ){
          found_match = true;
          break;
        }
      }
      if( found_match ){
        if( debug ) std::cout << "\t\tFound a match" << std::endl;
        examples.push_back( std::make_shared<h2sl::Example>( filename, instruction, p_cv_true, cvs, p_lv, p_world, symbol ) );
      } else {
        if( debug ) std::cout << "\t\tNo match found" << std::endl;
        examples.push_back( std::make_shared<h2sl::Example>( filename, instruction, p_cv_false, cvs, p_lv, p_world, symbol ) );
      }
    }
  }

  // Recursively scrape examples from any LanguageVariable children
  if( debug ) std::cout << "Now scraping examples from language variable children..." << std::endl;
  for( const auto& child_connection : p_lv->children() ){
    scrape_examples( filename, instruction, cvs, child_connection.child, p_world, p_symbolspace, examples, debug );
  }
  return;
}

/**
  Method to compute the objective for the provided examples and the given log-linear model parameters. The objective function
  is the sum of log-likelihood over all examples given the provided log-linear model parameters. An optional lambda hyper-parameter
  can be provided to tune the regularization term of l2-norm ( 0.5*lambda*l2-norm ).

  @brief Compute the objective
  @param[in]    p_llm       The current log-linear model with which to compute each example's likelihood
  @param[in]    examples    The examples
  @param[in]    lambda      The hyper-parameter of the regularization term (default is 0.0)
  @returns                  The value of the computed objective (sum of log-likelihood)
  @throws                   no expected throws
**/
double compute_objective( std::shared_ptr<h2sl::LLM>& p_llm,
                          const std::vector< std::shared_ptr<h2sl::Example> >& examples,
                          const double lambda = 0.0 )
{
  // Compute the sum of log-likelihoods for all examples
  double sum_of_log_prob = 0.0;
  for( const auto& example : examples ){
    auto result = p_llm->pygx( example->cv, example->indices );
    sum_of_log_prob += log( result.prob );
  }
  // Compute the regularization term: 0.5 * lambda * l2_norm( weights )
  double l2_norm_weights = 0.0;
  for( const auto& weight : p_llm->weights ){
    l2_norm_weights += ( weight*weight );
  }
  // Return the objective
  return sum_of_log_prob - (lambda/2.0)*l2_norm_weights;
}

/**
  Method to numerically compute a gradient for each feature weight parameter. Two values of the objective are computed with a small
  parameter perturbation, and the difference is treated as an approximation of the gradient.

  @brief Compute numerical gradient
  @param[in]    p_llm       The current log-linear model with which to compute the objective
  @param[in]    examples    The examples
  @returns                  The computed gradient
  @throws                   no expected throws
**/
std::vector< double >
compute_numerical_gradient( std::shared_ptr<h2sl::LLM>& p_llm, const std::vector< std::shared_ptr<h2sl::Example> >& examples ){
  std::vector< double > gradient( p_llm->weights.size(), 0.0 );
  double delta = 0.001;
  double base = compute_objective( p_llm, examples );
  for( size_t i = 0; i < p_llm->weights.size(); ++i ){
    p_llm->weights[i] += delta;
    gradient[i] = ( compute_objective( p_llm, examples ) - base ) / delta;
    p_llm->weights[i] -= delta;
  }
  return gradient;
}

/**
  Method to analytically compute a gradient for each feature weight parameter. The gradient function consists of 3 terms:
    Gradient = X1 - X2 - X3
    X1: the frequency of activation for a feature over all examples
    X2: the expected frequency of activitation given the current model parameters
    X3: the regularization term (partial derivative of the l2-norm regularization of the objective function)

  @brief Compute the analytical gradient
  @param[in]    p_llm       The current log-linear model
  @param[in]    examples    The examples
  @param[in]    lambda      The hyper-parameter of the regularization term (default is 0.0)
  @returns                  The computed gradient
  @throws                   no expected throws
**/
std::vector< double > compute_analytical_gradient( std::shared_ptr<h2sl::LLM>& p_llm,
                                                   const std::vector< std::shared_ptr<h2sl::Example> >& examples,
                                                   const double lambda = 0.0 )
{
  // Initialize the return object with the size of the feature weight parameter vector
  std::vector< double > gradient( p_llm->weights.size(), 0.0 );

  // Compute the frequency and expected frequency of each feature (the X1 and X2 terms)
  for( const auto& example : examples ){
    for( const auto& cv : *example->cvs ){
      auto result = p_llm->pygx( cv, example->indices );
      auto const cit_indices = example->indices.find( *cv );
      // Update the X2 term
      for( const auto& feature_index : cit_indices->second.feature_indices ){
        gradient[ feature_index.weight ] -= result.prob;
      }
      // Update the X1 Term
      if( *cv == *example->cv ){
        for( const auto& feature_index : cit_indices->second.feature_indices ){
          gradient[ feature_index.weight ] += 1.0;
        }
      }
    }
  }

  // Apply the regularization term (X3)
  for( size_t i = 0; i < gradient.size(); ++i ){
    gradient[i] -= lambda*p_llm->weights[i];
  }
  return gradient;
}

/**
  Method to be used by the LBFGS optimizer object to update the log-linear model weight parameters.

  @brief  LBFGS evaluate method to update LLM weights
  @param[in]    func_data     A structure to provide the LLM and other parameters that are not part of the function definition
  @param[in]    x             A c-style array of parameter values
  @param[in]    g             A c-style array for parameter gradient values
  @param[in]    n             The number of examples
  @param[in]    step          The line-search step used for this iteration
  @returns                    The negated objective value
  @throws                     No expected throws
**/
lbfgsfloatval_t evaluate( void* func_data,
                          const lbfgsfloatval_t* x,
                          lbfgsfloatval_t* g,
                          const int n,
                          const lbfgsfloatval_t step ){

  // Cast the extra function data for use
  func_data_t* data = reinterpret_cast< func_data_t* >( func_data );

  // Copy x into weights
  for( size_t i = 0; i < n; ++i ){
    data->p_llm->weights[ i ] = x[ i ];
  }

  // compute objective
  lbfgsfloatval_t objective = ( lbfgsfloatval_t )( compute_objective( data->p_llm, data->examples ) );

  // get gradient
  std::vector< double > gradient = compute_analytical_gradient( data->p_llm, data->examples, data->lambda );

  // fill g based on gradient
  for( size_t i = 0; i < gradient.size(); ++i ){
    g[ i ] = -gradient[ i ];
  }

  return -objective;

}

/**
  Method to be used by the LBFGS optimizer to print training status

  @brief A method to report training progress
  @param[in]    func_data     A structure to provide the LLM and other parameters that are not part of the function definition
  @param[in]    x             A c-style array of parameter values
  @param[in]    g             A c-style array of parameter gradient values
  @param[in]    fx            The current objective value
  @param[in]    xnorm         The l2-norm of parameter values
  @param[in]    gnorm         The l2-norm of parameter gradient values
  @param[in]    step          The line-search step used for this iteration
  @param[in]    n             The number of examples
  @param[in]    k             The current iteration count
  @param[in]    ls            The number of evaluations called for this iteration
  @return                     0 to continue the optimization process; A non-zero value will cancel optimization
**/
int lbfgs_training_progress( void* func_data,
                             const lbfgsfloatval_t* x,
                             const lbfgsfloatval_t* g,
                             const lbfgsfloatval_t fx,
                             const lbfgsfloatval_t xnorm,
                             const lbfgsfloatval_t gnorm,
                             const lbfgsfloatval_t step,
                             int n, int k, int ls ) {
  std::cout << std::setw(3) << std::setfill(' ') << k << "\t"
            << std::showpoint << std::setprecision(7) << -fx << "\t(" << xnorm << ")\t(" << gnorm << ")" << std::endl;
  return 0;
}

/**
  Method to extract the unique examples from all of examples scraped from training example files. Consider each training example as
  a ground-truth (x,y) pair. An x is defined by the feature indices of the associated factor.

  @brief Extract the unique training examples.
  @param[in]    examples    The vector of examples from which to extract the unique examples
  @returns                  The vector of unique examples
  @throws                   no expected throws
**/
std::vector< std::shared_ptr<h2sl::Example> > extract_unique_examples( const std::vector< std::shared_ptr<h2sl::Example> >& examples ){
  // Initialize the return vector
  std::vector< std::shared_ptr<h2sl::Example> > unique_examples;
  unique_examples.reserve( examples.size() );
  // Incrementally add unique examples
  for( const auto& example : examples ){
    bool is_unique = true;
    // Create a sorted vector of the example's indices for easy element-wise comparison
    std::vector<size_t> example_indices;
    example_indices.reserve( example->indices[ *example->cv ].feature_indices.size() );
    for( const auto& feature_index : example->indices[ *example->cv ].feature_indices ){
      example_indices.push_back( feature_index.weight );
    }
    std::sort( example_indices.begin(), example_indices.end() );
    // Compare the example to the unique_examples
    for( const auto unique_example : unique_examples ){
      // Ensure the CVs are the same && the number of indices are the same
      if( *example->cv != *unique_example->cv ) continue;
      if( example_indices.size() != unique_example->indices[ *unique_example->cv ].feature_indices.size() ) continue;
      // Create a sorted vector of the unique_example's indices
      std::vector<size_t> unique_example_indices;
      unique_example_indices.reserve( unique_example->indices[ *unique_example->cv ].feature_indices.size() );
      for( const auto& feature_index : unique_example->indices[ *unique_example->cv ].feature_indices ){
        unique_example_indices.push_back( feature_index.weight );
      }
      std::sort( unique_example_indices.begin(), unique_example_indices.end() );
      // Compare the two vectors
      auto [first, second] = std::mismatch( example_indices.begin(), example_indices.end(), unique_example_indices.begin() );
      if( first == example_indices.end() ){
        is_unique = false;
        break;
      }
    }
    // Skip if not unique; otherwise, add it to unique_examples
    if( !is_unique ) continue;
    unique_examples.push_back( std::make_shared<h2sl::Example>( *example ) );
  }
  // Shrink the capacity if not all examples were unique
  unique_examples.shrink_to_fit();
  return unique_examples;
}


/**
  A program to train the optimal parameter values of feature weights for a log-linear model that approximates the conditional
  probability of an individual factor in a Distributed Correspondence Graph. Provided training example files are used to generate
  the set of ground-truth (x,y) pairs at a factor-level. A single training example file consists of an instance of a world and a
  parse tree of an utterance annotated per its meaning in the context of the world. The main steps of the program are:

  1. Load the command-line arguments (symbol dictionary, feature pool, training examples filenames, output filename, and debug flag)
  2. Generate the full set of factor-level examples by scraping the provided training example files
  3. Extract the full set of unique factor-level examples to prevent any bias due to data redundancy
  4. Optimize the log-linear model feature weight parameter values via the NLopt package LBFGS algorithm
  5. Evaluate and report the performance of the optimized model on the training examples
  6. Export the optimized model
**/
int main( int argc, char* argv[] ){
  std::cout << "Start of the H2SL LLM train program" << std::endl << std::endl;

  /***********************************************************************************/
  /************************ Load the command-line arguments **************************/
  /***********************************************************************************/
  boost::program_options::options_description desc("Options");
  desc.add_options()("help,h", "Help Screen")(
    "sd,s", boost::program_options::value<std::string>(), "symbol dictionary file")(
    "feature-pool,f", boost::program_options::value<std::string>(), "feature pool file")(
    "examples,e", boost::program_options::value<std::vector<std::string>>()->multitoken(), "example files")(
    "world-folder,w", boost::program_options::value<std::string>(), "world folder")(
    "lambda,l", boost::program_options::value<double>()->default_value(0.0), "regularization tuning parameter")(
    "epsilon,e", boost::program_options::value<double>()->default_value(0.001), "optimization termination condition")(
    "max_iterations,i", boost::program_options::value<double>()->default_value(1000), "maximum number of training iterations")(
    "threshold,t", boost::program_options::value<double>()->default_value(0.9), "Optimized model likelihood threshold")(
    "output,o", boost::program_options::value<std::string>()->default_value("/tmp/llm.xml"), "output file")(
    "debug,d", boost::program_options::value<bool>()->default_value(false), "debug flag");
  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  // Enforce that the arguments are provided
  if( vm.count("help") ){
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }
  if( !vm.count( "sd" ) ){
    std::cout << "No symbol dictionary file provided, exiting." << std::endl << std::endl;
    return EXIT_FAILURE;
  }
  if( !vm.count( "feature-pool" ) ){
    std::cout << "No feature-pool provided, exiting." << std::endl << std::endl;
    return EXIT_FAILURE;
  }
  if( !vm.count( "examples" ) ){
    std::cout << "No example files provided, exiting." << std::endl << std::endl;
    return EXIT_FAILURE;
  }

  // Load the debug flag
  bool debug = vm["debug"].as<bool>();
  // Load the symbol dictionary
  auto p_symboldictionary = std::make_shared<h2sl::SymbolDictionary>();
  p_symboldictionary->from_file( vm["sd"].as<std::string>().c_str() );
  if( debug ) std::cout << "Imported symbol dictionary " << *p_symboldictionary << "\n" << std::endl;
  // Load the training example filenames
  std::vector< std::string > example_filepaths = vm["examples"].as<std::vector<std::string>>();
  // Load the feature pool
  auto feature_pool = std::make_shared<h2sl::FeaturePool>();;
  feature_pool->from_file( vm["feature-pool"].as<std::string>().c_str() );
  if( debug ) std::cout << "Imported feature-pool with " << feature_pool->num_constituent_features() << " features" << std::endl;
  // Construct the log-linear model with feature weight parameters initialized to 0
  auto llm = std::make_shared<h2sl::LLM>( feature_pool );
  llm->weights.resize( feature_pool->num_constituent_features(), 0.0 );
  if( debug ) std::cout << "Constructed llm with " << llm->weights.size() << " weights" << std::endl;

  /***********************************************************************************/
  /************************ Generate the factor-level training examples **************/
  /************************ Extract the unique factor-level training examples ********/
  /***********************************************************************************/
  // Construct the correspondence variable values
  auto cvs = std::make_shared< std::vector< std::shared_ptr< std::string > > >();
  cvs->push_back( std::make_shared< std::string >( "false" ) );
  cvs->push_back( std::make_shared< std::string >( "true" ) );

  // Initialize the data structures to hold the world, language variables, symbol spaces, and examples
  std::vector< std::shared_ptr<h2sl::World> > worlds;
  std::vector< std::shared_ptr<h2sl::SymbolSpace> > symbolspaces;
  std::vector< std::shared_ptr<h2sl::Example> > examples;

  // Generate factor-level examples by iterating over the provided training example files
  for( const auto& example_filepath : example_filepaths ){
    std::cout << "Generating examples from \"" << example_filepath << "\"" << std::endl;
    worlds.push_back( std::make_shared<h2sl::World>() );
    // If a directory of world is included, load the world based on the example's filename
    std::string world_filepath = example_filepath;
    if( vm.count( "world-folder" ) ){
      // Extract world name from the filepath
      std::string example_name = example_filepath.substr( example_filepath.find_last_of( "/" ) + 1 );
      std::string world_name = example_name.substr( 0, example_name.find_first_of( "_" ) );
      if( debug ) std::cout << "World name found was \"" << world_name << "\"" << std::endl;
      world_filepath = vm["world-folder"].as<std::string>() + "/" + world_name + ".xml";
    }
    worlds.back()->from_xml( world_filepath );
    if( debug ) std::cout << "Loaded the world: " << worlds.back() << std::endl;

    symbolspaces.push_back( std::make_shared<h2sl::SymbolSpace>() );
    if( !symbolspaces.back()->fill( p_symboldictionary, worlds.back() ) ){
      std::cout << "Failed to fill symbolspace for \"" << example_filepath << "\"" << std::endl;
      return EXIT_FAILURE;
    }
    if( debug ) std::cout << "Constructed the symbolspace: " << *symbolspaces.back() << std::endl;

    // Construct a sentence with an empty langauge variable child
    auto p_sentence = std::make_shared<h2sl::Sentence>();
    p_sentence->from_xml( example_filepath.c_str(), symbolspaces.back().get() );

    // Construct LanguageVariable from imported content
    if( debug ) std::cout << "Loaded the language variable: " << *p_sentence->child() << std::endl;
    // Load instruction from the filepath
    std::optional< std::string > o_instruction = instruction_from_xml( example_filepath.c_str() );
    std::string instruction = "";
    if( o_instruction ) instruction = *o_instruction;
    if( debug ) std::cout << "Now scraping examples from " << example_filepath.c_str() << std::endl;
    scrape_examples( example_filepath, instruction, cvs, p_sentence->child(), worlds.back(), symbolspaces.back(), examples, debug );
  }

  // Compute the feature indices for each example - this is necessary before extracting unique examples
  for( auto & example : examples ){
    example->indices = feature_pool->evaluate( example->cvs, example->symbol, example->lv, example->world, debug );
    if( debug ){
      std::cout << "Example\n";
      std::cout << "\tFilename: \"" << example->filename << "\"\n";
      std::cout << "\tProb: " << llm->pygx( example->cv, example->indices ).prob;
      std::cout << "\tcv: " << *(example->cv) << "\n";
      std::cout << "\tSymbol: " << *(example->symbol) << "\n";
      std::cout << "\tLV: " << *(example->lv) << "\n";
      std::cout << "\tWorld: " << *(example->world) << "\n";
      std::cout << "\tIndices: ";
      for( const auto& feature_index : example->indices[ *(example->cv) ].feature_indices ){
        std::cout << feature_index.weight << ",";
      }
      std::cout << std::endl;
    }
  }

  // Extract the unique training examples
  auto unique_examples = extract_unique_examples( examples );
  if( debug ){
    std::cout << "\nNumber of generated examples: " << examples.size() << "\n";
    std::cout << "Number of unique examples: " << unique_examples.size() << std::endl;
    std::cin.get();
  }

  /***********************************************************************************/
  /************************ Optimize the log-linear model feature weights  ***********/
  /***********************************************************************************/
  // Construct a structure to hold problem data used by objective_function
  func_data_t data;
  data.debug = debug;
  data.p_llm = llm;
  data.examples = unique_examples;
  data.lambda = vm["lambda"].as<double>();

  // Declare lbfgs variables which store the objective value (fx) and weights (x)
  lbfgsfloatval_t fx;
  lbfgsfloatval_t * x = lbfgs_malloc( llm->weights.size() );
  // Fill in the weights from the llm object
  for( size_t i = 0; i < llm->weights.size(); ++i ){
    x[ i ] = llm->weights[ i ];
  }

  // Declare LBFGS parameters
  lbfgs_parameter_t param;
  lbfgs_parameter_init(&param);
  param.max_iterations = vm["max_iterations"].as<double>();
  param.epsilon = vm["epsilon"].as<double>();

  // Run LBFGS optimization
  auto start_time = std::chrono::steady_clock::now();
  auto error_code = lbfgs( llm->weights.size(), x, &fx, evaluate, lbfgs_training_progress, &data, &param );
  auto stop_time = std::chrono::steady_clock::now();

  // Print termination status
  // Non-zero error code implies unsuccessful termination
  // Descriptions of error code are given in the "lbfgs.h" file.
  std::cout << "L-BFGS optimization terminated with status code = " << error_code << std::endl;

  // Transfer learned weights back into llm object
  for( size_t i = 0; i < llm->weights.size(); ++i ){
    llm->weights[ i ] = x[ i ];
  }

  // Print final weights and the min and maximumm value
  if(debug){
    double min_weight = 0.0;
    double max_weight = 0.0;

    std::cout << "weights[" << llm->weights.size() << "]:{";
    for( size_t i = 0; i < llm->weights.size(); ++i ){
      std::cout << llm->weights[i];
      if( i != ( llm->weights.size() - 1 ) ){
        std::cout << ",";
      }
      if( llm->weights[i] < min_weight ){
        min_weight = llm->weights[i];
      }
      if( llm->weights[i] > max_weight ){
        max_weight = llm->weights[i];
      }
    }
    std::cout << "}\nmin_weight:" << min_weight << "\nmax_weight:" << max_weight << std::endl;
  }


  // clear the memory held by lbfgs weights array
  lbfgs_free( x );

  /***********************************************************************************/
  /************************ Evaluate and report the performance on all examples  *****/
  /************************ Export the model  ****************************************/
  /***********************************************************************************/
  unsigned int num_above_90 = 0;
  unsigned int num_above_80 = 0;
  unsigned int num_above_70 = 0;
  unsigned int num_above_50 = 0;
  unsigned int num_correct = 0;
  unsigned int num_incorrect = 0;

  // Apply the optimized model to each provided training example (not just unique)
  for( auto & example : examples ){
    auto result = llm->pygx( example->cv, example->indices );
    if( result.prob > 0.5 ) num_above_50++;
    if( result.prob > 0.7 ) num_above_70++;
    if( result.prob > 0.8 ) num_above_80++;
    if( result.prob > 0.9 ) num_above_90++;
    // Report any example that has a likelihood below the threshold
    if( result.prob < vm["threshold"].as<double>() ){
      num_incorrect++;
      std::cout << "Example\n";
      std::cout << "\tFilename: \"" << example->filename << "\"\n";
      std::cout << "\tProb: " << llm->pygx( example->cv, example->indices ).prob;
      std::cout << "\tcv: " << *(example->cv) << "\n";
      std::cout << "\tSymbol: " << *(example->symbol) << "\n";
      std::cout << "\tLV: " << *(example->lv) << "\n";
      std::cout << "\tWorld: " << *(example->world) << "\n";
      std::cout << "\tIndices: ";
      for( const auto& feature_index : example->indices[ *(example->cv) ].feature_indices ){
        std::cout << "\nIndex: " << feature_index.weight << "\tWeight: " << llm->weights[ feature_index.weight ]
                  << "\nLanguage Constituent Feature: " << *feature_pool->constituent_feature_sets[1][feature_index.language()]
                  << "\nSymbol Constituent Feature: " << *feature_pool->constituent_feature_sets[2][feature_index.symbol()] << "\n";
      }
      std::cout << std::endl;
    }
    else num_correct++;
  }
  // Printout accuracy based on multiple thresholds
  std::cout << "\nExamples above 0.5: " << num_above_50 << "/" << ( num_correct + num_incorrect );
  std::cout << "\nExamples above 0.7: " << num_above_70 << "/" << ( num_correct + num_incorrect );
  std::cout << "\nExamples above 0.8: " << num_above_80 << "/" << ( num_correct + num_incorrect );
  std::cout << "\nExamples above 0.9: " << num_above_90 << "/" << ( num_correct + num_incorrect ) << std::endl;
  // Report the accuracy given the likelihood threshold & the optimization runtime
  std::cout << "Likelihood threshold used for accuracy test: " << vm["threshold"].as<double>() << std::endl;
  std::cout << "Accuracy: " << num_correct << "/" << ( num_correct + num_incorrect )
            << " (" << (double)(num_correct)/(double)(num_correct + num_incorrect) << ")" << std::endl;
  std::cout << "Optimization time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time ).count()/1000.0 << std::endl;
  // Export the model
  if( debug ) std::cout << "Exporting the trained model to \"" << vm["output"].as<std::string>() << "\"" << std::endl;
  llm->to_xml( vm["output"].as<std::string>().c_str() );

  std::cout << "End of the H2SL LLM train program" << std::endl;
  return EXIT_SUCCESS;
}
