result=${PWD##*/}
if [ $result == "build" ]
then
  ./bin/h2sl-index-demo --input ../data/validation/symbols_test.xml --output /tmp/index_demo_out.xml
else
  echo "RUN THIS SCRIPT FROM ~/.../h2sl/build/"
fi
