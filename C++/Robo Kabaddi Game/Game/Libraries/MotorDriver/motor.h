//
//  motor.h
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 21/10/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#ifndef __XbeeInterface__motor__
#define __XbeeInterface__motor__

#include "pos_2d.h"

class Motor {
public:
    Motor();
    bool is_driver_setup();
    bool send_motion_command(const Pos2d& curr_pos, const Pos2d& next_pos);
    void move(int command); // For Testing
    
private:
    int orientation;
    bool driver_successful;
    void driver_setup();
    int get_motion_command_id(const Pos2d& curr_pos, const Pos2d& next_pos);
    bool is_valid_move(const Pos2d& curr_pos, const Pos2d& next_pos);
    void move_forward();
    void move_backward();
    void rotate_left(int angle);
    void rotate_right(int angle);
    void rotate_45_left();
    void rotate_45_right();
    void rotate_90_left();
    void rotate_90_right();
    void stop();
    
};

#endif /* defined(__XbeeInterface__motor__) */
