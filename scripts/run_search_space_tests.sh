#!/bin/bash          

echo Search Space Test 

#../build/bin/h2sl-dcg-test ../data/training/navigation_corpus/objects_in_groups/example_0086.xml --symbol_dictionary_groundings ../data/training/navigation_corpus/combined/symbol_dictionary_groundings_combined.xml --llm ../data/training/navigation_corpus/llm.xml --output /tmp/search_space_dcg.xml --debug 1 --beam_width 1


../build/bin/h2sl-adcg-test ../data/training/navigation_corpus/objects_in_groups/example_0086.xml --symbol_dictionary_groundings ../data/training/navigation_corpus/combined/symbol_dictionary_groundings_combined.xml --llm ../data/training/navigation_corpus/llm.xml --output /tmp/search_space_dcg.xml --debug 1 --beam_width 1
