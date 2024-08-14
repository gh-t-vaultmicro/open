#include "timer.hpp"

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
    if (save_log){
        log_file << ss.str() << ": " << message << endl;
    }
}

string getCurrentTimeFormatted() {
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%Y-%m-%d-%H-%M-%S");
    return oss.str();
}
