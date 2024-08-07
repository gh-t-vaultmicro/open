#include <pcap.h>
#include <iostream>

int main(int argc, char *argv[]) {
    pcap_t *handle;                // Session handle
    char errbuf[PCAP_ERRBUF_SIZE]; // Error string
    const u_char *packet;          // The actual packet
    struct pcap_pkthdr header;     // The header that pcap gives us
    int packet_number = 0;         // Packet counter

    // Check for proper usage
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <pcap file>" << std::endl;
        return 2;
    }

    // Open the pcap file
    handle = pcap_open_offline(argv[1], errbuf);
    if (handle == NULL) {
        std::cerr << "Couldn't open pcap file " << argv[1] << ": " << errbuf << std::endl;
        return 2;
    }

    // Read packets from the file
    while ((packet = pcap_next(handle, &header)) != NULL) {
        packet_number++;
        std::cout << "Packet #" << packet_number << ": length " << header.len << std::endl;
    }

    // Close the session
    pcap_close(handle);
    return 0;
}
