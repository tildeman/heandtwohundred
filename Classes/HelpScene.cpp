#include "HelpScene.h"
#include "MenuScene.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;

Scene* HelpScene::createScene() {
    return HelpScene::create();
}

bool HelpScene::init() {
    if (!Scene::init()) return false;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto title = Label::createWithTTF("He And Two Hundred", "fonts/arial.ttf", 80);
    title->setPosition(Vec2(origin.x + visibleSize.width/2,
        origin.y + visibleSize.height - title->getContentSize().height));
    this->addChild(title, 1);

    auto instructions = Label::createWithTTF("Left click to attract\nMiddle click to pan\nRight click to repel\nObscure click to log coordinates\n\nGame developer: tydereee (tildeman123)\nIdea commentator: Algiers eater/Bocja (arczi77)\nBuilt using cocos2d-x", "fonts/arial.ttf", 28);
    instructions->setPosition(Vec2(origin.x + visibleSize.width/3,
        origin.y + visibleSize.height - instructions->getContentSize().height - 100));
    this->addChild(instructions, 1);

    ui::Button* back = ui::Button::create("CloseNormal.png", "CloseSelected.png", "CloseNormal.png");
    back->setPosition(Vec2(10, origin.y + visibleSize.height - 10));
    back->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN){
			Director::getInstance()->replaceScene(MenuScene::createScene());
		}
	});
    back->setAnchorPoint(Vec2(0,1));

    this->addChild(back, 1);

    return true;
}