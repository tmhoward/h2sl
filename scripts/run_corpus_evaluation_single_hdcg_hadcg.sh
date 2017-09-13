#!/bin/bash

# generate rule examples
../build/bin/h2sl-generate-rule-examples ../data/training/manipulation_corpus/example_0*.xml 

# generate symbol dictionary for groundings
../build/bin/h2sl-generate-symbol-dictionary ../data/training/manipulation_corpus/example_0*.xml --output sdg_hdcg_man.xml

# generate symbol dictionary for rules
../build/bin/h2sl-generate-symbol-dictionary ../data/training/manipulation_corpus/rule_example_0*.xml --output sdr_hdcg_man.xml

# generate symbol dictionary for combined
../build/bin/h2sl-generate-symbol-dictionary ../data/training/manipulation_corpus/example_0*.xml ../data/training/manipulation_corpus/rule_example_0*.xml --output sdc_hdcg_man.xml

# generate feature set from combined symbol dictionary
../build/bin/h2sl-generate-feature-set-groundings-and-rules ../data/training/manipulation_corpus/example_0*.xml ../data/training/manipulation_corpus/rule_example_0*.xml --sd sdc_hdcg_man.xml --output fs_hdcg_man.xml

# train models
../build/bin/h2sl-llm-train-groundings-and-rules ../data/training/manipulation_corpus/example_0*.xml ../data/training/manipulation_corpus/rule_example_0*.xml --sdg sdg_hdcg_man.xml --sdr sdr_hdcg_man.xml --feature_set fs_hdcg_man.xml --max_iterations 200 --output llm_hdcg_man.xml --threads 16

# ============================= Partition ========================================

# Not creating the partition.

# Random partition generation
# Case I: Training set is composed of all examples. Note that this command produces two files (test_0000.xml is 100% test set && test_0001.xml is 100% training_set)
# ../build/bin/h2sl-test-generator --num=1 --fr=2 --min_fr=0.0 --max_fr=1.0 ../data/training/manipulation_corpus/example_0*.xml
 
#mkdir -p ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition
#mv /tmp/test_0001.xml ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition/test_full.xml

# ============================= Training ========================================
# Not training using the test process training program
# ../build/bin/h2sl-test-process-training ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition/test_full.xml --symbol_dictionary_groundings ../data/training/manipulation_corpus/sdg_dcg_man.xml --feature_set ../data/training/manipulation_corpus/fs_dcg_man.xml --output ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition/ --evaluate_model 0 --threads 16

# ============================= Inference  ========================================

# Assume that the symbol dictionary and the feature sets are in ../data/training/manipulation_corpus
# The partition, llm and feature set in ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition/test_collated_full_h_models.xml 

# HDCG: Case I: Full corpus one file. 
mkdir -p ../data/results/manipulation_corpus/results_hdcg_single/results_beam_width_1/solutions

../build/bin/h2sl-test-process-inference --model hdcg --feature_set ../data/training/manipulation_corpus/fs_hdcg_man.xml --output ../data/results/manipulation_corpus/results_hdcg_single/results_beam_width_1/ --solution_directory ../data/results/manipulation_corpus/results_hdcg_single/results_beam_width_1/solutions/ ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition/test_collated_full_h_models.xml --beam_width 1 --test_training_set 1 --test_test_set 0

# ADCG: Case I: Full corpus one file. Beam 1.
mkdir -p ../data/results/manipulation_corpus/results_hadcg_single/results_beam_width_1/solutions

../build/bin/h2sl-test-process-inference --model hadcg --feature_set ../data/training/manipulation_corpus/fs_hdcg_man.xml --output ../data/results/manipulation_corpus/results_hadcg_single/results_beam_width_1/ --solution_directory ../data/results/manipulation_corpus/results_hadcg_single/results_beam_width_1/solutions/ ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition/test_collated_full_h_models.xml --beam_width 1 --test_training_set 1 --test_test_set 0

# HADCG: Case I: Full corpus one file. Beam 4.
mkdir -p ../data/results/manipulation_corpus/results_hadcg_single/results_beam_width_4/solutions

../build/bin/h2sl-test-process-inference --model hadcg --feature_set ../data/training/manipulation_corpus/fs_hdcg_man.xml --output ../data/results/manipulation_corpus/results_hadcg_single/results_beam_width_4/ --solution_directory ../data/results/manipulation_corpus/results_hadcg_single/results_beam_width_4/solutions/ ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition/test_collated_full_h_models.xml --beam_width 4 --test_training_set 1 --test_test_set 0

# HADCG: Case I: Full corpus one file. Beam 2.
mkdir -p ../data/results/manipulation_corpus/results_hadcg_single/results_beam_width_2/solutions

../build/bin/h2sl-test-process-inference --model hadcg --feature_set ../data/training/manipulation_corpus/fs_hdcg_man.xml --output ../data/results/manipulation_corpus/results_hadcg_single/results_beam_width_2/ --solution_directory ../data/results/manipulation_corpus/results_hadcg_single/results_beam_width_2/solutions/ ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition/test_collated_full_h_models.xml --beam_width 2 --test_training_set 1 --test_test_set 0

# ============================= combining results  =======================================
mkdir -p ../data/results/manipulation_corpus/results_hdcg_hadcg_single/results_beam_width_1/solutions

../build//bin/h2sl-test-combine-model-results --source_model hdcg --source ../data/results/manipulation_corpus/results_hdcg_single/results_beam_width_1/result_full.xml --target ../data/results/manipulation_corpus/results_hadcg_single/results_beam_width_1/result_full.xml --output ../data/results/manipulation_corpus/results_hdcg_hadcg_single/results_beam_width_1/result_hdcg_hadcg_single.xml

mkdir -p ../data/results/manipulation_corpus/results_hdcg_hadcg_single/results_beam_width_4/solutions

../build//bin/h2sl-test-combine-model-results --source_model hdcg --source ../data/results/manipulation_corpus/results_hdcg_single/results_beam_width_1/result_full.xml --target ../data/results/manipulation_corpus/results_hadcg_single/results_beam_width_4/result_full.xml --output ../data/results/manipulation_corpus/results_hdcg_hadcg_single/results_beam_width_4/result_hdcg_hadcg_single.xml

mkdir -p ../data/results/manipulation_corpus/results_hdcg_hadcg_single/results_beam_width_2/solutions

../build//bin/h2sl-test-combine-model-results --source_model hdcg --source ../data/results/manipulation_corpus/results_hdcg_single/results_beam_width_1/result_full.xml --target ../data/results/manipulation_corpus/results_hadcg_single/results_beam_width_2/result_full.xml --output ../data/results/manipulation_corpus/results_hdcg_hadcg_single/results_beam_width_2/result_hdcg_hadcg_single.xml

# ============================  Timing data for table ==================================================
../build/bin/h2sl-scrape-results-runtime-normalized-vs-world-size --input ../data/results/manipulation_corpus/results_hdcg_hadcg_single/results_beam_width_1/result_hdcg_hadcg_single.xml > normalized_timing_comparison_hdcg_hadcg_man.txt

# ============================  Search space data for table and scatter plot ==================================================
# Search Space Table
../build/bin/h2sl-search-space-corpus-analysis-hdcg-hadcg ../data/results/manipulation_corpus/results_hdcg_single/results_beam_width_1/result_full.xml ../data/results/manipulation_corpus/results_hadcg_single/results_beam_width_1/result_full.xml --output_script evaluation_search_space_hdcg_hadcg_man_beam_1.m --output_text evaluation_search_space_hdcg_hadcg_man_beam_1.txt

../build/bin/h2sl-search-space-corpus-analysis-hdcg-hadcg ../data/results/manipulation_corpus/results_hdcg_single/results_beam_width_1/result_full.xml ../data/results/manipulation_corpus/results_hadcg_single/results_beam_width_2/result_full.xml --output_script evaluation_search_space_hdcg_hadcg_man_beam_2.m --output_text evaluation_search_space_hdcg_hadcg_man_beam_2.txt



