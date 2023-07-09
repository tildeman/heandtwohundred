
#include "MainGameScene.h"
#include <cmath>
#include <random>

USING_NS_CC;

double pi = 3.14159265358979323846;

int TILE_WIDTH = 32;
int TILE_HEIGHT = 32;

std::random_device device;
std::mt19937 generator(device());

cocos2d::Vec2 mouse_position;

Scene* MainGame::createScene() {
    return MainGame::create();
}

float distance(Vec2 p1, Vec2 p2){
    float dx = p2.x - p1.x, dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

void MainGame::onMouseDown(cocos2d::EventMouse* event) {
    switch((int) event->getMouseButton()) {
        case GLFW_MOUSE_BUTTON_MIDDLE:
        move_camera = true;
        coords_startpos.set(event->getCursorX(), event->getCursorY());
        break;
        case GLFW_MOUSE_BUTTON_RIGHT:
        zombies_are_moving = true + true;
        mouse_position.set(event->getCursorX(), event->getCursorY());
        break;
        case GLFW_MOUSE_BUTTON_4:
        log("Position: %.2f %.2f\nTile limits: (%d, %d) and (%d, %d)",
            event->getCursorX() + offset_coordinates.x, event->getCursorY() + offset_coordinates.y,
            tileLimits[0], tileLimits[1], tileLimits[2], tileLimits[3]);
        break;
        case GLFW_MOUSE_BUTTON_LEFT:
        zombies_are_moving = true;
        mouse_position.set(event->getCursorX(), event->getCursorY());
    }
}

void MainGame::onMouseMove(cocos2d::EventMouse* event) {
    if (move_camera) {
        offset_coordinates.set(
            offset_coordinates.x + coords_startpos.x - event->getCursorX(),
            offset_coordinates.y + coords_startpos.y - event->getCursorY()
        );

        float asx,asy;
        ai_survivor->getPosition(&asx, &asy);
        ai_survivor->setPosition(
            asx - coords_startpos.x + event->getCursorX(),
            asy - coords_startpos.y + event->getCursorY()
        );

        for (int i = 0; i < zombies.size(); i++) {
            zombies[i]->getPosition(&asx, &asy);
            zombies[i]->setPosition(
                asx - coords_startpos.x + event->getCursorX(),
                asy - coords_startpos.y + event->getCursorY()
            );
        }

        for (int i = 0; i < ai_survivor->bullets.size(); i++) {
            ai_survivor->bullets[i]->getPosition(&asx, &asy);
            ai_survivor->bullets[i]->setPosition(
                asx - coords_startpos.x + event->getCursorX(),
                asy - coords_startpos.y + event->getCursorY()
            );
        }

        coords_startpos.set(event->getCursorX(), event->getCursorY());
    }
}

void MainGame::onMouseUp(cocos2d::EventMouse* event) {
    move_camera = false;
    zombies_are_moving = false;
}

void MainGame::updateTileLimits(){
    Vec2 origin_coords = Director::getInstance()->getVisibleOrigin();
    Size size = Director::getInstance()->getVisibleSize();
    Vec2 corner_coords = Vec2(origin_coords.x + size.width, origin_coords.y + size.height);

    Vec2 origin_shifted = Vec2(
        origin_coords.x + offset_coordinates.x, origin_coords.y + offset_coordinates.y);
    Vec2 corner_shifted = Vec2(
        corner_coords.x + offset_coordinates.x, corner_coords.y + offset_coordinates.y);

    tileLimits = { 
        (int) std::floor(origin_shifted.x / TILE_WIDTH),
        (int) std::floor(origin_shifted.y / TILE_HEIGHT),
        (int) std::floor(corner_shifted.x / TILE_WIDTH),
        (int) std::floor(corner_shifted.y / TILE_HEIGHT)};
}

void MainGame::retile(){
    this->updateTileLimits();
    int k = 0, i, j;

    for (i = tileLimits[0]; i <= tileLimits[2]; i++) {
        for (j = tileLimits[1]; j <= tileLimits[3]; j++) {
            tiles[k]->setVisible(true);
            tiles[k]->setPosition(offset(i * TILE_WIDTH, j * TILE_HEIGHT));
            k++;
        }
    }

    for (i = k; i < tiles.size(); i++) {
        tiles[i]->setVisible(false);
    }
}

Vec2 MainGame::offset(Vec2 invec){
    return Vec2(invec.x - offset_coordinates.x, invec.y - offset_coordinates.y);
}
Vec2 MainGame::offset(float x, float y){
    return Vec2(x - offset_coordinates.x, y - offset_coordinates.y);
}

bool MainGame::init(){
    if (!Scene::init()) return false;

    offset_coordinates.set(Vec2(-300,-300));
    this->updateTileLimits();
    zombies_are_moving = false;
    zombie_coins = 0;
    survivor_coins = 0;
    int tileCount = (tileLimits[2] - tileLimits[0] + 2) * (tileLimits[3] - tileLimits[1] + 2);
    tiles.resize(tileCount);
    for (int i = 0; i < tileCount; i++) {
        tiles[i] = Sprite::create("tile.png"); // Contributor: Bocja the Algiers eater
        tiles[i]->setAnchorPoint(Vec2::ZERO);
        this->addChild(tiles[i], 1);
    }

    mouse_listener = EventListenerMouse::create();
    mouse_listener->onMouseDown = CC_CALLBACK_1(MainGame::onMouseDown, this);
    mouse_listener->onMouseMove = CC_CALLBACK_1(MainGame::onMouseMove, this);
    mouse_listener->onMouseUp = CC_CALLBACK_1(MainGame::onMouseUp, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse_listener, this);

    Size size = Director::getInstance()->getVisibleSize();
    std::uniform_int_distribution<> xrandom(0, (int) size.width);
    std::uniform_int_distribution<> yrandom(0, (int) size.height);

    ai_survivor = Survivor::create("survivor.png");
    ai_survivor->setStats(5000, 2000, 0, Color3B(225, 53, 69));


    ai_survivor->setPosition(offset(xrandom(generator), yrandom(generator)));
    this->addChild(ai_survivor, 3);


    zombies.resize(200);
    for (int i = 0; i < 200; i++) {
        zombies[i] = Character::create("zombie.png");
        zombies[i]->setPosition(xrandom(generator),yrandom(generator));
        zombies[i]->setStats(8, 8, 8, Color3B(96, 114, 74));
        this->addChild(zombies[i], 2);
    }

    float framerate = 1 / Director::getInstance()->getAnimationInterval();
    ai_survivor->fire_delay = (int) round(framerate * 3);

    // Hack: an object's position can be modified during movement.
    // MoveTo* mover = MoveTo::create(5, offset(360, 360));
    // ai_survivor->runAction(mover);

    this->scheduleUpdate();
    return true;
}

void MainGame::update(float delta){
    this->retile();

    float d, a1, a2, b1, b2, a, b, theta, da, db;
    float framerate = 1 / Director::getInstance()->getAnimationInterval();

    for (int i = 0; i < zombies.size(); i++) {
        if (!zombies[i]->is_dead){
            if (zombies_are_moving) {
                d = distance(zombies[i]->getPosition(), mouse_position);
                if (d > 20) {
                    a1 = mouse_position.x;
                    b1 = mouse_position.y;
                    zombies[i]->getPosition(&a2, &b2);
                    a = a2 - a1;
                    b = b2 - b1;
                    theta = std::atan2(a,b);
                    // Modify this later to include levelups
                    da = std::sin(theta), db = std::cos(theta);
                    if (zombies_are_moving == 1) zombies[i]->setPosition(a2 - da, b2 - db);
                    else zombies[i]->setPosition(a2 + da, b2 + db);
                }
                else if (zombies_are_moving == 2) {
                    std::uniform_real_distribution<> drc(-pi, pi);
                    theta = drc(generator);
                    da = std::sin(theta) * 20, db = std::cos(theta) * 20;
                    zombies[i]->getPosition(&a2, &b2);
                    zombies[i]->setPosition(a2 + da, b2 + db);
                }
            }
            zombies[i]->dealDamage(ai_survivor, (int) std::round(0.75 * framerate), (int) std::round(0.75 * framerate));

            zombies[i]->updateStatus();
        }
    }
    ai_survivor->updateStatus();
    for (std::vector<Ammunition*>::iterator i = ai_survivor->bullets.begin(); i != ai_survivor->bullets.end();){
        (*i)->move();
        (*i)->dealDamage(zombies, (int) std::round(0.75 * framerate));
        if ((*i)->shot) {
            this->removeChild(*i, true);
            i = (ai_survivor->bullets).erase(i);
            continue;
        }
        i++;
    }
    std::uniform_int_distribution<> xpos(-320, 320);
    std::uniform_int_distribution<> ypos(-320, 320);

    (ai_survivor->fire_delay)--;
    if (!ai_survivor->fire_delay && !ai_survivor->is_dead) {
        std::uniform_int_distribution<> zombie_select(0, zombies.size() - 1);
        ai_survivor->fire_delay = (int) round(1 * framerate);
        ai_survivor->shoot(zombies[zombie_select(generator)]);
    }
}