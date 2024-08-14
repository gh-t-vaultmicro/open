#CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(
    openmt
    DESCRIPTION "Capturing Frames"
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENCV REQUIRED opencv4)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(
    openmt
    ${CMAKE_CURRENT_SOURCE_DIR}/src/openmt.cpp
)

target_link_libraries(
    openmt
    PRIVATE
        #${OPENCV_LIBRARIES}
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_core.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_imgproc.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_highgui.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_imgcodecs.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_videoio.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_video.so 

)

#include(gtest.cmake)