#!/bin/bash          

echo HDCG Training

#First, generate the rule examples by running the following command from the build directory.  This generates examples with the "rule_" prefix and rule-based symbols.
../build/bin/h2sl-generate-rule-examples ../data/training/example_corpus/example_00*.xml

#Second, generate the symbol dictionary for the groundings by running the following command from the build directory.  This describes the space of symbols in the grounding-based model.
../build/bin/h2sl-generate-symbol-dictionary ../data/training/example_corpus/example_00*.xml --output ../data/training/example_corpus/symbol_dictionary_groundings.xml

#Third, generate the symbol dictionary for the rules by running the following command from the build directory.  This describes the space of symbols in the rule-based model.
../build/bin/h2sl-generate-symbol-dictionary ../data/training/example_corpus/rule_example_00*.xml --output ../data/training/example_corpus/symbol_dictionary_rules.xml

#Next, generate the feature set from the examples and symbol dictionaries by running the following command from the build directory.  This will create the feature set for the log-linear model you will train in the following step.
../build/bin/h2sl-generate-feature-set-groundings-and-rules ../data/training/example_corpus/example_00*.xml ../data/training/example_corpus/rule_example_00*.xml --symbol_dictionary_groundings ../data/training/example_corpus/symbol_dictionary_groundings.xml --symbol_dictionary_rules ../data/training/example_corpus/symbol_dictionary_rules.xml --output ../data/training/example_corpus/feature_set_hdcg.xml

#Now train the combined model by running the following command from the build directory.  This will produce a llm.xml file that contains the trained log-linear models.
../build/bin/h2sl-llm-train-groundings-and-rules ../data/training/example_corpus/example_00*.xml ../data/training/example_corpus/rule_example_00*.xml --feature_set ../data/training/example_corpus/feature_set_hdcg.xml --symbol_dictionary_groundings ../data/training/example_corpus/symbol_dictionary_groundings.xml --symbol_dictionary_rules ../data/training/example_corpus/symbol_dictionary_rules.xml --output ../data/training/example_corpus/llm_hdcg.xml

