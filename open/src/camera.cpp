#include "camera.hpp"
#include "logger.hpp"

using namespace cv;
using namespace std;

extern ofstream log_file;
extern bool save_log;


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
    log_message("Camera limit 10 exceeded");
    return num_cameras;
}

void list_cameras(int num_cameras) {
    log_message("List Cameras\n");

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
        
        stringstream ss;
        ss << "Device ID: " << device_id << endl;
        ss << "Resolution: " << width << "x" << height << endl;
        ss << "FPS: " << fps << endl;
        ss << "FOURCC: " << fourcc_chars << endl << endl;

        log_message(ss.str());

        cap.release();
    }
}
