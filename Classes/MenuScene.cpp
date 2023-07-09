#include "MenuScene.h"
#include "MainGameScene.h"
#include "HelpScene.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;

Scene* MenuScene::createScene() {
    return MenuScene::create();
}

bool MenuScene::init() {
    if (!Scene::init()) return false;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto title = Label::createWithTTF("He And Two Hundred", "fonts/arial.ttf", 80);
    title->setPosition(Vec2(origin.x + visibleSize.width/2,
        origin.y + visibleSize.height - title->getContentSize().height));
    this->addChild(title, 1);

    auto stuff = Label::createWithTTF("Welcome to Giżycko. There is no escape.", "fonts/arial.ttf", 28);
    stuff->setPosition(Vec2(origin.x + visibleSize.width/2,
        origin.y + visibleSize.height - stuff->getContentSize().height + 10));
    this->addChild(stuff, 1);

    ui::Button* button = ui::Button::create("button.png", "button.png", "button.png");
    button->setScale(0.5);
    button->setPosition(Vec2(origin.x + visibleSize.width/2,
        (origin.y + visibleSize.height) * 2 / 3));
    button->setTitleText("Play");
    button->setTitleFontName("fonts/arial.ttf");
	button->setTitleFontSize(96);
    button->setTitleColor(Color3B(0,0,0));
    button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN){
			Director::getInstance()->replaceScene(TransitionFade::create(1,MainGame::createScene()));
		}
	});
    this->addChild(button, 2);

    ui::Button* help = ui::Button::create("button.png", "button.png", "button.png");
    help->setScale(0.5);
    help->setPosition(Vec2(origin.x + visibleSize.width/2,
        origin.y + visibleSize.height / 3));
    help->setTitleText("Help");
    help->setTitleFontName("fonts/arial.ttf");
	help->setTitleFontSize(96);
    help->setTitleColor(Color3B(0,0,0));
    help->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN){
			Director::getInstance()->replaceScene(HelpScene::createScene());
		}
	});
    this->addChild(help, 2);

    return true;
}