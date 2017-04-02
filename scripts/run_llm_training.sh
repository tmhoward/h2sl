#!/bin/bash          

echo DCG Training

#To generate a symbol dictionary for the example corpus, run the following command from the build directory:
echo Generating symbol dictionary
../build/bin/h2sl-generate-symbol-dictionary ../data/training/example_corpus/example_00*.xml --output ../data/training/example_corpus/symbol_dictionary_groundings.xml

#To generate a feature set for the example corpus using the just generated symbol dictionary, run the following command from the build directory:
echo Generating feature sets. 
../build/bin/h2sl-generate-feature-set-groundings ../data/training/example_corpus/example_00*.xml --symbol_dictionary ../data/training/example_corpus/symbol_dictionary_groundings.xml --output ../data/training/example_corpus/feature_set_dcg.xml

#To train a model using the just generated symbol dictionary and feature set, run the following command from the build directory:
echo LLM Training 
../build/bin/h2sl-llm-train-groundings ../data/training/example_corpus/example_00*.xml --feature_set ../data/training/example_corpus/feature_set_dcg.xml --symbol_dictionary_groundings ../data/training/example_corpus/symbol_dictionary_groundings.xml --output ../data/training/example_corpus/llm_dcg.xml

