#!/bin/bash

# ADCG: Case I: Full corpus one file. Note: folder index 2 has the single file generated. 
../build/bin/h2sl-test-process-inference --model adcg --feature_set ../data/training/navigation_corpus/combined/feature_set_groundings_combined.xml --output ../data/results/results-adcg-single/ --solution_directory ../data/results/results-adcg-single/solutions/ ../data/cross_validation_partitions/combined_nav_corpora/collated_0002/test_collated_full.xml --beam_width 4 --test_training_set 1 --test_test_set 0

# Search Space Table
../build/bin/h2sl-search-space-corpus-analysis ../data/results/results-dcg-single/results/result_full.xml ../data/results/results-adcg-single/result_full.xml
 
../build/bin/h2sl-search-space-corpus-analysis ../data/results/results-dcg-single/results/result_full.xml ../data/results/results-adcg-single/result_full.xml --output_script evaluation_search_space.m --output_text evaluation_search_space.txt


