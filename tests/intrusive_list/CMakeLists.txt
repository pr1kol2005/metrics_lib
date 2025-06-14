add_executable(intrusive_list_unit unit.cpp)

target_link_libraries(intrusive_list_unit
  PRIVATE lf gtest_main
)