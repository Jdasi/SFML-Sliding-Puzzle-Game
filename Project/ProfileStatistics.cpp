#include "ProfileStatistics.h"
#include "GameProfile.h"
#include "GameSettings.h"
#include "MainMenu.h"
#include "TimeUtils.h"

USING_NS_CC;

Scene *ProfileStatistics::createScene()
{
    Scene *scene = Scene::create();
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
    initMenuPane();
    initStaticLabels();
    initDynamicLabels();

    return true;
}

void ProfileStatistics::initBackdrop()
{
    Sprite *backdrop = Sprite::create
        ("backdrops/" + GameProfile::getCurrentBackground() + "_menu.jpg");

    backdrop->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    this->addChild(backdrop, 0);
}

void ProfileStatistics::initMenu()
{
    MenuItemSprite *menuMain = MenuItemSprite::create(
        Sprite::create("utility/main_up.png"),
        Sprite::create("utility/main_dn.png"),
        nullptr,
        CC_CALLBACK_1(ProfileStatistics::gotoMainMenu, this));

    Menu *menu = Menu::create(menuMain, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    menuMain->setScale(0.66f);
    menuMain->setPosition(Vec2(0, -300));

    this->addChild(menu, 2);
}

void ProfileStatistics::initMenuPane()
{
    Sprite *pane = Sprite::create();
    pane->setTextureRect(Rect(0, 0, 600, 768));
    pane->setPosition(Vec2((visibleSize.width / 2), visibleSize.height / 2));
    pane->setColor(Color3B::BLACK);
    pane->setOpacity(175);

    this->addChild(pane, 1);
}

void ProfileStatistics::initStaticLabels()
{
    Label *playedLabel = Label::createWithTTF
        ("Total Time Played:", GameSettings::getFontName(), 26);
    playedLabel->setPosition(Vec2(visibleSize.width / 2 - 100, visibleSize.height - 200));

    Label *starsLabel = Label::createWithTTF
        ("Total Stars Collected:", GameSettings::getFontName(), 26);
    starsLabel->setPosition
        (Vec2(playedLabel->getPositionX(), playedLabel->getPositionY() - 80));

    Label *movesLabel = Label::createWithTTF
        ("Total Moves Made:", GameSettings::getFontName(), 26);
    movesLabel->setPosition
        (Vec2(starsLabel->getPositionX(), starsLabel->getPositionY() - 80));

    Label *attemptedLabel = Label::createWithTTF
        ("Total Puzzles Attempted:", GameSettings::getFontName(), 26);
    attemptedLabel->setPosition
        (Vec2(movesLabel->getPositionX(), movesLabel->getPositionY() - 80));

    Label *completedLabel = Label::createWithTTF
        ("Total Puzzles Completed:", GameSettings::getFontName(), 26);
    completedLabel->setPosition
        (Vec2(attemptedLabel->getPositionX(), attemptedLabel->getPositionY() - 80));

    this->addChild(playedLabel, 2);
    this->addChild(starsLabel, 2);
    this->addChild(movesLabel, 2);
    this->addChild(attemptedLabel, 2);
    this->addChild(completedLabel, 2);
}

void ProfileStatistics::initDynamicLabels()
{
    Label *playedLabel = Label::createWithTTF
        (Utility::timeToString(GameProfile::getProfileStat(ProfileStat::timePlayed)),
        GameSettings::getFontName(), 26);
    playedLabel->setPosition(Vec2(visibleSize.width / 2 + 150, visibleSize.height - 200));

    Label *starsLabel = Label::createWithTTF
        (GameProfile::getProfileStat(ProfileStat::totalStarsCollected),
        GameSettings::getFontName(), 26);
    starsLabel->setPosition
        (Vec2(playedLabel->getPositionX(), playedLabel->getPositionY() - 80));

    Label *movesLabel = Label::createWithTTF
        (GameProfile::getProfileStat(ProfileStat::totalMoves),
        GameSettings::getFontName(), 26);
    movesLabel->setPosition
        (Vec2(starsLabel->getPositionX(), starsLabel->getPositionY() - 80));

    Label *attemptedLabel = Label::createWithTTF
        (GameProfile::getProfileStat(ProfileStat::puzzlesAttempted), 
        GameSettings::getFontName(), 26);
    attemptedLabel->setPosition
        (Vec2(movesLabel->getPositionX(), movesLabel->getPositionY() - 80));

    Label *completedLabel = Label::createWithTTF
        (GameProfile::getProfileStat(ProfileStat::puzzlesCompleted),
        GameSettings::getFontName(), 26);
    completedLabel->setPosition
        (Vec2(attemptedLabel->getPositionX(), attemptedLabel->getPositionY() - 80));

    this->addChild(playedLabel, 2);
    this->addChild(starsLabel, 2);
    this->addChild(movesLabel, 2);
    this->addChild(attemptedLabel, 2);
    this->addChild(completedLabel, 2);
}

void ProfileStatistics::gotoMainMenu(Ref* const sender) const
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}