#ifndef __CHARACTER_CLASS_H__
#define __CHARACTER_CLASS_H__

#include "cocos2d.h"

class Character : public cocos2d::Sprite
{
protected:
    // The maximum health points that the character can have
    int max_health_points;
    // The maximum shield that the character can have
    int max_defense_points;
    // The amount of body damage a character can deal
    int body_damage;

    // The frame of the status bar
    cocos2d::Sprite* status_bar;
    // The bar showing the health of the player
    cocos2d::Sprite* health_level;
    // The bar showing the shield of the player
    cocos2d::Sprite* defense_level;
public:
    // Whether the character is dead or not
    bool is_dead;
    // An integer denoting the health points of a character
    int health_points;
    // An integer for the shield capacity of a character
    int defense_points;
    // The number of frames that the character cannot attack
    int attack_cooldown;
    // The number of frames that the shield remains deactivated
    int shield_cooldown;
    // Define the character's statistics
    void setStats(int hp, int s, int bd = 0, cocos2d::Color3B color = cocos2d::Color3B(0,0,0));
    // Update the character's statistics as needed
    void updateStats();
    // Is a character touching another character?
    bool isTouching(cocos2d::Sprite* sprite);
    // Deal damage to the character if touching
    void dealDamage(Character* sprite, int scd = 0, int acd = 0);
    // Update the status of the character
    void updateStatus();
    // Stuff
    static Character* create(const std::string& filename);
};

#endif // __CHARACTER_CLASS_H__