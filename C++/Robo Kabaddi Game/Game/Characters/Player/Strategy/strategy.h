//
//  strategy.h
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 29/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#ifndef __XbeeInterface__strategy__
#define __XbeeInterface__strategy__

#include "pos_2d.h"
#include "message_protocol.h"
#include "stl.h"

#define FEILD_WIDTH 400
#define FEILD_HEIGHT 400
#define KORF_CONSTANT 0.8
#define BORDER_OFFSET 5

class Strategy {
private:
    int spread;
    int rin;
    bool aggressive;
    int rel_pos[3];
    float foffset;
    float korfH(const Pos2d& pos, const Pos2d& raider_pos,
                const std::vector<AgentData>& others);
    float closest_anti_raider(const Pos2d& pos,
                              const std::vector<AgentData>& others);
    float add_borderH(const Pos2d& pos);
    Pos2d play(const AgentData& own, const AgentData& raider,
               const std::vector<AgentData>& others, const int formation_id);
    float formationH(const Pos2d& pos, const Pos2d& raider_pos,
                     const std::vector<AgentData>& others,
                     const int formation_id);
    float get_formation_distance(const Pos2d& pos, const Pos2d& raider_pos,
                                 const int formaiton_id);
    Pos2d play_aggressive(const AgentData& own, const AgentData& raider,
                          const std::vector<AgentData>& others,
                          const int formation_id);
    Pos2d play_defensive(const AgentData& own, const AgentData& raider,
                         const std::vector<AgentData>& others,
                         const int formation_id);
    void init_rel_pos();
    
public:
    Strategy();
    void set_spread(int spread);
    void set_rin(int rin);
    Pos2d move_raider(const AgentData& raider,
                      const std::vector<AgentData>& others);
    Pos2d move_anti_raider(const AgentData& own, const AgentData& raider,
                           const std::vector<AgentData>& others);
    
};

#endif /* defined(__XbeeInterface__strategy__) */
