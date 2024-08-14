#ifndef TIMER_HPP
#define TIMER_HPP

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;
using namespace chrono;

extern bool save_log;
extern ofstream log_file;

void log_with_time(const string& message);
string getCurrentTimeFormatted();

#endif // TIMER_HPP
