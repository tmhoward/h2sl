#!/bin/bash          

#mkdir ../data/results

#mkdir ../data/results/results_0000

#mkdir ../data/results/results_0000/solutions

#mkdir -p ../data/cross_validation_partitions/combined_nav_corpora/collated_0000

../build/bin/h2sl-test-process-inference --model adcg --feature_set ../data/training/navigation_corpus/combined/feature_set_groundings_combined.xml --output ../data/results/results_0001/ --solution_directory ../data/results/results_0001/solutions/ ../data/cross_validation_partitions/combined_nav_corpora/collated_0000/test_collated_*.xml
