#!/bin/bash          

echo Search Space Test 

#../build/bin/h2sl-test-process-inference --model dcg --feature_set ../data/training/navigation_corpus/combined/feature_set_groundings_combined.xml --output ../data/results/results_0003/ --solution_directory ../data/results/results_0003/solutions/ ../data/cross_validation_partitions/combined_nav_corpora/collated_0000/test_collated_0130.xml --beam_width 1


#../build/bin/h2sl-test-process-inference --model adcg --feature_set ../data/training/navigation_corpus/combined/feature_set_groundings_combined.xml --output ../data/results/results/ --solution_directory ../data/results/results/solutions/ ../data/cross_validation_partitions/combined_nav_corpora/collated_0000/test_collated_0130.xml --beam_width 1


../build/bin/h2sl-test-process-inference --model adcg --feature_set ../data/training/navigation_corpus/combined/feature_set_groundings_combined.xml --output ../data/results/results/ --solution_directory ../data/results/results/solutions/ ../data/cross_validation_partitions/combined_nav_corpora/collated_0000/test_full.xml --beam_width 1
