//
//  player.cpp
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 19/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#include "player.h"
#include <iostream>

Player::Player() {
    xbee_api = new XbeeAPI();
}

Player::~Player() {
    delete xbee_api;
}

/**
 * Initialize agent with intial position and mode of play
 */
void Player::get_init_command() {
    if (xbee_api->is_data_available()) {
        std::vector<BYTE> data = xbee_api->fetch_and_extract_data();
        //XXX: Find a way to make value 12 generic
        if (data.size() > 12 && data[0] == TRANSMIT_RESPONSE) {
            BYTE coordinator_addr64[SIZE_ADDR64] = {
                data[1], data[2], data[3], data[4],
                data[5], data[6], data[7], data[8]
            };
            BYTE coordinator_network_addr[SIZE_NETWORK_ADDR] = {
                data[9], data[10]
            };
            agent.set_coordinator_addr64(coordinator_addr64);
            agent.set_coordinator_network_addr(coordinator_network_addr);
            
            std::vector<BYTE> message(std::vector<BYTE> (
                                                         data.begin() +
                                                         RECEIVE_DATA_OFFSET,
                                                         data.end()
                                                         ));
            InitData init_data = message_api.deserialize_init_cmd(message);
            agent.set_agent_id(init_data.idn);
            agent.set_mode(init_data.mode);
            agent.set_position(init_data.pos.get_x(), init_data.pos.get_y());
            
            std::cout << "Agent Confugured successfully with parameters: "
                << std::endl;
            std::cout << "Id: " << agent.get_agent_id() << std::endl;
            std::cout << "Mode: " << agent.get_mode() << std::endl;
            std::cout << "Position: " << "("  << agent.get_x() << ", ";
            std::cout << agent.get_y() << ")" << std::endl;
        }
    }
}

/**
 * Wait for data from coordinator
 */
void Player::wait_for_data() {
    while (!xbee_api->is_data_available()) {
        //Wait until data is available
        xbee_api->sleep_for(RECEIVE_WAIT);
    }
    xbee_api->sleep_for(RECEIVE_WAIT);
}

/**
 * Starts the game
 */
void Player::run() {
    wait_for_data();
    get_init_command();
    
    while (true) {
        update();
    }
}

/**
 * Update agent position and take next move
 */
void Player::update() {
    wait_for_data();
    std::vector<BYTE> recv_data = xbee_api->fetch_and_extract_data();
    std::vector<BYTE> message(std::vector<BYTE> (
                                                 recv_data.begin() +
                                                 RECEIVE_DATA_OFFSET,
                                                 recv_data.end()
                                                 ));
    GameData game_data;
    game_data = message_api.deserialize_game_data(message);
    AgentData own;
    AgentData raider;
    std::vector<AgentData> others;
    traverse(game_data.agents, agent_ptr) {
        AgentData agent_data;
        agent_data.idn = agent_ptr->idn;
        agent_data.mode = agent_ptr->mode;
        agent_data.pos.set_x(agent_ptr->pos.get_x());
        agent_data.pos.set_y(agent_ptr->pos.get_y());
        agent_data.is_active = agent_ptr->is_active;
        agent_data.is_touched = agent_ptr->is_touched;
        
        std::cout << "(";
        std::cout << agent_data.idn << ",";
        std::cout << agent_data.pos.get_x() << ",";
        std::cout << agent_data.pos.get_y();
        std::cout << ")" << std::endl;
        
        if (agent_ptr->idn == agent.get_agent_id()) {
            agent.set_mode(agent_ptr->mode);
            agent.set_position(agent_ptr->pos);
            agent_ptr->is_touched ? agent.touched() : agent.not_touched();
            agent_ptr->is_active ? agent.active() : agent.inactive();
            own = agent_data;
        }
        else if (agent_ptr->mode == ANTI_RAIDER_MODE){
            others.push_back(agent_data);
        }
        
        if (agent_ptr->mode == RAIDER_MODE) {
            raider = agent_data;
        }
    }
    
    Pos2d next_pos;
    if (agent.get_mode() == RAIDER_MODE) {
        next_pos = strategy.move_raider(raider, others);
    }
    else {
        next_pos = strategy.move_anti_raider(own, raider, others);
    }
    std::cout << "(" << next_pos.get_x() << ",";
    std::cout << next_pos.get_y() << ")" << std::endl;
    
    Pos2d curr_pos = agent.get_position();
    agent.set_position(next_pos);
    if (motor.send_motion_command(curr_pos, next_pos)) {
        agent.set_position(next_pos);
    }
    
    PlayerData player_data;
    player_data.idn = agent.get_agent_id();
    player_data.pos.set_position(agent.get_x(), agent.get_y());
    
    std::vector<BYTE> data = message_api.serialize_player_data(&player_data);
    BYTE addr64[SIZE_ADDR64];
    agent.get_coordinator_addr64(addr64);
    
    BYTE network_addr[SIZE_NETWORK_ADDR];
    agent.get_coordinator_network_addr(network_addr);
    
    BYTE frame_id = 0x01;
    BYTE b_radius = 0x00;
    BYTE options = 0x00;
    //wait here ?
    if (xbee_api->is_com_port_available()) {
        xbee_api->build_and_transmit_data(addr64, network_addr, data, frame_id,
                                          b_radius, options);
        xbee_api->sleep_for(RECEIVE_WAIT);
        if (!xbee_api->is_data_transmitted()) {
            std::cout << "sending player data failed" << std::endl;
        }
    }
}