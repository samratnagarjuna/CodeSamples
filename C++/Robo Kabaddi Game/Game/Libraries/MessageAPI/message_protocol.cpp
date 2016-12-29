//
//  message_protocol.cpp
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 18/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#include "message_protocol.h"
#include <cstdlib>

#define MESSAGE_DELIMITER '$'

/**
 * Clear the string buffer stream
 */
void MessageAPI::clear_stream() {
    string_stream.str("");
}

/**
 * Split data based on the delimiter
 */
std::vector<std::string> MessageAPI::split_data(std::vector<BYTE>& data,
                                                BYTE delimiter) {
    std::vector<std::string> output;
    int i = 0;
    while (i < data.size()) {
        std::string str = "";
        while (i < data.size() && data[i] != delimiter) {
            str = str + (char)data[i];
            i++;
        }
        output.push_back(str);
        i++;
    }
    return output;
}

/**
 * Convert string to interger
 */
int MessageAPI::string_to_int(std::string str) {
    char c_string[str.length()];
    loop(i, str.length()) {
        c_string[i] = str[i];
    }
    return atoi(c_string);
    
}

/**
 * Convert string to byte data
 */
std::vector<BYTE> MessageAPI::serialize_string(std::string str) {
    std::string data = str;;
    std::vector<BYTE> serialized_data(data.length());
    loop(i, data.length()) {
        serialized_data[i] = (BYTE) data[i];
    }
    return serialized_data;
}

/**
 * Serialize Initialize command to byte data
 */
//TODO: Change the access specifier of Pos2d to const for afew functions
std::vector<BYTE> MessageAPI::serialize_init_cmd(InitData* init_data) {
    clear_stream();
    string_stream << "IN" << MESSAGE_DELIMITER;
    string_stream << init_data->idn << MESSAGE_DELIMITER;
    string_stream << init_data->mode << MESSAGE_DELIMITER;
    string_stream << init_data->pos.get_x() << MESSAGE_DELIMITER;
    string_stream << init_data->pos.get_y();
    
    return serialize_string(string_stream.str());
}

/**
 * Deserialize Initialize command in Init Data structure
 */
InitData MessageAPI::deserialize_init_cmd(std::vector<BYTE>& data) {
    InitData init_data;
    init_data.idn = -1;
    init_data.mode = -1;
    std::vector<std::string> response = split_data(data, MESSAGE_DELIMITER);
    if (response.size() == 5 && response[0].compare("IN") == 0) {
        init_data.idn = string_to_int(response[1]);
        init_data.mode = string_to_int(response[2]);
        init_data.pos.set_x(string_to_int(response[3]));
        init_data.pos.set_y(string_to_int(response[4]));
    }
    return init_data;
}

/**
 * Serialize the GameData sturcture to byte
 */
std::vector<BYTE> MessageAPI::serialize_game_data(GameData* game_data) {
    clear_stream();
    string_stream << "GD" << MESSAGE_DELIMITER;
    string_stream << game_data->agents.size();
    traverse(game_data->agents, agent_ptr) {
        string_stream << MESSAGE_DELIMITER;
        string_stream << agent_ptr->idn << MESSAGE_DELIMITER;
        string_stream << agent_ptr->mode << MESSAGE_DELIMITER;
        string_stream << agent_ptr->is_active << MESSAGE_DELIMITER;
        string_stream << agent_ptr->pos.get_x() << MESSAGE_DELIMITER;
        string_stream << agent_ptr->pos.get_y() << MESSAGE_DELIMITER;
        string_stream << agent_ptr->is_touched;
    }
    return serialize_string(string_stream.str());
}

/**
 * Deserialize byte data to GameData structure
 */
GameData MessageAPI::deserialize_game_data(std::vector<BYTE>& data) {
    GameData game_data;
    std::vector<std::string> response = split_data(data, MESSAGE_DELIMITER);
    if (response.size() && response[0].compare("GD") == 0) {
        int num_agents = string_to_int(response[1]);
        loop(i, num_agents) {
            // IMPORTANT: change the equaiton below if you increase the agent data
            int index = 2 + i * 6;
            AgentData agent_data;
            agent_data.idn = string_to_int(response[index]);
            agent_data.mode = string_to_int(response[index + 1]);
            agent_data.is_active = string_to_int(response[index + 2]);
            agent_data.pos.set_x(string_to_int(response[index + 3]));
            agent_data.pos.set_y(string_to_int(response[index + 4]));
            agent_data.is_touched = string_to_int(response[index + 5]);
            
            game_data.agents.push_back(agent_data);
        }
    }
    return game_data;
}

/**
 * Serialize the PlayerData sturcture to byte
 */
std::vector<BYTE> MessageAPI::serialize_player_data(PlayerData* player_data) {
    clear_stream();
    string_stream << "PD" << MESSAGE_DELIMITER;
    string_stream << player_data->idn << MESSAGE_DELIMITER;
    string_stream << player_data->pos.get_x() << MESSAGE_DELIMITER;
    string_stream << player_data->pos.get_y();
    return serialize_string(string_stream.str());
}

/**
 * Deserialize byte data to PlayerData structure
 */
PlayerData MessageAPI::deserialize_player_data(std::vector<BYTE>& data) {
    PlayerData player_data;
    std::vector<std::string> response = split_data(data, MESSAGE_DELIMITER);
    
    if (response.size() && response[0].compare("PD") == 0) {
        player_data.idn = string_to_int(response[1]);
        player_data.pos.set_position(string_to_int(response[2]),
                                     string_to_int(response[3]));
    }
    return player_data;
}