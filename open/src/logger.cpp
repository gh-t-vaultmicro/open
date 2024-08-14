#include "logger.hpp"

using namespace std;

void log_message(const string& message) {
    cout << message << endl;

    if (save_log) {
        log_file << message << endl;
    }
}

void log_error(const string& error_message) {
    cerr << error_message << endl;

    if (save_log) {
        log_file << error_message << endl;
    }
}