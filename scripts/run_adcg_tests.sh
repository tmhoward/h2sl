#!/bin/bash          

echo ADCG Test 

# Finally, to run the ADCG demo program run the following command from the build directory.

../build/bin/h2sl-adcg-demo --grammar ../data/grammar/grammar.xml --command "walk to the furthest chair" --symbol_dictionary ../data/training/example_corpus/symbol_dictionary_groundings.xml --world ../data/training/example_corpus/example_0034.xml --llm ../data/training/example_corpus/llm_adcg.xml --output /tmp/example_adcg.xml --debug 1


# ADCG Tests
../build/bin/h2sl-adcg-test ../data/training/example_corpus/example_00*.xml --grammar ../data/grammar/grammar.xml --llm ../data/training/example_corpus/llm_adcg.xml --symbol_dictionary_groundings ../data/training/example_corpus/symbol_dictionary_groundings.xml





