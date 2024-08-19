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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${googletest_SOURCE_DIR}/googlemock/include)
include_directories(${googletest_SOURCE_DIR}/googletest/include)

# Define the OpenCV and related libraries as a variable
set(OPENCV_LIBRARIES
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
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgdcmDICT.so.3.0
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgdcmjpeg8.so.3.0
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgdcmjpeg12.so.3.0
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgdcmjpeg16.so.3.0
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcharls.so.2
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgdcmIOD.so.3.0
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgdcmCommon.so.3.0
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libodbc.so.2
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libodbcinst.so.2
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libblosc.so.1
    #${CMAKE_CURRENT_SOURCE_DIR}/lib/libarmadillo.so.12
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libqhull_r.so.8.0
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libxerces-c-3.2.so
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgeotiff.so.5
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libaec.so.0
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libnetcdf.so.19
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcfitsio.so.10
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libhdf5_serial.so.103
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libpq.so.5
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libkmlbase.so.1
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libkmldom.so.1
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libkmlengine.so.1
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libfyba.so.0
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libspatialite.so.8
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libmysqlclient.so.21
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libfreexl.so.1
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libogdi.so.4.1
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgeos_c.so.1
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libproj.so.25
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libdfalt.so.0
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libmfhdfalt.so.0
)

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
        ${OPENCV_LIBRARIES}
)

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
        ${OPENCV_LIBRARIES}
)

include(GoogleTest)
gtest_discover_tests(open_test)
