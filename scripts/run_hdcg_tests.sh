#!/bin/bash          

echo HDCG Test 

# Run the HDCG demo program run the following command from the build directory.
../build/bin/h2sl-hdcg-demo --grammar ../data/grammar/grammar.xml --command "walk to the box" --symbol_dictionary ../data/training/example_corpus/symbol_dictionary_groundings.xml --world ../data/training/example_corpus/example_0001.xml --llm ../data/training/example_corpus/llm_hdcg.xml --output /tmp/example_hdcg.xml --debug 1

#To run the HDCG test, run the following command from the build directory
../build/bin/h2sl-hdcg-test ../data/training/example_corpus/example_00*.xml --grammar ../data/grammar/grammar.xml --llm ../data/training/example_corpus/llm_hdcg.xml --symbol_dictionary_groundings ../data/training/example_corpus/symbol_dictionary_groundings.xml


