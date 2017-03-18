#!/bin/bash          

echo HADCG Test 

# Finally, to run the HADCG demo program run the following command from the build directory.

../build/bin/h2sl-hadcg-demo --grammar ../data/grammar/grammar.xml --command "walk to the furthest chair" --symbol_dictionary ../data/training/example_corpus/symbol_dictionary_groundings.xml --world ../data/training/example_corpus/example_0034.xml --llm ../data/training/example_corpus/llm_hadcg.xml --output /tmp/example_hadcg.xml --debug 1

# To run the HADCG test, run the following command from the build directory

../build/bin/h2sl-hadcg-test ../data/training/example_corpus/example_00*.xml --grammar ../data/grammar/grammar.xml --llm ../data/training/example_corpus/llm_hadcg.xml --symbol_dictionary_groundings ../data/training/example_corpus/symbol_dictionary_groundings.xml


