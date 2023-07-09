#include "Character.h"

USING_NS_CC;

Rect intersect(Rect a, Rect b){
	int x1=std::max(a.getMinX(),b.getMinX()), x2=std::min(a.getMaxX(),b.getMaxX()), y1=std::max(a.getMinY(),b.getMinY()), y2=std::min(a.getMaxY(),b.getMaxY());
	return Rect(x1,y1,x1-x2,y1-y2);
}

void Character::setStats(int hp, int s, int bd, Color3B color) {
    health_points = hp;
    defense_points = s;
    max_health_points = hp;
    max_defense_points = s;
    body_damage = bd;
    attack_cooldown = 0;
    shield_cooldown = 0;
    is_dead = false;

    this->status_bar = Sprite::create("statusbar.png");
    this->status_bar->setPosition(17, 36);
    this->addChild(this->status_bar, 1);

    this->health_level = Sprite::create("pixel.png");
    this->health_level->setColor(color);
    this->health_level->setScaleY(5);
    this->health_level->setAnchorPoint(Vec2(0, 0.5));
    this->health_level->setPosition(-4, 36);
    this->addChild(this->health_level, 1);

    this->defense_level = Sprite::create("pixel.png");
    this->defense_level->setColor(Color3B(0,0,0));
    this->defense_level->setOpacity(128);
    this->defense_level->setScaleY(5);
    this->defense_level->setAnchorPoint(Vec2(0, 0.5));
    this->defense_level->setPosition(-4, 36);
    this->addChild(this->defense_level, 2);

    this->updateStats();
}

bool Character::isTouching(Sprite* sprite) {
    return this->getBoundingBox().intersectsRect(sprite->getBoundingBox());
}

void Character::updateStats() {
    float hp = health_points, mhp = max_health_points;
    float s = defense_points, ms = max_defense_points;
    this->defense_level->setScaleX(s / ms * 42);
    this->health_level->setScaleX(hp / mhp * 42);
}

void Character::dealDamage(Character* sprite, int scd, int acd) {
    if (this->isTouching(sprite) && !this->attack_cooldown) {
        if (this->body_damage > sprite->defense_points) {
            int health_penalty = this->body_damage - sprite->defense_points;
            if (health_penalty > sprite->health_points) sprite->health_points = 0;
            else sprite->health_points -= health_penalty;
            sprite->defense_points = 0;
            sprite->shield_cooldown = scd * 10;
        }
        else {
            sprite->defense_points -= this->body_damage;
            sprite->shield_cooldown = scd;
        }
        this->attack_cooldown = acd;

        sprite->updateStats();
    }
}

void Character::updateStatus() {
    if (health_points <= 0) {
        if (!is_dead) {
            this->setColor(Color3B(0,0,0));
            this->setRotation(90);
            status_bar->setVisible(false);
            is_dead = true;
        }
        return;
    }
    if (attack_cooldown > 0) attack_cooldown--;
    if (shield_cooldown > 0) shield_cooldown--;
    else {
        if (defense_points < max_defense_points) defense_points++;
    }
    this->updateStats();
}

Character* Character::create(const std::string& filename)
{
    Character *sprite = new (std::nothrow) Character();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}