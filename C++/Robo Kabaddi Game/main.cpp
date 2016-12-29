//
//  main.cpp
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 03/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#include "coordinator.h"
#include "player.h"

int main(int argc, const char * argv[])
{
    //Coordinator role
    //Coordinator coordinator;
    //coordinator.run();
    
    //Player role
    Player player;
    player.run();
    
    //Note: run() function puts the character into the game
    
    return 0;
}