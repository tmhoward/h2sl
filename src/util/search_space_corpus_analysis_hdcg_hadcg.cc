/**
 * @file search_space_corpus_analysis_hdcg_hadcg.cc 
 *
 * @brief
 *
 * Scrape a partition result for statistics related to 
 * search space sizes 
 *
 */

#include "search_space_corpus_analysis_hdcg_hadcg_cmdline.h"

#include <math.h> 
#include <assert.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>
#include <libxml/tree.h>
#include <glib.h>

using namespace std;


/**
 * Helper function to export a vector of doubles to a stringstream variable
 */
string
export_vector_double( vector< double >& data ){
  stringstream tmp;
  tmp << " = [";
  for( unsigned int i = 0; i < data.size(); i++ ){
    tmp << data[ i ];
    if( i != ( data.size() - 1 ) ){
      tmp << "; ";
    }
  }
  tmp << "];";
  return tmp.str();
}


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
    cout << "vector of size 0" << endl;
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

  // HDCG
  // map< string, vector< double > > hdcg_runtimes;
  vector< double > hdcg_ss_concrete_size;
  vector< double > hdcg_ss_abstract_avg_size;
  vector< double > hdcg_ss_abstract_max_size;

  map< string, vector< double > > hdcg_ss_concrete_wm;
  map< string, vector< double > > hdcg_ss_abstract_avg_wm;
  map< string, vector< double > > hdcg_ss_abstract_max_wm;

  // HADCG
  //map< string, vector< double > > hadcg_runtimes;
  vector< double > hadcg_ss_concrete_size;
  vector< double > hadcg_ss_abstract_avg_size;
  vector< double > hadcg_ss_abstract_max_size;

  map< string, vector< double > > hadcg_ss_concrete_wm;
  map< string, vector< double > > hadcg_ss_abstract_avg_wm;
  map< string, vector< double > > hadcg_ss_abstract_max_wm;

  // Iterate over the input files.  
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
                      //if( xmlStrcmp( l3->name, ( const xmlChar* )( "test_set" ) ) == 0 ){
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

                                    if( xmlStrcmp( l5->name, ( const xmlChar* )( "hdcg" ) ) == 0 ){
                                     // =========== HDCG SEARCH SPACE CONCRETE ================
                                      xmlChar * ss_concrete = xmlGetProp( l5, ( const xmlChar* )( "search_space_concrete_size" ) );
                                      if( ss_concrete != NULL ){
                                        stringstream ss_concrete_string;
                                        ss_concrete_string << ss_concrete;
					hdcg_ss_concrete_size.push_back( std::stod( ss_concrete_string.str() ) );

                                        map< string, vector< double > >::iterator it_hdcg_ss_concrete_wm = hdcg_ss_concrete_wm.find( num_world_objects_string.str() );
                                        if( it_hdcg_ss_concrete_wm != hdcg_ss_concrete_wm.end() ){
                                          it_hdcg_ss_concrete_wm->second.push_back( std::stod( ss_concrete_string.str() ) );
                                        } else{
                                          hdcg_ss_concrete_wm.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( ss_concrete_string.str() ) ) ) 
                                          );
                                        }
                                        xmlFree( ss_concrete );
                                      }

                                      // =========== HDCG SEARCH SPACE ABSTRACT AVG ================
                                      xmlChar * ss_abstract_avg = xmlGetProp( l5, ( const xmlChar* )( "search_space_abstract_avg_size" ) );
                                      if( ss_abstract_avg != NULL ){
                                        stringstream ss_abstract_avg_string;
                                        ss_abstract_avg_string << ss_abstract_avg;
					hdcg_ss_abstract_avg_size.push_back( std::stod( ss_abstract_avg_string.str() ) );

                                        map< string, vector< double > >::iterator it_hdcg_ss_abstract_avg_wm = hdcg_ss_abstract_avg_wm.find( num_world_objects_string.str() );
                                        if( it_hdcg_ss_abstract_avg_wm != hdcg_ss_abstract_avg_wm.end() ){
                                          it_hdcg_ss_abstract_avg_wm->second.push_back( std::stod( ss_abstract_avg_string.str() ) );
                                        } else{
                                          hdcg_ss_abstract_avg_wm.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( ss_abstract_avg_string.str() ) ) ) 
                                          );
                                        }
                                        xmlFree( ss_abstract_avg);
                                      }

                                      // =========== HDCG SEARCH SPACE ABSTRACT AVG ================
                                      xmlChar * ss_abstract_max = xmlGetProp( l5, ( const xmlChar* )( "search_space_abstract_max_size" ) );
                                      if( ss_abstract_max != NULL ){
                                        stringstream ss_abstract_max_string;
                                        ss_abstract_max_string << ss_abstract_max;
					hdcg_ss_abstract_max_size.push_back( std::stod( ss_abstract_max_string.str() ) );

                                        map< string, vector< double > >::iterator it_hdcg_ss_abstract_max_wm = hdcg_ss_abstract_max_wm.find( num_world_objects_string.str() );
                                        if( it_hdcg_ss_abstract_max_wm != hdcg_ss_abstract_max_wm.end() ){
                                          it_hdcg_ss_abstract_max_wm->second.push_back( std::stod( ss_abstract_max_string.str() ) );
                                        } else{
                                          hdcg_ss_abstract_max_wm.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( ss_abstract_max_string.str() ) ) ) 
                                          );
                                        }
                                        xmlFree( ss_abstract_max);
                                      }
 

                                    } // hdcg block ends
 
                                    if( xmlStrcmp( l5->name, ( const xmlChar* )( "hadcg" ) ) == 0 ){
                                     // =========== HADCG SEARCH SPACE CONCRETE ================
                                      xmlChar * ss_concrete = xmlGetProp( l5, ( const xmlChar* )( "search_space_concrete_size" ) );
                                      if( ss_concrete != NULL ){
                                        stringstream ss_concrete_string;
                                        ss_concrete_string << ss_concrete;
					hadcg_ss_concrete_size.push_back( std::stod( ss_concrete_string.str() ) );

                                        map< string, vector< double > >::iterator it_hadcg_ss_concrete_wm = hadcg_ss_concrete_wm.find( num_world_objects_string.str() );
                                        if( it_hadcg_ss_concrete_wm != hadcg_ss_concrete_wm.end() ){
                                          it_hadcg_ss_concrete_wm->second.push_back( std::stod( ss_concrete_string.str() ) );
                                        } else{
                                          hadcg_ss_concrete_wm.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( ss_concrete_string.str() ) ) ) 
                                          );
                                        }
                                        xmlFree( ss_concrete );
                                      }

                                      // =========== HADCG SEARCH SPACE ABSTRACT AVG ================
                                      xmlChar * ss_abstract_avg = xmlGetProp( l5, ( const xmlChar* )( "search_space_abstract_avg_size" ) );
                                      if( ss_abstract_avg != NULL ){
                                        stringstream ss_abstract_avg_string;
                                        ss_abstract_avg_string << ss_abstract_avg;
					hadcg_ss_abstract_avg_size.push_back( std::stod( ss_abstract_avg_string.str() ) );

                                        map< string, vector< double > >::iterator it_hadcg_ss_abstract_avg_wm = hadcg_ss_abstract_avg_wm.find( num_world_objects_string.str() );
                                        if( it_hadcg_ss_abstract_avg_wm != hadcg_ss_abstract_avg_wm.end() ){
                                          it_hadcg_ss_abstract_avg_wm->second.push_back( std::stod( ss_abstract_avg_string.str() ) );
                                        } else{
                                          hadcg_ss_abstract_avg_wm.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( ss_abstract_avg_string.str() ) ) ) 
                                          );
                                        }
                                        xmlFree( ss_abstract_avg);
                                      }

                                      // =========== HADCG SEARCH SPACE ABSTRACT AVG ================
                                      xmlChar * ss_abstract_max = xmlGetProp( l5, ( const xmlChar* )( "search_space_abstract_max_size" ) );
                                      if( ss_abstract_max != NULL ){
                                        stringstream ss_abstract_max_string;
                                        ss_abstract_max_string << ss_abstract_max;
					hadcg_ss_abstract_max_size.push_back( std::stod( ss_abstract_max_string.str() ) );

                                        map< string, vector< double > >::iterator it_hadcg_ss_abstract_max_wm = hadcg_ss_abstract_max_wm.find( num_world_objects_string.str() );
                                        if( it_hadcg_ss_abstract_max_wm != hadcg_ss_abstract_max_wm.end() ){
                                          it_hadcg_ss_abstract_max_wm->second.push_back( std::stod( ss_abstract_max_string.str() ) );
                                        } else{
                                          hadcg_ss_abstract_max_wm.insert( 
                                            pair< string, vector< double > >( num_world_objects_string.str(), vector< double >( 1, std::stod( ss_abstract_max_string.str() ) ) ) 
                                          );
                                        }
                                        xmlFree( ss_abstract_max);
                                      }
 

                                    } // hadcg block ends

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
  if( input_doc != NULL ){
    xmlFreeDoc( input_doc );
  }
 
  } // XML file parsing ends.

  cout << " Determining the Search Space sizes" << endl;
 

  if( ( hdcg_ss_concrete_wm.size() > 0 ) && ( hdcg_ss_abstract_avg_wm.size() > 0 ) && ( hdcg_ss_abstract_max_wm.size() > 0 ) &&
      (  hadcg_ss_concrete_wm.size() > 0 ) && ( hadcg_ss_abstract_avg_wm.size() > 0 ) && ( hadcg_ss_abstract_max_wm.size() > 0 ) ){

    // Map to store the WM results.
    std::map< std::string, vector< double> > wm_results;
   
    //====================  HDCG  ==============================================================

    // HDCG: concrete numbers.
    for( map< string, vector< double > >::const_iterator it_hdcg_ss_concrete_wm = hdcg_ss_concrete_wm.begin(); it_hdcg_ss_concrete_wm != hdcg_ss_concrete_wm.end(); ++it_hdcg_ss_concrete_wm ){
      std::map< string, vector< double> >::iterator it_wm_results = wm_results.find( it_hdcg_ss_concrete_wm->first );
      if( it_wm_results != wm_results.end() ){
        it_wm_results->second.push_back( it_hdcg_ss_concrete_wm->second.size() );
        cout << "HDCG: avg. concrete" << average( it_hdcg_ss_concrete_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_hdcg_ss_concrete_wm->second ) );
        cout << "sd " << standard_deviation( it_hdcg_ss_concrete_wm->second ) << endl;
        it_wm_results->second.push_back( standard_deviation( it_hdcg_ss_concrete_wm->second ) );
      } else {
        wm_results.insert( pair< string, vector< double > >( it_hdcg_ss_concrete_wm->first, vector< double >() ) );
        it_wm_results = wm_results.find( it_hdcg_ss_concrete_wm->first );
        it_wm_results->second.push_back( it_hdcg_ss_concrete_wm->second.size() );
        it_wm_results->second.push_back( average( it_hdcg_ss_concrete_wm->second ) );
        it_wm_results->second.push_back( standard_deviation( it_hdcg_ss_concrete_wm->second ) );
      }
    }
    
    // HDCG: Abstract avg. 
    for( map< string, vector< double > >::const_iterator it_hdcg_ss_abstract_avg_wm = hdcg_ss_abstract_avg_wm.begin(); it_hdcg_ss_abstract_avg_wm != hdcg_ss_abstract_avg_wm.end(); ++it_hdcg_ss_abstract_avg_wm ){
      std::map< string, vector< double> >::iterator it_wm_results = wm_results.find( it_hdcg_ss_abstract_avg_wm->first );
      if( it_wm_results != wm_results.end() ){
        cout << "HDCG: avg. abstract avg" << average( it_hdcg_ss_abstract_avg_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_hdcg_ss_abstract_avg_wm->second ) );
        cout << "sd" << standard_deviation( it_hdcg_ss_abstract_avg_wm->second ) << endl;
        it_wm_results->second.push_back( standard_deviation( it_hdcg_ss_abstract_avg_wm->second ) );
      } else {
        wm_results.insert( pair< string, vector< double > >( it_hdcg_ss_abstract_avg_wm->first, vector< double >() ) );
        it_wm_results = wm_results.find( it_hdcg_ss_abstract_avg_wm->first );
        it_wm_results->second.push_back( average( it_hdcg_ss_abstract_avg_wm->second ) );
        it_wm_results->second.push_back( standard_deviation( it_hdcg_ss_abstract_avg_wm->second ) );
      }
    }

    // HDCG: Abstract max 
    for( map< string, vector< double > >::const_iterator it_hdcg_ss_abstract_max_wm = hdcg_ss_abstract_max_wm.begin(); it_hdcg_ss_abstract_max_wm != hdcg_ss_abstract_max_wm.end(); ++it_hdcg_ss_abstract_max_wm ){
      std::map< string, vector< double> >::iterator it_wm_results = wm_results.find( it_hdcg_ss_abstract_max_wm->first );
      if( it_wm_results != wm_results.end() ){
        cout << "HDCG avg. abstract max" << average( it_hdcg_ss_abstract_max_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_hdcg_ss_abstract_max_wm->second ) );
        cout << "sd" << standard_deviation( it_hdcg_ss_abstract_max_wm->second ) << endl;
        it_wm_results->second.push_back( standard_deviation( it_hdcg_ss_abstract_max_wm->second ) );
      } else {
        wm_results.insert( pair< string, vector< double > >( it_hdcg_ss_abstract_max_wm->first, vector< double >() ) );
        it_wm_results = wm_results.find( it_hdcg_ss_abstract_max_wm->first );
        cout << "avg. abstract max" << average( it_hdcg_ss_abstract_max_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_hdcg_ss_abstract_max_wm->second ) );
        it_wm_results->second.push_back( standard_deviation( it_hdcg_ss_abstract_max_wm->second ) );
      } 
    }

     //====================  HADCG  ==============================================================

    // HADCG: concrete numbers.
    for( map< string, vector< double > >::const_iterator it_hadcg_ss_concrete_wm = hadcg_ss_concrete_wm.begin(); it_hadcg_ss_concrete_wm != hadcg_ss_concrete_wm.end(); ++it_hadcg_ss_concrete_wm ){
      std::map< string, vector< double> >::iterator it_wm_results = wm_results.find( it_hadcg_ss_concrete_wm->first );
      if( it_wm_results != wm_results.end() ){
        cout << "HADCG: avg. concrete" << average( it_hadcg_ss_concrete_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_hadcg_ss_concrete_wm->second ) );
        cout << "sd " << standard_deviation( it_hadcg_ss_concrete_wm->second ) << endl;
        it_wm_results->second.push_back( standard_deviation( it_hadcg_ss_concrete_wm->second ) );
      } else {
        wm_results.insert( pair< string, vector< double > >( it_hadcg_ss_concrete_wm->first, vector< double >() ) );
        it_wm_results = wm_results.find( it_hadcg_ss_concrete_wm->first );
        it_wm_results->second.push_back( average( it_hadcg_ss_concrete_wm->second ) );
        it_wm_results->second.push_back( standard_deviation( it_hadcg_ss_concrete_wm->second ) );
      }
    }
    
    // HADCG: Abstract avg. 
    for( map< string, vector< double > >::const_iterator it_hadcg_ss_abstract_avg_wm = hadcg_ss_abstract_avg_wm.begin(); it_hadcg_ss_abstract_avg_wm != hadcg_ss_abstract_avg_wm.end(); ++it_hadcg_ss_abstract_avg_wm ){
      std::map< string, vector< double> >::iterator it_wm_results = wm_results.find( it_hadcg_ss_abstract_avg_wm->first );
      if( it_wm_results != wm_results.end() ){
        cout << "HADCG: avg. abstract avg" << average( it_hadcg_ss_abstract_avg_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_hadcg_ss_abstract_avg_wm->second ) );
        cout << "sd" << standard_deviation( it_hadcg_ss_abstract_avg_wm->second ) << endl;
        it_wm_results->second.push_back( standard_deviation( it_hadcg_ss_abstract_avg_wm->second ) );
      } else {
        wm_results.insert( pair< string, vector< double > >( it_hadcg_ss_abstract_avg_wm->first, vector< double >() ) );
        it_wm_results = wm_results.find( it_hadcg_ss_abstract_avg_wm->first );
        it_wm_results->second.push_back( average( it_hadcg_ss_abstract_avg_wm->second ) );
        it_wm_results->second.push_back( standard_deviation( it_hadcg_ss_abstract_avg_wm->second ) );
      }
    }

    // HADCG: Abstract max 
    for( map< string, vector< double > >::const_iterator it_hadcg_ss_abstract_max_wm = hadcg_ss_abstract_max_wm.begin(); it_hadcg_ss_abstract_max_wm != hadcg_ss_abstract_max_wm.end(); ++it_hadcg_ss_abstract_max_wm ){
      std::map< string, vector< double> >::iterator it_wm_results = wm_results.find( it_hadcg_ss_abstract_max_wm->first );
      if( it_wm_results != wm_results.end() ){
        cout << "HADCG avg. abstract max" << average( it_hadcg_ss_abstract_max_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_hadcg_ss_abstract_max_wm->second ) );
        cout << "sd" << standard_deviation( it_hadcg_ss_abstract_max_wm->second ) << endl;
        it_wm_results->second.push_back( standard_deviation( it_hadcg_ss_abstract_max_wm->second ) );
      } else {
        wm_results.insert( pair< string, vector< double > >( it_hadcg_ss_abstract_max_wm->first, vector< double >() ) );
        it_wm_results = wm_results.find( it_hadcg_ss_abstract_max_wm->first );
        cout << "avg. abstract max" << average( it_hadcg_ss_abstract_max_wm->second ) << endl;
        it_wm_results->second.push_back( average( it_hadcg_ss_abstract_max_wm->second ) );
        it_wm_results->second.push_back( standard_deviation( it_hadcg_ss_abstract_max_wm->second ) );
      } 
    }

    //====================  PRINT the timing results: DCG - ADCG  ==============================================================
    cout << " World Size " << " Num examples" << " HDCG concrete + s.d. " << " HDCG abstract avg + s.d. " << "" <<" HDCG abstract max + s.d."; 
    cout << " HADCG concrete + s.d. " << " HADCG abstract avg + s.d. " << "" <<" HADCG abstract max + s.d." << endl; 
    for( map< string, vector< double > >::const_iterator it_wm_results = wm_results.begin(); it_wm_results != wm_results.end(); ++it_wm_results ){
      cout << it_wm_results->first << ": "  << to_string( it_wm_results->second[ 0 ] )  << " ";
      cout << to_string( it_wm_results->second[ 1 ] ) << " pm " << to_string( it_wm_results->second[ 2 ] ) << " ";
      cout << to_string( it_wm_results->second[ 3 ] ) << " pm " << to_string( it_wm_results->second[ 4 ] ) << " "; 
      cout << to_string( it_wm_results->second[ 5 ] ) << " pm " << to_string( it_wm_results->second[ 6 ] ) << " "; 
      cout << to_string( it_wm_results->second[ 7 ] ) << " pm " << to_string( it_wm_results->second[ 8 ] ) << " "; 
      cout << to_string( it_wm_results->second[ 9 ] ) << " pm " << to_string( it_wm_results->second[ 10 ] ) << " "; 
      cout << to_string( it_wm_results->second[ 11 ] ) << " pm " << to_string( it_wm_results->second[ 12 ] ) << " " << endl; 
    }

  //===========  Writing out the table ================================================================================
  // Writing out the table if the filename is given.
  if( args.output_text_given ){
    cout << "Writing the tex file" << endl;
    ofstream textfile;
    textfile.open( args.output_text_arg, std::ofstream::binary );

    // Print numbers for DCG and ADCG
    textfile << " World Size " << " Num examples" << " HDCG concrete + s.d. " << " HDCG abstract avg + s.d. " << "" <<" HDCG abstract max + s.d."; 
    textfile << " HADCG concrete + s.d. " << " HADCG abstract avg + s.d. " << "" <<" HADCG abstract max + s.d." << endl; 
    for( map< string, vector< double > >::const_iterator it_wm_results = wm_results.begin(); it_wm_results != wm_results.end(); ++it_wm_results ){
      textfile << it_wm_results->first << ": "  << to_string( it_wm_results->second[ 0 ] )  << " ";
      textfile << "$" << to_string( it_wm_results->second[ 1 ] ) << " \\pm " << to_string( it_wm_results->second[ 2 ] ) << "$" << " ";
      textfile << "$" << to_string( it_wm_results->second[ 3 ] ) << " \\pm " << to_string( it_wm_results->second[ 4 ] ) << "$" << " "; 
      textfile << "$" << to_string( it_wm_results->second[ 5 ] ) << " \\pm " << to_string( it_wm_results->second[ 6 ] ) << "$" << " "; 
      textfile << "$" << to_string( it_wm_results->second[ 7 ] ) << " \\pm " << to_string( it_wm_results->second[ 8 ] ) << "$" << " "; 
      textfile << "$" << to_string( it_wm_results->second[ 9 ] ) << " \\pm " << to_string( it_wm_results->second[ 10 ] ) << "$" << " "; 
      textfile << "$" << to_string( it_wm_results->second[ 11 ] ) << " \\pm " << to_string( it_wm_results->second[ 12 ] ) << "$" << " \\\\ " <<  endl; 
    } 
    textfile.close();
  }

  //======= writing out the vector of values for the scatter plot =====================================================

  // HDCG parameters for the instruction-level plot.
  cout <<  "HDCG: concrete search space" << endl;
  cout << "hdcg_ss_concrete_size" << export_vector_double( hdcg_ss_concrete_size ) << endl;
 
  cout <<  "HDCG: abstract_avg_size" << endl;
  cout << "hdcg_ss_abstract_avg_size" << export_vector_double( hdcg_ss_abstract_avg_size ) << endl;

  cout <<  "HDCG: abstract_max_size" << endl;
  cout << "hdcg_ss_abstract_max_size" << export_vector_double( hdcg_ss_abstract_max_size ) << endl;

  // HADCG parameters for the instruction-level plot.
  cout <<  "HADCG: concrete search space" << endl;
  cout << "hadcg_ss_concrete_size" << export_vector_double( hadcg_ss_concrete_size ) << endl;
 
  cout <<  "HADCG: abstract_avg_size" << endl;
  cout << "hadcg_ss_abstract_avg_size" << export_vector_double( hadcg_ss_abstract_avg_size ) << endl;

  cout <<  "HADCG: abstract_max_size" << endl;
  cout << "hadcg_ss_abstract_max_size" << export_vector_double( hadcg_ss_abstract_max_size ) << endl;

  //create the matlab file using the output argument
  if( args.output_script_given ){
    cout << "Writing the matlab script" << endl;

    ofstream outfile;
    outfile.open( args.output_script_arg, std::ofstream::binary );
    outfile << "\% program to plot the search space statistics from the h2sl::search_space_corpus_analysis program" << endl;

    //newline to output matlab file
    outfile << endl;
    outfile << "clear all;" << endl << endl;

    // HDCG parameters for the instruction-level plot.
    outfile << "hdcg_ss_concrete_size" << export_vector_double( hdcg_ss_concrete_size ) << endl;
    outfile << "hdcg_ss_abstract_avg_size" << export_vector_double( hdcg_ss_abstract_avg_size ) << endl;
    outfile << "hdcg_ss_abstract_max_size" << export_vector_double( hdcg_ss_abstract_max_size ) << endl;
    outfile << "hdcg_adaptive_avg = hdcg_ss_concrete_size + hdcg_ss_abstract_avg_size;" << endl;
    //outfile << "adaptive_max = hdcg_ss_concrete_size + hdcg_ss_abstract_max_size;" << endl;
    outfile << endl << endl;

    // HADCG parameters for the instruction-level plot.
    outfile << "hadcg_ss_concrete_size" << export_vector_double( hadcg_ss_concrete_size ) << endl;
    outfile << "hadcg_ss_abstract_avg_size" << export_vector_double( hadcg_ss_abstract_avg_size ) << endl;
    outfile << "hadcg_ss_abstract_max_size" << export_vector_double( hadcg_ss_abstract_max_size ) << endl;
    outfile << "hadcg_adaptive_avg = hadcg_ss_concrete_size + hadcg_ss_abstract_avg_size;" << endl;
    outfile << "hadcg_adaptive_max = hadcg_ss_concrete_size + hadcg_ss_abstract_max_size;" << endl;
    outfile << endl << endl;

    // HDCG - HADCG plot. Plot the search space figure. 
    outfile << "figure(1);" << endl;
    outfile << "hold on;" << endl;
    outfile << "box on;" << endl;
    outfile << "grid on;" << endl;
    outfile << "scatter(baseline,baseline,200);" << endl;
    outfile << "scatter(baseline,hdcg_adaptive_avg,200,'.');" << endl;
    outfile << "scatter(baseline,hadcg_adaptive_avg,200,'+');" << endl;
    outfile << "scatter(baseline,hadcg_adaptive_max,200,'^');" << endl;
    outfile << "legend('DCG','HDCG avg', 'HADCG avg', 'HADCG avg max', 'Location','NorthWest');" << endl;
    outfile << "title('Search space comparison');" << endl;
    outfile << "xlhand = get(gca,'xlabel');" << endl;
    outfile << "set(xlhand,'string','Total search space size','fontsize',15);" << endl;
    outfile << "ylhand = get(gca,'ylabel');" << endl;
    outfile << "set(ylhand,'string','Instantiated search space size','fontsize',15);" << endl;
    outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
    outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
    outfile << "set(gca, 'xscale', 'log');" << endl; 
    outfile << "set(gca, 'yscale', 'log');" << endl; 

    outfile << endl << endl;
    outfile << "saveas(gcf, 'search_space_comparison_hdcg_hadcg', 'epsc');" << endl << endl;
    outfile << "saveas(gcf, 'search_space_comparison_hdcg_hadcg', 'pdf');" << endl << endl;

    /*
    // All combined plot. Plot the search space figure. 
    outfile << "figure(3);" << endl;
    outfile << "hold on;" << endl;
    outfile << "box on;" << endl;
    outfile << "grid on;" << endl;
    outfile << "scatter(baseline,baseline,200);" << endl;
    outfile << "scatter(baseline,hdcg_adaptive_avg,200,'.');" << endl;
    outfile << "scatter(baseline,adcg_adaptive_avg,200,'*');" << endl;
    outfile << "scatter(baseline,hadcg_adaptive_avg,200,'^');" << endl;
    outfile << "legend('DCG','HDCG avg.', 'ADCG avg.',  'HADCG avg.', 'Location','NorthWest');" << endl;
    outfile << "title('Search space comparison');" << endl;
    outfile << "xlhand = get(gca,'xlabel');" << endl;
    outfile << "set(xlhand,'string','Total search space size','fontsize',15);" << endl;
    outfile << "ylhand = get(gca,'ylabel');" << endl;
    outfile << "set(ylhand,'string','Instantiated search space size','fontsize',15);" << endl;
    outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
    outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
    outfile << "set(gca, 'yscale', 'log');" << endl; 

    outfile << endl << endl;
    outfile << "saveas(gcf, 'search_space_comparison_all', 'epsc');" << endl << endl;
    outfile << "saveas(gcf, 'search_space_comparison_all', 'pdf');" << endl << endl;

    // DCG - HDCG  plot. Plot the search space figure. 
    outfile << "figure(4);" << endl;
    outfile << "hold on;" << endl;
    outfile << "box on;" << endl;
    outfile << "grid on;" << endl;
    outfile << "scatter(baseline,baseline,200);" << endl;
    outfile << "scatter(baseline,hdcg_adaptive_avg,200,'.');" << endl;
    outfile << "legend('DCG','HDCG avg','Location','NorthWest');" << endl;
    outfile << "title('Search space comparison');" << endl;
    outfile << "xlhand = get(gca,'xlabel');" << endl;
    outfile << "set(xlhand,'string','Total search space size','fontsize',15);" << endl;
    outfile << "ylhand = get(gca,'ylabel');" << endl;
    outfile << "set(ylhand,'string','Instantiated search space size','fontsize',15);" << endl;
    outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
    outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
    outfile << "set(gca, 'yscale', 'log');" << endl; 

    outfile << endl << endl;
    outfile << "saveas(gcf, 'search_space_comparison_dcg_hdcg', 'epsc');" << endl << endl;
    outfile << "saveas(gcf, 'search_space_comparison_dcg_hdcg', 'pdf');" << endl << endl;

    // ADCG - HADCG  plot. Plot the search space figure. 
    outfile << "figure(5);" << endl;
    outfile << "hold on;" << endl;
    outfile << "box on;" << endl;
    outfile << "grid on;" << endl;
    outfile << "scatter(baseline,adcg_adaptive_avg,200,'+');" << endl;
    outfile << "scatter(baseline,adcg_adaptive_max,200,'^');" << endl;
    outfile << "scatter(baseline,hadcg_adaptive_avg,200,'*');" << endl; 
    outfile << "scatter(baseline,hadcg_adaptive_max,200,'.');" << endl;
    outfile << "legend('ADCG avg', 'ADCG max', 'HADCG avg', 'HADCG max', 'Location','NorthWest');" << endl;
    outfile << "title('Search space comparison');" << endl;
    outfile << "xlhand = get(gca,'xlabel');" << endl;
    outfile << "set(xlhand,'string','Total search space size','fontsize',15);" << endl;
    outfile << "ylhand = get(gca,'ylabel');" << endl;
    outfile << "set(ylhand,'string','Instantiated search space size','fontsize',15);" << endl;
    outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
    outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
    outfile << "set(gca, 'yscale', 'log');" << endl; 

    outfile << endl << endl;
    outfile << "saveas(gcf, 'search_space_comparison_adcg_hadcg', 'epsc');" << endl << endl;
    outfile << "saveas(gcf, 'search_space_comparison_adcg_hadcg', 'pdf');" << endl << endl;
    */
 
    /* 
    // Plot the log-log scale figure. 
    outfile << "figure(2);" << endl;
    outfile << "hold on;" << endl;
    outfile << "box on;" << endl;
    outfile << "grid on;" << endl;
    //outfile << "ylim( [0.0 1.0] );" << endl;
    //outfile << "xlim( [0.1 0.9] );" << endl;
    outfile << "scatter(baseline,baseline,200);" << endl;
    outfile << "scatter(baseline,adaptive_avg,200,'+');" << endl;
    outfile << "scatter(baseline,adaptive_max,200,'^');" << endl;
    //outfile << "plot(training_ratios_matrix_means,training_set_dcg_accuracies_root_matrix_means,training_ratios_matrix_means,training_set_dcg_accuracies_complete_matrix_means);" << endl;
    outfile << "legend('DCG','ADCG avg', 'ADCG max','Location','NorthWest');" << endl;
    outfile << "title('Search space comparison log-log scale');" << endl;
    outfile << "xlhand = get(gca,'xlabel');" << endl;
    outfile << "set(xlhand,'string','Total search space size (log-scale) ','fontsize',15);" << endl;
    outfile << "ylhand = get(gca,'ylabel');" << endl;
    outfile << "set(ylhand,'string','Instantiated search space size (log-scale)','fontsize',15);" << endl;
    outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
    outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
    outfile << "set(gca, 'xscale', 'log');" << endl;
    outfile << "set(gca, 'yscale', 'log');" << endl; 
 
    outfile << endl << endl;
    outfile << "saveas(gcf, 'search_space_comparison log log scale', 'epsc');" << endl << endl;
    outfile << "saveas(gcf, 'search_space_comparison log log scale', 'pdf');" << endl << endl;
    */
  
    outfile.close();
  } // matlab script writing ends here.
 
  } // Check for vectors not being zero ends here.
  
  cout << "end of search space corpus analysis program" << endl;
  return 0;
}
