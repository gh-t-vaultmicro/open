#include <iostream>
#include <pcap.h>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <cstdint>
#include <sstream>
#include <filesystem>
#include <cstring>

using namespace std;
namespace fs = std::filesystem;

string getCurrentTimeFormatted() {
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%Y-%m-%d-%H-%M-%S");
    return oss.str();
}

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    ofstream *log_file = reinterpret_cast<ofstream*>(user_data);

    time_t raw_time = pkthdr->ts.tv_sec;
    struct tm* timeinfo = localtime(&raw_time);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);

    *log_file << "Packet Time Stamp: " << pkthdr->ts.tv_sec << "." << pkthdr->ts.tv_usec << endl;
    *log_file << "Timestamp: " << time_str << "." << pkthdr->ts.tv_usec << endl;
    *log_file << "Packet Length: " << pkthdr->len << " bytes" << endl;
    *log_file << "Captured Packet Length: " <<  pkthdr->caplen << " bytes" << endl;

    cout << "Timestamp: " << time_str << "." << pkthdr->ts.tv_usec << endl; 
    cout << "Packet Length: " << pkthdr->len << " bytes" << endl;
    cout << "Captured Packet Length: " << pkthdr->caplen << " bytes" << endl;
    cout << "Packet Time Stamp: " << pkthdr->ts.tv_sec << "." << pkthdr->ts.tv_usec << endl;

    for (u_int i = 0; i < pkthdr->caplen; ++i) {
        if (i % 16 == 0 && i != 0) {
            *log_file << endl;
        }
        *log_file << hex << setw(2) << setfill('0') << static_cast<int>(packet[i]) << " ";
    }

    *log_file << dec << endl; // Reset to decimal format, necessary for the time stamp
}

string getLogFilePath(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "-d") != 0) {
        cerr << "Usage: " << argv[0] << " -d usbmonX" << endl;
        return "";
    }

    string selected_device = argv[2];
    cout << "If code is not working try sudo modeprobe usbmon" << endl;
    cout << endl;

    string current_time_str = getCurrentTimeFormatted();

    string base_path = "./";
    string log_dir = base_path + "../log";
    if (!fs::exists(log_dir)) {
        fs::create_directory(log_dir);
    }

    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_if_t *interfaces, *device;

    if (pcap_findalldevs(&interfaces, error_buffer) == -1) {
        cerr << "Error finding Device: " << error_buffer << endl;
        return "";
    }

    for (device = interfaces; device != nullptr; device = device->next) {
        if (selected_device == device->name) {
            break;
        }
    }

    if (device == nullptr) {
        cerr << "Error: Device " << selected_device << " not found" << endl;
        pcap_freealldevs(interfaces);
        return "";
    }

    pcap_t *handle = pcap_open_live(device->name, BUFSIZ, 1, 1000, error_buffer);
    if (handle == NULL) {
        cerr << "Error opening device: " << error_buffer << endl;
        pcap_freealldevs(interfaces);
        return "";
    }

    string log_path = log_dir + "/log_pcap_" + current_time_str + ".txt";

    ofstream log_file;
    log_file.open(log_path, ios::out);
    if (!log_file) {
        cerr << "Failed to open log file" << endl;
        return "";
    }
    log_file << "Log file created" << endl;

    pcap_freealldevs(interfaces);

    pcap_loop(handle, 0, packet_handler, reinterpret_cast<u_char*>(&log_file));

    pcap_close(handle);
    log_file.close();

    return log_path;
}

int main(int argc, char *argv[]) {
    string log_path = getLogFilePath(argc, argv);
    
    if (!log_path.empty()) {
        cout << "Log file created at: " << log_path << endl;
    } else {
        cerr << "Failed to create log file." << endl;
        return 1;
    }

    return 0;
}
