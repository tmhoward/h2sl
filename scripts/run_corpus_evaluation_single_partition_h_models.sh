#!/bin/bash

# =============================  partition ========================================
# Random partition generation

# =============================  training ========================================
# Training of the training partition and creation of LLM and insertion of the symbol dictionary
# Case I: Full partition single file. Picks the partition from single_partition and puts the trained llm and features used in collated_single_partition_trained
../build/bin/h2sl-test-process-training ../data/cross_validation_partitions/combined_nav_corpora/single_partition/test_full.xml --symbol_dictionary_groundings ../data/training/navigation_corpus/combined/sdg_hdcg_nav.xml --feature_set ../data/training/navigation_corpus/combined/fs_hdcg_nav.xml --output ../data/cross_validation_partitions/combined_nav_corpora/collated_single_partition_trained/ --evaluate_model 1 --threads 16

# =============================  inference  ========================================
# HDCG: Case I: Full corpus one file. 
mkdir -p ../data/results/results_hdcg_single/results_beam_width_1/solutions

../build/bin/h2sl-test-process-inference --model hdcg --feature_set ../data/training/navigation_corpus/combined/fs_hdcg_nav.xml --output ../data/results/results_hdcg_single/results_beam_width_1/ --solution_directory ../data/results/results_hdcg_single/results_beam_width_1/solutions/ ../data/cross_validation_partitions/combined_nav_corpora/collated_single_partition/test_collated_full.xml --beam_width 1 --test_training_set 1 --test_test_set 0

# HADCG: Case I: Full corpus one file. Beam 1.
mkdir -p ../data/results/results_hadcg_single/results_beam_width_1/solutions

../build/bin/h2sl-test-process-inference --model hadcg --feature_set ../data/training/navigation_corpus/combined/fs_hdcg_nav.xml --output ../data/results/results_hadcg_single/results_beam_width_1/ --solution_directory ../data/results/results_hadcg_single/results_beam_width_1/solutions/ ../data/cross_validation_partitions/combined_nav_corpora/collated_single_partition/test_collated_full.xml --beam_width 1 --test_training_set 1 --test_test_set 0

# HADCG: Case I: Full corpus one file. Beam 4.
mkdir -p ../data/results/results_hadcg_single/results_beam_width_4/solutions

../build/bin/h2sl-test-process-inference --model hadcg --feature_set ../data/training/navigation_corpus/combined/fs_hdcg_nav.xml --output ../data/results/results_hadcg_single/results_beam_width_4/ --solution_directory ../data/results/results_hadcg_single/results_beam_width_4/solutions/ ../data/cross_validation_partitions/combined_nav_corpora/collated_single_partition/test_collated_full.xml --beam_width 4 --test_training_set 1 --test_test_set 0

# ============================= combining results  =======================================
mkdir -p ../data/results/results_hadcg_single/results_beam_width_1/solutions

../build//bin/h2sl-test-combine-model-results --source_model hdcg --source ../data/results/results_hdcg_single/results_beam_width_1/result_full.xml --target ../data/results/results_hadcg_single/results_beam_width_1/result_full.xml --output ../data/results/results_hdcg_hadcg_single/results_beam_width_1/result_hdcg_hadcg_single.xml

mkdir -p ../data/results/results_hadcg_single/results_beam_width_4/solutions

../build//bin/h2sl-test-combine-model-results --source_model hdcg --source ../data/results/results_hdcg_single/results_beam_width_1/result_full.xml --target ../data/results/results_hadcg_single/results_beam_width_4/result_full.xml --output ../data/results/results_hdcg_hadcg_single/results_beam_width_4/result_hdcg_hadcg_single.xml

# ============================  Timing data for table ==================================================
../build/bin/h2sl-scrape-results-runtime-normalized-vs-world-size --input ../data/results/results_hdcg_hadcg_single/results_beam_width_1/result_hdcg_hadcg_single.xml > normalized_timing_comparison.txt

# ============================  Search space data for table and scatter plot ==================================================
# Search Space Table
../build/bin/h2sl-search-space-corpus-analysis ../data/results/results_hdcg_single/results_beam_width_1/result_full.xml ../data/results/results_hadcg_single/results_beam_width_4/result_full.xml --output_script evaluation_search_space_hdcg_hadcg.m --output_text evaluation_search_space_hdcg_hadcg.txt
