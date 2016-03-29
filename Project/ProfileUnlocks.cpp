#include "ProfileUnlocks.h"
#include "GameSettings.h"
#include "GameProfile.h"
#include "MainMenu.h"

USING_NS_CC;

std::vector<GameUnlock> &ProfileUnlocks::unlocksRef = GameProfile::getUnlocks();

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

    fadedOpacity = 150;
    selectedOpacity = 255;
    currentSelection = 0;

    Sprite *sceneTitle = Sprite::create("utility/unlocks.png");
    sceneTitle->setPosition
        (Vec2((visibleSize.width / 2), visibleSize.height - 100));

    this->addChild(sceneTitle, 2);

    initBackdrop();
    initMenu();
    initPreviewImages();

    return true;
}

void ProfileUnlocks::initBackdrop()
{
    backdrop = Sprite::create();

    updateBackdrop();

    this->addChild(backdrop, 0);
}

void ProfileUnlocks::initMenu()
{
    actionButton = new MenuItemSprite();
    updateActionButton();

    MenuItemSprite *menuMain = new MenuItemSprite();
    menuMain->initWithNormalSprite(
        Sprite::create("utility/main_up.png"),
        Sprite::create("utility/main_dn.png"),
        nullptr,
        CC_CALLBACK_1(ProfileUnlocks::gotoMainMenu, this));

    cocos2d::Menu *menu = Menu::create(actionButton, menuMain, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    menuMain->setScale(0.66f);
    menuMain->setPosition(Vec2(0, -300));

    actionButton->setScale(0.66f);
    actionButton->setPosition(Vec2(0, -240));

    this->addChild(menu, 2);
}

void ProfileUnlocks::initPreviewImages()
{
    std::string imgSuffix;
    for (int i = 0; i < 4; ++i)
    {
        Sprite *spr = nullptr;

        if (unlocksRef[i].isLocked())
        {
            imgSuffix = "_locked.jpg";
        }
        else
        {
            imgSuffix = "_puzzle.jpg";
        }

        spr = Sprite::create("backdrops/" + unlocksRef[i].getName() + imgSuffix);

        spr->setPosition(Vec2(250 + (i * 285) + (i * 2), (visibleSize.height / 2) + 100));
        spr->setScaleX(273 / spr->getContentSize().width);
        spr->setScaleY(154 / spr->getContentSize().height);
        spr->setOpacity(fadedOpacity);
        spr->setTag(i);

        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(ProfileUnlocks::imageClick, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, spr);

        previewImages.push_back(spr);

        this->addChild(spr, 2);
    }

    previewImages[GameProfile::stringToBackgroundID
        (GameProfile::getCurrentBackground())]->setOpacity(selectedOpacity);

    Sprite *pane = Sprite::create("utility/pane.png");
    pane->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height / 2) + 50));
    pane->setScaleY(2.0f);
    pane->setRotation(90.0f);
    
    this->addChild(pane, 1);
}

bool ProfileUnlocks::imageClick(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Sprite *spr = static_cast<Sprite*>(event->getCurrentTarget());

    Point pt = touch->getLocation();
    Rect recTemp = spr->getBoundingBox();

    if (!recTemp.containsPoint(pt))
    {
        return false;
    }

    for (auto i : previewImages)
    {
        i->setOpacity(fadedOpacity);
    }

    spr->setOpacity(selectedOpacity);
    currentSelection = spr->getTag();

    updateActionButton();

    return true;
}

void ProfileUnlocks::updateActionButton()
{
    if (unlocksRef[currentSelection].isLocked())
    {
        if (stoi(GameProfile::getProfileStat
            (ProfileStat::stars)) >= unlocksRef[currentSelection].getStarCost())
        {
            actionButton->initWithNormalSprite(
                Sprite::create("utility/unlock_up.png"),
                Sprite::create("utility/unlock_dn.png"),
                nullptr,
                CC_CALLBACK_1(ProfileUnlocks::performContextAction, this));
            action = ContextAction::unlock;
        }
        else
        {
            actionButton->initWithNormalSprite(
                Sprite::create("utility/locked_up.png"),
                Sprite::create("utility/locked_dn.png"),
                nullptr,
                CC_CALLBACK_1(ProfileUnlocks::performContextAction, this));
            action = ContextAction::null;
        }

        return;
    }

    actionButton->initWithNormalSprite(
        Sprite::create("utility/select_up.png"),
        Sprite::create("utility/select_dn.png"),
        nullptr,
        CC_CALLBACK_1(ProfileUnlocks::performContextAction, this));
    action = ContextAction::select;
}

void ProfileUnlocks::performContextAction(cocos2d::Ref *sender)
{
    switch (action)
    {
        case ContextAction::unlock:
        {
            GameProfile::modifyProfileStat
                (ProfileStat::stars, -unlocksRef[currentSelection].getStarCost());
            GameProfile::modifyProfileStat
                (ProfileStat::backgroundsUnlocked, unlocksRef[currentSelection].getName());

            previewImages[currentSelection]->initWithFile
                ("backdrops/" + unlocksRef[currentSelection].getName() + "_menu.jpg");

            unlocksRef[currentSelection].setLocked(false);

            break;
        }
        case ContextAction::select:
        {
            GameProfile::setProfileStat
                (ProfileStat::currentBackground, unlocksRef[currentSelection].getName());

            updateBackdrop();

            break;
        }
        default: {}
    }

    updateActionButton();
}

void ProfileUnlocks::updateBackdrop()
{
    backdrop->initWithFile
        ("backdrops/" + GameProfile::getCurrentBackground() + "_menu.jpg");
    backdrop->setAnchorPoint(Vec2(0, 0));
    backdrop->setPosition(Vec2(0, 0));
}

void ProfileUnlocks::gotoMainMenu(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}