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
  ./bin/intrusive_list_unit --gtest_output=xml:tests/reports/intrusive_list_unit_results.xml
  ./bin/lock_free_queue_unit --gtest_output=xml:tests/reports/lock_free_queue_unit_results.xml
  # ./bin/lock_free_queue_stress --gtest_output=xml:tests/reports/lock_free_queue_stress_results.xml
  ./bin/lock_free_stack_unit --gtest_output=xml:tests/reports/lock_free_stack_unit_results.xml
  ./bin/lock_free_stack_stress --gtest_output=xml:tests/reports/lock_free_stack_stress_results.xml
else
  ./bin/intrusive_list_unit
  ./bin/lock_free_queue_unit
  # ./bin/lock_free_queue_stress
  ./bin/lock_free_stack_unit
  ./bin/lock_free_stack_stress
fi

exit 0
