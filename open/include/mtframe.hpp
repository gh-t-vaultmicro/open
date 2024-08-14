#ifndef MTFRAME_HPP
#define MTFRAME_HPP

#include "opencv2/opencv.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <queue>
#include <mutex>
#include <condition_variable>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace cv;
using namespace std;
using namespace chrono;

extern bool verbose;
extern bool save_video;
extern bool save_picture;
extern bool save_log;
extern bool stop_capture;

extern ofstream log_file;
extern mutex frame_mutex;
extern condition_variable frame_cv;
extern queue<Mat> frame_queue;

void capture_frames(VideoCapture& cap, int fps);
void process_frames(int fps, const string& video_name, const string& log_name, const string& frame_dir, int width, int height);

#endif // MTFRAME_HPP
