#!/bin/bash
ED=../data/training/example_corpus/
SSEOD=../data/training/example_corpus/search_space_evaluation_output/
echo "ED:${ED}"
echo "SSEOD:${SSEOD}"

if [ $# -gt 0 ]
  then
    MINFR=$1
else
  MINFR=0.2
fi

if [ $# -gt 1 ]; then
  MAXFR=$2
else
  MAXFR=0.8
fi

if [ $# -gt 2 ]; then
  NUMFR=$3
else
  NUMFR=13
fi

if [ $# -gt 3 ]; then
  NUM=$4
else
  NUM=5
fi

if [ $# -gt 4 ]; then
  BW=$5
else
  BW=1
fi

echo "MINFR:${MINFR}"
echo "MAXFR:${MAXFR}"
echo "NUMFR:${NUMFR}"
echo "NUM:${NUM}"
echo "BW:${BW}"

export GROUNDING_EXAMPLES=${ED}/example_0*.xml

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
../build/bin/h2sl-generate-rule-examples ${GROUNDING_EXAMPLES}

export RULE_EXAMPLES=${ED}/rule_example_0*.xml

# symbol dictionary generation
echo "generating symbol dictionary for dcg groundings"
../build/bin/h2sl-generate-symbol-dictionary ${GROUNDING_EXAMPLES} --output ${SSEOD}/sdg_dcg.xml
echo "generating symbol dictionary for hdcg groundings"
../build/bin/h2sl-generate-symbol-dictionary ${GROUNDING_EXAMPLES} --output ${SSEOD}/sdg_hdcg.xml
echo "generating symbol dictionary for hdcg rules"
../build/bin/h2sl-generate-symbol-dictionary ${RULE_EXAMPLES} --output ${SSEOD}/sdr_hdcg.xml
echo "generating symbol dictionary for hdcg groundings and rules"
../build/bin/h2sl-generate-symbol-dictionary ${GROUNDING_EXAMPLES} ${RULE_EXAMPLES} --output ${SSEOD}/sdc_hdcg.xml

# generate feature set
echo "generating feature set for dcg and adcg models"
../build/bin/h2sl-generate-feature-set-groundings ${GROUNDING_EXAMPLES} --sd ${SSEOD}/sdg_dcg.xml --output ${SSEOD}/fs_dcg.xml
echo "generating feature set for hdcg and hadcg models"
../build/bin/h2sl-generate-feature-set-groundings-and-rules ${GROUNDING_EXAMPLES} ${RULE_EXAMPLES} --sd ${SSEOD}/sdc_hdcg.xml --output ${SSEOD}/fs_hdcg.xml

# generate tests
echo "generating tests"
../build/bin/h2sl-test-generator --num=${NUM} --fr=${NUMFR} --min_fr=${MINFR} --max_fr=${MAXFR} ${GROUNDING_EXAMPLES} --output ${SSEOD}/test/

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

mkdir -p ${SSEOD}/test/dcg/solution
mkdir -p ${SSEOD}/test/hdcg/solution
mkdir -p ${SSEOD}/test/adcg/solution
mkdir -p ${SSEOD}/test/hadcg/solution
mkdir -p ${SSEOD}/test/dcg/output
mkdir -p ${SSEOD}/test/hdcg/output
mkdir -p ${SSEOD}/test/adcg/output
mkdir -p ${SSEOD}/test/hadcg/output

# test dcg models
 ../build/bin/h2sl-test-process-inference ${SSEOD}/test/dcg/collated/test_*.xml --model dcg --feature_set ${SSEOD}/fs_dcg.xml --solution_directory ${SSEOD}/test/dcg/solution/ --beam_width ${BW} --test_training_set 1 --test_test_set 1 --output ${SSEOD}/test/dcg/output/

# test adcg models
 ../build/bin/h2sl-test-process-inference ${SSEOD}/test/adcg/collated/test_*.xml --model adcg --feature_set ${SSEOD}/fs_dcg.xml --solution_directory ${SSEOD}/test/adcg/solution/ --beam_width ${BW} --test_training_set 1 --test_test_set 1 --output ${SSEOD}/test/adcg/output/

# test hdcg models
../build/bin/h2sl-test-process-inference ${SSEOD}/test/hdcg/collated/test_*.xml --model hdcg --feature_set ${SSEOD}/fs_hdcg.xml --solution_directory ${SSEOD}/test/hdcg/solution/ --beam_width ${BW} --test_training_set 1 --test_test_set 1 --output ${SSEOD}/test/hdcg/output/

# test hadcg models
../build/bin/h2sl-test-process-inference ${SSEOD}/test/hadcg/collated/test_*.xml --model hadcg --feature_set ${SSEOD}/fs_hdcg.xml --solution_directory ${SSEOD}/test/hadcg/solution/ --beam_width ${BW} --test_training_set 1 --test_test_set 1 --output ${SSEOD}/test/hadcg/output/

# list results models
echo "listing dcg output results"
ls ${SSEOD}/test/dcg/output/result_*.xml
echo "listing adcg output results"
ls ${SSEOD}/test/adcg/output/result_*.xml
echo "listing hdcg output results"
ls ${SSEOD}/test/hdcg/output/result_*.xml
echo "listing hadcg output results"
ls ${SSEOD}/test/hadcg/output/result_*.xml

