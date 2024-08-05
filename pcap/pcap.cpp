#include <pcap.h>
#include <iostream>

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    std::cout << "패킷 길이: " << pkthdr->len << std::endl;
    std::cout << "캡처된 패킷 길이: " << pkthdr->caplen << std::endl;
    std::cout << "패킷 타임스탬프: " << pkthdr->ts.tv_sec << "." << pkthdr->ts.tv_usec << std::endl;

    // 패킷 데이터를 여기에 추가 분석
    for (unsigned int i = 0; i < pkthdr->len; i++) {
        std::cout << std::hex << (int)packet[i];
        if (i % 16 == 15 || i == pkthdr->len - 1) {
            std::cout << std::endl;
        } else {
            std::cout << " ";
        }
    }
}

int main() {
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_if_t *interfaces, *device;
    pcap_t *handle;

    // 네트워크 인터페이스 목록을 가져옴
    if (pcap_findalldevs(&interfaces, error_buffer) == -1) {
        std::cerr << "네트워크 장치를 찾을 수 없음: " << error_buffer << std::endl;
        return 1;
    }

    std::cout << "사용 가능한 네트워크 장치 목록:" << std::endl;
    for (device = interfaces; device != nullptr; device = device->next) {
        std::cout << device->name << std::endl;
    }

    // USB 패킷을 캡처할 장치를 선택 (예: 'usbmon0' 또는 다른 적절한 이름)
    const char *device_name = "usbmon0";  // 이 이름은 실제 시스템에 따라 다를 수 있음

    // 장치 열기
    handle = pcap_open_live(device_name, BUFSIZ, 1, 1000, error_buffer);
    if (handle == nullptr) {
        std::cerr << "장치를 열 수 없음: " << error_buffer << std::endl;
        return 1;
    }

    // 패킷 캡처 시작
    pcap_loop(handle, 0, packet_handler, nullptr);

    // 자원 해제
    pcap_close(handle);
    pcap_freealldevs(interfaces);

    return 0;
}
