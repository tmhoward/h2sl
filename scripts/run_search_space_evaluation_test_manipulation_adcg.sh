#!/bin/bash
export ED=../data/training/manipulation_corpus/
export SSEOD=../data/training/manipulation_corpus/search_space_evaluation_output/
echo ${SSEOD}

mkdir -p ${SSEOD}/test/adcg/solution
mkdir -p ${SSEOD}/test/adcg/output

# test adcg models
 ../build/bin/h2sl-test-process-inference ${SSEOD}/test/adcg/collated/test_*.xml --model adcg --feature_set ${SSEOD}/fs_dcg.xml --solution_directory ${SSEOD}/test/adcg/solution/ --beam_width 1 --test_training_set 1 --test_test_set 1 --output ${SSEOD}/test/adcg/output/
