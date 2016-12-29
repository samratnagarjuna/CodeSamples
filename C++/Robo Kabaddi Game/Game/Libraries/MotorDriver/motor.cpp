//
//  motor.cpp
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 21/10/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#include "motor.h"
#include <iostream>
#include <cmath>
#include <wiringPi.h>

Motor::Motor() {
    driver_successful = false;
    orientation = 0;
    driver_setup();
}

/**
 * Setup GPIO for motor driver
 */
void Motor::driver_setup() {
    if (wiringPiSetup() != -1) {
        driver_successful = true;
        std::cout << "Driver Successfully setup" << std::endl;
    }
    if (!driver_successful) {
        std::cout << "Motor driver failed to setup" << std::endl;
    }
}

/**
 * Check if driver is successfully setup
 */
bool Motor::is_driver_setup() {
    return driver_successful;
}

/**
 * Send corresponding action command based on current and next position
 */
bool Motor::send_motion_command(const Pos2d &curr_pos, const Pos2d &next_pos) {
    if (is_driver_setup()) {
        int command = get_motion_command_id(curr_pos, next_pos);
        std::cout << "Cmd: " << command << std::endl;
        move(command);
        return true;
    }
    else return false;
}

/**
 * Move to corresponding cell based on its current state of orientation and
 * command
 */
void Motor::move(int command) {
    if (orientation == 0) {
        switch (command) {
            case 0:
                stop();
                break;

            case 1:
                rotate_45_left();
                move_forward();
                orientation = 7;
                break;
                
            case 2:
                move_forward();
                orientation = 0;
                break;
                
            case 3:
                rotate_45_right();
                move_forward();
                orientation = 1;
                break;
                
            case 4:
                rotate_90_right();
                move_forward();
                orientation = 2;
                break;
                
            case 5:
                rotate_45_left();
                move_backward();
                orientation = 7;
                break;
                
            case 6:
                move_backward();
                orientation = 0;
                break;
                
            case 7:
                rotate_45_right();
                move_backward();
                orientation = 1;
                break;
                
            case 8:
                rotate_90_left();
                move_forward();
                orientation = 6;
                break;
                
            default:
                break;
        }
    }
    else if (orientation == 1) {
        switch (command) {
            case 0:
                stop();
                break;
                
            case 1:
                rotate_90_left();
                move_forward();
                orientation = 7;
                break;
                
            case 2:
                rotate_45_left();
                move_forward();
                orientation = 0;
                break;
                
            case 3:
                move_forward();
                orientation = 1;
                break;
                
            case 4:
                rotate_45_right();
                move_forward();
                orientation = 2;
                break;
                
            case 5:
                rotate_90_right();
                move_forward();
                orientation = 3;
                break;
                
            case 6:
                rotate_45_left();
                move_backward();
                orientation = 0;
                break;
                
            case 7:
                move_backward();
                orientation = 1;
                break;
                
            case 8:
                rotate_45_right();
                move_backward();
                orientation = 6;
                break;
                
            default:
                break;
        }
    }
    else if (orientation == 2) {
        switch (command) {
            case 0:
                stop();
                break;
                
            case 1:
                rotate_45_right();
                move_backward();
                orientation = 3;
                break;
                
            case 2:
                rotate_90_left();
                move_forward();
                orientation = 0;
                break;
                
            case 3:
                rotate_45_left();
                move_forward();
                orientation = 1;
                break;
                
            case 4:
                move_forward();
                orientation = 2;
                break;
                
            case 5:
                rotate_45_right();
                move_forward();
                orientation = 3;
                break;
                
            case 6:
                rotate_90_right();
                move_forward();
                orientation = 4;
                break;
                
            case 7:
                rotate_45_left();
                move_backward();
                orientation = 1;
                break;
                
            case 8:
                move_backward();
                orientation = 2;
                break;
                
            default:
                break;
        }
    }
    else if (orientation == 3) {
        switch (command) {
            case 0:
                stop();
                break;
                
            case 1:
                rotate_45_right();
                move_backward();
                orientation = 3;
                break;
                
            case 2:
                rotate_90_left();
                move_forward();
                orientation = 0;
                break;
                
            case 3:
                rotate_45_left();
                move_forward();
                orientation = 1;
                break;
                
            case 4:
                move_forward();
                orientation = 2;
                break;
                
            case 5:
                rotate_45_right();
                move_forward();
                orientation = 3;
                break;
                
            case 6:
                rotate_90_right();
                move_forward();
                orientation = 4;
                break;
                
            case 7:
                rotate_45_left();
                move_backward();
                orientation = 1;
                break;
                
            case 8:
                move_backward();
                orientation = 2;
                break;
                
            default:
                break;
        }
    }
    else if (orientation == 4) {
        switch (command) {
            case 0:
                stop();
                break;
                
            case 1:
                rotate_45_left();
                move_backward();
                orientation = 3;
                break;
                
            case 2:
                move_backward();
                orientation = 4;
                break;
                
            case 3:
                rotate_45_right();
                move_backward();
                orientation = 5;
                break;
                
            case 4:
                rotate_90_left();
                move_forward();
                orientation = 2;
                break;
                
            case 5:
                rotate_45_left();
                move_forward();
                orientation = 3;
                break;
                
            case 6:
                move_forward();
                orientation = 4;
                break;
                
            case 7:
                rotate_45_right();
                move_forward();
                orientation = 5;
                break;
                
            case 8:
                rotate_90_right();
                move_forward();
                orientation = 6;
                break;
                
            default:
                break;
        }
    }
    else if (orientation == 5) {
        switch (command) {
            case 0:
                stop();
                break;
                
            case 1:
                rotate_90_right();
                move_forward();
                orientation = 7;
                break;
                
            case 2:
                rotate_45_left();
                move_backward();
                orientation = 4;
                break;
                
            case 3:
                move_backward();
                orientation = 5;
                break;
                
            case 4:
                rotate_45_right();
                move_backward();
                orientation = 6;
                break;
                
            case 5:
                rotate_90_left();
                move_forward();
                orientation = 3;
                break;
                
            case 6:
                rotate_45_left();
                move_forward();
                orientation = 4;
                break;
                
            case 7:
                move_forward();
                orientation = 5;
                break;
                
            case 8:
                rotate_45_right();
                move_forward();
                orientation = 6;
                break;
                
            default:
                break;
        }
    }
    else if (orientation == 6) {
        switch (command) {
            case 0:
                stop();
                break;
                
            case 1:
                rotate_45_right();
                move_forward();
                orientation = 7;
                break;
                
            case 2:
                rotate_90_right();
                move_forward();
                orientation = 0;
                break;
                
            case 3:
                rotate_45_left();
                move_backward();
                orientation = 5;
                break;
                
            case 4:
                move_backward();
                orientation = 6;
                break;
                
            case 5:
                rotate_45_right();
                move_backward();
                orientation = 7;
                break;
                
            case 6:
                rotate_90_left();
                move_forward();
                orientation = 4;
                break;
                
            case 7:
                rotate_45_left();
                move_forward();
                orientation = 5;
                break;
                
            case 8:
                move_forward();
                orientation = 6;
                break;
                
            default:
                break;
        }
    }
    else if (orientation == 7) {
        switch (command) {
            case 0:
                stop();
                break;
                
            case 1:
                move_forward();
                orientation = 7;
                break;
                
            case 2:
                rotate_45_right();
                move_forward();
                orientation = 0;
                break;
                
            case 3:
                rotate_90_right();
                move_forward();
                orientation = 1;
                break;
                
            case 4:
                rotate_45_left();
                move_backward();
                orientation = 6;
                break;
                
            case 5:
                move_backward();
                orientation = 7;
                break;
                
            case 6:
                rotate_45_right();
                move_backward();
                orientation = 0;
                break;
                
            case 7:
                rotate_90_left();
                move_forward();
                orientation = 5;
                break;
                
            case 8:
                rotate_45_left();
                move_forward();
                orientation = 6;
                break;
                
            default:
                break;
        }
    }
    stop();
}

/**
 * Get command identifier based on the current and next positions
 */
int Motor::get_motion_command_id(const Pos2d &curr_pos, const Pos2d &next_pos) {
    int command = -1;
    if (is_valid_move(curr_pos, next_pos)) {
        short int x_offset = next_pos.get_x() - curr_pos.get_x();
        short int y_offset = next_pos.get_y() - curr_pos.get_y();
        
        if(x_offset == 1 && y_offset == 0) command = 2;
        else if(x_offset == 1 && y_offset == 1) command = 1;
        else if(x_offset == 1 && y_offset == -1) command = 3;
        else if(x_offset == 0 && y_offset == 1) command = 8;
        else if(x_offset == 0 && y_offset == -1) command = 4;
        else if(x_offset == -1 && y_offset == 0) command = 6;
        else if(x_offset == -1 && y_offset == 1) command = 7;
        else if(x_offset == -1 && y_offset == -1) command = 5;
        else if(x_offset == 0 && y_offset == 0) command = 0;
        return command;
    }
    return -1;
}

/**
 * Check if the next position is valid move
 */
bool Motor::is_valid_move(const Pos2d &curr_pos, const Pos2d &next_pos) {
    short int x_offset = fabs(next_pos.get_x() - curr_pos.get_x());
    short int y_offset = fabs(next_pos.get_y() - curr_pos.get_y());
    return (x_offset == 0 || x_offset == 1) && (y_offset == 0 || y_offset == 1);
}

/** Move functions **/

void Motor::move_forward() {
    digitalWrite (7, 1);
    digitalWrite (2, 0);
    digitalWrite (0, 1);
    digitalWrite (3, 0);
    delay(570);
}

void Motor::move_backward() {
    digitalWrite (7, 0);
    digitalWrite (2, 1);
    digitalWrite (0, 0);
    digitalWrite (3, 1);
    delay(750);
}

void Motor::rotate_45_left() {
    digitalWrite (7, 1);
    digitalWrite (2, 0);
    digitalWrite (0, 0);
    digitalWrite (3, 1);
    delay(300);
}

void Motor::rotate_90_left() {
    digitalWrite (7, 1);
    digitalWrite (2, 0);
    digitalWrite (0, 0);
    digitalWrite (3, 1);
    delay(600);
}

void Motor::rotate_45_right() {
    digitalWrite (7, 0);
    digitalWrite (2, 1);
    digitalWrite (0, 1);
    digitalWrite (3, 0);
    delay(300);
}

void Motor::rotate_90_right() {
    digitalWrite (7, 0);
    digitalWrite (2, 1);
    digitalWrite (0, 1);
    digitalWrite (3, 0);
    delay(600);
}

void Motor::rotate_left(int angle) {
    
}

void Motor::rotate_right(int angle) {
    
}

void Motor::stop() {
    digitalWrite (7, 0);
    digitalWrite (2, 0);
    digitalWrite (0, 0);
    digitalWrite (3, 0) ;
}
