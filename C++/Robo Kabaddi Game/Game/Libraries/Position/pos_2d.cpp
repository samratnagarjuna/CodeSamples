//
//  pos_2d.cpp
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 17/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#include "pos_2d.h"
#include <cmath>

Pos2d::Pos2d() {
    this->x = 0;
    this->y = 0;
}

Pos2d::Pos2d(int x, int y) {
    set_position(x, y);
}

void Pos2d::set_position(int x, int y) {
    this->x = x;
    this->y = y;
}

void Pos2d::set_x(int x) {
    this->x = x;
}

void Pos2d::set_y(int y) {
    this->y = y;
}

Pos2d Pos2d::get_position() const{
    return *this;
}

int Pos2d::get_x() const{
    return x;
}

int Pos2d::get_y() const{
    return y;
}

float Pos2d::get_distance(Pos2d pos) const {
    return sqrtf((x - pos.x) * (x - pos.x) +
                 (y - pos.y) * (y - pos.y));
}