#include <pcap.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdint>
#include <iomanip>

void analyze_packet(const u_char* packet, uint32_t length, std::ofstream& log_file) {
    // USB 패킷 분석 (예: UVC 패킷)
    if (length < 14) {
        log_file << "Packet too short to analyze" << std::endl;
        return;
    }

    // USB 패킷의 특정 필드를 확인하여 UVC 패킷인지 여부를 확인
    uint8_t bmHeaderInfo = packet[1]; // 예시: UVC 헤더 정보 필드
    uint8_t bFrameID = packet[2];     // 예시: UVC 프레임 ID 필드

    // 단순 예시로 UVC 헤더 정보 필드의 특정 값을 확인
    if ((bmHeaderInfo & 0x0F) == 1) {
        log_file << "Identified as UVC packet, Frame ID: " << static_cast<int>(bFrameID) << std::endl;
    } else {
        log_file << "Not identified as UVC packet" << std::endl;
    }
}

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    std::ofstream* log_file = reinterpret_cast<std::ofstream*>(user_data);

    std::time_t raw_time = pkthdr->ts.tv_sec;
    struct tm* timeinfo = localtime(&raw_time);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);

    (*log_file) << "Timestamp: " << time_str << "." << pkthdr->ts.tv_usec << std::endl;
    (*log_file) << "Packet length: " << pkthdr->len << " bytes" << std::endl;
    (*log_file) << "Captured length: " << pkthdr->caplen << " bytes" << std::endl;
    (*log_file) << "Packet data: \n";
    
    // 패킷 데이터를 여러 줄로 포맷하여 가독성 향상
    for (u_int i = 0; i < pkthdr->caplen; ++i) {
        if (i % 16 == 0 && i != 0) {
            (*log_file) << std::endl;
        }
        (*log_file) << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(packet[i]) << " ";
    }
    (*log_file) << std::dec << std::endl;

    analyze_packet(packet, pkthdr->caplen, *log_file);
    (*log_file) << std::endl;
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *interfaces, *device;

    // 디바이스 목록 가져오기
    if (pcap_findalldevs(&interfaces, errbuf) == -1) {
        std::cerr << "Error finding devices: " << errbuf << std::endl;
        return 1;
    }

    // 디바이스 목록 출력
    int i = 0;
    for (device = interfaces; device != NULL; device = device->next) {
        std::cout << ++i << ": " << (device->name ? device->name : "No name") << std::endl;
        if (device->description)
            std::cout << " (" << device->description << ")" << std::endl;
    }

    // 사용할 디바이스 번호 입력
    int dev_num;
    std::cout << "Enter the number of the device to use: \n";
    std::cout << "usbmonX (X is the number of the USB bus to monitor) \n" << std::endl;

    std::cin >> dev_num;

    // 선택한 디바이스 열기
    for (device = interfaces, i = 0; i < dev_num - 1; device = device->next, ++i);

    pcap_t *handle = pcap_open_live(device->name, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        std::cerr << "Error opening device: " << errbuf << std::endl;
        pcap_freealldevs(interfaces);
        return 1;
    }

    // 디바이스 목록 해제
    pcap_freealldevs(interfaces);

    std::ofstream log_file("usb_camera_packets.log", std::ios::out | std::ios::app);
    if (!log_file.is_open()) {
        std::cerr << "Error opening log file" << std::endl;
        pcap_close(handle);
        return 1;
    }

    // 패킷 캡처 시작
    pcap_loop(handle, 0, packet_handler, reinterpret_cast<u_char*>(&log_file));

    // 파일 및 pcap 핸들 닫기
    log_file.close();
    pcap_close(handle);

    return 0;
}
