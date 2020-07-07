cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

# set the project name
project(Unit-Value VERSION 0.1.0)

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