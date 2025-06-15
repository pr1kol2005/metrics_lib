add_library(lf INTERFACE)
target_include_directories(lf INTERFACE ${SOURCE_DIR})

target_link_libraries(lf
  INTERFACE hazard
)
