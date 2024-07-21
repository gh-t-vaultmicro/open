
//g++ -std=c++11 -o open openy.cpp `pkg-config --cflags --libs opencv4`

#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h> // For SetPriorityClass()
#else
#include <unistd.h> // For nice()
#endif

using namespace cv;
using namespace std;
using namespace std::chrono;

#include <iomanip> // For std::put_time
#include <sstream> // For std::stringstream



void list_cameras() {
    cout << "List Cameras\n " << endl;
    for (int device_id = 0; device_id < 3; ++device_id) {
        VideoCapture cap(device_id);
        if (!cap.isOpened()) {
            continue;
        }
        double width = cap.get(CAP_PROP_FRAME_WIDTH);
        double height = cap.get(CAP_PROP_FRAME_HEIGHT);
        double fps = cap.get(CAP_PROP_FPS);
        int fourcc = static_cast<int>(cap.get(CAP_PROP_FOURCC));
        char fourcc_chars[] = {
            static_cast<char>(fourcc & 0XFF),
            static_cast<char>((fourcc >> 8) & 0XFF),
            static_cast<char>((fourcc >> 16) & 0XFF),
            static_cast<char>((fourcc >> 24) & 0XFF),
            '\0'
        };
        cout << "Device ID: " << device_id << endl;
        cout << "Resolution: " << width << "x" << height << endl;
        cout << "FPS: " << fps << endl;
        cout << "FOURCC: " << fourcc_chars << endl << endl;
        cap.release();
    }
}


int main(int argc, char** argv) {

    //recieve vid, pid and endpoint number
    //select fps width height

    //select video type yuy2 mjpeg rgb .., but first show like libuvc.
    //autosetting only change when use ffmpeg 



    //also show show_usb.py written with pyusb.
    //use functions here.

#ifdef _WIN32
    if (!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)) {
        cerr << "Failed to set process priority" << endl;
    }
#else
    if (nice(-10) == -1) {
        perror("nice error");
    }
#endif


    list_cameras();
    int desired_device_id = 0; // Change this to the desired device ID
    VideoCapture cap(desired_device_id); //0 is open the default camera
    if (!cap.isOpened()) { // Check if we succeeded
        cerr << "Error: Could not open video." << endl;
        return -1;
    }

    cap.set(CAP_PROP_FPS, 30);
    cap.set(CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G')); // 추가된 코드

    double fps = cap.get(CAP_PROP_FPS);
    double width = cap.get(CAP_PROP_FRAME_WIDTH);
    double height = cap.get(CAP_PROP_FRAME_HEIGHT);
    cout << "Camera FPS set to: " << fps << endl;
    cout << "Camera resolution set to: " << width << "x" << height << endl;


    int frame_count = 0;
    auto last_time = steady_clock::now();
    int frames_last_second = 0;
    int frame_loss = 0;

    while (true) {
        auto start_time = steady_clock::now();

        Mat frame;
        bool ret = cap.read(frame);
        if (!ret) {
            frame_loss++;
            cerr << "Error: Camera connection lost. Exiting..." << endl;
            break;
        }

        auto read_time = steady_clock::now(); // 프레임 읽기 완료 시간
        frame_count++;
        frames_last_second++;

        auto current_time = steady_clock::now();
        auto elapsed_time = duration_cast<seconds>(current_time - last_time).count();

        if (elapsed_time >= 1) {
            auto now = system_clock::now();
            auto in_time_t = system_clock::to_time_t(now);
            auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

            std::stringstream ss;

#ifdef _WIN32
            tm ltm;
            localtime_s(&ltm, &in_time_t);
            ss << std::put_time(&ltm, "%H:%M:%S");
#else
            tm* ltm = localtime(&in_time_t);
            ss << std::put_time(ltm, "%H:%M:%S");
#endif
            ss << '.' << std::setfill('0') << std::setw(3) << ms.count();

            cout << ss.str()
                << ":  Frames in the last second: " << frames_last_second
                << ", Frame loss: " << frame_loss << endl;
            frames_last_second = 0;
            frame_loss = 0;
            last_time = current_time;
        }

        imshow("USB Video", frame);

        if (waitKey(1) == 'q') {
            cout << "Exit key is pressed. Stopping the video" << endl;
            break;
        }

        auto end_time = steady_clock::now(); // 전체 루프 완료 시간
        auto elapsed_loop_time = duration_cast<microseconds>(end_time - start_time).count();
        auto frame_read_time = duration_cast<microseconds>(read_time - start_time).count();
        auto frame_display_time = duration_cast<microseconds>(end_time - read_time).count();

        cout << "Frame read time: " << frame_read_time << " microseconds" << endl;
        cout << "Frame display time: " << frame_display_time << " microseconds" << endl;

        double sleep_time = max(1.0e6 / fps - elapsed_loop_time, 0.0);
        this_thread::sleep_for(microseconds(static_cast<int>(sleep_time)));
    }


    cap.release();
    destroyAllWindows();

    return 0;
}
