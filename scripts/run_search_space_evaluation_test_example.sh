#!/bin/bash
export ED=../data/training/example_corpus/
export SSEOD=../data/training/example_corpus/search_space_evaluation_output/
echo ${SSEOD}

mkdir -p ${SSEOD}/test/dcg/solution
mkdir -p ${SSEOD}/test/hdcg/solution
mkdir -p ${SSEOD}/test/adcg/solution
mkdir -p ${SSEOD}/test/hadcg/solution
mkdir -p ${SSEOD}/test/dcg/output
mkdir -p ${SSEOD}/test/hdcg/output
mkdir -p ${SSEOD}/test/adcg/output
mkdir -p ${SSEOD}/test/hadcg/output

# test dcg models
 ../build/bin/h2sl-test-process-inference ${SSEOD}/test/dcg/collated/test_*.xml --model dcg --feature_set ${SSEOD}/fs_dcg.xml --solution_directory ${SSEOD}/test/dcg/solution/ --beam_width 2 --test_training_set 1 --test_test_set 1 --output ${SSEOD}/test/dcg/output/

# test adcg models
 ../build/bin/h2sl-test-process-inference ${SSEOD}/test/adcg/collated/test_*.xml --model adcg --feature_set ${SSEOD}/fs_dcg.xml --solution_directory ${SSEOD}/test/adcg/solution/ --beam_width 2 --test_training_set 1 --test_test_set 1 --output ${SSEOD}/test/adcg/output/

# test hdcg models
../build/bin/h2sl-test-process-inference ${SSEOD}/test/hdcg/collated/test_*.xml --model hdcg --feature_set ${SSEOD}/fs_hdcg.xml --solution_directory ${SSEOD}/test/hdcg/solution/ --beam_width 2 --test_training_set 1 --test_test_set 1 --output ${SSEOD}/test/hdcg/output/

# test hadcg models
../build/bin/h2sl-test-process-inference ${SSEOD}/test/hadcg/collated/test_*.xml --model hadcg --feature_set ${SSEOD}/fs_hdcg.xml --solution_directory ${SSEOD}/test/hadcg/solution/ --beam_width 2 --test_training_set 1 --test_test_set 1 --output ${SSEOD}/test/hadcg/output/

