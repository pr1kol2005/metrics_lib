#!/bin/bash

if [ -n "$GTEST_OUTPUT_XML" ]; then
	mkdir -p tests/reports
fi

# GTEST_OUTPUT_XML=1 ./tests/run_tests.sh

if [ -n "$GTEST_OUTPUT_XML" ]; then
  ./bin/unit --gtest_output=xml:tests/reports/unit_results.xml
else
  ./bin/unit
fi
