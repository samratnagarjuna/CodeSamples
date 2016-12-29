//
//  coordinator.cpp
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 12/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#include "coordinator.h"
#include <iostream>

Coordinator::Coordinator() {
    xbee_api = new XbeeAPI();
    game_on = false;
}

Coordinator::~Coordinator() {
    delete xbee_api;
}

/**
 * Discover other nodes in the network
 */
void Coordinator::discover_agents() {
    BYTE AT_cmd[2] = {'N', 'D'};
    BYTE frame_id = 0x01;
    
    // Send Node discover command
    xbee_api->send_AT_command(frame_id, AT_cmd, std::vector<BYTE>());
    std::cout << "Scanning for agents please wait ......" << std::endl;
    // Sleep for 5 seconds
    xbee_api->sleep_for(5);
    int num_agents = 0;
    while (xbee_api->is_data_available()) {
        std::vector<BYTE> data = xbee_api->fetch_and_extract_data();
        // Parse data to get the agents data
        if (data.size() && data[0] == AT_RESPONSE &&
            data[2] == 'N' && data[3] == 'D' &&
            data[4] == AT_RESPONSE_SUCCESS) {
            BYTE network_addr[SIZE_NETWORK_ADDR] = { data[5], data[6] };
            BYTE addr64[SIZE_ADDR64] = {
                data[7], data[8], data[9], data[10],
                data[11], data[12], data[13], data[14]
            };
            
            std::cout << "Agent " << std::dec << num_agents << ":" << std::endl;
            std::cout << "Network address: ";
            xbee_api->print_data(network_addr, SIZE_NETWORK_ADDR);
            std::cout << "64 Bit Address: ";
            xbee_api->print_data(addr64, SIZE_ADDR64);
            
            agents.push_back(Agent(addr64, network_addr));
            num_agents++;
        }
    }
    std::cout << "Number of Agents found: " << num_agents << std::endl;
}

/**
 * Initialize an agent in the network
 */
void Coordinator::init_agent(int agent_id) {
    agents[agent_id].set_agent_id(agent_id);
    
    int mode = 0;
    std::cout << "Enter catching mode(1/0) for agent " << agent_id  << ": ";
    std::cin >> mode;
    
    if (mode) {
        rid = agent_id;
    }
    
    int x_pos = 0;
    std::cout << "Enter X cordinate for agent " << agent_id  << ": ";
    std::cin >> x_pos;
    
    int y_pos = 0;
    std::cout << "Enter Y cordinate for agent " << agent_id  << ": ";
    std::cin >> y_pos;

    agents[agent_id].active();
    agents[agent_id].not_touched();
    agents[agent_id].set_mode(mode);
    agents[agent_id].set_position(x_pos, y_pos);
    
    BYTE addr64[SIZE_ADDR64];
    agents[agent_id].get_addr64(addr64);
    
    BYTE network_addr[SIZE_NETWORK_ADDR];
    agents[agent_id].get_network_addr(network_addr);
    
    BYTE frame_id = 0x01;
    BYTE b_radius = 0x00;
    BYTE options = 0x00;
    
    InitData init_data;
    init_data.idn = agent_id;
    init_data.mode  = mode;
    init_data.pos.set_x(x_pos);
    init_data.pos.set_y(y_pos);

    std::vector<BYTE> data = message_api.serialize_init_cmd(&init_data);
    
    if (xbee_api->is_com_port_available()) {
        xbee_api->build_and_transmit_data(addr64, network_addr, data, frame_id,
                                          b_radius, options);
        xbee_api->sleep_for(RECEIVE_CORD_WAIT);
        if (!xbee_api->is_data_transmitted()) {
            std::cout << "Setting agent position failed" << std::endl;
        }
    }
}

/**
 * Initialize all agents in the network
 */
void Coordinator::init_agents() {
    std::cout << "Configure Agents...." << std::endl;
    loop(i, agents.size()) {
        std::cout << "Agent " << std::dec << i << " parameters:" << std::endl;
        init_agent(i);
    }
}

/**
 * Wait for data from players
 */
void Coordinator::wait_for_data() {
    while (!xbee_api->is_data_available()) {
        //Wait until data is available
        xbee_api->sleep_for(RECEIVE_CORD_WAIT);
    }
}

/**
 * Starts the game
 */
void Coordinator::run() {
    discover_agents();
    init_agents();

    std::cout << "Hold your breath!! raid begins in " << STARTUP_TIME << " seconds";
    std::cout << std::endl;
    game_on = true;
    loop(i, STARTUP_TIME) {
        xbee_api->sleep_for(1);
        std::cout << "Kabaddi.. Kabaddi..." << std::endl;
    }
    
    while (game_on) {
        update();
    }
}

/**
 * Recieve updates from players and send updates to all the
 * other agents
 */
void Coordinator::update() {
    
    check_and_update_agents();
    
    GameData game_data;
    traverse(agents, agent_ptr) {
        AgentData agent_data;
        agent_data.idn = agent_ptr->get_agent_id();
        agent_data.mode = agent_ptr->get_mode();
        agent_data.is_active = agent_ptr->is_agent_active();
        agent_data.pos.set_x(agent_ptr->get_x());
        agent_data.pos.set_y(agent_ptr->get_y());
        agent_data.is_touched = agent_ptr->is_agent_touched();
        
        game_data.agents.push_back(agent_data);
    }
    
    std::vector<BYTE> data = message_api.serialize_game_data(&game_data);
    
    traverse(agents, agent_ptr) {
        if (xbee_api->is_com_port_available()) {
            BYTE addr64[SIZE_ADDR64];
            agent_ptr->get_addr64(addr64);
            
            BYTE network_addr[SIZE_NETWORK_ADDR];
            agent_ptr->get_network_addr(network_addr);
            
            BYTE frame_id = 0x01;
            BYTE b_radius = 0x00;
            BYTE options = 0x00;
            
            xbee_api->build_and_transmit_data(addr64, network_addr, data, frame_id,
                                              b_radius, options);
            xbee_api->sleep_for(RECEIVE_CORD_WAIT);
            if (!xbee_api->is_data_transmitted()) {
                std::cout << "Failed to send game data to Agent: ";
                std::cout << agent_ptr->get_agent_id() << std::endl;
            }
            
            std::cout << "Game data sent to Agent: ";
            std::cout << agent_ptr->get_agent_id() << std::endl;
        }
    }
    
    xbee_api->sleep_for(STEP_TIME); //decide the correct value for this
    // have to modified ?? or change it to below
    
    //which is better
    
    // wait for data from all the agents . i.e loop for n wait for datas
    while (xbee_api->is_data_available()) {
        std::vector<BYTE> data = xbee_api->fetch_and_extract_data();
        std::vector<BYTE> message(std::vector<BYTE> (
                                                     data.begin() +
                                                     RECEIVE_DATA_OFFSET,
                                                     data.end()
                                                     ));
        PlayerData player_data;
        player_data = message_api.deserialize_player_data(message);
        int agent_id = player_data.idn;
        std::cout << "Received packet from Agent: " << agent_id << std::endl;;
        std::cout << "(" << player_data.pos.get_x() << ",";
        std::cout << player_data.pos.get_y() << ")" << std::endl;
        agents[agent_id].set_position(player_data.pos);
    }
}

/**
 * Check for updates and update agent data in server
 */
void Coordinator::check_and_update_agents() {
    Pos2d r_pos;
    bool raider_found = false;
    int raider_id = -1;
    traverse(agents, agent_ptr) {
        if (agent_ptr->get_mode() == RAIDER_MODE) {
            r_pos = agent_ptr->get_position();
            raider_id = agent_ptr->get_agent_id();
            raider_found = true;
            break;
        }
    }
    int touch_count = 0;
    int touched_agents = 0;
    if (raider_found) {
        traverse(agents, agent_ptr) {
            if (agent_ptr->is_agent_active()) {
                if (game_rules.check_border_cross(agent_ptr->get_position())) {
                    agent_ptr->inactive();
                }
                if (agent_ptr->get_mode() != RAIDER_MODE &&
                    game_rules.check_touch(r_pos, agent_ptr->get_position())) {
                    touch_count++;
                    agent_ptr->touched();
                }
            }
            if (agent_ptr->is_agent_touched()) {
                touched_agents++;
            }
        }
        
        if (!agents[raider_id].is_agent_active()) {
            std::cout << "Unsuccessful Raid !! Raider crossed Border !!";
            std::cout << std::endl;
            game_on = false;
        }
        else if (touch_count >= CAPTURE_NUMBER) {
            std::cout << "Raider Captured !!" << std::endl;
            std::cout << "Anti-Raiders win !! Vamos !!" << std::endl;
            game_on = false;
        }
        else if (game_rules.check_successfull_raid(r_pos)) {
            if (touched_agents) {
                std::cout << "Raider wins with a score of :" << touched_agents;
                std::cout << std::endl;
            }
            else {
                std::cout << "Unsuccessful Raid !!" << std::endl;
            }
            game_on = false;
        }
        
    }
}