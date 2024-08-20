#ifndef INTERPRETE_HPP
#define INTERPRETE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>
#include <map>

using namespace std;

void parse_file(const string& file_path);
vector<string> split(const string& s, const string& delimiter);
vector<pair<string, string>> parse_packet_data(const string& packet_data);