#include "Ammunition.h"

void Ammunition::setStats(int d, int s, float r, int t){
    damage = d;
    speed = s;
    direction = r;
    duration = t;
    shot = false;
}

void Ammunition::dealDamage(const std::vector<Character*>& targets, int scd){
    for (Character* sprite: targets) {
        if (!sprite->is_dead){
            if (this->getBoundingBox().intersectsRect(sprite->getBoundingBox())) {
                if (this->damage > sprite->defense_points) {
                    int health_penalty = this->damage - sprite->defense_points;
                    if (health_penalty > sprite->health_points) sprite->health_points = 0;
                    else sprite->health_points -= health_penalty;
                    sprite->defense_points = 0;
                    sprite->shield_cooldown = scd * 10;
                }
                else {
                    sprite->defense_points -= this->damage;
                    sprite->shield_cooldown = scd;
                }
                sprite->updateStats();
                shot = true;
            }
        }
    }
}

void Ammunition::move() {
    if (!duration) {
        if (!shot) shot = true;
        return;
    }
    float da = std::sin(direction) * speed, db = std::cos(direction) * speed;
    float a, b;
    this->getPosition(&a, &b);
    this->setPosition(a + da, b + db);
    duration--;
}

Ammunition* Ammunition::create(const std::string& filename)
{
    Ammunition *sprite = new (std::nothrow) Ammunition();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}