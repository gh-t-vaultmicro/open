#include "mtframe.hpp"
#include "timer.hpp"
#include "logger.hpp"

void capture_frames(VideoCapture& cap, int fps) {

    while (!stop_capture) {
        Mat frame;
        bool ret = cap.read(frame);
        if (!ret) {
            log_error("Error: Camera connection lost. Exiting...");
            break;
        }


        {
            lock_guard<mutex> lock(frame_mutex);
            frame_queue.push(frame);
        }
        frame_cv.notify_one();
    }
}

void process_frames(int fps, const string& video_name, const string& log_name, const string& frame_dir, int width, int height) {
    int frame_count = 0;
    int frames_last_second = 0;
    int frame_loss = 0;
    auto last_time = steady_clock::now();

    FILE* ffmpeg = nullptr;
    if (save_video){
        stringstream ffmpeg_cmd;
        ffmpeg_cmd << "ffmpeg -y -loglevel debug -f rawvideo -pixel_format bgr24 -video_size " 
                   << width << "x" << height
                   << " -r " << fps 
                   << " -i - -c:v libx264 -pix_fmt yuv420p " 
                   << video_name << " 2> " << log_name;

        ffmpeg = popen(ffmpeg_cmd.str().c_str(), "w");
        if (!ffmpeg) {
            log_error("Error: Could not open FFmpeg.");
            return;
        }
    }

    while (!stop_capture || !frame_queue.empty()) {
        Mat frame;
        auto start_time = steady_clock::now();
        
        {
            unique_lock<mutex> lock(frame_mutex);
            frame_cv.wait(lock, []{ return !frame_queue.empty() || stop_capture; });
            if (frame_queue.empty()) {
                continue;
            }
            frame = frame_queue.front();
            frame_queue.pop();
        }

        if(save_video){
            fwrite(frame.data, 1, frame.total() * frame.elemSize(), ffmpeg);
        }

        if (save_picture){
            stringstream frame_path_ss;
            frame_path_ss << frame_dir << "/frame_" << setfill('0') << setw(6) << frame_count << ".jpg";
            imwrite(frame_path_ss.str(), frame);
        }

        frame_count++;
        frames_last_second++;

        auto current_time = steady_clock::now();
        auto elapsed_time = duration_cast<seconds>(current_time - last_time).count();

        if (verbose) {
            auto end_time = steady_clock::now();
            auto elapsed_loop_time = duration_cast<microseconds>(end_time - start_time).count();

            stringstream ss;
            ss << elapsed_loop_time << "ms" << ", Total frame: " << frame_count;
            log_with_time(ss.str());
        }

        if (elapsed_time >= 1) {
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
            stop_capture = true;
            break;
        }
    }

    if (save_video){
        pclose(ffmpeg);
    }
}
