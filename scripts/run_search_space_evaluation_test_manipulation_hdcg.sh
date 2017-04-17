#!/bin/bash
export ED=../data/training/manipulation_corpus/
export SSEOD=../data/training/manipulation_corpus/search_space_evaluation_output/
echo ${SSEOD}

mkdir -p ${SSEOD}/test/hdcg/solution
mkdir -p ${SSEOD}/test/hdcg/output

# test hdcg models
../build/bin/h2sl-test-process-inference ${SSEOD}/test/hdcg/collated/test_*.xml --model hdcg --feature_set ${SSEOD}/fs_hdcg.xml --solution_directory ${SSEOD}/test/hdcg/solution/ --beam_width 1 --test_training_set 1 --test_test_set 1 --output ${SSEOD}/test/hdcg/output/
