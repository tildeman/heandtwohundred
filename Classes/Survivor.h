#ifndef __SURVIVOR_CLASS_H__
#define __SURVIVOR_CLASS_H__

#include "cocos2d.h"
#include "Ammunition.h"
#include <vector>

class Survivor : public Character
{
public:
    int fire_delay;
    int move_delay;
    std::vector<Ammunition*> bullets;
    void shoot(Character* target);
    void update_bullets();
    static Survivor* create(const std::string& filename);
};

#endif // __SURVIVOR_CLASS_H__