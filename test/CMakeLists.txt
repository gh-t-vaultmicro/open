cmake_minimum_required(VERSION 3.20)
project(opentest name>)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(opentest
        opentest.cpp
)

target_link_libraries(<executable name> ${OpenCV_LIBS})
