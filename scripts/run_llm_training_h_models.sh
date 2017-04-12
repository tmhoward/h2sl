#!/bin/bash          

# generate rule examples
../build/bin/h2sl-generate-rule-examples ../data/training/navigation_corpus/groups/example_00*.xml ../data/training/navigation_corpus/objects_in_groups/example_00*.xml

# generate symbol dictionary for groundings
../build/bin/h2sl-generate-symbol-dictionary ../data/training/navigation_corpus/groups/example_00*.xml ../data/training/navigation_corpus/objects_in_groups/example_00*.xml --output sdg_hdcg_nav.xml

# generate symbol dictionary for rules
../build/bin/h2sl-generate-symbol-dictionary ../data/training/navigation_corpus/groups/rule_example_00*.xml ../data/training/navigation_corpus/objects_in_groups/rule_example_00*.xml --output sdr_hdcg_nav.xml

# generate symbol dictionary for combined
../build/bin/h2sl-generate-symbol-dictionary ../data/training/navigation_corpus/groups/example_00*.xml ../data/training/navigation_corpus/groups/rule_example_00*.xml ../data/training/navigation_corpus/objects_in_groups/example_00*.xml ../data/training/navigation_corpus/objects_in_groups/rule_example_00*.xml --output sdc_hdcg_nav.xml

# generate feature set from combined symbol dictionary
../build/bin/h2sl-generate-feature-set-groundings-and-rules ../data/training/navigation_corpus/groups/example_00*.xml ../data/training/navigation_corpus/groups/rule_example_00*.xml ../data/training/navigation_corpus/objects_in_groups/example_00*.xml ../data/training/navigation_corpus/objects_in_groups/rule_example_00*.xml --sd sdc_hdcg_nav.xml --output fs_hdcg_nav.xml

# train models
../build/bin/h2sl-llm-train-groundings-and-rules ../data/training/navigation_corpus/groups/example_00*.xml ../data/training/navigation_corpus/groups/rule_example_00*.xml ../data/training/navigation_corpus/objects_in_groups/example_00*.xml ../data/training/navigation_corpus/objects_in_groups/rule_example_00*.xml --sdg sdg_hdcg_nav.xml --sdr sdr_hdcg_nav.xml --feature_set fs_hdcg_nav.xml --max_iterations 200 --output llm_hdcg_nav.xml --threads 16



