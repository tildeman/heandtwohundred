#ifndef __AMMUNITION_CLASS_H__
#define __AMMUNITION_CLASS_H__

#include "cocos2d.h"
#include "Character.h"
#include <vector>

class Ammunition : public cocos2d::Sprite
{
protected:
    // The amount of damage the ammunition can deal
    int damage;
    // The speed of the ammunition
    int speed;
    // Direction of the ammunition, in radians
    float direction;
    // The number of frames that the ammunition exists
    int duration;
public:
    bool shot;
    // Define the ammunition's statistics
    void setStats(int d, int s, float r, int t);
    // Deal damage to the character if touching
    void dealDamage(const std::vector<Character*>& targets, int scd = 0);
    // Move in the specified direction
    void move();
    // Stuff
    static Ammunition* create(const std::string& filename);
};

#endif // __AMMUNITION_CLASS_H__