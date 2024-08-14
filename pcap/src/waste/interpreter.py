import re
import os

def parse_packet_data(packet_data):
    # Split the data into the necessary fields
    urb_id = packet_data[0:16]
    urb_type = packet_data[16:18]
    urb_transfer_type = packet_data[18:20]
    endpoint = packet_data[20:22]
    device_number = packet_data[22:24]
    urb_bus_id = packet_data[24:28]
    device_setup_request = packet_data[28:30]
    data_present = packet_data[30:32]
    urb_second = packet_data[32:48]
    urb_usecond = packet_data[48:56]
    urb_status = packet_data[56:64]
    urb_length = packet_data[64:72]
    data_length = packet_data[72:80]
    setup_header = packet_data[80:96]
    interval = packet_data[96:104]
    start_of_frame = packet_data[104:112]
    copy_of_transfer_flag = packet_data[112:120]
    iso_descriptor_number = packet_data[120:128]
    
    # Convert URB Second and URB Usecond to decimal
    urb_second_decimal = int(urb_second, 16)
    urb_usecond_decimal = int(urb_usecond, 16)

    # Interpret URB Type
    urb_type_interpreted = {
        "53": "53 (SUBMIT)",
        "43": "43 (SUCCESS)"
    }.get(urb_type, urb_type)

 # Interpret URB Transfer Type
    urb_transfer_type_interpreted = {
        "00": "00 (ISO)",
        "01": "01 (INTERRUPT)",
        "02": "02 (CONTROL)",
        "03": "03 (BULK)"
    }.get(urb_transfer_type, urb_transfer_type)

    # Interpret Endpoint
    endpoint_interpreted = {
        "80": "80 (IN Endpoint 0)",
        "81": "81 (IN Endpoint 1)",
        "83": "83 (IN Endpoint 3)",
        "84": "84 (IN Endpoint 4)",
        "00": "00 (OUT Endpoint 0)",
        "01": "01 (OUT Endpoint 1)",
        "02": "02 (OUT Endpoint 2)",
        "03": "03 (OUT Endpoint 3)",
        "04": "04 (OUT Endpoint 4)",
    }.get(endpoint, endpoint)

    # Interpret URB Status
    urb_status_interpreted = {
        "8dffffff": "8dffffffff (-EINPROGRESS)",
        "00000000": "00000000 (SUCCESS)"
    }.get(urb_status, urb_status)

    # Prepare the parsed data
    parsed_data = {
        'URB ID': urb_id,
        'URB Type': urb_type_interpreted,
        'URB Transfer Type': urb_transfer_type_interpreted,
        'Endpoint': endpoint_interpreted,
        'Device Number': device_number,
        'URB Bus ID': urb_bus_id,
        'Device Setup Request': device_setup_request,
        'Data Present': data_present,
        'URB Second (Hex)': urb_second,
        'URB Second (Decimal)': urb_second_decimal,
        'URB Usecond (Hex)': urb_usecond,
        'URB Usecond (Decimal)': urb_usecond_decimal,
        'URB Status': urb_status_interpreted,
        'URB Length': urb_length,
        'Data Length': data_length,
        'Setup Header': setup_header,
        'Interval': interval,
        'Start of Frame': start_of_frame,
        'Copy of Transfer Flag': copy_of_transfer_flag,
        'ISO Descriptor Number': iso_descriptor_number
    }

    return parsed_data

def parse_file(file_path):
    with open(file_path, 'r') as file:
        content = file.read()

    # Split the content by packet using "Packet Time Stamp:"
    packets = re.split(r'Packet Time Stamp:', content)
    packets = [packet.strip() for packet in packets if packet.strip()]

    # Create a log file to save the parsed data
    log_file_path = os.path.splitext(file_path)[0] + "_parsed_log.txt"
    with open(log_file_path, 'w') as log_file:
        for packet in packets:
            # Extract the packet data
            lines = packet.split('\n')
            if len(lines) < 8:
                continue  # Skip incomplete packets
            packet_data = ''.join(lines[4:8]).replace(' ', '')

            # Parse the packet data
            parsed_data = parse_packet_data(packet_data)

            # Print and save the parsed data for each packet
            log_file.write(f"Packet: {lines[0]}\n")
            for key, value in parsed_data.items():
                log_file.write(f"{key}: {value}\n")
            log_file.write("\n")

            print(f"Packet: {lines[0]}")
            for key, value in parsed_data.items():
                print(f"{key}: {value}")
            print("\n")

    print(f"Parsed data has been saved to {log_file_path}")


# Example usage
log_file_path = input("Type path to your file: ")
if not log_file_path:
    print("No path provided. Using default path.")
    print("Showing example")
    log_file_path = "../log/log_pcap_2024-08-08-11-22-51.txt"

parse_file(log_file_path)
