#include "GameSettings.h"
#include "PuzzleGame.h"
#include "MainMenu.h"
#include "PuzzleSelection.h"

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
    this->addChild(label);

    initSettings();
    initMenu();

    return true;
}

void MainMenu::initSettings()
{
    if (!GameSettings::isInitialised())
    {
        GameSettings::enumeratePuzzles();
        GameSettings::setImageName("puzzles/" + GameSettings::getPuzzles()[0], 0);
        GameSettings::setSegmentsX(4);
        GameSettings::setSegmentsY(4);
        GameSettings::setInitialised(true);
    }
}

void MainMenu::initMenu()
{
    MenuItemFont *menuStart = MenuItemFont::create(
        "Start",
        CC_CALLBACK_1(MainMenu::menuStart, this));

    MenuItemSprite *mainMenuExit = new MenuItemSprite();
    mainMenuExit->initWithNormalSprite(
        Sprite::create("utility/CloseNormal.png"),
        Sprite::create("utility/CloseSelected.png"),
        nullptr,
        CC_CALLBACK_1(MainMenu::menuCloseCallback, this));

    cocos2d::Menu *menu = Menu::create(menuStart, mainMenuExit, nullptr);
    menu->alignItemsVertically();
    this->addChild(menu, 1);
}

void MainMenu::menuStart(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, PuzzleSelection::createScene()));
}

void MainMenu::menuCloseCallback(cocos2d::Ref *pSender)
{
    Director::getInstance()->end();
}
