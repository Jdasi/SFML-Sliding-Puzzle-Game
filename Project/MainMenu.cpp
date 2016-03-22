#include "GameSettings.h"
#include "PuzzleScene.h"
#include "MainMenu.h"
#include "SettingsMenu.h"
USING_NS_CC;

cocos2d::Scene *MainMenu::createScene()
{
    cocos2d::Scene *scene = Scene::create();
    auto layer = MainMenu::create();

    scene->addChild(layer);

    return scene;
}

bool MainMenu::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    cocos2d::Label *label = Label::createWithTTF("Sliding Puzzle", "fonts/Marker Felt.ttf", 32);
    label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
    this->addChild(label, 1);

    initSettings();
    initMenu();

    return true;
}

void MainMenu::initSettings()
{
    if (!GameSettings::isInitialised())
    {
        GameSettings::setImageName("puzzles/bridge.jpg", 0);
        GameSettings::setSegmentsX(4);
        GameSettings::setSegmentsY(4);
        GameSettings::setInitialised(true);
    }
}

void MainMenu::initMenu()
{
    MenuItemFont *mainMenuStartGame = MenuItemFont::create(
        "Play Game",
        CC_CALLBACK_1(MainMenu::menuStartGame, this));

    MenuItemFont *menuSettings = MenuItemFont::create(
        "Settings",
        CC_CALLBACK_1(MainMenu::menuSettings, this));

    MenuItemSprite *mainMenuExit = new MenuItemSprite();
    mainMenuExit->initWithNormalSprite(
        Sprite::create("CloseNormal.png"),
        Sprite::create("CloseSelected.png"),
        nullptr,
        CC_CALLBACK_1(MainMenu::menuCloseCallback, this));

    cocos2d::Menu *menu = Menu::create(mainMenuStartGame, menuSettings, mainMenuExit, nullptr);
    menu->alignItemsVertically();
    this->addChild(menu, 1);
}

void MainMenu::menuStartGame(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, PuzzleScene::createScene()));
}

void MainMenu::menuSettings(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, SettingsMenu::createScene()));
}

void MainMenu::menuCloseCallback(cocos2d::Ref *pSender)
{
    Director::getInstance()->end();
}
