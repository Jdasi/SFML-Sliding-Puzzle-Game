#include "GameSettings.h"
#include "GameProfile.h"
#include "FileUtils.h"

#include "MainMenu.h"
#include "PuzzleSelection.h"
#include "ProfileStatistics.h"
#include "ProfileUnlocks.h"

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

    Sprite *logo = Sprite::create("utility/logo.png");
    logo->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 150));
    this->addChild(logo, 1);

    initSettings();
    initProfile();
    initStarDisplay();
    initBackdrop();
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

void MainMenu::initProfile()
{
    if (!GameProfile::isInitialised())
    {
        loadProfile();
        GameProfile::setCurrentBackground
            (GameProfile::getProfileStat(ProfileStat::currentBackground));
        GameProfile::setInitialised(true);
    }
}

void MainMenu::initStarDisplay()
{
    Sprite *star = Sprite::create("utility/star.png");
    star->setPosition(Vec2(visibleSize.width - 200, visibleSize.height - 100));
    star->setScale(0.4f);

    cocos2d::Label *numStars = 
        Label::createWithTTF("numStars", "fonts/Marker Felt.ttf", 24);
    numStars->setPosition(Vec2(star->getPositionX() + 100, star->getPositionY() - 50));
    numStars->setString("x " + GameProfile::getProfileStat(ProfileStat::stars));
    numStars->enableGlow(Color4B::BLACK);

    this->addChild(star, 1);
    this->addChild(numStars, 1);
}

void MainMenu::initBackdrop()
{
    Sprite *backdrop = Sprite::create
        ("backdrops/" + GameProfile::getCurrentBackground() + "_menu.jpg");

    backdrop->setAnchorPoint(Vec2(0, 0));
    backdrop->setPosition(Vec2(0, 0));

    this->addChild(backdrop, 0);
}

void MainMenu::initMenu()
{
    MenuItemSprite *menuPlay = new MenuItemSprite();
    menuPlay->initWithNormalSprite(
        Sprite::create("launchpad/play_up.png"),
        Sprite::create("launchpad/play_dn.png"),
        nullptr,
        CC_CALLBACK_1(MainMenu::gotoPuzzleSelection, this));

    MenuItemSprite *menuUnlocks = new MenuItemSprite();
    menuUnlocks->initWithNormalSprite(
        Sprite::create("launchpad/unlocks_up.png"),
        Sprite::create("launchpad/unlocks_dn.png"),
        nullptr,
        CC_CALLBACK_1(MainMenu::gotoUnlocks, this));

    MenuItemSprite *menuStatistics = new MenuItemSprite();
    menuStatistics->initWithNormalSprite(
        Sprite::create("launchpad/statistics_up.png"),
        Sprite::create("launchpad/statistics_dn.png"),
        nullptr,
        CC_CALLBACK_1(MainMenu::gotoStatistics, this));

    MenuItemSprite *menuExit = new MenuItemSprite();
    menuExit->initWithNormalSprite(
        Sprite::create("launchpad/quit_up.png"),
        Sprite::create("launchpad/quit_dn.png"),
        nullptr,
        CC_CALLBACK_1(MainMenu::exitGame, this));

    cocos2d::Menu *menu = Menu::create
        (menuPlay, menuUnlocks, menuStatistics, menuExit, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height / 2) - 100));

    menuPlay->setPosition(Vec2(-100, 100));
    menuPlay->setScale(0.66f);

    menuUnlocks->setPosition(Vec2(100, 100));
    menuUnlocks->setScale(0.66f);

    menuStatistics->setPosition(Vec2(-100, -100));
    menuStatistics->setScale(0.66f);

    menuExit->setPosition(Vec2(100, -100));
    menuExit->setScale(0.66f);

    this->addChild(menu, 1);
}

void MainMenu::gotoPuzzleSelection(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, PuzzleSelection::createScene()));
}

void MainMenu::gotoUnlocks(cocos2d::Ref* sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, ProfileUnlocks::createScene()));
}

void MainMenu::gotoStatistics(cocos2d::Ref* sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, ProfileStatistics::createScene()));
}

void MainMenu::exitGame(cocos2d::Ref *pSender)
{
    saveProfile();

    Director::getInstance()->end();
}
