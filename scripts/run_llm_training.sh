#!/bin/bash          

# Symbol dictionary generation
../build/bin/h2sl-generate-symbol-dictionary ../data/training/navigation_corpus/groups/example_00*.xml ../data/training/navigation_corpus/objects_in_groups/example_00*.xml --output ../data/training/navigation_corpus/combined/symbol_dictionary_groundings_combined.xml

# Feature set generation
../build/bin/h2sl-generate-feature-set-groundings ../data/training/navigation_corpus/groups/example_00*.xml ../data/training/navigation_corpus/objects_in_groups/example_00*.xml --sd ../data/training/navigation_corpus/combined/symbol_dictionary_groundings_combined.xml  --output ../data/training/navigation_corpus/combined/feature_set_groundings_combined.xml 

# LLM training
../build/bin/h2sl-llm-train-groundings ../data/training/navigation_corpus/groups/example_00*.xml ../data/training/navigation_corpus/objects_in_groups/example_00*.xml --feature_set ../data/training/navigation_corpus/combined/feature_set_groundings_combined.xml --sd ../data/training/navigation_corpus/combined/symbol_dictionary_groundings_combined.xml --output ../data/training/navigation_corpus/combined/llm_combined.xml --max_iterations 200
