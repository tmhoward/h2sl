/**
 * @file parser-server.cc
 *
 * @brief ROS node providing services of a Parser
 *
 */

#include <boost/program_options.hpp>

#include "h2sl/parser.h"
#include "h2sl/pcfg.h"
#include "h2sl/common.h"

#include <ros/ros.h>
#include "h2sl/Parse.h"

/// Constant service server channel names
const std::string server_parse_channel = "ros_service_parser_channel";

///
/// Class to hold the ROS utilities for an DCG node
///
class Parser_ROS{
public:
  // Do not allow implicit compiler constructor
  Parser_ROS() = delete;
  // Parameter constructor for Parser_ROS
  Parser_ROS( ros::NodeHandle& nh, const h2sl::PCFG& pcfg ) : _nh( nh ), _parser( pcfg ){}
  // Default destructor for Parser_ROS
  ~Parser_ROS() = default;

  // Callback function to handle a parsing service request
  //  1. Parse the request string
  //  2. Check for parses
  //  3. Fill the response with the top parse
  bool handle_parse_callback( h2sl::Parse::Request& req, h2sl::Parse::Response& res ){
    ROS_INFO("Received a service request to parse language.");
    ROS_INFO_STREAM("Parsing the string \"" << req.instruction.data << "\".");

    // Parse the message data & create a string message
    auto ret_parse = _parser.parse( req.instruction.data, 10 );
    if( ret_parse.size() == 0 ){
      ROS_WARN_STREAM("No parses found.");
      return false;
    }

    // Return request back to client
    res.parse = ret_parse[0].first.to_msg();
    return true;
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
  //      Member Accessor Methods           //
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

  // Method to provide read-only & read-write access to the service_servers member data
  inline const std::map< std::string, ros::ServiceServer >& service_servers( void )const{ return _service_servers; }
  inline std::map< std::string, ros::ServiceServer >& service_servers( void ){ return _service_servers; }

protected:
private:
  /// ROS-related members
  ros::NodeHandle _nh;
  std::map< std::string, ros::ServiceServer > _service_servers = std::map< std::string, ros::ServiceServer>();

  // Parser_ROS private parser member
  h2sl::Parser _parser;

}; // class DCG_ROS

///
/// Program to construct and run a ROS node responsible for providing a constituency parsing service
///
int main( int argc, char* argv[] ){
  try{
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    //                              Load the CLI Arguments                          //
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    // Initialize the ROS process & Create a node handle
    ros::init(argc, argv, "parser_server_node");
    ros::NodeHandle nh;
    ROS_INFO("Start of the parser-server node program.");

    // Parse the command-line
    boost::program_options::options_description desc("Options");
    desc.add_options()("help,h", "Help Screen")(
      "channels,c", boost::program_options::value<std::string>(), "channel file location for use by the ROS node")(
      "pcfg,p", boost::program_options::value<std::string>(), "probabilistic context free grammer file location");
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);
    if( vm.count("help") ){
      std::cout << desc << std::endl;
      return EXIT_SUCCESS;
    }
    // Load the channels from file into a map
    std::unordered_map< std::string, std::string > channels;
    if( !vm.count("channels") )throw std::runtime_error("No channels file provided");
    else{
      ROS_INFO_STREAM("Loading the channels file \"" << vm["channels"].as<std::string>() << "\"");
      channels = h2sl::channels_from_xml( vm["channels"].as<std::string>().c_str() );
    }
    // Load the probabilistic context free grammer from file
    auto pcfg = h2sl::PCFG();
    if( !vm.count("pcfg") ) throw std::runtime_error("No probabilistic context free grammer file provided");
    else{
      ROS_INFO_STREAM("Loading the probabilistic context free grammer file \"" << vm["pcfg"].as<std::string>() << "\"");
      pcfg.from_file( vm["pcfg"].as<std::string>().c_str() );
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    //                              Setup the ROS Utilities                         //
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

    // Create the Parser_ROS object
    Parser_ROS parser_ros( nh, pcfg );

    // Create the ROS_Helper
    h2sl::ROS_Helper<Parser_ROS> ros_helper{ nh, parser_ros, channels };

    // Create any ROS ServiceServers
    ros_helper.create_service_server( server_parse_channel, &Parser_ROS::handle_parse_callback );

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    //                              Begin ROS Spinning                              //
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
    ROS_INFO("ROS initiated, entering spin loop.");
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
