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
        "43": "43 (COMPLETE)"
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
        "8dffffff": "8dffffff (Operation Now in Progress)",
        "feffffff": "feffffff (No Such File or Directory)",
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
        'ISO Descriptor Number': iso_descriptor_number,
        'ISO Descriptors': []
    }

    if urb_transfer_type == "00":  # ISO transfer
        iso_descriptor_count = int(iso_descriptor_number[:2], 16) * 8
        descriptors_data = packet_data[128:128 + iso_descriptor_count * 32]
        for i in range(iso_descriptor_count):
            start_index = i * 32
            iso_desc = {
                'Status': descriptors_data[start_index:start_index + 8],
                'Offset': int(descriptors_data[start_index + 8:start_index + 16], 16),
                'Length': int(descriptors_data[start_index + 16:start_index + 24], 16)
            }
            parsed_data['ISO Descriptors'].append(iso_desc)

    return parsed_data

def parse_file(file_path):
    with open(file_path, 'r') as file:
        content = file.read()
        
    if "Capture Statistics:" in content:
        stats_section = content.split("Capture Statistics:")[1].strip()
        capture_stats = dict(re.findall(r'(\w[\w\s]+): (\d+)', stats_section))
        content = content.split("Capture Statistics:")[0].strip() 


    # Split the content by packet using "Packet Time Stamp:"
    packets = re.split(r'Packet Time Stamp:', content)
    packets = [packet.strip() for packet in packets if packet.strip()]

    # List to store packets with "No Such File or Directory" URB Status
    no_such_file_packets = []

    # Create a log file to save the parsed data
    log_file_path = os.path.splitext(file_path)[0] + "_parsed_log.txt"
    with open(log_file_path, 'w') as log_file:
        total_packets = len(packets)
        iso_success_count = 0
        iso_fail_count = 0
        iso_fail_packets = []
        URB_status_count = {'Operation Now in Progress': 0, 'No Such File or Directory': 0, 'SUCCESS': 0}

        for packet in packets:
            lines = packet.split('\n')
            if len(lines) < 9:
                continue  # Skip incomplete packets
            timestamp = lines[1].split(': ')[1]  # Extract Timestamp
            chrono_time = lines[2].split(': ')[1]  # Extract Chrono Time
            packet_data = ''.join(lines[5:9]).replace(' ', '')

            parsed_data = parse_packet_data(packet_data)
            parsed_data['Chrono Time'] = chrono_time  # Add Chrono Time to parsed data
            urb_transfer_type = parsed_data['URB Transfer Type']
            urb_status = parsed_data['URB Status']

            if urb_transfer_type in ["00 (ISO)", "03 (BULK)"]:
                if urb_status == "8dffffff (Operation Now in Progress)":
                    URB_status_count['Operation Now in Progress'] += 1
                elif urb_status == "feffffff (No Such File or Directory)":
                    URB_status_count['No Such File or Directory'] += 1
                    no_such_file_packets.append((timestamp, chrono_time))
                elif urb_status == "00000000 (SUCCESS)":
                    URB_status_count['SUCCESS'] += 1

            if urb_transfer_type == "00 (ISO)":
                for iso_desc in parsed_data['ISO Descriptors']:
                    if iso_desc['Status'] == "00000000":
                        iso_success_count += 1
                    else:
                        iso_fail_count += 1
                        iso_fail_packets.append(packet)

            # # Log format updated to include Timestamp and Chrono Time
            # log_file.write(f"Packet:      {timestamp}\n")
            # log_file.write(f"Chrono Time: {chrono_time}\n")
            # for key, value in parsed_data.items():
            #     log_file.write(f"{key}: {value}\n")
            # log_file.write("\n")

        log_file.write(f"--------------------------------------------------------------------------------\n")
        #total_URB = URB_status_count['Operation Now in Progress'] + URB_status_count['No Such File or Directory'] + URB_status_count['SUCCESS']
        total_URB = URB_status_count['No Such File or Directory'] + URB_status_count['SUCCESS']
        log_file.write(f"Operation Now in Progress excluded from the percentage calculation \n")
        #log_file.write(f"Operation Now in Progress: {URB_status_count['Operation Now in Progress'] / total_URB * 100:.2f}%\n")
        log_file.write(f"SUCCESS: {URB_status_count['SUCCESS'] / total_URB * 100:.2f}%\n")
        log_file.write(f"No Such File or Directory: {URB_status_count['No Such File or Directory'] / total_URB * 100:.2f}%\n")

        log_file.write("\nNo Such File or Directory Logged Time:\n")
        for packet_time in no_such_file_packets:
            log_file.write(f"Packet Time Stamp: {packet_time[0]}, Chrono Time: {packet_time[1]}\n")

        
        if iso_success_count + iso_fail_count > 0:
            log_file.write(f"\nISO Descriptor Success Rate: {iso_success_count / (iso_success_count + iso_fail_count) * 100:.2f}%\n")
            log_file.write(f"ISO Descriptor Failure Rate: {iso_fail_count / (iso_success_count + iso_fail_count) * 100:.2f}%\n")

        log_file.write("\nFailed ISO Packets:\n")
        for packet in iso_fail_packets:
            log_file.write(f"Packet: {packet.split('\\n')[0]}\n")
            
        # Capture Statistics added to the log file
        if capture_stats:
            log_file.write("\nCapture Statistics:\n")
            for key, value in capture_stats.items():
                log_file.write(f"{key}: {value}\n")


    print(f"Parsed data has been saved to {log_file_path}")


# Example usage
log_file_path = input("Type path to your file: ")
if not log_file_path:
    print("No path provided. Using default path.")
    print("Showing example")
    default_log_file_path = "../log/log_pcap_2024-08-12-15-47-15.txt"
    log_file_path = default_log_file_path

parse_file(log_file_path)
