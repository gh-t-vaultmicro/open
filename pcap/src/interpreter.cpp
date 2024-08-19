#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>

using namespace std;

// Function prototypes
vector<pair<string, string>> parse_packet_data(const string& packet_data);
vector<string> split(const string& s, const string& delimiter);

void parse_file(const string& file_path) {
    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << file_path << endl;
        return;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();

    // Capture Statistics Parsing
    size_t capture_stats_pos = content.find("Capture Statistics:");
    map<string, int> capture_stats;

    if (capture_stats_pos != string::npos) {
        string stats_section = content.substr(capture_stats_pos);
        content = content.substr(0, capture_stats_pos);
        regex stats_regex(R"((\w[\w\s]+): (\d+))");
        smatch match;
        string::const_iterator search_start(stats_section.cbegin());
        while (regex_search(search_start, stats_section.cend(), match, stats_regex)) {
            capture_stats[match[1]] = stoi(match[2]);
            search_start = match.suffix().first;
        }
    }

    vector<string> packets = split(content, "Packet Time Stamp:");

    string log_file_path = file_path.substr(0, file_path.find_last_of(".")) + "_parsed_log.txt";
    ofstream log_file(log_file_path);

    int operation_now_in_progress_count = 0;
    int no_such_file_count = 0;
    int success_count = 0;

    int iso_success_count = 0;
    int iso_fail_count = 0;
    vector<pair<string, string>> no_such_file_packets;
    vector<string> iso_fail_packets;

    for (const string& packet : packets) {
        vector<string> lines = split(packet, "\n");
        if (lines.size() < 9) {
            continue;  // Skip incomplete packets
        }

        string timestamp = lines[1].substr(lines[1].find(": ") + 2);
        string chrono_time = lines[2].substr(lines[2].find(": ") + 2);
        string packet_data = lines[5] + lines[6] + lines[7] + lines[8];
        packet_data.erase(remove(packet_data.begin(), packet_data.end(), ' '), packet_data.end());

        vector<pair<string, string>> parsed_data = parse_packet_data(packet_data);

        string urb_transfer_type = parsed_data[2].second;
        string urb_status = parsed_data[10].second;

        // Count URB statuses
        if (urb_transfer_type == "00 (ISO)" || urb_transfer_type == "03 (BULK)" || urb_transfer_type == "01 (INTERRUPT)" || urb_transfer_type == "02 (CONTROL)") {
            if (urb_status == "8dffffff (Operation Now in Progress)") {
                operation_now_in_progress_count++;
            } else if (urb_status == "feffffff (No Such File or Directory)") {
                no_such_file_count++;
                no_such_file_packets.push_back(make_pair(timestamp, chrono_time));
            } else if (urb_status == "00000000 (SUCCESS)") {
                cout << "SUCCESS found, increasing count" << endl;
                success_count++;
            }
        }

        // Analyze ISO Descriptors
        if (urb_transfer_type == "00 (ISO)") {
            // Parsing ISO Descriptors (if any)
            for (const auto& kv : parsed_data) {
                if (kv.first == "ISO Descriptor Number") {
                    string iso_desc_data = packet_data.substr(128);
                    for (size_t i = 0; i < stoi(kv.second, nullptr, 16); i++) {
                        string iso_status = iso_desc_data.substr(i * 32, 8);
                        if (iso_status == "00000000") {
                            iso_success_count++;
                        } else {
                            iso_fail_count++;
                            iso_fail_packets.push_back(packet);
                        }
                    }
                }
            }
        }

        log_file << "Packet:      " << timestamp << "\n";
        log_file << "Chrono Time: " << chrono_time << "\n";
        for (const auto& kv : parsed_data) {
            log_file << kv.first << ": " << kv.second << "\n";
        }
        log_file << "\n";
    }

    log_file << "--------------------------------------------------------------------------------\n";

    int total_urb = no_such_file_count + success_count;

        // Log URB status counts
    log_file << "Total URBs to consider (excluding 'Operation Now in Progress'): " << total_urb << "\n";
    log_file << "SUCCESS Count: " << success_count << "\n";
    log_file << "No Such File or Directory Count: " << no_such_file_count << "\n";


    log_file << "Operation Now in Progress excluded from the percentage calculation \n";
    log_file << "SUCCESS: " << (success_count * 100.0 / total_urb) << "%\n";
    log_file << "No Such File or Directory: " << (no_such_file_count * 100.0 / total_urb) << "%\n";

    // Log No Such File or Directory packet timestamps
    log_file << "\nNo Such File or Directory Logged Time:\n";
    for (const auto& packet_time : no_such_file_packets) {
        log_file << "Packet Time Stamp: " << packet_time.first << ", Chrono Time: " << packet_time.second << "\n";
    }

    // Log ISO Descriptor success/failure rates
    if (iso_success_count + iso_fail_count > 0) {
        log_file << "\nISO Descriptor Success Rate: " << (iso_success_count * 100.0 / (iso_success_count + iso_fail_count)) << "%\n";
        log_file << "ISO Descriptor Failure Rate: " << (iso_fail_count * 100.0 / (iso_success_count + iso_fail_count)) << "%\n";
    }

    // Log failed ISO packets
    log_file << "\nFailed ISO Packets:\n";
    for (const string& packet : iso_fail_packets) {
        log_file << "Packet: " << packet.substr(0, packet.find('\n')) << "\n";
    }

    // Log Capture Statistics
    if (!capture_stats.empty()) {
        log_file << "\nCapture Statistics:\n";
        for (const auto& stat : capture_stats) {
            log_file << stat.first << ": " << stat.second << "\n";
        }
    }

    cout << "Parsed data has been saved to " << log_file_path << endl;
}

// Utility function to split a string based on a delimiter
vector<string> split(const string& s, const string& delimiter) {
    vector<string> tokens;
    size_t start = 0, end = 0;
    while ((end = s.find(delimiter, start)) != string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(s.substr(start));
    return tokens;
}

// Function to parse packet data
vector<pair<string, string>> parse_packet_data(const string& packet_data) {
    string urb_id = packet_data.substr(0, 16);
    string urb_type = packet_data.substr(16, 2);
    string urb_transfer_type = packet_data.substr(18, 2);
    string endpoint = packet_data.substr(20, 2);
    string device_number = packet_data.substr(22, 2);
    string urb_bus_id = packet_data.substr(24, 4);
    string device_setup_request = packet_data.substr(28, 2);
    string data_present = packet_data.substr(30, 2);
    string urb_status = packet_data.substr(56, 8);
    string urb_length = packet_data.substr(64, 8);
    string data_length = packet_data.substr(72, 8);
    string setup_header = packet_data.substr(80, 16);
    string interval = packet_data.substr(96, 8);
    string start_of_frame = packet_data.substr(104, 8);
    string copy_of_transfer_flag = packet_data.substr(112, 8);
    string iso_descriptor_number = packet_data.substr(120, 8);

    vector<pair<string, string>> parsed_data = {
        {"URB ID", urb_id},
        {"URB Type", (urb_type == "53") ? "53 (SUBMIT)" : 
                    "43 (COMPLETE)"},
        {"URB Transfer Type", (urb_transfer_type == "00") ? "00 (ISO)" :
                             (urb_transfer_type == "01") ? "01 (INTERRUPT)" :
                             (urb_transfer_type == "02") ? "02 (CONTROL)" : 
                             "03 (BULK)"},
        {"Endpoint", (endpoint == "80") ? "80 (IN Endpoint 0)" :
                     (endpoint == "81") ? "81 (IN Endpoint 1)" :
                     (endpoint == "83") ? "83 (IN Endpoint 3)" : 
                     "84 (IN Endpoint 4)"},
        {"Device Number", device_number},
        {"URB Bus ID", urb_bus_id},
        {"Device Setup Request", device_setup_request},
        {"Data Present", data_present},
        {"URB Status", (urb_status == "8dffffff") ? "8dffffff (Operation Now in Progress)" :
                       (urb_status == "feffffff") ? "feffffff (No Such File or Directory)" : 
                       "00000000 (SUCCESS)"},
        {"URB Length", urb_length},
        {"Data Length", data_length},
        {"Setup Header", setup_header},
        {"Interval", interval},
        {"Start of Frame", start_of_frame},
        {"Copy of Transfer Flag", copy_of_transfer_flag},
        {"ISO Descriptor Number", iso_descriptor_number},
    };

    return parsed_data;
}

int main() {
    string log_file_path;
    cout << "Type path to your file: ";
    getline(cin, log_file_path);
    if (log_file_path.empty()) {
        cout << "No path provided. Using default path." << endl;
        cout << "Showing example" << endl;
        log_file_path = "../log/log_pcap_example.txt";
    }

    parse_file(log_file_path);

    return 0;
}
