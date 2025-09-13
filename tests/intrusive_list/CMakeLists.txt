add_executable(intrusive_list_unit unit.cpp)

target_link_libraries(intrusive_list_unit
  PRIVATE lf gtest_main
)

gtest_discover_tests(intrusive_list_unit)
