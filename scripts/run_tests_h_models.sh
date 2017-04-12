#!/bin/bash          

# hdcg tests
../build/bin/h2sl-hdcg-test ../data/training/navigation_corpus/groups/example_00*.xml ../data/training/navigation_corpus/objects_in_groups/example_00*.xml --sd sdg_hdcg_nav.xml --llm llm_hdcg_nav.xml --beam_width 1 --debug 0

# hadcg tests
../build/bin/h2sl-hadcg-test ../data/training/navigation_corpus/groups/example_00*.xml ../data/training/navigation_corpus/objects_in_groups/example_00*.xml --sd sdg_hdcg_nav.xml --llm llm_hdcg_nav.xml --beam_width 1 --debug 0

