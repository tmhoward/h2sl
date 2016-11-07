result=${PWD##*/}
if [ $result == "build" ]
then
  ./bin/h2sl-region-abstract-container-demo --input ../data/validation/symbols_test.xml --output /tmp/region_abstract_container_demo.xml
else
  echo "RUN THIS SCRIPT FROM ~/.../h2sl/build/"
fi
