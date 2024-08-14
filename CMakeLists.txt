#CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(
    synch
    DESCRIPTION "Synch Process"
    VERSION 0.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


add_executable(
    synch 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/synch.cpp
)

