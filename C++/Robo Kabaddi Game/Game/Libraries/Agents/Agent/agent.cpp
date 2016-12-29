//
//  agent.cpp
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 12/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#include "agent.h"
#include <iostream>
#include <stdlib.h>

Agent::Agent() {
    init();
}

Agent::Agent(int x,int y) {
    init();
    pos = new Pos2d(x,y);
}

Agent::Agent(const Agent& other_agent) {
    copy_other_agent(other_agent);
}

Agent& Agent::operator = (const Agent& other_agent) {
    if (this != &other_agent) {
        clean();
        copy_other_agent(other_agent);
    }
    return *this;
}

Agent::Agent(const BYTE* addr64, const BYTE* network_addr, int x, int y) {
    init();
    set_addr64(addr64);
    set_network_addr(network_addr);
    pos = new Pos2d(x,y);
}

void Agent::init() {
    pos = NULL;
    addr64 = NULL;
    network_addr = NULL;
}

void Agent::copy_other_agent(const Agent& other_agent) {
    init();
    set_addr64(other_agent.addr64);
    set_network_addr(other_agent.network_addr);
    set_position(other_agent.pos->get_x(), other_agent.pos->get_y());
}

void Agent::clean() {
    delete pos;
    delete[] addr64;
    delete[] network_addr;
    pos = NULL;
    addr64 = NULL;
    network_addr = NULL;
}

void Agent::set_addr64(const BYTE* addr64) {
    if (addr64) {
        this->addr64 = (!this->addr64) ? new BYTE[SIZE_ADDR64] : this->addr64;
        memcpy(this->addr64, addr64, SIZE_ADDR64);
    }
    else {
        if (this->addr64) {
            delete[] this->addr64;
            this->addr64 = NULL;
        }
    }
}

BYTE* Agent::get_addr64() {
    return addr64;
}

BYTE* Agent::get_network_addr() {
    return network_addr;
}

void Agent::set_network_addr(const BYTE* network_addr) {
    if (network_addr) {
        this->network_addr = (!this->network_addr) ?
            new BYTE[SIZE_NETWORK_ADDR] : this->network_addr;
        memcpy(this->network_addr, network_addr, SIZE_NETWORK_ADDR);
    }
    else {
        if (this->network_addr) {
            delete[] this->network_addr;
            this->network_addr = NULL;
        }
    }
}

void Agent::set_position(int x, int y) {
    pos = (!pos) ? new Pos2d() : pos;
    pos->set_position(x, y);
}

Agent::~Agent() {
    clean();
}