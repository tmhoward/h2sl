#!/bin/bash          

echo ADCG Training

#============================= Generate symbol dictionary 
# The groups data alone.
#../build/bin/h2sl-generate-symbol-dictionary ../data/training/navigation_corpus/groups/example_00*.xml --output ../data/training/navigation_corpus/groups/symbol_dictionary_groundings.xml

# Combined groups and objects in groups data set.
../build/bin/h2sl-generate-symbol-dictionary ../data/training/navigation_corpus/groups/example_00*.xml ../data/training/navigation_corpus/objects_in_groups/example_00*.xml --output symbol_dictionary.xml


#============================ Generate feature sets. 
# Groups data set alone.

# Combined data set.
../build/bin/h2sl-generate-feature-set-groundings ../data/training/navigation_corpus/groups/example_00*.xml ../data/training/navigation_corpus/objects_in_groups/example_00*.xml --sd symbol_dictionary.xml --output feature_set.xml

# LLM Train 
../build/bin/h2sl-llm-train-groundings ../data/training/navigation_corpus/groups/example_00*.xml ../data/training/navigation_corpus/objects_in_groups/example_00*.xml --sd symbol_dictionary.xml --feature_set feature_set.xml --max_iterations 50 --output llm.xml
