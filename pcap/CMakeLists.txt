#CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(
    pcaplpcq
    DESCRIPTION "Packet Capture"
    VERSION 0.0.3
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(PCAP REQUIRED libpcap)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    #${PCAP_INCLUDE_DIRS}
)


add_executable(
    pcaplpcq
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pcaplpcq.cpp
)


target_link_libraries(
    pcaplpcq
    PRIVATE
        ${PCAP_LIBRARIES}
		${CMAKE_CURRENT_SOURCE_DIR}/lib/libpcap.so
        # ${LIBPCAP_DIR}/libpcap.so
        # ${LIBPCAP_DIR}/libpcap.so.1.10.4
)

 
#include(gtest.cmake)

#sudo modprobe usbmon
#g++ -o pcaplpcq pcaplpcq.cpp -lpcap
