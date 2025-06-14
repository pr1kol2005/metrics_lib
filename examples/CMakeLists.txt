add_executable(cpu_http cpu_http.cpp)

target_link_libraries(cpu_http
  PRIVATE metrics
)

# add_executable(custom custom.cpp)

# target_link_libraries(custom
#   PRIVATE metrics
# )
