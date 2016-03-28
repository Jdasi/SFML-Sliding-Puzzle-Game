#include "ProfileUnlocks.h"
#include "GameProfile.h"
#include "MainMenu.h"

USING_NS_CC;

cocos2d::Scene *ProfileUnlocks::createScene()
{
    cocos2d::Scene *scene = Scene::create();
    auto layer = ProfileUnlocks::create();

    scene->addChild(layer);

    return scene;
}

bool ProfileUnlocks::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Sprite *sceneTitle = Sprite::create("utility/unlocks.png");
    sceneTitle->setPosition
        (Vec2((visibleSize.width / 2), visibleSize.height - 100));

    this->addChild(sceneTitle, 2);

    initBackdrop();
    initMenu();

    return true;
}

void ProfileUnlocks::initBackdrop()
{
    Sprite *backdrop = Sprite::create
        ("backdrops/" + GameProfile::getCurrentBackground() + "_menu.jpg");

    backdrop->setAnchorPoint(Vec2(0, 0));
    backdrop->setPosition(Vec2(0, 0));

    this->addChild(backdrop, 0);
}

void ProfileUnlocks::initMenu()
{

    MenuItemSprite *menuMain = new MenuItemSprite();
    menuMain->initWithNormalSprite(
        Sprite::create("utility/main_up.png"),
        Sprite::create("utility/main_dn.png"),
        nullptr,
        CC_CALLBACK_1(ProfileUnlocks::gotoMainMenu, this));

    cocos2d::Menu *menu = Menu::create(menuMain, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    menuMain->setScale(0.66f);
    menuMain->setPosition(Vec2(0, -300));

    this->addChild(menu, 2);
}

void ProfileUnlocks::gotoMainMenu(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}