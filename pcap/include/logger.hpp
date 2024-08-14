#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <string>

extern std::ofstream log_file;
extern bool save_log;

void log_message(const std::string& message);
void log_error(const std::string& error_message);

#endif // LOGGER_HPP
