#!/bin/bash          

echo ADCG Training

# Generate symbol dictionary 

../build/bin/h2sl-generate-symbol-dictionary ../data/training/navigation_corpus/groups/example_00*.xml --output ../data/training/navigation_corpus/groups/symbol_dictionary_groundings.xml

# Generate feature sets. 

../build/bin/h2sl-generate-feature-set-groundings ../data/training/navigation_corpus/groups/example_00*.xml --symbol_dictionary_groundings ../data/training/navigation_corpus/groups/symbol_dictionary_groundings.xml --output ../data/training/navigation_corpus/groups/feature_set_adcg.xml

# LLM Train 

../build/bin/h2sl-llm-train-groundings ../data/training/navigation_corpus/groups/example_00*.xml --feature_set ../data/training/navigation_corpus/groups/feature_set_adcg.xml --symbol_dictionary_groundings ../data/training/navigation_corpus/groups/symbol_dictionary_groundings.xml --output ../data/training/navigation_corpus/groups/llm_adcg.xml

