#!/bin/bash
export ED=../data/training/example_corpus/
export SSEOD=../data/training/example_corpus/search_space_evaluation_output/
echo ${SSEOD}

# create results directory
mkdir -p ${SSEOD}
mkdir -p ${SSEOD}/test
mkdir -p ${SSEOD}/test/dcg
mkdir -p ${SSEOD}/test/hdcg
mkdir -p ${SSEOD}/test/adcg
mkdir -p ${SSEOD}/test/hadcg
mkdir -p ${SSEOD}/test/dcg/collated
mkdir -p ${SSEOD}/test/hdcg/collated
mkdir -p ${SSEOD}/test/adcg/collated
mkdir -p ${SSEOD}/test/hadcg/collated

# generate rules
echo "generating rule examples"
../build/bin/h2sl-generate-rule-examples ${ED}/example_00*.xml

# symbol dictionary generation
echo "generating symbol dictionary for dcg groundings"
../build/bin/h2sl-generate-symbol-dictionary ${ED}/example_0*.xml --output ${SSEOD}/sdg_dcg.xml
echo "generating symbol dictionary for hdcg groundings"
../build/bin/h2sl-generate-symbol-dictionary ${ED}/example_0*.xml --output ${SSEOD}/sdg_hdcg.xml
echo "generating symbol dictionary for hdcg rules"
../build/bin/h2sl-generate-symbol-dictionary ${ED}/rule_example_00*.xml --output ${SSEOD}/sdr_hdcg.xml
echo "generating symbol dictionary for hdcg groundings and rules"
../build/bin/h2sl-generate-symbol-dictionary ${ED}/example_00*.xml ${ED}/rule_example_00*.xml --output ${SSEOD}/sdc_hdcg.xml

# generate feature set
echo "generating feature set for dcg and adcg models"
../build/bin/h2sl-generate-feature-set-groundings ${ED}/example_00*.xml --sd ${SSEOD}/sdg_dcg.xml --output ${SSEOD}/fs_dcg.xml
echo "generating feature set for hdcg and hadcg models"
../build/bin/h2sl-generate-feature-set-groundings-and-rules ${ED}/example_00*.xml ${ED}/rule_example_00*.xml --sd ${SSEOD}/sdc_hdcg.xml --output ${SSEOD}/fs_hdcg.xml

# generate tests
echo "generating tests"
../build/bin/h2sl-test-generator --num=1 --fr=7 --min_fr=0.2 --max_fr=0.8 ${ED}/example_0*.xml --output ${SSEOD}/test/

# train dcg and adcg models
echo "training dcg models"
../build/bin/h2sl-test-process-training ${SSEOD}/test/test_*.xml --symbol_dictionary_groundings ${SSEOD}/sdg_dcg.xml --feature_set ${SSEOD}/fs_dcg.xml --output ${SSEOD}/test/dcg/collated/ --evaluate_model 0 --threads 12
echo "copying dcg models to adcg folder (identical trained model)"
cp -r ${SSEOD}/test/dcg/collated/test_*.xml ${SSEOD}/test/adcg/collated/

# train hdcg and hadcg models
echo "training hdcg models"
../build/bin/h2sl-test-process-training ${SSEOD}/test/test_*.xml --symbol_dictionary_groundings ${SSEOD}/sdg_hdcg.xml --symbol_dictionary_rules ${SSEOD}/sdr_hdcg.xml --feature_set ${SSEOD}/fs_hdcg.xml --output ${SSEOD}/test/hdcg/collated --evaluate_model 0 --threads 12
echo "copying hdcg models to hadcg folder (identical trained model)"
cp -r ${SSEOD}/test/hdcg/collated/test_*.xml ${SSEOD}/test/hadcg/collated/

# list trained models
echo "listing dcg collated tests"
ls ${SSEOD}/test/dcg/collated/test_*.xml
echo "listing adcg collated tests"
ls ${SSEOD}/test/adcg/collated/test_*.xml
echo "listing hdcg collated tests"
ls ${SSEOD}/test/hdcg/collated/test_*.xml
echo "listing hadcg collated tests"
ls ${SSEOD}/test/hadcg/collated/test_*.xml
