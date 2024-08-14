#include <pcap.h>
#include <iostream>

using namespace std;

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    cout << "Pakcet Length: " << pkthdr->len << endl;
    cout << "Captured Packet Length: " << pkthdr->caplen << endl;
    cout << "Packet Time Stamp: " << pkthdr->ts.tv_sec << "." << pkthdr->ts.tv_usec << endl;

    // Packet Data Additional Analyse
    for (unsigned int i = 0; i < pkthdr->len; i++) {
        cout << hex << (int)packet[i];
        if (i % 16 == 15 || i == pkthdr->len - 1) {
            cout << endl;
        } else {
            cout << " ";
        }
    }
}

int main() {
    printf("If code is not working try sudo modeprobe usbmon\n");
    printf("Or try change usbmon number in the code\n");

    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_if_t *interfaces, *device;
    pcap_t *handle;

    if (pcap_findalldevs(&interfaces, error_buffer) == -1) {
         cerr << "Can not find network device: " << error_buffer << endl;
        return 1;
    }

    cout << "Network device list:" << endl;
    for (device = interfaces; device != nullptr; device = device->next) {
        cout << device->name << endl;
    }

    // USB packet capture device (eg: 'usbmon0' or 'usbmon1')
    const char *device_name = "usbmon1";  
    //one interface may have several devices
    //to filter may need vendor id and product id

    // open device
    handle = pcap_open_live(device_name, BUFSIZ, 1, 1000, error_buffer);
    if (handle == nullptr) {
        cerr << "Can not open device: " << error_buffer << endl;
        pcap_freealldevs(interfaces);

        return 1;
    }

    // Start packet capture
    pcap_loop(handle, 0, packet_handler, nullptr);

    // Close the handle
    pcap_close(handle);
    pcap_freealldevs(interfaces);

    return 0;
}
