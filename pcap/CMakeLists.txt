#CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(
    pcaplpc
    DESCRIPTION "Packet Capture"
    VERSION 0.0.2
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
    pcaplpc
    ${CMAKE_SOURCE_DIR}/src/pcaplpc.cpp
)


include_directories(
    #${CMAKE_SOURCE_DIR}/include
    ${PCAP_INCLUDE_DIRS}
)

target_link_libraries(
    pcaplpc
    PRIVATE
        #dlib
        ${PCAP_LIBRARIES}
		#${CMAKE_SOURCE_DIR}/lib/opencv.lib
)


#include(gtest.cmake)

#sudo modprobe usbmon
#g++ -o pcaplpc pcaplpc.cpp -lpcap