#include <pcap.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdint>
#include <iomanip>

using namespace std;

void analyze_packet(const u_char* packet, uint32_t length, ofstream& log_file) {
    // Analyze USB packet (e.g., UVC packet)
    if (length < 14) {
        log_file << "Packet too short to analyze" << endl;
        return;
    }

    // Check specific fields in the USB packet to determine if it is a UVC packet
    uint8_t bmHeaderInfo = packet[1]; // Example: UVC header information field
    uint8_t bFrameID = packet[2];     // Example: UVC frame ID field

    // Simple example of checking a specific value in the UVC header information field
    if ((bmHeaderInfo & 0x0F) == 1) {
        log_file << "Identified as UVC packet, Frame ID: " << static_cast<int>(bFrameID) << endl;
    } else {
        log_file << "Not identified as UVC packet" << endl;
    }
}

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    ofstream* log_file = reinterpret_cast<ofstream*>(user_data);

    time_t raw_time = pkthdr->ts.tv_sec;
    struct tm* timeinfo = localtime(&raw_time);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);

    (*log_file) << "Timestamp: " << time_str << "." << pkthdr->ts.tv_usec << endl;
    (*log_file) << "Packet length: " << pkthdr->len << " bytes" << endl;
    (*log_file) << "Captured length: " << pkthdr->caplen << " bytes" << endl;
    (*log_file) << "Packet data: \n";
    
    // Format packet data into multiple lines for better readability
    for (u_int i = 0; i < pkthdr->caplen; ++i) {
        if (i % 16 == 0 && i != 0) {
            (*log_file) << endl;
        }
        (*log_file) << hex << setw(2) << setfill('0') << static_cast<int>(packet[i]) << " ";
    }
    (*log_file) << dec << endl;

    analyze_packet(packet, pkthdr->caplen, *log_file);
    (*log_file) << endl;
}

int main() {
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_if_t *interfaces, *device;

    // Get the list of devices
    if (pcap_findalldevs(&interfaces, error_buffer) == -1) {
        cerr << "Error finding devices: " << error_buffer << endl;
        return 1;
    }

    // Print the list of devices
    int i = 0;
    for (device = interfaces; device != NULL; device = device->next) {
        cout << ++i << ": " << (device->name ? device->name : "No name") << endl;
        if (device->description)
            cout << " (" << device->description << ")" << endl;
    }

    // Input the number of the device to use
    int dev_num;
    cout << "Enter the number of the device to use: \n";
    cout << "usbmonX (X is the number of the USB bus to monitor) \n" << endl;

    cin >> dev_num;

    // Open the selected device
    for (device = interfaces, i = 0; i < dev_num - 1; device = device->next, ++i);

    pcap_t *handle = pcap_open_live(device->name, BUFSIZ, 1, 1000, error_buffer);
    if (handle == NULL) {
        cerr << "Error opening device: " << error_buffer << endl;
        pcap_freealldevs(interfaces);
        return 1;
    }

    // Free the device list
    pcap_freealldevs(interfaces);

    ofstream log_file("usb_camera_packets.log", ios::out | ios::app);
    if (!log_file.is_open()) {
        cerr << "Error opening log file" << endl;
        pcap_close(handle);
        return 1;
    }

    // Start capturing packets
    pcap_loop(handle, 0, packet_handler, reinterpret_cast<u_char*>(&log_file));

    // Close the log file and pcap handle
    log_file.close();
    pcap_close(handle);

    return 0;
}
