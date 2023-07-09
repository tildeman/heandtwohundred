#ifndef __MAIN_GAME_SCENE_H__
#define __MAIN_GAME_SCENE_H__

#include "cocos2d.h"
#include "Character.h"
#include "Survivor.h"
#include <vector>

// The main game where you annihilate the survivor
class MainGame : public cocos2d::Scene
{
protected:
    std::vector<cocos2d::Sprite*> tiles;
    cocos2d::Vec2 offset_coordinates;

    std::vector<int> tileLimits;
    bool move_camera;
    cocos2d::Vec2 coords_startpos;
    cocos2d::EventListenerMouse* mouse_listener;

    cocos2d::Sprite* reddot;
    Survivor* ai_survivor;
    std::vector<Character*> zombies;
    std::vector<cocos2d::Sprite*> coins;
    int zombies_are_moving;

    int zombie_coins, survivor_coins;

    void retile();
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    void update(float delta);

    // Update tile limits with new data
    void updateTileLimits();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // Mouse down event, record the start position
    void onMouseDown(cocos2d::EventMouse* event);
    // Mouse move event, track the move position
    void onMouseMove(cocos2d::EventMouse* event);
    // Mouse up event, deactivate the move triggers
    void onMouseUp(cocos2d::EventMouse* event);

    // Offset a point such that it works with the "camera"
    cocos2d::Vec2 offset(cocos2d::Vec2 invec);
    cocos2d::Vec2 offset(float x, float y);

    // implement the "static create()" method manually
    CREATE_FUNC(MainGame);
};

#endif // __MAIN_GAME_SCENE_H__
