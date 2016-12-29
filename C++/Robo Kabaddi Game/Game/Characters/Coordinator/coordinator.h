//
//  coordinator.h
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 12/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#ifndef __XbeeInterface__coordinator__
#define __XbeeInterface__coordinator__

#include "xbee_api.h"
#include "simple_agent.h"
#include "message_protocol.h"
#include "game_rules.h"
#include "stl.h"

#define RECEIVE_CORD_WAIT 1
#define STARTUP_TIME 5
#define STEP_TIME 5
#define RAIDER_MODE 1

class Coordinator {
private:
    XbeeAPI* xbee_api;
    int rid;
    std::vector<Agent> agents;
    MessageAPI message_api;
    GameRules game_rules;
    bool game_on;
    
public:
    Coordinator();
    ~Coordinator();
    void discover_agents();
    void init_agent(int agent_id);
    void init_agents();
    void wait_for_data();
    void send_game_data();
    void start_game();
    void update();
    void check_and_update_agents();
    void run();
};

#endif /* defined(__XbeeInterface__coordinator__) */