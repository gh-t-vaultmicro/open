//synch_logger.hpp
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

std::ofstream log_file("shared_log.txt");

void log_with_time(const std::string &message) {
    auto now = std::chrono::steady_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << now_ms.count();
    ss << " - " << message << std::endl;

    log_file << ss.str();
    log_file.flush();
}
