add_executable(custom example.cpp)

target_link_libraries(custom
  PRIVATE metrics
)
