/**
 * @file scrape_results_runtime_norm_vs_world_size.cc
 *
 * @brief
 *
 * Scrape a partition result for statistics about runtime_norm per world size
 *
 */

#include "scrape_results_runtime_normalized_vs_world_size_cmdline.h"

#include <math.h> //sqrt()
#include <assert.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>
#include <libxml/tree.h>
#include <glib.h>

#include "h2sl/world.h"

using namespace std;


/**
 * Function to return the average of a vector of doubles
 */
double
average( const vector< double >& vector ){
  double tmp = 0.0;
  //accumulate the elements
  for( unsigned int i = 0; i < vector.size(); i++ ){
    tmp += vector[ i ];
  }
  //divide by the number of elements
  tmp = ( tmp / vector.size() );
  return tmp;
}

/**
 * Function to return the standard deviation of a vector of doubles
 */
double
standard_deviation( const vector< double >& tmp ){
  // find the square of the average
  double tmp_avg = average( tmp );
  tmp_avg = tmp_avg * tmp_avg;
  // make a vector of squared values
  vector< double > tmp_squares( tmp );
  for( unsigned int i = 0; i < tmp_squares.size(); i++ ){
    // square each element
    tmp_squares[ i ] = tmp_squares[ i ] * tmp_squares[ i ];
  }
  // find the average of the squared values
  double tmp_squares_avg = average( tmp_squares );
  // find the difference between tmp_square_avg and tmp_avg
  double standard_deviation = ( tmp_squares_avg - tmp_avg );
  standard_deviation = sqrt( standard_deviation );
  return standard_deviation;
}

/**
 * Program to scrape a partition result for statistics about inference runtime_norm per world size
 */
int
main( int argc,
      char* argv[] ){
  cout << "start of scrape_results_runtime_normalized_vs_world_size program" << endl;

  // Determine the arguments provided.
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }

  // create the data structures to hold the runtime information per model
  // map key_value: the world size as a string
  // map mapped_value: vector of runtimes (double) for worlds of the key_value size
  map< string, vector< double > > dcg_runtimes;
  map< string, vector< double > > adcg_runtimes;
  map< string, vector< double > > hdcg_runtimes;
  map< string, vector< double > > hadcg_runtimes;

  // data structure to keep track of unique worlds per world size
  // map key_value: the world size as a string
  // map mapped_value: a vector of each unique world for the key_value size
  map< string, vector< const h2sl::World > > world_models;
  
  // XML pointers to read the input file
  xmlDoc * input_doc = NULL;
  xmlNodePtr input_root = NULL;
  input_doc = xmlReadFile( args.input_arg, NULL, 0 );

  if( input_doc != NULL ){
    // find the root node of the document
    input_root = xmlDocGetRootElement( input_doc );
    if( input_root->type == XML_ELEMENT_NODE ){
      // loop through each child xml node && look for "results"
      for( xmlNodePtr l1 = input_root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "results" ) ) == 0 ){
            // loop through each child of "results" && look for "test"
            for( xmlNodePtr l2 = l1->children; l2; l2 = l2->next ){
              if( l2->type == XML_ELEMENT_NODE ){
                if( xmlStrcmp( l2->name, ( const xmlChar* )( "test" ) ) == 0 ){
                  // loop through each child of "test" && look for "training_set"
                  for( xmlNodePtr l3 = l2->children; l3; l3 = l3->next ){
                    if( l3->type == XML_ELEMENT_NODE ){
                      if( xmlStrcmp( l3->name, ( const xmlChar* )( "training_set" ) ) == 0 ){
                        // loop through each child of "training_set" && look for "example"
                        // each "example" node contains the world size information
                        for( xmlNodePtr l4 = l3->children; l4; l4 = l4->next ){
                          if( l4->type == XML_ELEMENT_NODE ){
                            if( xmlStrcmp( l4->name, ( const xmlChar* )( "example" ) ) == 0 ){
                              // look for the "world_objects" property; this is the size of the world
                              xmlChar * num_world_objects = xmlGetProp( l4, ( const xmlChar* )( "world_objects" ) );
                              if( num_world_objects != NULL ){
                                // convert property to a stringstream type for using as a string
                                stringstream num_world_objects_string;
                                num_world_objects_string << num_world_objects;
                                // loop through each child of "example" && look for a model (e.g. "dcg" or "adcg")
                                for( xmlNodePtr l5 = l4->children; l5; l5 = l5->next ){
                                  if( l5->type == XML_ELEMENT_NODE ){
                                    if( xmlStrcmp( l5->name, ( const xmlChar* )( "dcg" ) ) == 0 ){
                                      // look for the "runtime" property; this is the runtime for the given model
                                      xmlChar * runtime = xmlGetProp( l5, ( const xmlChar* )( "runtime_norm" ) );
                                      if( runtime != NULL ){
                                        // convert runtime to a stringstream type for using as a string
                                        stringstream runtime_string;
                                        runtime_string << runtime;
                                        // check whether "dcg_runtimes" has a key_value matching num_world_objects_string
                                        map< string, vector< double > >::iterator it_dcg_runtimes = dcg_runtimes.find( num_world_objects_string.str() );
                                        // check the return of map::find()
                                        // if not map::end(), the key_value exists
                                        // else, insert the new world size key_value
                                        if( it_dcg_runtimes != dcg_runtimes.end() ){
                                          it_dcg_runtimes->second.push_back( std::stod( runtime_string.str() ) );
                                        } else{
                                          dcg_runtimes.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( runtime_string.str() ) ) ) 
                                          );
                                        }
                                        // free the xmlChar pointer
                                        xmlFree( runtime );
                                      }
                                    }
                                    if( xmlStrcmp( l5->name, ( const xmlChar* )( "adcg" ) ) == 0 ){
                                      // look for the "runtime" property; this is the runtime for the given model
                                      xmlChar * runtime = xmlGetProp( l5, ( const xmlChar* )( "runtime_norm" ) );
                                      if( runtime != NULL ){
                                        // convert runtime to a stringstream type for using as a string
                                        stringstream runtime_string;
                                        runtime_string << runtime;
                                        // check whether "adcg_runtimes" has a key_value matching num_world_objects_string
                                        map< string, vector< double > >::iterator it_adcg_runtimes = adcg_runtimes.find( num_world_objects_string.str() );
                                        // check the return of map::find()
                                        // if not map::end(), the key_value exists
                                        // else, insert the new world size key_value
                                        if( it_adcg_runtimes != adcg_runtimes.end() ){
                                          it_adcg_runtimes->second.push_back( std::stod( runtime_string.str() ) );
                                        } else{
                                          adcg_runtimes.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( runtime_string.str() ) ) ) 
                                          );
                                        }
                                        // free the xmlChar pointer
                                        xmlFree( runtime );
                                      }
                                    }
                                    if( xmlStrcmp( l5->name, ( const xmlChar* )( "hdcg" ) ) == 0 ){
                                      // look for the "runtime" property; this is the runtime for the given model
                                      xmlChar * runtime = xmlGetProp( l5, ( const xmlChar* )( "runtime_norm" ) );
                                      if( runtime != NULL ){
                                        // convert runtime to a stringstream type for using as a string
                                        stringstream runtime_string;
                                        runtime_string << runtime;
                                        // check whether "hdcg_runtimes" has a key_value matching num_world_objects_string
                                        map< string, vector< double > >::iterator it_hdcg_runtimes = hdcg_runtimes.find( num_world_objects_string.str() );
                                        // check the return of map::find()
                                        // if not map::end(), the key_value exists
                                        // else, insert the new world size key_value
                                        if( it_hdcg_runtimes != hdcg_runtimes.end() ){
                                          it_hdcg_runtimes->second.push_back( std::stod( runtime_string.str() ) );
                                        } else{
                                          hdcg_runtimes.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( runtime_string.str() ) ) ) 
                                          );
                                        }
                                        // free the xmlChar pointer
                                        xmlFree( runtime );
                                      }
                                    }
                                    if( xmlStrcmp( l5->name, ( const xmlChar* )( "hadcg" ) ) == 0 ){
                                      // look for the "runtime" property; this is the runtime for the given model
                                      xmlChar * runtime = xmlGetProp( l5, ( const xmlChar* )( "runtime_norm" ) );
                                      if( runtime != NULL ){
                                        // convert runtime to a stringstream type for using as a string
                                        stringstream runtime_string;
                                        runtime_string << runtime;
                                        // check whether "hadcg_runtimes" has a key_value matching num_world_objects_string
                                        map< string, vector< double > >::iterator it_hadcg_runtimes = hadcg_runtimes.find( num_world_objects_string.str() );
                                        // check the return of map::find()
                                        // if not map::end(), the key_value exists
                                        // else, insert the new world size key_value
                                        if( it_hadcg_runtimes != hadcg_runtimes.end() ){
                                          it_hadcg_runtimes->second.push_back( std::stod( runtime_string.str() ) );
                                        } else{
                                          hadcg_runtimes.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( runtime_string.str() ) ) ) 
                                          );
                                        }
                                        // free the xmlChar pointer
                                        xmlFree( runtime );
                                      }
                                    }
                                    if( xmlStrcmp( l5->name, ( const xmlChar* )( "world" ) ) == 0 ){
                                      // load the world from the xml node
                                      h2sl::World * tmp = new h2sl::World();
                                      tmp->from_xml( l5 );
                                      if( tmp != NULL ){
                                        // get the vector of worlds for that world size
                                        map< string, vector< const h2sl::World > >::iterator it_worlds = world_models.find( to_string( tmp->objects().size() ) );
                                        if( it_worlds != world_models.end() ){
                                          // check whether the tmp world model matches any in the vector
                                          bool found_match = false;
                                          for( unsigned int i = 0; i < it_worlds->second.size(); i++ ){
                                            if( *tmp == it_worlds->second[ i ] ){
                                              found_match = true;
                                            }
                                          }
                                          // if the world does not exist in the vector, add it; otherwise ignore and move on
                                          if(  !found_match ){
                                            it_worlds->second.push_back( *tmp );
                                          }
                                        } else{
                                          // the vector for the world size does not exist, so make it and add duplicate of current world*
                                          world_models.insert(
                                            pair< string, vector< const h2sl::World > >( to_string( tmp->objects().size() ), vector< const h2sl::World >( 1, *tmp ) ) 
                                          );

                                        }
                                        // dynamic memory clean up
                                        delete tmp;
                                        tmp = NULL;
                                      }
                                    }
                                  }
                                }
                                // free the xmlChar pointer
                                xmlFree( num_world_objects );
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  // Report information about the number of worlds
  // Scope Decision: Keep the total number of worlds for later reports
  unsigned int number_of_worlds = 0;
  if( world_models.size() > 0 ){
    cout << "World Information - Number of Worlds" << endl;
    for( map< string, vector< const h2sl::World> >::const_iterator it_world_models = world_models.begin(); it_world_models != world_models.end(); ++it_world_models ){
      cout << "world size( " << it_world_models->first << " ): " << it_world_models->second.size() << endl;
      // add to the total number of worlds
      number_of_worlds = number_of_worlds + it_world_models->second.size();
    }
    cout << "  Total # of Worlds: " << number_of_worlds << endl;
  }
  cout << endl;

  // Report runtime statistics per model per world size
  cout << "Reporting runtime statistics:" << endl;

  // Report DCG runtime statistics
  if( dcg_runtimes.size() > 0 ){
    cout << "DCG" << endl;
    unsigned int number_of_examples = 0; // counter for the total number of examples
    vector< double > all_runtimes; // vector of all of the runtime for this model
    for( map< string, vector< double > >::const_iterator it_dcg_runtimes = dcg_runtimes.begin(); it_dcg_runtimes != dcg_runtimes.end(); ++it_dcg_runtimes ){
      // find the average runtime for the examples with the current world size
      double runtime_average = average( it_dcg_runtimes->second );
      double runtime_standard_deviation = standard_deviation( it_dcg_runtimes->second );
      // find the standard deviation
      cout << "  world size: " << it_dcg_runtimes->first << endl;
      cout << "  # of examples: " << it_dcg_runtimes->second.size() << endl;
      cout << "    runtime norm avg: " << to_string( runtime_average ) << endl; 
      cout << "    runtime norm std: " << to_string( runtime_standard_deviation ) << endl; 

      // add the number of examples for the current world size to the total
      number_of_examples = number_of_examples + it_dcg_runtimes->second.size();

      // update the all_runtimes vector with each runtime from the current world size
      for( unsigned int i = 0; i < it_dcg_runtimes->second.size(); i++ ){
        all_runtimes.push_back( it_dcg_runtimes->second[ i ] );
      }
    }
    // find the results for the total
    cout << endl << "  Total: " << endl;
    cout << "    # of examples: " << number_of_examples << endl; 
    cout << "    # of worlds: " << number_of_worlds << endl; 
    cout << "    runtime norm avg: " << to_string( average( all_runtimes ) ) << endl;
    cout << "    runtime norm std: " << to_string( standard_deviation( all_runtimes ) ) << endl;
  } else{
    cout << "No DCG runtime statistics to report." << endl;
  }
  cout << endl;

  // Report ADCG runtime statistics
  if( adcg_runtimes.size() > 0 ){
    cout << "ADCG" << endl;
    unsigned int number_of_examples = 0; // counter for the total number of examples
    vector< double > all_runtimes; // vector of all of the runtime for this model
    for( map< string, vector< double > >::const_iterator it_adcg_runtimes = adcg_runtimes.begin(); it_adcg_runtimes != adcg_runtimes.end(); ++it_adcg_runtimes ){
      // find the average runtime for the examples with the current world size
      double runtime_average = average( it_adcg_runtimes->second );
      double runtime_standard_deviation = standard_deviation( it_adcg_runtimes->second );
      // find the standard deviation
      cout << "  world size: " << it_adcg_runtimes->first << endl;
      cout << "  # of examples: " << it_adcg_runtimes->second.size() << endl;
      cout << "    runtime norm avg: " << to_string( runtime_average ) << endl; 
      cout << "    runtime norm std: " << to_string( runtime_standard_deviation ) << endl; 

      // add the number of examples for the current world size to the total
      number_of_examples = number_of_examples + it_adcg_runtimes->second.size();

      // update the all_runtimes vector with each runtime from the current world size
      for( unsigned int i = 0; i < it_adcg_runtimes->second.size(); i++ ){
        all_runtimes.push_back( it_adcg_runtimes->second[ i ] );
      }
    }
    // find the results for the total
    cout << endl << "  Total: " << endl;
    cout << "    # of examples: " << number_of_examples << endl; 
    cout << "    # of worlds: " << number_of_worlds << endl; 
    cout << "    runtime norm avg: " << to_string( average( all_runtimes ) ) << endl;
    cout << "    runtime norm std: " << to_string( standard_deviation( all_runtimes ) ) << endl;
  } else{
    cout << "No ADCG runtime statistics to report." << endl;
  }
  cout << endl;

  // Report HDCG runtime statistics
  if( hdcg_runtimes.size() > 0 ){
    cout << "HDCG" << endl;
    unsigned int number_of_examples = 0; // counter for the total number of examples
    vector< double > all_runtimes; // vector of all of the runtime for this model
    for( map< string, vector< double > >::const_iterator it_hdcg_runtimes = hdcg_runtimes.begin(); it_hdcg_runtimes != hdcg_runtimes.end(); ++it_hdcg_runtimes ){
      // find the average runtime for the examples with the current world size
      double runtime_average = average( it_hdcg_runtimes->second );
      double runtime_standard_deviation = standard_deviation( it_hdcg_runtimes->second );
      // find the standard deviation
      cout << "  world size: " << it_hdcg_runtimes->first << endl;
      cout << "  # of examples: " << it_hdcg_runtimes->second.size() << endl;
      cout << "    runtime norm avg: " << to_string( runtime_average ) << endl;
      cout << "    runtime norm std: " << to_string( runtime_standard_deviation ) << endl;

      // add the number of examples for the current world size to the total
      number_of_examples = number_of_examples + it_hdcg_runtimes->second.size();

      // update the all_runtimes vector with each runtime from the current world size
      for( unsigned int i = 0; i < it_hdcg_runtimes->second.size(); i++ ){
        all_runtimes.push_back( it_hdcg_runtimes->second[ i ] );
      }
    }
    // find the results for the total
    cout << endl << "  Total: " << endl;
    cout << "    # of examples: " << number_of_examples << endl;
    cout << "    # of worlds: " << number_of_worlds << endl;
    cout << "    runtime norm avg: " << to_string( average( all_runtimes ) ) << endl;
    cout << "    runtime norm std: " << to_string( standard_deviation( all_runtimes ) ) << endl;
  } else{
    cout << "No HDCG runtime statistics to report." << endl;
  }
  cout << endl;

  // Report HADCG runtime statistics
  if( hadcg_runtimes.size() > 0 ){
    cout << "HADCG" << endl;
    unsigned int number_of_examples = 0; // counter for the total number of examples
    vector< double > all_runtimes; // vector of all of the runtime for this model
    for( map< string, vector< double > >::const_iterator it_hadcg_runtimes = hadcg_runtimes.begin(); it_hadcg_runtimes != hadcg_runtimes.end(); ++it_hadcg_runtimes ){
      // find the average runtime for the examples with the current world size
      double runtime_average = average( it_hadcg_runtimes->second );
      double runtime_standard_deviation = standard_deviation( it_hadcg_runtimes->second );
      // find the standard deviation
      cout << "  world size: " << it_hadcg_runtimes->first << endl;
      cout << "  # of examples: " << it_hadcg_runtimes->second.size() << endl;
      cout << "    runtime norm avg: " << to_string( runtime_average ) << endl;
      cout << "    runtime norm std: " << to_string( runtime_standard_deviation ) << endl;

      // add the number of examples for the current world size to the total
      number_of_examples = number_of_examples + it_hadcg_runtimes->second.size();

      // update the all_runtimes vector with each runtime from the current world size
      for( unsigned int i = 0; i < it_hadcg_runtimes->second.size(); i++ ){
        all_runtimes.push_back( it_hadcg_runtimes->second[ i ] );
      }
    }
    // find the results for the total
    cout << endl << "  Total: " << endl;
    cout << "    # of examples: " << number_of_examples << endl;
    cout << "    # of worlds: " << number_of_worlds << endl;
    cout << "    runtime norm avg: " << to_string( average( all_runtimes ) ) << endl;
    cout << "    runtime norm std: " << to_string( standard_deviation( all_runtimes ) ) << endl;
  } else{
    cout << "No HADCG runtime statistics to report." << endl;
  }
  cout << endl;

  // Memory cleanup
  // free the xml pointers
  if( input_doc != NULL ){
    xmlFreeDoc( input_doc );
  }
  cout << "end of scrape_results_runtime_normalized_vs_world_size program" << endl;
  return 0;
}
