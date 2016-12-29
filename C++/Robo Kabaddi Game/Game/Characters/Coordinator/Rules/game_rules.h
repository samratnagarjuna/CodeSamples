//
//  game_rules.h
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 27/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#ifndef __XbeeInterface__game_rules__
#define __XbeeInterface__game_rules__

#include "pos_2d.h"
#include "stl.h"

#define TOUCH_DISTANCE 1
#define MID_LINE 400
#define ORIGIN_X 0
#define ORIGIN_Y 0
#define WIDTH 400
#define HEIGHT 400
#define CAPTURE_NUMBER 2

class GameRules {
public:
    bool check_touch(Pos2d a_pos, Pos2d b_pos);
    bool check_border_cross(Pos2d pos);
    bool check_successfull_raid(Pos2d r_pos);
    bool check_capture(std::vector<Pos2d> ar_positions,
                       Pos2d r_pos);
};

#endif /* defined(__XbeeInterface__game_rules__) */
