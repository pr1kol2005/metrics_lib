add_executable(lock_free_stack_unit unit.cpp)

target_link_libraries(lock_free_stack_unit
  PRIVATE lf gtest_main
)

# add_test(NAME LockFreeStackUnit COMMAND lock_free_stack_unit)
