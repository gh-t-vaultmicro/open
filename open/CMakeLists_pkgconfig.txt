#CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(
    open
    DESCRIPTION "Capturing Frames"
    VERSION 0.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENCV REQUIRED opencv4)

# add_library(
#     dlib SHARED 
#     ${CMAKE_SOURCE_DIR}/src/hello.cpp 
# )

add_executable(
    open
    ${CMAKE_SOURCE_DIR}/src/open.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
	${OPENCV_INCLUDE_DIRS}
)

target_link_libraries(
    open
    PRIVATE
        #dlib
        ${OPENCV_LIBRARIES}
		#${CMAKE_SOURCE_DIR}/lib/opencv.lib
)

include(gtest.cmake)