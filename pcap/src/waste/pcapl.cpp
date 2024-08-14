#include <pcap.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>

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
    //pkthdr: packet header
    ofstream *log_file = reinterpret_cast<ofstream*>(user_data);

    // Print packet information to log file and terminal
    *log_file << "Packet Length: " << pkthdr->len << endl;
    *log_file << "Captured Packet Length: " << pkthdr->caplen << endl;
    *log_file << "Packet Time Stamp: " << pkthdr->ts.tv_sec << "." << pkthdr->ts.tv_usec << endl;

    cout << "Packet Length: " << pkthdr->len << endl;
    cout << "Captured Packet Length: " << pkthdr->caplen << endl;
    cout << "Packet Time Stamp: " << pkthdr->ts.tv_sec << "." << pkthdr->ts.tv_usec << endl;

    // Convert packet data to hexadecimal and print to log file and terminal
    for (unsigned int i = 0; i < pkthdr->len; i++) {
        *log_file << hex << setw(2) << setfill('0') << (int)packet[i];
        cout << hex << setw(2) << setfill('0') << (int)packet[i];

        if (i % 16 == 15 || i == pkthdr->len - 1) {
            *log_file << endl;
            cout << endl;
        } else {
            *log_file << " ";
            cout << " ";
        }
    }
}

int main() {
    cout << "If code is not working try sudo modeprobe usbmon" << endl;
    cout << "Or try change usbmon number in the code" << endl;
    cout << endl;

    // Get current time formatted string
    string current_time_str = getCurrentTimeFormatted();

    // Ensure the log directory exists
    string base_path = "./";
    string log_dir = base_path + "../log";
    if (!fs::exists(log_dir)) {
        fs::create_directory(log_dir);
    }

    //Log file path
    string log_path = log_dir + "/log_pcap" + current_time_str + ".txt";

    // Open log file
    ofstream log_file;
    log_file.open(log_path, ios::out);
    if (!log_file) {
        cerr << "Failed to open log file" << endl;
        return 1;
    }
    log_file << "Log file created" << endl;


    // Find network devices
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_if_t *interfaces, *device;
    pcap_t *handle;

    if (pcap_findalldevs(&interfaces, error_buffer) == -1) {
        cerr << "Cannot find network device: " << error_buffer << endl;
        log_file.close();
        return 1;
    }

    cout << "Network device list:" << endl;
    for (device = interfaces; device != nullptr; device = device->next) {
        cout << device->name << endl;
    }

    // USB packet capture device (e.g., 'usbmon0' or 'usbmon1')
    const char *device_name = "usbmon1";  

    // Open device
    handle = pcap_open_live(device_name, BUFSIZ, 1, 1000, error_buffer);
    if (handle == nullptr) {
        cerr << "Cannot open device: " << error_buffer << endl;
        pcap_freealldevs(interfaces);
        log_file.close();
        return 1;
    }

    // Start packet capture
    pcap_loop(handle, 0, packet_handler, reinterpret_cast<u_char*>(&log_file));

    // Close the handle and free the device list
    pcap_close(handle);
    pcap_freealldevs(interfaces);
    log_file.close();

    return 0;
}
