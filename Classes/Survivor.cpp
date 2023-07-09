#include "Survivor.h"
#include "MainGameScene.h"

void Survivor::shoot(Character* target){
    float framerate = 1 / cocos2d::Director::getInstance()->getAnimationInterval();
    Ammunition* bullet = Ammunition::create("bullet.png");
    MainGame* parent = (MainGame*) this->getParent();
    bullet->setPosition(this->getPosition());

    float target_x, target_y, source_x, source_y;
    target->getPosition(&target_x, &target_y);
    this->getPosition(&source_x, &source_y);
    float x = target_x - source_x, y = target_y - source_y;

    bullet->setStats(10, 3, atan2(x, y), 300);
    bullets.push_back(bullet);
    parent->addChild(bullet, 3);
}

Survivor* Survivor::create(const std::string& filename)
{
    Survivor *sprite = new (std::nothrow) Survivor();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}