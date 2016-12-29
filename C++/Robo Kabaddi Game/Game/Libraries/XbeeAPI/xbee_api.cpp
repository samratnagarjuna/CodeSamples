//
//  xbee_api.cpp
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 12/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#include "xbee_api.h"
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <stdlib.h>
#include <wiringSerial.h>

/**
 * Connects to the com port attached to Raspi device
 */
XbeeAPI::XbeeAPI() {
    com_port = -1;
    get_com_port_connection();
    if (com_port == -1) {
        std::cout << "Connection to COM port failed." << std::endl;
    }
}

/**
 * Sleeps for a given time
 * @param
 * time: Sleep time in seconds
 */
void XbeeAPI::sleep_for(const int time) {
    usleep(time * 1000000);
}

/**
 * Connects to the com port attached to Raspi device
 */
void XbeeAPI::get_com_port_connection() {
    com_port = serialOpen("/dev/ttyAMA0", BAUD_RATE);
}

/**
 * Returns COM PORT identifier for the connection established
 * @return COM PORT value for serial communication
 */
int XbeeAPI::get_com_port_id() {
    //TODO: Check if we can do better than terminating
    if (!is_com_port_available()) {
        std::cout << "COM port not available. Please check connection.";
        std::cout << std::endl;
        exit(EXIT_FAILURE);
    }
    return com_port;
}

/**
 * Checks if COM connection is established
 * @return TRUE if connection is eastablished else FALSE
 */
bool XbeeAPI::is_com_port_available() {
    return (com_port != -1);
}

/**
 * Check if serial data in avialable
 * @return number of bytes avialable to read
 */
int XbeeAPI::is_data_available() {
    int num_bytes = serialDataAvail(get_com_port_id());
    return (num_bytes != -1) ? num_bytes : 0;
}

/**
 * Returns next byte in the serial buffer
 * @return Next byte is avialable else 0x00
 */
BYTE XbeeAPI::get_next_byte() {
    return (is_data_available()) ? (BYTE) serialGetchar(get_com_port_id())
        : ZERO_BYTE;
}

/**
 * Puts next byte in serial buffer
 */
void XbeeAPI::put_next_byte(const BYTE byte) {
    serialPutchar(get_com_port_id(), byte);
}

/**
 * Prints the give data in Hex format
 */
void XbeeAPI::print_data(const std::vector<BYTE>& data) {
    std::cout << "{ " ;
    traverse(data, data_ptr) {
        std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2)
            << int(*data_ptr) << ",";
    }
    std::cout << "}" << std::endl;
    std::cout << std::dec;
}

void XbeeAPI::print_data(const BYTE* data, int num_bytes) {
    std::cout << "{ " ;
    loop(i, num_bytes) {
        std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2)
        << (int)data[i] << ",";
    }
    std::cout << "}" << std::endl;
    std::cout << std::dec;
}

/**
 * Sends serial data through the given com port
 * @param
 * data: data vector to be transmitted
 */
void XbeeAPI::send_data(const std::vector<BYTE>& data) {
    traverse(data, data_ptr) {
        put_next_byte(*data_ptr);
    }
}

/**
 * Build transmit packect for Xbee series 2
 * @param
 * addr64: Pointer to 64 bit address of receiver Xbee
 * network_addr: Network address of receiver Xbee
 * data: Data vector to be transmitted through Xbee
 * frame_id: Frame identifier for the transmit packet
 * b_radius: Broadcast radius
 * options: Options for the transmit packer
 * @see: Structure of Xbee transmit packet from documentation
 */
void XbeeAPI::build_and_transmit_data(const BYTE* addr64,
                                      const BYTE* network_addr,
                                      const std::vector<BYTE>& data,
                                      const BYTE frame_id, const BYTE b_radius,
                                      const BYTE options) {
    int num_data_bytes = (int)data.size();
    /*
     * Total packet length = Frame delimiter(1 byte) + Length(2 bytes) +
     *                       Frame type(1 byte) + Frame Id(1 byte) +
     *                       64 Bit Addr(8 bytes) + Network Addr(2 bytes) +
     *                       Broadcast radius(1 byte) + options(1 byte) +
     *                       data(num_data_bytes) + checksum(1 byte)
     */
    int num_bytes = num_data_bytes + 18; //As per above calculation
    int pos = 0;
    std::vector<BYTE> packet(num_bytes);
    
    //Frame delimiter
    packet[pos++] = FRAME_DELIMITER;
    
    //Length = Length of packet excluding Frame delimiter, Length, Checksum
    WORD length = num_data_bytes + 14;
    BYTE* byte_ptr = (BYTE*) &length;
    //Memory storage : assuming little endian
    packet[pos++] = *(byte_ptr + 1);
    packet[pos++] = *byte_ptr;
    
    //Frame Type: Here transmit request
    packet[pos++] = TRANSMIT_REQUEST;
    
    //Frame id
    packet[pos++] = frame_id;
    
    //Frame data: 64 Bit address(8 bytes)
    for (int i = 0 ; i < SIZE_ADDR64 ; i++) {
        packet[pos++] = *(addr64 + i);
    }
    
    //Frame data: Network address(2 bytes)
    for (int i = 0 ; i < SIZE_NETWORK_ADDR ; i++) {
        packet[pos++] = *(network_addr + i);
    }
    
    //Frame data: Broadcast radius
    packet[pos++] = b_radius;
    
    //Frame data: options
    packet[pos++] = options;
    
    //Frame data: Transmit data
    traverse(data, data_ptr) {
        packet[pos++] = *data_ptr;
    }
    
    //Checksum
    std::vector<BYTE> checksum_data(packet.begin() + CHECKSUM_OFFSET,
                                    packet.end() - 1);
    BYTE checksum = calculate_checksum(checksum_data);
    packet[pos] = checksum;
    
    //print_data(packet);
    send_data(packet);
    //TODO:: Check for successful response??
}

/**
 * Checks if the data is transmitted successfully
 * @return: True if successful transmisstion of data packet else false
 */
bool XbeeAPI::is_data_transmitted() {
    std::vector<BYTE> response = fetch_and_extract_data();
    bool success = false;
    if (response.size() && response[0] == TRANSMIT_STATUS) {
        BYTE network_addr[SIZE_NETWORK_ADDR] = {response[2], response[3]};
        if (response[5] == TRANSMIT_SUCCESS) {
            std::cout << "Packet delivered successfully to: ";
            //print_data(network_addr, SIZE_NETWORK_ADDR);
            success = true;
        }
    }
    return success;
}

/**
 * Fetches packet from the receiving port and extracts the data
 * @return: returns the data received
 */
std::vector<BYTE> XbeeAPI::fetch_and_extract_data() {
    while (get_next_byte() != FRAME_DELIMITER) {
        /* Discarding unread or garbage data*/
    }
    //Encountered a FRAME DELIMITER
    
    //Extract length
    WORD length = 0;
    BYTE* byte_ptr = (BYTE*)&length;
    //Assuming little endian
    *(byte_ptr+1) = get_next_byte();
    *(byte_ptr) = get_next_byte();
    
    //Frame specific data
    std::vector<BYTE> frame_data(length);
    traverse(frame_data, data_ptr) {
        *data_ptr = get_next_byte();
    }
    
    //Checksum
    BYTE checksum = get_next_byte();
    return !(is_data_corrupted(frame_data, checksum)) ? frame_data :
        std::vector<BYTE>();
}

/**
 * Builds AT command packet and queries
 * @param
 * frame_id: frame_id to correlate with subsequent ACK
 * AT_cmd: AT command (NI, NJ, etc..,)
 * value: optional value
 * @see Xbee Series 2 documentation
 */
void XbeeAPI::send_AT_command(const BYTE frame_id,const BYTE* AT_cmd,
                              const std::vector<BYTE>& value) {
    int cmd_length = 8 + (int)value.size();
    std::vector<BYTE> command(cmd_length);
    
    int pos = 0;
    //FRAME DELIMITER
    command[pos++] = FRAME_DELIMITER;
    
    //Length = Length of packet excluding Frame delimiter, Length, Checksum
    WORD length = cmd_length - 4;
    BYTE* byte_ptr = (BYTE*) &length;
    //Memory storage : assuming little endian
    command[pos++] = *(byte_ptr + 1);
    command[pos++] = *byte_ptr;

    //FRAME TYPE
    command[pos++] = AT_COMMAND;
    
    //FRAME ID
    command[pos++] = frame_id;
    
    //AT Command
    command[pos++] = AT_cmd[0];// First character
    command[pos++] = AT_cmd[1]; // Second character
    
    //Optional value
    traverse(value, byte_ptr) {
        command[pos++] = *byte_ptr;
    }
    
    //Checksum
    std::vector<BYTE> checksum_data(command.begin() + CHECKSUM_OFFSET,
                                    command.end() - 1);
    BYTE checksum = calculate_checksum(checksum_data);
    command[pos] = checksum;
    
    //print_data(packet, num_bytes);
    send_data(command);
}

/**
 * Calculates checksum for given data
 * @param
 * data: Data vector for which checksum is to be calculated
 * @see: Xbee series 2 documentation
 */
BYTE XbeeAPI::calculate_checksum(const std::vector<BYTE> &data) {
    BYTE checksum = ZERO_BYTE;
    traverse(data, data_ptr) {
        checksum += *data_ptr;
    }
    return 0xFF - checksum;
}
/**
 * Checks if the data is not corrupted
 * @return TRUE if not corrupted else FALSE
 * @see: Xbee series 2 documentation
 */
bool XbeeAPI::is_data_corrupted(const std::vector<BYTE>& data,
                                const BYTE checksum) {
    BYTE sum = ZERO_BYTE;
    traverse(data, data_ptr) {
        sum += *data_ptr;
    }
    return !(sum + checksum == 0xFF);
}