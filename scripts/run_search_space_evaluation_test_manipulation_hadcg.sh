#!/bin/bash
export ED=../data/training/manipulation_corpus/
export SSEOD=../data/training/manipulation_corpus/search_space_evaluation_output/
echo ${SSEOD}

mkdir -p ${SSEOD}/test/hadcg/solution
mkdir -p ${SSEOD}/test/hadcg/output

# test hadcg models
../build/bin/h2sl-test-process-inference ${SSEOD}/test/hadcg/collated/test_*.xml --model hadcg --feature_set ${SSEOD}/fs_hdcg.xml --solution_directory ${SSEOD}/test/hadcg/solution/ --beam_width 1 --test_training_set 1 --test_test_set 1 --output ${SSEOD}/test/hadcg/output/

