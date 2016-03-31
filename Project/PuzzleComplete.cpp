#include "PuzzleComplete.h"
#include "GameSettings.h"
#include "GameProfile.h"
#include "MainMenu.h"
#include "PuzzleSelection.h"

USING_NS_CC;

cocos2d::Scene *PuzzleComplete::createScene()
{
    cocos2d::Scene *scene = Scene::create();
    auto layer = PuzzleComplete::create();

    scene->addChild(layer);

    return scene;
}

bool PuzzleComplete::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Sprite *sceneTitle = Sprite::create("utility/win.png");
    sceneTitle->setPosition
        (Vec2(visibleSize.width - 210, visibleSize.height - 100));

    this->addChild(sceneTitle, 2);

    initBackdrop();
    initMenu();
    initPreviewImage();

    rewardPlayer();

    return true;
}

void PuzzleComplete::initBackdrop()
{
    Sprite *backdrop = Sprite::create
        ("backdrops/" + GameProfile::getCurrentBackground() + "_menu.jpg");

    backdrop->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    this->addChild(backdrop, 0);
}

void PuzzleComplete::initMenu()
{
    MenuItemSprite *menuNew = new MenuItemSprite();
    menuNew->initWithNormalSprite(
        Sprite::create("utility/new_up.png"),
        Sprite::create("utility/new_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleComplete::gotoPuzzleSelection, this));

    MenuItemSprite *menuMain = new MenuItemSprite();
    menuMain->initWithNormalSprite(
        Sprite::create("utility/main_up.png"),
        Sprite::create("utility/main_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleComplete::gotoMainMenu, this));

    Menu *menu = Menu::create(menuNew, menuMain, nullptr);
    menu->setPosition(Vec2(visibleSize.width - 210, visibleSize.height / 2));

    menuNew->setScale(0.66f);
    menuMain->setScale(0.66f);

    menuNew->setPositionY(-215);
    menuMain->setPositionY(-275);

    Sprite *pane = Sprite::create();
    pane->setTextureRect(Rect(0, 0, 300, 768));
    pane->setPosition(Vec2(visibleSize.width - 210, visibleSize.height / 2));
    pane->setColor(Color3B::BLACK);
    pane->setOpacity(175);

    this->addChild(menu, 2);
    this->addChild(pane, 1);
}

void PuzzleComplete::initPreviewImage()
{
    Sprite *spr = Sprite::create(GameSettings::getImageName());
    spr->setAnchorPoint(Vec2(0, 1));
    spr->setPosition(Vec2(100, visibleSize.height - 80));
    spr->setScaleX(800 / spr->getContentSize().width);
    spr->setScaleY(600 / spr->getContentSize().height);

    this->addChild(spr, 2);
}

void PuzzleComplete::rewardPlayer()
{
    GameProfile::modifyProfileStat(ProfileStat::puzzlesCompleted, 1);
    GameProfile::modifyProfileStat
        (ProfileStat::stars, GameSettings::getCurrentPuzzleValue());
    GameProfile::modifyProfileStat
        (ProfileStat::totalStarsCollected, GameSettings::getCurrentPuzzleValue());

    // ------- code possibly for somewhere else?
    Sprite *spr = Sprite::create();
    spr->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height));
    spr->setColor(Color3B::WHITE);
    spr->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    spr->setOpacity(100);
    FadeOut *fade = FadeOut::create(1);
    spr->runAction(fade);
    this->addChild(spr, 100);
}

void PuzzleComplete::gotoMainMenu(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}

void PuzzleComplete::gotoPuzzleSelection(cocos2d::Ref* sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, PuzzleSelection::createScene()));
}