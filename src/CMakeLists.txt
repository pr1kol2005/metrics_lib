# add_subdirectory(another_dir)

# можно объединить в одну библиотеку:
# add_library(some_lib INTERFACE)

# target_include_directories(some_lib INTERFACE
#   ${CMAKE_CURRENT_SOURCE_DIR}/some_dir
# )

# target_link_libraries(db_lib INTERFACE
#   some_dir
# )
