#!/bin/bash

# =============================  PARTITION GENERATION ========================================
# Random partition generation

# =============================  TRAINING ========================================
# Training of the training partition and creation of LLM and insertion of the symbol dictionary
# Case I: Full partition single file
../build/bin/h2sl-test-process-training ../data/cross_validation_partitions/combined_nav_corpora/0002/test_full.xml --symbol_dictionary_groundings ../data/training/navigation_corpus/combined/symbol_dictionary_groundings_combined.xml --feature_set ../data/training/navigation_corpus/combined/feature_set_groundings_combined.xml --output ../data/cross_validation_partitions/combined_nav_corpora/collated_0002/ --evaluate_model 1 --threads 16

# Case II: Corpus. Multiple files.
../build/bin/h2sl-test-process-training ../data/cross_validation_partitions/combined_nav_corpora/0001/test_*.xml --symbol_dictionary_groundings ../data/training/navigation_corpus/combined/symbol_dictionary_groundings_combined.xml --feature_set ../data/training/navigation_corpus/combined/feature_set_groundings_combined.xml --output ../data/cross_validation_partitions/combined_nav_corpora/collated_0001/ --evaluate_model 0 --threads 16

# =============================  INFERENCE ========================================
# ADCG: Case I: Full corpus one file. Note: folder index 2 has the single file generated. 
../build/bin/h2sl-test-process-inference --model adcg --feature_set ../data/training/navigation_corpus/combined/feature_set_groundings_combined.xml --output ../data/results/results-adcg-single/ --solution_directory ../data/results/results-adcg-single/solutions/ ../data/cross_validation_partitions/combined_nav_corpora/collated_0002/test_collated_full.xml --beam_width 1 --test_training_set 1 --test_test_set 0

# ADCG: Case II: Entire corpus. 
../build/bin/h2sl-test-process-inference --model adcg --feature_set ../data/training/navigation_corpus/combined/feature_set_groundings_combined.xml --output ../data/results/results-adcg-full-corpus/ --solution_directory ../data/results/results-adcg-full-corpus/solutions/ ../data/cross_validation_partitions/combined_nav_corpora/collated_0001/test_*.xml --beam_width 1 --test_training_set 0 --test_test_set 1

# DCG: Case I: Full corpus one file. Note: folder index 2 has the single file generated.
../build/bin/h2sl-test-process-inference --model dcg --feature_set ../data/training/navigation_corpus/combined/feature_set_groundings_combined.xml --output ../data/results/results-dcg-single/results/ --solution_directory ../data/results/results-dcg-single/results/solutions/ ../data/cross_validation_partitions/combined_nav_corpora/collated_0002/test_collated_full.xml --beam_width 1 --test_training_set 1 --test_test_set 0

# DCG: Case II: Entire corpus.
../build/bin/h2sl-test-process-inference --model dcg --feature_set ../data/training/navigation_corpus/combined/feature_set_groundings_combined.xml --output ../data/results/results-dcg-full-corpus/ --solution_directory ../data/results/results-dcg-full-corpus/solutions/ ../data/cross_validation_partitions/combined_nav_corpora/collated_0001/test_*.xml --beam_width 1 --test_training_set 0 --test_test_set 1

# =============================  COMBINING THE RESULTS =======================================
../build//bin/h2sl-test-combine-model-results --source_model dcg --source ../data/results/results-dcg-adcg-full-corpus/result_dcg_0129.xml --target ../data/results/results-dcg-adcg-full-corpus/result_adcg_0129.xml --output ../data/results/results-dcg-adcg-full-corpus/result_dcg_adcg_all.xml

../build//bin/h2sl-test-combine-model-results --source_model dcg --source ../data/results/results-dcg-adcg-single/result_dcg_single.xml --target ../data/results/results-dcg-adcg-single/result_adcg_single.xml --output ../data/results/results-dcg-adcg-single/result_dcg_adcg_single.xml


# =============================  MATLAB CONVERSION ===========================================
../build/bin/h2sl-test-convert-xml-to-mat --input ../data/results/results-dcg-adcg-full-corpus/result_dcg_adcg_all.xml --output ../data/results/results-dcg-adcg-full-corpus/result_dcg_adcg_all_backup.m

