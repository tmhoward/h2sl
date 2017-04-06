/**
 * @file test_convert_xml_to_mat.cc
 *
 * @brief
 *
 * program used to convert xml results file to matlab format given:
 *  -an input xml results file (args.input_arg; no default)
 *  -an output filename to save the matlab code (args.output_arg; no default)
 */

#include "test_convert_xml_to_mat_cmdline.h"

#include <assert.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <libxml/tree.h>

#include "h2sl/common.h"

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
 *`Convert XML to MAT Program
 */
int
main( int argc,
      char* argv[] ){
  cout << "start of test_convert_xml_to_mat program" << endl;

  gengetopt_args_info args;
  if( cmdline_parser( argc, argv, &args ) != 0 ){
    exit( 1 );
  }

  //create the matlab file using the output argument
  ofstream outfile;
  outfile.open( args.output_arg, std::ofstream::binary );
    
  outfile << "\% program to plot the accuracy and runtime results from the h2sl::test_process program" << endl;

  //vectors of relevant properties of the results for different tests in the cross-vaildation
  vector< double > training_set_sizes; //training_set sizes
  vector< double > training_ratios; //percentage of examples in the training_sets
  vector< double > test_avg_num_phrases; //average number of phrases for all examples in a given test_set
  vector< double > training_avg_num_phrases; //average number of phrases for all examples in a given training_set
  vector< double > test_avg_num_objects; //average number of objects for all examples in a given test_set
  vector< double > training_avg_num_objects; //average number of objects for all examples in a given training_set

  //dcg model accuracy and runtime results vectors
  vector< double > training_set_dcg_runtimes; //runtimes for the dcg model on the training_sets
  vector< double > test_set_dcg_runtimes; //runtimes for the dcg model on the test_sets
  vector< double > training_set_dcg_accuracies_root; //root-accuracy results for the dcg model on the training_sets
  vector< double > test_set_dcg_accuracies_root; //root-accuracy results for the dcg model on the test_sets
  vector< double > training_set_dcg_accuracies_complete; //complete-accuracy results for the dcg model on the training_sets
  vector< double > test_set_dcg_accuracies_complete; //complete-accuracy results for the dcg model on the trainings_sets

  //adcg model accuracy and runtime results vectors
  vector< double > training_set_adcg_runtimes; //runtimes for the adcg model on the training_sets
  vector< double > test_set_adcg_runtimes; //runtime for the adcg model on the test_sets
  vector< double > training_set_adcg_accuracies_root; //root-accuracy results for the adcg model on the training_sets
  vector< double > test_set_adcg_accuracies_root; //root-accuracy results for the adcg model on the test_sets
  vector< double > training_set_adcg_accuracies_complete; //complete-accuracy results for the adcg model on the training_sets
  vector< double > test_set_adcg_accuracies_complete; //complete-accuracy results for the adcg model on the test_sets

  //hdcg model accuracy and runtime results vectors
  vector< double > training_set_hdcg_runtimes; //runtimes for the hdcg model on the training_sets
  vector< double > test_set_hdcg_runtimes; //runtime for the hdcg model on the test_sets
  vector< double > training_set_hdcg_accuracies_root; //root-accuracy results for the hdcg model on the training_sets
  vector< double > test_set_hdcg_accuracies_root; //root-accuracy results for the hdcg model on the test_sets
  vector< double > training_set_hdcg_accuracies_complete; //complete-accuracy results for the hdcg model on the training_sets
  vector< double > test_set_hdcg_accuracies_complete; //complete-accuracy results for the hdcg model on the test_sets

  //hadcg model accuracy and runtime results vectors
  vector< double > training_set_hadcg_runtimes; //runtimes for the hadcg model on the training_sets
  vector< double > test_set_hadcg_runtimes; //runtime for the hadcg model on the test_sets
  vector< double > training_set_hadcg_accuracies_root; //root-accuracy results for the hadcg model on the training_sets
  vector< double > test_set_hadcg_accuracies_root; //root-accuracy results for the hadcg model on the test_sets
  vector< double > training_set_hadcg_accuracies_complete; //complete-accuracy results for the hadcg model on the training_sets
  vector< double > test_set_hadcg_accuracies_complete; //complete-accuracy results for the hadcg model on the test_sets
  
  //read the provided input xml results file
  xmlDoc * doc = NULL;
  xmlNodePtr root = NULL;
  doc = xmlReadFile( args.input_arg, NULL, 0 );
  
  //march down the tree structure to find the cross-validation tests
  if( doc != NULL ){
    root = xmlDocGetRootElement( doc );
    if( root->type == XML_ELEMENT_NODE ){
      //loop through each child of the root node
      for( xmlNodePtr l1 = root->children; l1; l1 = l1->next ){
        if( l1->type == XML_ELEMENT_NODE ){
          //look for a node named "tests"
          if( xmlStrcmp( l1->name, ( const xmlChar* )( "results" ) ) == 0 ){
            //loop through each child node of the "tests" xml node
            for( xmlNodePtr l2 = l1->children; l2; l2 = l2->next ){
              if( l2->type == XML_ELEMENT_NODE ){
                //look for a node named "test"
                if( xmlStrcmp( l2->name, ( const xmlChar* )( "test" ) ) == 0 ){

                  //RELEVANT PROPERTIES EXTRACTION
                  //extract the training set size
                  pair< bool, double > training_set_size_prop = h2sl::has_prop< double >( l2, "training_set_size" );
                  if( training_set_size_prop.first ){
                    training_set_sizes.push_back( training_set_size_prop.second );
                  }
                  //extract the training ratios
                  pair< bool, double > training_ratio_prop = h2sl::has_prop< double >( l2, "training_ratio" );
                  if( training_ratio_prop.first ){
                    training_ratios.push_back( training_ratio_prop.second );
                  }
                  //extract the training set average number of phrases 
                  pair< bool, double > training_avg_num_phrases_prop = h2sl::has_prop< double >( l2, "training_set_avg_num_phrases" );
                  if( training_avg_num_phrases_prop.first ){
                    training_avg_num_phrases.push_back( training_avg_num_phrases_prop.second );
                  }
                  //extract the test set average number of phrases 
                  pair< bool, double > test_avg_num_phrases_prop = h2sl::has_prop< double >( l2, "test_set_avg_num_phrases" );
                  if( test_avg_num_phrases_prop.first ){
                    test_avg_num_phrases.push_back( test_avg_num_phrases_prop.second );
                  }
                  //extract the training set average number of objects 
                  pair< bool, double > training_avg_num_objects_prop = h2sl::has_prop< double >( l2, "training_set_avg_num_objects" );
                  if( training_avg_num_objects_prop.first ){
                    training_avg_num_objects.push_back( training_avg_num_objects_prop.second );
                  }
                  //extract the test set average number of objects 
                  pair< bool, double > test_avg_num_objects_prop = h2sl::has_prop< double >( l2, "test_set_avg_num_objects" );
                  if( test_avg_num_objects_prop.first ){
                    test_avg_num_objects.push_back( test_avg_num_objects_prop.second );
                  }


                  //DCG RESULTS EXTRACTION
                  //extract the training set runtimes for dcg
                  pair< bool, double > training_set_runtime_dcg_prop = h2sl::has_prop< double >( l2, "training_set_runtime_dcg" );
                  if( training_set_runtime_dcg_prop.first ){
                    training_set_dcg_runtimes.push_back( training_set_runtime_dcg_prop.second );
                  }
                  //extract the test set runtimes for dcg
                  pair< bool, double > test_set_runtime_dcg_prop = h2sl::has_prop< double >( l2, "test_set_runtime_dcg" );
                  if( test_set_runtime_dcg_prop.first ){
                    test_set_dcg_runtimes.push_back( test_set_runtime_dcg_prop.second );
                  }
                  //extract the training set correct for dcg root-accuracy
                  pair< bool, double > training_set_correct_root_dcg_prop = h2sl::has_prop< double >( l2, "training_set_correct_root_dcg" );
                  if( training_set_correct_root_dcg_prop.first ){
                    training_set_dcg_accuracies_root.push_back( training_set_correct_root_dcg_prop.second );
                  }
                  //extract the test set correct for dcg root-accuracy
                  pair< bool, double > test_set_correct_root_dcg_prop = h2sl::has_prop< double >( l2, "test_set_correct_root_dcg" );
                  if( test_set_correct_root_dcg_prop.first ){
                    test_set_dcg_accuracies_root.push_back( test_set_correct_root_dcg_prop.second );
                  }
                  //extract the training set correct for dcg complete-accuracy
                  pair< bool, double > training_set_correct_complete_dcg_prop = h2sl::has_prop< double >( l2, "training_set_correct_complete_dcg" );
                  if( training_set_correct_complete_dcg_prop.first ){
                    training_set_dcg_accuracies_complete.push_back( training_set_correct_complete_dcg_prop.second );
                  }
                  //extract the test set correct for dcg complete-accuracy
                  pair< bool, double > test_set_correct_complete_dcg_prop = h2sl::has_prop< double >( l2, "test_set_correct_complete_dcg" );
                  if( test_set_correct_complete_dcg_prop.first ){
                    test_set_dcg_accuracies_complete.push_back( test_set_correct_complete_dcg_prop.second );
                  }


                  //ADCG RESULTS EXTRACTION
                  //extract the training set runtimes for adcg
                  pair< bool, double > training_set_runtime_adcg_prop = h2sl::has_prop< double >( l2, "training_set_runtime_adcg" );
                  if( training_set_runtime_adcg_prop.first ){
                    training_set_adcg_runtimes.push_back( training_set_runtime_adcg_prop.second );
                  }
                  //extract the test set runtimes for adcg
                  pair< bool, double > test_set_runtime_adcg_prop = h2sl::has_prop< double >( l2, "test_set_runtime_adcg" );
                  if( test_set_runtime_adcg_prop.first ){
                    test_set_adcg_runtimes.push_back( test_set_runtime_adcg_prop.second );
                  }
                  //extract the training set correct for adcg root-accuracy
                  pair< bool, double > training_set_correct_root_adcg_prop = h2sl::has_prop< double >( l2, "training_set_correct_root_adcg" );
                  if( training_set_correct_root_adcg_prop.first ){
                    training_set_adcg_accuracies_root.push_back( training_set_correct_root_adcg_prop.second );
                  }
                  //extract the test set correct for adcg root-accuracy
                  pair< bool, double > test_set_correct_root_adcg_prop = h2sl::has_prop< double >( l2, "test_set_correct_root_adcg" );
                  if( test_set_correct_root_adcg_prop.first ){
                    test_set_adcg_accuracies_root.push_back( test_set_correct_root_adcg_prop.second );
                  }
                  //extract the training set correct for adcg complete-accuracy
                  pair< bool, double > training_set_correct_complete_adcg_prop = h2sl::has_prop< double >( l2, "training_set_correct_complete_adcg" );
                  if( training_set_correct_complete_adcg_prop.first ){
                    training_set_adcg_accuracies_complete.push_back( training_set_correct_complete_adcg_prop.second );
                  }
                  //extract the test set correct for adcg complete-accuracy
                  pair< bool, double > test_set_correct_complete_adcg_prop = h2sl::has_prop< double >( l2, "test_set_correct_complete_adcg" );
                  if( test_set_correct_complete_adcg_prop.first ){
                    test_set_adcg_accuracies_complete.push_back( test_set_correct_complete_adcg_prop.second );
                  }


                  //HDCG RESULTS EXTRACTION
                  //extract the training set runtimes for hdcg
                  pair< bool, double > training_set_runtime_hdcg_prop = h2sl::has_prop< double >( l2, "training_set_runtime_hdcg" );
                  if( training_set_runtime_hdcg_prop.first ){
                    training_set_hdcg_runtimes.push_back( training_set_runtime_hdcg_prop.second );
                  }
                  //extract the test set runtimes for hdcg
                  pair< bool, double > test_set_runtime_hdcg_prop = h2sl::has_prop< double >( l2, "test_set_runtime_hdcg" );
                  if( test_set_runtime_hdcg_prop.first ){
                    test_set_hdcg_runtimes.push_back( test_set_runtime_hdcg_prop.second );
                  }
                  //extract the training set correct for hdcg root-accuracy
                  pair< bool, double > training_set_correct_root_hdcg_prop = h2sl::has_prop< double >( l2, "training_set_correct_root_hdcg" );
                  if( training_set_correct_root_hdcg_prop.first ){
                    training_set_hdcg_accuracies_root.push_back( training_set_correct_root_hdcg_prop.second );
                  }
                  //extract the test set correct for hdcg root-accuracy
                  pair< bool, double > test_set_correct_root_hdcg_prop = h2sl::has_prop< double >( l2, "test_set_correct_root_hdcg" );
                  if( test_set_correct_root_hdcg_prop.first ){
                    test_set_hdcg_accuracies_root.push_back( test_set_correct_root_hdcg_prop.second );
                  }
                  //extract the training set correct for hdcg complete-accuracy
                  pair< bool, double > training_set_correct_complete_hdcg_prop = h2sl::has_prop< double >( l2, "training_set_correct_complete_hdcg" );
                  if( training_set_correct_complete_hdcg_prop.first ){
                    training_set_hdcg_accuracies_complete.push_back( training_set_correct_complete_hdcg_prop.second );
                  }
                  //extract the test set correct for hdcg complete-accuracy
                  pair< bool, double > test_set_correct_complete_hdcg_prop = h2sl::has_prop< double >( l2, "test_set_correct_complete_hdcg" );
                  if( test_set_correct_complete_hdcg_prop.first ){
                    test_set_hdcg_accuracies_complete.push_back( test_set_correct_complete_hdcg_prop.second );
                  }


                  //HADCG RESULTS EXTRACTION
                  //extract the training set runtimes for hadcg
                  pair< bool, double > training_set_runtime_hadcg_prop = h2sl::has_prop< double >( l2, "training_set_runtime_hadcg" );
                  if( training_set_runtime_hadcg_prop.first ){
                    training_set_hadcg_runtimes.push_back( training_set_runtime_hadcg_prop.second );
                  }
                  //extract the test set runtimes for hadcg
                  pair< bool, double > test_set_runtime_hadcg_prop = h2sl::has_prop< double >( l2, "test_set_runtime_hadcg" );
                  if( test_set_runtime_hadcg_prop.first ){
                    test_set_hadcg_runtimes.push_back( test_set_runtime_hadcg_prop.second );
                  }
                  //extract the training set correct for hadcg root-accuracy
                  pair< bool, double > training_set_correct_root_hadcg_prop = h2sl::has_prop< double >( l2, "training_set_correct_root_hadcg" );
                  if( training_set_correct_root_hadcg_prop.first ){
                    training_set_hadcg_accuracies_root.push_back( training_set_correct_root_hadcg_prop.second );
                  }
                  //extract the test set correct for hadcg root-accuracy
                  pair< bool, double > test_set_correct_root_hadcg_prop = h2sl::has_prop< double >( l2, "test_set_correct_root_hadcg" );
                  if( test_set_correct_root_hadcg_prop.first ){
                    test_set_hadcg_accuracies_root.push_back( test_set_correct_root_hadcg_prop.second );
                  }
                  //extract the training set correct for hadcg complete-accuracy
                  pair< bool, double > training_set_correct_complete_hadcg_prop = h2sl::has_prop< double >( l2, "training_set_correct_complete_hadcg" );
                  if( training_set_correct_complete_hadcg_prop.first ){
                    training_set_hadcg_accuracies_complete.push_back( training_set_correct_complete_hadcg_prop.second );
                  }
                  //extract the test set correct for hadcg complete-accuracy
                  pair< bool, double > test_set_correct_complete_hadcg_prop = h2sl::has_prop< double >( l2, "test_set_correct_complete_hadcg" );
                  if( test_set_correct_complete_hadcg_prop.first ){
                    test_set_hadcg_accuracies_complete.push_back( test_set_correct_complete_hadcg_prop.second );
                  }


/*
    OLD CODE TO EXTRACT AVERAGE PHRASES AND OBJECTS FROM TRAINING && TEST SETS
    JAKE: leaving this as a comment until test_process.cc is updated to either include
      the averages at the test_group level ("test" xml node) and use the code above OR
      leave the averages at the test_set and training_set node level and use the code below


                  //we are creating a temporary search into the sub-nodes (training_set and test_set nodes) to get the phrase and object info
                  //the test_process.cc has been modified to include these values at the 'test' node, but the entire test_process would need to be run again
                  //therefore, this loop is effectively a solution to a very temporary problem and should be replaced ASAP. The info scraping will be done as above
                 for( xmlNodePtr l3 = root->children; l3; l3 = l3->next ){
                    if( l3->type == XML_ELEMENT_NODE ){
                      if( xmlStrcmp( l3->name, ( const xmlChar* )( "test_set" ) ) == 0 ){             
                        assert( false );
                        //extract the average number of phrases in the test set
                        //pair< bool, double > test_avg_num_phrases_prop = rocbot::has_prop< double >( l3, "avg_num_phrases" );
                        pair< bool, double > test_avg_num_phrases_prop = h2sl::has_prop< double >( l3, "avg_num_phrases" );
                        if( test_avg_num_phrases_prop.first ){
                          test_avg_num_phrases.push_back( test_avg_num_phrases_prop.second );
                        }
                        //extract the average number of objects in the test set
                        //pair< bool, double > test_avg_num_objects_prop = rocbot::has_prop< double >( l3, "avg_num_objects" );
                        pair< bool, double > test_avg_num_objects_prop = h2sl::has_prop< double >( l3, "avg_num_objects" );
                        if( test_avg_num_objects_prop.first ){
                          test_avg_num_objects.push_back( test_avg_num_objects_prop.second );
                        }
                      }
                      if( xmlStrcmp( l3->name, ( const xmlChar* )( "training_set" ) ) == 0 ){             
                        //extract the average number of phrases in the training set
                        //pair< bool, double > training_avg_num_phrases_prop = rocbot::has_prop< double >( l3, "avg_num_phrases" );
                        pair< bool, double > training_avg_num_phrases_prop = h2sl::has_prop< double >( l3, "avg_num_phrases" );
                        if( training_avg_num_phrases_prop.first ){
                          training_avg_num_phrases.push_back( training_avg_num_phrases_prop.second );
                        }
                        //extract the average number of objects in the training set
                        //pair< bool, double > training_avg_num_objects_prop = rocbot::has_prop< double >( l3, "avg_num_objects" );
                        pair< bool, double > training_avg_num_objects_prop = h2sl::has_prop< double >( l3, "avg_num_objects" );
                        if( training_avg_num_objects_prop.first ){
                          training_avg_num_objects.push_back( training_avg_num_objects_prop.second );
                        }
                      }
                    }
                  }
*/
                }
              }
            }
          }
        }
      }
      xmlFreeDoc( doc );
    }
  }

  //print extracted relevant properties
  cout << "training_ratios" << h2sl::std_vector_to_std_string( training_ratios ) << endl;
  cout << "training_set_sizes" << h2sl::std_vector_to_std_string( training_set_sizes ) << endl;
  //print extracted DCG results
  cout << "training_set_dcg_runtimes" << h2sl::std_vector_to_std_string( training_set_dcg_runtimes ) << endl;
  cout << "test_set_dcg_runtimes" << h2sl::std_vector_to_std_string( test_set_dcg_runtimes ) << endl;
  cout << "training_set_dcg_accuracies_root" << h2sl::std_vector_to_std_string( training_set_dcg_accuracies_root ) << endl;
  cout << "test_set_dcg_accuracies_complete" << h2sl::std_vector_to_std_string( test_set_dcg_accuracies_complete ) << endl;
  cout << "training_set_dcg_accuracies_complete" << h2sl::std_vector_to_std_string( training_set_dcg_accuracies_complete ) << endl;
  cout << "test_set_dcg_accuracies_root" << h2sl::std_vector_to_std_string( test_set_dcg_accuracies_root ) << endl;
  //print extracted ADCG results
  cout << "training_set_adcg_runtimes" << h2sl::std_vector_to_std_string( training_set_adcg_runtimes ) << endl;
  cout << "test_set_adcg_runtimes" << h2sl::std_vector_to_std_string( test_set_adcg_runtimes ) << endl;
  cout << "training_set_adcg_accuracies_root" << h2sl::std_vector_to_std_string( training_set_adcg_accuracies_root ) << endl;
  cout << "test_set_adcg_accuracies_root" << h2sl::std_vector_to_std_string( test_set_adcg_accuracies_root ) << endl;
  cout << "training_set_adcg_accuracies_complete" << h2sl::std_vector_to_std_string( training_set_adcg_accuracies_complete ) << endl;
  cout << "test_set_adcg_accuracies_complete" << h2sl::std_vector_to_std_string( test_set_adcg_accuracies_complete ) << endl;
  //print extracted HDCG results
  cout << "training_set_hdcg_runtimes" << h2sl::std_vector_to_std_string( training_set_hdcg_runtimes ) << endl;
  cout << "test_set_hdcg_runtimes" << h2sl::std_vector_to_std_string( test_set_hdcg_runtimes ) << endl;
  cout << "training_set_hdcg_accuracies_root" << h2sl::std_vector_to_std_string( training_set_hdcg_accuracies_root ) << endl;
  cout << "test_set_hdcg_accuracies_root" << h2sl::std_vector_to_std_string( test_set_hdcg_accuracies_root ) << endl;
  cout << "training_set_hdcg_accuracies_complete" << h2sl::std_vector_to_std_string( training_set_hdcg_accuracies_complete ) << endl;
  cout << "test_set_hdcg_accuracies_complete" << h2sl::std_vector_to_std_string( test_set_hdcg_accuracies_complete ) << endl;
  //print extracted HADCG results
  cout << "training_set_hadcg_runtimes" << h2sl::std_vector_to_std_string( training_set_hadcg_runtimes ) << endl;
  cout << "test_set_hadcg_runtimes" << h2sl::std_vector_to_std_string( test_set_hadcg_runtimes ) << endl;
  cout << "training_set_hadcg_accuracies_root" << h2sl::std_vector_to_std_string( training_set_hadcg_accuracies_root ) << endl;
  cout << "test_set_hadcg_accuracies_root" << h2sl::std_vector_to_std_string( test_set_hadcg_accuracies_root ) << endl;
  cout << "training_set_hadcg_accuracies_complete" << h2sl::std_vector_to_std_string( training_set_hadcg_accuracies_complete ) << endl;
  cout << "test_set_hadcg_accuracies_complete" << h2sl::std_vector_to_std_string( test_set_hadcg_accuracies_complete ) << endl;

  //newline to output matlab file
  outfile << endl;
  outfile << "clear all;" << endl << endl;

  //add extracted relevant properties to the output matlab file
  outfile << "\% extracted relevant properties; used for constructing matrices for plots" << endl;
  outfile << "training_set_sizes" << export_vector_double( training_set_sizes ) << endl;
  outfile << "training_ratios" << export_vector_double( training_ratios ) << endl << endl;
  //add extracted DCG results to the output matlab file
  outfile << "\% DCG training_set/test_set accuracy && runtime results" << endl;
  outfile << "\% later converted to matrix form" << endl;
  outfile << "training_set_dcg_runtimes" << export_vector_double( training_set_dcg_runtimes ) << endl;
  outfile << "test_set_dcg_runtimes" << export_vector_double( test_set_dcg_runtimes ) << endl;
  outfile << "training_set_dcg_accuracies_root" << export_vector_double( training_set_dcg_accuracies_root ) << endl;
  outfile << "test_set_dcg_accuracies_root" << export_vector_double( test_set_dcg_accuracies_root ) << endl;
  outfile << "training_set_dcg_accuracies_complete" << export_vector_double( training_set_dcg_accuracies_complete ) << endl;
  outfile << "test_set_dcg_accuracies_complete" << export_vector_double( test_set_dcg_accuracies_complete ) << endl << endl;
  //add extracted ADCG results to the output matlab file
  outfile << "\% ADCG training_set/test_set accuracy && runtime results" << endl;
  outfile << "\% later converted to matrix form" << endl;
  outfile << "training_set_adcg_runtimes" << export_vector_double( training_set_adcg_runtimes ) << endl;
  outfile << "test_set_adcg_runtimes" << export_vector_double( test_set_adcg_runtimes ) << endl;
  outfile << "training_set_adcg_accuracies_root" << export_vector_double( training_set_adcg_accuracies_root ) << endl;
  outfile << "test_set_adcg_accuracies_root" << export_vector_double( test_set_adcg_accuracies_root ) << endl;
  outfile << "training_set_adcg_accuracies_complete" << export_vector_double( training_set_adcg_accuracies_complete ) << endl;
  outfile << "test_set_adcg_accuracies_complete" << export_vector_double( test_set_adcg_accuracies_complete ) << endl << endl;
  //add extracted HDCG results to the output matlab file
  outfile << "\% HDCG training_set/test_set accuracy && runtime results" << endl;
  outfile << "\% later converted to matrix form" << endl;
  outfile << "training_set_hdcg_runtimes" << export_vector_double( training_set_hdcg_runtimes ) << endl;
  outfile << "test_set_hdcg_runtimes" << export_vector_double( test_set_hdcg_runtimes ) << endl;
  outfile << "training_set_hdcg_accuracies_root" << export_vector_double( training_set_hdcg_accuracies_root ) << endl;
  outfile << "test_set_hdcg_accuracies_root" << export_vector_double( test_set_hdcg_accuracies_root ) << endl;
  outfile << "training_set_hdcg_accuracies_complete" << export_vector_double( training_set_hdcg_accuracies_complete ) << endl;
  outfile << "test_set_hdcg_accuracies_complete" << export_vector_double( test_set_hdcg_accuracies_complete ) << endl << endl;
  //add extracted HADCG results to the output matlab file
  outfile << "\% HADCG training_set/test_set accuracy && runtime results" << endl;
  outfile << "\% later converted to matrix form" << endl;
  outfile << "training_set_hadcg_runtimes" << export_vector_double( training_set_hadcg_runtimes ) << endl;
  outfile << "test_set_hadcg_runtimes" << export_vector_double( test_set_hadcg_runtimes ) << endl;
  outfile << "training_set_hadcg_accuracies_root" << export_vector_double( training_set_hadcg_accuracies_root ) << endl;
  outfile << "test_set_hadcg_accuracies_root" << export_vector_double( test_set_hadcg_accuracies_root ) << endl;
  outfile << "training_set_hadcg_accuracies_complete" << export_vector_double( training_set_hadcg_accuracies_complete ) << endl;
  outfile << "test_set_hadcg_accuracies_complete" << export_vector_double( test_set_hadcg_accuracies_complete ) << endl << endl;


  //matlab code to generate the training_set indices
  outfile << "\% extract each unique training_set size" << endl;
  outfile << "training_indices = [];" << endl;
  outfile << "for i=1:size(training_set_sizes,1)" << endl;
  outfile << "  if(isempty(find(training_indices == training_set_sizes(i))))" << endl;
  outfile << "    training_indices = [ training_indices; training_set_sizes(i) ];" << endl;
  outfile << "  end" << endl;
  outfile << "end" << endl << endl;

  //matlab code to generate a matrix of training_ratios
  outfile << "\% initialize matrices to be filled on a per-test basis" << endl;
  outfile << "\% each element is a result or property of a single cross validation test" << endl;
  outfile << "training_ratios_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;

  //matlab code to generate zeros-matrices for the extracted DCG results
  outfile << "training_set_dcg_accuracies_root_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;
  outfile << "training_set_dcg_accuracies_complete_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;
  outfile << "test_set_dcg_accuracies_root_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;
  outfile << "test_set_dcg_accuracies_root_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;
  //matlab code to generate zeros-matrices for the extracted ADCG results
  outfile << "training_set_adcg_accuracies_root_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;
  outfile << "training_set_adcg_accuracies_complete_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;
  outfile << "test_set_adcg_accuracies_complete_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;
  outfile << "test_set_adcg_accuracies_complete_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;
  //matlab code to generate zeros-matrices for the extracted HDCG results
  outfile << "training_set_hdcg_accuracies_root_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;
  outfile << "training_set_hdcg_accuracies_complete_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;
  outfile << "test_set_hdcg_accuracies_complete_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;
  outfile << "test_set_hdcg_accuracies_complete_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;
  //matlab code to generate zeros-matrices for the extracted HADCG results
  outfile << "training_set_hadcg_accuracies_root_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;
  outfile << "training_set_hadcg_accuracies_complete_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;
  outfile << "test_set_hadcg_accuracies_complete_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;
  outfile << "test_set_hadcg_accuracies_complete_matrix = zeros(size(training_indices,1),size(training_ratios,1)/size(training_indices,1));" << endl;

  //matlab code to fill in each of the zeros-matrices for each model
  outfile << "\% fill each cell of each matrix with the appropriate value" << endl;
  outfile << "\% once finished, the filled matrices can be used for plotting" << endl;
  outfile << "indices = ones([size(training_indices,1),1]);" << endl;
  outfile << "for i=1:size(training_set_sizes,1)" << endl;
  outfile << "  for j=1:size(training_indices,1)" << endl;
  outfile << "    if(training_set_sizes(i) == training_indices(j))" << endl;
  outfile << "      training_ratios_matrix(j,indices(j)) = training_ratios(i);" << endl;
  //fill in the DCG model extracted results
  outfile << "      training_set_dcg_accuracies_root_matrix(j,indices(j)) = training_set_dcg_accuracies_root(i);" << endl;
  outfile << "      training_set_dcg_accuracies_complete_matrix(j,indices(j)) = training_set_dcg_accuracies_complete(i);" << endl;
  outfile << "      test_set_dcg_accuracies_root_matrix(j,indices(j)) = test_set_dcg_accuracies_root(i);" << endl;
  outfile << "      test_set_dcg_accuracies_complete_matrix(j,indices(j)) = test_set_dcg_accuracies_complete(i);" << endl;
  //fill in the ADCG model extracted results
  outfile << "      training_set_adcg_accuracies_root_matrix(j,indices(j)) = training_set_adcg_accuracies_root(i);" << endl;
  outfile << "      training_set_adcg_accuracies_complete_matrix(j,indices(j)) = training_set_adcg_accuracies_complete(i);" << endl;
  outfile << "      test_set_adcg_accuracies_root_matrix(j,indices(j)) = test_set_adcg_accuracies_root(i);" << endl;
  outfile << "      test_set_adcg_accuracies_complete_matrix(j,indices(j)) = test_set_adcg_accuracies_complete(i);" << endl;
  //fill in the HDCG model extracted results
  outfile << "      training_set_hdcg_accuracies_root_matrix(j,indices(j)) = training_set_hdcg_accuracies_root(i);" << endl;
  outfile << "      training_set_hdcg_accuracies_complete_matrix(j,indices(j)) = training_set_hdcg_accuracies_complete(i);" << endl;
  outfile << "      test_set_hdcg_accuracies_root_matrix(j,indices(j)) = test_set_hdcg_accuracies_root(i);" << endl;
  outfile << "      test_set_hdcg_accuracies_complete_matrix(j,indices(j)) = test_set_hdcg_accuracies_complete(i);" << endl;
  //fill in the HADCG model extracted results
  outfile << "      training_set_hadcg_accuracies_root_matrix(j,indices(j)) = training_set_hadcg_accuracies_root(i);" << endl;
  outfile << "      training_set_hadcg_accuracies_complete_matrix(j,indices(j)) = training_set_hadcg_accuracies_complete(i);" << endl;
  outfile << "      test_set_hadcg_accuracies_root_matrix(j,indices(j)) = test_set_hadcg_accuracies_root(i);" << endl;
  outfile << "      test_set_hadcg_accuracies_complete_matrix(j,indices(j)) = test_set_hadcg_accuracies_complete(i);" << endl;
  outfile << "      indices(j) = indices(j) + 1;" << endl;
  outfile << "    end" << endl;
  outfile << "  end" << endl;
  outfile << "end" << endl << endl;

  //matlab code to find the training_set and test_set mean accuracy (root && complete) for each model
  outfile << "\% find the mean accuracy (root && complete) for each model" << endl;
  outfile << "training_ratios_matrix_means = mean(training_ratios_matrix,2);" << endl;
  //mean accuracies for the DCG model extracted results
  outfile << "training_set_dcg_accuracies_root_matrix_means = mean(training_set_dcg_accuracies_root_matrix,2);" << endl;
  outfile << "training_set_dcg_accuracies_complete_matrix_means = mean(training_set_dcg_accuracies_complete_matrix,2);" << endl;
  outfile << "test_set_dcg_accuracies_root_matrix_means = mean(test_set_dcg_accuracies_root_matrix,2);" << endl;
  outfile << "test_set_dcg_accuracies_complete_matrix_means = mean(test_set_dcg_accuracies_complete_matrix,2);" << endl;
  //mean accuracies for the ADCG model extracted results
  outfile << "training_set_adcg_accuracies_root_matrix_means = mean(training_set_adcg_accuracies_root_matrix,2);" << endl;
  outfile << "training_set_adcg_accuracies_complete_matrix_means = mean(training_set_adcg_accuracies_complete_matrix,2);" << endl;
  outfile << "test_set_adcg_accuracies_root_matrix_means = mean(test_set_adcg_accuracies_root_matrix,2);" << endl;
  outfile << "test_set_adcg_accuracies_complete_matrix_means = mean(test_set_adcg_accuracies_complete_matrix,2);" << endl << endl;
  //mean accuracies for the HDCG model extracted results
  outfile << "training_set_hdcg_accuracies_root_matrix_means = mean(training_set_hdcg_accuracies_root_matrix,2);" << endl;
  outfile << "training_set_hdcg_accuracies_complete_matrix_means = mean(training_set_hdcg_accuracies_complete_matrix,2);" << endl;
  outfile << "test_set_hdcg_accuracies_root_matrix_means = mean(test_set_hdcg_accuracies_root_matrix,2);" << endl;
  outfile << "test_set_hdcg_accuracies_complete_matrix_means = mean(test_set_hdcg_accuracies_complete_matrix,2);" << endl << endl;
  //mean accuracies for the HADCG model extracted results
  outfile << "training_set_hadcg_accuracies_root_matrix_means = mean(training_set_hadcg_accuracies_root_matrix,2);" << endl;
  outfile << "training_set_hadcg_accuracies_complete_matrix_means = mean(training_set_hadcg_accuracies_complete_matrix,2);" << endl;
  outfile << "test_set_hadcg_accuracies_root_matrix_means = mean(test_set_hadcg_accuracies_root_matrix,2);" << endl;
  outfile << "test_set_hadcg_accuracies_complete_matrix_means = mean(test_set_hadcg_accuracies_complete_matrix,2);" << endl << endl;

  //matlab code to find the mean and standard-deviation for the extracted runtime results
  //mean && standard-deviation for the DCG extracted runtime results
  outfile << "\% find the mean and standard deviation for the runtimes for each model" << endl;
  outfile << "total_dcg_runtime = [ training_set_dcg_runtimes; test_set_dcg_runtimes ];" << endl;
  outfile << "mean_dcg_runtime = mean( total_dcg_runtime );" << endl;
  outfile << "std_dcg_runtime = std( total_dcg_runtime );" << endl << endl;
  //mean && standard-deviation for the ADCG extracted runtime results
  outfile << "total_adcg_runtime = [ training_set_adcg_runtimes; test_set_adcg_runtimes ];" << endl;
  outfile << "mean_adcg_runtime = mean( total_adcg_runtime );" << endl;
  outfile << "std_adcg_runtime = std( total_adcg_runtime );" << endl << endl;
  //mean && standard-deviation for the HDCG extracted runtime results
  outfile << "total_hdcg_runtime = [ training_set_hdcg_runtimes; test_set_hdcg_runtimes ];" << endl;
  outfile << "mean_hdcg_runtime = mean( total_hdcg_runtime );" << endl;
  outfile << "std_hdcg_runtime = std( total_hdcg_runtime );" << endl << endl;
  //mean && standard-deviation for the HADCG extracted runtime results
  outfile << "total_hadcg_runtime = [ training_set_hadcg_runtimes; test_set_hadcg_runtimes ];" << endl;
  outfile << "mean_hadcg_runtime = mean( total_hadcg_runtime );" << endl;
  outfile << "std_hadcg_runtime = std( total_hadcg_runtime );" << endl << endl;

  //combine the means and the standard-deviations for plotting
  outfile << "\% combine the runtime statistics to allow for plotting" << endl;
  outfile << "means = [ mean_dcg_runtime, mean_adcg_runtime, mean_hdcg_runtime, mean_hadcg_runtime ];" << endl;
  outfile << "stds = [ std_dcg_runtime, std_adcg_runtime, std_hdcg_runtime, std_hadcg_runtime ];" << endl << endl;
  outfile << "close all;" << endl << endl; 

  //MATLAB code to plot various results
  //plot the DCG training_set accuracy (root && complete)
  outfile << "\% plot the DCG training_set accuracies" << endl;
  outfile << "figure(1);" << endl;
  outfile << "hold on;" << endl;
  outfile << "box on;" << endl;
  outfile << "grid on;" << endl;
  outfile << "ylim( [0.0 1.0] );" << endl;
  outfile << "xlim( [0.1 0.9] );" << endl;
  outfile << "scatter(training_ratios,training_set_dcg_accuracies_root,200);" << endl;
  outfile << "scatter(training_ratios,training_set_dcg_accuracies_complete,200,'+');" << endl;
  outfile << "plot(training_ratios_matrix_means,training_set_dcg_accuracies_root_matrix_means,training_ratios_matrix_means,training_set_dcg_accuracies_complete_matrix_means);" << endl;
  outfile << "legend('DCG root','DCG complete','Location','NorthWest');" << endl;
  outfile << "title('DCG Training Set Accuracy (root & complete) vs Holdout');" << endl;
  outfile << "xlhand = get(gca,'xlabel');" << endl;
  outfile << "set(xlhand,'string','Training Fraction','fontsize',15);" << endl;
  outfile << "ylhand = get(gca,'ylabel');" << endl;
  outfile << "set(ylhand,'string','Training Set Accuracy','fontsize',15);" << endl;
  outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
  outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
  outfile << "saveas(gcf, 'dcg-training-accuracy-root-complete', 'epsc');" << endl;
  outfile << "saveas(gcf, 'dcg-training-accuracy-root-complete', 'pdf');" << endl << endl;

  //plot the DCG test_set accuracy (root && complete)
  outfile << "\% plot the DCG test_set accuracies" << endl;
  outfile << "figure(2);" << endl;
  outfile << "hold on;" << endl;
  outfile << "box on;" << endl;
  outfile << "grid on;" << endl;
  outfile << "ylim( [0.0 1.0] );" << endl;
  outfile << "xlim( [0.1 0.9] );" << endl;
  outfile << "scatter(training_ratios,test_set_dcg_accuracies_root,200);" << endl;
  outfile << "scatter(training_ratios,test_set_dcg_accuracies_complete,200,'+');" << endl;
  outfile << "plot(training_ratios_matrix_means,test_set_dcg_accuracies_root_matrix_means,training_ratios_matrix_means,test_set_dcg_accuracies_complete_matrix_means);" << endl;
  outfile << "legend('DCG root','DCG complete','Location','NorthWest');" << endl;
  outfile << "title('DCG Test Set Accuracy (root & complete) vs Holdout');" << endl;
  outfile << "xlhand = get(gca,'xlabel');" << endl;
  outfile << "set(xlhand,'string','Training Fraction','fontsize',15);" << endl;
  outfile << "ylhand = get(gca,'ylabel');" << endl;
  outfile << "set(ylhand,'string','Test Set Accuracy','fontsize',15);" << endl;
  outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
  outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
  outfile << "saveas(gcf, 'dcg-test-accuracy-root-complete', 'epsc');" << endl;
  outfile << "saveas(gcf, 'dcg-test-accuracy-root-complete', 'pdf');" << endl << endl;

  //plot the ADCG training_set accuracy (root && complete)
  outfile << "\% plot the ADCG training_set accuracies" << endl;
  outfile << "figure(3);" << endl;
  outfile << "hold on;" << endl;
  outfile << "box on;" << endl;
  outfile << "grid on;" << endl;
  outfile << "ylim( [0.0 1.0] );" << endl;
  outfile << "xlim( [0.1 0.9] );" << endl;
  outfile << "scatter(training_ratios,training_set_adcg_accuracies_root,200);" << endl;
  outfile << "scatter(training_ratios,training_set_adcg_accuracies_complete,200,'+');" << endl;
  outfile << "plot(training_ratios_matrix_means,training_set_adcg_accuracies_root_matrix_means,training_ratios_matrix_means,training_set_adcg_accuracies_complete_matrix_means);" << endl;
  outfile << "legend('ADCG root','ADCG complete','Location','NorthWest');" << endl;
  outfile << "title('ADCG Training Set Accuracy (root & complete) vs Holdout');" << endl;
  outfile << "xlhand = get(gca,'xlabel');" << endl;
  outfile << "set(xlhand,'string','Training Fraction','fontsize',15);" << endl;
  outfile << "ylhand = get(gca,'ylabel');" << endl;
  outfile << "set(ylhand,'string','Training Set Accuracy','fontsize',15);" << endl;
  outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
  outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
  outfile << "saveas(gcf, 'adcg-training-accuracy-root-complete', 'epsc');" << endl;
  outfile << "saveas(gcf, 'adcg-training-accuracy-root-complete', 'pdf');" << endl << endl;

  //plot the ADCG test_set accuracy (root && complete)
  outfile << "\% plot the ADCG test_set accuracies" << endl;
  outfile << "figure(4);" << endl;
  outfile << "hold on;" << endl;
  outfile << "box on;" << endl;
  outfile << "grid on;" << endl;
  outfile << "ylim( [0.0 1.0] );" << endl;
  outfile << "xlim( [0.1 0.9] );" << endl;
  outfile << "scatter(training_ratios,test_set_adcg_accuracies_root,200);" << endl;
  outfile << "scatter(training_ratios,test_set_adcg_accuracies_complete,200,'+');" << endl;
  outfile << "plot(training_ratios_matrix_means,test_set_adcg_accuracies_root_matrix_means,training_ratios_matrix_means,test_set_adcg_accuracies_complete_matrix_means);" << endl;
  outfile << "legend('ADCG root','ADCG complete','Location','NorthWest');" << endl;
  outfile << "title('ADCG Test Set Accuracy (root & complete) vs Holdout');" << endl;
  outfile << "xlhand = get(gca,'xlabel');" << endl;
  outfile << "set(xlhand,'string','Training Fraction','fontsize',15);" << endl;
  outfile << "ylhand = get(gca,'ylabel');" << endl;
  outfile << "set(ylhand,'string','Test Set Accuracy','fontsize',15);" << endl;
  outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
  outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
  outfile << "saveas(gcf, 'adcg-test-accuracy-root-complete', 'epsc');" << endl;
  outfile << "saveas(gcf, 'adcg-test-accuracy-root-complete', 'pdf');" << endl << endl;


  //plot the HDCG training_set accuracy (root && complete)
  outfile << "\% plot the HDCG training_set accuracies" << endl;
  outfile << "figure(5);" << endl;
  outfile << "hold on;" << endl;
  outfile << "box on;" << endl;
  outfile << "grid on;" << endl;
  outfile << "ylim( [0.0 1.0] );" << endl;
  outfile << "xlim( [0.1 0.9] );" << endl;
  outfile << "scatter(training_ratios,training_set_hdcg_accuracies_root,200);" << endl;
  outfile << "scatter(training_ratios,training_set_hdcg_accuracies_complete,200,'+');" << endl;
  outfile << "plot(training_ratios_matrix_means,training_set_hdcg_accuracies_root_matrix_means,training_ratios_matrix_means,training_set_hdcg_accuracies_complete_matrix_means);" << endl;
  outfile << "legend('HDCG root','HDCG complete','Location','NorthWest');" << endl;
  outfile << "title('HDCG Training Set Accuracy (root & complete) vs Holdout');" << endl;
  outfile << "xlhand = get(gca,'xlabel');" << endl;
  outfile << "set(xlhand,'string','Training Fraction','fontsize',15);" << endl;
  outfile << "ylhand = get(gca,'ylabel');" << endl;
  outfile << "set(ylhand,'string','Training Set Accuracy','fontsize',15);" << endl;
  outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
  outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
  outfile << "saveas(gcf, 'hdcg-training-accuracy-root-complete', 'epsc');" << endl;
  outfile << "saveas(gcf, 'hdcg-training-accuracy-root-complete', 'pdf');" << endl << endl;

  //plot the HDCG test_set accuracy (root && complete)
  outfile << "\% plot the HDCG test_set accuracies" << endl;
  outfile << "figure(6);" << endl;
  outfile << "hold on;" << endl;
  outfile << "box on;" << endl;
  outfile << "grid on;" << endl;
  outfile << "ylim( [0.0 1.0] );" << endl;
  outfile << "xlim( [0.1 0.9] );" << endl;
  outfile << "scatter(training_ratios,test_set_hdcg_accuracies_root,200);" << endl;
  outfile << "scatter(training_ratios,test_set_hdcg_accuracies_complete,200,'+');" << endl;
  outfile << "plot(training_ratios_matrix_means,test_set_hdcg_accuracies_root_matrix_means,training_ratios_matrix_means,test_set_hdcg_accuracies_complete_matrix_means);" << endl;
  outfile << "legend('HDCG root','HDCG complete','Location','NorthWest');" << endl;
  outfile << "title('HDCG Test Set Accuracy (root & complete) vs Holdout');" << endl;
  outfile << "xlhand = get(gca,'xlabel');" << endl;
  outfile << "set(xlhand,'string','Training Fraction','fontsize',15);" << endl;
  outfile << "ylhand = get(gca,'ylabel');" << endl;
  outfile << "set(ylhand,'string','Test Set Accuracy','fontsize',15);" << endl;
  outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
  outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
  outfile << "saveas(gcf, 'hdcg-test-accuracy-root-complete', 'epsc');" << endl;
  outfile << "saveas(gcf, 'hdcg-test-accuracy-root-complete', 'pdf');" << endl << endl;


  //plot the HADCG training_set accuracy (root && complete)
  outfile << "\% plot the HADCG training_set accuracies" << endl;
  outfile << "figure(7);" << endl;
  outfile << "hold on;" << endl;
  outfile << "box on;" << endl;
  outfile << "grid on;" << endl;
  outfile << "ylim( [0.0 1.0] );" << endl;
  outfile << "xlim( [0.1 0.9] );" << endl;
  outfile << "scatter(training_ratios,training_set_hadcg_accuracies_root,200);" << endl;
  outfile << "scatter(training_ratios,training_set_hadcg_accuracies_complete,200,'+');" << endl;
  outfile << "plot(training_ratios_matrix_means,training_set_hadcg_accuracies_root_matrix_means,training_ratios_matrix_means,training_set_hadcg_accuracies_complete_matrix_means);" << endl;
  outfile << "legend('HADCG root','HADCG complete','Location','NorthWest');" << endl;
  outfile << "title('HADCG Training Set Accuracy (root & complete) vs Holdout');" << endl;
  outfile << "xlhand = get(gca,'xlabel');" << endl;
  outfile << "set(xlhand,'string','Training Fraction','fontsize',15);" << endl;
  outfile << "ylhand = get(gca,'ylabel');" << endl;
  outfile << "set(ylhand,'string','Training Set Accuracy','fontsize',15);" << endl;
  outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
  outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
  outfile << "saveas(gcf, 'hadcg-training-accuracy-root-complete', 'epsc');" << endl;
  outfile << "saveas(gcf, 'hadcg-training-accuracy-root-complete', 'pdf');" << endl << endl;

  //plot the HADCG test_set accuracy (root && complete)
  outfile << "\% plot the HADCG test_set accuracies" << endl;
  outfile << "figure(8);" << endl;
  outfile << "hold on;" << endl;
  outfile << "box on;" << endl;
  outfile << "grid on;" << endl;
  outfile << "ylim( [0.0 1.0] );" << endl;
  outfile << "xlim( [0.1 0.9] );" << endl;
  outfile << "scatter(training_ratios,test_set_hadcg_accuracies_root,200);" << endl;
  outfile << "scatter(training_ratios,test_set_hadcg_accuracies_complete,200,'+');" << endl;
  outfile << "plot(training_ratios_matrix_means,test_set_hadcg_accuracies_root_matrix_means,training_ratios_matrix_means,test_set_hadcg_accuracies_complete_matrix_means);" << endl;
  outfile << "legend('HADCG root','HADCG complete','Location','NorthWest');" << endl;
  outfile << "title('HADCG Test Set Accuracy (root & complete) vs Holdout');" << endl;
  outfile << "xlhand = get(gca,'xlabel');" << endl;
  outfile << "set(xlhand,'string','Training Fraction','fontsize',15);" << endl;
  outfile << "ylhand = get(gca,'ylabel');" << endl;
  outfile << "set(ylhand,'string','Test Set Accuracy','fontsize',15);" << endl;
  outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
  outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
  outfile << "saveas(gcf, 'hadcg-test-accuracy-root-complete', 'epsc');" << endl;
  outfile << "saveas(gcf, 'hadcg-test-accuracy-root-complete', 'pdf');" << endl << endl;


  //matlab code to plot the DCG, ADCG, HDCG, and HADCG average test_set root-accuracies
  outfile << "\% plot all of the models' average test_set root accuracies" << endl;
  outfile << "figure(9);" << endl;
  outfile << "hold on;" << endl;
  outfile << "box on;" << endl;
  outfile << "grid on;" << endl;
  outfile << "ylim( [0.0 1.0] );" << endl;
  outfile << "xlim( [0.1 0.9] );" << endl;
  outfile << "plot(training_ratios_matrix_means,test_set_dcg_accuracies_root_matrix_means, 'b');" << endl;
  outfile << "plot(training_ratios_matrix_means,test_set_adcg_accuracies_root_matrix_means, 'r');" << endl;
  outfile << "plot(training_ratios_matrix_means,test_set_hdcg_accuracies_root_matrix_means, 'g');" << endl;
  outfile << "plot(training_ratios_matrix_means,test_set_hadcg_accuracies_root_matrix_means, 'y');" << endl;
  outfile << "legend('DCG Root','ADCG Root','HDCG Root','HADCG Root','Location','NorthWest');" << endl;
  outfile << "title('Mean Test Set Accuracy (root) vs Holdout');" << endl;
  outfile << "xlhand = get(gca,'xlabel');" << endl;
  outfile << "set(xlhand,'string','Training Fraction','fontsize',15);" << endl;
  outfile << "ylhand = get(gca,'ylabel');" << endl;
  outfile << "set(ylhand,'string','Test Set Accuracy','fontsize',15);" << endl;
  outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
  outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
  outfile << "saveas(gcf, 'model-avg-test-accuracy-root', 'epsc');" << endl;
  outfile << "saveas(gcf, 'model-avg-test-accuracy-root', 'pdf');" << endl << endl;

  //matlab code to plot the DCG, ADCG, HDCG, and HADCG average training_set root-accuracies
  outfile << "\% plot all of the models' average training_set root accuracies" << endl;
  outfile << "figure(10);" << endl;
  outfile << "hold on;" << endl;
  outfile << "box on;" << endl;
  outfile << "grid on;" << endl;
  outfile << "ylim( [0.0 1.0] );" << endl;
  outfile << "xlim( [0.1 0.9] );" << endl;
  outfile << "plot(training_ratios_matrix_means,training_set_dcg_accuracies_root_matrix_means, 'b');" << endl;
  outfile << "plot(training_ratios_matrix_means,training_set_adcg_accuracies_root_matrix_means, 'r');" << endl;
  outfile << "plot(training_ratios_matrix_means,training_set_hdcg_accuracies_root_matrix_means, 'g');" << endl;
  outfile << "plot(training_ratios_matrix_means,training_set_hadcg_accuracies_root_matrix_means, 'y');" << endl;
  outfile << "legend('DCG Root','ADCG Root','HDCG Root','HADCG Root','Location','NorthWest');" << endl;
  outfile << "title('Mean Training Set Accuracy (root) vs Holdout');" << endl;
  outfile << "xlhand = get(gca,'xlabel');" << endl;
  outfile << "set(xlhand,'string','Training Fraction','fontsize',15);" << endl;
  outfile << "ylhand = get(gca,'ylabel');" << endl;
  outfile << "set(ylhand,'string','Training Set Accuracy','fontsize',15);" << endl;
  outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
  outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
  outfile << "saveas(gcf, 'model-avg-training-accuracy-root', 'epsc');" << endl;
  outfile << "saveas(gcf, 'model-avg-training-accuracy-root', 'pdf');" << endl << endl;

  //matlab code to plot the DCG, ADCG, HDCG, and HADCG average test_set complete-accuracies
  outfile << "\% plot all of the models' average test_set complete accuracies" << endl;
  outfile << "figure(11);" << endl;
  outfile << "hold on;" << endl;
  outfile << "box on;" << endl;
  outfile << "grid on;" << endl;
  outfile << "ylim( [0.0 1.0] );" << endl;
  outfile << "xlim( [0.1 0.9] );" << endl;
  outfile << "plot(training_ratios_matrix_means,test_set_dcg_accuracies_complete_matrix_means, 'b');" << endl;
  outfile << "plot(training_ratios_matrix_means,test_set_adcg_accuracies_complete_matrix_means, 'r');" << endl;
  outfile << "plot(training_ratios_matrix_means,test_set_hdcg_accuracies_complete_matrix_means, 'g');" << endl;
  outfile << "plot(training_ratios_matrix_means,test_set_hadcg_accuracies_complete_matrix_means, 'y');" << endl;
  outfile << "legend('DCG Complete','ADCG Complete','HDCG Complete','HADCG Complete','Location','NorthWest');" << endl;
  outfile << "title('Mean Test Set Accuracy (complete) vs Holdout');" << endl;
  outfile << "xlhand = get(gca,'xlabel');" << endl;
  outfile << "set(xlhand,'string','Training Fraction','fontsize',15);" << endl;
  outfile << "ylhand = get(gca,'ylabel');" << endl;
  outfile << "set(ylhand,'string','Test Set Accuracy','fontsize',15);" << endl;
  outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
  outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
  outfile << "saveas(gcf, 'model-avg-test-accuracy-complete', 'epsc');" << endl;
  outfile << "saveas(gcf, 'model-avg-test-accuracy-complete', 'pdf');" << endl << endl;

  //matlab code to plot the DCG, ADCG, HDCG, and HADCG average training_set complete-accuracies
  outfile << "\% plot all of the models' average training_set complete accuracies" << endl;
  outfile << "figure(12);" << endl;
  outfile << "hold on;" << endl;
  outfile << "box on;" << endl;
  outfile << "grid on;" << endl;
  outfile << "ylim( [0.0 1.0] );" << endl;
  outfile << "xlim( [0.1 0.9] );" << endl;
  outfile << "plot(training_ratios_matrix_means,training_set_dcg_accuracies_complete_matrix_means, 'b');" << endl;
  outfile << "plot(training_ratios_matrix_means,training_set_adcg_accuracies_complete_matrix_means, 'r');" << endl;
  outfile << "plot(training_ratios_matrix_means,training_set_hdcg_accuracies_complete_matrix_means, 'g');" << endl;
  outfile << "plot(training_ratios_matrix_means,training_set_hadcg_accuracies_complete_matrix_means, 'y');" << endl;
  outfile << "legend('DCG Complete','ADCG Complete','HDCG Complete','HADCG Complete','Location','NorthWest');" << endl;
  outfile << "title('Mean Training Set Accuracy (complete) vs Holdout');" << endl;
  outfile << "xlhand = get(gca,'xlabel');" << endl;
  outfile << "set(xlhand,'string','Training Fraction','fontsize',15);" << endl;
  outfile << "ylhand = get(gca,'ylabel');" << endl;
  outfile << "set(ylhand,'string','Training Set Accuracy','fontsize',15);" << endl;
  outfile << "set(gcf, 'PaperPosition', [0 0 6 6]);" << endl;
  outfile << "set(gcf, 'PaperSize', [6 6]);" << endl;
  outfile << "saveas(gcf, 'model-avg-training-accuracy-complete', 'epsc');" << endl;
  outfile << "saveas(gcf, 'model-avg-training-accuracy-complete', 'pdf');" << endl << endl;


  outfile.close();

  cout << "end of test_convert_xml_to_mat program" << endl;
  return 0;
} 
