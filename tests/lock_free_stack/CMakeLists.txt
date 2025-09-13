add_executable(lock_free_stack_unit unit.cpp)

target_link_libraries(lock_free_stack_unit
  PRIVATE lf gtest_main
)

add_executable(lock_free_stack_stress stress.cpp)

target_link_libraries(lock_free_stack_stress
  PRIVATE lf gtest_main
)

gtest_discover_tests(lock_free_stack_unit)
gtest_discover_tests(lock_free_stack_stress)
