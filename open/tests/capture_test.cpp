#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "mtframe.hpp"
#include "timer.hpp"
#include "logger.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace testing;
using namespace cv;
using namespace std;

bool stop_capture;
queue<Mat> frame_queue;
mutex frame_mutex;
condition_variable frame_cv;

// Mock Logger function
void log_error(const std::string& msg) {
}

// Mock VideoCapture class
class MockVideoCapture {
public:
    MOCK_METHOD(bool, read, (Mat&), ());
};

// Test fixture class
class CaptureFramesTest : public ::testing::Test {
protected:
    MockVideoCapture mock_cap;

    void SetUp() override {
        stop_capture = false;
    }

    void TearDown() override {
        while (!frame_queue.empty()) {
            frame_queue.pop();
        }
        stop_capture = true;
    }
};

TEST_F(CaptureFramesTest, CapturesFramesCorrectly) {
    EXPECT_CALL(mock_cap, read(_))
        .WillOnce(DoAll(SetArgReferee<0>(Mat(100, 100, CV_8UC3, Scalar(0, 0, 0))), Return(true)))
        .WillOnce(Return(false));  // Simulate end of capture

    std::thread capture_thread(capture_frames, std::ref(mock_cap), 30);

    {
        std::unique_lock<std::mutex> lock(frame_mutex);
        frame_cv.wait_for(lock, std::chrono::seconds(1), []{ return !frame_queue.empty(); });
    }

    ASSERT_FALSE(frame_queue.empty());
    Mat captured_frame = frame_queue.front();
    ASSERT_EQ(captured_frame.rows, 100);
    ASSERT_EQ(captured_frame.cols, 100);
    ASSERT_EQ(captured_frame.type(), CV_8UC3);

    capture_thread.join();
}

TEST_F(CaptureFramesTest, StopsOnError) {
    EXPECT_CALL(mock_cap, read(_))
        .WillOnce(Return(false));  // Simulate camera connection lost

    std::thread capture_thread(capture_frames, std::ref(mock_cap), 30);

    capture_thread.join();

    ASSERT_TRUE(frame_queue.empty());  // No frames should be captured
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}