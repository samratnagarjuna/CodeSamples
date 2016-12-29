//
//  simple_agent.cpp
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 17/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#include "simple_agent.h"
#include "stl.h"
#include <cstring>

Agent::Agent() {
    init();
}


Agent::Agent(int agent_id, int x,int y) {
    init();
    this->agent_id = agent_id;
    pos.set_position(x, y);
}

void Agent::init() {
    agent_id = -1;
    mode = -1;
    is_touched = false;
    is_active = false;
    loop(i, SIZE_ADDR64) {
        addr64[i] = 0x00;
        coordinator_addr64[i] = 0x00;
    }
    loop(i, SIZE_NETWORK_ADDR) {
        network_addr[i] = 0x00;
    }
}

void Agent::set_agent_id(int agent_id) {
    this->agent_id = agent_id;
}

void Agent::set_mode(int mode) {
    this->mode = mode;
}

void Agent::touched() {
    is_touched = true;
}

void Agent::not_touched() {
    is_touched = false;
}

void Agent::active() {
    is_active = true;
}

void Agent::inactive() {
    is_active = false;
}

bool Agent::is_agent_active() {
    return is_active;
}

bool Agent::is_agent_touched() {
    return is_touched;
}

Agent::Agent(const BYTE* addr64, const BYTE* network_addr, int agent_id,
             int x, int y) {
    init();
    this->agent_id = agent_id;
    set_addr64(addr64);
    set_network_addr(network_addr);
    set_position(x, y);
}

void Agent::set_addr64(const BYTE* addr64) {
    if (addr64) {
        memcpy(this->addr64, addr64, SIZE_ADDR64);
    }
}

void Agent::set_network_addr(const BYTE* network_addr) {
    if (network_addr) {
        memcpy(this->network_addr, network_addr, SIZE_NETWORK_ADDR);
    }
}

void Agent::set_coordinator_addr64(const BYTE* coordinator_addr64) {
    if (coordinator_addr64) {
        memcpy(this->coordinator_addr64, coordinator_addr64, SIZE_ADDR64);
    }
}

void Agent::set_coordinator_network_addr(const BYTE* coordinator_network_addr) {
    if (coordinator_network_addr) {
        memcpy(this->coordinator_network_addr, coordinator_network_addr,
               SIZE_NETWORK_ADDR);
    }
}

void Agent::set_position(int x, int y) {
    pos.set_position(x, y);
}

void Agent::set_position(Pos2d pos) {
    this->pos.set_position(pos.get_x(), pos.get_y());
}

int Agent::get_agent_id() {
    return agent_id;
}

int Agent::get_mode() {
    return mode;
}

int Agent::get_x() {
    return pos.get_x();
}

int Agent::get_y() {
    return pos.get_y();
}

void Agent::get_addr64(BYTE* addr64) {
    if (addr64) {
        memcpy(addr64, this->addr64, SIZE_ADDR64);
    }
}

void Agent::get_network_addr(BYTE* network_addr) {
    if (network_addr) {
        memcpy(network_addr, this->network_addr, SIZE_NETWORK_ADDR);
    }
}

void Agent::get_coordinator_addr64(BYTE* coordinator_addr64) {
    if (coordinator_addr64) {
        memcpy(coordinator_addr64, this->coordinator_addr64, SIZE_ADDR64);
    }
}

void Agent::get_coordinator_network_addr(BYTE* coordinator_network_addr) {
    if (coordinator_network_addr) {
        memcpy(coordinator_network_addr, this->coordinator_network_addr,
               SIZE_NETWORK_ADDR);
    }
}

Pos2d Agent::get_position() {
    return pos;
}



