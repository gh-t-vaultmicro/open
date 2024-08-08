#include <pcap.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdint>

void analyze_packet(const u_char* packet, uint32_t length, std::ofstream& log_file) {
    // USB 패킷 분석 (예: UVC 패킷)
    // USB 헤더를 파싱하는 기본 로직을 추가해야 합니다.
    // 이 예제에서는 USB 패킷의 특정 오프셋에서 데이터를 읽어 UVC 패킷을 식별합니다.
    
    if (length < 14) {
        log_file << "Packet too short to analyze" << std::endl;
        return;
    }

    // USB 패킷의 특정 필드를 확인하여 UVC 패킷인지 여부를 확인
    // UVC 패킷의 일반적인 형식에 따라 필드를 파싱해야 합니다.
    uint8_t bmHeaderInfo = packet[1]; // 예시: UVC 헤더 정보 필드 // bullshit
    uint8_t bFrameID = packet[2];     // 예시: UVC 프레임 ID 필드 // bullshit

    // 여기서는 단순히 예시로 UVC 헤더 정보 필드의 특정 값을 확인합니다.
    if ((bmHeaderInfo & 0x0F) == 1) {
        log_file << "Identified as UVC packet, Frame ID: " << (int)bFrameID << std::endl;
    } else {
        log_file << "Not identified as UVC packet" << std::endl;
    }
}

void packet_handler(u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    std::ofstream* log_file = reinterpret_cast<std::ofstream*>(user);
    
    std::time_t raw_time = pkthdr->ts.tv_sec;
    struct tm* timeinfo = localtime(&raw_time);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);

    (*log_file) << "Timestamp: " << time_str << "." << pkthdr->ts.tv_usec << std::endl;
    (*log_file) << "Packet length: " << pkthdr->len << " bytes" << std::endl;
    (*log_file) << "Captured length: " << pkthdr->caplen << " bytes" << std::endl;
    (*log_file) << "Packet data: ";
    for (u_int i = 0; i < pkthdr->caplen; ++i) {
        (*log_file) << std::hex << (int)packet[i] << " ";
    }
    (*log_file) << std::dec << std::endl;

    analyze_packet(packet, pkthdr->caplen, *log_file);
    (*log_file) << std::endl;
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs, *device;

    // Retrieve the device list
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::cerr << "Error finding devices: " << errbuf << std::endl;
        return 1;
    }

    // Print the list of devices
    int i = 0;
    for (device = alldevs; device != NULL; device = device->next) {
        std::cout << ++i << ": " << (device->name ? device->name : "No name") << std::endl;
        if (device->description)
            std::cout << " (" << device->description << ")" << std::endl;
    }

    // Ask user which device to use
    int dev_num;
    std::cout << "Enter the number of the device to use: ";
    std::cin >> dev_num;

    // Select the device
    for (device = alldevs, i = 0; i < dev_num - 1; device = device->next, ++i);

    // Open the device for capturing
    pcap_t *handle = pcap_open_live(device->name, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        std::cerr << "Error opening device: " << errbuf << std::endl;
        pcap_freealldevs(alldevs);
        return 1;
    }

    // Free the device list
    pcap_freealldevs(alldevs);

    std::ofstream log_file("usb_camera_packets.log", std::ios::out | std::ios::app);
    if (!log_file.is_open()) {
        std::cerr << "Error opening log file" << std::endl;
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
