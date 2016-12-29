//
//  player.h
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 19/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#ifndef __XbeeInterface__player__
#define __XbeeInterface__player__

#include "xbee_api.h"
#include "simple_agent.h"
#include "message_protocol.h"
#include "strategy.h"
#include "motor.h"
#include "stl.h"

#define RECEIVE_WAIT 1
#define RAIDER_MODE 1
#define ANTI_RAIDER_MODE 0

class Player {
private:
    XbeeAPI* xbee_api;
    MessageAPI message_api;
    Agent agent;
    Strategy strategy;
    Motor motor;
    
public:
    Player();
    ~Player();
    void get_init_command();
    void wait_for_data();
    void update();
    void run();
};

#endif /* defined(__XbeeInterface__player__) */
