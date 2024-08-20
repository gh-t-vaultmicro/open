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

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)


#find_package(PkgConfig REQUIRED)
#pkg_check_modules(PCAP REQUIRED libpcap)
set(PCAP_LIBRARIES
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libpcap.so
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libpcap.so.1.10.4
)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${googletest_SOURCE_DIR}/googlemock/include)
include_directories(${googletest_SOURCE_DIR}/googletest/include)


add_executable(
    pcaplpcq
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pcaplpcq.cpp
)


target_link_libraries(
    pcaplpcq
    PRIVATE
        ${PCAP_LIBRARIES}
		#${CMAKE_CURRENT_SOURCE_DIR}/lib/libpcap.so
)

add_executable(
    interpreter
    ${CMAKE_CURRENT_SOURCE_DIR}/src/interpreter.cpp
)


#test code

#

# add_executable(
#     packet_test 
#     ${CMAKE_CURRENT_SOURCE_DIR}/tests/packet_test.cpp
# )

# target_link_libraries(
#     packet_test
#     PRIVATE
#         gtest_main
#         gmock_main
#         ${PCAP_LIBRARIES}
# )
 
# include(GoogleTest)
# gtest_discover_tests(packet_test)



#sudo apt-get install libpcap-dev
#sudo modprobe usbmon
#g++ -o pcaplpcq pcaplpcq.cpp -lpcap
