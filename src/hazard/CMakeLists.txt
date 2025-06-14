add_library(hazard STATIC
  hazard.cpp
)

target_include_directories(hazard
  PUBLIC ${SOURCE_DIR}
)

target_link_libraries(hazard
  PUBLIC utils
)
