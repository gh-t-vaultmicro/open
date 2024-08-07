#include <pcap.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>

using namespace std;

// Function to analyze the packet
void analyze_packet(const u_char* packet, uint32_t length, ofstream& log_file) {
    // Analyze USB packet (example: UVC packet)
    if (length < 14) {
        log_file << "Packet too short to analyze" << endl;
        return;
    }

    // Check specific fields of the USB packet to identify UVC packet
    uint8_t bmHeaderInfo = packet[1]; // Example: UVC header info field
    uint8_t bFrameID = packet[2];     // Example: UVC frame ID field

    // Simple example to check UVC header info field
    if ((bmHeaderInfo & 0x0F) == 1) {
        log_file << "Identified as UVC packet, Frame ID: " << (int)bFrameID << endl;
    } else {
        log_file << "Not identified as UVC packet" << endl;
    }
}

// Packet handler callback function
void packet_handler(u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    ofstream* log_file = reinterpret_cast<ofstream*>(user);

    time_t raw_time = pkthdr->ts.tv_sec;
    struct tm* timeinfo = localtime(&raw_time);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);

    *log_file << "Timestamp: " << time_str << "." << pkthdr->ts.tv_usec << endl;
    *log_file << "Packet length: " << pkthdr->len << " bytes" << endl;
    *log_file << "Captured length: " << pkthdr->caplen << " bytes" << endl;

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

    analyze_packet(packet, pkthdr->caplen, *log_file);
    *log_file << endl;
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs, *device;

    // Retrieve the device list
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        cerr << "Error finding devices: " << errbuf << endl;
        return 1;
    }

    // Print the list of devices
    int i = 0;
    for (device = alldevs; device != NULL; device = device->next) {
        cout << ++i << ": " << (device->name ? device->name : "No name") << endl;
        if (device->description)
            cout << " (" << device->description << ")" << endl;
    }

    // Ask user which device to use
    int dev_num;
    cout << "Enter the number of the device to use: ";
    cin >> dev_num;

    // Select the device
    for (device = alldevs, i = 0; i < dev_num - 1; device = device->next, ++i);

    // Open the device for capturing
    pcap_t *handle = pcap_open_live(device->name, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        cerr << "Error opening device: " << errbuf << endl;
        pcap_freealldevs(alldevs);
        return 1;
    }

    // Free the device list
    pcap_freealldevs(alldevs);

    ofstream log_file("usb_camera_packets.log", ios::out | ios::app);
    if (!log_file.is_open()) {
        cerr << "Error opening log file" << endl;
        pcap_close(handle);
        return 1;
    }

    // Start packet capturing
    pcap_loop(handle, 0, packet_handler, reinterpret_cast<u_char*>(&log_file));

    // Close the file and pcap handle
    log_file.close();
    pcap_close(handle);

    return 0;
}
