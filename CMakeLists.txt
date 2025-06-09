cmake_minimum_required(VERSION 3.20)
project(exam_project)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF)

include(cmake/sanitizers.cmake)
include(cmake/doctest.cmake)
include(cmake/benchmark.cmake)

include_directories(include)
add_library(symbol_table INTERFACE)

enable_testing()

add_subdirectory(source)
add_subdirectory(tests)


add_executable(main main.cpp)
# target_link_libraries(main PRIVATE symbol_table) 


