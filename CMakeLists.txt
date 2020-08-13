cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

# set the project name
project(Unit-Value VERSION 0.1.0)

add_definitions(-DUV_USE_LONG_DOUBLE)

add_library(UValue
    SHARED
    include/UValue.hpp
    include/Conversions.h
    src/Conversions.cpp   
    src/UValue.cpp
)

target_include_directories(
    UValue
    PRIVATE
    ./include
)

# add the executable
add_executable(UnitsExample
    examples/UnitsExample.cpp
)

target_include_directories(
    UnitsExample
    PRIVATE
    ./include
)

add_dependencies(UnitsExample
    UValue
)

target_link_libraries(UnitsExample
    PRIVATE
    UValue
)

add_test(
  NAME
    TestMapping
  COMMAND
    "${CMAKE_CTEST_COMMAND}"
             --build-and-test "${My_SOURCE_DIR}/tests/TestMapping.cpp"
                              "${CMAKE_CURRENT_BINARY_DIR}/build"
             --build-generator "${CMAKE_GENERATOR}"
             --test-command "${CMAKE_CTEST_COMMAND}"
)