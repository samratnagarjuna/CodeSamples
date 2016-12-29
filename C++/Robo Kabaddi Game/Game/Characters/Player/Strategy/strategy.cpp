//
//  strategy.cpp
//  XbeeInterface
//
//  Created by Samrat Nagarjuna Pasila on 29/09/14.
//  Copyright (c) 2014 Samrat Nagarjuna Pasila. All rights reserved.
//

#include "strategy.h"
#include <iostream>
#include <cmath>

/**
 * Initializes strategy parameters
 */
Strategy::Strategy() {
    spread = 0;
    rin = 0;
    aggressive = false;
    foffset = 2;
    init_rel_pos();
}

/**
 * Initializes relative position array
 */
void Strategy::init_rel_pos() {
    rel_pos[0] = -1;
    rel_pos[1] = 0;
    rel_pos[2] = 1;
}

/**
 * Sets value of spreads among agents
 */
void Strategy::set_spread(int spread) {
    this->spread = spread;
}

/**
 * Sets radius of formation structure
 */
void Strategy::set_rin(int rin) {
    this->rin = rin;
}

/**
 * Returns the next step of the agent depending upon
 * aggressive or defensive strategy
 * @return Postion that agent should take in next move
 */
Pos2d Strategy::play(const AgentData& own, const AgentData& raider,
                     const std::vector<AgentData>& others,
                     const int formation_id) {
    Pos2d next_pos;
    float h_value, ht_value;
    if (!aggressive) {
        h_value = formationH(own.pos, raider.pos, others, formation_id);
    }
    else {
        h_value = korfH(own.pos, raider.pos, others);
    }
    
    loop(i, 3) {
        loop(j, 3) {
            Pos2d pos;
            pos.set_x(own.pos.get_x() + rel_pos[i]);
            pos.set_y(own.pos.get_y() + rel_pos[j]);
            
            if (!aggressive) {
                ht_value = formationH(own.pos, raider.pos, others,
                                      formation_id);
            }
            else {
                ht_value = korfH(pos, raider.pos, others);
            }
            
            if (h_value > ht_value) {
                h_value = ht_value;
                next_pos = pos;
            }
        }
    }
    
    return next_pos;
}

/**
 * Aggresive play
 * @params
 * own: Players data
 * raider: Raiders data
 * others: Data of other agents in team
 * formation id: Id of formation to take in play
 */
Pos2d Strategy::play_aggressive(const AgentData& own, const AgentData& raider,
                                const std::vector<AgentData>& others,
                                const int formation_id) {
    if (!aggressive) {
        float rdis = get_formation_distance(own.pos, raider.pos, formation_id);
        if (rdis  > rin - foffset && rdis < rin + foffset) {
            bool be_aggressive = true;
            traverse(others, other_ptr) {
                float fdis = get_formation_distance(other_ptr->pos, raider.pos,
                                                    formation_id);
                if(!(fdis > rin - foffset && fdis < rin + foffset)) {
                    be_aggressive = false;
                    break;
                }
            }
            aggressive = be_aggressive;
        }
    }
    return play(own, raider, others, formation_id);
}

/**
 * Defensive play
 * @params
 * own: Players data
 * raider: Raiders data
 * others: Data of other agents in team
 * formation id: Id of formation to take in play
 */
Pos2d Strategy::play_defensive(const AgentData& own, const AgentData& raider,
                                const std::vector<AgentData>& others,
                                const int formation_id) {
    aggressive = (!aggressive) ? raider.is_touched : aggressive;
    return play(own, raider, others, formation_id);
}

/**
 * Anti-raiders next move
 * @params
 * own: Players data
 * raider: Raiders data
 * others: Data of other agents in team
 * @return Next position of anti-raider
 */
Pos2d Strategy::move_anti_raider(const AgentData& own, const AgentData& raider,
                                 const std::vector<AgentData>& others) {
    
    int strategy_id = 0;
    Pos2d next_pos;
    
    switch (strategy_id) {
        case 0:
            //Korf-aggressive
            aggressive = true;
            next_pos = play(own, raider, others, 1);
            break;
        case 1:
            //korf-defensive
            aggressive = false;
            next_pos = play(own, raider, others, 1);
            break;
        case 2:
            //straight-line-aggressive
            spread = 4;
            rin = 3;
            next_pos = play_aggressive(own, raider, others, 0);
            break;
        case 3:
            //straight-line-defensive
            spread = 4;
            rin = 3;
            next_pos = play_defensive(own, raider, others, 0);
            break;
        case 4:
            //semi-circle-aggressive
            spread = 4;
            rin = 3;
            next_pos = play_aggressive(own, raider, others, 1);
            break;
        case 5:
            //semi-circle-defensive
            spread = 4;
            rin = 3;
            next_pos = play_defensive(own, raider, others, 1);
            break;
        case 6:
            //circle-aggresive
            spread = 6;
            rin = 3;
            next_pos = play_aggressive(own, raider, others, 1);
            break;
        case 7:
            //circle-defensive
            spread = 6;
            rin = 3;
            next_pos = play_defensive(own, raider, others, 1);
            break;
        default:
            break;
    }

    return next_pos;
}

/**
 * Move raider
 * @params
 * raider: Raiders data
 * others: Anti-raiders team data
 */
Pos2d Strategy::move_raider(const AgentData &raider,
                            const std::vector<AgentData> &others) {
    Pos2d next_pos;
    next_pos.set_y(raider.pos.get_y());
    if (raider.is_touched) {
        next_pos.set_x(raider.pos.get_x() + 1);
    }
    else {
        next_pos.set_x(raider.pos.get_x() - 1);
    }
    return next_pos;
}

/**
 * Heuristic value for Korf strategy
 * @params
 * pos: Position of agent
 * raider_pos: Raider position
 * others: Data of other agents in team
 */
float Strategy::korfH(const Pos2d& pos, const Pos2d& raider_pos,
                      const std::vector<AgentData>& others) {
    float value = 0;
    float closest_anti_raider_dis = closest_anti_raider(pos, others);
    float raider_dis = pos.get_distance(raider_pos);
    value =  raider_dis - KORF_CONSTANT * closest_anti_raider_dis;
    value += add_borderH(pos);
    return value;
}

/**
 * @return distance of raider from current position depending upon formation
 */
float Strategy::get_formation_distance(const Pos2d &pos,
                                       const Pos2d &raider_pos,
                                       const int formaiton_id) {
    float distance = 0;
    switch (formaiton_id) {
        case 0:
            //Straight line distance
            distance = fabs(raider_pos.get_x() - pos.get_x());
            break;
            
        case 1:
            //curved distance
            distance = pos.get_distance(raider_pos);
            break;
            
        default:
            //cartesian distance
            distance = pos.get_distance(raider_pos);
            break;
    }
    return distance;
}

/**
 * Heuristic value for formation strategy
 */
float Strategy::formationH(const Pos2d &pos, const Pos2d &raider_pos,
                           const std::vector<AgentData> &others,
                           const int formation_id) {
    float value = 0;
    float distance = get_formation_distance(pos, raider_pos, formation_id);
    value = fabs(rin - distance);
    float spread_value = spread - closest_anti_raider(pos, others);
    if (spread_value > 0) {
        value += fabs(spread_value);
    }
    value += add_borderH(pos);
    return value;
}

/**
 * @return distance from closest anti-raider
 */
float Strategy::closest_anti_raider(const Pos2d& pos,
                                    const std::vector<AgentData> &others) {
    float dis = FEILD_WIDTH;
    float tdis = dis;
    traverse(others, other_ptr) {
        tdis = pos.get_distance(other_ptr->pos);
        dis = (dis > tdis) ? tdis : dis;
    }
    return dis;
}

/**
 * @return heuristic value for borders
 */
float Strategy::add_borderH(const Pos2d &pos) {
    float value = 0;
    int x = pos.get_x();
    int y = pos.get_y();
    if (x < BORDER_OFFSET) {
        value = FEILD_WIDTH * 10 - x;
    }
    else if (y < BORDER_OFFSET) {
        value = FEILD_WIDTH * 10 - y;
    }
    else if (y > FEILD_HEIGHT - BORDER_OFFSET) {
        value = FEILD_WIDTH * 10 - (FEILD_HEIGHT - y);
    }
    return value;
}
