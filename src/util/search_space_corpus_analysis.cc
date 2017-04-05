/**
 * @file search_space_corpus_analysis.cc 
 *
 * @brief
 *
 * Scrape a partition result for statistics related to 
 * search space sizes 
 *
 */

#include "search_space_corpus_analysis_cmdline.h"

#include <math.h> //sqrt()
#include <assert.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>
#include <libxml/tree.h>
#include <glib.h>

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
  //divide by the number of elements if vector is not empty
  if( vector.size() ){
    tmp = ( tmp / vector.size() );
  } else {
    tmp = 0.0;
  }
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

/*
void
extract_from_node_dcg( xmlNodePtr dcg_node, 
                       std::string& num_world_objects_str ){

  // CONCRETE 
  xmlChar * ss_concrete = xmlGetProp( dcg_node , ( const xmlChar* )( "search_space_concrete_size" ) );
  if( ss_concrete != NULL ){
    // convert ss_concrete to a stringstream type for using as a string
    stringstream ss_concrete_string;
    ss_concrete_string << ss_concrete;
    dcg_ss_concrete_size.push_back( std::stod( ss_concrete_string.str() ) );

    // check whether "dcg_ss_concrete_wm" has a key_value matching num_world_objects_string
    map< string, vector< double > >::iterator it_dcg_ss_concrete_wm = dcg_ss_concrete_wm.find( num_world_objects_str );

    // check the return of map::find() if not map::end(), the key_value exists. Else, insert the new world size key_value
    if( it_dcg_ss_concrete_wm != dcg_ss_concrete_wm.end() ){
      it_dcg_ss_concrete_wm->second.push_back( std::stod( ss_concrete_string.str() ) );
    } else{
      dcg_ss_concrete_wm.insert(
        pair< string, vector< double > >( num_world_objects_str, vector< double >( 1, std::stod( ss_concrete_string.str() ) ) )
      );
    }
    // free the xmlChar pointer
    xmlFree( ss_concrete );
  }
}
*/

/**
 * Program to scrape a partition result for statistics about the search space sizes. 
 */
int
main( int argc,
      char* argv[] ){
  cout << "start of the search space corpus analysis program" << endl;

  // Determine the arguments provided.
  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }

  // Recording the statistics for the search spaces.
  vector< double > dcg_ss_concrete_size;
  vector< double > dcg_ss_abstract_avg_size;
  vector< double > dcg_ss_abstract_max_size;

  map< string, vector< double > > dcg_ss_concrete_wm;
  map< string, vector< double > > dcg_ss_abstract_avg_wm;
  map< string, vector< double > > dcg_ss_abstract_max_wm;

  // ADCG
  vector< double > adcg_ss_concrete_size;
  vector< double > adcg_ss_abstract_avg_size;
  vector< double > adcg_ss_abstract_max_size;

  map< string, vector< double > > adcg_ss_concrete_wm;
  map< string, vector< double > > adcg_ss_abstract_avg_wm;
  map< string, vector< double > > adcg_ss_abstract_max_wm;

  // HDCG
  map< string, vector< double > > hdcg_runtimes;

  // HADCG
  map< string, vector< double > > hadcg_runtimes;
  
  for ( unsigned int i = 0; i < args.inputs_num; i++ ){
  
  // XML pointers to read the input file
  xmlDoc * input_doc = NULL;
  xmlNodePtr input_root = NULL;
  input_doc = xmlReadFile( args.inputs[ i ], NULL, 0 );

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
                      if( xmlStrcmp( l3->name, ( const xmlChar* )( "test_set" ) ) == 0 ){
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

                                      // =========== DCG SEARCH SPACE CONCRETE ================
                                      xmlChar * ss_concrete = xmlGetProp( l5, ( const xmlChar* )( "search_space_concrete_size" ) );
                                      if( ss_concrete != NULL ){
                                        stringstream ss_concrete_string;
                                        ss_concrete_string << ss_concrete;
					dcg_ss_concrete_size.push_back( std::stod( ss_concrete_string.str() ) );

                                        map< string, vector< double > >::iterator it_dcg_ss_concrete_wm = dcg_ss_concrete_wm.find( num_world_objects_string.str() );
                                        if( it_dcg_ss_concrete_wm != dcg_ss_concrete_wm.end() ){
                                          it_dcg_ss_concrete_wm->second.push_back( std::stod( ss_concrete_string.str() ) );
                                        } else{
                                          dcg_ss_concrete_wm.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( ss_concrete_string.str() ) ) ) 
                                          );
                                        }
                                        xmlFree( ss_concrete );
                                      }

                                      // =========== DCG SEARCH SPACE ABSTRACT AVG ================
                                      xmlChar * ss_abstract_avg = xmlGetProp( l5, ( const xmlChar* )( "search_space_abstract_avg_size" ) );
                                      if( ss_abstract_avg != NULL ){
                                        stringstream ss_abstract_avg_string;
                                        ss_abstract_avg_string << ss_abstract_avg;
					dcg_ss_abstract_avg_size.push_back( std::stod( ss_abstract_avg_string.str() ) );

                                        map< string, vector< double > >::iterator it_dcg_ss_abstract_avg_wm = dcg_ss_abstract_avg_wm.find( num_world_objects_string.str() );
                                        if( it_dcg_ss_abstract_avg_wm != dcg_ss_abstract_avg_wm.end() ){
                                          it_dcg_ss_abstract_avg_wm->second.push_back( std::stod( ss_abstract_avg_string.str() ) );
                                        } else{
                                          dcg_ss_abstract_avg_wm.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( ss_abstract_avg_string.str() ) ) ) 
                                          );
                                        }
                                        xmlFree( ss_abstract_avg);
                                      }


                                      // =========== DCG SEARCH SPACE ABSTRACT AVG ================
                                      xmlChar * ss_abstract_max = xmlGetProp( l5, ( const xmlChar* )( "search_space_abstract_max_size" ) );
                                      if( ss_abstract_max != NULL ){
                                        stringstream ss_abstract_max_string;
                                        ss_abstract_max_string << ss_abstract_max;
					dcg_ss_abstract_max_size.push_back( std::stod( ss_abstract_max_string.str() ) );

                                        map< string, vector< double > >::iterator it_dcg_ss_abstract_max_wm = dcg_ss_abstract_max_wm.find( num_world_objects_string.str() );
                                        if( it_dcg_ss_abstract_max_wm != dcg_ss_abstract_max_wm.end() ){
                                          it_dcg_ss_abstract_max_wm->second.push_back( std::stod( ss_abstract_max_string.str() ) );
                                        } else{
                                          dcg_ss_abstract_max_wm.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( ss_abstract_max_string.str() ) ) ) 
                                          );
                                        }
                                        xmlFree( ss_abstract_max);
                                      }

 
                                    }
                                    if( xmlStrcmp( l5->name, ( const xmlChar* )( "adcg" ) ) == 0 ){
                                      // look for the "runtime" property; this is the runtime for the given model
				      /*
                                      xmlChar * runtime = xmlGetProp( l5, ( const xmlChar* )( "runtime" ) );
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
                                      */

                                      // =========== DCG SEARCH SPACE CONCRETE ================
                                      xmlChar * ss_concrete = xmlGetProp( l5, ( const xmlChar* )( "search_space_concrete_size" ) );
                                      if( ss_concrete != NULL ){
                                        stringstream ss_concrete_string;
                                        ss_concrete_string << ss_concrete;
					adcg_ss_concrete_size.push_back( std::stod( ss_concrete_string.str() ) );

                                        map< string, vector< double > >::iterator it_adcg_ss_concrete_wm = adcg_ss_concrete_wm.find( num_world_objects_string.str() );
                                        if( it_adcg_ss_concrete_wm != adcg_ss_concrete_wm.end() ){
                                          it_adcg_ss_concrete_wm->second.push_back( std::stod( ss_concrete_string.str() ) );
                                        } else{
                                          adcg_ss_concrete_wm.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( ss_concrete_string.str() ) ) ) 
                                          );
                                        }
                                        xmlFree( ss_concrete );
                                      }

                                      // =========== DCG SEARCH SPACE ABSTRACT AVG ================
                                      xmlChar * ss_abstract_avg = xmlGetProp( l5, ( const xmlChar* )( "search_space_abstract_avg_size" ) );
                                      if( ss_abstract_avg != NULL ){
                                        stringstream ss_abstract_avg_string;
                                        ss_abstract_avg_string << ss_abstract_avg;
					adcg_ss_abstract_avg_size.push_back( std::stod( ss_abstract_avg_string.str() ) );

                                        map< string, vector< double > >::iterator it_adcg_ss_abstract_avg_wm = adcg_ss_abstract_avg_wm.find( num_world_objects_string.str() );
                                        if( it_adcg_ss_abstract_avg_wm != adcg_ss_abstract_avg_wm.end() ){
                                          it_adcg_ss_abstract_avg_wm->second.push_back( std::stod( ss_abstract_avg_string.str() ) );
                                        } else{
                                          adcg_ss_abstract_avg_wm.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( ss_abstract_avg_string.str() ) ) ) 
                                          );
                                        }
                                        xmlFree( ss_abstract_avg);
                                      }


                                      // =========== DCG SEARCH SPACE ABSTRACT AVG ================
                                      xmlChar * ss_abstract_max = xmlGetProp( l5, ( const xmlChar* )( "search_space_abstract_max_size" ) );
                                      if( ss_abstract_max != NULL ){
                                        stringstream ss_abstract_max_string;
                                        ss_abstract_max_string << ss_abstract_max;
					adcg_ss_abstract_max_size.push_back( std::stod( ss_abstract_max_string.str() ) );

                                        map< string, vector< double > >::iterator it_adcg_ss_abstract_max_wm = adcg_ss_abstract_max_wm.find( num_world_objects_string.str() );
                                        if( it_adcg_ss_abstract_max_wm != adcg_ss_abstract_max_wm.end() ){
                                          it_adcg_ss_abstract_max_wm->second.push_back( std::stod( ss_abstract_max_string.str() ) );
                                        } else{
                                          adcg_ss_abstract_max_wm.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( ss_abstract_max_string.str() ) ) ) 
                                          );
                                        }
                                        xmlFree( ss_abstract_max);
                                      }
 

                                    }
                                    if( xmlStrcmp( l5->name, ( const xmlChar* )( "hdcg" ) ) == 0 ){
                                      // look for the "runtime" property; this is the runtime for the given model
                                      xmlChar * runtime = xmlGetProp( l5, ( const xmlChar* )( "runtime" ) );
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
                                      xmlChar * runtime = xmlGetProp( l5, ( const xmlChar* )( "runtime" ) );
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


  // Memory cleanup
  // free the xml pointers
  if( input_doc != NULL ){
    xmlFreeDoc( input_doc );
  }
 
  }

  cout << "Search Space Sizes" << endl;

  if( ( dcg_ss_concrete_wm.size() > 0 ) && ( dcg_ss_abstract_avg_wm.size() > 0 ) && ( dcg_ss_abstract_max_wm.size() > 0 ) &&  
      (  adcg_ss_concrete_wm.size() > 0 ) && ( adcg_ss_abstract_avg_wm.size() > 0 ) && ( adcg_ss_abstract_max_wm.size() > 0 ) ){

    // Map to store the WM results.
    std::map< std::string, vector< double> > wm_results;

    // DCG: concrete numbers.
    for( map< string, vector< double > >::const_iterator it_dcg_ss_concrete_wm = dcg_ss_concrete_wm.begin(); it_dcg_ss_concrete_wm != dcg_ss_concrete_wm.end(); ++it_dcg_ss_concrete_wm ){
      std::map< string, vector< double> >::iterator it_wm_results = wm_results.find( it_dcg_ss_concrete_wm->first );
      if( it_wm_results != wm_results.end() ){
        cout << "avg. concrete" << average( it_dcg_ss_concrete_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_dcg_ss_concrete_wm->second ) );
        it_wm_results->second.push_back( standard_deviation( it_dcg_ss_concrete_wm->second ) );
      } else {
        wm_results.insert( pair< string, vector< double > >( it_dcg_ss_concrete_wm->first, vector< double >() ) );
        it_wm_results = wm_results.find( it_dcg_ss_concrete_wm->first );
        cout << "avg. concrete" << average( it_dcg_ss_concrete_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_dcg_ss_concrete_wm->second ) );
        it_wm_results->second.push_back( standard_deviation( it_dcg_ss_concrete_wm->second ) );
      }
    }
    
    // DCG: Abstract avg. 
    for( map< string, vector< double > >::const_iterator it_dcg_ss_abstract_avg_wm = dcg_ss_abstract_avg_wm.begin(); it_dcg_ss_abstract_avg_wm != dcg_ss_abstract_avg_wm.end(); ++it_dcg_ss_abstract_avg_wm ){
      std::map< string, vector< double> >::iterator it_wm_results = wm_results.find( it_dcg_ss_abstract_avg_wm->first );
      if( it_wm_results != wm_results.end() ){
        cout << "avg. abstract avg" << average( it_dcg_ss_abstract_avg_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_dcg_ss_abstract_avg_wm->second ) );
        it_wm_results->second.push_back( standard_deviation( it_dcg_ss_abstract_avg_wm->second ) );
      } else {
        wm_results.insert( pair< string, vector< double > >( it_dcg_ss_abstract_avg_wm->first, vector< double >() ) );
        cout << "avg. abstract avg" << average( it_dcg_ss_abstract_avg_wm->second ) << endl;
        it_wm_results = wm_results.find( it_dcg_ss_abstract_avg_wm->first );
        it_wm_results->second.push_back( average( it_dcg_ss_abstract_avg_wm->second ) );
        it_wm_results->second.push_back( standard_deviation( it_dcg_ss_abstract_avg_wm->second ) );
      }
    }

    // DCG: Abstract max 
    for( map< string, vector< double > >::const_iterator it_dcg_ss_abstract_max_wm = dcg_ss_abstract_max_wm.begin(); it_dcg_ss_abstract_max_wm != dcg_ss_abstract_max_wm.end(); ++it_dcg_ss_abstract_max_wm ){
      std::map< string, vector< double> >::iterator it_wm_results = wm_results.find( it_dcg_ss_abstract_max_wm->first );
      if( it_wm_results != wm_results.end() ){
        cout << "avg. abstract max" << average( it_dcg_ss_abstract_max_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_dcg_ss_abstract_max_wm->second ) );
        it_wm_results->second.push_back( standard_deviation( it_dcg_ss_abstract_max_wm->second ) );
      } else {
        wm_results.insert( pair< string, vector< double > >( it_dcg_ss_abstract_max_wm->first, vector< double >() ) );
        it_wm_results = wm_results.find( it_dcg_ss_abstract_max_wm->first );
        cout << "avg. abstract max" << average( it_dcg_ss_abstract_max_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_dcg_ss_abstract_max_wm->second ) );
        it_wm_results->second.push_back( standard_deviation( it_dcg_ss_abstract_max_wm->second ) );
      } 
    }

    // ADCG: concrete numbers.
    for( map< string, vector< double > >::const_iterator it_adcg_ss_concrete_wm = adcg_ss_concrete_wm.begin(); it_adcg_ss_concrete_wm != adcg_ss_concrete_wm.end(); ++it_adcg_ss_concrete_wm ){
      std::map< string, vector< double> >::iterator it_wm_results = wm_results.find( it_adcg_ss_concrete_wm->first );
      if( it_wm_results != wm_results.end() ){
        cout << "ADCG: avg. concrete" << average( it_adcg_ss_concrete_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_adcg_ss_concrete_wm->second ) );
        cout << "sd " << standard_deviation( it_adcg_ss_concrete_wm->second ) << endl;
        it_wm_results->second.push_back( standard_deviation( it_adcg_ss_concrete_wm->second ) );
      } else {
        wm_results.insert( pair< string, vector< double > >( it_adcg_ss_concrete_wm->first, vector< double >() ) );
        it_wm_results = wm_results.find( it_adcg_ss_concrete_wm->first );
        it_wm_results->second.push_back( average( it_adcg_ss_concrete_wm->second ) );
        it_wm_results->second.push_back( standard_deviation( it_adcg_ss_concrete_wm->second ) );
      }
    }
    
    // ADCG: Abstract avg. 
    for( map< string, vector< double > >::const_iterator it_adcg_ss_abstract_avg_wm = adcg_ss_abstract_avg_wm.begin(); it_adcg_ss_abstract_avg_wm != adcg_ss_abstract_avg_wm.end(); ++it_adcg_ss_abstract_avg_wm ){
      std::map< string, vector< double> >::iterator it_wm_results = wm_results.find( it_adcg_ss_abstract_avg_wm->first );
      if( it_wm_results != wm_results.end() ){
        cout << "ADCG: avg. abstract avg" << average( it_adcg_ss_abstract_avg_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_adcg_ss_abstract_avg_wm->second ) );
        cout << "sd" << standard_deviation( it_adcg_ss_abstract_avg_wm->second ) << endl;
        it_wm_results->second.push_back( standard_deviation( it_adcg_ss_abstract_avg_wm->second ) );
      } else {
        wm_results.insert( pair< string, vector< double > >( it_adcg_ss_abstract_avg_wm->first, vector< double >() ) );
        it_wm_results = wm_results.find( it_adcg_ss_abstract_avg_wm->first );
        it_wm_results->second.push_back( average( it_adcg_ss_abstract_avg_wm->second ) );
        it_wm_results->second.push_back( standard_deviation( it_adcg_ss_abstract_avg_wm->second ) );
      }
    }

    // ADCG: Abstract max 
    for( map< string, vector< double > >::const_iterator it_adcg_ss_abstract_max_wm = adcg_ss_abstract_max_wm.begin(); it_adcg_ss_abstract_max_wm != adcg_ss_abstract_max_wm.end(); ++it_adcg_ss_abstract_max_wm ){
      std::map< string, vector< double> >::iterator it_wm_results = wm_results.find( it_adcg_ss_abstract_max_wm->first );
      if( it_wm_results != wm_results.end() ){
        cout << "ADCG avg. abstract max" << average( it_adcg_ss_abstract_max_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_adcg_ss_abstract_max_wm->second ) );
        cout << "sd" << standard_deviation( it_adcg_ss_abstract_max_wm->second ) << endl;
        it_wm_results->second.push_back( standard_deviation( it_adcg_ss_abstract_max_wm->second ) );
      } else {
        wm_results.insert( pair< string, vector< double > >( it_adcg_ss_abstract_max_wm->first, vector< double >() ) );
        it_wm_results = wm_results.find( it_adcg_ss_abstract_max_wm->first );
        cout << "avg. abstract max" << average( it_adcg_ss_abstract_max_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_adcg_ss_abstract_max_wm->second ) );
        it_wm_results->second.push_back( standard_deviation( it_adcg_ss_abstract_max_wm->second ) );
      } 
    }


    // Print out the final numbers.
    cout << " World Size " << " DCG concrete + s.d. " << " DCG abstract avg + s.d. " << "" <<" DCG abstract max + s.d."; 
    cout << " ADCG concrete + s.d. " << " ADCG abstract avg + s.d. " << "" <<" ADCG abstract max + s.d." << endl; 
    for( map< string, vector< double > >::const_iterator it_wm_results = wm_results.begin(); it_wm_results != wm_results.end(); ++it_wm_results ){
      cout << it_wm_results->first << ": "  << to_string( it_wm_results->second[ 0 ] ) << " pm " << to_string( it_wm_results->second[ 1 ] ) << " ";
      cout << to_string( it_wm_results->second[ 2 ] ) << " pm " << to_string( it_wm_results->second[ 3 ] ) << " ";
      cout << to_string( it_wm_results->second[ 4 ] ) << " pm " << to_string( it_wm_results->second[ 5 ] ) << " "; 
      cout << to_string( it_wm_results->second[ 6 ] ) << " pm " << to_string( it_wm_results->second[ 7 ] ) << " "; 
      cout << to_string( it_wm_results->second[ 8 ] ) << " pm " << to_string( it_wm_results->second[ 9 ] ) << " "; 
      cout << to_string( it_wm_results->second[ 10 ] ) << " pm " << to_string( it_wm_results->second[ 11 ] ) << " " << endl; 
    }
  }

  cout << "DCG concrete size:" << endl;
  if( dcg_ss_concrete_wm.size() > 0 ){
    cout << "DCG" << endl;
    for( map< string, vector< double > >::const_iterator it_dcg_ss_concrete_wm = dcg_ss_concrete_wm.begin(); it_dcg_ss_concrete_wm != dcg_ss_concrete_wm.end(); ++it_dcg_ss_concrete_wm ){
      // find the average runtime
      double ss_concrete_average = average( it_dcg_ss_concrete_wm->second );
      double ss_concrete_standard_deviation = standard_deviation( it_dcg_ss_concrete_wm->second );
      // find the standard deviation
      cout << "  world size: " << it_dcg_ss_concrete_wm->first << endl;
      cout << "    concrete average: " << to_string( ss_concrete_average ) << endl; 
      cout << "    concrete average std: " << to_string( ss_concrete_standard_deviation ) << endl; 
    }
  } else{
    cout << "No DCG search space information" << endl;
  }
  cout << endl;

  cout << "DCG concrete size:" << endl;
  if( dcg_ss_concrete_wm.size() > 0 ){
    cout << "DCG" << endl;
    for( map< string, vector< double > >::const_iterator it_dcg_ss_concrete_wm = dcg_ss_concrete_wm.begin(); it_dcg_ss_concrete_wm != dcg_ss_concrete_wm.end(); ++it_dcg_ss_concrete_wm ){
      // find the average runtime
      double ss_concrete_average = average( it_dcg_ss_concrete_wm->second );
      double ss_concrete_standard_deviation = standard_deviation( it_dcg_ss_concrete_wm->second );
      // find the standard deviation
      cout << "  world size: " << it_dcg_ss_concrete_wm->first << endl;
      cout << "    concrete average: " << to_string( ss_concrete_average ) << endl; 
      cout << "    concrete average std: " << to_string( ss_concrete_standard_deviation ) << endl; 
    }
  } else{
    cout << "No DCG search space information" << endl;
  }
  cout << endl;




 
  if( hdcg_runtimes.size() > 0 ){
      cout << "HDCG" << endl;
      for( map< string, vector< double > >::const_iterator it_hdcg_runtimes = hdcg_runtimes.begin(); it_hdcg_runtimes != hdcg_runtimes.end(); ++it_hdcg_runtimes ){
        // find the average runtime
        double runtime_average = average( it_hdcg_runtimes->second );
        double runtime_standard_deviation = standard_deviation( it_hdcg_runtimes->second );
        // find the standard deviation
        cout << "  world size: " << it_hdcg_runtimes->first << endl;
        cout << "    runtime avg: " << to_string( runtime_average ) << endl; 
        cout << "    runtime std: " << to_string( runtime_standard_deviation ) << endl; 
    }
  } else{
    cout << "No HDCG runtime statistics to report." << endl;
  }
  cout << endl;
 
  if( hadcg_runtimes.size() > 0 ){
    cout << "HADCG" << endl;
    for( map< string, vector< double > >::const_iterator it_hadcg_runtimes = hadcg_runtimes.begin(); it_hadcg_runtimes != hadcg_runtimes.end(); ++it_hadcg_runtimes ){
      // find the average runtime
      double runtime_average = average( it_hadcg_runtimes->second );
      double runtime_standard_deviation = standard_deviation( it_hadcg_runtimes->second );
      // find the standard deviation
      cout << "  world size: " << it_hadcg_runtimes->first << endl;
      cout << "    runtime avg: " << to_string( runtime_average ) << endl; 
      cout << "    runtime std: " << to_string( runtime_standard_deviation ) << endl; 
    }
  } else{
    cout << "No HADCG runtime statistics to report." << endl;
  }
  cout << endl;
 
 cout << "end of scrape_results_runtime_vs_world_size program" << endl;
  return 0;
}
