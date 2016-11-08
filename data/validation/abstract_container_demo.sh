result=${PWD##*/}
if [ $result == "build" ]
then
  ./bin/h2sl-abstract-container-demo --input ../data/validation/symbols_test.xml --output /tmp/abstract_container_demo_output.xml
else
  echo "RUN THIS SCRIPT FROM ~/.../h2sl/build/"
fi
