#include "ProfileStatistics.h"
#include "GameProfile.h"
#include "MainMenu.h"

USING_NS_CC;

cocos2d::Scene *ProfileStatistics::createScene()
{
    cocos2d::Scene *scene = Scene::create();
    auto layer = ProfileStatistics::create();

    scene->addChild(layer);

    return scene;
}

bool ProfileStatistics::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Sprite *sceneTitle = Sprite::create("utility/statistics.png");
    sceneTitle->setPosition
        (Vec2((visibleSize.width / 2), visibleSize.height - 100));

    this->addChild(sceneTitle, 2);

    initBackdrop();
    initMenu();

    return true;
}

void ProfileStatistics::initBackdrop()
{
    Sprite *backdrop = Sprite::create
        ("backdrops/" + GameProfile::getCurrentBackground() + "_menu.jpg");

    backdrop->setAnchorPoint(Vec2(0, 0));
    backdrop->setPosition(Vec2(0, 0));

    this->addChild(backdrop, 0);
}

void ProfileStatistics::initMenu()
{
    MenuItemSprite *menuMain = new MenuItemSprite();
    menuMain->initWithNormalSprite(
        Sprite::create("utility/main_up.png"),
        Sprite::create("utility/main_dn.png"),
        nullptr,
        CC_CALLBACK_1(ProfileStatistics::gotoMainMenu, this));

    cocos2d::Menu *menu = Menu::create(menuMain, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    menuMain->setScale(0.66f);
    menuMain->setPosition(Vec2(0, -300));

    Sprite *pane = Sprite::create("utility/pane.png");
    pane->setPosition(Vec2((visibleSize.width / 2), visibleSize.height / 2));
    pane->setScaleX(2.5f);

    this->addChild(menu, 2);
    this->addChild(pane, 1);
}

void ProfileStatistics::gotoMainMenu(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}