#ifndef PCAPLPCQ_HPP
#define PCAPLPCQ_HPP

#include "pcap.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <cstdint>
#include <sstream>
#include <filesystem>
#include <cstring>
#include <chrono>
#include <csignal>

using namespace std;
namespace fs = std::filesystem;

// Global handle for pcap, so it can be closed on exit
// Global log file for the same reason
pcap_t *handle = nullptr; 
ofstream log_file;        

// Global counters for packets and lengths
unsigned int packet_count = 0;
unsigned long long total_packet_length = 0;
unsigned long long total_captured_length = 0;


void clean_exit(int signum);
string getCurrentTimeFormatted();
void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet);
