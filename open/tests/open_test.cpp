#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

bool CaptureFrame(cv::VideoCapture& cap, cv::Mat& frame) {
    cap >> frame;
    return !frame.empty();
}

TEST(VideoCaptureTest, FrameCapture) {
    cv::VideoCapture cap(0);

    ASSERT_TRUE(cap.isOpened()) << "Failed to open the camera.";

    cv::Mat frame;

    ASSERT_TRUE(CaptureFrame(cap, frame)) << "Failed to capture frame.";
    EXPECT_FALSE(frame.empty()) << "Captured frame is empty.";

    EXPECT_GT(frame.rows, 0) << "Frame has zero rows.";
    EXPECT_GT(frame.cols, 0) << "Frame has zero columns.";
    EXPECT_EQ(frame.type(), CV_8UC3) << "Unexpected frame type, expected CV_8UC3 for a color image.";

    cv::imshow("Test Frame", frame);
    cv::waitKey(1);

    cap.release();
    cv::destroyAllWindows();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
