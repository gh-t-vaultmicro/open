//pcaplp.cpp
#include <pcap.h>
#include <iostream>
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
    

    // Print packet information to log file and terminal
    *log_file << "Packet Time Stamp: " << pkthdr->ts.tv_sec << "." << pkthdr->ts.tv_usec << endl;
    *log_file << "Timestamp: " << time_str << "." << pkthdr->ts.tv_usec << endl;
    *log_file << "Packet Length: " << pkthdr->len << " bytes" << endl;
    *log_file << "Captured Packet Length: " <<  pkthdr->caplen << " bytes" << endl;
    
    cout << "Timestamp: " << time_str << "." << pkthdr->ts.tv_usec << endl; 
    cout << "Packet Length: " << pkthdr->len << " bytes" << endl;
    cout << "Captured Packet Length: " << pkthdr->caplen << " bytes" << endl;
    cout << "Packet Time Stamp: " << pkthdr->ts.tv_sec << "." << pkthdr->ts.tv_usec << endl;

    // // Convert packet data to hexadecimal and print to log file and terminal
    // // Use to print binary data on terminal window
    // for (u_int i = 0; i < pkthdr->len; i++) {
    //     *log_file << hex << setw(2) << setfill('0') << (int)packet[i];
    //     cout << hex << setw(2) << setfill('0') << (int)packet[i];

    //     if (i % 16 == 15 || i == pkthdr->len - 1) {
    //         *log_file << endl;
    //         cout << endl;
    //     } else {
    //         *log_file << " ";
    //         cout << " ";
    //     }
    // }

    // Format packet data into multiple lines for better readability
    for (u_int i = 0; i < pkthdr->caplen; ++i) {
        if (i % 16 == 0 && i != 0) {
            *log_file << endl;
        }
        *log_file << hex << setw(2) << setfill('0') << static_cast<int>(packet[i]) << " ";
    }

    *log_file << dec << endl; // Reset to decimal format, nessary for the time stamp

}

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "-d") != 0) {
        cerr << "Usage: " << argv[0] << " -d usbmonX" << endl;
        return 1;
    }

    string selected_device = argv[2];
    cout << "If code is not working try sudo modeprobe usbmon" << endl;
    cout << endl;

    // Get current time formatted string
    string current_time_str = getCurrentTimeFormatted();

    // Ensure the log directory exists
    string base_path = "./";
    string log_dir = base_path + "../log";
    if (!fs::exists(log_dir)) {
        fs::create_directory(log_dir);
    }

    // Find Devices
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_if_t *interfaces, *device;

    if (pcap_findalldevs(&interfaces, error_buffer) == -1) {
        cerr << "Error finding Device: " << error_buffer << endl;
        return 1;
    }

    // Print the list of devices
    int i = 0;
    for (device = interfaces; device != nullptr; device = device->next) {
        cout << ++i << ": " << (device->name ? device->name : "No name") << endl;
        if (device->description)
            cout << " (" << device->description << ")" << endl;
    }
    
    // int dev_num;
    // cout << "Enter the number of the device to use: \n";
    // cout << "usbmonX (X is the number of the USB bus to monitor) \n" << endl;
    // cout << "Type lsusb in terminal to see the USB bus number \n" << endl;
    // cin >> dev_num;

    // //Open the selected device
    // for (device = interfaces, i = 0; i < dev_num - 1; device = device->next, ++i);

        // Find the specified device
    for (device = interfaces; device != nullptr; device = device->next) {
        if (selected_device == device->name) {
            break;
        }
    }

    if (device == nullptr) {
        cerr << "Error: Device " << selected_device << " not found" << endl;
        pcap_freealldevs(interfaces);
        return 1;
    }


    pcap_t *handle = pcap_open_live(device->name, BUFSIZ, 1, 1000, error_buffer);
    if (handle == NULL) {
        cerr << "Error opening device: " << error_buffer << endl;
        pcap_freealldevs(interfaces);
        return 1;
    }
    
    //Log file path
    string log_path = log_dir + "/log_pcap_" + current_time_str + ".txt";

    // Open log file
    ofstream log_file;
    log_file.open(log_path, ios::out);
    if (!log_file) {
        cerr << "Failed to open log file" << endl;
        return 1;
    }
    log_file << "Log file created" << endl;

    // Free the device list
    pcap_freealldevs(interfaces);

    // Start packet capture
    pcap_loop(handle, 0, packet_handler, reinterpret_cast<u_char*>(&log_file));

    // Close the handle and free the device list
    pcap_close(handle);
    log_file.close();

    return 0;
}
