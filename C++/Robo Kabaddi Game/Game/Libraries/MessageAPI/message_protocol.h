//
//  message_protocol.h
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 18/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#ifndef __XbeeInterface__message_protocol__
#define __XbeeInterface__message_protocol__

#include "pos_2d.h"
#include "stl.h"
#include "xbee_api.h"
#include <sstream>
#include <string>

typedef struct {
    int idn;
    int mode;
    Pos2d pos;
} InitData;

typedef struct {
    int idn;
    int mode;
    bool is_active;
    Pos2d pos;
    bool is_touched;
} AgentData;

typedef struct {
    std::vector<AgentData> agents;
} GameData;

typedef struct {
    int idn;
    Pos2d pos;
} PlayerData;

class MessageAPI {
private:
    std::ostringstream string_stream;
    void clear_stream();
    std::vector<std::string> split_data(std::vector<BYTE>& data,
                                        BYTE delimiter);
    int string_to_int(std::string str);
    std::vector<BYTE> serialize_string(std::string str);

public:
    std::vector<BYTE> serialize_init_cmd(InitData* init_data);
    InitData deserialize_init_cmd(std::vector<BYTE>& data);
    std::vector<BYTE> serialize_game_data(GameData* game_data);
    GameData deserialize_game_data(std::vector<BYTE>& data);
    std::vector<BYTE> serialize_player_data(PlayerData* player_data);
    PlayerData deserialize_player_data(std::vector<BYTE>& data);
};

#endif /* defined(__XbeeInterface__message_protocol__) */
