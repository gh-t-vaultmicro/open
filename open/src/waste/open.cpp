//open.cpp
#include "opencv2/opencv.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <getopt.h>


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
ofstream log_file;

void show_usage(){
    cout << "----------------------------Usage---------------------------- " << endl;
    cout << "[-d device_id] [-f fps] [-c codec] [-w width] [-h height] [-v verbose]" << endl;
    cout << "e.g.) sudo ./open -d 0 -f 24 -w 1920 -h 1080 -v " << endl;
    cout << "build) cmake .. // make " << endl;
    cout << "build) g++ -std=c++11 -o open open.cpp `pkg-config --cflags --libs opencv4` " << endl;
    cout << "./open to show usage " << endl;
    cout << "Only detect less than 10 cameras " << endl;
    cout << "Igore the errors shown at the very first, -no device found " << endl;
    cout << "This shows the streaming screen and saves on file using ffmpeg " << endl;
    cout << "Each frames are captured on picture_frame " << endl;
    cout << "If opencv is not found, try sudo apt install libopencv-dev" << endl;
}

int count_cameras() {
    int num_cameras = 0;
    for (int device_id = 0; device_id < 10; ++device_id) {
        VideoCapture cap(device_id);
        if (cap.isOpened()) {
            num_cameras++;
            cap.release();
        } else {
            return num_cameras;
        }
    }
    cout << "Camera limit 10 exceeded" << endl;
    log_file << "Camera limit 10 exceeded" << endl;
    return num_cameras;
}

void list_cameras(int num_cameras) {
    cout << "List Cameras\n " << endl;
    log_file << "List Cameras\n " << endl;
    for (int device_id = 0;  device_id < num_cameras; ++device_id) {
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
        log_file << "Device ID: " << device_id << endl;
        log_file << "Resolution: " << width << "x" << height << endl;
        log_file << "FPS: " << fps << endl;
        log_file << "FOURCC: " << fourcc_chars << endl << endl;
        cap.release();
    }
}

void log_with_time(const string& message) {
    auto now = system_clock::now();
    auto in_time_t = system_clock::to_time_t(now);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    stringstream ss;
#ifdef _WIN32
    tm ltm;
    localtime_s(&ltm, &in_time_t);
    ss << std::put_time(&ltm, "%H:%M:%S");
#else
    tm* ltm = localtime(&in_time_t);
    ss << std::put_time(ltm, "%H:%M:%S");
#endif
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    cout << ss.str() << ": " << message << endl;
    log_file << ss.str() << ": " << message << endl;
}


string getCurrentTimeFormatted() {
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%Y-%m-%d-%H-%M-%S");
    return oss.str();
}

int main(int argc, char** argv) {
    string base_path = "./";
    string current_time_str = getCurrentTimeFormatted();
    string log_path = base_path + "../log/log_" + current_time_str + ".txt";
    log_file.open(log_path);

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
    while ((opt = getopt(argc, argv, "d:f:c:w:h:v")) != -1) {
        switch (opt) {
            case 'd': desired_device_id = stoi(optarg); break;
            case 'f': fps = stoi(optarg); break;
            case 'c': codec = stoi(optarg); break;
            case 'p': pixel_format = stoi(optarg); break;
            case 'w': width = stoi(optarg); break;
            case 'h': height = stoi(optarg); break;
            case 'v': verbose = true; break;
            default:
                cerr << "Usage: " << argv[0] << " [-d device_id] [-f fps] [-c codec] [-w width] [-h height] [-v verbose]" << endl;
                log_file << "Usage: " << argv[0] << " [-d device_id] [-f fps] [-c codec] [-w width] [-h height] [-v verbose]" << endl;
                return -1;
        }
    }

    if (desired_device_id == -1 && fps == -1 && codec ==-1 && width == -1 && height == -1) {
        int num_cameras = count_cameras();
        show_usage();
        list_cameras(num_cameras);
        return 0;
    }

    VideoCapture cap(desired_device_id); // Open the selected camera
    if (!cap.isOpened()) { // Check if we succeeded
        cerr << "Error: Could not open video." << endl;
        log_file << "Error: Could not open video." << endl;
        return -1;
    }

    cap.set(CAP_PROP_FPS, fps);
    cap.set(CAP_PROP_FRAME_WIDTH, width);
    cap.set(CAP_PROP_FRAME_HEIGHT, height);
    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));

    double set_fps = cap.get(CAP_PROP_FPS);
    double set_width = cap.get(CAP_PROP_FRAME_WIDTH);
    double set_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    cout << endl << "Camera FPS set to: " << set_fps << endl;
    cout << "Camera resolution set to: " << set_width << "x" << set_height << endl << endl;
    log_file << endl << "Camera FPS set to: " << set_fps << endl;
    log_file << "Camera resolution set to: " << set_width << "x" << set_height << endl << endl;

    int frame_count = 0;
    auto last_time = steady_clock::now();
    int frames_last_second = 0;
    int frame_loss = 0;
    double sleep_time_per_frame = 1.0e6 / fps;

    string log_name = base_path + "../log/ffmpeg_log_" + current_time_str + ".txt";
    string video_name = base_path + "../video/output_" + current_time_str + "." + video_format;
    string frame_dir = base_path + "../picture_frame/" + current_time_str;

    // Create directory for frames
    fs::create_directories(frame_dir);
    
    //FFMPEG COMMAND
    // Start FFmpeg process with log level set to error and log output redirected to a file
    stringstream ffmpeg_cmd;

    ffmpeg_cmd << "ffmpeg -y -loglevel debug -f rawvideo -pixel_format bgr24 -video_size " 
               << width << "x" << height
               << " -r " << fps 
               << " -i - -c:v libx264 -pix_fmt yuv420p " 
               << video_name << " 2> " << log_name;

    FILE* ffmpeg = popen(ffmpeg_cmd.str().c_str(), "w");
    if (!ffmpeg) {
        cerr << "Error: Could not open FFmpeg." << endl;
        log_file << "Error: Could not open FFmpeg." << endl;
        return -1;
    }


    while (true) {
        auto start_time = steady_clock::now();

        Mat frame;
        bool ret = cap.read(frame);
        if (!ret) {
            frame_loss++;
            cerr << "Error: Camera connection lost. Exiting..." << endl;
            log_file << "Error: Camera connection lost. Exiting..." << endl;
            break;
        }

        // Write frame to FFmpeg
        fwrite(frame.data, 1, frame.total() * frame.elemSize(), ffmpeg);

        // Save frame as JPEG
        stringstream frame_path_ss;
        frame_path_ss << frame_dir << "/frame_" << setfill('0') << setw(6) << frame_count << ".jpg";
        imwrite(frame_path_ss.str(), frame);

        frame_count++;
        frames_last_second++;

        auto current_time = steady_clock::now();
        auto elapsed_time = duration_cast<seconds>(current_time - last_time).count();

        if (elapsed_time >= 1 || frames_last_second >= fps) {

            stringstream ss;
            ss << "Frames in the last second: " << frames_last_second 
            << ", Frame loss: " << frame_loss 
            << ", Total frame: " << frame_count;
            log_with_time(ss.str());



            frames_last_second = 0;
            frame_loss = 0;
            last_time = current_time;
        }

        imshow("USB Video", frame);

        if (waitKey(1) == 'q') {
            log_with_time("Exit key is pressed. Stopping the video");
            break;
        }

        if (verbose) {
            auto end_time = steady_clock::now();
            auto elapsed_loop_time = duration_cast<microseconds>(end_time - start_time).count();

            stringstream ss;
            ss << elapsed_loop_time << "ms" << ", Total frame: " << frame_count;
            log_with_time(ss.str());
        }
    }

    cap.release();
    destroyAllWindows();
    pclose(ffmpeg);
    log_file.close();

    return 0;
}

