#!/bin/bash

# =============================  PARTITION ========================================
# Random partition generation
# Case II: Generate cross validation data partitions using the default settings
mkdir -p ../data/cross_validation_partitions/combined_nav_corpora/all_partitions
../build/bin/h2sl-test-generator --output ../data/cross_validation_partitions/combined_nav_corpora/all_partitions/ ../data/training/navigation_corpus/groups/example_00*.xml ../data/training/navigation_corpus/objects_in_groups/example_00*

# =============================  TRAINING ========================================
# Training of the training partition and creation of LLM and insertion of the symbol dictionary
# Case II: Corpus. Multiple files.
mkdir -p ../data/cross_validation_partitions/combined_nav_corpora/collated_all_partitions

../build/bin/h2sl-test-process-training ../data/cross_validation_partitions/combined_nav_corpora/all_partitions/test_*.xml --symbol_dictionary_groundings ../data/training/navigation_corpus/combined/sdg_hdcg_nav.xml --feature_set ../data/training/navigation_corpus/combined/fs_hdcg_nav.xml --output ../data/cross_validation_partitions/combined_nav_corpora/collated_all_partitions/ --evaluate_model 0 --threads 16

# =============================  INFERENCE ========================================

mkdir -p ../data/results/results_hdcg_all/results_beam_width_1/solutions

../build/bin/h2sl-test-process-inference --model hdcg --feature_set ../data/training/navigation_corpus/combined/fs_hdcg_nav.xml --output ../data/results/results_hdcg_all/results_beam_width_1/ --solution_directory ../data/results/results_hdcg_all/results_beam_width_1/solutions/ ../data/cross_validation_partitions/combined_nav_corpora/collated_all_partitions/test_*.xml --beam_width 1 --test_training_set 0 --test_test_set 1


mkdir -p ../data/results/results_hadcg_all/results_beam_width_1/solutions

../build/bin/h2sl-test-process-inference --model hadcg --feature_set ../data/training/navigation_corpus/combined/fs_hdcg_nav.xml --output ../data/results/results_hadcg_all/results_beam_width_1/ --solution_directory ../data/results/results_hadcg_all/results_beam_width_1/solutions/ ../data/cross_validation_partitions/combined_nav_corpora/collated_all_partitions/test_*.xml --beam_width 1 --test_training_set 0 --test_test_set 1

# =============================  COMBINING THE RESULTS =======================================

mkdir -p ../data/results/results_hdcg_hadcg_all/results_beam_width_1/solutions

../build//bin/h2sl-test-combine-model-results --source_model hdcg --source ../data/results/results_hdcg_full/results_beam_width_1/result_0129.xml --target ../data/results/results_hadcg_full/results_beam_width_1/result_0129.xml --output ../data/results/results_hdcg_hadcg_all/results_beam_width_1/result_hdcg_hadcg_all.xml

# =============================  MATLAB CONVERSION ===========================================
../build/bin/h2sl-test-convert-xml-to-mat --input ../data/results/results-hdcg-hadcg-all/result_hdcg_hadcg_all.xml --output ../data/results/results-hdcg-hadcg-all/result_hdcg_hadcg_all.m

