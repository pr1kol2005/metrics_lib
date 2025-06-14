add_executable(metrics_unit unit.cpp)

target_link_libraries(metrics_unit
  PRIVATE metrics gtest_main
)

add_executable(metrics_stress stress.cpp)

target_link_libraries(metrics_stress
  PRIVATE metrics gtest_main
)
