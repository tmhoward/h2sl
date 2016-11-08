result=${PWD##*/}
if [ $result == "build" ]
then
  ./bin/h2sl-constraint-demo --input ../data/validation/symbols_test.xml --output /tmp/constraint_out.xml
else
  echo "RUN THIS SCRIPT FROM ~/.../h2sl/build/"
fi
