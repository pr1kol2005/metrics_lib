add_executable(unit unit.cpp)

target_link_libraries(unit
  PRIVATE lf gtest gtest_main
)

add_test(NAME UnitTests COMMAND unit)
