add_executable(lock_free_queue_unit unit.cpp)

target_link_libraries(lock_free_queue_unit
  PRIVATE lf gtest_main
)

# add_executable(lock_free_queue_stress unit.cpp)

# target_link_libraries(lock_free_queue_stress
#   PRIVATE lf gtest_main
# )
