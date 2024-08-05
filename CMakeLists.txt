cmake_minimum_required(VERSION 3.10)

# project name, version, language
project(
    open
    LANGUAGES CXX
    VERSION 0.0.0
)

#Include executable
add_executable(
    OpenCVCapture ${SOURCE_FILES}
    add_executable(OpenCVCapture ${SOURCE_FILES})

)

#Include library
add_library(
    target_link_libraries(OpenCVCapture ${OpenCV_LIBS})
)



# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})



set(SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/open.c
	#${CMAKE_SOURCE_DIR}/src/pcap.c
)

# Source file
set(SOURCE_FILES main.cpp)

