#!/bin/bash

# Symbol dictionary generation
../build/bin/h2sl-generate-symbol-dictionary ../data/training/manipulation_corpus/example_0*.xml --output ../data/training/manipulation_corpus/sdg_dcg_man.xml

# Feature set generation
../build/bin/h2sl-generate-feature-set-groundings ../data/training/manipulation_corpus/example_0*.xml --sd ../data/training/manipulation_corpus/sdg_dcg_man.xml  --output ../data/training/manipulation_corpus/fs_dcg_man.xml

# ============================= Partition ========================================
# Random partition generation
# Case I: Training set is composed of all examples. Note that this command produces two files (test_0000.xml is 100% test set && test_0001.xml is 100% training_set)
../build/bin/h2sl-test-generator --num=1 --fr=2 --min_fr=0.0 --max_fr=1.0 ../data/training/manipulation_corpus/example_0*.xml
 
mkdir -p ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition
mv /tmp/test_0001.xml ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition/test_full.xml

# ============================= Training ========================================
../build/bin/h2sl-test-process-training ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition/test_full.xml --symbol_dictionary_groundings ../data/training/manipulation_corpus/sdg_dcg_man.xml --feature_set ../data/training/manipulation_corpus/fs_dcg_man.xml --output ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition/ --evaluate_model 0 --threads 16

# ============================= Inference  ========================================
# DCG: Case I: Full corpus one file. 
mkdir -p ../data/results/manipulation_corpus/results_dcg_single/results_beam_width_1/solutions

../build/bin/h2sl-test-process-inference --model dcg --feature_set ../data/training/manipulation_corpus/fs_dcg_man.xml --output ../data/results/manipulation_corpus/results_dcg_single/results_beam_width_1/ --solution_directory ../data/results/manipulation_corpus/results_dcg_single/results_beam_width_1/solutions/ ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition/test_collated_full.xml --beam_width 1 --test_training_set 1 --test_test_set 0

# ADCG: Case I: Full corpus one file. Beam 1.
mkdir -p ../data/results/manipulation_corpus/results_adcg_single/results_beam_width_1/solutions

../build/bin/h2sl-test-process-inference --model adcg --feature_set ../data/training/manipulation_corpus/fs_dcg_man.xml --output ../data/results/manipulation_corpus/results_adcg_single/results_beam_width_1/ --solution_directory ../data/results/manipulation_corpus/results_adcg_single/results_beam_width_1/solutions/ ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition/test_collated_full.xml --beam_width 1 --test_training_set 1 --test_test_set 0

# HADCG: Case I: Full corpus one file. Beam 4.
mkdir -p ../data/results/manipulation_corpus/results_adcg_single/results_beam_width_4/solutions

../build/bin/h2sl-test-process-inference --model adcg --feature_set ../data/training/manipulation_corpus/fs_dcg_man.xml --output ../data/results/manipulation_corpus/results_adcg_single/results_beam_width_4/ --solution_directory ../data/results/manipulation_corpus/results_adcg_single/results_beam_width_4/solutions/ ../data/cross_validation_partitions/manipulation_corpus/collated_single_partition/test_collated_full.xml --beam_width 4 --test_training_set 1 --test_test_set 0

# ============================= combining results  =======================================
mkdir -p ../data/results/manipulation_corpus/results_dcg_adcg_single/results_beam_width_1/solutions

../build//bin/h2sl-test-combine-model-results --source_model dcg --source ../data/results/manipulation_corpus/results_dcg_single/results_beam_width_1/result_full.xml --target ../data/results/manipulation_corpus/results_adcg_single/results_beam_width_1/result_full.xml --output ../data/results/manipulation_corpus/results_dcg_adcg_single/results_beam_width_1/result_dcg_adcg_single.xml

mkdir -p ../data/results/manipulation_corpus/results_dcg_adcg_single/results_beam_width_4/solutions

../build//bin/h2sl-test-combine-model-results --source_model dcg --source ../data/results/manipulation_corpus/results_dcg_single/results_beam_width_1/result_full.xml --target ../data/results/manipulation_corpus/results_adcg_single/results_beam_width_4/result_full.xml --output ../data/results/manipulation_corpus/results_dcg_adcg_single/results_beam_width_4/result_dcg_adcg_single.xml

mkdir -p ../data/results/manipulation_corpus/results_dcg_adcg_single/results_beam_width_2/solutions

../build//bin/h2sl-test-combine-model-results --source_model dcg --source ../data/results/manipulation_corpus/results_dcg_single/results_beam_width_1/result_full.xml --target ../data/results/manipulation_corpus/results_adcg_single/results_beam_width_2/result_full.xml --output ../data/results/manipulation_corpus/results_dcg_adcg_single/results_beam_width_2/result_dcg_adcg_single.xml

#

# ============================  Timing data for table ==================================================
../build/bin/h2sl-scrape-results-runtime-normalized-vs-world-size --input ../data/results/manipulation_corpus/results_dcg_adcg_single/results_beam_width_1/result_dcg_adcg_single.xml > normalized_timing_comparison_dcg_adcg_man.txt

# ============================  Search space data for table and scatter plot ==================================================
# Search Space Table

../build/bin/h2sl-search-space-corpus-analysis-dcg-adcg ../data/results/manipulation_corpus/results_dcg_single/results_beam_width_1/result_full.xml ../data/results/manipulation_corpus/results_adcg_single/results_beam_width_1/result_full.xml --output_script evaluation_search_space_dcg_adcg_man_beam_1.m --output_text evaluation_search_space_dcg_adcg_man_beam_1.txt

../build/bin/h2sl-search-space-corpus-analysis-dcg-adcg ../data/results/manipulation_corpus/results_dcg_single/results_beam_width_1/result_full.xml ../data/results/manipulation_corpus/results_adcg_single/results_beam_width_2/result_full.xml --output_script evaluation_search_space_dcg_adcg_man_beam_2.m --output_text evaluation_search_space_dcg_adcg_man_beam_2.txt



























