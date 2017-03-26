#!/bin/bash          

echo DCG Test 

# To run the DCG demo program run the following command from the build directory.

echo DCG Demo
../build/bin/h2sl-dcg-demo --grammar ../data/grammar/grammar.xml --command "walk to the furthest chair" --symbol_dictionary ../data/training/example_corpus/symbol_dictionary_groundings.xml --world ../data/training/example_corpus/example_0034.xml --llm ../data/training/example_corpus/llm_dcg.xml --output /tmp/example_dcg.xml --debug 1

#To run the DCG test, run the following command from the build directory:
echo DCG test
../build/bin/h2sl-dcg-test ../data/training/example_corpus/example_00*.xml --llm ../data/training/example_corpus/llm_dcg.xml --grammar ../data/grammar/grammar.xml --symbol_dictionary_groundings ../data/training/example_corpus/symbol_dictionary_groundings.xml

#../build/bin/h2sl-dcg-test ../data/training/example_corpus/example_0001.xml --llm ../data/training/example_corpus/llm_dcg.xml --symbol_dictionary_groundings ../data/training/example_corpus/symbol_dictionary_groundings.xml

#To run the Graphical User Interface (GUI), run the following command from the build directory:
echo DCG GUI
../build/bin/h2sl-gui-demo --world ../data/training/example_corpus/example_0001.xml --llm ../data/training/example_corpus/llm_dcg.xml --symbol_dictionary_groundings ../data/training/example_corpus/symbol_dictionary_groundings.xml --grammar ../data/grammar/grammar.xml

