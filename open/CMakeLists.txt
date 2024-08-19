#CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(
    open_d
    DESCRIPTION "Capturing Frames"
    VERSION 0.1.0
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

# find_package(PkgConfig REQUIRED)
# pkg_check_modules(OPENCV REQUIRED opencv4)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${googletest_SOURCE_DIR}/googlemock/include)
include_directories(${googletest_SOURCE_DIR}/googletest/include)

add_library(
    open_lib
    ${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mtframe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/timer.cpp
)

add_executable(
    open_d
    ${CMAKE_CURRENT_SOURCE_DIR}/src/open.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mtframe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/timer.cpp
)

target_link_libraries(
    open_d
    PRIVATE
        #${OPENCV_LIBRARIES}
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_core.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_imgproc.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_highgui.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_imgcodecs.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_videoio.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libtbb.so.12
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libQt5Test.so.5
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libQt5OpenGL.so.5
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgdcmMSFF.so.3.0
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgdal.so.34
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgdcmDSED.so.3.0


)


#test code

#open_test

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(
    open_test
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/open_test.cpp
)

target_link_libraries(
    open_test
    PRIVATE
        gtest_main
        gmock_main
        open_lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_core.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_imgproc.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_highgui.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_imgcodecs.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencv_videoio.so
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libtbb.so.12
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libQt5Test.so.5
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libQt5OpenGL.so.5
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgdcmMSFF.so.3.0
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgdal.so.34
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgdcmDSED.so.3.0

)


# #capture_test
# add_executable(
#     capture_test
#     ${CMAKE_CURRENT_SOURCE_DIR}/tests/capture_test.cpp
# )

# target_link_libraries(
#     capture_test
#     gtest_main
#     gmock_main
#     open_lib
#     ${OPENCV_LIBRARIES}
# )

include(GoogleTest)
gtest_discover_tests(open_test)
