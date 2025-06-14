#!/bin/bash

# ./tests/run_tests.sh --output-xml

GTEST_OUTPUT_XML=0

if [[ "$1" == "--output-xml" ]]; then
  GTEST_OUTPUT_XML=1
fi

if [[ "$GTEST_OUTPUT_XML" == 1 ]]; then
  mkdir -p tests/reports
fi

if [[ "$GTEST_OUTPUT_XML" == 1 ]]; then
  ./bin/unit --gtest_output=xml:tests/reports/unit_results.xml
else
  ./bin/unit
fi

exit 0
