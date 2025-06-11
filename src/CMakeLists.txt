# add_subdirectory(another_dir)

# add_executable(main_app main.cpp)

# target_link_libraries(main_app PRIVATE
#   another_dir)

# target_include_directories(main_app PRIVATE
#   ${CMAKE_SOURCE_DIR}/src
# )

# можно объединить в одну библиотеку:
# add_library(some_lib INTERFACE)

# target_include_directories(some_lib INTERFACE
#   ${CMAKE_CURRENT_SOURCE_DIR}/some_dir
# )

# target_link_libraries(db_lib INTERFACE
#   some_dir
# )
