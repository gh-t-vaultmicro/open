#CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(
    pcapl
    DESCRIPTION "Packet Capture"
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PCAP REQUIRED libpcap)

# add_library(
#     dlib SHARED 
#     ${CMAKE_SOURCE_DIR}/src/hello.cpp 
# )

add_executable(
    pcapl
    ${CMAKE_SOURCE_DIR}/src/pcapl.cpp
)


include_directories(
    #${CMAKE_SOURCE_DIR}/include
    ${PCAP_INCLUDE_DIRS}
)

target_link_libraries(
    pcapl
    PRIVATE
        #dlib
        ${PCAP_LIBRARIES}
		#${CMAKE_SOURCE_DIR}/lib/opencv.lib
)


#include(gtest.cmake)

#sudo modprobe usbmon
#g++ -o pcapl pcapl.cpp -lpcap