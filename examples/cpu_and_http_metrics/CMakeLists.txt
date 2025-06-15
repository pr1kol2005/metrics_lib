add_executable(cpu_http example.cpp)

target_link_libraries(cpu_http
  PRIVATE metrics
)
