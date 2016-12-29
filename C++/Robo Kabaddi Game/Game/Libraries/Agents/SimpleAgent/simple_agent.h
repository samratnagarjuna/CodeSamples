//
//  simple_agent.h
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 17/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#ifndef __XbeeInterface__simple_agent__
#define __XbeeInterface__simple_agent__

#include "xbee_api.h"
#include "pos_2d.h"

class Agent {
private:
    void init();
    
protected:
    int agent_id;
    int mode;
    bool is_touched;
    bool is_active;
    Pos2d pos;
    BYTE addr64[SIZE_ADDR64];
    BYTE network_addr[SIZE_NETWORK_ADDR];
    BYTE coordinator_addr64[SIZE_ADDR64];
    BYTE coordinator_network_addr[SIZE_NETWORK_ADDR];
    
public:
    Agent();
    Agent(int agent_id, int x, int y);
    Agent(const BYTE* addr64, const BYTE* network_addr, int agent_id  = -1,
          int x = 0, int y = 0);
    void set_agent_id(int agent_id);
    void set_mode(int mode);
    void touched();
    void not_touched();
    bool is_agent_touched();
    void active();
    void inactive();
    bool is_agent_active();
    void set_addr64(const BYTE* addr64);
    void set_network_addr(const BYTE* network_addr);
    void set_coordinator_addr64(const BYTE* coordinator_addr64);
    void set_coordinator_network_addr(const BYTE* coordinator_network_addr);
    void set_position(int x, int y);
    void set_position(const Pos2d pos);
    int get_agent_id();
    int get_mode();
    int get_x();
    int get_y();
    void get_addr64(BYTE* addr64);
    void get_network_addr(BYTE* network_addr);
    void get_coordinator_addr64(BYTE* coordinator_addr64);
    void get_coordinator_network_addr(BYTE* coordinator_network_addr);
    Pos2d get_position();
};

#endif /* defined(__XbeeInterface__simple_agent__) */
