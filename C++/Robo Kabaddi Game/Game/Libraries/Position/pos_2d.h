//
//  pos_2d.h
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 17/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#ifndef __XbeeInterface__pos_2d__
#define __XbeeInterface__pos_2d__

class Pos2d {
protected:
    int x;
    int y;
    
public:
    Pos2d();
    Pos2d(int x, int y);
    void set_position(int x,int y);
    void set_x(int x);
    void set_y(int y);
    Pos2d get_position() const;
    int get_x() const;
    int get_y() const;
    float get_distance(Pos2d pos) const;
};

#endif /* defined(__XbeeInterface__pos_2d__) */
