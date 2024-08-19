#include <opencv2/opencv.hpp>

int main() {
    // 웹캠에서 비디오 캡처 객체 생성 (기본 카메라 사용)
    cv::VideoCapture cap(0);

    // 캡처 객체가 열리지 않았을 경우 종료
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open the camera." << std::endl;
        return -1;
    }

    // 무한 루프를 통해 프레임을 계속해서 읽고 화면에 출력
    while (true) {
        cv::Mat frame;
        // 비디오 프레임 읽기
        cap >> frame;

        // 프레임이 비어 있으면 루프 종료
        if (frame.empty()) {
            break;
        }

        // 프레임을 화면에 표시
        cv::imshow("Video Stream", frame);

        // 'q' 키를 누르면 종료
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // 캡처 객체 및 모든 창 닫기
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
