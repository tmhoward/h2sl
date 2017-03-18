#!/bin/bash          

echo ADCG Training

# First, generate the symbol dictionary for the groundings by running the following command from the build directory. This describes the space of symbols in the grounding-based model.
# ../build/bin/h2sl-generate-symbol-dictionary ../data/training/example_corpus/example_00*.xml --output ../data/training/example_corpus/symbol_dictionary_groundings.xml
../build/bin/h2sl-generate-symbol-dictionary ../data/training/navigation_corpus/groups/example_00*.xml --output ../data/training/navigation_corpus/groups/symbol_dictionary_groundings.xml


# Next, generate the feature set from the examples and symbol dictionaries by running the following command from the build directory. This will create the feature set for the log-linear model you will train in the following step.
# ../build/bin/h2sl-generate-feature-set-groundings ../data/training/navigation_corpus/groups/example_00*.xml --symbol_dictionary_groundings ../data/training/navigation_corpus/groups/symbol_dictionary_groundings.xml --output ../data/training/navigation_corpus/groups/feature_set_adcg.xml

# Now train the model by running the following command from the build directory. This will produce a llm_adcg.xml file that contains the trained log-linear models.
# ../build/bin/h2sl-llm-train-groundings ../data/training/example_corpus/example_00*.xml --feature_set ../data/training/example_corpus/feature_set_adcg.xml --symbol_dictionary_groundings ../data/training/example_corpus/symbol_dictionary_groundings.xml --output ../data/training/example_corpus/llm_adcg.xml


# ../build/bin/h2sl-llm-train-groundings ../data/training/navigation_corpus/groups/example_00*.xml --feature_set ../data/training/navigation_corpus/groups/feature_set_adcg.xml --symbol_dictionary_groundings ../data/training/navigation_corpus/groups/symbol_dictionary_groundings.xml --output ../data/training/navigation_corpus/groups/llm_adcg.xml
