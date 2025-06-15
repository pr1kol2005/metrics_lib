add_library(metrics INTERFACE)
target_include_directories(metrics INTERFACE ${SOURCE_DIR})

target_link_libraries(metrics
  INTERFACE lf
)
