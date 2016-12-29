//
//  agent.h
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 12/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#ifndef __XbeeInterface__agent__
#define __XbeeInterface__agent__

#include "xbee_api.h"
#include "pos_2d.h"

class Agent {
private:
    void init();
    void clean();
    void copy_other_agent(const Agent& other_agent);
    
protected:
    Pos2d* pos;
    BYTE* addr64;
    BYTE* network_addr;
    
public:
    Agent();
    ~Agent();
    Agent(int x,int y);
    Agent(const Agent& other_agent);
    Agent& operator =(const Agent& other_agent);
    Agent(const BYTE* addr64, const BYTE* network_addr, int x = 0, int y = 0);
    void set_addr64(const BYTE* addr64);
    void set_network_addr(const BYTE* network_addr);
    void set_position(int x, int y);
    BYTE* get_addr64();
    BYTE* get_network_addr();
};

#endif /* defined(__XbeeInterface__agent__) */
