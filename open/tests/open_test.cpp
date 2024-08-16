#include "opencv2/opencv.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <fstream>
#include <regex>
#include <iostream>

#include "logger.hpp"
#include "camera.hpp"
#include "timer.hpp"
#include "mtframe.hpp"

std::ofstream log_file;
bool save_log = false;

// CountCameras test
TEST(CameraTest, CountCamerasTest) {
    int num_cameras = count_cameras();
    EXPECT_GE(num_cameras, 0);
    EXPECT_LE(num_cameras, 10);
}

// ListCameras test
TEST(CameraTest, ListCamerasTest) {
    int num_cameras = count_cameras();
    if (num_cameras > 0) {
        list_cameras(num_cameras);
        SUCCEED();
    }
}

TEST(TimerTest, GetCurrentTimeFormattedTest) {
    std::string formatted_time = getCurrentTimeFormatted();

    // Check that the formatted time matches the expected format
    std::regex date_time_regex(R"(\d{4}-\d{2}-\d{2}-\d{2}-\d{2}-\d{2})");
    EXPECT_TRUE(std::regex_match(formatted_time, date_time_regex));
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
