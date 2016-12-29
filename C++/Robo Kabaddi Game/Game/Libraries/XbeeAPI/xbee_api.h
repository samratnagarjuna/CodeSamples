//
//  xbee_api.h
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 12/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//  Note: The code is only compatible with wiringPi on a configured Raspi
//

#ifndef __XbeeInterface__xbee_api__
#define __XbeeInterface__xbee_api__

#include "stl.h"

#define FRAME_DELIMITER 0x7E
#define TRANSMIT_REQUEST 0x10
#define TRANSMIT_STATUS 0x8B
#define TRANSMIT_RESPONSE 0x90
#define TRANSMIT_SUCCESS 0x00
#define AT_COMMAND 0x08
#define AT_RESPONSE 0x88
#define AT_RESPONSE_SUCCESS 0x00
#define SLEEP_TIME_ND 5000000
#define SIZE_ADDR64 8
#define SIZE_NETWORK_ADDR 2
#define BAUD_RATE 9600
#define CHECKSUM_OFFSET 3
#define RECEIVE_DATA_OFFSET 12
#define ZERO_BYTE 0x00

typedef unsigned char BYTE;
typedef unsigned short int WORD;

class XbeeAPI {
protected:
    int com_port;
    void get_com_port_connection();
    int get_com_port_id();
    BYTE get_next_byte();
    void put_next_byte(const BYTE byte);
    void send_data(const std::vector<BYTE>& data);
    BYTE calculate_checksum(const std::vector<BYTE>& data);
    bool is_data_corrupted(const std::vector<BYTE>& data, const BYTE checksum);
    
public:
    XbeeAPI();
    void sleep_for(const int time);
    bool is_com_port_available();
    int is_data_available();
    void send_AT_command(const BYTE frame_id,const BYTE* AT_cmd,
                         const std::vector<BYTE>& value);
    void print_data(const std::vector<BYTE>& data);
    void print_data(const BYTE* data, int num_bytes);
    void build_and_transmit_data(const BYTE* addr64, const BYTE* network_addr,
                                 const std::vector<BYTE>& data,
                                 const BYTE frame_id, const BYTE b_radius,
                                 const BYTE options);
    std::vector<BYTE> fetch_and_extract_data();
    bool is_data_transmitted();
    
};

#endif /* defined(__XbeeInterface__xbee_api__) */
