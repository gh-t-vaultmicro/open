#include "opencv2/opencv.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "camera.hpp"
#include "timer.hpp"
#include "mtframe.hpp"
#include "logger.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace cv;
using namespace std;
using namespace chrono;

#ifdef __linux__
namespace fs = std::filesystem;
#elif _WIN32
namespace fs = std::__fs::filesystem;
#elif __APPLE__
namespace fs = std::__fs::filesystem;
#else
#error "Unknown or unsupported operating system"
#endif

bool verbose = false;
bool save_video = false;
bool save_picture = false;
bool save_log = false;
bool stop_capture = false;

ofstream log_file;
mutex frame_mutex;
condition_variable frame_cv;

//Mat is the basic image container in OpenCV
//Mat is a matrix data structure that stores the image data
//Use to transfer data between the threads
queue<Mat> frame_queue;

void show_usage(){
    cout << "----------------------------Usage---------------------------- " << endl;
    std::cout << "[-d device_id] [-f fps] [-c codec] [-w width] [-h height] [-m videofile] [-s picturefile] [-l logfile] [-v verbose]" << std::endl;
    cout << "e.g.) sudo ./open -d 0 -f 30 -w 640 -h 480 -m 1 -s 1 -l 1 -v " << endl;
    cout << "build) cmake .. // make " << endl;
    cout << "build) g++ -std=c++11 -o openmt openmt.cpp `pkg-config --cflags --libs opencv4` " << endl;
    cout << "./open to show usage " << endl;
    cout << "Only detect less than 10 cameras " << endl;
    cout << "Ignore the errors shown at the very first, -no device found " << endl;
    cout << "This shows the streaming screen and saves on file using ffmpeg " << endl;
    cout << "Each frame is captured on picture_frame " << endl;
    cout << "If opencv is not found, try sudo apt install libopencv-dev" << endl;
}

int main(int argc, char** argv) {

#ifdef _WIN32
    if (!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)) {
        cerr << "Failed to set process priority" << endl;
    }
#else
    if (nice(-10) == -1) {
        perror("nice error");
    }
#endif

    int desired_device_id = -1;
    int fps = -1;
    int codec = -1;
    int width = -1;
    int height = -1;

    string pixel_format = "bgr24";
    string video_format = "mp4";

    // Command line options
    int opt;
    while ((opt = getopt(argc, argv, "d:f:c:w:h:vm:s:l:")) != -1) {
        switch (opt) {
            case 'd': desired_device_id = stoi(optarg); break;
            case 'f': fps = stoi(optarg); break;
            case 'c': codec = stoi(optarg); break;
            case 'p': pixel_format = stoi(optarg); break;
            case 'w': width = stoi(optarg); break;
            case 'h': height = stoi(optarg); break;
            case 'm': save_video = stoi(optarg) != 0; break;
            case 's': save_picture = stoi(optarg) != 0; break;
            case 'l': save_log = stoi(optarg) != 0; break;
            case 'v': verbose = true; break;
            default:
                //log_error("Usage: " + string(argv[0]) + " [-d device_id] [-f fps] [-c codec] [-w width] [-h height] [-m save_video] [-s save_picture] [-l save_log] [-v verbose]");
                return -1;
        }
    }

    if (desired_device_id == -1 && fps == -1 && codec ==-1 && width == -1 && height == -1) {
        int num_cameras = count_cameras();
        show_usage();
        list_cameras(num_cameras);
        return 0;
    }

    string base_path = "./";
    string current_time_str = getCurrentTimeFormatted();
    string log_path = base_path + "../log/log_" + current_time_str + ".txt";
    if (save_log) {
        log_file.open(log_path);
    }


    // Camera settings
    VideoCapture cap(desired_device_id); // Open the selected camera
    if (!cap.isOpened()) { // Check if we succeeded
        log_error("Error: Could not open video.");
        return -1;
    }

    cap.set(CAP_PROP_FPS, fps);
    cap.set(CAP_PROP_FRAME_WIDTH, width);
    cap.set(CAP_PROP_FRAME_HEIGHT, height);
    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));

    double set_fps = cap.get(CAP_PROP_FPS);
    double set_width = cap.get(CAP_PROP_FRAME_WIDTH);
    double set_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    stringstream ss;
    ss << endl << "Camera FPS set to: " << set_fps << endl;
    ss << "Camera resolution set to: " << set_width << "x" << set_height << endl << endl;
    log_message(ss.str());

    string log_name = base_path + "../log/ffmpeg_log_" + current_time_str + ".txt";
    string video_name = base_path + "../video/output_" + current_time_str + "." + video_format;
    string frame_dir = base_path + "../picture_frame/" + current_time_str;

    // Create directory for frames
    if (save_picture) {
        fs::create_directories(frame_dir);
    }


    // Using Multi-threads
    // Start the capture and processing threads
    thread capture_thread(capture_frames, ref(cap), fps);
    thread processing_thread(process_frames, fps, video_name, log_name, frame_dir, width, height);


    // Wait for threads to finish
    capture_thread.join();
    processing_thread.join();

    cap.release();
    destroyAllWindows();
    if (save_log){
        log_file.close();
    }

    return 0;
}
