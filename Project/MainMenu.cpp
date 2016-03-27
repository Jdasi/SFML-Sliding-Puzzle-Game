#include "GameSettings.h"
#include "GameProfile.h"
#include "PuzzleGame.h"
#include "MainMenu.h"
#include "PuzzleSelection.h"

#include "FileUtils.h"

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

    cocos2d::Label *sceneTitle = Label::createWithTTF
        ("Sliding Puzzle Game", "fonts/Marker Felt.ttf", 32);
    sceneTitle->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
    this->addChild(sceneTitle, 1);

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
        GameProfile::setInitialised(true);
    }
}

void MainMenu::initStarDisplay()
{
    Sprite *star = Sprite::create("utility/star.png");
    star->setPosition(Vec2(visibleSize.width - 200, visibleSize.height - 100));
    star->setScale(0.4f);

    cocos2d::Label *numStars = Label::createWithTTF("numStars", "fonts/Marker Felt.ttf", 24);
    numStars->setPosition(Vec2(star->getPositionX() + 100, star->getPositionY() - 50));
    numStars->setString("x " + GameProfile::getProfileStat(ProfileSetting::stars));

    this->addChild(star, 1);
    this->addChild(numStars, 1);
}

void MainMenu::initBackdrop()
{
    Sprite *backdrop = Sprite::create("backdrops/regal_menu.jpg");

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
    menu->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height / 2) - 25));

    menuPlay->setPosition(Vec2(-115, 115));
    menuPlay->setScale(0.75);

    menuUnlocks->setPosition(Vec2(115, 115));
    menuUnlocks->setScale(0.75);

    menuStatistics->setPosition(Vec2(-115, -115));
    menuStatistics->setScale(0.75);

    menuExit->setPosition(Vec2(115, -115));
    menuExit->setScale(0.75);

    this->addChild(menu, 1);
}

void MainMenu::gotoPuzzleSelection(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, PuzzleSelection::createScene()));
}

void MainMenu::gotoUnlocks(cocos2d::Ref* sender)
{

}

void MainMenu::gotoStatistics(cocos2d::Ref* sender)
{

}

void MainMenu::exitGame(cocos2d::Ref *pSender)
{
    saveProfile();

    Director::getInstance()->end();
}
