#!/bin/bash

# =============================  partition ========================================
# Random partition generation
# Case I: Training set is composed of all examples. Note that this command produces two files (test_0000.xml is 100% test set && test_0001.xml is 100% training_set)
../build/bin/h2sl-test-generator --num=1 --fr=2 --min_fr=0.0 --max_fr=1.0 ../data/training/navigation_corpus/groups/example_00*.xml ../data/training/navigation_corpus/objects_in_groups/example_00*.xml
mkdir -p ../data/cross_validation_partitions/combined_nav_corpora/collated_single_partition
mv /tmp/test_0001.xml ../data/cross_validation_partitions/combined_nav_corpora/collated_single_partition/test_full.xml

# =============================  training ========================================
# Training of the training partition and creation of LLM and insertion of the symbol dictionary
# Case I: Full partition single file. Picks the partition from single_partition and puts the trained llm and features used in collated_single_partition_trained
../build/bin/h2sl-test-process-training ../data/cross_validation_partitions/combined_nav_corpora/single_partition/test_full.xml --symbol_dictionary_groundings ../data/training/navigation_corpus/combined/sdg_hdcg_nav.xml --feature_set ../data/training/navigation_corpus/combined/fs_hdcg_nav.xml --output ../data/cross_validation_partitions/combined_nav_corpora/collated_single_partition/ --evaluate_model 1 --threads 16

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
mkdir -p ../data/results/results_hdcg_hadcg_single/results_beam_width_1/solutions

../build//bin/h2sl-test-combine-model-results --source_model hdcg --source ../data/results/results_hdcg_single/results_beam_width_1/result_full.xml --target ../data/results/results_hadcg_single/results_beam_width_1/result_full.xml --output ../data/results/results_hdcg_hadcg_single/results_beam_width_1/result_hdcg_hadcg_single.xml

mkdir -p ../data/results/results_hdcg_hadcg_single/results_beam_width_4/solutions

../build//bin/h2sl-test-combine-model-results --source_model hdcg --source ../data/results/results_hdcg_single/results_beam_width_1/result_full.xml --target ../data/results/results_hadcg_single/results_beam_width_4/result_full.xml --output ../data/results/results_hdcg_hadcg_single/results_beam_width_4/result_hdcg_hadcg_single.xml

# ============================  Timing data for table ==================================================
../build/bin/h2sl-scrape-results-runtime-normalized-vs-world-size --input ../data/results/results_hdcg_hadcg_single/results_beam_width_1/result_hdcg_hadcg_single.xml > normalized_timing_comparison.txt

# ============================  Search space data for table and scatter plot ==================================================
# Search Space Table
#../build/bin/h2sl-search-space-corpus-analysis ../data/results/results_hdcg_single/results_beam_width_1/result_full.xml ../data/results/results_hadcg_single/results_beam_width_4/result_full.xml --output_script evaluation_search_space_hdcg_hadcg.m --output_text evaluation_search_space_hdcg_hadcg.txt

../build/bin/h2sl-search-space-corpus-analysis ../data/results/results-dcg-single/results/result_full.xml ../data/results/results-adcg-single/result_full.xml ../data/results/results_hdcg_single/results_beam_width_1/result_full.xml ../data/results/results_hadcg_single/results_beam_width_4/result_full.xml --output_script evaluation_search_space_hdcg_hadcg.m --output_text evaluation_search_space_hdcg_hadcg.txt


