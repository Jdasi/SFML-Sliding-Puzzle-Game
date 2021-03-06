#include "GameSettings.h"
#include "GameProfile.h"
#include "FileUtils.h"
#include "MainMenu.h"
#include "PuzzleSelection.h"
#include "ProfileStatistics.h"
#include "ProfileUnlocks.h"

USING_NS_CC;

Scene *MainMenu::createScene()
{
    Scene *scene = Scene::create();
    Layer *layer = MainMenu::create();

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

    initProfile();
    initSettings();
    initBackdrop();
    initStarDisplay();
    initMenu();

    return true;
}

void MainMenu::initProfile() const
{
    if (!GameProfile::isInitialised())
    {
        Utility::loadProfile();
        GameProfile::enumerateUnlocks();
        GameProfile::setInitialised(true);
    }
}

void MainMenu::initSettings() const
{
    if (!GameSettings::isInitialised())
    {
        GameSettings::setFontName("fonts/forte.ttf");
        GameSettings::setSegmentsX(4);
        GameSettings::setSegmentsY(4);
        GameSettings::setInitialised(true);
    }
}

void MainMenu::initBackdrop()
{
    Sprite *backdrop = Sprite::create
        ("backdrops/" + GameProfile::getCurrentBackground() + "_menu.jpg");

    backdrop->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    this->addChild(backdrop, 0);
}

void MainMenu::initStarDisplay()
{
    Sprite *star = Sprite::create("utility/star.png");
    star->setPosition(Vec2(visibleSize.width - 200, visibleSize.height - 100));
    star->setScale(0.4f);

    Label *numStars =
        Label::createWithTTF("numStars", GameSettings::getFontName(), 28);
    numStars->setPosition(Vec2(star->getPositionX() + 100, star->getPositionY() - 50));
    numStars->setString("x " + GameProfile::getProfileStat(ProfileStat::stars));
    numStars->enableGlow(Color4B::BLACK);

    this->addChild(star, 1);
    this->addChild(numStars, 1);
}

void MainMenu::initMenu()
{
    MenuItemSprite *menuPlay = MenuItemSprite::create(
        Sprite::create("launchpad/play_up.png"),
        Sprite::create("launchpad/play_dn.png"),
        nullptr,
        CC_CALLBACK_1(MainMenu::gotoPuzzleSelection, this));

    MenuItemSprite *menuUnlocks = MenuItemSprite::create(
        Sprite::create("launchpad/unlocks_up.png"),
        Sprite::create("launchpad/unlocks_dn.png"),
        nullptr,
        CC_CALLBACK_1(MainMenu::gotoUnlocks, this));

    MenuItemSprite *menuStatistics = MenuItemSprite::create(
        Sprite::create("launchpad/statistics_up.png"),
        Sprite::create("launchpad/statistics_dn.png"),
        nullptr,
        CC_CALLBACK_1(MainMenu::gotoStatistics, this));

    MenuItemSprite *menuExit = MenuItemSprite::create(
        Sprite::create("launchpad/quit_up.png"),
        Sprite::create("launchpad/quit_dn.png"),
        nullptr,
        CC_CALLBACK_1(MainMenu::exitGame, this));

    Menu *menu = Menu::create
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

void MainMenu::gotoPuzzleSelection(Ref* const sender) const
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, PuzzleSelection::createScene()));
}

void MainMenu::gotoUnlocks(Ref* const sender) const
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, ProfileUnlocks::createScene()));
}

void MainMenu::gotoStatistics(Ref* const sender) const
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, ProfileStatistics::createScene()));
}

void MainMenu::exitGame(Ref* const sender) const
{
    Utility::saveProfile();
    Director::getInstance()->end();
}
