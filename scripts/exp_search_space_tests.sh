#!/bin/bash          

echo Search Space Test 

#../build/bin/h2sl-dcg-test ../data/training/navigation_corpus/objects_in_groups/example_0086.xml --symbol_dictionary_groundings ../data/training/navigation_corpus/combined/symbol_dictionary_groundings_combined.xml --llm ../data/training/navigation_corpus/llm.xml --output /tmp/search_space_dcg.xml --debug 1 --beam_width 1

# DCG: /dcg_solution_ followed be example number
#../build/bin/h2sl-dcg-test ../data/training/navigation_corpus/objects_in_groups/example_0086.xml --symbol_dictionary_groundings ../data/training/navigation_corpus/combined/symbol_dictionary_groundings_combined.xml --llm ../data/training/navigation_corpus/combined/llm_combined.xml --debug 1 --beam_width 1

# ADCG: Solution file written in /tmp/dcg_solution file number 
#../build/bin/h2sl-adcg-test ../data/training/navigation_corpus/objects_in_groups/example_0086.xml --symbol_dictionary_groundings ../data/training/navigation_corpus/combined/symbol_dictionary_groundings_combined.xml --llm ../data/training/navigation_corpus/combined/llm_combined.xml --debug 1 --beam_width 1

# Run on the full data set.
../build/bin/h2sl-test-process-inference --model adcg --feature_set ../data/training/navigation_corpus/combined/feature_set_groundings_combined.xml --output ../data/results/results_0000/ --solution_directory ../data/results/results_0000/solutions/ ../data/cross_validation_partitions/combined_nav_corpora/collated_0000/test_collated_0130.xml



