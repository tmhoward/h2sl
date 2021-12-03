/**
 * @file dcg-server.cc
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

#include <boost/program_options.hpp>

#include "h2sl/dcg.h"
#include "h2sl/feature.h"
#include "h2sl/feature-pool.h"
#include "h2sl/language-variable.h"

#include <ros/ros.h>
#include <std_msgs/String.h>
#include "h2sl/GroundLanguage.h"

/// Constant service server channel names
const std::string server_ground_language_channel = "ros_service_ground_language_channel";

/// Constant service client channel names
const std::string client_world_channel = "ros_service_world_channel";

///
/// Class to hold the ROS utilities for an DCG node
///
class DCG_ROS{
public:
  // Do not allow implicit compiler constructor
  DCG_ROS() = delete;
  // Parameter constructor for DCG_ROS that expects no world
  DCG_ROS( ros::NodeHandle& nh, const std::shared_ptr<h2sl::LLM>& llm, const std::shared_ptr<h2sl::SymbolDictionary>& sd )
        : _nh( nh ), _p_llm( llm ), _p_sd( sd )
  {
    // Fill the Correspondence Varaiables used for DCG construction and search/update methods
    _p_cvs = std::make_shared< std::vector< std::shared_ptr<std::string> > >();
    _p_cvs->emplace_back( std::make_shared< std::string >( "true" ) );
    _p_cvs->emplace_back( std::make_shared< std::string >( "false" ) );
  }

  // Parameter constructor for DCG_ROS that accepts a world
  DCG_ROS( ros::NodeHandle& nh, const std::shared_ptr<h2sl::LLM>& llm,
          const std::shared_ptr<h2sl::SymbolDictionary>& sd,
          const std::shared_ptr<h2sl::World>& world )
          : _nh( nh ), _p_llm( llm ), _p_sd( sd ), _p_world( world )
  {
    // Fill the Correspondence Varaiables used for DCG construction and search/update methods
    _p_cvs = std::make_shared< std::vector< std::shared_ptr<std::string> > >();
    _p_cvs->emplace_back( std::make_shared< std::string >( "true" ) );
    _p_cvs->emplace_back( std::make_shared< std::string >( "false" ) );
  }

  // Default destructor for DCG_ROS
  ~DCG_ROS() = default;

  // Service Server callback to handle a grounding service request
  //  1. Extract the LanguageVariable (LV) from the request
  //  2. Construct a Sentence with the LV
  //  3. Request the most recent world (or used the stored one)
  //  4. Construct a DCG
  //  5. Search the DCG with a specified beamwidth and debug state
  //  6. Check for solutions
  //  7. Fill the response with the best solution
  bool handle_ground_language_callback( h2sl::GroundLanguage::Request& req, h2sl::GroundLanguage::Response& res ){
    ROS_INFO_STREAM("Received a service request to ground language.");
    // Extract the language variable from the request content
    auto o_lv = h2sl::LanguageVariable::from_msg( req.language );
    if( !o_lv ){
      ROS_ERROR_STREAM("Could not convert the request message to a LanguageVariable, not grounding.");
      return false;
    }
    // Construct a Sentence with the received LV
    auto p_sentence = std::make_shared< h2sl::Sentence >();
    p_sentence->child() = std::make_shared< h2sl::LanguageVariable >( *o_lv );
    ROS_INFO_STREAM("Loaded the sentence \"" << p_sentence->words_to_std_string() << "\" from the request message.");

    // Request an updated world if the service client exists
    const auto it_world_client = _service_clients.find( client_world_channel );
    if( it_world_client != _service_clients.cend() ){
      // TODO
    }
    if( _p_world == nullptr ){
      ROS_ERROR_STREAM("No world model associated with the request, not grounding.");
      return false;
    }
    // Fill the symbolspace and Construct the DCG
    auto p_symbolspace = std::make_shared<h2sl::SymbolSpace>();
    p_symbolspace->fill( _p_sd, _p_world );
    _o_dcg = h2sl::DCG( p_symbolspace, p_sentence, _p_world, _p_llm, _p_cvs, false );
    // Search the DCG
    auto start_time = std::chrono::steady_clock::now();
    _o_solutions = _o_dcg->search( 4, false );
    auto stop_time = std::chrono::steady_clock::now();
    auto search_runtime = std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time).count()/1000.0;
    // Handle failure cases
    if( !_o_solutions ){
      ROS_INFO_STREAM("DCG::search() failed, solutions were nullopt.");
      return false;
    }
    if( _o_solutions->empty() ){
      ROS_INFO_STREAM("DCG::search() did not provide any solutions.");
      return true;
    }
    // Fill the reponse with the best solution
    auto& r_best_solution = _o_solutions->at(0);
    ROS_INFO_STREAM("Best solution has a probability: " << r_best_solution.prob);
    for( const auto& symbol : r_best_solution.sentence.child()->symbols() ){
      res.symbols.emplace_back( symbol->to_msg() );
    }
    return true;
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
  //      Member Accessor Methods           //
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

  // Methods to provide read-only & read-write access to the publishers member data
  inline const std::map< std::string, ros::Publisher >& publishers( void )const{ return _publishers; }
  inline std::map< std::string, ros::Publisher >& publishers( void ){ return _publishers; }

  // Methods to provide read-only & read-write access to the subscribers member data
  inline const std::map< std::string, ros::Subscriber >& subscribers( void )const{ return _subscribers; }
  inline std::map< std::string, ros::Subscriber >& subscribers( void ){ return _subscribers; }

  // Methods to provide read-only & read-write access to the service_servers member data
  inline const std::map< std::string, ros::ServiceServer >& service_servers( void )const{ return _service_servers; }
  inline std::map< std::string, ros::ServiceServer >& service_servers( void ){ return _service_servers; }

  // Methods to provide read-only & read-write access to the service_clients member data
  inline const std::map< std::string, ros::ServiceClient >& service_clients( void )const{ return _service_clients; }
  inline std::map< std::string, ros::ServiceClient >& service_clients( void ){ return _service_clients; }
protected:
private:
  /// ROS-related members
  ros::NodeHandle _nh;
  std::map< std::string, ros::Publisher > _publishers = std::map< std::string, ros::Publisher>();
  std::map< std::string, ros::Subscriber > _subscribers = std::map< std::string, ros::Subscriber>();
  std::map< std::string, ros::ServiceServer > _service_servers = std::map< std::string, ros::ServiceServer>();
  std::map< std::string, ros::ServiceClient > _service_clients = std::map< std::string, ros::ServiceClient>();

  /// DCG-related members
  // Trained log-linear model, typically loaded from a file
  std::shared_ptr< h2sl::LLM > _p_llm = nullptr;
  // SymbolDictionary from which a SymbolSpace can be constructed
  std::shared_ptr< h2sl::SymbolDictionary > _p_sd = nullptr;
  // World model, typically constructed from a perception world topic
  std::shared_ptr< h2sl::World > _p_world = nullptr;
  // Correspondence Variables to be used for DCG construction and search
  std::shared_ptr<std::vector<std::shared_ptr< std::string >>> _p_cvs = nullptr;
  // Optional DCG object to be constructed from the llm, world, and sentence
  std::optional< h2sl::DCG > _o_dcg = std::nullopt;
  // Optional storage of the DCG solutions found after search (or other update methods)
  std::optional< h2sl::DCG::vecDCGSearchSolutions > _o_solutions = std::nullopt;

}; // class DCG_ROS

///
/// Program to construct and run a ROS node responsible for providing Distributed Correspondence Graph services
///
int main( int argc, char* argv[] ){
  try{
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    //                              Load the CLI Arguments                          //
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // Initialize the ROS process & Create a node handle
    ros::init(argc, argv, "dcg_server_node");
    ros::NodeHandle nh;
    ROS_INFO("Start of the dcg-server node program.");

    // Parse the command-line
    boost::program_options::options_description desc("Options");
    desc.add_options()("help,h", "Help Screen")(
      "channels,c", boost::program_options::value<std::string>(), "channel file location for use by the ROS node")(
      "world,w", boost::program_options::value<std::string>(), "world file location for use by DCG instead of a world service client")(
      "symbol_dictionary,s", boost::program_options::value<std::string>(), "symbol dictionary file")(
      "llm,l", boost::program_options::value<std::string>(), "log-linear model file");
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);
    if( vm.count("help") ){
      std::cout << desc << std::endl;
      return EXIT_SUCCESS;
    }
    // Load the channels from file into a map
    std::unordered_map< std::string, std::string > channels;
    if( !vm.count("channels") ) throw std::runtime_error("No channels file provided");
    else{
      ROS_INFO_STREAM("Loading the channels file \"" << vm["channels"].as<std::string>() << "\"");
      channels = h2sl::channels_from_xml( vm["channels"].as<std::string>().c_str() );
    }
    // Load the world model if provided; otherwise, expect a world service to be provided
    auto p_world = std::make_shared<h2sl::World>();
    if( !vm.count("world") ){
      ROS_INFO_STREAM("No world file provided - will construct a ROS::serviceClient() object to request world models");
      p_world = nullptr;
    } else{
      ROS_INFO_STREAM("Loading the world file \"" << vm["world"].as<std::string>() << "\"");
      ROS_INFO("Not creating a ROS::serviceClient() object for a world service");
      p_world = std::make_shared<h2sl::World>();
      p_world->from_xml( vm["world"].as<std::string>() );
    }
    // Load the symboldictionary from file
    auto p_sd = std::make_shared<h2sl::SymbolDictionary>();
    if( !vm.count("symbol_dictionary") ) throw std::runtime_error("No symbol dictionary file provided");
    else{
      ROS_INFO_STREAM("Loading the symbol dictionary file \"" << vm["symbol_dictionary"].as<std::string>() << "\"");
      p_sd->from_file( vm["symbol_dictionary"].as<std::string>().c_str() );
    }
    // Load the log-linear model from file
    auto p_llm = std::make_shared<h2sl::LLM>();
    if( !vm.count("llm") ) throw std::runtime_error("No log-linear model file provided");
    else{
      ROS_INFO_STREAM("Loading the log-linear model file \"" << vm["llm"].as<std::string>() << "\"");
      p_llm->from_file( vm["llm"].as<std::string>().c_str() );
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    //                              Setup the ROS Utilities                         //
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    // Create the DCG_ROS object
    DCG_ROS dcg_ros( nh, p_llm, p_sd );

    // Create the ROS_Helper
    h2sl::ROS_Helper<DCG_ROS> ros_helper{ nh, dcg_ros, channels };

    // Create any ROS ServiceServers
    ros_helper.create_service_server( server_ground_language_channel, &DCG_ROS::handle_ground_language_callback );
    // Create any ROS ServiceClients
    if( p_world == nullptr ){
      ros_helper.create_service_client<h2sl::GroundLanguage>( client_world_channel );
    }


    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    //                              Begin ROS Spinning                              //
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    ROS_INFO("ROS initiated, entering spin loop");
    ros::Rate r(10);
    while( ros::ok() ){
      ros::spinOnce();
      r.sleep();
    }
    ROS_INFO("Node terminating, exiting the dcg-server node program.");
    return EXIT_SUCCESS;
  }
  catch( const std::runtime_error& error ){
    std::cerr << "Terminate called after throwing an instance of \'std::runtime_error\'\nThe error: " << error.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
