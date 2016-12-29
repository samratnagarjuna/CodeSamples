//
//  game_rules.cpp
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 27/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#include "game_rules.h"
#include <cmath>

/**
 * Check if there is touch between two agents
 * @params: Position of two agents
 * @return True if there is touch else false
 */
bool GameRules::check_touch(Pos2d a_pos, Pos2d b_pos) {
    return fabs(a_pos.get_x() - b_pos.get_x()) <= TOUCH_DISTANCE &&
        fabs(a_pos.get_y() - b_pos.get_y()) <= TOUCH_DISTANCE;
}

/**
 * Checks if an agent crossed the border
 * @param: Position of agent
 * @return: True if there is border cross else False
 */
bool GameRules::check_border_cross(Pos2d pos) {
    int x = pos.get_x();
    int y = pos.get_y();
    return x < ORIGIN_X || y < ORIGIN_Y || x > WIDTH || y > HEIGHT;
}
/**
 * Checks if the raider returned back to its arena
 * @param: Position of raider
 * @return: True if raider crosses the mid line else False
 */
bool GameRules::check_successfull_raid(Pos2d r_pos) {
    return r_pos.get_x() - MID_LINE >= 0;
}

/**
 * Checks if the anti-raiders captures the raider
 * @params: Postions of anti-raiders and position of raider
 # @return: True if the more than 2 anti-raiders touch the raider
 */
bool GameRules::check_capture(std::vector<Pos2d> ar_positions,
                              Pos2d r_pos) {
    int touch_count = 0;
    traverse(ar_positions, pos_ptr) {
        if (check_touch(*pos_ptr, r_pos)) {
            touch_count++;
            if (touch_count == CAPTURE_NUMBER) {
                return true;
            }
        }
    }
    return false;
}